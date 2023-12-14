#pragma once
#include "afxdialogex.h"


// CUnitedEntriesErrors dialog

class CUnitedEntriesErrors : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitedEntriesErrors)

public:
	CUnitedEntriesErrors(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUnitedEntriesErrors();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITED_ENTRIES_ERRORS };
#endif

protected:
	CString m_command;
	CString str;
	CStringArray A;
	CStringArray B;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool CUnitedEntriesErrors::inputFile();
	void CUnitedEntriesErrors::copyAtoB();
	void CUnitedEntriesErrors::push(int i);
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHtmlNotepad();
	afx_msg void OnCompare2People();


public:
	CListCtrlEx m_ListCtrlE;
	std::vector<TCHAR*> vPeople;
	virtual BOOL OnInitDialog();
	afx_msg void OnCompareAscendants();
};
