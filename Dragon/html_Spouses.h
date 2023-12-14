#pragma once
#include "listctrlex.h"
#include "colorstatic.h"

// CHtmlSpouses dialog

class CHtmlSpouses : public CDialogEx
{
	DECLARE_DYNAMIC(CHtmlSpouses)


public:
	CHtmlSpouses(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHtmlSpouses();

// Dialog Data
	enum { IDD = IDD_HTML_MARRIAGES };

	CWnd* m_pParent;

	int m_orderix;

	int m_familyNumber;
	int m_tableNumber;
	int m_lineNumber;
	int m_numberOfTables;

	CString m_tableName;
	CString m_htmlPathName;
	CString m_htmlFileName;
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
//	virtual void PostNcDestroy();

	void CHtmlSpouses::OnMarriagesFile();
	void CHtmlSpouses::OnMarriagesTable();
	void CHtmlSpouses::OnMarriagesLine();
	void OnHtmlEdit();
	void OnHtmlNotepad();
	void keress( int start );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNewtable();
	afx_msg void OnFilter();
	afx_msg void OnUnfilter();
	afx_msg void OnList();
	CColorStatic colorKeress;
	afx_msg void OnClickedKeress();
};
