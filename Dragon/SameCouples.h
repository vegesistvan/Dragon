#pragma once
#include "listctrlex.h"
#include "afxwin.h"


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

	CString m_head;
	CString m_description;
	CString m_columns;

	
	CString	_husband;		// csak ezt a házaspárt vizsgájuk!!
	CString _wife;

	CString str;
	CString m_command;
	CString m_info;
	CString m_explanation;
	CString	_contractions;
	CString unitedSpec;
	CString differentSpec;
	CString p_fields;

	CStringArray m_colors;

	FILE* fU;
	FILE* fD;

	int m_deleted;

	UINT	ic;			// vCouples indexe
	UINT	m_numOfGroups;
	int		m_contracted;
	int		_azonos;

	CComboBox ComboCtrl;
	
	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;

	std::vector<COUPLES> vCouples;
	std::vector<COUPLES> vSame;
	std::vector<COUPLES> vWife;
	std::vector<CString> vDeleted;
	COUPLES r;

	void openUnited();
	void openDifferent();

	void createHead( CString title );
	void sameSpouses();
	void getSameCouples();
	
	void resetRef();
	void setRef( int i );
	void processSame();
	void listDiff();
	void listUnited();

	UINT getNumOfGroups();
	void contract();
	void printYellow( UINT i );

	bool identical( UINT i1, UINT i2 );

	void ReplaceSpouse1( CString rowid, CString rowidBy, CString name, int source );
	void ReplaceSpouse2( CString rowid, CString rowidBy, CString name, int source );


	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

};
