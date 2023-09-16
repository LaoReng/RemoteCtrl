
// RemoteCtrlClientDlg.h: 头文件
//

#pragma once
#include "CLSockInfoDlg.h"

// CRemoteCtrlClientDlg 对话框
class CRemoteCtrlClientDlg : public CDialogEx
{
// 构造
public:
	CRemoteCtrlClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMOTECTRLCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


private:
	CLSockInfoDlg m_SockInfo; // 网络套接字信息
	// 目录显示控件
	CTreeCtrl m_TreeDrive;
	// 文件列表控件
	CListCtrl m_FileList;
	// 文件列表菜单控件
	CMenu m_menu;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// 获取文件树空间选定的文件目录，并返回当前选定项的句柄
	HTREEITEM GetSelectedDir(CString& str);
	// 删除文件树指定项的子级
	void DeleteTreeChildItem(HTREEITEM hItem);
	// 设置菜单控件文件操作菜单栏的状态
	// IsForbidden：是否禁用菜单，TRUE 禁用菜单 FALSE 启用菜单
	void SetMenuState(BOOL IsForbidden = TRUE);
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	// 点击测试连接按钮，响应函数
	afx_msg void OnBnClickedButTestlink();
	// 点击卷获取按钮，响应函数
	afx_msg void OnBnClickedButGetdrive();
	// 双击文件树里的项，响应函数
	afx_msg void OnNMDblclkTreeDrive(NMHDR* pNMHDR, LRESULT* pResult);
	// 单击文件树里的项，更新文件列表，响应函数
	afx_msg void OnNMClickTreeDrive(NMHDR* pNMHDR, LRESULT* pResult);
	// List Control里用户左键单击了项，响应函数
	afx_msg void OnNMClickListFileinfo(NMHDR* pNMHDR, LRESULT* pResult);
	// 点击网络菜单的设置网络选项，响应函数
	afx_msg void OnSetnetwork();
	// 点击文件菜单的文件下载选项，响应函数
	afx_msg void OnDownloadfile();
	// 点击文件菜单的文件删除选项，响应函数
	afx_msg void OnDeletefile();
	// 点击文件菜单的文件上传选项，响应函数
	afx_msg void OnUploadfile();
};
