#pragma once

#if !defined(AFX_FA_H__930379D8_BDD1_4973_93FF_041F3F3811E4__INCLUDED_)
#define AFX_FA_H__930379D8_BDD1_4973_93FF_041F3F3811E4__INCLUDED_


#include "EasySizeHD.h"
#include "SqliteDBEx.h"
#include "ListCtrlEx.h"
#include "structures.h"
//#include "GA_structures.h"
#include "database.h"
#include "Color.h"
#include "WM_USER.h"
#include "resource.h"		// main symbols
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// egy people rekordb�l kivett adatok
typedef struct
{
	CString rowid;
	CString xid;
	CString fileNumber;
	CString familyNumber;
	CString tableNumber;
	CString lineNumber;
	CString source;
	CString generation;
	CString sex_id;
	CString title;
	CString titolo;
	CString first_name;
	CString last_name;
	CString posterior;
	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
	CString comment;
	CString father_id;;
	CString mother_id;
	CString parentIndex;
	CString parentIndexCalc;
	CString folyt;
	CString tableAncestry;
	CString tableRoman;
}P;


#define	TOSCREEN FALSE
#define	FROMSCREEN TRUE

#define	MAN 1
#define	WOMAN 2

#define MANS L"1"
#define WOMANS L"2"


#define NOFATHERID 1000000	// ha a t�bl�ban olyan lesz�rmazottak vannak, akiknek nincs apjuk, de testv�rek, �s ez�rt kell nekik egy dummy apa


#define MANUAL	L"k�zi adatbevitel"
#define GEDCOM	L"GEDCOM f�jlb�l"
#define GAHTML	L"GA.html f�jlb�l"
#define URES	L"�res adatb�zis"


#ifdef _M_X64 
#define PLATFORM L"64 bites"
#else 
#define PLATFORM L"32 bites"
#endif
/*
// f�jl t�pusok a 'filespec' t�bl�ban
enum
{
	GA_HTML = 0,
	GEDCOM_FILE, 
	UNITED_FILE,
	UNITED1_HTML_FILE,
	UNITED2_HTML_FILE,
	DIFFERENT_FILE,
	DIFFERENT1_HTML_FILE,
	DIFFERENT2_HTML_FILE,
	COUPLESU_TEXT_FILE,
	COUPLESD_TEXT_FILE,
	COUPLESU1_HTML_FILE,
	COUPLESU2_HTML_FILE,
	COUPLESD1_HTML_FILE,
	COUPLESD2_HTML_FILE,
};
*/
enum
{
	GA_HTML = 0,
	GEDCOM_FILE, 
	UNITED_FILE,
	DIFFERENT_FILE,
	COUPLESU_TEXT_FILE,
	COUPLESD_TEXT_FILE,
	COUPLESU1_HTML_FILE,
	COUPLESU2_HTML_FILE,
	COUPLESD1_HTML_FILE,
	COUPLESD2_HTML_FILE,
};


//bool openFileSpec( FILE** ff, CString fileSpec, CString mode );
CString getTimeTag();


class CDragonApp : public CWinApp
{
public:
	CDragonApp();

	enum
{
	ROWID = 0,
	LINENUMBER,
	SEXID,
	LASTNAME,
	FIRSTNAME,
	BIRTHPLACE,
	BIRTHDATE,
	DEATHPLACE,
	DEATHDATE,
	PROFESSION,
	COMMENT,
	FATHERID,
	MOTHERID,
	COLUMNSCOUNT
};



// Overrides
public:
	virtual BOOL InitInstance();

	int	m_inputCode;
	// a k�perny� m�retei pixelben
	int _w;
	int _h;

	bool	NOTEPAD;

	CSqliteDB*	mainDB;
	CSqliteDB*	systemDB;
	CSqliteDB*	blobDB;
	
	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;
	CSqliteDBRecordSet*	 m_recordsetSystem;
	CSqliteDBRecordSet*	 m_recordsetBlob;
	CSqliteDBRecordSet*	 m_recordsetName;
	CSqliteDBRecordSet*	 m_masterTable;
	CSqliteDBRecordSet*	 m_rSetCheck;
	CSqliteDBRecordSet*	 m_recSetX;
// SETUP V�LTOZ�K //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int m_fatherAgeMin;
	int m_fatherAgeMax;
	int m_motherAgeMin;
	int m_motherAgeMax;
// SETUP V�LTOZ�K //////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TCHAR	_startFileSpec[_MAX_PATH];
	CString m_helpFileSpec;
	CString m_exePath;
	CString m_bitmapSpec;

