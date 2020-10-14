#pragma once
#include "afxwin.h"


// CParam1 dialog

class CParam1 : public CDialogEx
{
	DECLARE_DYNAMIC(CParam1)

public:
	CParam1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParam1();

// Dialog Data
	enum { IDD = IDD_PARAM1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboCtrl;
	CString m_text;
	CString m_info;
	CEdit	m_edit;
	CString m_caption;
	int		_azonos;
	int		_default;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
