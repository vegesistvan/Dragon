#pragma once
#include "listctrlex.h"


// CSameSpouses dialog

class CSameSpouses : public CDialogEx
{
	DECLARE_DYNAMIC(CSameSpouses)
	DECLARE_EASYSIZE


public:
	CSameSpouses(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSameSpouses();

// Dialog Data
	enum { IDD = IDD_SAMESPOUSES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CProgressWnd wndP; 

	CString _fullname;
	CString _last_name;
	CString _first_name;

	CString _birth;
	CString _death;
	CString _father;
	CString _mother;
	CString _spouses;

	CString str;
	CString m_command;
	CString m_explanation;
	CString	_contractions;
	CString fileSpec;

	FILE* fh1;
	int nItem;
	int	m_cnt;
	int m_deleted;

	CString p_fields;
	CString _tag;

	CString m_rowid;
	CString m_sex_id;


	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	std::vector<COUPLES> vCouples;
	std::vector<COUPLES> vGroup;

	void createColumns();
	void sameSpouses();
	void fillTable();
	void listCtrlLine( UINT i );
	void printGroup();
	void listGroup( UINT i );
	void CSameSpouses::processGroup();
	void htmlHeader( CString title );


	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CListCtrlEx m_ListCtrl;
	afx_msg void OnHtml();
};
