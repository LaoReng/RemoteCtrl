#pragma once
#include <vector>
#include <WinSock2.h>

#define WSAV1 2 // WSA�汾ƴ��1
#define WSAV2 2 // WSA�汾ƴ��2

//�׽��ֵ�ַ��
class CSockAddr
{
public:
	//CSockAddr() = default;
	~CSockAddr() = default;
	CSockAddr(const char* ip = NULL, const short port = 0) {
		if (ip == NULL) {
			memset(&m_addr, 0, sizeof(m_addr));
			return;
		}
		m_addr.sin_family = AF_INET;
		m_addr.sin_addr.s_addr = inet_addr(ip);
		m_addr.sin_port = htons(port);
	}
	CSockAddr(const CSockAddr& addr) {
		m_addr = addr.m_addr;
	}
	CSockAddr& operator=(const CSockAddr& addr) {
		if (&addr != this) {
			m_addr = addr.m_addr;
		}
		return *this;
	}
	sockaddr_in* operator&() {
		return &m_addr;
	}
	void SetAddr(const char* ip, const short port) {
		m_addr.sin_family = AF_INET;
		m_addr.sin_addr.s_addr = inet_addr(ip);
		m_addr.sin_port = htons(port);
	}
	//��ȡsockaddr_in��С
	size_t Size() {
		return sizeof(m_addr);
	}
private:
	sockaddr_in m_addr; // ����addr��Ϣ
};

class CTCP
{
protected:
	// ����isServer: 1 ������TCP��������0 ������TCP�ͻ���
	CTCP(int isServer = 1);
	~CTCP();
	CTCP(const CTCP&) = delete;
	CTCP& operator=(const CTCP&) = delete;
public:
	// ��ʼ������Ϊ����������е�ַ�İ�
	// ip�����ڷ������󶨵�ַ��port�����ڷ������󶨶˿ڣ�value��listen���Ӷ�����󳤶�(���ڷ�����)
	int Init(const char* ip = NULL, const short port = 0, int value = 0);
	// �ȴ�����/���ӷ�����
	// port: ���ӷ������Ķ˿ں�(���ڿͻ���)��ip: ���ӷ������ĵ�ַ(���ڿͻ���) 
	SOCKET Joint(int port = 0, const char* ip = NULL);
	// ��������
	int Recv(PBYTE buffer, size_t BufSize, size_t index = 0);
	// ��������
	int Send(const PBYTE& buffer, size_t BufSize);
	SOCKET GetJointSock() const;
	// �ر��׽���
	void Close();
	// �رյ�ǰ���ӵ��׽���
	void CloseJointSock();
private:
	SOCKET    m_sock;      // ���洴�����׽���
	SOCKET    m_jointSock; // ���������������׽���
	CSockAddr m_serAddr;   // �����������ַ
	CSockAddr m_cliAddr;   // ����ͻ��˵�ַ
protected:
	int       m_isServer;  // �Ƿ�Ϊ������
};

class CUDP
{
protected:
	// ����isServer: 1 ������UDP��������0 ������UDP�ͻ���
	CUDP(int isServer = 1);
	~CUDP();
	CUDP(const CUDP&) = delete;
	CUDP& operator=(const CUDP&) = delete;
public:
	// ��ʼ������Ϊ����������е�ַ�İ�
	int Init(const char* ip = NULL, const short port = -1);
	// ��������
	int Recv(std::string& buffer);
	// ��������
	int Send(const std::string& buffer, const char* ip = NULL, const short& port = -1);
	// �ر��׽���
	void Close();
private:
	SOCKET    m_sock;     // ���洴�����׽���
	CSockAddr m_serAddr;  // �����������ַ
	CSockAddr m_cliAddr;  // ����ͻ��˵�ַ
protected:
	int       m_isServer; // �Ƿ�Ϊ������
};


template<class T>
class CLNetworkSocket
	:public T
{
public:
	CLNetworkSocket(int isServer) {
		T::m_isServer = isServer;
	}
};

