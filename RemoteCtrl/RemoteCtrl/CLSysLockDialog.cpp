// CLSysLockDialog.cpp: 实现文件
//

#include "pch.h"
#include "RemoteCtrl.h"
#include "CLSysLockDialog.h"
#include "afxdialogex.h"


// CLSysLockDialog 对话框

IMPLEMENT_DYNAMIC(CLSysLockDialog, CDialogEx)

CLSysLockDialog::CLSysLockDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SYSLOCK, pParent)
{

}

CLSysLockDialog::~CLSysLockDialog()
{
}

BOOL CLSysLockDialog::OnInitDialog()
{
	return 0;
}

void CLSysLockDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLSysLockDialog, CDialogEx)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CLSysLockDialog 消息处理程序


void CLSysLockDialog::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MessageBox("按下了键盘");
	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}


void CLSysLockDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MessageBox("按下了键盘");
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CLSysLockDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MessageBox("按下了键盘");
	CDialogEx::OnLButtonDown(nFlags, point);
}
