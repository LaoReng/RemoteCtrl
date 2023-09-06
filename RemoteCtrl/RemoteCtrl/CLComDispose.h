#pragma once
#include "CLNetworkSocket.h"
#include "CLPackage.h"
#include "CLSysLockDialog.h"

const int MAX_BUFFER = 1024;



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
	// ����������Ӧ����
	void testConnect();
	// �û���¼��Ӧ����
	void userLogin();
	// ��ȡ����Ӧ����
	void getDrive();
	// ��ȡ�ļ���Ӧ����
	void getFile();
	// �ļ�������Ӧ����
	void fileDownload();
	// �ļ��ϴ���Ӧ����
	void fileUpload();
	// Զ��������Ӧ����
	void remoteDesktop();
	// ϵͳ������Ӧ����
	void systemLock();
	// ϵͳ������Ӧ����
	void systemUnlock();
private:
	// Volume��ͷ��ʽ�ж�
	BOOL VolumeFormat(const char* volumename, const char* format);
	// �����߳̿�������
	static unsigned __cdecl sysLockThread(void* args);
	// �����߳�������
	void sysLockThreadMain();
private:
	CLNetworkSocket<CTCP> m_sock;         // �����׽���
	CLPackage             m_pack;         // ������ջ��͵İ�����
	std::string           m_buffer;       // ���տͻ�����Ϣ�Ļ�����
	CLSysLockDialog       m_lockDlg;      // ϵͳ�������ڶ���
	UINT                  m_lockThreadID; // ϵͳ�����߳�ID
	HANDLE                m_hLockThread;  // ϵͳ�����߳̾��
};

