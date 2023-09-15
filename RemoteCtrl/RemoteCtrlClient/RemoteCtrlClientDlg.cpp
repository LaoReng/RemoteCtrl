﻿
// RemoteCtrlClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RemoteCtrlClient.h"
#include "RemoteCtrlClientDlg.h"
#include "afxdialogex.h"
#include "CLRCCliControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRemoteCtrlClientDlg 对话框



CRemoteCtrlClientDlg::CRemoteCtrlClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REMOTECTRLCLIENT_DIALOG, pParent)
	, m_SockInfo(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteCtrlClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DRIVE, m_TreeDrive);
	DDX_Control(pDX, IDC_LIST_FILEINFO, m_FileList);
}

BEGIN_MESSAGE_MAP(CRemoteCtrlClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CHAR()
	ON_BN_CLICKED(IDOK, &CRemoteCtrlClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUT_TESTLINK, &CRemoteCtrlClientDlg::OnBnClickedButTestlink)
	ON_BN_CLICKED(IDC_BUT_GETDRIVE, &CRemoteCtrlClientDlg::OnBnClickedButGetdrive)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_DRIVE, &CRemoteCtrlClientDlg::OnNMDblclkTreeDrive)
	ON_NOTIFY(NM_CLICK, IDC_TREE_DRIVE, &CRemoteCtrlClientDlg::OnNMClickTreeDrive)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILEINFO, &CRemoteCtrlClientDlg::OnNMRClickListFileinfo)
END_MESSAGE_MAP()


// CRemoteCtrlClientDlg 消息处理程序

BOOL CRemoteCtrlClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE); // 窗口最大化显示
	ShowWindow(SW_SHOW);
	//ShowWindow(SW_MINIMIZE); // 窗口最小化

	// TODO: 在此添加额外的初始化代码
	/*// 非模态对话框
	m_SockInfo.Create(IDD_DLG_SOCKINFO, this);
	m_SockInfo.ShowWindow(SW_HIDE);*/

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRemoteCtrlClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRemoteCtrlClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRemoteCtrlClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HTREEITEM CRemoteCtrlClientDlg::GetSelectedDir(CString& str)
{
	CPoint ptMouse;
	GetCursorPos(&ptMouse);
	m_TreeDrive.ScreenToClient(&ptMouse);
	HTREEITEM hTree = m_TreeDrive.HitTest(ptMouse, 0);
	if (hTree == NULL)
		return NULL;
	HTREEITEM hParent = hTree;
	str = m_TreeDrive.GetItemText(hParent);
	str += "\\";
	do {
		hParent = m_TreeDrive.GetParentItem(hParent);
		if (hParent) {
			str = m_TreeDrive.GetItemText(hParent) + "\\" + str;
		}
	} while (hParent);
	return hTree;
}

void CRemoteCtrlClientDlg::DeleteTreeChildItem(HTREEITEM hItem)
{
	if (m_TreeDrive.ItemHasChildren(hItem)) {
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_TreeDrive.GetChildItem(hItem);
		while (hChildItem != NULL) {
			hNextItem = m_TreeDrive.GetNextItem(hChildItem, TVGN_NEXT);
			m_TreeDrive.DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
}

void CRemoteCtrlClientDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}


void CRemoteCtrlClientDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	/*//非模态对话框
	m_SockInfo.CenterWindow();
	m_SockInfo.ShowWindow(SW_SHOW);*/
	m_SockInfo.DoModal();
}


void CRemoteCtrlClientDlg::OnBnClickedButTestlink()
{
	CLRCCliControl* pControl = CLRCCliControl::getInstance();
	pControl->SetPackage(COM_TESTCONNECT);
	if (pControl->Send() > 0) {
		pControl->Recv();
		if (pControl->GetPackage().GetCmd() == COM_TESTCONNECT) {
			//获取对话框句柄 GetSafeHwnd()
			//MessageBox("成功连接", "成功", MB_OK | MB_ICONINFORMATION);
			MessageBox("成功连接    ", "成功", MB_OK | MB_USERICON);
			return;
		}
	}
	// 连接失败
	MessageBox("连接失败", "失败", MB_OK | MB_ICONERROR);
}


