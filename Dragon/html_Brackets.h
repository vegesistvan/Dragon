#pragma once
#include "listctrlex.h"


// CHtmlBrackets dialog

class CHtmlBrackets : public CDialogEx
{
	DECLARE_DYNAMIC(CHtmlBrackets)

public:
	CHtmlBrackets(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHtmlBrackets();

// Dialog Data
	enum { IDD = IDD_HTM_BRACKETS };

	int _square;
	int	_round;
	int	_braces;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString _p;

	TCHAR	_first;
	TCHAR	_last;
	int		_orderix;

	int		_filter1;
	int		_filter2;
	

	CMenu menu;

	CString _caption;
	CString _filterText;

	void CHtmlBrackets::fillTable();
	void OnHtmlEdit();
	void OnHtmlNotepad();

	DECLARE_MESSAGE_MAP()
	DECLARE_EASYSIZE
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFilterMindAll();
	afx_msg void OnFilterMindAg();
	afx_msg void OnFilterMindAdopt();
	afx_msg void OnFilterMindCsalad();
	afx_msg void OnFilterMindOs();
	afx_msg void OnFilterMindTorzs();
	afx_msg void OnFilterMindVonal();
	afx_msg void OnFilterMindOther();
	
	afx_msg void OnFilterFAll();
	afx_msg void OnFilterFTorzs();
	afx_msg void OnFilterFAg();
	afx_msg void OnFilterFOther();

	afx_msg void OnFilterTAll();
	afx_msg void OnFilterTAg();
	afx_msg void OnFilterTVonal();
	afx_msg void OnFilterTOther();

	
	afx_msg void OnFilterDAll();
	afx_msg void OnFilterDAg();
	afx_msg void OnFilterDCsalad();
	afx_msg void OnFilterDTorzs();
	afx_msg void OnFilterDAdopt();
	afx_msg void OnFilterDVonal();
	afx_msg void OnFilterDOs();
	afx_msg void OnFilterDOther();

	
	afx_msg void OnFilterRFamily();
	afx_msg void OnFilterRTable();
	afx_msg void OnRDescAll();
	afx_msg void OnRDescOther();
//	afx_msg void OnRDescParents();
	afx_msg void OnRDescRelatives();
	afx_msg void OnFilterRAll();
//	afx_msg void OnRDescFia();
};
