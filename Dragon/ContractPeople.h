#pragma once
#include "afxwin.h"
#include "ProgressWnd.h"

// CContractPeople dialog
typedef struct
{
	UINT	cnt;			// számláló  
	UINT	group;			// csoport száma ( 0: nem tartozik csoportba, azaz nincs még egy olyan ember, mint õ
	UINT	group2;			// más csoporttól vette el
	int		status;			// 1 = megtartandó, -1 törlendõ 
	UINT	rgbcolor;		// color kód, csak ellenõrzésra
	UINT	match;			// 

	CString rowid;			// az ember és apja, anyaja adatai, házastársainak név-listája
	CString line;
	CString generation;
	CString source;
	CString united;
	CString	sex_id;

	CString name;
	CString birth;
	CString death;
	CString father;				// father
	CString rowidF;
	CString birthF;
	CString deathF;
	CString mother;				// mother
	CString rowidM;			
	CString birthM;
	CString deathM;
	CString spouses;
	CString rowidS;				// spouse 
	CString lineF;
	CString nameBD;
	CString wedding;
}SAMENAMES;

class CContractPeople : public CDialogEx
{
	DECLARE_DYNAMIC(CContractPeople)

public:
	CContractPeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CContractPeople();

	
// Dialog Data
	enum { IDD = IDD_CONTRACT_PEOPLE_DLG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	int m_azonos;			// az egyesítéshez szükséges azonosságok száma
	bool m_checkSpouse;
	CProgressWnd wndP; 

	CString m_head;				// html fejléc
	CString m_description1;		// leírás a html fájlban
	CString m_description2;		// leírás a html fájlban
	CString p_fields;			// lekérdezett people oszlopok
	CString m_command;
	CString str;
	CString m_name;				// egy ember neve, amit elõírhatunk vizsgélatra
	int		m_match;			// az egyezõ adazpárok száma;
	int		nItem;
	int		m_loop;
	int		m_loopMax;
//	CString unitedSpec;			// egyesített bejegyzéseket listázó fájl
//	CString differentSpec;		// különbözü bejegyzéseket listázó fájl
	
//	FILE* fU;  
//	FILE* fD;  

	FILE* textU;
	FILE* textD;

	CString m_fileSpecTextU;
	CString m_fileSpecTextD;
	CString sWHITE;

	bool	m_contracted;		// jelzi, hogy vannak összevont bejegyzések a vPeople vektorban
	
	std::vector<SAMENAMES> vPeople;
	std::vector<SAMENAMES> vSpouses;

	std::vector<CONTRACT> vContract;
	std::vector<TCHAR*> tableLines; 

	SAMENAMES r;				// referencia adatok az összehasonlításnál
	
	CStringArray m_colors;
	UINT	m_rgb[10];
	

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;



	void createHead( CString title  );
	bool core();
	bool putPeople( CString name, UINT i );
	void processPeopleNew();
	void processPeople();
	void listPeople();
	int	 sameSpouses( CString spouse1, CString spouse2 );
	int  identical( UINT i1, UINT i2 );
	void pushVContract( UINT i1, UINT i2 );
	void contractFull();
	void deleteMarriages();
	void setRef( int i ) ;
	void resetRef();
	void emptyLine( FILE* fl, int loop );

	void openUnited( );

	void openDifferent();
	void keress( int start );
	void push( CString item );
	void list();

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );


	bool contractPeople();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox comboCtrl;
};
