#pragma once
#include "listctrlex.h"


// CCheckMotherDeath dialog

typedef struct
{
	int mother;
	int child;
}MC;

class CCheckMotherDeath : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckMotherDeath)
	// SELECT oszlopok
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
	CCheckMotherDeath(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckMotherDeath();

// Dialog Data
	enum { IDD = IDD_CHECK_MOTHERDEATH };

	CString fileSpec;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int m_cnt;
	int nItem;
	CString m_diff;
	CString m_rowid;
	CString m_line;
	CString m_table;
	CString m_source;
	CString m_united;
	CString m_orderWife;
	CString m_wedding;
	CString m_name;
	CString m_birth;
	CString m_death;

	CString m_columns;

	CString str;
	CListCtrlEx m_ListCtrl;

	void motherDeathChildBirth();
	void listFamily( CString rowid );
	void createColumns();
	CString checkDate( CString datum);

	std::vector<MC> v_MC;
	CString m_command;

	CSqliteDBRecordSet*	 m_F;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	BOOL queryF( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );
	void printHeader( CString title );
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
	
	afx_msg void OnGahtmlLine();
	
	afx_msg void OnList();
};
