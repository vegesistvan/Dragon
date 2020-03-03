#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CTableHtml dialog

class CTableHtml : public CDialogEx
{
	DECLARE_DYNAMIC(CTableHtml)
	DECLARE_EASYSIZE
public:
	CTableHtml(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableHtml();

// Dialog Data
	enum { IDD = IDD_TABLE_HTML };

	CWnd* m_pParent;

	CString m_fileSpec;
	CString m_fileName;
	CString m_substring;
	CString m_familyName;


	int		m_orderix;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	

	CString str;
	CListCtrlEx m_ListCtrl;
	CColorStatic colorCaption;

	void CTableHtml::title();
	void CTableHtml::fillTable();
	void CTableHtml::fillRow( CString item1, CString item2, CString item3 );
	BOOL CTableHtml::isAncestor();
	
	std::vector<TCHAR*> v_table_html; 
//	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnFilterUnfilter();
	afx_msg void OnOpenHtml();
	afx_msg void OnFilterSubstring();
//	afx_msg void OnKillfocusSearh();
	afx_msg void OnDblclkListhtml(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnChangeSearch();
//	afx_msg void OnSetfocusSearch();
	afx_msg void OnChangeSearch();
};
