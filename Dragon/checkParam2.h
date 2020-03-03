#pragma once
#include "afxwin.h"


// CCheckParam2 dialog

class CCheckParam2 : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckParam2)

public:
	CCheckParam2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckParam2();

// Dialog Data

	enum { IDD = IDD_CHECK_PARAM2 };

	CString _caption;
	CString _info;
	BOOL	_html;
	BOOL	_all;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_RadioList;
	CButton m_RadioAll;
	afx_msg void OnBnClickedOk();
};
