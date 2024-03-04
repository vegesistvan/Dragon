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

	int p_descendantName;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESCENDANTS_FULL };
#endif


protected:
	CString str;
	FILE* flDesc;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	std::vector<int> vid; // leszármazottak

	void CDescendantsFull::onDescendants(int print);
	void CDescendantsFull::OnDescendants2();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrlColoredHeader m_ListCtrlF;

	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAllAscendants();
	afx_msg void OnAllDescendants();
	afx_msg void OnAscendants();
	afx_msg void OnDescendants();
	afx_msg void OnNotepad();
};
