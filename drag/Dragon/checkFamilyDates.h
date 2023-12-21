#pragma once
#include "listctrlex.h"
#include "colorstatic.h"
#include "ProgressWnd.h"

typedef struct
{
	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString birthP;			// birth place
	CString death;
	CString deathP;
	CString birthC;			// empty, ha nem szabályos a dátum, vagy a szabályos dátum
	CString	deathC;			// empty, ha nem szabályos a dátum, vagy a szabályos dátum
	CString age;
	CString fatherAge;
	CString motherAge;
	int		numOfSpouses;
	CString message;
	CString comment;
	int		yellow;
}HUSBAND;

typedef struct
{
	CString rowidH;

	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString birthP;
	CString death;
	CString deathP;
	CString birthC;
	CString deathC;
	CString	age;
	CString fatherAge;
	CString motherAge;
	CString wedding;
	CString weddingC;
	int		motherOrder;
	CString message;
	CString comment;
	int		yellow;
}WIFES;


typedef struct
{
	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString birthP;
	CString death;
	CString deathP;
	CString birthC;
	CString deathC;
	CString	age;
	CString fatherAge;
	CString motherAge;
	CString father_id;
	CString mother_id;
	int		mother_index;
	CString message;
	CString comment;
	int		yellow;
}CHILDREN;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCheckFamilyDates : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckFamilyDates)

public:
	CCheckFamilyDates(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckFamilyDates();

// Dialog Data
	enum { IDD = IDD_CHECK_FAMILYDATES };
	bool m_all;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


	CHARFORMAT cf = { 0 };
	
	_int64 oneyear;

	CString m_title;
	CString m_info;
	CString m_infoFamilies;
	CString m_command;
	CString str;
	CString m_filespec;
	bool m_filtered;


	CProgressWnd wndP;

	CListCtrlEx m_ListCtrl;
	CColorStatic colorKeres;
	CColorStatic colorNext;

	std::vector<WIFES> vWifes;
	std::vector<CHILDREN> vChildren;
	std::vector<TCHAR*> vList;
	std::vector<TCHAR*> vListFiltered;

	UINT	m_rgb[11];

	HUSBAND h;
	WIFES w;
	CHILDREN c;

	// átvett adatok
    int	m_maxLifespan;
	int	m_maxAgeHAtWedd;
	int	m_minAgeHAtWedd;
	int	m_maxAgeWAtWedd;
	int	m_minAgeWAtWedd;
	int	m_maxDiffFC;
	int	m_minDiffFC;
	int	m_minDiffMC;
	int	m_maxDiffMC;

	int m_lifespan;

	int m_cnt;

	bool collectFamily();

	void createColumns();
	void printFamily();
	void emptyLine();
	
	void createColumnsAll();
	void printFamilyAll();
	void emptyLineAll();

	void checkFamily();
	void push( CString item );
	void keress( int start );

	int  pushBlock( UINT i, int L_REF );
	void filter( CString caption, CString azon );

	CString checkDate( CString datum);

	void OnHtmlEditLines();
	void OnHtmlNotepad();
	void OnHtmlNotepadParents();
	void OnHtmlFatherAndSiblings();
	void OnDbEdit();
	void CCheckFamilyDates::richText(CString caption);

	void OnEditDatabase();
	void On3Generations();

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;


public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnClickedKeress();
	afx_msg void OnClickedNext();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInfo();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnParameters();
	afx_msg void OnInfoFamilies();
	afx_msg void OnHtmlChildren();

	afx_msg void OnBornAfterFatherDeath9();
	afx_msg void OnFilterUnfiltered();
	afx_msg void OnFilterMotherage();
	afx_msg void OnFilterBeforefather();
	afx_msg void OnDeathBirth();
	afx_msg void OnFilterBirth();
	afx_msg void OnFilterDeath();
	afx_msg void OnFilterWedding();
	afx_msg void OnBornAftreHusbandDeath();
	afx_msg void OnBornBeforeMotherBirth();
	afx_msg void OnDiedBeforeMotherBirth();
	afx_msg void OnBornaftermotherdeath();
	afx_msg void OnLifespan();

//	CRichEditCtrl m_captionCtrl;
	CRichEditCtrl captionCtrl;
};
