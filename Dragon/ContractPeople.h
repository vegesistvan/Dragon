#pragma once
#include "afxwin.h"
#include "ProgressWnd.h"

// CContractPeople dialog
typedef struct
{
	UINT	cnt;			// sz�ml�l�  
	UINT	group;			// csoport sz�ma ( 0: nem tartozik csoportba, azaz nincs m�g egy olyan ember, mint �
	UINT	group2;			// m�s csoportt�l vette el
	int		status;			// 1 = megtartand�, -1 t�rlend� 
	UINT	rgbcolor;		// color k�d, csak ellen�rz�sra
	UINT	match;			// 

	CString rowid;			// az ember �s apja, anyaja adatai, h�zast�rsainak n�v-list�ja
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

	CString m_fileSpecTextU;
	CString m_fileSpecTextD;

// Dialog Data
	enum { IDD = IDD_CONTRACT_PEOPLE_DLG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	int m_azonos;			// az egyes�t�shez sz�ks�ges azonoss�gok sz�ma
	bool m_checkSpouse;
	CProgressWnd wndP; 

	CString m_head;				// html fejl�c
	CString m_description1;		// le�r�s a html f�jlban
	CString m_description2;		// le�r�s a html f�jlban
	CString p_fields;			// lek�rdezett people oszlopok
	CString m_command;
	CString str;
	CString m_name;				// egy ember neve, amit el��rhatunk vizsg�latra
	int		m_match;			// az egyez� adazp�rok sz�ma;
	int		nItem;
	int		m_loop;
	int		m_loopMax;
//	CString unitedSpec;			// egyes�tett bejegyz�seket list�z� f�jl
//	CString differentSpec;		// k�l�nb�z� bejegyz�seket list�z� f�jl
	
//	FILE* fU;  
//	FILE* fD;  

	FILE* textU;
	FILE* textD;

//	CString m_fileSpecTextU;
	
	CString sWHITE;

	bool	m_contracted;		// jelzi, hogy vannak �sszevont bejegyz�sek a vPeople vektorban
	bool	m_spouseName;
	
	std::vector<SAMENAMES> vPeople;
	std::vector<SAMENAMES> vSpouses;

	std::vector<CONTRACT> vContract;
//	std::vector<TCHAR*> tableLines; 

	SAMENAMES r;				// referencia adatok az �sszehasonl�t�sn�l
	
	CStringArray m_colors;
	UINT	m_rgb[13];
	CWnd* m_dlg;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;



	void createHead( CString title  );
	bool core();
	bool putPeople( CString name, UINT i );
	void processPeopleNew();
	void processPeople();
	void listPeople();
	int	 sameSpouses( CString source, CString spouse1, CString spouse2 );
	int	 howMany( CString spouse );
	int  identical( UINT i1, UINT i2 );
	void pushVContract( UINT i1, UINT i2 );
	void contractFull();
	void deleteMarriages();
	void setRef( int i ) ;
	void resetRef();
	void emptyLine( FILE* fl, int loop );
	int checkEmptyCouples( int group );
	int checkEmptyCouples2( int group );
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
