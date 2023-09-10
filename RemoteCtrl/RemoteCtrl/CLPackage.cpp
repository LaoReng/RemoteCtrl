#include "pch.h"
#include "CLPackage.h"

#define BUFSIZE 1024

CLPackage::CLPackage()
	: m_PHead(0xFEFF)
	, m_PCmd(1)
	, m_PData(NULL)
	, m_PDataSize(0)
	, m_PackIsChange(TRUE)
{
	m_strPackage = new BYTE[BUFSIZE]{ 0 };
	SetPLen();
	SetPAdd();
}

CLPackage::CLPackage(unsigned short cmd, const char* data, size_t dataSize)
	: m_PHead(0xFEFF)
	, m_PCmd(cmd)
	, m_PData(NULL)
	, m_PDataSize(dataSize)
	, m_PackIsChange(TRUE)
{
	if (data) {
		if (!dataSize)
			MessageBox(NULL, "数据长度不应为0！", "错误", MB_OK | MB_ICONERROR);
		m_PDataSize = dataSize;
		m_PData = std::make_shared<char*>(new char[BUFSIZE] { 0 });
		memcpy(*m_PData, data, m_PDataSize);
	}
	m_strPackage = new BYTE[BUFSIZE]{ 0 };
	SetPLen();
	SetPAdd();
}

CLPackage::CLPackage(char* buffer, int& size)
	: m_PHead(0), m_PCmd(0)
	, m_PLength(0), m_PAdd(0)
	, m_PData(NULL), m_PDataSize(0)
	, m_PackIsChange(TRUE)
{
	m_strPackage = new BYTE[BUFSIZE]{ 0 };
	size_t index = 0; // 当前对缓冲区处理位置
	int i = 0;     // 临时变量用于查找包头
	for (; i < size - 1; i++) {
		if (*(unsigned short*)(buffer + i) == 0xFEFF) {
			break;
		}
	}
	index = i;
	if (index + 10 > size) { // 后面的固定字节，要在buffer中，当前index在10的前面 10：m_PHead + m_PCmd + m_PLength + m_PAdd 的所占字节数
		// 把index前面的数据删除，没有用了
		memmove(buffer, buffer + index, size - index);
		memset(buffer + (size - index), 0, index);
		// 将size设置为当前存在数据的尾部位置
		size = size - (int)index;
		return;
	}
	m_PHead = 0xFEFF; index += 2;
	m_PCmd = *(unsigned short*)(buffer + index); index += 2;
	m_PLength = *(unsigned short*)(buffer + index); index += 2;
	if (index + m_PLength > size) { // 当前总长度不足m_PLength的长度，可能是没接收完
		//把前面的数据删掉
		memmove(buffer, buffer + index - 6, size - index + 6);
		memset(buffer + (size - index + 6), 0, index - 6);
		size = size - (int)index + 6;
		return;
	}
	m_PAdd = *(unsigned*)(buffer + index); index += 4;
	do {
		m_PDataSize = m_PLength - 4;
		if (!m_PDataSize) {
			break;
		}
		unsigned sum = 0;
		for (size_t i = index; i < index + m_PDataSize; i++) {
			sum += buffer[i];
		}
		if (m_PAdd == sum) {
			m_PData = std::make_shared<char*>(new char[BUFSIZE] { 0 });
			memcpy(*m_PData, buffer + index, m_PDataSize);
		}
		else {
			//TODO:把这这个数据清理掉，有错误
			CLTools::ErrorOut("校验和不一致，数据有误！", __FILE__, __LINE__);
		}
		index += m_PDataSize;
	} while (false);
	// 将处理过的数据清除
	memmove(buffer, buffer + index, size - index);
	memset(buffer + (size - index), 0, index);
	size = size - (int)index;
}

CLPackage::CLPackage(const CLPackage& clp)
{
	m_PHead = clp.m_PHead;
	m_PCmd = clp.m_PCmd;
	m_PLength = clp.m_PLength;
	m_PAdd = clp.m_PAdd;
	m_PDataSize = clp.m_PDataSize;
	m_PData = std::make_shared<char*>(new char[BUFSIZE] { 0 });
	if (m_PDataSize > 0) {
		memcpy(*m_PData, *clp.m_PData, m_PDataSize);
	}
	m_strPackage = new BYTE[BUFSIZE]{ 0 };
	m_PackIsChange = TRUE;
}

