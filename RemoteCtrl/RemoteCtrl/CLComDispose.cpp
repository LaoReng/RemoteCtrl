#include "pch.h"
#include "CLComDispose.h"

#define BUFSIZE 1024

CLComDispose::CLComDispose()
	: m_sock(1)
	, m_lockThreadID(0)
	, m_hLockThread(INVALID_HANDLE_VALUE)
{
	m_buffer = new char[BUFSIZE] {};
	m_sock.Init("127.0.0.1", 7968);
}

CLComDispose::~CLComDispose()
{
	if (m_buffer) {
		char* temp = m_buffer;
		m_buffer = NULL;
		delete temp;
	}
}

void CLComDispose::Accept()
{
	int TestCount = 0;
	CString str;
	while (true) {
		m_sock.Joint(4);
		if (Recv() < 0)
			break;
		ComDis();
		m_sock.CloseJointSock();
		str.Format("处理第%d个连接命令请求\r\n", ++TestCount);
		OutputDebugString(str);
	}
}

void CLComDispose::ComDis()
{
	switch (m_pack.GetCmd()) {
	case COM_NULL:
		break;
	case COM_TESTCONNECT:
		testConnect();
		break;
	case COM_USERLOGIN:
		userLogin();
		break;
	case COM_GETDRIVE:
		getDrive();
		break;
	case COM_GETFILE:
		getFile();
		break;
	case COM_FILEDOWNLOAD:
		fileDownload();
		break;
	case COM_FILEUPLOAD:
		fileUpload();
		break;
	case COM_REMOTEDESKTOP:
		remoteDesktop();
		break;
	case COM_SYSTEMLOCK:
		systemLock();
		break;
	case COM_SYSTEMUNLOCK:
		systemUnlock();
		break;
	default:
		break;
	}
}

int CLComDispose::Recv()
{
	int ret = m_sock.Recv((PBYTE)m_buffer, BUFSIZE);
	if (ret < 0) {
		CLTools::ErrorOut("数据接收错误！", __FILE__, __LINE__);
		return ret;
	}
	m_pack = CLPackage(m_buffer, ret);
	return ret;
}

int CLComDispose::Send()
{
	int ret = m_sock.Send((PBYTE)m_pack.MemStream(), m_pack.GetSize());
	if (ret < 0) {
		CLTools::ErrorOut("数据发送错误！", __FILE__, __LINE__);
		return ret;
	}
	return ret;
}

void CLComDispose::testConnect()
{
	if (Send() < 0) {
		CLTools::ErrorOut("testConnect send error!", __FILE__, __LINE__);
	}
}

void CLComDispose::userLogin()
{
}

void CLComDispose::getDrive()
{
	std::string DriInfo;
	BOOL ret;
	DWORD len = 0;
	char VolumeName[MAX_PATH] = "";
	HANDLE FindDriHandle = FindFirstVolume(VolumeName, sizeof(VolumeName));
	if (FindDriHandle == INVALID_HANDLE_VALUE) {
		CLTools::ErrorOut("磁盘获取失败！", __FILE__, __LINE__);
		m_pack = CLPackage(COM_GETDRIVE, "");
		if (Send() < 0) {
			CLTools::ErrorOut("getDrive send error!", __FILE__, __LINE__);
		}
		return;
	}
	do {
		// 判断格式是否正确
		size_t index = strlen(VolumeName) - 1;
		if (!VolumeFormat(VolumeName, "\\\\?\\") || VolumeName[index] != '\\') {
			CLTools::ErrorOut("磁盘获取失败！", __FILE__, __LINE__);
			DriInfo.clear();
			break;
		}
		char driveName[MAX_PATH] = "";
		GetVolumePathNamesForVolumeName(VolumeName, driveName, sizeof(driveName), &len);
		if (len > 1) {
			DriInfo += driveName[0];
			DriInfo += ',';
		}
		memset(VolumeName, 0, sizeof(VolumeName));
		ret = FindNextVolume(FindDriHandle, VolumeName, sizeof(VolumeName));
	} while (ret);
	if (GetLastError() != ERROR_NO_MORE_FILES || DriInfo.size() < 1) {
		CLTools::ErrorOut("磁盘获取失败！", __FILE__, __LINE__);
		m_pack = CLPackage(COM_GETDRIVE, "");
		return;
	}
	else {
		m_pack = CLPackage(COM_GETDRIVE, DriInfo.c_str(), DriInfo.size());
	}
	FindVolumeClose(FindDriHandle);
	if (Send() < 0) {
		CLTools::ErrorOut("getDrive send error!", __FILE__, __LINE__);
	}
}

