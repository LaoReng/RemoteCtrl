#pragma once
#include "CLNetworkSocket.h"
#include "CLPackage.h"

const int MAX_BUFFER = 1024;

enum {
	COM_NULL,          // û�������̽��
	COM_TESTCONNECT,   // ��������
	COM_USERLOGIN,     // �û���¼
	COM_GETDRIVE,      // ��ȡ��
	COM_GETFILE,       // ��ȡ�ļ�
	COM_FILEDOWNLOAD,  // �ļ�����
	COM_FILEUPLOAD,	   // �ļ��ϴ�
	COM_REMOTEDESKTOP, // Զ������
	COM_SYSTEMLOCK,    // ϵͳ����
	COM_SYSTEMUNLOCK   // ϵͳ����
};

class CLComDispose
{
public:
	CLComDispose();
	~CLComDispose();
	// �ȴ�����
	void Accept();
	// �����
	void ComDis();
	// ��������
	int Recv();
	// ��������
	int Send();
	void testConnect();
	void userLogin();
	void getDrive();
	void getFile();
	void fileDownload();
	void fileUpload();
	void remoteDesktop();
	void systemLock();
	void systemUnlock();
private:
	CLNetworkSocket<CTCP> m_sock;
	CLPackage             m_pack;
	std::string m_buffer;
};

