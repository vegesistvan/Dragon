#pragma once
#include "listctrlex.h"
#include "colorstatic.h"
#include "Table_people.h"
#include "Table_marriages.h"
#include "SelectTableHtml.h"
#include "Table_html.h"
#include "html_Descendants.h"
#include "html_Spouses.h"
#include "html_Marriages.h"
#include "html_SpouseSpouses.h"
#include "afxwin.h"


enum
{
	M_ROWID = 0,
	M_ROWID_FILE,
	M_FAMILYNUMBER,
	M_TABLENUMBER,
	M_LINENUMBER,
	M_SOURCE,
	M_TABLENUMBERROMAN,
	M_GENERATION,
	M_SEX,
	M_TITLE,
	M_TITOLO,
	M_LAST_NAME,
	M_KNOWN_AS,
	M_FIRST_NAME,
	M_BIRTH_PLACE,
	M_BIRTH_DATE,
	M_DEATH_PLACE,
	M_DEATH_DATE,
	M_AGE,
	M_COMMENT,
	M_ROWID_FATHER,
	M_TABLE_ID_FATHER,
	M_FATHER,
	M_BIRTH_DATE_FATHER,
	M_ROWID_MOTHER,
	M_TABLE_ID_MOTHER,
	M_MOTHER,
	M_BIRTH_DATE_MOTHER,
	M_FATHER_CNT,
	M_MOTHER_CNT,
	M_FOLYT,
	M_TABLE,
};

#ifdef RESAMPLE_EXPORTS
#define RESAMPLE_API __declspec(dllexport)
#else
#define RESAMPLE_API __declspec(dllimport)
#endif

RESAMPLE_API HBITMAP CreateResampledBitmap(HDC hdc, HBITMAP hBmpSource, DWORD dwWidth, DWORD dwHeight, DWORD dwFilter);

//#define WM_NEWHTML		WM_USER + 0x1FFB
#define WM_NEWHTML		0xC000



// CDragonDlg dialog
class CDragonDlg : public CDialogEx
{
// Construction
public:
	CDragonDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CDragonDlg();
//	DECLARE_EASYSIZE

	enum { IDD = IDD_FA_DIALOG };


	int m_rowid;
	int m_sex_id;

	CString	m_familyNumber;
	CString	m_tableNumber;
	CString m_rowid_father;
	CString m_rowid_mother;
	CString m_gener;
	CString m_name;

	void	mainTitle();
	void CDragonDlg::listHtmlLine( int lineNumber );
	void CDragonDlg::displayTree();
	void CDragonDlg::displayTree2();
	void CDragonDlg::displayTree3();
	void CDragonDlg::closeHtml();
	void CDragonDlg::reloadMSubstr();

	int _xDest;
	int _yDest;
	int _wDest;
	int _hDest;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	BOOL owners;
	CString m_title;

//	CFile* m_file;
	CStdioFile file;
	CTextFileWrite *m_txtfile;
	CMenu menu;


protected:

	CWnd* m_dlg; 
	HICON m_hIcon;

	CString BUILT;



	CString drive;
	CString dir;
	CString fname;
	CString ext;

	CString errorMsg;

	BOOL m_html_open;
	
	int	m_table_number;
	
	int cnt_ol;
	BOOL	m_join;		// összekapcsolja-e az elágazásokat?
	BOOL	m_limited;	// TRUE: csak a nála nagyobb generációkat listázza
	BOOL	WOMEN_DESCENDANT;

	int	m_numberOfPixelsHorizontal;
	int m_numberOfPixelsVertical;
	CString	str;
	CString	m_command;

	std::vector<TCHAR*> v_individuals;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	CString TABLE;

	CString m_join_father;
	CString	m_join_child;

	CString m_filterNew;
	CString	m_filter;

	CString		m_filterText;
	CString		m_filterTextNew;

	CString		m_substring;

	int		m_orderix;
	FILE * fl;

	int		m_fileNumber;
	int		m_lineNumber;
	CString m_familyName;
	CString m_generation;
	CString m_known_as;

	


	int		m_tableNumberSelected;
	CString m_tableNameSelected;


	std::vector<TCHAR*> v_peoples;
//	std::vector<FAMS> v_fams;
	
	HMENU	m_hMenuDefault;

	CString m_htmlFile;

	

	int		m_columnsCount;
	CString m_save;

