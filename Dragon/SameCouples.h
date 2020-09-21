#pragma once
#include "listctrlex.h"


// CSameCouples dialog

class CSameCouples : public CDialogEx
{
	DECLARE_DYNAMIC(CSameCouples)
	DECLARE_EASYSIZE


public:
	CSameCouples(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSameCouples();

// Dialog Data
	enum { IDD = IDD_SAME_COUPLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CProgressWnd wndP; 

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

	CString rowidS1_0;
	CString generationS1_0;
	CString sourceS1_0;
	CString unitedS1_0;
	CString lineS1_0;
	CString spouse1_0;
	CString birthS1_0;
	CString deathS1_0;
	CString motherS1_0;

	CString rowidS2_0;
	CString generationS2_0;
	CString sourceS2_0;
	CString unitedS2_0;
	CString lineS2_0;
	CString spouse2_0;
	CString birthS2_0;
	CString deathS2_0;
	CString motherS2_0;

	CListCtrlEx m_ListCtrl;


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
	void data0();
	void printGroup();
	void listGroup( UINT i );
	void CSameCouples::processGroup();
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
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHtml();
	afx_msg void OnLineHusband();
	afx_msg void OnLineWife();
	afx_msg void OnInfo();
};
