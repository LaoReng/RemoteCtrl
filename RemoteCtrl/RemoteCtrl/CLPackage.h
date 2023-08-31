#pragma once
#include <memory>
#include <vector>

#pragma pack(push,1) // 设置内存对齐方式，将对齐系数进栈
class CLPackage
{
public:
	CLPackage();
	// 封装
	CLPackage(unsigned short cmd,const char* data = "");
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
	// 字符串化
	const char* Str();
private:
	// 设置数据包长度，内部使用
	void SetPLen();
	// 设置数据校验和，内部使用
	void SetPAdd();
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
