#pragma once
#include <vector>
#include <WinSock2.h>

#define WSAV1 2
#define WSAV2 2

//套接字地址类
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
	//获取sockaddr_in大小
	size_t Size() {
		return sizeof(m_addr);
	}
private:
	sockaddr_in m_addr; // 保存addr信息
};

class CTCP
{
protected:
	// 参数isServer: 1 将构造TCP服务器，0 将构造TCP客户端
	CTCP(int isServer = 1);
	~CTCP();
	CTCP(const CTCP&) = delete;
	CTCP& operator=(const CTCP&) = delete;
public:
	// 初始化，若为服务端则会进行地址的绑定
	int Init(const std::string& ip = "", const short port = 0);
	// 等待连接/连接服务器
	SOCKET Joint(int ltNum = 0, const std::string& ip = "", const short port = 0);
	// 接收数据
	int Recv(std::string& buffer);
	// 发送数据
	int Send(const std::string& buffer);
	// 关闭套接字
	void Close();
	// 关闭当前连接的套接字
	void CloseJointSock();
private:
	SOCKET    m_sock;      // 保存创建的套接字
	SOCKET    m_jointSock; // 保存连接上来的套接字
	CSockAddr m_serAddr;   // 保存服务器地址
	CSockAddr m_cliAddr;   // 保存客户端地址
protected:
	int       m_isServer;  // 是否为服务器
};

class CUDP
{
protected:
	// 参数isServer: 1 将构造UDP服务器，0 将构造UDP客户端
	CUDP(int isServer = 1);
	~CUDP();
	CUDP(const CUDP&) = delete;
	CUDP& operator=(const CUDP&) = delete;
public:
	// 初始化，若为服务端则会进行地址的绑定
	int Init(const std::string& ip = "", const short port = -1);
	// 接收数据
	int Recv(std::string& buffer);
	// 发送数据
	int Send(const std::string& buffer, const std::string& ip = "", const short& port = -1);
	// 关闭套接字
	void Close();
private:
	SOCKET    m_sock;     // 保存创建的套接字
	CSockAddr m_serAddr;  // 保存服务器地址
	CSockAddr m_cliAddr;  // 保存客户端地址
protected:
	int       m_isServer; // 是否为服务器
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

