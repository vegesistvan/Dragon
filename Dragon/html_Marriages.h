#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CMarriages dialog

class CMarriages : public CDialogEx
{
	DECLARE_DYNAMIC(CMarriages)
	DECLARE_EASYSIZE

public:
	CMarriages(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMarriages();

// Dialog Data
	enum { IDD = IDD_HTML_SPOUSES };

	CWnd* m_pParent;

	int m_orderix;

	int m_familyNumber;
	int m_tableNumber;
	int m_lineNumber;

	CString m_tableName;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	FILE* fh1;

	CString str;
	CString m_filter;
	CString m_filterText;

	CListCtrlEx m_ListCtrl;
	CColorStatic m_colorCaption;
	void	menu();
	void	fillTable( );
	
	BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNewtable();
	afx_msg void OnFilter();
	afx_msg void OnUnfilter();
	afx_msg void OnList();
	afx_msg void OnChangeSearch();
	
};