CLPackage& CLPackage::operator=(const CLPackage& clp)
{
	if (this != &clp) {
		m_PHead = clp.m_PHead;
		m_PCmd = clp.m_PCmd;
		m_PLength = clp.m_PLength;
		m_PAdd = clp.m_PAdd;
		if (clp.m_PDataSize) {
			if (m_PData) {
				memset(*m_PData, 0, m_PDataSize);
				memcpy(*m_PData, *clp.m_PData, clp.m_PDataSize);
			}
			else {
				m_PData = std::make_shared<char*>(new char[BUFSIZE] { 0 });
				memcpy(*m_PData, *clp.m_PData, clp.m_PDataSize);
			}
		}
		m_PDataSize = clp.m_PDataSize;
		m_PackIsChange = TRUE;
	}
	return *this;
}

CLPackage::~CLPackage()
{
	if (m_PData.use_count() == 1) { // 当前析构为m_PData最后一个实例
		char* pStr = *m_PData;
		*m_PData = NULL;
		m_PData.reset();
		delete[] pStr;
	}
    if (m_strPackage)
		delete[] m_strPackage;
}

void CLPackage::SetCmd(unsigned short cmd)
{
	m_PCmd = cmd;
	m_PackIsChange = TRUE;
}

void CLPackage::SetData(const char* data)
{
	if (data) {		
		if (!m_PData) {

			m_PData = std::make_shared<char*>(new char[m_PDataSize + 1]{ 0 });
		}
		else {
			memset(*m_PData, 0, m_PDataSize);
		}
		m_PDataSize = strlen(data);
		memcpy(*m_PData, data, m_PDataSize);
		SetPAdd();
	}
	m_PLength = 4 + (unsigned short)m_PDataSize;
	m_PackIsChange = TRUE;
}

unsigned short CLPackage::GetCmd() const
{
	return m_PCmd;
}

const char* CLPackage::GetData() const
{
    if (m_PData == NULL || (*m_PData) == NULL)
		return NULL;
	return *m_PData;
}

size_t CLPackage::GetDataSize() const
{
	return m_PDataSize;
}

#define BITSIZE(x) (sizeof(x) * 8)


const char* CLPackage::MemStream()
{
	if (m_PackIsChange) {
		memset(m_strPackage, 0, sizeof(m_strPackage));
		size_t index = 0;
		size_t bitAllSize = BITSIZE(m_PHead) + BITSIZE(m_PCmd) + BITSIZE(m_PLength) + BITSIZE(m_PAdd);
		char* strPtr = (char*)m_strPackage;
		Value2MemValue((PBYTE)(strPtr + index), m_PHead, sizeof(m_PHead)); index += sizeof(m_PHead);
		Value2MemValue((PBYTE)(strPtr + index), m_PCmd, sizeof(m_PCmd)); index += sizeof(m_PCmd);
		Value2MemValue((PBYTE)(strPtr + index), m_PLength, sizeof(m_PLength)); index += sizeof(m_PLength);
		Value2MemValue((PBYTE)(strPtr + index), m_PAdd, sizeof(m_PAdd)); index += sizeof(m_PAdd);
		/*unsigned short phead = *(unsigned short*)strPtr;
		CLTools::ErrorOut(m_strPackage.c_str(), __FILE__, __LINE__);*/
		if (m_PDataSize)
			memcpy(strPtr + index, *m_PData, m_PDataSize);
		m_PackIsChange = FALSE;
	}
	return (char*)m_strPackage;
}

size_t CLPackage::GetSize() const
{
	return sizeof(m_PHead) + sizeof(m_PCmd) + sizeof(m_PLength) + m_PLength;
}

void CLPackage::SetPLen()
{
	m_PLength = 4 + (unsigned short)m_PDataSize;
	m_PackIsChange = TRUE;
}

void CLPackage::SetPAdd()
{
	unsigned int sum = 0;
	for (size_t i = 0; i < m_PDataSize; i++) { // 计算数据校验和
		sum += (*m_PData)[i];
	}
	m_PAdd = sum;
	m_PackIsChange = TRUE;
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

/*const char* fileInfo::operator&()
{
	m_strFileInfo += m_fileName;
	m_strFileInfo += m_isDir;
	m_strFileInfo += m_isHidden;
	m_strFileInfo += m_isLast;
	return m_strFileInfo.c_str();
}*/

void fileInfo::MemStream(PBYTE mem)
{
	int index = 0;
	memcpy(mem, m_fileName, sizeof(m_fileName));
	index += sizeof(m_fileName);
	PBYTE byte = (PBYTE)&m_isDir;
	for (int i = 0; i < sizeof(BOOL) * 3; i++) {
		mem[index + i] = byte[i];
	}
}
