// CLSockInfoDlg.cpp: 实现文件
//

#include "pch.h"
#include "RemoteCtrlClient.h"
#include "CLSockInfoDlg.h"
#include "afxdialogex.h"


// CLSockInfoDlg 对话框

IMPLEMENT_DYNAMIC(CLSockInfoDlg, CDialogEx)

CLSockInfoDlg::CLSockInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SOCKINFO, pParent)
{
	
}

CLSockInfoDlg::~CLSockInfoDlg()
{
}

void CLSockInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADD_SERIP, m_SockIp);
	DDX_Control(pDX, IDC_EDIT_PORT, m_SockPort);
}

BOOL CLSockInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetBackgroundColor(RGB(58,121,163));

	m_SockIp.SetAddress(127, 0, 0, 1);
	m_SockPort.SetWindowTextA("9527");
	return TRUE;
}


BEGIN_MESSAGE_MAP(CLSockInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CLSockInfoDlg 消息处理程序
