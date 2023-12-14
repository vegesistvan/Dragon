#pragma once
#include "afxdialogex.h"


// CUnitedEntriesDifferent dialog

class CUnitedEntriesDifferent : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitedEntriesDifferent)

public:
	CUnitedEntriesDifferent(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUnitedEntriesDifferent();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITED_ENTRIES_DIFFERENT };
#endif

protected:
	CString m_command;
	CString str;
	CStringArray A;

	
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool CUnitedEntriesDifferent::inputFile();
	void CUnitedEntriesDifferent::push(int i);
	void CUnitedEntriesDifferent::pushLine(int i);
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHtmlNotepad();
	afx_msg void OnCompare2People();


public:
	CListCtrlEx m_ListCtrlD;
	std::vector<TCHAR*> vPeople;
	std::vector<TCHAR*> vFiltered;
	virtual BOOL OnInitDialog();

	afx_msg void OnCompareAscendants();
	afx_msg void OnFilterByName();
	afx_msg void OnUnite();
};
