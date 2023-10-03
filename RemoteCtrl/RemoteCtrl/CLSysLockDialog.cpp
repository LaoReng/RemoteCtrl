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
END_MESSAGE_MAP()


