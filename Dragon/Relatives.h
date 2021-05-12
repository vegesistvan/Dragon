#pragma once
#include "colorstatic.h"
#include "listboxex.h"
#include "afxwin.h"
#include "listctrlex.h"


// CRelatives dialog

enum
{
	ROWID = 0,
	TABLENUMBER,
	SEXID,
	LASTNAME,
	FIRSTNAME,
	BIRTHPLACE,
	BIRTHDATE,
	DEATHPLACE,
	DEATHDATE,
	PROFESSION,
	COMMENT,
	FATHERID,
	MOTHERID,
	PARENTINDEX,
	COLUMNSCOUNT
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CRelatives : public CDialogEx
{
	DECLARE_DYNAMIC(CRelatives)
	DECLARE_EASYSIZE

enum
{
	L_NAME = 0,
	L_PARENT_INDEX,
	L_ROWID,
	L_PEOPLE,
};

public:
	CRelatives(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRelatives();

// Dialog Data
	enum { IDD = IDD_RELATIVES };
	CString m_rowid;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;
	CString m_columns;
	CString m_rowidFirst;

	UINT	m_numOfSpouses;
	int		m_listCtrlFlag;

	CListCtrlEx m_ListCtrlP;
	CListCtrlEx m_ListCtrlM;
	CListCtrlEx m_ListCtrlC;
	CListCtrlEx m_ListCtrlS;


	CString m_tablenumber;
	CString m_sexid;
	CString m_lastname;
	CString m_firstname;
	CString m_birthplace;
	CString m_birthdate;
	CString m_deathplace;
	CString m_deathdate;
	CString m_profession;
	CString m_comment;
	CString m_father_id;
	CString m_mother_id;
	CString m_name;
	CString m_parent_index;

	CColorStatic colorPeople;
	CColorStatic colorFather;
	CColorStatic colorMother;
	CColorStatic colorMarriages;
	CColorStatic colorChildren;
	CColorStatic colorSiblings;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;


	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	void createColumns( CListCtrlEx* p_ListCtrl );
	BOOL createScreen();
	BOOL createPeople();
	BOOL createFather( CString father_id );
	BOOL createMother( CString mother_id );
	BOOL createSpouses();
	BOOL createChildren();
	BOOL createSiblings();
	CString selectPeople( CString rowid );
	CString pack( CString tag, CString place, CString date );
	void marry( CString father_i, CString mother_id, CString nameF, CString nameM );

	void deleteParents();
	void deleteSpouse( CString rowid );
	void deleteChild( CString rowid );
	void deleteSibling( CString rowid );


	CString whichSpouse( CString father_id );

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditDelete();
	afx_msg void OnEditUpdate();
	afx_msg void OnEditInsert();
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	
	afx_msg void OnAscestor();
	afx_msg void OnDescendants();
	afx_msg void OnReset();
	afx_msg void OnClickedStaticFather();
	afx_msg void OnClickedStaticMother();
	
	afx_msg void OnClickedStaticMarriages();
	afx_msg void OnClickedStaticSiblings();
	afx_msg void OnClickedStaticChildren();
	afx_msg void OnInsertGyerek();

	afx_msg void OnDblclkStaticPeople();
	afx_msg void OnDblclkListCtrlMarriages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListCtrlSiblings(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListCtrlChildren(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListCtrlParents(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnInfo();
	afx_msg void OnEditMarriage();
};
