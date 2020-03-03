#pragma once

#if !defined(AFX_FA_H__930379D8_BDD1_4973_93FF_041F3F3811E4__INCLUDED_)
#define AFX_FA_H__930379D8_BDD1_4973_93FF_041F3F3811E4__INCLUDED_


#include "EasySizeHD.h"
#include "SqliteDBEx.h"
#include "ListCtrlEx.h"
#include "ListBoxEx.h"
#include "structures.h"
#include "utilities.h"
#include "database.h"
#include "ProgressWnd.h"
#include "version.h"
#include "Color.h"
#include "ColorStatic.h"
#include "textfile.h"
#include "WM_USER.h"



#define	TOSCREEN FALSE
#define	FROMSCREEN TRUE

#define	MAN 1
#define	WOMAN 2

#define MANS L"1"
#define WOMANS L"2"


#define FATHERID 1000000


#define MANUAL	L"kézi adatbevitel"
#define GEDCOM	L"GEDCOM fájlból"
#define GAHTML	L"GA.html fájlból"
#define URES	L"üres adatbázis"

#ifdef _M_X64 
#define PLATFORM L"64 bites"
#else 
#define PLATFORM L"32 bites"
#endif


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


enum
{
	M_TYPE = 0,
	M_NAME,
	M_TABLE_NAME,
	M_ROOTPAGE,
	M_SQL
};

bool openFileSpec( FILE** ff, CString fileSpec, CString mode );
CString getTimeTag();


class CFaApp : public CWinApp
{
public:
	CFaApp();

//	static char THIS_FILE[]=__FILE__;

// Overrides
public:
	virtual BOOL InitInstance();

	int	m_inputCode;
	// a képernyõ méretei pixelben
	int _w;
	int _h;
	int	_iterationCount;

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
// SETUP VÁLTOZÓK //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int m_fatherAgeMin;
	int m_fatherAgeMax;
	int m_motherAgeMin;
	int m_motherAgeMax;
// SETUP VÁLTOZÓK //////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	int		m_cntMarriages;

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
	std::vector<MOTHERINDEX> v_mother_index;
	std::vector<SPOUSES> v_spouses;
	std::vector<SPOUSES> v_spouses2;
	std::vector<REPLACE> v_Replace;
	std::vector<BLOBSTAT> vBlobs;

	bool	checkIntegrity( CSqliteDB* db );
	void	createColumnVector( CString list, std::vector<CString>* vColumns );
	int		getUserVersion();
	void	setUserVersion( int iterationCount );
	void	insertIntoFiles( CString fileSpec );
// Implementation
	void	replaceBy( CString rowid, CString rowidBy, CString sex_id, int source );
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
	CString	getDateI( CString dateS, int month ) ;

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
	CString	getDateStrFromI( CString dateI );
	CString	getDateIFromStr( CString dateS );
	void	showLogFile();
	void	showFile(CString fileSpec );
	void	showHtmlFile(CString fileSpec );
	void	editNotepad( CString lineNumber );
	void	notepad( CString fileSpec, CString option );
	bool	selectFile(  CString fileSpec, BLOBSTAT* stat);
	bool	selectFiles( );

	BOOL	openDatabase();
	BOOL	openSystemDatabase();
	BOOL	openBlob();
	FILE*	openLogFile(CString fname,CString title);
//	CString openFile( CString fileName, CString mode );
//	bool	openFileSpec( FILE** ff, CString fileSpec, CString mode );
	CString openHtmlFile( FILE** fh,  CString fileName, CString mode );
	CString	openTextFile( FILE** fl,  CString fileName, CString mode );

	bool	getStructure( const DB* filestructure, CString fileSpec );
	bool	checkStructure( CSqliteDBRecordSet* rec,  const DB* fileStructure, int tableNumnberDB, CString fileSpec );

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
	void	search( CString search, INT_PTR orderix,  CListCtrlEx* p_ListCtrl );
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
	int		getYearFromDate( CString date );


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
};
extern CFaApp theApp;

#endif // !defined(AFX_FA_H__930379D8_BDD1_4973_93FF_041F3F3811E4__INCLUDED_)