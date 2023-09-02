#pragma once


// CLSysLockDialog 对话框

class CLSysLockDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLSysLockDialog)

public:
	CLSysLockDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLSysLockDialog();
	BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SYSLOCK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
