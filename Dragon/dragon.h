// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// dragon.h : main header file for the drag application
//
#pragma once

#include "pch.h"
#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "structures.h"
#include "database.h"
#include "Color.h"
#include "utilities.h"
#include "SqliteDBEx.h"
#include "ListCtrlEx.h"
#include "ProgressWnd.h"
#include "MainFrm.h"
#include "dragonView.h"

// CDragApp:
// See drag.cpp for the implementation of this class
//

enum
{
	CAT_DATABASE = 0,
	CAT_TABLES,
	CAT_CHECK1,
	CAT_MERGE,
	CAT_CHECK2,
	CAT_GAHTML,
	CAT_GEDCOM,
	CAT_UTILITIES,
};

typedef struct
{
	CString	columnname;
	int		justification;
	int		length;
}COLUMNS;

typedef struct
{
	CString	sex_id;
	CString rowid;
	int		source;
	CString rowidBy;
}REPLACE;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// egy people rekordból kivett adatok
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
	CString folyt;
	CString tableAncestry;
	CString tableRoman;
}P;

typedef struct
{
	int		exeVersion;
	bool	bUnited;	// 0: nem egyesített, 1: egyesített
}DBPROPERTIES;

#define	TOSCREEN FALSE
#define	FROMSCREEN TRUE


#define BISEX L"0"
#define	MAN L"1"
#define	WOMAN L"2"



//#define NOFATHERID 1000000	// ha a táblában olyan leszármazottak vannak, akiknek nincs apjuk, de testvérek, és ezért kell nekik egy dummy apa


#define MANUAL	L"kézi adatbevitel"
#define GEDCOM	L"GEDCOM fájlból"
#define GAHTML	L"GA.html fájlból"
#define URES	L"üres adatbázis"



#ifdef _M_X64 
#define PLATFORM L"64 bites"
#else 
#define PLATFORM L"32 bites"
#endif

enum
{
	HTML_FILE = 0,
	GEDCOM_FILE,
	UNITED_FILE,
	DIFFERENT_FILE,
	ERROR_FILE,
	X_FILE,
	COUPLESU_TEXT_FILE,
	COUPLESD_TEXT_FILE,
	COUPLESU1_HTML_FILE,
	COUPLESU2_HTML_FILE,
	COUPLESD1_HTML_FILE,
	COUPLESD2_HTML_FILE,
};

class CDragApp : public CWinAppEx
{
public:
	CDragApp() noexcept;
	UINT  m_nAppLook;
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
	// a képernyõ méretei pixelben
	int _w;
	int _h;

	CSqliteDB* mainDB;
	CSqliteDB* systemDB;
	CSqliteDB* systemDB8;
	CSqliteDB* blobDB;

	CSqliteDBRecordSet* m_recordset;
	CSqliteDBRecordSet* m_recordset1;
	CSqliteDBRecordSet* m_recordset2;
	CSqliteDBRecordSet* m_recordset3;
	CSqliteDBRecordSet* m_recordset4;
	CSqliteDBRecordSet* m_recordsetSystem;
	CSqliteDBRecordSet* m_recordsetBlob;
	CSqliteDBRecordSet* m_recordsetName;
	CSqliteDBRecordSet* m_masterTable;
	CSqliteDBRecordSet* m_rSetCheck;
	CSqliteDBRecordSet* m_recSetX;

	CString	m_caption;
	int		m_userVersion;
	int		m_exeVersion;
	CString m_inputVersion;
	CString m_uniteVersion;
	CString m_inputMode;

	CMFCRibbonCategory* pCAT_DATABASE;
	CMFCRibbonCategory* pCAT_TABLES;
	CMFCRibbonCategory* pCAT_CHECK1;
	CMFCRibbonCategory* pCAT_MERGE;
	CMFCRibbonCategory* pCAT_CHECK2;
	CMFCRibbonCategory* pCAT_GAHTML;
	CMFCRibbonCategory* pCAT_GEDCOM;
	CMFCRibbonCategory* pCAT_UTILITIES;

	// SETUP VÁLTOZÓK //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int m_fatherAgeMin;
	int m_fatherAgeMax;
	int m_motherAgeMin;
	int m_motherAgeMax;
	// SETUP VÁLTOZÓK //////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TCHAR	_startFileSpec[_MAX_PATH];
	CString m_version;
	CString m_helpFileSpec;
	CString m_bitmapSpec;

	CString drive;
	CString path;
	CString filename;
	CString ext;


	CString spouse;
	CString spouse_father;
	CString spouse_mother;
	CString	spouse_spouse;

