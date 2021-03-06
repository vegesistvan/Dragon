#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CDescendant dialog

class CDescendant : public CDialogEx
{
	DECLARE_DYNAMIC(CDescendant)
	DECLARE_EASYSIZE
public:
	CDescendant(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDescendant();
	enum { IDD = IDD_HTML_DESCENDANTS };

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

	void CDescendant::listHtmlLine();
	void CDescendant::fillTable( );
	void keress();
	void OnHtmlEdit();
	void OnHtmlNotepad();
	CListCtrlEx m_ListCtrl;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNewtable();
	afx_msg void OnDList();

	afx_msg void OnClickedKeress();
};
