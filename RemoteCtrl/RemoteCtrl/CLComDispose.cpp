#include "pch.h"
#include "CLComDispose.h"

CLComDispose::CLComDispose()
	:m_sock(1)
{
	m_buffer.resize(MAX_BUFFER);
	m_sock.Init("127.0.0.1", 7968);
}

CLComDispose::~CLComDispose()
{}

void CLComDispose::Accept()
{
	while (true) {
		m_sock.Joint(4);
		Recv();
		ComDis();
		//Send();
		m_sock.CloseJointSock();
	}
}

void CLComDispose::ComDis()
{
	switch (m_pack.GetCmd()) {
	case COM_NULL:
		break;
	case COM_TESTCONNECT:
		testConnect();
		break;
	case COM_USERLOGIN:
		userLogin();
		break;
	case COM_GETDRIVE:
		getDrive();
		break;
	case COM_GETFILE:
		getFile();
		break;
	case COM_FILEDOWNLOAD:
		fileDownload();
		break;
	case COM_FILEUPLOAD:
		fileUpload();
		break;
	case COM_REMOTEDESKTOP:
		remoteDesktop();
		break;
	case COM_SYSTEMLOCK:
		systemLock();
		break;
	case COM_SYSTEMUNLOCK:
		systemUnlock();
		break;
	default:
		break;
	}
}

int CLComDispose::Recv()
{
	int ret = m_sock.Recv(m_buffer);
	if (ret < 0) {
		CLTools::ErrorOut("数据接收错误！", __FILE__, __LINE__);
		return ret;
	}
	m_pack = CLPackage((char*)m_buffer.c_str(), strlen(m_buffer.c_str()));
	return ret;
}

int CLComDispose::Send()
{
	int ret = m_sock.Send(m_pack.Str());
	if (ret < 0) {
		CLTools::ErrorOut("数据发送错误！", __FILE__, __LINE__);
		return ret;
	}
	return ret;
}

void CLComDispose::testConnect()
{
}

void CLComDispose::userLogin()
{
}

void CLComDispose::getDrive()
{
}

void CLComDispose::getFile()
{
}

void CLComDispose::fileDownload()
{
}

void CLComDispose::fileUpload()
{
}

void CLComDispose::remoteDesktop()
{
}

void CLComDispose::systemLock()
{
}

void CLComDispose::systemUnlock()
{
}
