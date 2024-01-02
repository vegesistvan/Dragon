#pragma once
#include "afxdialogex.h"
#include <vector>
#include "uniteStructures.h"
#include "ListCtrlEx.h"


// CUnitedEntriesX dialog

class CUnitedEntriesX : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitedEntriesX)

public:
	CUnitedEntriesX(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUnitedEntriesX();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITED_ENTRIES_X };
#endif

	CListCtrlEx m_ListCtrlX;
	std::vector<TCHAR*> vPeople;
	std::vector<TCHAR*> vFiltered;
	std::vector<UNITE_ENTRIES> vUnite;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool CUnitedEntriesX::inputFile();
	void CUnitedEntriesX::push(int i);
	void CUnitedEntriesX::pushLine(int i);
	bool CUnitedEntriesX::contractSameItems();

	

	UINT	m_rgb[13];
	int		m_numOfColors;

	CWnd* pParent;
	
	virtual BOOL OnInitDialog();
	void atvett();

	CString m_command;
	CString str;
	CStringArray A;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHtmlNotepad();
	afx_msg void OnHtmlEditLines();
	afx_msg void OnCompare2People();
	afx_msg void OnFilterByName();
	afx_msg void OnUnite();
	afx_msg void OnCompareAscendants();
	afx_msg void OnFilterByMother();
};
