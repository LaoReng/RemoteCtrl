// CLSockInfoDlg.cpp: 实现文件
//

#include "pch.h"
#include "RemoteCtrlClient.h"
#include "CLSockInfoDlg.h"
#include "afxdialogex.h"


// CLSockInfoDlg 对话框

BYTE CLSockInfoDlg::nIpld0 = 127, CLSockInfoDlg::nIpld1 = 0;
BYTE CLSockInfoDlg::nIpld2 = 0, CLSockInfoDlg::nIpld3 = 1;
CString CLSockInfoDlg::m_port = "7968";

IMPLEMENT_DYNAMIC(CLSockInfoDlg, CDialogEx)

CLSockInfoDlg::CLSockInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SOCKINFO, pParent)
{
	
}

CLSockInfoDlg::~CLSockInfoDlg()
{
}

CString CLSockInfoDlg::GetSockIp()
{	
	CString ip;
	ip.Format("%d.%d.%d.%d", nIpld0, nIpld1, nIpld2, nIpld3);
	return ip;
}

short CLSockInfoDlg::GetSockPort()
{
	return (short)atoi(m_port);
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
	m_SockIp.SetAddress(nIpld0, nIpld1, nIpld2, nIpld3);
	m_SockPort.SetWindowTextA(m_port);
	return TRUE;
}


BEGIN_MESSAGE_MAP(CLSockInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLSockInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLSockInfoDlg 消息处理程序


void CLSockInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SockIp.GetAddress(nIpld0, nIpld1, nIpld2, nIpld3);
	m_SockPort.GetWindowTextA(m_port);
	CDialogEx::OnOK();
}
