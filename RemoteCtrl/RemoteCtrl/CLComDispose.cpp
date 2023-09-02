#include "pch.h"
#include "CLComDispose.h"


CLComDispose::CLComDispose()
	:m_sock(1)
{
	m_buffer.resize(MAX_BUFFER);
	m_sock.Init("127.0.0.1", 7968);
}

CLComDispose::~CLComDispose()
{}

void CLComDispose::Accept()
{
	while (true) {
		m_sock.Joint(4);
		Recv();
		ComDis();
		//Send();
		m_sock.CloseJointSock();
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
	int ret = m_sock.Recv(m_buffer);
	if (ret < 0) {
		CLTools::ErrorOut("数据接收错误！", __FILE__, __LINE__);
		return ret;
	}
	m_pack = CLPackage((char*)m_buffer.c_str(), strlen(m_buffer.c_str()));
	return ret;
}

int CLComDispose::Send()
{
	int ret = m_sock.Send(m_pack.Str());
	if (ret < 0) {
		CLTools::ErrorOut("数据发送错误！", __FILE__, __LINE__);
		return ret;
	}
	return ret;
}

void CLComDispose::testConnect()
{
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
		return;
	}
	do {
		// 判断格式是否正确
		size_t index = strlen(VolumeName) - 1;
		if (!VolumeFormat(VolumeName, "\\\\?\\") || VolumeName[index] != '\\') {
			CLTools::ErrorOut("磁盘获取失败！", __FILE__, __LINE__);
			m_pack = CLPackage(COM_GETDRIVE, "");
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
	len = GetLastError();
	if (len != ERROR_NO_MORE_FILES) {
		CLTools::ErrorOut("磁盘获取失败！", __FILE__, __LINE__);
		m_pack = CLPackage(COM_GETDRIVE, "");
		FindVolumeClose(FindDriHandle);
		return;
	}
	m_pack = CLPackage(COM_GETDRIVE, DriInfo.c_str());
	FindVolumeClose(FindDriHandle);
}

void CLComDispose::getFile()
{
	std::string dir = m_pack.GetData();dir += '*';
	WIN32_FIND_DATA fileInfo = {};
	std::string strFileInfos;
	BOOL ret = FALSE;
	HANDLE findFileHandle = FindFirstFile(dir.c_str(), &fileInfo);
	if (findFileHandle == INVALID_HANDLE_VALUE) {
		CLTools::ErrorOut("文件夹打开失败！", __FILE__, __LINE__);
		FILEINFO fileinfo;
		m_pack = CLPackage(COM_GETFILE, &fileinfo);
		return;
	}
	do {
		FILEINFO _fileinfo(fileInfo.cFileName, fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY, fileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN, FALSE);
		strFileInfos += &_fileinfo;
		strFileInfos += ',';
		ret = FindNextFile(findFileHandle, &fileInfo);
		if (strFileInfos.size() > 800) {
			// TODO:封包，Send发送回去
			m_pack = CLPackage(COM_GETFILE, strFileInfos.c_str());
			strFileInfos.clear();
		}
	} while (ret);
	if (GetLastError() != ERROR_NO_MORE_FILES) {
		CLTools::ErrorOut("文件夹读取失败！", __FILE__, __LINE__);
	}
	FILEINFO fileinfo;
	strFileInfos += &fileinfo;
	m_pack = CLPackage(COM_GETFILE, strFileInfos.c_str());
	FindClose(findFileHandle);
}

void CLComDispose::fileDownload()
{
}

void CLComDispose::fileUpload()
{
}

void CLComDispose::remoteDesktop()
{
}

void CLComDispose::systemLock()
{
	m_hLockThread = (HANDLE)_beginthreadex(NULL, 0, sysLockThread, this, 0, &m_lockThreadID);
}

void CLComDispose::systemUnlock()
{
	PostThreadMessage(m_lockThreadID, WM_KEYDOWN, 0x41, 0x001E0001);
	PostMessage((HWND)m_lockThreadID, WM_KEYDOWN, 0x41, 0x001E0001);
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
			if (msg.wParam == 0x41) {
				break;
			}
		}
	}
	m_lockDlg.DestroyWindow(); // Create必须调用
}
