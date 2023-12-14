#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CCheckSameSpouses dialog

typedef struct
{
	CString rowidM;
	CString rowid;
	CString lineNumber;
	CString tableNumber;
	CString sex_id;
	CString generation;
	CString source;
	CString united;
	CString spouse;
	CString birthDate;
	CString deathDate;
	CString wedding;
	CString father;
	CString mother;
	CString sourceF;
	CString sourceM;
	CString father_id;
	CString mother_id;
	CString birthDateF;
	CString birthDateM;
	CString deathDateF;
	CString deathDateM;
	CString lineNumberF;
	CString lineNumberM;
	int		diff;
}MORESPOUSES;


class CCheckSameSpouses : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckSameSpouses)

public:
	CCheckSameSpouses(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckSameSpouses();

// Dialog Data
	enum { IDD = IDD_CHECK_SAMESPOUSES };
	CListCtrlEx m_ListCtrl;

	CString _fullname;
	CString _lastname;
	CString _firstname;

	CString _birth;
	CString _death;
	CString _father;
	CString _mother;
	CString _spouses;

	bool m_wedding;

protected:

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	CString str;
	CString m_command;
	CString m_explanation;
//	CString fileSpec;
	CString m_caption;

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
	void listBlock();
	
	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	CColorStatic colorNext;
	CString m_search;
	CColorStatic colorKeress;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void createColumns();
	void OnHtmlEdit();
	
	
	void sameSpouses();
	void sameSpouses2();
	void same();
	void keress( int start );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHtml();
	afx_msg void OnInfo();
	afx_msg void OnClickedKeress();
	afx_msg void OnClickedNext();

	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnHtmlEditLines();
	afx_msg void OnHtmlNotepad();
	afx_msg void OnHtmlNotepadParents();
	afx_msg void OnHtmlFatherAndSiblings();
	afx_msg void OnHtmlChildren();
	afx_msg void OnDbEdit();
	afx_msg void On3Generations();


};
