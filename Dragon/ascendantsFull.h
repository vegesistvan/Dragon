#pragma once
#include "afxdialogex.h"
#include "ListCtrlColoredHeader.h"


// CAscendantsFull dialog

class CAscendantsFull : public CDialogEx
{
	DECLARE_DYNAMIC(CAscendantsFull)

public:
	CAscendantsFull(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAscendantsFull();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASCENDANTS_FULL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;

	FILE* flDesc;
	std::vector<int> vid;

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlColoredHeader m_ListCtrlF;
	virtual BOOL OnInitDialog();

	void CAscendantsFull::listTable();
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFunctionsNotepad();
	afx_msg void OnAllAscendants();
	afx_msg void OnAllDescendants();
	afx_msg void OnDblclkListFull(NMHDR* pNMHDR, LRESULT* pResult);
};
