#pragma once
#include "afxdialogex.h"
#include "ListCtrlColoredHeader.h"

// CDescendantsUnique dialog

class CDescendantsUnique : public CDialogEx
{
	DECLARE_DYNAMIC(CDescendantsUnique)

public:
	CDescendantsUnique(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDescendantsUnique();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESCENDANTS_UNIQUE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;

	DECLARE_MESSAGE_MAP()

public:
	CListCtrlColoredHeader m_ListCtrlU;
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeressBeginning();
	afx_msg void OnKeressNext();
	afx_msg void OnKeressPrevious();
	afx_msg void OnFunctionsDescendands();
	afx_msg void OnFunctionsAscendants();
	afx_msg void OnFunctionsNotepad();
};
