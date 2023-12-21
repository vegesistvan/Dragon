#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


class CSpouseSpouses : public CDialogEx
{
	DECLARE_DYNAMIC(CSpouseSpouses)

public:
	CSpouseSpouses(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSpouseSpouses();

// Dialog Data
	enum { IDD = IDD_HTML_SPSP };
	int m_tableNumber;
	int	m_lineNumber;
	int m_familyNumber;
	CString m_tableName;
	CString m_htmlPathName;
	CString m_htmlFileName;
	int m_numberOfTables;

	int	m_orderix;
	CListCtrlEx m_ListCtrl;
	CString m_search;
	CString str;

protected:
	FILE* fh1;

	CColorStatic m_colorCaption;
	CColorStatic colorKeress;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CSpouseSpouses::fillTable( );

	void CSpouseSpouses::keress( int start );
	void CSpouseSpouses::menu();

	void OnHtmlEdit();
	void OnHtmlNotepad();
	DECLARE_MESSAGE_MAP()

public:
	CWnd* m_pParent;

	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnNewtable();
	afx_msg void OnClickedKeress();

};
