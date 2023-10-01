// CLRemoteDesktopDlg.cpp: 实现文件
//

#include "pch.h"
#include "RemoteCtrlClient.h"
#include "CLRemoteDesktopDlg.h"
#include "afxdialogex.h"

// CLRemoteDesktopDlg 对话框


IMPLEMENT_DYNAMIC(CLRemoteDesktopDlg, CDialogEx)

CLRemoteDesktopDlg::CLRemoteDesktopDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REMOTEDESKTOP, pParent)
	, m_pControl(NULL)
	, m_index(0)
{}

CLRemoteDesktopDlg::~CLRemoteDesktopDlg()
{
}

void CLRemoteDesktopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STA_DESKTOP, m_DesktopControl);
	DDX_Control(pDX, IDC_BUT_LOCK, m_LockBut);
}
#define TIMERID1 0
BOOL CLRemoteDesktopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	RECT rect = {};
	m_pControl = CLRCCliControl::getInstance();
	m_DesktopControl.GetWindowRect(&rect);
	m_DesktopControl.ScreenToClient(&rect);
	m_DesktopControl.ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
	m_image.Create(rect.right - rect.left, rect.bottom - rect.top, GetDeviceCaps(::GetDC(NULL), BITSPIXEL));
	SetTimer(TIMERID1, 500, NULL); // 设置定时器
	m_pControl->SetPackage(COM_REMOTEDESKTOP);
	INT ret = m_pControl->Send();
	if (ret < 0) {
		MessageBox("请检查网络是否连接！", "错误", MB_OK | MB_ICONERROR);
		CLTools::ErrorOut("数据包发送失败！", __FILE__, __LINE__);
		return FALSE;
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


BEGIN_MESSAGE_MAP(CLRemoteDesktopDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUT_LOCK, &CLRemoteDesktopDlg::OnBnClickedButLock)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CLRemoteDesktopDlg 消息处理程序


void CLRemoteDesktopDlg::OnBnClickedButLock()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_LockBut.GetWindowTextA(str);
	if (str == "锁定") {
		str = "解锁";
	}
	else {
		str = "锁定";
	}
	m_LockBut.SetWindowText(str);
}


void CLRemoteDesktopDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMERID1) {
		HGDIOBJ memOb = GlobalAlloc(GMEM_MOVEABLE, 0);
		if (!memOb)goto end;
		IStream* pStream = NULL;
		if (S_OK != CreateStreamOnHGlobal(memOb, TRUE, &pStream)) {
			GlobalFree(memOb);
			goto end;
		}
		ULONG len = 0;
		do {
			m_index = m_pControl->Recv(FALSE, m_index);
			if (m_pControl->GetPackage().GetCmd() == COM_REMOTEDESKTOP) {
				pStream->Write(m_pControl->GetPackage().GetData(), (ULONG)m_pControl->GetPackage().GetDataSize(), &len);
			}
		} while (m_index > 0 && len > 0);
		CImage image;
		if (image.Load(pStream) < 0) {
			GlobalFree(memOb);
			goto end;
		}
		image.Draw(
			m_image.GetDC()
			, 0, 0, m_image.GetWidth(), m_image.GetHeight()
			, 0, 0, image.GetWidth(), image.GetHeight()
		);
		m_image.ReleaseDC();
		m_DesktopControl.SetBitmap(HBITMAP(m_image));
		GlobalFree(memOb);
	}
end:
	CDialogEx::OnTimer(nIDEvent);
}


void CLRemoteDesktopDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(TIMERID1);
	m_pControl->Close();
}
