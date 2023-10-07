#pragma once
#include "CLPackage.h"
#include "CLNetworkSocket.h"
#include <vector>
// ���Ʋ�
class CLRCCliControl
{
public:
	// �������ݰ�
	int Send();
	// ��������
	// isAutoClose���Ƿ��Զ��ر�������������� TRUE �Զ��ر����� FALSE ��Ҫ�û�����Close()�ر�������������ӣ�index��ָ����ջ�������ʼ���յ�λ�ã��״ε��ø�ֵΪ0���´ε��ø�ֵ��Recv()�ķ���ֵ����
	// ����ֵ������ִ��ʧ�ܣ��򷵻�ֵС��0������ִ�гɹ����򷵻����ڽ��ջ���������β��λ��
	int Recv(BOOL isAutoClose = TRUE, size_t index = 0);
	// ��ȡ��Ψһ����ָ��
	static CLRCCliControl* getInstance();
	// ��ȡ���ݰ�
	CLPackage& GetPackage();
	// �������ݰ������������
	// cmd���������ݰ����� data���������ݰ����ݣ�Ĭ��ΪNULL
	void SetPackage(unsigned short cmd, const char* data = NULL);
	// ���÷�������IP��ַ
	void SetSerIp(const char* ip);
	// ���÷������Ķ˿�
	void SetSerPort(const short port);
	// ��ȡ��������IP��ַ
	const char* GetSerIp();
	// ��ȡ�������Ķ˿�
	short GetSerPort();
	// �رտͻ����׽���
	void Close();
private:
	CLRCCliControl();
	~CLRCCliControl();
	CLRCCliControl(const CLRCCliControl&) = delete;
	CLRCCliControl& operator=(const CLRCCliControl&) = delete;
	// ������Ψһ����ָ��
	static void releaseInstance();
private:
	class Helper {
	public:
		Helper() {}
		~Helper() {
			releaseInstance();
		}
	};
	CLNetworkSocket<CTCP>  m_sock;     // �ͻ����׽���
	CString                m_SerIp;    // ������IP��ַ       
	short                  m_SerPort;  // �������˿ں�
	CLPackage              m_pack;     // ����ʵ��������
	std::shared_ptr<char*> m_buffer;   // �������ݵĻ�����
	static Helper          m_helper;   // Helper�������������CLRCCliControl������
	static CLRCCliControl* m_pControl; // CLRCCliControl��Ψһʵ����ָ��
};