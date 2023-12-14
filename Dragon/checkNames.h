#pragma once
#include "listctrlex.h"
#include "ColorStatic.h"

// CCheckNames dialog

class CCheckNames : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckNames)
	DECLARE_MESSAGE_MAP()

public:
	CCheckNames(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckNames();

// Dialog Data
	enum { IDD = IDD_CHECK_NAMES };

//	bool m_message;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CListCtrlEx m_ListCtrl;

	CString str;
	CString m_command;
	CString cLine;

	CStdioFile gafile;

//	int m_lineNumberPrev;

//	CString getHtmlLine( CString lineNumber );
//	CString cleanHtmlLine( CString cLine );
//	CString getPeople();
//	CString getName();

//	void OnHtmlEditLines();
//	void OnHtmlNotepad();
//	void OnHtmlNotepadParents();
//	void OnHtmlFatherAndSiblings();
//	void OnDbEdit();
	
public:
	virtual BOOL OnInitDialog();
//	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnList();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFirstnames();
private:
	CColorStatic colorAttention;
};
