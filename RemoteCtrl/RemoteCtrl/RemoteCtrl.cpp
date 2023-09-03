// RemoteCtrl.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "framework.h"
#include "RemoteCtrl.h"
#include "CLComDispose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int main(int argc, char** argv, char** env)
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(nullptr);

	if (hModule != nullptr)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
		{
			// TODO: 在此处为应用程序的行为编写代码。
			wprintf(L"错误: MFC 初始化失败\n");
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO: 更改错误代码以符合需要
		wprintf(L"错误: GetModuleHandle 失败\n");
		nRetCode = 1;
	}
	/*if (argc == 1) {
		CLNetworkSocket<CUDP> sock(1);
		sock.Init("127.0.0.1", 9889);
		std::string buf;
		buf.resize(256);
		sock.Recv(buf);
		std::cout << "form client:[" << buf << "]" << std::endl;
		sock.Send(buf);
	}
	else {
		CLNetworkSocket<CUDP> sock(0);
		sock.Init();
		std::string buf;
		std::cout << "请输入要发给服务器的信息：";
		std::cin >> buf;
		sock.Send(buf, "127.0.0.1", 9889);
		memset((char*)buf.c_str(), 0, buf.size());
		sock.Recv(buf);
		std::cout << "form server:[" << buf << "]" << std::endl;
	}*/
	/*
	//WM_LBUTTONDOWN //鼠标宏
	for (int i = 0; i < 100; i++) {
		SetCursorPos(i, i); // 设置鼠标位置
		//mouse_event 设置鼠标按下抬起函数
		//SendInput 鼠标键盘输入
		Sleep(20);
	}*/
	//ClipCursor禁止鼠标移动函数
}
