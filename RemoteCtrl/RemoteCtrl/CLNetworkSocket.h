#pragma once
#include <vector>
#include <WinSock2.h>

#define WSAV1 2
#define WSAV2 2

//�׽��ֵ�ַ��
class CSockAddr
{
public:
	CSockAddr() = default;
	~CSockAddr() = default;
	CSockAddr(const std::string& ip, const short port) {
		m_addr.sin_family = AF_INET;
		m_addr.sin_addr.s_addr = inet_addr(ip.data());
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
	int Init(const std::string& ip = "", const short port = 0);
	// �ȴ�����/���ӷ�����
	SOCKET Joint(int ltNum = 0, const std::string& ip = "", const short port = 0);
	// ��������
	int Recv(std::string& buffer);
	// ��������
	int Send(const std::string& buffer);
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
	int Init(const std::string& ip = "", const short port = -1);
	// ��������
	int Recv(std::string& buffer);
	// ��������
	int Send(const std::string& buffer, const std::string& ip = "", const short& port = -1);
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

