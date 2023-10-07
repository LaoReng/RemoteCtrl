#pragma once
#include <memory>
#include <vector>

enum {
	COM_NULL,          // 没有命令，嗅探包
	COM_INVALID,       // 无效的包命令
	COM_TESTCONNECT,   // 测试连接
	COM_USERLOGIN,     // 用户登录
	COM_GETDRIVE,      // 获取卷
	COM_GETFILE,       // 获取文件和文件夹
	COM_GETFILES,      // 获取文件不获取文件夹
	COM_FILEDOWNLOAD,  // 文件下载
	COM_FILEDELETE,    // 文件删除
	COM_FILEUPLOAD,	   // 文件上传
	COM_REMOTEDESKTOP, // 远程桌面
	COM_MOUSEEVENT,    // 鼠标消息
	COM_KEYEVENT,      // 键盘消息
	COM_SYSTEMLOCK,    // 系统锁定
	COM_SYSTEMUNLOCK   // 系统解锁
};

#pragma pack(push,1) // 设置内存对齐方式，将对齐系数进栈

#define FILESIZE 80
// 文件信息类
typedef struct fileInfo
{
	char*       m_fileName;           // 文件名
	BOOL        m_isDir;              // 是否为目录
	BOOL        m_isHidden;           // 是否是隐藏状态
	BOOL        m_isLast;             // 是否是最后一个
	//BYTE m_strFileInfo[MAX_PATH + sizeof(BOOL) * 3] = "";
	fileInfo() :m_isDir(FALSE), m_isHidden(FALSE), m_isLast(TRUE) {
		m_fileName = new char[FILESIZE] {};
	}
	fileInfo(const char* filename, BOOL isdir, BOOL ishidden, BOOL islast);
	void operator=(const char* str) {
		if (str == NULL)return;
		memcpy(m_fileName, str, FILESIZE);
		int index = FILESIZE;
		m_isDir = *(BOOL*)(str + index); index += sizeof(BOOL);
		m_isHidden = *(BOOL*)(str + index); index += sizeof(BOOL);
		m_isLast = *(BOOL*)(str + index); index += sizeof(BOOL);
	}
	~fileInfo() {
		if (m_fileName) {
			char* temp = m_fileName;
			m_fileName = NULL;
			delete[] temp;
		}
	}
	void setAll(const char* filename, BOOL isdir, BOOL ishidden, BOOL islast);
	void MemStream(PBYTE mem);
	static constexpr size_t getSize() {
		return FILESIZE + sizeof(BOOL) * 3;
	}
}FILEINFO, * PFILEINFO;

// 数据包类
class CLPackage
{
public:
	CLPackage();
	// 封装
	CLPackage(unsigned short cmd, const char* data = NULL, size_t dataSize = 0);
	// 解封装，并将size修改为处理完缓冲区当前数据的最后一个字节
	CLPackage(char* buffer, int& size);
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
	// 数据包内存流化
	const char* MemStream();
	// 获取包的大小
	size_t GetSize() const;
	// 清除数据包中的数据
	void DataClear();
private:
	// 设置数据包长度，内部使用
	void SetPLen();
	// 设置数据校验和，内部使用
	void SetPAdd();
	// 将数值转换为二进制，指定位数不够补0，内部使用
	// value：数值，ByteSize：二进制所占字节数
	const char* Value2BinStr(unsigned int value, unsigned char ByteSize);
	// 将数值转换为内存值
	// mem：内存缓冲区，value：要转换的值，ValueSize：值所占的字节数
	void Value2MemValue(PBYTE mem, unsigned int value, size_t ValueSize);
protected:
	unsigned short         m_PHead;        // 数据包头，固定0xFEFF
	unsigned short         m_PCmd;         // 数据包命令
	unsigned short         m_PLength;      // 数据包长度，从数据校验和到数据包尾部 
	unsigned int           m_PAdd;         // 数据校验和，就是数组部分的和，默认是0如果没有数组就不需要计算
	std::shared_ptr<char*> m_PData;        // 数据
private:
	size_t                 m_PDataSize;    // 数据长度
	PBYTE                  m_strPackage;   // 字符化包数据
	BOOL                   m_PackIsChange; // 包是否发生变化，TRUE 发生变化 FALSE 未发生变化
};
#pragma pack(pop) // 将自定义对齐系数出栈，恢复默认对齐方式
