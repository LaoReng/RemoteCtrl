#include "pch.h"
#include "CLPackage.h"

CLPackage::CLPackage()
	: m_PHead(0xFEFF)
	, m_PSeq(1)
	, m_PData(NULL)
	, m_PDataSize(0)
{
	SetPLen();
	SetPAdd();
}

CLPackage::CLPackage(const char* data, unsigned short sql)
	: m_PHead(0xFEFF)
	, m_PSeq(sql)
	, m_PData(NULL)
	, m_PDataSize(0)
{
	if (data) {
		m_PDataSize = strlen(data) + 1;
		m_PData = std::make_shared<char*>(new char[m_PDataSize] { 0 });
		memcpy(*m_PData, data, m_PDataSize - 1);
	}
	SetPLen();
	SetPAdd();
}

CLPackage::CLPackage(char* buffer, size_t size)
	: m_PHead(0), m_PSeq(0)
	, m_PLength(0), m_PAdd(0)
	, m_PData(NULL), m_PDataSize(0)
{
	size_t index = 0;
	for (int i = 0; i < size - 1; i++) {
		if ((unsigned short)(buffer + i) == 0xFEFF) {
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
	m_PSeq = unsigned short(buffer + index); index += 2;
	m_PLength = unsigned short(buffer + index); index += 2;
	if (index + m_PLength > size) {
		//TODO:把前面的数据删掉
		memmove(buffer, buffer + index - 6, size - index + 6);
		memset(buffer + (size - index + 6), 0, index - 6);
		return;
	}
	m_PDataSize = m_PLength - 4;
	m_PAdd = unsigned(buffer + index); index += 4;
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
	m_PSeq = clp.m_PSeq;
	m_PLength = clp.m_PLength;
	m_PAdd = clp.m_PAdd;
	m_PData = clp.m_PData;
	m_PDataSize = clp.m_PDataSize;
}

CLPackage& CLPackage::operator=(const CLPackage& clp)
{
	if (this != &clp) {
		m_PHead = clp.m_PHead;
		m_PSeq = clp.m_PSeq;
		m_PLength = clp.m_PLength;
		m_PAdd = clp.m_PAdd;
		m_PData = clp.m_PData;
		m_PDataSize = clp.m_PDataSize;
	}
	return *this;
}

CLPackage::~CLPackage()
{
	if (m_PData.use_count() == 1) {
		char* pStr = *m_PData;
		*m_PData = NULL;
		m_PData.reset();
		delete pStr;
	}
}

void CLPackage::SetData(const char* data)
{
	if (m_PData.use_count() == 1) {
		delete* m_PData;
	}
	else {
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
}

unsigned short CLPackage::GetSeq() const
{
	return m_PSeq;
}

const char* CLPackage::GetData() const
{
	return *m_PData;
}

const char* CLPackage::Str() const
{
	std::string str;
	str.resize(10 + m_PDataSize);
	str += (unsigned)m_PHead;
	str += (unsigned)m_PSeq;
	str += (unsigned)m_PLength;
	str += m_PAdd;
	str += *m_PData;
	return str.data();
}

void CLPackage::SetPLen()
{
	m_PLength = 4 + (unsigned short)m_PDataSize;
}

void CLPackage::SetPAdd()
{
	unsigned int sum = 0;
	for (size_t i = 0; i < m_PDataSize; i++) {
		sum += (*m_PData)[i];
	}
	m_PAdd = sum;
}