#pragma once
#include "colorstatic.h"
#include "listctrlex.h"


// CCheckWeddingDate dialog

class CCheckWeddingDate : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckWeddingDate)

public:
	CCheckWeddingDate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckWeddingDate();

// Dialog Data
	enum { IDD = IDD_CHECK_WEDDING_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	int nItem;
	int	m_cnt;
	CString	_contractions;

	CString _fullname;
	CString _lastname;
	CString _firstname;

	CString _birth;
	CString _death;
	CString _father;
	CString _mother;
	CString _spouses;


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



	void keress( int start );
	BOOL PreTranslateMessage(MSG* pMsg);
	void createColumns();
	void sameSpouses();
	void fillSpouses( CString rowid, CString sex_id );
	void fillSameSpouses( );
	void same();
	void listBlock();


	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_EASYSIZE
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CColorStatic colorKeres;
	CColorStatic colorNext;
	afx_msg void OnClickedKeres();
	afx_msg void OnClickedNext();
	CListCtrlEx m_ListCtrl;
};
