#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CHtmlSpouses dialog

class CHtmlSpouses : public CDialogEx
{
	DECLARE_DYNAMIC(CHtmlSpouses)
	DECLARE_EASYSIZE
public:
	CHtmlSpouses(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHtmlSpouses();

// Dialog Data
	enum { IDD = IDD_HTML_SPOUSES };

	int m_lineNumber;
	int m_tableNumber;
	int m_familyNumber;

	CWnd* m_pParent;
	CString m_tableName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int m_orderix;
	FILE* fh1;
	CString str;

	CColorStatic colorCaption;
	CColorStatic colorKeress;	

	CListCtrlEx m_ListCtrl;
	void CHtmlSpouses::listHtmlLine();
	void CHtmlSpouses::fillTable( );
	BOOL PreTranslateMessage(MSG* pMsg);
	void keress( int start );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNewtable();
	afx_msg void OnClickedStaticSearch();
};
