#pragma once
#include "afxwin.h"
#include "colorstatic.h"


// CEditFirstName dialog

class CEditFirstName : public CDialogEx
{
	DECLARE_DYNAMIC(CEditFirstName)

public:
	CEditFirstName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditFirstName();

// Dialog Data
	enum { IDD = IDD_EDIT_FIRSTNAME };

	CString m_rowid;
	BOOL	m_uj;
	int		m_sexLast;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;
	int		m_language_id;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CComboBox m_ComboCtrl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedLanguage();
	CColorStatic colorLanguage;
	afx_msg void OnClickedStaticLanguage();
	CColorStatic colorStatic;
//	afx_msg void OnClickedSaveAndNew();

	int m_RadioSex;
};
