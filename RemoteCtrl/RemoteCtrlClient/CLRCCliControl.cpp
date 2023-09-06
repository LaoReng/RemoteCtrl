#include "pch.h"
#include "CLRCCliControl.h"

#define BUFSIZE 1024

CLRCCliControl::Helper CLRCCliControl::m_helper;
CLRCCliControl* CLRCCliControl::m_pControl = NULL;

CLRCCliControl::CLRCCliControl()
	: m_sock(0)
{
	m_sock.Init();
	m_buffer.resize(BUFSIZE);
}

CLRCCliControl::~CLRCCliControl()
{
	m_sock.Close();
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
	m_pack = CLPackage(cmd, data);
}

int CLRCCliControl::Send()
{
	m_sock.Joint(7968, "127.0.0.1");
	return m_sock.Send(m_pack.Str());
}

int CLRCCliControl::Recv(BOOL isAutoClose)
{
	if (m_sock.GetJointSock() == INVALID_SOCKET)
		return -1;
	m_sock.Recv(m_buffer);
	m_pack = CLPackage((char*)m_buffer.c_str(), m_buffer.size());
	if (isAutoClose)
		m_sock.CloseJointSock();
	return 0;
}