	CString drive;
	CString path;
	CString filename;
	CString ext;

	CString m_inputMode; 

	CString spouse;
	CString spouse_father;
	CString spouse_mother;
	CString	spouse_spouse;

	CString SQLITEVERSION;
	
	TCHAR m_username[100];

	int		m_cntPeople;
//	int		m_cntMarriages;

	int		m_requested;

	std::vector<CString> v_notfound;
	std::vector<int> v_rowids;
	std::vector<int> v_tableNumbers;


	CString		str;
	CString		m_command;
	CString		m_databasePath;
	CString		m_databaseSpec;
	CString		m_systemDatabaseSpec;
	CString		m_databaseName;
	CString		m_baseName;
	
	CString		m_viewerSpec;
	CString		m_viewerName;
	CString		m_viewerFolder;

	CString		m_pdfSpec;
	CString		m_pdfFolder;
	CString		m_pdfName;

	CString		m_editorSpec;
	CString		m_editorFolder;
	CString		m_editorName;

	CString		m_blobSpec;
	CString		m_blobName;


	int		m_loop;
	int		m_azonos;

	const static COLORREF	m_colorClick = LIGHTBLUE;
	const static COLORREF	m_colorWarning = RED;
	
	CStringArray aSexes;
	CStringArray aRelations;

	int			m_time_start;

	CString	m_workingDirectory;
	CString	m_texteditor;
	CString	m_texteditor_default;
	CString m_htmlFileSpec;
	CString m_htmlFileName;
	CString m_htmlPath;
	CString m_htmlFile;
	CString m_faDbSpec;

	CString m_gedFileSpec;
	CString m_gedFileName;
	CString	m_gedFolder;

	CStringArray sourceA;

	CString m_columnsP;
	CString m_columnsM;
	CString m_columnsT;
	CString m_columnsB;

	_int64 m_cntDeleted;
	P pCurr;
	P pPrev;
	P pNext;


	FILE*	fl;
	FILE*	flU;
	FILE*	flS;

	CStdioFile errorFile;
	CStdioFile file;
	CStringArray CODE;

	CString veg;
	CString tag0;
	int		index0;
	int		iStart;
	int		m_fileNumber;
	CString	m_familyNumber;
	CString	m_tableNumber;
	int		position;
	int		m_deletedP;
	int		m_deletedM;
	CString m_listFile;
	
	std::vector<FAMS> v_fams;
	std::vector<CString> v_tables;
	std::vector<SPOUSES> v_spouses;
	std::vector<SPOUSES> v_spouses2;
	std::vector<REPLACE> v_Replace;
	std::vector<BLOBSTAT> vBlobs;

	CString	getYear( _int64 diff );
	int		diff( CString date1, CString date2 );
	bool	checkIntegrity( CSqliteDB* db );
	void	createColumnVector( CString list, std::vector<CString>* vColumns );
	int		getUserVersion();
	void	setUserVersion( int user_version );
	bool	insertIntoFiles( CString fileSpec, int type );
// Implementation
	BOOL	question( CString caption, CString question );
	void	message( CString caption, CString message );
	void	inputGed( int ins );
	void	getZeroLevel( CString cLine );
	int		getIndex( CString tag );
	BOOL	processHead( int ins );
	BOOL	processGenomap( int ins );
	BOOL	processOccupation( int ins );
	BOOL	processIndi( int ins ) ;
	BOOL	processFam( int ins );
	BOOL	processContact( int ins );
	BOOL	dateDiff( CString date1, CString date2, int month );
	_int64	getDateI( CString dateS, int month ) ;

	LPTSTR	upperCase( CString str );
	int		getTableNumber( CString tableName );
	void	extractChildren( int fams, int motherIndex );
	CString	getLanguage( int language_id );
	CString	getSpouses( CString rowid, CString sex_id, std::vector<SPOUSES>* vektor, SPOUSES2* sp );

	CStringA MultiToUtf8( LPCSTR multi );
	void	clearDatabase();
	BOOL	selectDatabase();
	BOOL	selectHtml( BOOL sameDB );
	BOOL	selectGedcom( BOOL sameDB );
	BOOL	selectTextEditor();
	BOOL	selectViewer();
	void	createColumnList();
	CString	getColumns( COLUMN* table, int n );

	CString	getInputMode();
	CString	contractions( void );

	void	newDatabase( CString tag );

