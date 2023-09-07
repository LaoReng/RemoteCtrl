#include "pch.h"
#include "CLPackage.h"

CLPackage::CLPackage()
	: m_PHead(0xFEFF)
	, m_PCmd(1)
	, m_PData(NULL)
	, m_PDataSize(0)
	, m_PackIsChange(1)
{
	SetPLen();
	SetPAdd();
}

CLPackage::CLPackage(unsigned short cmd, const char* data, size_t dataSize)
	: m_PHead(0xFEFF)
	, m_PCmd(cmd)
	, m_PData(NULL)
	, m_PDataSize(dataSize)
	, m_PackIsChange(1)
{
	if (data) {
		if (!dataSize)
			m_PDataSize = strlen(data);
		m_PData = std::make_shared<char*>(new char[m_PDataSize + 1]{ 0 });
		memcpy(*m_PData, data, m_PDataSize);
	}
	SetPLen();
	SetPAdd();
}

CLPackage::CLPackage(char* buffer, size_t size)
	: m_PHead(0), m_PCmd(0)
	, m_PLength(0), m_PAdd(0)
	, m_PData(NULL), m_PDataSize(0)
	, m_PackIsChange(1)
{
	size_t index = 0;
	for (int i = 0; i < size - 1; i++) {
		if (*(unsigned short*)(buffer + i) == 0xFEFF) {
			index = i;
			break;
		}
	}
	if (index + 10 > size) { // 后面的固定字节，要在buffer中
		memmove(buffer, buffer + index, size - index);
		memset(buffer + (size - index), 0, index);
		//TODO:把index前面的数据删除，没有用了
		return;
	}
	m_PHead = 0xFEFF; index += 2;
	m_PCmd = *(unsigned short*)(buffer + index); index += 2;
	m_PLength = *(unsigned short*)(buffer + index); index += 2;
	if (index + m_PLength > size) {
		//TODO:把前面的数据删掉
		memmove(buffer, buffer + index - 6, size - index + 6);
		memset(buffer + (size - index + 6), 0, index - 6);
		return;
	}
	m_PDataSize = m_PLength - 4;
	m_PAdd = *(unsigned*)(buffer + index); index += 4;
	unsigned sum = 0;
	for (size_t i = index; i < index + m_PDataSize; i++) {
		sum += buffer[i];
	}
	index += m_PDataSize;
	if (sum != m_PAdd) {
		//TODO:把这这个数据清理掉，有错误
		memmove(buffer, buffer + index, size - index);
		memset(buffer + (size - index), 0, index);
		return;
	}
}

CLPackage::CLPackage(const CLPackage& clp)
{
	m_PHead = clp.m_PHead;
	m_PCmd = clp.m_PCmd;
	m_PLength = clp.m_PLength;
	m_PAdd = clp.m_PAdd;
	m_PData = clp.m_PData;
	m_PDataSize = clp.m_PDataSize;
	m_strPackage = clp.m_strPackage;
	m_PackIsChange = clp.m_PackIsChange;
}

CLPackage& CLPackage::operator=(const CLPackage& clp)
{
	if (this != &clp) {
		m_PHead = clp.m_PHead;
		m_PCmd = clp.m_PCmd;
		m_PLength = clp.m_PLength;
		m_PAdd = clp.m_PAdd;
		m_PData = clp.m_PData;
		m_PDataSize = clp.m_PDataSize;
		m_strPackage = clp.m_strPackage;
		m_PackIsChange = clp.m_PackIsChange;
	}
	return *this;
}

CLPackage::~CLPackage()
{
	if (m_PData.use_count() == 1) { // 当前析构为m_PData最后一个实例
		char* pStr = *m_PData;
		*m_PData = NULL;
		m_PData.reset();
		delete pStr;
	}
}

void CLPackage::SetCmd(unsigned short cmd)
{
	m_PCmd = cmd;
	m_PackIsChange = 1;
}

void CLPackage::SetData(const char* data)
{
	if (m_PData.use_count() == 1) { // 如果本类是他最后一个实例，则释放资源
		delete* m_PData;
	}
	else { // 否则将指针计数减一
		m_PData.reset();
	}
	m_PDataSize = 0;
	if (data) {
		m_PDataSize = strlen(data) + 1;
		m_PData = std::make_shared<char*>(new char[m_PDataSize] { 0 });
		memcpy(*m_PData, data, m_PDataSize - 1);
	}
	SetPLen();
	SetPAdd();
	m_PackIsChange = 1;
}

