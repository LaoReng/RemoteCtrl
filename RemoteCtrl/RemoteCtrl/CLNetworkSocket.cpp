#include "pch.h"
#include "CLNetworkSocket.h"

CTCP::CTCP(int isServer)
	: m_sock(INVALID_SOCKET)
	, m_jointSock(INVALID_SOCKET)
	, m_serAddr()
	, m_cliAddr()
	, m_isServer(isServer)
{
	WSADATA data = {};
	if (WSAStartup(MAKEWORD(WSAV1, WSAV2), &data)) {
		CLTools::ErrorOut("�׽��ֿ����ʧ�ܣ�", __FILE__, __LINE__);
		return;
	}
}
CTCP::~CTCP()
{
	CloseJointSock();
	Close();
	WSACleanup();
}

int CTCP::Init(const char* ip, const short port, int value)
{
	//�����׽���
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) {
		CLTools::ErrorOut("�׽��ֳ�ʼ��ʧ�ܣ�", __FILE__, __LINE__);
		return -1;
	}
	if (m_isServer) {
		if (ip == NULL) {
			CLTools::ErrorOut("���ṩ�������󶨵�ַ��", __FILE__, __LINE__);
			return -2;
		}
		m_serAddr = CSockAddr(ip, port);
		//TCP�������˿ڰ󶨲���
		if (bind(m_sock, (sockaddr*)&m_serAddr, (int)m_serAddr.Size())) {
			CLTools::ErrorOut("��ʧ�ܣ�", __FILE__, __LINE__);
			return -2;
		}
		listen(m_sock, value);
	}
	return 0;
}

SOCKET CTCP::Joint(int port, const char* ip)
{
	if (m_isServer) {
		int len = (int)m_cliAddr.Size();
		m_jointSock = accept(m_sock, (sockaddr*)&m_cliAddr, &len);
		if (m_jointSock == INVALID_SOCKET) {
			CLTools::ErrorOut("����˵ȴ����Ӵ���", __FILE__, __LINE__);
			return -3;
		}
	}
	else {
		if (m_sock == INVALID_SOCKET)
			Init();
		m_serAddr = CSockAddr(ip, port);
		if (connect(m_sock, (sockaddr*)&m_serAddr, (int)m_serAddr.Size())) {
			CLTools::ErrorOut("�ͻ������ӷ�����ʧ�ܣ�", __FILE__, __LINE__);
			return -4;
		}
		m_jointSock = m_sock;
	}
	return m_jointSock;
}

int CTCP::Recv(PBYTE buffer, size_t BufSize, size_t index)
{
	if (index >= BufSize) {
		CLTools::ErrorOut("���δ���򻺳���������", __FILE__, __LINE__);
		return -1;
	}
	int ret = recv(m_jointSock, (char*)(buffer + index), (int)(BufSize - index), 0);
	if (ret < 0) {
		CLTools::ErrorOut("���ݽ���ʧ�ܣ�", __FILE__, __LINE__);
	}
	return ret;
}

int CTCP::Send(const PBYTE& buffer, size_t BufSize)
{
	int ret = 0;
	int index = 0;
	int buflen = (int)BufSize;
	while (buflen > 0) {
		ret = send(m_jointSock, (char*)(buffer + index), buflen, 0);
		if (ret < 0) {
			CString str;
			str.Format("���ݷ���ʧ��! lasterror:%d", WSAGetLastError());
			CLTools::ErrorOut(str, __FILE__, __LINE__);
			index *= -1;
			break;
		}
		index += ret;
		buflen -= ret;
	}
	return index;
}

SOCKET CTCP::GetJointSock() const
{
	return m_jointSock;
}

void CTCP::Close()
{
	if (m_sock != INVALID_SOCKET) {
		SOCKET sock = m_sock;
		m_sock = INVALID_SOCKET;
		closesocket(sock);
	}
	if (m_jointSock != INVALID_SOCKET) {
		SOCKET sock = m_jointSock;
		m_jointSock = INVALID_SOCKET;
		closesocket(sock);
	}
}

void CTCP::CloseJointSock()
{
	if (m_jointSock != INVALID_SOCKET) {
		SOCKET sock = m_jointSock;
		m_jointSock = INVALID_SOCKET;
		closesocket(sock);
		if (!m_isServer)
			m_sock = INVALID_SOCKET;
	}
}

CUDP::CUDP(int isServer)
	: m_sock(INVALID_SOCKET)
	, m_isServer(isServer)
{
	m_serAddr = CSockAddr("", -1);
	m_cliAddr = CSockAddr("", -1);
	WSADATA data = {};
	if (WSAStartup(MAKEWORD(WSAV1, WSAV2), &data)) {
		CLTools::ErrorOut("�׽��ֿ����ʧ�ܣ�", __FILE__, __LINE__);
		return;
	}
}

CUDP::~CUDP()
{
	Close();
	WSACleanup();
}

int CUDP::Init(const char* ip, const short port)
{
	if ((m_sock = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		CLTools::ErrorOut("�׽��ֳ�ʼ��ʧ�ܣ�", __FILE__, __LINE__);
		return -1;
	}
	if (m_isServer) {
		m_serAddr = CSockAddr(ip, port);
		if (bind(m_sock, (sockaddr*)&m_serAddr, (int)m_serAddr.Size())) {
			CLTools::ErrorOut("�׽��ְ�ʧ��!", __FILE__, __LINE__);
			return -2;
		}
	}
	return 0;
}

int CUDP::Recv(std::string& buffer)
{
	int addrLen = (int)m_cliAddr.Size();
	int ret = recvfrom(m_sock, (char*)buffer.c_str(), (int)buffer.size(), 0, (sockaddr*)&m_cliAddr, &addrLen);
	if (ret < 0) {
		CLTools::ErrorOut("���ݽ��մ���", __FILE__, __LINE__);
	}
	return ret;
}

int CUDP::Send(const std::string& buffer, const char* ip, const short& port)
{
	if (port != -1) {
		m_cliAddr = CSockAddr(ip, port);
	}
	int index = 0;
	int ret = 0;
	int  buflen = (int)buffer.size();
	while (buflen > 0) {
		ret = sendto(m_sock, buffer.c_str() + index, buflen, 0, (sockaddr*)&m_cliAddr, (int)m_cliAddr.Size());
		if (ret < 0) {
			CLTools::ErrorOut("���ݷ��ʹ���", __FILE__, __LINE__);
			break;
		}
		index += ret;
		buflen -= ret;
	}
	return ret;
}

void CUDP::Close()
{
	if (m_sock != INVALID_SOCKET) {
		SOCKET sock = m_sock;
		m_sock = INVALID_SOCKET;
		closesocket(sock);
	}
}
