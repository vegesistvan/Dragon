#pragma once
#include "listboxex.h"


// CSameParams dialog

class CSameParams : public CDialogEx
{
	DECLARE_DYNAMIC(CSameParams)
	DECLARE_MESSAGE_MAP()
public:
	CSameParams(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSameParams();

	enum { IDD = IDD_SAMEPARAMS };

	CString m_lastName;
	CString m_firstName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_command;
	CString str;
	CStringList m_strList;

	CString		m_sex_id;
	CListCtrlEx m_ListCtrl;

public:
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnCreateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);

};
