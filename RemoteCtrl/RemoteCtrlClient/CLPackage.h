#pragma once
#include <memory>
#include <vector>

enum {
	COM_NULL,          // û�������̽��
	COM_TESTCONNECT,   // ��������
	COM_USERLOGIN,     // �û���¼
	COM_GETDRIVE,      // ��ȡ��
	COM_GETFILE,       // ��ȡ�ļ�
	COM_FILEDOWNLOAD,  // �ļ�����
	COM_FILEUPLOAD,	   // �ļ��ϴ�
	COM_REMOTEDESKTOP, // Զ������
	COM_MOUSEEVENT,    // �����Ϣ
	COM_KEYEVENT,      // ������Ϣ
	COM_SYSTEMLOCK,    // ϵͳ����
	COM_SYSTEMUNLOCK   // ϵͳ����
};

#pragma pack(push,1) // �����ڴ���뷽ʽ��������ϵ����ջ

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
	// ��װ
	CLPackage(unsigned short cmd, const char* data = "",size_t dataSize = 0);
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
	// ��ȡ�������ݲ��ִ�С
	size_t GetDataSize() const;
	// �ַ�����
	const char* Str();
	
private:
	// �������ݰ����ȣ��ڲ�ʹ��
	void SetPLen();
	// ��������У��ͣ��ڲ�ʹ��
	void SetPAdd();
	// ����ֵת��Ϊ�����ƣ�ָ��λ��������0���ڲ�ʹ��
	// value����ֵ��ByteSize����������ռ�ֽ���
	const char* Value2BinStr(unsigned int value, unsigned char ByteSize);
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