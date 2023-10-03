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
	CLNetworkSocket<CTCP> m_sock;
	CString m_SerIp;
	short m_SerPort;
	CLPackage m_pack;
	std::shared_ptr<char*> m_buffer;
	class Helper {
	public:
		Helper() {}
		~Helper() {
			releaseInstance();
		}
	};
	static Helper m_helper;
	static CLRCCliControl* m_pControl;
};