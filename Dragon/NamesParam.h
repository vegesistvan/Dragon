#pragma once
#include "colorstatic.h"
#include "afxwin.h"


// CNamesParam dialog

class CNamesParam : public CDialogEx
{
	DECLARE_DYNAMIC(CNamesParam)

public:
	CNamesParam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNamesParam();

// Dialog Data
	enum { IDD = IDD_NAMESPARAM };

	CString m_fileSpec;
	int		m_language_id;
	CString	m_sex_id;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CColorStatic colorFile;
	CColorStatic colorFileSpec;
	CColorStatic colorLanguage;
	CColorStatic colorStaticLang;
	afx_msg void OnClickedFile();
	afx_msg void OnClickedStaticLang();
	CComboBox m_ComboCtrl;
	afx_msg void OnBnClickedOk();
};
