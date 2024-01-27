#pragma once
#include "afxdialogex.h"
#include "ListCtrlColoredHeader.h"

// CDescendantsFull dialog

enum
{
	FULL = 0,
	UNIQUE,
	FULL_P,
	UNIQUE_P
};
enum
{
	FULL_TAB = 0,
	UNIQUE_TAB,
};

class CDescendantsFull : public CDialogEx
{
	DECLARE_DYNAMIC(CDescendantsFull)

public:
	CDescendantsFull(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDescendantsFull();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESCENDANTS_FULL };
#endif

protected:
	CString str;
	FILE* flDesc;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	std::vector<int> vid; // leszármazottak

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlColoredHeader m_ListCtrlF;

	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeressBeginning();
	afx_msg void OnKeressNext();
	afx_msg void OnKeressPrevious();
	afx_msg void OnFunctionsNotepad();
	afx_msg void OnDescendantsMarked();
	afx_msg void OnDescendantsListed();
	afx_msg void OnAscendantsListed();
	afx_msg void OnAscendantsMarked();
};
