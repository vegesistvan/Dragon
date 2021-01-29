#pragma once
#include "listctrlex.h"
#include "afxwin.h"
#include "ProgressWnd.h"


// CContractCouples

class CContractCouples : public CWnd
{
	DECLARE_DYNAMIC(CContractCouples)

public:
	CContractCouples();
	virtual ~CContractCouples();
	void contractCouples();
protected:
	CProgressWnd wndP; 

	CString m_head;
	CString m_description;
	CString m_columns;

	
	CString	_husband;		// csak ezt a házaspárt vizsgájuk!!
	CString _wife;
	int		m_loop;
	int		m_loopMax;

	CString str;
	CString m_command;
	CString m_info;
	CString m_explanation;
	CString	_contractions;
	CString unitedSpec;
	CString differentSpec;
	CString p_fields;
	bool	m_contract;			// végrehajtsa-e az összevonást
	CStringArray m_colors;

	FILE* fU;
	FILE* fD;

	FILE* textU;
	FILE* textD;

	CString m_fileSpecTextU;
	CString m_fileSpecTextD;
	CString sWHITE;

	UINT	ic;			// vCouples indexe
	UINT	m_numOfGroups;
	bool	m_contracted;
	int		m_azonos;

//	CComboBox ComboCtrl;
	
	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;

	std::vector<COUPLES> vCouples;
	std::vector<COUPLES> vSame;
	std::vector<COUPLES> vWife;
	std::vector<CONTRACT> vContract;

	COUPLES r;

	void openUnited();
	void openDifferent();

	void createHead( CString title );
	void core( int loop );
	void getSameCouples( int loop );
	
	void resetRef();
	void setRef( int i );
	void processSame();
	void listDiff();
	void listUnited();

	void contract();
	void contractFull( int loop );
	void deleteMarriages( int loop );
	void printYellow( UINT i );
	void emptyLine( FILE* fl );
	bool identical( UINT i1, UINT i2 );

	void ReplaceSpouse1( CString rowid, CString rowidBy, CString name, int source );
	void ReplaceSpouse2( CString rowid, CString rowidBy, CString name, int source );


	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );

	DECLARE_MESSAGE_MAP()
};


