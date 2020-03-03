#pragma once
#include "afxwin.h"


// CHtmlEdit dialog

class CHtmlEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CHtmlEdit)
	DECLARE_EASYSIZE
public:
	CHtmlEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHtmlEdit();

// Dialog Data
	enum { IDD = IDD_HTML_EDITOR };
	CString m_line;
	CString m_caption;
	int		m_lineNumber;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_EditCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeEdit();
};
