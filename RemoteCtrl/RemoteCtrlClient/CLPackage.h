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
	CLPackage(unsigned short cmd, const char* data = NULL, size_t dataSize = 0);
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
	// ���ݰ��ڴ�����
	const char* MemStream();
	// ��ȡ���Ĵ�С
	size_t GetSize() const;
private:
	// �������ݰ����ȣ��ڲ�ʹ��
	void SetPLen();
	// ��������У��ͣ��ڲ�ʹ��
	void SetPAdd();
	// ����ֵת��Ϊ�����ƣ�ָ��λ��������0���ڲ�ʹ��
	// value����ֵ��ByteSize����������ռ�ֽ���
	const char* Value2BinStr(unsigned int value, unsigned char ByteSize);
	// ����ֵת��Ϊ�ڴ�ֵ
	// mem���ڴ滺������value��Ҫת����ֵ��ValueSize��ֵ��ռ���ֽ���
	void Value2MemValue(PBYTE mem, unsigned int value, size_t ValueSize);
protected:
	unsigned short         m_PHead;        // ���ݰ�ͷ���̶�0xFEFF
	unsigned short         m_PCmd;         // ���ݰ�����
	unsigned short         m_PLength;      // ���ݰ����ȣ�������У��͵����ݰ�β��
	unsigned int           m_PAdd;         // ����У��ͣ��������鲿�ֵĺͣ�Ĭ����0���û������Ͳ���Ҫ����
	std::shared_ptr<char*> m_PData;        // ����
private:
	size_t                 m_PDataSize;    // ���ݳ���
	std::string            m_strPackage;   // �ַ���������
	BOOL                   m_PackIsChange; // ���Ƿ����仯��TRUE �����仯 FALSE δ�����仯
};
#pragma pack(pop) // ���Զ������ϵ����ջ���ָ�Ĭ�϶��뷽ʽ
