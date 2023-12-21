#pragma once
#include "afxwin.h"
#include "ListBoxEx.h"

// CLanguages dialog

class CLanguages : public CDialogEx
{
	DECLARE_DYNAMIC(CLanguages)

public:
	CLanguages(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLanguages();

// Dialog Data
	enum { IDD = IDD_LANGUAGES };

	CString	m_language;
	int		m_language_id;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CListBoxEx m_ListBox;
	CString str;
	CString m_command;

	void CLanguages::fillTable();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnListBoxMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditUpdate();
	afx_msg void OnEditInsert();
	afx_msg void OnEditDelete();
	afx_msg void OnBnClickedOk();
};
