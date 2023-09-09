#pragma once
#include "CLPackage.h"
#include "CLNetworkSocket.h"
#include <vector>
// 控制层
class CLRCCliControl
{
public:
	int Send();
	// isAutoClose：是否自动关闭与服务器的连接
	int Recv(BOOL isAutoClose = TRUE);
	static CLRCCliControl* getInstance();
	CLPackage& GetPackage();
	void SetPackage(unsigned short cmd, const char* data = NULL);
private:
	CLRCCliControl();
	~CLRCCliControl();
	CLRCCliControl(const CLRCCliControl&) = delete;
	CLRCCliControl& operator=(const CLRCCliControl&) = delete;
	static void releaseInstance();
private:
	CLNetworkSocket<CTCP> m_sock;
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