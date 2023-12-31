#include "pch.h"
#include "CLRCCliControl.h"

#define BUFSIZE 1024

CLRCCliControl::Helper CLRCCliControl::m_helper;
CLRCCliControl* CLRCCliControl::m_pControl = NULL;

CLRCCliControl::CLRCCliControl()
	: m_sock(0)
	, m_pack()
{
	m_sock.Init();
	m_SerIp.Format("%s", "127.0.0.1");
	m_SerPort = 7968;
	m_buffer = std::make_shared<char*>(new char[BUFSIZE] {});
}

CLRCCliControl::~CLRCCliControl()
{
	m_sock.Close();
	if (m_buffer.use_count() == 1) {
		void* temp = *m_buffer;
		*m_buffer = NULL;
		delete temp;
	}
}

CLRCCliControl* CLRCCliControl::getInstance()
{
	if (m_pControl == NULL) {
		m_pControl = new CLRCCliControl();
	}
	return m_pControl;
}

CLPackage& CLRCCliControl::GetPackage()
{
	return m_pack;
}

void CLRCCliControl::releaseInstance()
{
	if (m_pControl) {
		CLRCCliControl* ptr = m_pControl;
		m_pControl = NULL;
		delete ptr;
	}
}

void CLRCCliControl::SetPackage(unsigned short cmd, const char* data)
{
	//m_pack = CLPackage(cmd, data);
	m_pack.DataClear();
	m_pack.SetCmd(cmd);
	if (data)
		m_pack.SetData(data);
}

void CLRCCliControl::SetSerIp(const char* ip)
{
	if (ip == NULL)return;
	if (m_SerIp != ip) {
		m_SerIp.Format("%s", ip);
	}
}

void CLRCCliControl::SetSerPort(const short port)
{
	if (port < 0)return;
	if (m_SerPort != port) {
		m_SerPort = port;
	}
}

const char* CLRCCliControl::GetSerIp()
{
	return m_SerIp.GetString();
}

short CLRCCliControl::GetSerPort()
{
	return m_SerPort;
}

void CLRCCliControl::Close()
{
	m_sock.CloseJointSock();
}

int CLRCCliControl::Send()
{
	if (INVALID_SOCKET == m_sock.Joint(m_SerPort, m_SerIp))
		return -1;
	return m_sock.Send((PBYTE)m_pack.MemStream(), m_pack.GetSize());
}

int CLRCCliControl::Recv(BOOL isAutoClose, size_t index)
{
	if (m_sock.GetJointSock() == INVALID_SOCKET)
		return -1;
	int RecvSize = m_sock.Recv((PBYTE)*m_buffer, BUFSIZE, index);
	RecvSize += (int)index;
	if (RecvSize > 0) {
		m_pack = CLPackage(*m_buffer, RecvSize);
	}
	else {
		RecvSize = -1;
	}
	if (isAutoClose)
		m_sock.CloseJointSock();
	return RecvSize;
}
