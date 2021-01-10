#pragma once
#include "listctrlex.h"
#include "ProgressWnd.h"
#include "afxwin.h"
#include "colorstatic.h"

// CSamePeople dialog

class CSamePeople : public CDialogEx
{
	DECLARE_DYNAMIC(CSamePeople)
	DECLARE_EASYSIZE
public:
	CSamePeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamePeople();

// Dialog Data
	enum { IDD = IDD_SAME_PEOPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

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
	int		m_columnsCount;

	CString unitedSpec;			// egyesített bejegyzéseket listázó fájl
	CString differentSpec;		// különbözü bejegyzéseket listázó fájl
	
	FILE* fU;  
	FILE* fD;  

	bool	m_contracted;		// jelzi, hogy vannak összevont bejegyzések a vPeople vektorban
	int		m_azonos;			// az egyesítéshez szükséges azonosságok száma
	bool	m_contract;			// végrehajtsa-e az összevonást
	std::vector<SAMENAMES> vPeople;
	std::vector<CONTRACT> vContract;
	std::vector<int> vColor;
	std::vector<TCHAR*> tableLines; 
	SAMENAMES r;				// referencia adatok az összehasonlításnál
	
	CStringArray m_colors;
	UINT	m_rgb[10];
	

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;

	CColorStatic colorSearch;
	CString m_search;


//	CComboBox ComboCtrl;

	void createColumns();

	void createHead( CString title  );
	void core();
	void putPeople( CString name, UINT i );
	void processPeople();
	void listPeople();
	int	 sameSpouses( CString spouse1, CString spouse2 );
	int  identical( UINT i1, UINT i2 );
	void contract( UINT i1, UINT i2 );
	void contractFull();
	void deleteMarriages();
	void setRef( int i ) ;
	void resetRef();
	void printRef( int group );
	void fillTable();

	void openUnited( );
	void openDifferent();

	void push( CString item );
	void list();

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_ListCtrl;	


	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedSearchTxt();
	afx_msg void OnHtml();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHtmlEdit();
	afx_msg void OnHtmlShows();
	afx_msg void OnHtmlPeoplefather();
	afx_msg void OnHtmlNotepad();

	afx_msg void OnInfo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
