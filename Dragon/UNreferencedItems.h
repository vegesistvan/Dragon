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
L_PEOPLE,
};


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString m_command;
	CString str;
	CListCtrlEx m_ListCtrl;

	void OnHtmlNotepad();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	
};
