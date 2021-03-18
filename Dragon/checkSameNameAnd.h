#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CcheckSameNameAnd dialog

class CcheckSameNameAnd : public CDialogEx
{
	DECLARE_DYNAMIC(CcheckSameNameAnd)
	DECLARE_EASYSIZE

public:
	CcheckSameNameAnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CcheckSameNameAnd();

// Dialog Data
	enum { IDD = IDD_CHECK_SAMENAMEAND };

	CString fileSpec;
	CString and;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CListCtrlEx m_ListCtrl;

	CString _extension;
	CString str;
	CString m_command;
	FILE* txt;
	FILE* fh1;

	int		_cnt;
	int		nItem;

	CString _fullname;
	CString _lastname;
	CString _firstname;

	CString	_contractions;

	std::vector<SAME> vSame;



	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;



	void CcheckSameNameAnd::createColumns();
	void CcheckSameNameAnd::fillTable();

	void CcheckSameNameAnd::SameNameAndMother();
	void CcheckSameNameAnd::SameNameAndFather();
	void CcheckSameNameAnd::SameNameAndDeath();
	void CcheckSameNameAnd::SameNameAndBirth();

	BOOL CcheckSameNameAnd::sameNameParent( int PARENT_ID );
	BOOL CcheckSameNameAnd::sameNameDate( int ID_DATE );
	BOOL CcheckSameNameAnd::sameSpouses();


	void OnHtmlEditLines();
	void OnHtmlNotepad();
	void OnHtmlFamily();
	void OnDbEdit();
	void OnHtmlFatherAndSiblings();
	void keress( int start );
/*
	void CcheckSameNameAnd::OnHtmlPeoplefather();
	void CcheckSameNameAnd::OnHtmlEditLines();
	void CcheckSameNameAnd::OnHtmlNotepad();
	void OnEditNotpadParents();
	void OnEdit2lines();
*/
	void CcheckSameNameAnd::init( CString title );
	void CcheckSameNameAnd::listSameVector();
	

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnHtml();
	afx_msg void OnEditNotepadParents();
	afx_msg void OnFatherandsiblings();
	afx_msg void OnClickedKeres();
	afx_msg void OnClickedNext();
	CColorStatic colorKeres;
	CColorStatic colorNext;
};
