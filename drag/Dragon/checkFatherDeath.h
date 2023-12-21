#pragma once
#include "listctrlex.h"


// CCheckFatherDeath9 dialog

class CCheckFatherDeath9 : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckFatherDeath9)

	enum 
	{
	ROWID = 0,
	LINE,
	TABLE,
	SOURCE,
	UNITED,
	LASTNAME,
	FIRSTNAME,
	BIRTH,
	DEATH,
	};
public:
	CCheckFatherDeath9(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckFatherDeath9();

// Dialog Data
	enum { IDD = IDD_CHECK_FATHERDEATH };

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;

	CListCtrlEx m_ListCtrl;

	CString m_diff;
	CString m_rowid;
	CString m_name;
	CString m_line;
	CString m_table;
	CString	m_source;
	CString m_united;
	CString m_birth;
	CString m_death;
	CString m_wedding;
	CString m_orderWife;


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
	void listPeople( int nItem, int who );

	void OnHtmlEditLines();
	void OnHtmlNotepad();
	void OnHtmlNotepadParents();
	void OnHtmlFatherAndSiblings();
	void OnDbEdit();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnList();
};
