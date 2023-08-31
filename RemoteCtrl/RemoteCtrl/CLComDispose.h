#pragma once
#include "CLNetworkSocket.h"
#include "CLPackage.h"

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

