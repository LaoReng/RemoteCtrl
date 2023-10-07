#pragma once

// CLSockInfoDlg 对话框
class CLSockInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLSockInfoDlg)
public:
	CLSockInfoDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLSockInfoDlg();
	// 获取IP地址
	static CString GetSockIp();
	// 获取端口号
	static short GetSockPort();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SOCKINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	// 服务器ip地址
	CIPAddressCtrl m_SockIp;
	// 服务器端口
	CEdit m_SockPort;
	// 保存服务器ip地址
	static BYTE nIpld0, nIpld1, nIpld2, nIpld3;
	// 保存服务器端口号
	static CString m_port;
public:
	// 点击确定按钮响应函数
	afx_msg void OnBnClickedOk();
};