	void CDragonDlg::createListColumns( );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GEDCOM	
	CString tag0;
	int		index0;
	int		position;
	

	std::vector<CString> v_tables;


	void CDragonDlg::extractChildren( int fams, int motherIndex );
//	void	fillFams( int fams, int husb, int wife, ORDERS* orders );
	void CDragonDlg::getZeroLevel( CString cLine );
	BOOL CDragonDlg::processHead( int ins );
	BOOL CDragonDlg::processGenomap( int ins );
	BOOL CDragonDlg::processOccupation( int ins );
	BOOL CDragonDlg::processIndi( int ins );
	BOOL CDragonDlg::processFam( int ins );
	BOOL CDragonDlg::processContact( int ins );

//	int CDragonDlg::getRowid( int index_indi  );
	int CDragonDlg::getTableNumber( CString tableName );
	int CDragonDlg::getIndex( CString tag );
	void CDragonDlg::inputGed( int ins );


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void getTables( CString caption );
	int CDragonDlg::getYearFromDate( CString date );
	void CDragonDlg::enableMenu();
	
	void CDragonDlg::crateDatabaseColumns();
	void CDragonDlg::crateMarriagesColumns();
	void	fillGATable();
	void	fillMTable( UINT nItem );
	void	fillTables();
	void	fillMRow( int nItem );
	void CDragonDlg::insertRow();
	void CDragonDlg::selectAll( );
	void CDragonDlg::unselectAll( );

	void	EditUpdate( int nItem );
	BOOL	query( CString command );
	BOOL	query1( CString command );
	BOOL	query2( CString command );
	BOOL	query3( CString command );
	BOOL	query4( CString command );
	
	void	push( CString str );

	void CDragonDlg::Table_html();
	CString CDragonDlg::getFamilyName( CString cLine );
	
	void CDragonDlg::ChangeMenu();
		
	BOOL CDragonDlg::isAncestor();
	void CDragonDlg::predecessors_old();
	void CDragonDlg::predecessors( CString field );
	
	void CDragonDlg::TableSpouses( );
	void CDragonDlg::fillSTable( );
	void CDragonDlg::fillPTable( );
	void CDragonDlg::fillRTable( );


	void CDragonDlg::htmlInput0();
	void CDragonDlg::htmlInput();

	void CDragonDlg::selectTable( CString tableNumber );
	void CDragonDlg::extractTable( int tableNumber, CString table_name );	
	void extract( FILE* flOut, int tablePosition, int tableLength );

	void CDragonDlg::checkLifespan( int lifespan );
	void CDragonDlg::spousesAgeDiff( int age_diff );
	void CDragonDlg::parentChildDiff( int sex_id, int parentChildMin, int parentChildMax );
	void CDragonDlg::ageAtMarriage( int sex_id, int ageMin, int ageMax );


	void CDragonDlg::OnFilterUnfilter();

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	int		m_generation_previous;
	int		m_parent_id;
	

public:
	void	descendant();
	void	descendantManual();

	void CDragonDlg::newSp();


//modeless dialog boxes////////////////
	CTablePeople* m_pIndividuals;
	CTableMarriages* m_pMarriages;

	CDescendant* m_pDescendantsF;
	CDescendant* m_pDescendantsT;
	CDescendant* m_pDescendantsL;

	CMarriages* m_pMarriagesF;
	CMarriages* m_pMarriagesT;
	CMarriages* m_pMarriagesL;

	CHtmlSpouses* m_pSpousesF;
	CHtmlSpouses* m_pSpousesT;
	CHtmlSpouses* m_pSpousesL;

	CSpouseSpouses* m_pSsF;
	CSpouseSpouses* m_pSsT;
	CSpouseSpouses* m_pSsL;

	CStatic m_picture;
///////////////////////////////////////

	afx_msg void OnSelectedFiles();
	afx_msg void OnTitles();
	afx_msg void OnDatabaseInformation();
//	afx_msg void OnTabletables();
	afx_msg void OnPeoplessubstrings();
	afx_msg void OnCheckDates();
//	afx_msg void OnFiles();
	afx_msg void OnFilterAncestors();
	afx_msg void OnTreePeoplesTablesNoBranch();
	afx_msg void OnTrsubstrings();
	afx_msg void OnOperation();

