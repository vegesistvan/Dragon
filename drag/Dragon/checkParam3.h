#pragma once
#include "afxwin.h"


// CCheckParam3 dialog

class CCheckParam3 : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckParam3)

public:
	CCheckParam3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckParam3();

// Dialog Data
	enum { IDD = IDD_CHECK_PARAM3 };

	CString _caption;
	CString _info;
	BOOL	_html;
	BOOL	_all;
	int		_azonos;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_RadioList;
	CButton m_RadioAll;
	afx_msg void OnBnClickedOk();
	CComboBox m_Combo;
};
