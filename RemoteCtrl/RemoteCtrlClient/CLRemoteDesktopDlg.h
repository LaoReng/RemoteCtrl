#pragma once


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

	DECLARE_MESSAGE_MAP()
private:
	// 显示远程桌面控件
	CStatic m_DesktopControl;
public:
	afx_msg void OnBnClickedButLock();
};
