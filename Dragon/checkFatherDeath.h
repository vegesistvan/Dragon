#pragma once
#include "listctrlex.h"


// CCheckFatherDeath9 dialog

class CCheckFatherDeath9 : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckFatherDeath9)
	DECLARE_EASYSIZE

public:
	CCheckFatherDeath9(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckFatherDeath9();

// Dialog Data
	enum { IDD = IDD_CHECK_FATHERDEATH };

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	FILE* fl;


	int		m_cnt;

	CString str;
	CString m_command;
	CString m_fileSpec;

	CListCtrlEx m_ListCtrl;

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

	void createColumns();
	void fatherDeathChildBirth();
	void fillTable();

	void OnHtmlEditLines();
	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnDbEdit();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnList();
};
