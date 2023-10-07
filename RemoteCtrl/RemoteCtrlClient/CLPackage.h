#pragma once
#include <memory>
#include <vector>

enum {
	COM_NULL,          // û�������̽��
	COM_INVALID,       // ��Ч�İ�����
	COM_TESTCONNECT,   // ��������
	COM_USERLOGIN,     // �û���¼
	COM_GETDRIVE,      // ��ȡ��
	COM_GETFILE,       // ��ȡ�ļ����ļ���
	COM_GETFILES,      // ��ȡ�ļ�����ȡ�ļ���
	COM_FILEDOWNLOAD,  // �ļ�����
	COM_FILEDELETE,    // �ļ�ɾ��
	COM_FILEUPLOAD,	   // �ļ��ϴ�
	COM_REMOTEDESKTOP, // Զ������
	COM_MOUSEEVENT,    // �����Ϣ
	COM_KEYEVENT,      // ������Ϣ
	COM_SYSTEMLOCK,    // ϵͳ����
	COM_SYSTEMUNLOCK   // ϵͳ����
};

#pragma pack(push,1) // �����ڴ���뷽ʽ��������ϵ����ջ

#define FILESIZE 80
// �ļ���Ϣ��
typedef struct fileInfo
{
	char*       m_fileName;           // �ļ���
	BOOL        m_isDir;              // �Ƿ�ΪĿ¼
	BOOL        m_isHidden;           // �Ƿ�������״̬
	BOOL        m_isLast;             // �Ƿ������һ��
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

// ���ݰ���
class CLPackage
{
public:
	CLPackage();
	// ��װ
	CLPackage(unsigned short cmd, const char* data = NULL, size_t dataSize = 0);
	// ���װ������size�޸�Ϊ�����껺������ǰ���ݵ����һ���ֽ�
	CLPackage(char* buffer, int& size);
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
	// ������ݰ��е�����
	void DataClear();
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
	PBYTE                  m_strPackage;   // �ַ���������
	BOOL                   m_PackIsChange; // ���Ƿ����仯��TRUE �����仯 FALSE δ�����仯
};
#pragma pack(pop) // ���Զ������ϵ����ջ���ָ�Ĭ�϶��뷽ʽ