void CRemoteCtrlClientDlg::OnBnClickedButGetdrive()
{
	m_FileList.DeleteAllItems();
	CLRCCliControl* pControl = CLRCCliControl::getInstance();
	pControl->SetPackage(COM_GETDRIVE);
	if (pControl->Send() > 0) {
		pControl->Recv();
		CLPackage pack = pControl->GetPackage();
		if (pack.GetCmd() == COM_GETDRIVE) {
			m_TreeDrive.DeleteAllItems();
			const char* str = pack.GetData();
			if (!str) {
				CLTools::ErrorOut("未获取到数据！", __FILE__, __LINE__);
				return;
			}
			CString drive;
			for (int i = 0; str[i] != '\0'; i++) {
				if (str[i] == ',')
					continue;
				drive.Format("%c:", str[i]);
				//HTREEITEM hPA = m_TreeDrive.InsertItem(drive);
				m_TreeDrive.InsertItem(drive, TVI_ROOT, TVI_LAST);
			}
		}
	}
}

void CRemoteCtrlClientDlg::OnNMDblclkTreeDrive(NMHDR* pNMHDR, LRESULT* pResult)
{

	*pResult = 0;
	CString str;
	HTREEITEM hTree = GetSelectedDir(str);
	if (hTree == NULL) {
		CLTools::ErrorOut("未选中控件!", __FILE__, __LINE__);
		return;
	}
	DeleteTreeChildItem(hTree);
	m_FileList.DeleteAllItems();
	CLRCCliControl* pControl = CLRCCliControl::getInstance();
	pControl->SetPackage(COM_GETFILE, str);
	INT ret = pControl->Send();
	if (ret < 0) {
		CLTools::ErrorOut("数据包发送失败！", __FILE__, __LINE__);
	}
	INT index = 0;
	INT nCol = 0;
	INT islast = 0;
	FILEINFO finfo;
	do {
		index = pControl->Recv(FALSE, index);
		if (index < 0)
			break;
		CLPackage& pack = pControl->GetPackage();
		if (pack.GetCmd() == COM_GETFILE) {
			char* finfos = (char*)pack.GetData();
			size_t finfosSize = pack.GetDataSize();
			for (int i = 0; i < finfosSize / FILEINFO::getSize(); i++) {
				finfo = (finfos + FILEINFO::getSize() * i);
				if (finfo.m_isLast) {
					islast = 1;
					break;
				}
				if (finfo.m_isDir) {
					m_TreeDrive.InsertItem(finfo.m_fileName, hTree, TVI_LAST); // TVI_SORT按字母顺序插入
				}
				else {
					//m_FileList.InsertItem(nCol++, finfo->m_fileName);
					m_FileList.InsertItem(nCol++, finfo.m_fileName);
				}
			}
		}
	} while (!islast || index >= 0);
	pControl->Close();
	m_TreeDrive.Expand(hTree, TVE_EXPAND); // 展开子项
}


void CRemoteCtrlClientDlg::OnNMClickTreeDrive(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	CString str;
	HTREEITEM hTree = GetSelectedDir(str);
	if (!m_TreeDrive.ItemHasChildren(hTree)) { // 不存在子级，就不需要更新文件列表
		return;
	}
	m_FileList.DeleteAllItems();
	CLRCCliControl* pControl = CLRCCliControl::getInstance();
	pControl->SetPackage(COM_GETFILES, str);
	INT ret = pControl->Send();
	if (ret < 0) {
		CLTools::ErrorOut("数据包发送失败！", __FILE__, __LINE__);
	}
	INT index = 0;
	INT nCol = 0;
	INT islast = 0;
	do {
		index = pControl->Recv(FALSE, index);
		if (index < 0)
			break;
		CLPackage& pack = pControl->GetPackage();
		if (pack.GetCmd() == COM_GETFILES) {
			char* finfos = (char*)pack.GetData();
			size_t finfosSize = pack.GetDataSize();
			for (int i = 0; i < finfosSize / FILEINFO::getSize(); i++) {
				PFILEINFO finfo = (PFILEINFO)(finfos + FILEINFO::getSize() * i);
				if (finfo->m_isLast) {
					islast = 1;
					break;
				}
				m_FileList.InsertItem(nCol++, finfo->m_fileName);
			}
		}
	} while (!islast || index >= 0);
	pControl->Close();
}

void CRemoteCtrlClientDlg::OnNMRClickListFileinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (m_FileList.GetSelectedCount() <= 0) // 判断当前是否有选定项
		return;
	CString str = m_FileList.GetItemText(m_FileList.GetSelectionMark(), 0); // 获取右键选定的项
	CMenu m_menu;
	m_menu.LoadMenuA(IDR_MENU_FILEDIS);
	SetMenu(&m_menu);
	m_menu.TrackPopupMenu(TPM_LEFTALIGN, 255, 255, GetWindow(GW_CHILD));
}
