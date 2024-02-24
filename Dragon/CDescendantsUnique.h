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

	FILE* flDesc;
	std::vector<int> vid;
public:
	CListCtrlColoredHeader m_ListCtrlU;
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAllAscendants();
	afx_msg void OnAllDescendants();
	afx_msg void OnNotepad();


};
