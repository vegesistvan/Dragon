#pragma once
#include "listctrlex.h"


// CCheckMotherDeath dialog

class CCheckMotherDeath : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckMotherDeath)
	DECLARE_EASYSIZE

public:
	CCheckMotherDeath(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckMotherDeath();

// Dialog Data
	enum { IDD = IDD_CHECK_MOTHERDEATH };

	CString fileSpec;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int m_cnt;
	CString str;
	CListCtrlEx m_ListCtrl;

	void motherDeathChildBirth();
	void createColumns();
	void fillTable();

	void OnHtmlNotepad();
	void OnHtmlEditLines();

	FILE* fh1;
	FILE* fl;

	CString m_command;
	
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
	void printHeader( CString title );



//	void OnHtmlPeoplefather();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnGahtmlLine();
	
	afx_msg void OnList();
};