	CString SQLITEVERSION;

	TCHAR m_username[100];

	int		m_cntPeople;

	int		m_requested;

	std::vector<CString> v_notfound;
	std::vector<CString> v_rowid;
	std::vector<CString> v_tableNumbers;

	CMainFrame* pM;
	CDragonView* pW;

	CString		str;
	CString		m_command;

	CString m_dbFolderPath;
	CString m_dbPathName;
	CString m_dbFileTitle;
	CString m_dbFileName;

	CString	m_blobPathName;
	CString	m_blobFileTitle;
	CString	m_blobFileName;

	CString m_htmlPathName;
	CString m_htmlFileTitle;
	CString m_htmlFileName;
	CString m_htmlFolderPath;


	CString m_gedPathName;
	CString m_gedFileTitle;
	CString m_gedFileName;
	CString	m_gedFolderPath;

	CString		m_editorFolderPath;
	CString		m_editorFileName;

	CString		m_systemDatabaseSpec;
	CString		m_systemDatabaseSpec8;
	CString		m_systemDatabaseSpec16;
	CString		m_workingDirectory;
	CString		m_dragonjpg;
	CString		m_texteditor_default;
	CString		m_inputHtml;
	CString		m_versionPathName;


	BOOL		m_dbFileExists;


	const static COLORREF	m_colorClick = LIGHTBLUE;
	const static COLORREF	m_colorWarning = RED;

	int			m_time_start;

	_int64 m_cntDeleted;
	P pCurr;
	P pPrev;
	P pNext;


	FILE* fl;
	FILE* flU;
	FILE* flS;

	CStdioFile errorFile;
	CStdioFile file;

	CString veg;
	CString tag0;
	int		index0;
	int		iStart;
	CString	m_familyNumber;
	CString	m_tableNumber;
	int		position;
	int		m_deletedP;
	int		m_deletedM;
	CString m_listFile;

	_int64 m_startTime;

	std::vector<CString> v_tables;
	std::vector<SPOUSES> v_spouses;
	std::vector<SPOUSES> v_spouses2;
	std::vector<REPLACE> v_Replace;
	std::vector<BLOBSTAT> vBlobs;

	bool	gedFileExists();
	CString	getYear(_int64 diff);
	bool	getDBtype();
	int		getUserVersion();
	void	setUserVersion(int user_version);
	bool	insertIntoFiles(CString filename, int type);
	// Implementation

	void	message(CString caption, CString message);
	BOOL	dateDiff(CString date1, CString date2, int month);
	_int64	getDateI(CString dateS, int month);

	CString	getNotepadPath();

	CString	getLanguage(int language_id);
	CString	getSpouses(CString rowid, CString sex_id, std::vector<SPOUSES>* vektor, SPOUSES2* sp);

	CStringA MultiToUtf8(LPCSTR multi);
	bool	clearDatabase();
	BOOL	selectDatabase();
	void	createCaption();
	bool	selectHtml();
	bool	selectGedcom();
	void CDragApp::fillDbChildren();

	CString	contractions(void);

	_int64	getStartTime();
	CString	getPresentDate();
	CString	getPresentDateTime();

	CString getTimeElapsed(_int64 startTime);

	_int64  dateDiff(CString date1, CString date2);
	CString	getDateStrFromI(CString dateI);
	_int64	getDateIFromStr(CString dateS);
	void	showLogFile();
	void	showFile(CString fileSpec);
	void	showHtmlFile(CString fileSpec);
	void	editNotepad(CString file, CString lineNumber);
	void	notepad(CString fileSpec, CString option);
	bool	selectFile(CString fileSpec, BLOBSTAT* stat);
	bool	selectFiles();

	void insertVersion(CString inputMode);
	void change(CString linenumber, CString line);
	void htmlEditLines(CListCtrlEx* p_ListCtrl, int col_linenumber, CString title);
	void HtmlNotepadParents(CString rowid);

	void	saveHtmlLine(int lineNumber, CString line);
	BOOL	openDatabase(bool same);
	BOOL	openSystemDatabase();
	FILE* openLogFile(CString fname, CString title);
	CString openHtmlFile(FILE** fh, CString fileName, CString mode);
	CString	openTextFile(FILE** fl, CString fileName, CString mode);

	CString selectPeople(CString rowid, CString* linenumber);
	void	keress(CString searh, CListCtrlEx* m_listCtrl, int column, int start, bool exact);
	void	showItem(int nItem, CListCtrlEx* p_listCtrl);

