#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CCheckSameSpouses dialog


class CCheckSameSpouses : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckSameSpouses)
	DECLARE_EASYSIZE

public:
	CCheckSameSpouses(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckSameSpouses();

// Dialog Data
	enum { IDD = IDD_CHECK_SAMESPOUSES };
	CListCtrlEx m_ListCtrl;

	CString _fullname;
	CString _last_name;
	CString _first_name;

	CString _birth;
	CString _death;
	CString _father;
	CString _mother;
	CString _spouses;


protected:

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	CString str;
	CString m_command;
	CString m_explanation;
	CString	_contractions;
	CString fileSpec;

	FILE* fh1;
	int nItem;
	int	m_cnt;

	CString p_fields;
	CString _tag;
	CString _info;

	std::vector<MORESPOUSES> vSpouses;
	std::vector<MORESPOUSES>vSameSpouses;

	CString name;
	CString rowid;
	CString firstName;
	CString lastName;
	CString lineNumber;
	CString tableNumber;
	CString generation;
	CString source;
	CString united;
	CString birth;
	CString death;
	CString father_id;
	CString mother_id;
	CString father;
	CString mother;
	CString sourceF;
	CString sourceM;
	CString birthDateF;
	CString deathDateF;
	CString birthDateM;
	CString deathDateM;
	CString lineNumberF;
	CString lineNumberM;


	void fillSpouses( CString rowid, CString sex_id );

	void fillSameSpouses();

	void htmlHeader( CString title );
	void printHtml();
	
	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void CCheckSameSpouses::createColumns();
	void CCheckSameSpouses::OnHtmlEdit();
	
	void CCheckSameSpouses::OnHtmlShows();
	void CCheckSameSpouses::OnHtmlPeoplefather();
	

	void sameSpouses();
	void sameSpouses2();
	void same();
	void keress( int start );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);

	void CCheckSameSpouses::OnHtmlNotepad();
	afx_msg void OnHtml();
	afx_msg void OnInfo();
	afx_msg void OnSpousesDiff();
	CString m_search;
	CColorStatic colorKeress;
	afx_msg void OnClickedKeress();
	afx_msg void OnClickedNext();
	CColorStatic colorNext;
	afx_msg void OnEdit2lines();
	afx_msg void OnEditNotepadParents();
};
