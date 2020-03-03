#pragma once
#include "listctrlex.h"


// CListPeopleAndFamily dialog

class CListPeopleAndFamily : public CDialogEx
{
	DECLARE_DYNAMIC(CListPeopleAndFamily)
	DECLARE_EASYSIZE

public:
	CListPeopleAndFamily(CWnd* pParent = NULL);   // standard constructor
	virtual ~CListPeopleAndFamily();

// Dialog Data
	enum { IDD = IDD_LIST_PEOPLEANDFAMILY };

	int		sex;
	CString fullname;
	CString first_name;
	CString last_name;

	CString fileSpec;
	CString name;
	std::vector<CString> vE;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int nItem;

	FILE* fh1;

	CString str;
	CString m_command;
	CString _contractions;
	CString _explanation;
	CString peopleColumns;
	CString m_fileId;

	void nameParents();
	void printHeader( CString title );
	void printPeople( TCHAR who, int cnt  );

	void createColumns();
	void fillTable();
	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnHtmlShows();

	CString _birth;
	CString _death;
	CString _father;
	CString _mother;
	CString _spouses;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditnotepad();
	afx_msg void OnListPeople();
	afx_msg void OnHtml();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	CListCtrlEx m_ListCtrl;
};
