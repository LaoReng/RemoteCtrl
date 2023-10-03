#pragma once


// CLSockInfoDlg 对话框

class CLSockInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLSockInfoDlg)

public:
	CLSockInfoDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLSockInfoDlg();
	static CString GetSockIp();
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
	static CString m_ip;
	static BYTE nIpld0, nIpld1, nIpld2, nIpld3;
	static CString m_port;
public:
	afx_msg void OnBnClickedOk();
};
