#pragma once
#include <memory>
#include <vector>

#pragma pack(push,1) // �����ڴ���뷽ʽ��������ϵ����ջ
class CLPackage
{
public:
	CLPackage();
	// ��װ
	CLPackage(unsigned short cmd,const char* data = "");
	// ���װ
	CLPackage(char* buffer, size_t size);
	// ���ƹ���
	CLPackage(const CLPackage& clp);
	// ���ں�����
	CLPackage& operator=(const CLPackage& clp);
	~CLPackage();
	// ���ð�����
	void SetCmd(unsigned short cmd);
	// ���ð������ݲ���
	void SetData(const char* data);
	// ��ȡ������
	unsigned short GetCmd() const;
	// ��ȡ�������ݲ���
	const char* GetData() const;
	// �ַ�����
	const char* Str();
private:
	// �������ݰ����ȣ��ڲ�ʹ��
	void SetPLen();
	// ��������У��ͣ��ڲ�ʹ��
	void SetPAdd();
private:
	unsigned short         m_PHead;        // ���ݰ�ͷ
	unsigned short         m_PCmd;         // ���ݰ�����
	unsigned short         m_PLength;      // ���ݰ�����
	unsigned int           m_PAdd;         // ����У���
	std::shared_ptr<char*> m_PData;        // ����
	size_t                 m_PDataSize;    // ���ݳ���
	std::string            m_strPackage;   // �ַ���������
	BOOL                   m_PackIsChange; // ���Ƿ����仯��1 �����仯 0 δ�����仯
};
#pragma pack(pop) // ���Զ������ϵ����ջ���ָ�Ĭ�϶��뷽ʽ
