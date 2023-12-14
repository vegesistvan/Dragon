#pragma once
#include "afxdialogex.h"


// CCheckParents dialog

class CCheckParents : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckParents)

public:
	CCheckParents(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCheckParents();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECK_PARENTS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;
	CStdioFile notepad;


	
	BOOL CCheckParents::fillTable();
	CString CCheckParents::getHtmlLine(CString lineNumber);
	CString CCheckParents::cleanHtmlLine(CString cLine);
	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
};
