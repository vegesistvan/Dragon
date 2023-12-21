#pragma once
#include "listboxex.h"
#include "afxwin.h"
#include "listctrlex.h"
#include "colorstatic.h"


// CTableFirstnames dialog

class CTableFirstnames : public CDialogEx
{
	DECLARE_DYNAMIC(CTableFirstnames)
	DECLARE_MESSAGE_MAP()
public:
	CTableFirstnames(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableFirstnames();

// Dialog Data
	enum { IDD = IDD_TABLE_FIRSTNAMES };



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString		m_filter;
	CString		m_filterText;
	CString		str;
	CString		m_command;
	int			m_orderix;
	CListCtrlEx m_ListCtrl;
	CColorStatic colorKeress;

	int			m_nItem;
	int			m_sexLast;
	void Update();
	BOOL firstNameExists( CString first_name );
	void fillTable();
	void fillRow( int nItem );
	void keress( int start );
	

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedDelete();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnClickedUpdate();
	afx_msg void OnListTable();
	afx_msg void OnUnfilter();
	afx_msg void OnFilterOccurance();
	afx_msg void OnFilterMan();
	afx_msg void OnFilterWomen();
	afx_msg void OnMan();
	afx_msg void OnBiSex();
	afx_msg void OnWomen();
	afx_msg void OnFilterBisex();
	afx_msg void OnCheckUsed();
	afx_msg void OnFilterCorrect();
	afx_msg void OnReadtxtfile();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditInsert();
	afx_msg void OnFilterLanguage();
	afx_msg void OnClickedKeress();
};
