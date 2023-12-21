#pragma once
#include "listctrlex.h"

// CLifeSpan dialog

class CLifeSpan : public CDialogEx
{
	DECLARE_DYNAMIC(CLifeSpan)

public:
	CLifeSpan(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLifeSpan();

	CString _fileSpec;
	CString _caption;

// Dialog Data
	enum { IDD = IDD_CHECK_LIFESPAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int _lifespan;
	CString m_command;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );

	void createColumns();
	void fillTable();
	CString str;
	void OnHtmlEdit();
	void OnDbEdit();
	void OnHtmlNotepad();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnList();

};