void CLComDispose::getFile()
{
	std::string dir = m_pack.GetData(); dir += '*'; //对文件目录进行处理
	WIN32_FIND_DATA fileInfo = {}; // 保存查询到的文件信息
	BYTE strFileInfos[sizeof(FILEINFO) * 13] = ""; // 保存文件信息字符串流
	BOOL ret = FALSE;
	HANDLE findFileHandle = FindFirstFile(dir.c_str(), &fileInfo);
	if (findFileHandle == INVALID_HANDLE_VALUE) { // 目录获取失败，发送终止信息
		CLTools::ErrorOut("文件夹打开失败！", __FILE__, __LINE__);
		FILEINFO fileinfo;
		fileinfo.MemStream(strFileInfos);
		m_pack = CLPackage(COM_GETFILE, (char*)strFileInfos, sizeof(fileinfo));
		if (Send() < 0) {
			CLTools::ErrorOut("getFile send error!", __FILE__, __LINE__);
		}
		return;
	}
	// 查找目录下的文件和文件夹
	int count = 0;
	int cun = 0;
	do {
		FILEINFO _fileinfo(fileInfo.cFileName, fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY, fileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN, FALSE);
		_fileinfo.MemStream(strFileInfos + sizeof(FILEINFO) * count); // 添加到内存流中
		count++;
		if (count == 13) {
			m_pack = CLPackage(COM_GETFILE, (char*)strFileInfos, sizeof(strFileInfos));
			if (Send() < 0) {
				CLTools::ErrorOut("getFile send error!", __FILE__, __LINE__);
			}
			memset(strFileInfos, 0, sizeof(strFileInfos));
			count = 0;
		}
		ret = FindNextFile(findFileHandle, &fileInfo);
		Sleep(1);
		cun++;
	} while (ret);
	if (GetLastError() != ERROR_NO_MORE_FILES) {
		CLTools::ErrorOut("文件夹读取失败！", __FILE__, __LINE__);
	}
	FindClose(findFileHandle);
	// 最后一个终止信息
	FILEINFO fileinfo;
	fileinfo.MemStream(strFileInfos + sizeof(FILEINFO) * count);
	count++;
	m_pack = CLPackage(COM_GETFILE, (char*)strFileInfos, sizeof(fileinfo) * count);
	if (Send() < 0) {
		CLTools::ErrorOut("getFile send error!", __FILE__, __LINE__);
	}
	CString str;
	str.Format("一个%d个文件和文件夹", ++cun);
	CLTools::ErrorOut(str, __FILE__, __LINE__);
	Sleep(10);
}

void CLComDispose::fileDownload()
{
	HANDLE hFile = CreateFile(m_pack.GetData(), GENERIC_READ, FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		CLTools::ErrorOut("文件打开失败！", __FILE__, __LINE__);
		m_pack = CLPackage(COM_FILEDOWNLOAD);
		if (Send() < 0) {
			CLTools::ErrorOut("fileDownload send error!", __FILE__, __LINE__);
		}
		return;
	}
	DWORD FileSize = GetFileSize(hFile, NULL);
	if (FileSize == INVALID_FILE_SIZE) {
		CloseHandle(hFile);
		CLTools::ErrorOut("文件大小获取失败！", __FILE__, __LINE__);
		m_pack = CLPackage(COM_FILEDOWNLOAD);
		if (Send() < 0) {
			CLTools::ErrorOut("fileDownload send error!", __FILE__, __LINE__);
		}
		return;
	}
	char buffer[1000] = ""; // 文件数据缓冲区
	DWORD RLen = {};        // ReadFile() 读取到的数据大小
	// 发送文件的大小
	m_pack = CLPackage(COM_FILEDOWNLOAD, _ultoa(FileSize, buffer, 10));
	if (Send() < 0) {
		CLTools::ErrorOut("fileDownload send error!", __FILE__, __LINE__);
	}
	do {
		memset(buffer, 0, sizeof(buffer));
		if (FALSE == ReadFile(hFile, buffer, sizeof(buffer), &RLen, NULL)) {
			CLTools::ErrorOut("文件读取失败！", __FILE__, __LINE__);
			break;
		}
		m_pack = CLPackage(COM_FILEDOWNLOAD, buffer);
		if (Send() < 0) {
			CLTools::ErrorOut("fileDownload send error!", __FILE__, __LINE__);
		}
		FileSize -= RLen;
	} while (FileSize > 0);
	CloseHandle(hFile);
}

