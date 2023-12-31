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
	// 等待连接
	void Accept();
	// 命令处理
	void ComDis();
	// 接收数据
	int Recv();
	// 发送数据
	int Send();
	// 嗅探包响应函数
	void SnifferPacket();
	// 测试连接响应函数
	void testConnect();
	// 用户登录响应函数
	void userLogin();
	// 获取卷响应函数
	void getDrive();
	// 获取文件响应函数
	// isOnlyGetFile：是否只获取文件不获取文件夹 false 不是 true 是
	void getFile(bool isOnlyGetFile = false);
	// 文件下载响应函数
	void fileDownload();
	// 文件删除响应函数
	void fileDelete();
	// 文件上传响应函数
	void fileUpload();
	// 远程桌面响应函数
	void remoteDesktop();
	// 系统锁定响应函数
	void systemLock();
	// 系统解锁响应函数
	void systemUnlock();
private:
	// Volume开头格式判断
	BOOL VolumeFormat(const char* volumename, const char* format);
	// 锁定线程开启函数
	static unsigned __cdecl sysLockThread(void* args);
	// 锁定线程主函数
	void sysLockThreadMain();
private:
	CLNetworkSocket<CTCP> m_sock;         // 网络套接字
	CLPackage             m_pack;         // 保存接收或发送的包数据
	char*                 m_buffer;       // 接收客户端数据的缓冲区
	int                   m_index;        // 接收客户端数据的缓冲区指针，表示缓冲区中未处理的数据
	CLSysLockDialog       m_lockDlg;      // 系统锁定窗口对象
	UINT                  m_lockThreadID; // 系统锁定线程ID
	HANDLE                m_hLockThread;  // 系统锁定线程句柄
};

