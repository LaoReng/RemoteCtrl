#pragma once
#include "CLRCCliControl.h"

// CLRemoteDesktopDlg 对话框

class CLRemoteDesktopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLRemoteDesktopDlg)

public:
	CLRemoteDesktopDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLRemoteDesktopDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMOTEDESKTOP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	// 显示远程桌面控件
	CStatic m_DesktopControl;
	// 系统锁定/解锁控件变量
	CButton m_LockBut;
	// 控制层实例指针
	CLRCCliControl* m_pControl;
	// 接收屏幕信息缓冲区的指针索引
	int m_index;
	CImage m_image;
public:
	// 点击系统锁按钮
	afx_msg void OnBnClickedButLock();
public:
	// 定时器
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
