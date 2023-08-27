#pragma once
#include <vector>
#include <Windows.h>
#include <WinSock2.h>

class CLNetworkSocket
{
};

class CSockAddr
{
public:
	CSockAddr() = default;
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
	~CSockAddr() = default;
	size_t Size() {
		return sizeof(m_addr);
	}
private:
	sockaddr_in m_addr;
};

class CTCP
{
public:
	CTCP(int isServer = 1);
	~CTCP();
	int Init(const std::string& ip = "", const short port = 0);
	int Joint(int ltNum = 0, const std::string& ip = "", const short port = 0);
	int Recv(std::string& buffer);
	int Send(const std::string& buffer);
	void Close();
private:
	SOCKET m_sock;
	SOCKET m_jointSock;
	CSockAddr m_serAddr;
	CSockAddr m_cliAddr;
	int m_isServer;
};

class CUDP
{
public:
	CUDP();
	~CUDP();
	int Init();
	int Joint();
	int Recv();
	int Send();
	void Close();
private:
	SOCKET m_sock;
};

