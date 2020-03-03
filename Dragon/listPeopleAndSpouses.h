#pragma once
#include "listctrlex.h"


// CListPeopleAndSpouses dialog

class CListPeopleAndSpouses : public CDialogEx
{
	DECLARE_DYNAMIC(CListPeopleAndSpouses)
	DECLARE_EASYSIZE

public:
	CListPeopleAndSpouses(CWnd* pParent = NULL);   // standard constructor
	virtual ~CListPeopleAndSpouses();

// Dialog Data
	enum { IDD = IDD_LIST_PEOPLEANDSPOUSES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;
	CString _fullname;
	CString _firstname;
	CString _lastname;
	CString _explanation;
	CString _contractions;
	CString m_fileId;
	CString fileSpec;
	
	CListCtrlEx m_ListCtrl;

	int		_sex_id;
	int		nItem;
	FILE* fh1;

	std::vector<CString> vE;

	void CListPeopleAndSpouses::peopleMarriages();
	void CListPeopleAndSpouses::printHeader( CString title );
	void CListPeopleAndSpouses::printPeople( CString rowid, CString order, BOOL people, int cnt  );
	void CListPeopleAndSpouses::createColumns();

	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnHtmlShows();
	void OnHtmlPeoplefather();


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditnotepad();
	afx_msg void OnEditNotpadParents();
	afx_msg void OnListPeople();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHtml();

	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
};