	afx_msg void OnDescendants();
	afx_msg void OnDescendantFile();
	afx_msg void OnDescendantTable();
	afx_msg void OnDescendantLine();

	afx_msg void OnMarriagesFile();
	afx_msg void OnMarriagesTable();
	afx_msg void OnMarriagesLine();

	afx_msg void OnSpousesFile();
	afx_msg void OnSpousesLine();
	afx_msg void OnSpousesTable();

	afx_msg void OnSsFile();
	afx_msg void OnSsTable();
	afx_msg void OnSsLine();

	afx_msg void OnTreeTablesNobranches();
	afx_msg void OnListUtf8();
	afx_msg void OnOpenDb();
	afx_msg void OnDisplayMarriages();
	afx_msg void OnDisplayFilestable();
	afx_msg void OnListgaDescendants();
	afx_msg void OnInfoOnprogram();
	afx_msg void OnDisplayIndividuals();
	afx_msg void OnDisplayTables();
	afx_msg void OnSsubstrFile();
	afx_msg void OnSsubstrLine();
	afx_msg void OnSsubstrTable();
	afx_msg void OnSpousesFromfile();
	afx_msg void OnSpousesFromline();
	afx_msg void OnSpousesofspousesFromfile();
	afx_msg void OnSpousesofspousesFromline();
	afx_msg void OnSpousesofspousesFromtable();
	afx_msg void OnSpousesFromtable();
	afx_msg void OnMenuEmpty();
	afx_msg void OnMenuGahtml();
	afx_msg void OnMenuGedcom();
	afx_msg void OnMenuManual();
	afx_msg void OnInputGahtmlFile();
	afx_msg void OnInputGahtmlFamily();
	afx_msg void OnInputGahtmlFromline();
	afx_msg void OnInputGahtmlLine();
	afx_msg void OnInputGahtmlTable();
	afx_msg void OnConnectBranches();
	afx_msg void OnManualInputDialog();
	afx_msg void OnInputKeyboard();

	afx_msg void OnPaint();
	afx_msg void OnAppExit();
	afx_msg void OnBracketsSquare();
	afx_msg void OnBracketsRound();
	afx_msg void OnBraces();
//	afx_msg void OnGenerationcode();
	afx_msg void OnFromtable();
	afx_msg void OnFromfamily();
	afx_msg void OnConnectCsalad();
	afx_msg void OnCsalad();
	afx_msg void OnCsaladTorzs();
	afx_msg void OnMotherDeath();
	afx_msg void OnCheckLifespan();
	afx_msg void OnDateFormat();
	afx_msg void OnSameSpousesNameList();
	afx_msg void OnInputManual();
	afx_msg void OnListPeople();
	afx_msg void OnCheckTags();
	afx_msg void OnGedcomFam();
	afx_msg void OnGedcomIndi();
	afx_msg void OnGedcomFile();
	afx_msg void OnGedcom0level();
	afx_msg void OnGedcomHead();
	afx_msg void OnGedcomInput();
	afx_msg void OnListFamilyByName();
	afx_msg void OnGedcomVindi();
	afx_msg void OnNewFamilies();
	afx_msg void OnClose();
	afx_msg void OnCheckFather9();

	afx_msg void OnCheckSpousesDiff();
	afx_msg void OnFatherchildGreater();
	afx_msg void OnFatherchildLess();
	afx_msg void OnMotherchildGreater();
	afx_msg void OnMotherchildLess();
	afx_msg void OnHusbandAgeG();
	afx_msg void OnHusbandAgeL();
	afx_msg void OnWifeAgeG();
	afx_msg void OnWifeAgeL();
	afx_msg void OnCheckSpousesSex();
	afx_msg void OnSameNameAndBirth();
	afx_msg void OnSameNameAndDeath();
	afx_msg void OnSameNameAndFather();
	afx_msg void OnSameNameAndMother();
	afx_msg void OnCheckSameSpouseName();
	afx_msg void OnCheckSameNamesSpouses();
	afx_msg void OnListPeopleAndFamily();
	afx_msg void OnSamePeople();
	afx_msg void OnSamePeopleContract();
	afx_msg void OnNameProblems();
	afx_msg void OnMotherIndex();
	afx_msg void OnDisplayBlob();
	afx_msg void OnGedcomTagtable();
	afx_msg void OnGedcomINDIFAMS();
	afx_msg void OnSameNames();
	afx_msg void OnSame();
};
