#pragma once
#include <memory>
#include <vector>

#pragma pack(push,1) // �����ڴ���뷽ʽ��������ϵ����ջ
class CLPackage
{
public:
	CLPackage();
	// ��װ
	CLPackage(const char* data, unsigned short sql = 1);
	// ���װ
	CLPackage(char* buffer, size_t size);
	// ���ƹ���
	CLPackage(const CLPackage& clp);
	// ���ں�����
	CLPackage& operator=(const CLPackage& clp);
	~CLPackage();
	// ���ð������ݲ���
	void SetData(const char* data);
	// ��ȡ�����
	unsigned short GetSeq() const;
	// ��ȡ�������ݲ���
	const char* GetData() const;
	// �ַ�����
	const char* Str() const;
private:
	// �������ݰ�����
	void SetPLen();
	// ��������У���
	void SetPAdd();
private:
	unsigned short         m_PHead;     // ���ݰ�ͷ
	unsigned short         m_PSeq;      // ���ݰ����
	unsigned short         m_PLength;   // ���ݰ�����
	unsigned int           m_PAdd;      // ����У���
	std::shared_ptr<char*> m_PData;     // ����
	size_t                 m_PDataSize; // ���ݳ���
};
#pragma pack(pop) // ���Զ������ϵ����ջ���ָ�Ĭ�϶��뷽ʽ
