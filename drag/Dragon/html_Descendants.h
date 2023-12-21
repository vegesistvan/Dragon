#pragma once
#include "listctrlex.h"
#include "colorstatic.h"

// CHtmlDescendants dialog

class CHtmlDescendants : public CDialogEx
{
	DECLARE_DYNAMIC(CHtmlDescendants)
public:
	CHtmlDescendants(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHtmlDescendants();
	enum { IDD = IDD_HTML_DESCENDANTS };

	int m_lineNumber;
	int m_tableNumber;
	int m_familyNumber;

	CWnd* m_pParent;
	CString m_tableName;
	CString m_htmlPathName;
	CString m_htmlFileName;
	int m_numberOfTables;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int m_orderix;

	FILE* fh1;
	CString str;
	CColorStatic colorCaption;

	void CHtmlDescendants::listHtmlLine();
	void CHtmlDescendants::fillTable( );
	void keress();
	void OnHtmlEdit();
	void OnHtmlNotepad();
	CListCtrlEx m_ListCtrl;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNewtable();
	afx_msg void OnDList();

	afx_msg void OnClickedKeress();
};
