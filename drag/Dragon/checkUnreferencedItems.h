#pragma once
#include "listctrlex.h"


// CUnreferencedItems dialog

class CUnreferencedItems : public CDialogEx
{
	DECLARE_DYNAMIC(CUnreferencedItems)

public:
	CUnreferencedItems(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUnreferencedItems();

// Dialog Data
	enum { IDD = IDD_UNREFERENCEDITEMS };

enum
{
L_DUMMY = 0,
L_ROWID,
L_LINENUMBER,
L_UNITED,
L_SOURCE,
L_ANCESTRY,
L_PEOPLE,
};


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString m_command;
	CString str;
	CListCtrlEx m_ListCtrl;
	CString m_people;
	CString m_linenumber;
	CString m_source;
	CString m_united;
	CString m_ancestry;

	CString CUnreferencedItems::selectPeople(CString rowid);
	void OnHtmlNotepad();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
};
