#include "pch.h"
#include "CLNetworkSocket.h"


CTCP::CTCP(int isServer)
	: m_sock(INVALID_SOCKET)
	, m_jointSock(0)
	, m_isServer(isServer)
{
	m_serAddr = CSockAddr("", -1);
	m_cliAddr = CSockAddr("", -1);
	WSADATA data = {};
	if (WSAStartup(MAKEWORD(2, 2), &data)) {
		CString str;
		str.Format(_T("%s(%d):套接字库加载失败！\r\n"), __FILE__, __LINE__);
		OutputDebugString(str);
		return;
	}
}
CTCP::~CTCP()
{
	Close();
	WSACleanup();
}

int CTCP::Init(const std::string& ip , const short port )
{
	//创建套接字
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) {
		CString str;
		str.Format(_T("%s(%d):套接字初始化失败！\r\n"), __FILE__, __LINE__);
		OutputDebugString(str);
		return -1;
	}
	if (m_isServer) {
		m_serAddr = CSockAddr(ip, port);
		//TCP服务器端口绑定操作
		if (bind(m_sock, (sockaddr*)&m_serAddr, m_serAddr.Size())) {
			CString str;
			str.Format(_T("%s(%d):绑定失败！\r\n"), __FILE__, __LINE__);
			OutputDebugString(str);
			return -2;
		}
	}
	return 0;
}

int CTCP::Joint(int ltNum , const std::string& ip , const short port )
{
	if (m_isServer) {
		listen(m_sock, ltNum);
		int len = m_cliAddr.Size();
		m_jointSock = accept(m_sock, (sockaddr*)&m_cliAddr, &len);
		if (m_jointSock == INVALID_SOCKET) {
			CString str;
			str.Format(_T("%s(%d):服务端等待连接错误！\r\n"), __FILE__, __LINE__);
			OutputDebugString(str);
			return -3;
		}
	}
	else {
		m_serAddr = CSockAddr(ip, port);
		if (connect(m_sock, (sockaddr*)&m_serAddr, m_serAddr.Size())) {
			CString str;
			str.Format(_T("%s(%d):客户端连接服务器失败！\r\n"), __FILE__, __LINE__);
			OutputDebugString(str);
			return -4;
		}
		m_jointSock = m_sock;
	}
	return m_jointSock;
}

int CTCP::Recv(std::string& buffer)
{
	int buflen = buffer.size();
	int ret = recv(m_jointSock, (char*)buffer.c_str(), buflen, 0);
	if (ret < 0) {
		CString str;
		str.Format(_T("%s(%d):数据接收失败！\r\n"), __FILE__, __LINE__);
		OutputDebugString(str);
	}
	return ret;
}

int CTCP::Send(const std::string& buffer)
{
	int ret = 0;
	int index = 0;
	int buflen = buffer.size();
	while (buflen > 0) {
		ret = send(m_jointSock, buffer.c_str() + index, buflen, 0);
		if (ret < 0) {
			CString str;
			str.Format(_T("%s(%d):数据发送失败！\r\n"), __FILE__, __LINE__);
			OutputDebugString(str);
			break;
		}
		index += ret;
		buflen -= ret;
	}
	return 0;
}

void CTCP::Close()
{
	closesocket(m_sock);
}
