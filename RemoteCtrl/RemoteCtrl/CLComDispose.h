#pragma once
#include "CLNetworkSocket.h"
#include "CLPackage.h"
#include "CLSysLockDialog.h"

const int MAX_BUFFER = 1024;

enum {
	COM_NULL,          // 没有命令，嗅探包
	COM_TESTCONNECT,   // 测试连接
	COM_USERLOGIN,     // 用户登录
	COM_GETDRIVE,      // 获取卷
	COM_GETFILE,       // 获取文件
	COM_FILEDOWNLOAD,  // 文件下载
	COM_FILEUPLOAD,	   // 文件上传
	COM_REMOTEDESKTOP, // 远程桌面
	COM_MOUSEEVENT,    // 鼠标消息
	COM_KEYEVENT,      // 键盘消息
	COM_SYSTEMLOCK,    // 系统锁定
	COM_SYSTEMUNLOCK   // 系统解锁
};

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
	// 测试连接响应函数
	void testConnect();
	// 用户登录响应函数
	void userLogin();
	// 获取卷响应函数
	void getDrive();
	// 获取文件响应函数
	void getFile();
	// 文件加载响应函数
	void fileDownload();
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
	CLNetworkSocket<CTCP> m_sock;    // 网络套接字
	CLPackage             m_pack;    // 保存接收或发送的包数据
	std::string           m_buffer;  // 接收客户端消息的缓冲区
	CLSysLockDialog       m_lockDlg; // 系统锁定窗口
public:
	UINT  m_lockThreadID; // 系统锁定线程ID
	HANDLE m_hLockThread; // 系统锁定线程句柄
};

