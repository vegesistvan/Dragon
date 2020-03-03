#pragma once
#include "afxwin.h"


// CHelp dialog

class CHelp : public CDialogEx
{
	DECLARE_DYNAMIC(CHelp)
	DECLARE_EASYSIZE

public:
	CHelp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHelp();

// Dialog Data
	enum { IDD = IDD_HELP };

	CString m_caption;
	CString m_help;

protected:
	CEdit m_EditCtrl;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

};