unsigned short CLPackage::GetCmd() const
{
	return m_PCmd;
}

const char* CLPackage::GetData() const
{
	return *m_PData;
}

size_t CLPackage::GetDataSize() const
{
	return m_PDataSize;
}

#define BITSIZE(x) (sizeof(x) * 8)


const char* CLPackage::Str()
{
	if (m_PackIsChange) {
		m_strPackage.clear();
		/*m_strPackage += Value2BinStr(m_PHead, sizeof(m_PHead));
		m_strPackage += Value2BinStr(m_PCmd, sizeof(m_PCmd));
		m_strPackage += Value2BinStr(m_PLength, sizeof(m_PLength));
		m_strPackage += Value2BinStr(m_PAdd, sizeof(m_PAdd));*/
		//第二种实现方法
		size_t index = 0;
		size_t bitAllSize = BITSIZE(m_PHead) + BITSIZE(m_PCmd) + BITSIZE(m_PLength) + BITSIZE(m_PAdd);
		m_strPackage.resize(bitAllSize);
		char* strPtr = (char*)m_strPackage.c_str();
		Value2MemValue((PBYTE)(strPtr + index), m_PHead, sizeof(m_PHead)); index += sizeof(m_PHead);
		Value2MemValue((PBYTE)(strPtr + index), m_PCmd, sizeof(m_PCmd)); index += sizeof(m_PCmd);
		Value2MemValue((PBYTE)(strPtr + index), m_PLength, sizeof(m_PLength)); index += sizeof(m_PLength);
		Value2MemValue((PBYTE)(strPtr + index), m_PAdd, sizeof(m_PAdd)); index += sizeof(m_PAdd);
		unsigned short phead = *(unsigned short*)strPtr;
		CLTools::ErrorOut(m_strPackage.c_str(), __FILE__, __LINE__);// << std::endl;
		if (m_PDataSize)
			memcpy(strPtr + index, *m_PData, m_PDataSize);
		m_PackIsChange = FALSE;
	}
	return m_strPackage.c_str();
}

size_t CLPackage::GetSize() const
{
	return sizeof(m_PHead) + sizeof(m_PCmd) + sizeof(m_PLength) + m_PLength;
}

void CLPackage::SetPLen()
{
	m_PLength = 4 + (unsigned short)m_PDataSize;
	m_PackIsChange = 1;
}

void CLPackage::SetPAdd()
{
	unsigned int sum = 0;
	for (size_t i = 0; i < m_PDataSize; i++) { // 计算数据校验和
		sum += (*m_PData)[i];
	}
	m_PAdd = sum;
	m_PackIsChange = 1;
}

const char* CLPackage::Value2BinStr(unsigned int value, unsigned char ByteSize)
{
	const int BitSize = ByteSize * 8;

	char buf[125] = "";
	_ultoa(value, buf, 2);
	size_t size = strlen(buf), index = 0;
	for (int i = 0; i < size; i++)
		buf[i] -= '0';
	if (size != BitSize) {
		memset(buf + size, 0, BitSize - size);
		memcpy(buf + BitSize, buf, size);
		index = size;
	}
	return buf + index;
}

void CLPackage::Value2MemValue(PBYTE mem, unsigned int value, size_t ValueSize)
{
	if (mem == NULL)
		return;
	for (int i = 0; i < ValueSize; i++) {
		mem[i] = ((unsigned char*)&value)[i];
	}
}

fileInfo::fileInfo(const char* filename, BOOL isdir, BOOL ishidden, BOOL islast)
{
	memcpy(m_fileName, filename, strlen(filename));
	m_isDir = isdir;
	m_isHidden = ishidden;
	m_isLast = islast;
}

const char* fileInfo::operator&()
{
	m_strFileInfo += m_fileName;
	m_strFileInfo += m_isDir;
	m_strFileInfo += m_isHidden;
	m_strFileInfo += m_isLast;
	return m_strFileInfo.c_str();
}
