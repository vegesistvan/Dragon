#pragma once
#include "listctrlex.h"
#include "ProgressWnd.h"
#include "afxwin.h"

// CSamePeople dialog

class CSamePeople : public CDialogEx
{
	DECLARE_DYNAMIC(CSamePeople)

public:
	CSamePeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamePeople();

// Dialog Data
	enum { IDD = IDD_SAME_PEOPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CProgressWnd wndP; 

	CString m_head;				// html fejléc
	CString m_description;		// leírás a html fájlban
	CString m_columns;			// oszlop feliratok a html fájlban
	CString p_fields;			// lekérdezett people oszlopok
	CString m_command;
	CString str;
	CString m_name;				// egy ember neve, amit elõírhatunk vizsgélatra
	int		m_match;			// az egyezõ adazpárok száma;
	
	CString unitedSpec;			// egyesített bejegyzéseket listázó fájl
	CString differentSpec;		// különbözü bejegyzéseket listázó fájl
	
	FILE* fU;  
	FILE* fD;  

	int		m_deleted;			// törölt bejegyzések száma
	int		m_contracted;		// jelzi, hogy vannak összevont bejegyzések a vPeople vektorban
	int		m_azonos;			// az egyesítéshez szükséges azonosságok száma
	int		m_loop;

	CString m_info;				// információs szöveg a programról
	bool	m_contract;			// végrehajtsa-e az összevonást
	std::vector<SAMENAMES> vPeople;
	std::vector<CONTRACT> vContract;
	SAMENAMES r;				// referencia adatok az összehasonlításnál
	
	CStringArray m_colors;
	

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;

	
//	CComboBox ComboCtrl;

	void createHead( CString title, int loop  );
	void core( int loop );
	void putPeople( CString name, UINT i );
	void processPeople();
	void listPeople();
	int	 sameSpouses( CString spouse1, CString spouse2 );
	int  identical( UINT i1, UINT i2 );
	void contract( UINT i1, UINT i2 );
	void contractFull( int loop );
	void clearMarriages( int loop );
	void setRef( int i ) ;
	void resetRef();
	void printRef( int group );

	void openUnited( int loop );
	void openDifferent( int loop );

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();



};