	BOOL	query(CString command);
	BOOL	query1(CString command);
	BOOL	query2(CString command);
	BOOL	query3(CString command);
	BOOL	query4(CString command);
	BOOL	querySystem(CString command);
	BOOL	queryBlob(CString command);
	BOOL	queryName(CString command);
	BOOL	queryX(CSqliteDB* db, CString command);

	BOOL	execute(CString command);
	BOOL	executeSys(CString command);
	BOOL	executeSys8(CString command);
	BOOL	executeBlob(CString command);
	BOOL	executeX(CSqliteDB* db, CString command);

	void	selectAll(CListCtrlEx* p_ListCtrl);
	void	unselectAll(CListCtrlEx* p_ListCtrl);

	void	exportAll(CString logFile, CString title, CListCtrlEx* p_ListCtrl);
	void	exportSelected(CString str, CString title, CListCtrlEx* p_ListCtrl);
	BOOL	listTable(FILE* fl, CListCtrlEx* p_ListCtrl);
	void	underlining(FILE* fl, std::vector<BOOL>* selected, std::vector<COLUMNS>* columns);
	void	header(FILE* fl, std::vector<BOOL>* selected, std::vector<COLUMNS>* columns);

	void	listHtmlLine(int lineNumber);

	CString getFamily(CString familyNumber);
	CString getTable(CString tableNumber);

	CString	isFirstName(CString name);
	int		getNumberOfDb(std::vector<CString>* vE);
	CString getFileCreationDate(CString fileSpec);
	CString getHtmlLine(CString lineNumber);
	CString getFileSpec(int type);

	CWnd* _dlg;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTableFirstNames();
	afx_msg void OnUsermanual();
	afx_msg void OnHusbandWife();
	afx_msg void OnEmail();
	afx_msg void OnGithub();

	virtual int ExitInstance();


	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	afx_msg void OnAppAbout();
	afx_msg void OnOpen();