	_int64	getPresentTime();
	CString	getPresentDate(); 
	CString	getPresentDateTime();
	void	Setsystemvariables( CString caption );
	CString	get_time_elapsed();
	void	setStartTime();
	_int64 CDragonApp::dateDiff( CString date1, CString date2 );
	CString	getDateStrFromI( CString dateI );
	_int64	getDateIFromStr( CString dateS );
	void	showLogFile();
	void	showFile(CString fileSpec );
	void	showHtmlFile(CString fileSpec );
	void	editNotepad( CString lineNumber );
	void	notepad( CString fileSpec, CString option );
	bool	selectFile(  CString fileSpec, BLOBSTAT* stat);
	bool	selectFiles( );

	void CDragonApp::change( CString linenumber, CString line );
	void CDragonApp::edit1line( int lineNumber );
	void CDragonApp::edit2lines( std::vector<CString>* vLines );
	void CDragonApp::htmlEditLines( CListCtrlEx* p_ListCtrl, int col_linenumber, CString title );
	void CDragonApp::HtmlNotepadParents( CString rowid );

	void	saveHtmlLine( int lineNumber, CString line );
	BOOL	openDatabase();
	BOOL	openSystemDatabase();
	BOOL	openBlob();
	FILE*	openLogFile(CString fname,CString title);
	CString openHtmlFile( FILE** fh,  CString fileName, CString mode );
	CString	openTextFile( FILE** fl,  CString fileName, CString mode );

	bool	getStructure( const DB* filestructure, CString fileSpec );
	bool	checkStructure( CSqliteDBRecordSet* rec,  const DB* fileStructure, int tableNumnberDB, CString fileSpec );

	CString selectPeople( CString rowid, CString* linenumber );
	void	keress( CString searh, CListCtrlEx* m_listCtrl, int column, int start );
	void	showItem( int nItem, CListCtrlEx* p_listCtrl );

	BOOL	query( CString command );
	BOOL	query1( CString command );
	BOOL	query2( CString command );
	BOOL	query3( CString command );
	BOOL	query4( CString command );
//	BOOL	queryMaster( CString command );
	BOOL	querySystem( CString command );
	BOOL	queryBlob( CString command );
	BOOL	queryName( CString command );
	BOOL	queryX( CSqliteDB* db, CString command );

	BOOL	execute( CString command);
	BOOL	executeSys( CString command);
	BOOL	executeBlob( CString command);
	BOOL	executeX( CSqliteDB* db, CString command);
	

	void	createCommand( CString tablename, COLUMN *tableStruct, int n );
//	void	changeTime();
//	void	search( CString search, INT_PTR orderix,  CListCtrlEx* p_ListCtrl );
	void	selectAll( CListCtrlEx * p_ListCtrl );
	void	unselectAll( CListCtrlEx * p_ListCtrl );

	void	exportAll( CString logFile, CString title, CListCtrlEx* p_ListCtrl );
	void	exportSelected( CString str, CString title, CListCtrlEx* p_ListCtrl );
	BOOL	listTable( FILE * fl, CListCtrlEx* p_ListCtrl );
	void	underlining( FILE * fl, std::vector<BOOL>* selected, std::vector<COLUMNS>* columns );
	void	header( FILE * fl, std::vector<BOOL>* selected, std::vector<COLUMNS>* columns );
	int		selectFromTextFile( CListCtrlEx* p_ListCtrl, int column, CString caption  );
	void	deleteSame12( P* pKeep, P* pDel  );
	void	deleteSame22( P* pKeep, P* pDel  );
	void	deleteSame15( P* pKeep, P* pDel  );
	void	deleteSame25( P* pKeep, P* pDel  );

	void	saveInfo( CString rowid, CString rowidBy );

	void	listHtmlLine( int lineNumber );

	CString getFamily( CString familyNumber );
	CString getTable( CString tableNumber );
	void	unite( UINT iter );
	void	sameMarriages( int iter );

	int		isFirstName( CString name );
	int		getNumberOfDb( std::vector<CString>* vE );
	CString getHtmlLine( CString lineNumber );

	CWnd* _dlg;

	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnTableFirstNames();
	afx_msg void OnUsermanual();
	afx_msg void OnLanguages();
	afx_msg void OnPlaces();
	afx_msg void OnPrivatFolyt();
	afx_msg void OnUnite();
	afx_msg void OnHusbandWife();
	afx_msg void OnEmail();
	afx_msg void OnCheckIntegrity();

	afx_msg void OnCheckGenerations();
	afx_msg void OnFamilyDates();
	afx_msg void OnUnknownFirstnames();
};
extern CDragonApp theApp;

#endif // !defined(AFX_FA_H__930379D8_BDD1_4973_93FF_041F3F3811E4__INCLUDED_)