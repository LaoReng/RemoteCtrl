#pragma once
#include <memory>
#include <vector>

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

#pragma pack(push,1) // 设置内存对齐方式，将对齐系数进栈

typedef struct fileInfo
{
	char m_fileName[MAX_PATH] = "";
	BOOL m_isDir = FALSE;
	BOOL m_isHidden = FALSE;
	BOOL m_isLast = TRUE;
	std::string m_strFileInfo;
	fileInfo() :m_fileName(""), m_isDir(FALSE), m_isHidden(FALSE), m_isLast(TRUE) {}
	fileInfo(const char* filename, BOOL isdir, BOOL ishidden, BOOL islast);
	const char* operator&();
}FILEINFO;


class CLPackage
{
public:
	CLPackage();
	// 封装
	CLPackage(unsigned short cmd, const char* data = "",size_t dataSize = 0);
	// 解封装
	CLPackage(char* buffer, size_t size);
	// 复制构造
	CLPackage(const CLPackage& clp);
	// 等于号重载
	CLPackage& operator=(const CLPackage& clp);
	~CLPackage();
	// 设置包命令
	void SetCmd(unsigned short cmd);
	// 设置包中数据部分
	void SetData(const char* data);
	// 获取包命令
	unsigned short GetCmd() const;
	// 获取包中数据部分
	const char* GetData() const;
	// 获取包中数据部分大小
	size_t GetDataSize() const;
	// 字符串化
	const char* Str();
	
private:
	// 设置数据包长度，内部使用
	void SetPLen();
	// 设置数据校验和，内部使用
	void SetPAdd();
	// 将数值转换为二进制，指定位数不够补0，内部使用
	// value：数值，ByteSize：二进制所占字节数
	const char* Value2BinStr(unsigned int value, unsigned char ByteSize);
private:
	unsigned short         m_PHead;        // 数据包头
	unsigned short         m_PCmd;         // 数据包命令
	unsigned short         m_PLength;      // 数据包长度
	unsigned int           m_PAdd;         // 数据校验和
	std::shared_ptr<char*> m_PData;        // 数据
	size_t                 m_PDataSize;    // 数据长度
	std::string            m_strPackage;   // 字符化包数据
	BOOL                   m_PackIsChange; // 包是否发生变化，1 发生变化 0 未发生变化
};
#pragma pack(pop) // 将自定义对齐系数出栈，恢复默认对齐方式