void CLComDispose::fileUpload()
{
	//TODO:这块还有一个问题：需要创建一个文件夹存放控制端发过来的文件
	HANDLE hNewFile = CreateFile(m_pack.GetData(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hNewFile == INVALID_HANDLE_VALUE) {
		CLTools::ErrorOut("文件创建失败！", __FILE__, __LINE__);
		m_pack = CLPackage(COM_FILEUPLOAD);
		if (Send() < 0) {
			CLTools::ErrorOut("fileUpload send error!", __FILE__, __LINE__);
		}
		return;
	}
	DWORD FileSize = {};
	Recv();
	if (m_pack.GetCmd() == COM_FILEUPLOAD) {
		char* endptr = NULL;
		FileSize = strtoul(m_pack.GetData(), &endptr, 10);
		if (FileSize == INVALID_FILE_SIZE) {
			CloseHandle(hNewFile);
			CLTools::ErrorOut("文件大小获取失败！", __FILE__, __LINE__);
			m_pack = CLPackage(COM_FILEUPLOAD);
			if (Send() < 0) {
				CLTools::ErrorOut("fileUpload send error!", __FILE__, __LINE__);
			}
			return;
		}
	}
	DWORD WriteLen = {};
	do {
		Recv();
		if (m_pack.GetCmd() != COM_FILEUPLOAD) {
			break;
		}
		DWORD DataSize = (DWORD)m_pack.GetDataSize();
		WriteFile(hNewFile, m_pack.GetData(), DataSize, &WriteLen, NULL);
		if (WriteLen != DataSize) {
			CLTools::ErrorOut("文件写入出错！", __FILE__, __LINE__);
			break;
		}
		FileSize -= WriteLen;
	} while (FileSize > 0);
	CloseHandle(hNewFile);
	m_pack = CLPackage(COM_FILEUPLOAD, "ok");
	if (Send() < 0) {
		CLTools::ErrorOut("fileUpload send error!", __FILE__, __LINE__);
	}
}

void CLComDispose::remoteDesktop()
{
	CImage image;
	RECT rect = {};
	HDC hDC = ::GetDC(NULL); // 获取桌面DC
	if (::GetWindowRect(GetDesktopWindow(), &rect) == FALSE) { // 获取桌面宽高比
		CLTools::ErrorOut("屏幕大小获取失败！", __FILE__, __LINE__);
		return;
	}
	image.Create(rect.right, rect.bottom, GetDeviceCaps(hDC, BITSPIXEL)/*获取屏幕BPP*/);
	BOOL ret = BitBlt(image.GetDC(), rect.left, rect.top, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);
	image.ReleaseDC();
	if (ret) {
		HGDIOBJ memOb = NULL;
		if (memOb = GlobalAlloc(GMEM_MOVEABLE, 0)) { // 申请大小可变缓冲区
			IStream* pStream = NULL;
			if (CreateStreamOnHGlobal(memOb, TRUE, &pStream) == S_OK) { // 将缓冲区与输入流绑定
				image.Save(pStream, Gdiplus::ImageFormatPNG); // 将图片保存在缓冲区中
				LARGE_INTEGER move = {};
				pStream->Seek(move, STREAM_SEEK_SET, NULL); // 流指针移到开头
				PBYTE pData = NULL;
				if (pData = (PBYTE)GlobalLock(memOb)) { // 锁定缓冲区，并获取缓冲区数据的首地址
					size_t DataSize = GlobalSize(memOb); // 获取缓冲区数据大小
					m_pack = CLPackage(COM_REMOTEDESKTOP, (char*)pData);
					if (Send() < 0) {
						CLTools::ErrorOut("remoteDesktop send error!", __FILE__, __LINE__);
					}
					GlobalUnlock(memOb);
				}
			}
			GlobalFree(memOb);
		}
	}
	image.Destroy();

}

void CLComDispose::systemLock()
{
	m_hLockThread = (HANDLE)_beginthreadex(NULL, 0, sysLockThread, this, 0, &m_lockThreadID);
	m_pack = CLPackage(COM_SYSTEMLOCK, "ok");
	if (Send() < 0) {
		CLTools::ErrorOut("systemLock send error!", __FILE__, __LINE__);
	}
}

void CLComDispose::systemUnlock()
{
	PostThreadMessage(m_lockThreadID, WM_KEYDOWN, 0x4C, 0);
	m_pack = CLPackage(COM_SYSTEMUNLOCK, "ok");
	if (Send() < 0) {
		CLTools::ErrorOut("systemUnlock send error!", __FILE__, __LINE__);
	}
}

BOOL CLComDispose::VolumeFormat(const char* volumename, const char* format)
{
	for (int i = 0; format[i] != '\0'; i++) {
		if (volumename[i] != format[i])
			return FALSE;
	}
	return TRUE;
}

unsigned __cdecl CLComDispose::sysLockThread(void* args)
{
	CLComDispose* thiz = (CLComDispose*)args;
	thiz->sysLockThreadMain();
	_endthreadex(0);
	return 0;
}

void CLComDispose::sysLockThreadMain()
{
	m_lockDlg.Create(IDD_DLG_SYSLOCK);
	m_lockDlg.ShowWindow(SW_HIDE);
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeigth = GetSystemMetrics(SM_CYSCREEN);
	RECT rect = { 0,0,nWidth,nHeigth };
	RECT rectText = {};
	m_lockDlg.MoveWindow(&rect);
	CWnd* pWnd = m_lockDlg.GetDlgItem(IDC_STATIC_TEXT);
	pWnd->GetClientRect(&rectText);
	int x = (rect.right - rect.left) / 2 - (rectText.right - rectText.left) / 2;
	int y = (rect.bottom - rect.top) / 2 - (rectText.bottom - rectText.top) / 2;
	POINT point;
	point.x = x;
	point.y = y;
	pWnd->SetCaretPos(point);
	//m_lockDlg.BringWindowToTop();
	m_lockDlg.ShowWindow(SW_SHOW);
	BOOL bRet;
	MSG msg = {};
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_KEYDOWN) {
			if (msg.wParam == 0x4C) {
				break;
			}
		}
	}
	m_lockDlg.DestroyWindow(); // Create必须调用
}