	afx_msg void OnListsAll();
	afx_msg void OnListsRowid();
	afx_msg void OnFirstnames();
	afx_msg void OnInputSzluha();
	afx_msg void OnInfoDb();
	afx_msg void OnMarriages();
	afx_msg void OnTables();
	afx_msg void OnInputfiles();
	afx_msg void OnUniteSamename();
	afx_msg void OnFullname();
	afx_msg void OnNotepad();
	afx_msg void OnBrowse();
	afx_msg void OnDescendantsFile();
	afx_msg void OnDescendantsLine();
	afx_msg void OnDescendantsTable();
	afx_msg void OnConv16to8();
	afx_msg void OnMarriagesFile();
	afx_msg void OnMarriagesLine();
	afx_msg void OnMarriagesTable();
	afx_msg void OnSpousesFile();
	afx_msg void OnSpousesLine();
	afx_msg void OnSpousesTable();
	afx_msg void OnHtmlInfo();
	afx_msg void OnBracketsBraces();
	afx_msg void OnBracketsRound();
	afx_msg void OnBracketsSquare();
	afx_msg void OnInputGedcom();
	afx_msg void OnInputManual();
	afx_msg void OnUpdateInputSzluha(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInputManual(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInputGedcom(CCmdUI* pCmdUI);
	afx_msg void OnUniteSamenamenot();
	afx_msg void OnUniteInfo();
	afx_msg void OnUpdateUniteSamename(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUniteSamenamenot(CCmdUI* pCmdUI);
	afx_msg void OnUnionShow();
	afx_msg void OnUpdateUnionShow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUniteInfo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHtmlInfo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrowse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNotepad(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBracketsBraces(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBracketsRound(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBracketsSquare(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDescendantsFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDescendantsLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDescendantsTable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFirstnames(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFullname(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGithub(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInputfiles(CCmdUI* pCmdUI);
	afx_msg void OnUpdateListsAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateListsRowid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMarriages(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMarriagesFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMarriagesLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMarriagesTable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSpousesFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSpousesLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSpousesTable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTables(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEmail(CCmdUI* pCmdUI);
	afx_msg void OnUpdateConv16to8(CCmdUI* pCmdUI);
	afx_msg void OnGedcomInfo();
	afx_msg void OnUpdateGedcomInfo(CCmdUI* pCmdUI);
	afx_msg void OnGedcomList();
	afx_msg void OnUpdateGedcomList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGedcomListTab(CCmdUI* pCmdUI);
	afx_msg void OnGedcomListTab();
	afx_msg void OnUpdateGedcomChain(CCmdUI* pCmdUI);
	afx_msg void OnGedcomChain();
	afx_msg void OnUpdateGedcomFilter(CCmdUI* pCmdUI);
	afx_msg void OnGedcomFilter();
	afx_msg void OnUpdateGedcomKnownUnknown(CCmdUI* pCmdUI);
	afx_msg void OnGedcomKnownUnknown();
	afx_msg void OnUpdateGedcomLevel0(CCmdUI* pCmdUI);
	afx_msg void OnGedcomLevel0();
	afx_msg void OnUpdateCheckSamenameSamemothername(CCmdUI* pCmdUI);
	afx_msg void OnCheckSamenameSamemothername();
	afx_msg void OnUpdateCheckSamenameSamebirth(CCmdUI* pCmdUI);
	afx_msg void OnCheckSamenameSamebirth();
	afx_msg void OnUpdateCheckSamenameSamefathername(CCmdUI* pCmdUI);
	afx_msg void OnCheckSamenameSamefathername();
	afx_msg void OnUpdateCheckSamenameSamedeath(CCmdUI* pCmdUI);
	afx_msg void OnCheckSamenameSamedeath();
	afx_msg void OnCheckMorespouses();
	afx_msg void OnUpdateCheckMorespouses(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCouplesDifferentwdates(CCmdUI* pCmdUI);
	afx_msg void OnCouplesDifferentwdates();
	afx_msg void OnUpdateCheckReferences(CCmdUI* pCmdUI);
	afx_msg void OnCheckReferences();
	afx_msg void OnUpdateCheckUnreferenced(CCmdUI* pCmdUI);
	afx_msg void OnCheckUnreferenced();
	afx_msg void OnUpdateCheckUnknownNames(CCmdUI* pCmdUI);
	afx_msg void OnCheckUnknownNames();
	afx_msg void OnUpdateDatesFamily(CCmdUI* pCmdUI);
	afx_msg void OnDatesFamily();
	afx_msg void OnUpdateDatesValid(CCmdUI* pCmdUI);
	afx_msg void OnDatesValid();
	afx_msg void OnCheckLifetime();
	afx_msg void OnUpdateCheckLifetime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCheckSpousedates(CCmdUI* pCmdUI);
	afx_msg void OnCheckSpousedates();
	afx_msg void OnUpdateMotherandchild(CCmdUI* pCmdUI);
	afx_msg void OnMotherandchild();
	afx_msg void OnUpdateFatherandchild(CCmdUI* pCmdUI);
	afx_msg void OnFatherandchild();
	afx_msg void OnUpdateBornAfterfatherdeath(CCmdUI* pCmdUI);
	afx_msg void OnBornAfterfatherdeath();
	afx_msg void OnUpdateBornAftermotherdeath(CCmdUI* pCmdUI);
	afx_msg void OnBornAftermotherdeath();
	afx_msg void OnUpdateCheckHusbandage(CCmdUI* pCmdUI);
	afx_msg void OnCheckHusbandage();
	afx_msg void OnUpdateCheckWifeage(CCmdUI* pCmdUI);
	afx_msg void OnCheckWifeage();
	afx_msg void OnUpdateCheckGenerationcode(CCmdUI* pCmdUI);
	afx_msg void OnCheckGenerationcode();
	afx_msg void OnUpdateCheckMotherindex(CCmdUI* pCmdUI);
	afx_msg void OnCheckMotherindex();
	afx_msg void OnUpdateCheckSpousesex(CCmdUI* pCmdUI);
	afx_msg void OnCheckSpousesex();
	afx_msg void OnUpdateCheckFolyt(CCmdUI* pCmdUI);
	afx_msg void OnCheckFolyt();
	afx_msg void OnUpdateCsaladTorzs(CCmdUI* pCmdUI);
	afx_msg void OnCsaladTorzs();
	afx_msg void OnUpdateCheckRepeatedFamilies(CCmdUI* pCmdUI);
	afx_msg void OnCheckRepeatedFamilies();
	afx_msg void OnUpdateMarriageAll(CCmdUI* pCmdUI);
	afx_msg void OnMarriageAll();
	afx_msg void OnUpdateMarriageName(CCmdUI* pCmdUI);
	afx_msg void OnMarriageName();
	afx_msg void OnUpdateMarriageRowid(CCmdUI* pCmdUI);
	afx_msg void OnMarriageRowid();
	afx_msg void OnHelp();
	afx_msg void OnUpdateUnite(CCmdUI* pCmdUI);
	afx_msg void OnUnite();
	afx_msg void OnAverageLifespan();
};
extern CDragApp theApp;