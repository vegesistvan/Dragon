#pragma once
#include "afxdialogex.h"
#include <vector>
#include "uniteStructures.h"
#include "ListCtrlEx.h"

class CUnitedEntriesUnited : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitedEntriesUnited)

public:
	CUnitedEntriesUnited(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUnitedEntriesUnited();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITED_ENTRIES_UNITED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool CUnitedEntriesUnited::inputFile();
	void CUnitedEntriesUnited::push(int i);
	void CUnitedEntriesUnited::pushLine(int i);
	bool CUnitedEntriesUnited::contractSameItems();

	std::vector<UNITE_ENTRIES> vUnite;
	
	UINT	m_rgb[13];
	int		m_numOfColors;

	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	
	CString m_command;
	CString str;
	CStringArray A;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHtmlNotepad();
	afx_msg void OnHtmlEditLines();
	afx_msg void OnCompare2People();

public:
	CWnd* pParent;

	CListCtrlEx m_ListCtrlU;
	std::vector<TCHAR*> vPeople;
	std::vector<TCHAR*> vFiltered;
	virtual BOOL OnInitDialog();

	void atvett();
	afx_msg void OnFilterByName();
	afx_msg void OnUnite2();
	afx_msg void OnCompareAscendants();
	afx_msg void OnFilterByMother();
};
