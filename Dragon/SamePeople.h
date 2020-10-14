#pragma once
#include "listctrlex.h"
#include "ProgressWnd.h"

// CSamePeople dialog

class CSamePeople : public CDialogEx
{
	DECLARE_DYNAMIC(CSamePeople)
	DECLARE_EASYSIZE

public:
	CSamePeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamePeople();

// Dialog Data
	enum { IDD = IDD_SAME };

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
	
	CString unitedSpec;			// egyesített bejegyzéseket listázó fájl
	CString differentSpec;		// különbözü bejegyzéseket listázó fájl
	
	FILE* fU;  
	FILE* fD;  

	int		m_deleted;			// törölt bejegyzések száma
	int		m_contracted;		// jelzi, hogy vannak összevont bejegyzések a vPeople vektorban
	int		_azonos;			// az egyesítéshez szükséges azonosságok száma


	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;

	std::vector<SAMENAMES> vPeople;

// referencia adatok az összehasonlításhoz
	CString m_birthR;
	CString m_deathR;
	CString m_fatherR;
	CString m_motherR;
	CString m_birthFR;
	CString m_deathFR;
	CString m_birthMR;
	CString m_deathMR;
	CString m_spousesR;
	CString	m_generationR;

// egy rekord adatai
	int		_group;
	int		_status;
	CString _gen;
	CString _source;
	CString _united;
	CString _line;
	CString _rowid;
	CString _name;
	CString _birth;
	CString _death;
	CString _father;
	CString _mother;
	int		_married;
	CString _birthF;
	CString _deathF;
	CString _birthM;
	CString _deathM;
	CString _spouses;

// egy másik rekord adatai
	int		_group2;
	int		_status2;
	CString _gen2;
	CString _source2;
	CString _united2;
	CString _line2;
	CString _rowid2;
	CString _name2;
	CString _birth2;
	CString _death2;	
	CString _father2;
	CString _mother2;
	int		_married2;
	CString _birthF2;
	CString _deathF2;
	CString _birthM2;
	CString _deathM2;
	CString _spouses2;


	
	void createHead( CString title );
	void collectPeople();
	void putPeople( CString name, UINT i );
	void processPeople();
	void getData( UINT i );
	void getData2( UINT i );
	void listPeople();
	int  identical( UINT i1, UINT i2 );
	void contract( UINT i1, UINT i2 );
//	int gen();
	void setRef( int i ) ;
	void resetRef();

	void openUnited();
	void openDifferent();

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CListCtrlEx m_ListCtrl;
};
