#pragma once
#include "colorstatic.h"


// CSelectFamilyName dialog

class CSelectFamilyName : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectFamilyName)

public:
	CSelectFamilyName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectFamilyName();

// Dialog Data
	enum { IDD = IDD_SELECTFAMILYNAME };
	CString m_familyName;
	CString str;
	CString	m_familyNumber;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedFamilyname();
	CColorStatic colorFamilyName;
//	CColorStatic colorFamilyNumber;
//	afx_msg void OnClickedFamilynumber();
	CString m_num;
};
