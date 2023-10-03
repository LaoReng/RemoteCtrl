#pragma once
#include "CLPackage.h"
#include "CLNetworkSocket.h"
#include <vector>
// 控制层
class CLRCCliControl
{
public:
	// 发送数据包
	int Send();
	// 接收数据
	// isAutoClose：是否自动关闭与服务器的连接 TRUE 自动关闭连接 FALSE 需要用户调用Close()关闭与服务器的连接，index：指向接收缓冲区开始接收的位置，首次调用该值为0，下次调用该值由Recv()的返回值决定
	// 返回值：函数执行失败，则返回值小于0；函数执行成功，则返回现在接收缓冲区数据尾部位置
	int Recv(BOOL isAutoClose = TRUE, size_t index = 0);
	// 获取类唯一对象指针
	static CLRCCliControl* getInstance();
	// 获取数据包
	CLPackage& GetPackage();
	// 设置数据包中命令和数据
	// cmd：设置数据包命令 data：设置数据包数据，默认为NULL
	void SetPackage(unsigned short cmd, const char* data = NULL);
	// 设置服务器的IP地址
	void SetSerIp(const char* ip);
	// 设置服务器的端口
	void SetSerPort(const short port);
	// 获取服务器的IP地址
	const char* GetSerIp();
	// 获取服务器的端口
	short GetSerPort();
	// 关闭客户端套接字
	void Close();
private:
	CLRCCliControl();
	~CLRCCliControl();
	CLRCCliControl(const CLRCCliControl&) = delete;
	CLRCCliControl& operator=(const CLRCCliControl&) = delete;
	// 销毁类唯一对象指针
	static void releaseInstance();
private:
	CLNetworkSocket<CTCP> m_sock;
	CString m_SerIp;
	short m_SerPort;
	CLPackage m_pack;
	std::shared_ptr<char*> m_buffer;
	class Helper {
	public:
		Helper() {}
		~Helper() {
			releaseInstance();
		}
	};
	static Helper m_helper;
	static CLRCCliControl* m_pControl;
};