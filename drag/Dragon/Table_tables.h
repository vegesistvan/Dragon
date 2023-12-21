#pragma once
#include "listctrlex.h"
#include "ColorStatic.h"

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
	CString m_familyNumber;
	std::vector<CString> v_tableNumbers;

	enum { IDD = IDD_TABLE_TABLES };
//	DECLARE_EASYSIZE
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void CTableTables::fillTables();

	CColorStatic colorKeres;

	CString m_command;
	CString str;
	CListCtrlEx m_ListCtrl;
	
	CString m_condition;
	CString m_conditionText;
	CString m_tableNumber;

	int m_orderix;
	int m_nItem;
	CString m_namePrev;
	int		m_nItemPrev;
	bool	m_colored;
	bool	m_same;

	std::vector<int> vColor;

	void setRow( int nItem );
	void editUpdate( CString tableNUmber );

	void keress( int start );
	void OnHtmlEdit();
	void OnHtmlNotepad();
	void returnSelected();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEditInsert();
	afx_msg void OnEditDelete();
	afx_msg void OnEditUpdate();
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
	afx_msg void OnFilterName();
	afx_msg void OnFilterTables();
	afx_msg void OnClickedKeress();
	afx_msg void OnDescendantsTable();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);

};
