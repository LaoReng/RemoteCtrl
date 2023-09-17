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
{

}

CLRemoteDesktopDlg::~CLRemoteDesktopDlg()
{
}

void CLRemoteDesktopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STA_DESKTOP, m_DesktopControl);
}


BEGIN_MESSAGE_MAP(CLRemoteDesktopDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUT_LOCK, &CLRemoteDesktopDlg::OnBnClickedButLock)
END_MESSAGE_MAP()


// CLRemoteDesktopDlg 消息处理程序


void CLRemoteDesktopDlg::OnBnClickedButLock()
{
	// TODO: 在此添加控件通知处理程序代码
}
