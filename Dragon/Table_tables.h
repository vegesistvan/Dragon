#pragma once
#include "listctrlex.h"


// CTableTables dialog

class CTableTables : public CDialogEx
{
	DECLARE_DYNAMIC(CTableTables)

public:
	CTableTables(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableTables();

// Dialog Data

	CString m_caption;
	int		m_select;

	CString m_tableHeader;
	CString m_selected;
	CString m_id;
	CString m_filter;
	CString m_select_id;
	CString m_last_name;
	std::vector<CString> v_tableNumbers;

	enum { IDD = IDD_TABLE_TABLES };
	DECLARE_EASYSIZE
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void CTableTables::fillTables();

	CString m_command;
	CString str;
	CListCtrlEx m_ListCtrl;
	
	CString m_condition;
	CString m_conditionText;
	CString m_tableNumber;

	int m_orderix;
	int m_nItem;

	void setRow( int nItem );
	void CTableTables::editUpdate( CString tableNUmber );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEditInsert();
	afx_msg void OnEditDelete();
	afx_msg void OnEditUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
//	afx_msg void OnChangeSearch();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnExportAll();
	afx_msg void OnExportSelected();
	afx_msg void OnUnfilter();
	afx_msg void OnFilterFamily();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGaline();
	afx_msg void OnGahtml();
	afx_msg void OnDescendantsTable();
	afx_msg void OnFilterName();
	afx_msg void OnFilterTables();
//	afx_msg void OnPrivatDescendantsTable();
	afx_msg void OnPrivatDescendantsTable();
//	CColorStatic colorKeres;
//	CColorStatic colorKeres;
	afx_msg void OnClickedKeres();
	CColorStatic colorKeres;
};
