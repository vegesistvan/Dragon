
#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "dragon.h"
#include "MainFrm.h"
#include "about.h"
#include "dragonDoc.h"
#include "dragonView.h"
#include "utilities.h"
#include "locale.h"
#include "ProgressWnd.h"
#include "OpenDatabase.h"
#include "version.h"
#include "Table_people.h"
#include "GetString.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDragApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDragApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_OPEN, &CDragApp::OnOpen)
	ON_COMMAND(ID_LISTS_ALL, &CDragApp::OnListsAll)
	ON_COMMAND(ID_FIRSTNAMES, &CDragApp::OnFirstnames)
	ON_COMMAND(ID_INPUT_SZLUHA, &CDragApp::OnInputSzluha)
	ON_COMMAND(ID_INFO_DB, &CDragApp::OnInfoDb)
	ON_COMMAND(ID_MARRIAGES, &CDragApp::OnMarriages)
	ON_COMMAND(IDC_TABLES, &CDragApp::OnTables)
	ON_COMMAND(ID_INPUTFILES, &CDragApp::OnInputfiles)
	ON_COMMAND(ID_LISTS_ROWID, &CDragApp::OnListsRowid)
	ON_COMMAND(ID_FULLNAME, &CDragApp::OnFullname)
	ON_COMMAND(ID_NOTEPAD, &CDragApp::OnNotepad)
	ON_COMMAND(ID_BROWSE, &CDragApp::OnBrowse)
	ON_COMMAND(ID_DESCENDANTS_FILE, &CDragApp::OnDescendantsFile)
	ON_COMMAND(ID_DESCENDANTS_LINE, &CDragApp::OnDescendantsLine)
	ON_COMMAND(ID_DESCENDANTS_TABLE, &CDragApp::OnDescendantsTable)
	ON_COMMAND(ID_CONV_16TO8, &CDragApp::OnConv16to8)
	ON_COMMAND(ID_MARRIAGES_FILE, &CDragApp::OnMarriagesFile)
	ON_COMMAND(ID_MARRIAGES_LINE, &CDragApp::OnMarriagesLine)
	ON_COMMAND(ID_MARRIAGES_TABLE, &CDragApp::OnMarriagesTable)
	ON_COMMAND(ID_SPOUSES_FILE, &CDragApp::OnSpousesFile)
	ON_COMMAND(ID_SPOUSES_LINE, &CDragApp::OnSpousesLine)
	ON_COMMAND(ID_SPOUSES_TABLE, &CDragApp::OnSpousesTable)
	ON_COMMAND(ID_HTML_INFO, &CDragApp::OnHtmlInfo)
	ON_COMMAND(ID_BRACKETS_BRACES, &CDragApp::OnBracketsBraces)
	ON_COMMAND(ID_BRACKETS_ROUND, &CDragApp::OnBracketsRound)
	ON_COMMAND(ID_BRACKETS_SQUARE, &CDragApp::OnBracketsSquare)
	ON_COMMAND(ID_GITHUB, &CDragApp::OnGithub)
	ON_COMMAND(ID_EMAIL, &CDragApp::OnEmail)
	ON_COMMAND(ID_INPUT_GEDCOM, &CDragApp::OnInputGedcom)
	ON_COMMAND(ID_INPUT_MANUAL, &CDragApp::OnInputManual)
	ON_UPDATE_COMMAND_UI(ID_INPUT_SZLUHA, &CDragApp::OnUpdateInputSzluha)
	ON_UPDATE_COMMAND_UI(ID_INPUT_MANUAL, &CDragApp::OnUpdateInputManual)
	ON_UPDATE_COMMAND_UI(ID_INPUT_GEDCOM, &CDragApp::OnUpdateInputGedcom)
	ON_COMMAND(ID_UNION_SHOW, &CDragApp::OnUnionShow)
	ON_UPDATE_COMMAND_UI(ID_UNION_SHOW, &CDragApp::OnUpdateUnionShow)
	ON_UPDATE_COMMAND_UI(ID_HTML_INFO, &CDragApp::OnUpdateHtmlInfo)
	ON_UPDATE_COMMAND_UI(ID_BROWSE, &CDragApp::OnUpdateBrowse)
	ON_UPDATE_COMMAND_UI(ID_NOTEPAD, &CDragApp::OnUpdateNotepad)
	ON_UPDATE_COMMAND_UI(ID_BRACKETS_BRACES, &CDragApp::OnUpdateBracketsBraces)
	ON_UPDATE_COMMAND_UI(ID_BRACKETS_ROUND, &CDragApp::OnUpdateBracketsRound)
	ON_UPDATE_COMMAND_UI(ID_BRACKETS_SQUARE, &CDragApp::OnUpdateBracketsSquare)
	ON_UPDATE_COMMAND_UI(ID_DESCENDANTS_FILE, &CDragApp::OnUpdateDescendantsFile)
	ON_UPDATE_COMMAND_UI(ID_DESCENDANTS_LINE, &CDragApp::OnUpdateDescendantsLine)
	ON_UPDATE_COMMAND_UI(ID_DESCENDANTS_TABLE, &CDragApp::OnUpdateDescendantsTable)
	ON_UPDATE_COMMAND_UI(ID_FIRSTNAMES, &CDragApp::OnUpdateFirstnames)
	ON_UPDATE_COMMAND_UI(ID_FULLNAME, &CDragApp::OnUpdateFullname)
	ON_UPDATE_COMMAND_UI(ID_GITHUB, &CDragApp::OnUpdateGithub)
	ON_UPDATE_COMMAND_UI(ID_INPUTFILES, &CDragApp::OnUpdateInputfiles)
	ON_UPDATE_COMMAND_UI(ID_LISTS_ALL, &CDragApp::OnUpdateListsAll)
	ON_UPDATE_COMMAND_UI(ID_LISTS_ROWID, &CDragApp::OnUpdateListsRowid)
	ON_UPDATE_COMMAND_UI(ID_MARRIAGES, &CDragApp::OnUpdateMarriages)
	ON_UPDATE_COMMAND_UI(ID_MARRIAGES_FILE, &CDragApp::OnUpdateMarriagesFile)
	ON_UPDATE_COMMAND_UI(ID_MARRIAGES_LINE, &CDragApp::OnUpdateMarriagesLine)
	ON_UPDATE_COMMAND_UI(ID_MARRIAGES_TABLE, &CDragApp::OnUpdateMarriagesTable)
	ON_UPDATE_COMMAND_UI(ID_OPEN, &CDragApp::OnUpdateOpen)
	ON_UPDATE_COMMAND_UI(ID_SPOUSES_FILE, &CDragApp::OnUpdateSpousesFile)
	ON_UPDATE_COMMAND_UI(ID_SPOUSES_LINE, &CDragApp::OnUpdateSpousesLine)
	ON_UPDATE_COMMAND_UI(ID_SPOUSES_TABLE, &CDragApp::OnUpdateSpousesTable)
	ON_UPDATE_COMMAND_UI(IDC_TABLES, &CDragApp::OnUpdateTables)
	ON_UPDATE_COMMAND_UI(ID_EMAIL, &CDragApp::OnUpdateEmail)
	ON_UPDATE_COMMAND_UI(ID_CONV_16TO8, &CDragApp::OnUpdateConv16to8)
	ON_UPDATE_COMMAND_UI(ID_GEDCOM_INFO, &CDragApp::OnUpdateGedcomInfo)
	ON_COMMAND(ID_GEDCOM_INFO, &CDragApp::OnGedcomInfo)
	ON_UPDATE_COMMAND_UI(ID_GEDCOM_LIST, &CDragApp::OnUpdateGedcomList)
	ON_COMMAND(ID_GEDCOM_LIST, &CDragApp::OnGedcomList)
	ON_UPDATE_COMMAND_UI(ID_GEDCOM_LIST_TAB, &CDragApp::OnUpdateGedcomListTab)
	ON_COMMAND(ID_GEDCOM_LIST_TAB, &CDragApp::OnGedcomListTab)
	ON_UPDATE_COMMAND_UI(ID_GEDCOM_CHAIN, &CDragApp::OnUpdateGedcomChain)
	ON_COMMAND(ID_GEDCOM_CHAIN, &CDragApp::OnGedcomChain)
	ON_UPDATE_COMMAND_UI(ID_GEDCOM_FILTER, &CDragApp::OnUpdateGedcomFilter)
	ON_COMMAND(ID_GEDCOM_FILTER, &CDragApp::OnGedcomFilter)
	ON_UPDATE_COMMAND_UI(ID_GEDCOM_KNOWN_UNKNOWN, &CDragApp::OnUpdateGedcomKnownUnknown)
	ON_COMMAND(ID_GEDCOM_KNOWN_UNKNOWN, &CDragApp::OnGedcomKnownUnknown)
	ON_UPDATE_COMMAND_UI(ID_GEDCOM_LEVEL0, &CDragApp::OnUpdateGedcomLevel0)
	ON_COMMAND(ID_GEDCOM_LEVEL0, &CDragApp::OnGedcomLevel0)
	ON_UPDATE_COMMAND_UI(ID_CHECK_SAMENAME_SAMEMOTHERNAME, &CDragApp::OnUpdateCheckSamenameSamemothername)
	ON_COMMAND(ID_CHECK_SAMENAME_SAMEMOTHERNAME, &CDragApp::OnCheckSamenameSamemothername)
	ON_UPDATE_COMMAND_UI(ID_CHECK_SAMENAME_SAMEBIRTH, &CDragApp::OnUpdateCheckSamenameSamebirth)
	ON_COMMAND(ID_CHECK_SAMENAME_SAMEBIRTH, &CDragApp::OnCheckSamenameSamebirth)
	ON_UPDATE_COMMAND_UI(ID_CHECK_SAMENAME_SAMEFATHERNAME, &CDragApp::OnUpdateCheckSamenameSamefathername)
	ON_COMMAND(ID_CHECK_SAMENAME_SAMEFATHERNAME, &CDragApp::OnCheckSamenameSamefathername)
	ON_UPDATE_COMMAND_UI(ID_CHECK_SAMENAME_SAMEDEATH, &CDragApp::OnUpdateCheckSamenameSamedeath)
	ON_COMMAND(ID_CHECK_SAMENAME_SAMEDEATH, &CDragApp::OnCheckSamenameSamedeath)
	ON_COMMAND(ID_CHECK_MORESPOUSES, &CDragApp::OnCheckMorespouses)
	ON_UPDATE_COMMAND_UI(ID_CHECK_MORESPOUSES, &CDragApp::OnUpdateCheckMorespouses)
	ON_UPDATE_COMMAND_UI(ID_COUPLES_DIFFERENTWDATES, &CDragApp::OnUpdateCouplesDifferentwdates)
		ON_COMMAND(ID_COUPLES_DIFFERENTWDATES, &CDragApp::OnCouplesDifferentwdates)
		ON_UPDATE_COMMAND_UI(ID_CHECK_REFERENCES, &CDragApp::OnUpdateCheckReferences)
		ON_COMMAND(ID_CHECK_REFERENCES, &CDragApp::OnCheckReferences)
		ON_UPDATE_COMMAND_UI(ID_CHECK_UNREFERENCED, &CDragApp::OnUpdateCheckUnreferenced)
		ON_COMMAND(ID_CHECK_UNREFERENCED, &CDragApp::OnCheckUnreferenced)
		ON_UPDATE_COMMAND_UI(ID_CHECK_UNKNOWN_NAMES, &CDragApp::OnUpdateCheckUnknownNames)
		ON_COMMAND(ID_CHECK_UNKNOWN_NAMES, &CDragApp::OnCheckUnknownNames)
		ON_UPDATE_COMMAND_UI(ID_DATES_FAMILY, &CDragApp::OnUpdateDatesFamily)
		ON_COMMAND(ID_DATES_FAMILY, &CDragApp::OnDatesFamily)
		ON_UPDATE_COMMAND_UI(ID_DATES_VALID, &CDragApp::OnUpdateDatesValid)
		ON_COMMAND(ID_DATES_VALID, &CDragApp::OnDatesValid)
		ON_COMMAND(ID_CHECK_LIFETIME, &CDragApp::OnCheckLifetime)
		ON_UPDATE_COMMAND_UI(ID_CHECK_LIFETIME, &CDragApp::OnUpdateCheckLifetime)
		ON_UPDATE_COMMAND_UI(ID_CHECK_SPOUSEDATES, &CDragApp::OnUpdateCheckSpousedates)
		ON_COMMAND(ID_CHECK_SPOUSEDATES, &CDragApp::OnCheckSpousedates)
		ON_UPDATE_COMMAND_UI(ID_MOTHERANDCHILD, &CDragApp::OnUpdateMotherandchild)
		ON_COMMAND(ID_MOTHERANDCHILD, &CDragApp::OnMotherandchild)
		ON_UPDATE_COMMAND_UI(ID_FATHERANDCHILD, &CDragApp::OnUpdateFatherandchild)
		ON_COMMAND(ID_FATHERANDCHILD, &CDragApp::OnFatherandchild)
		ON_UPDATE_COMMAND_UI(ID_BORN_AFTERFATHERDEATH, &CDragApp::OnUpdateBornAfterfatherdeath)
		ON_COMMAND(ID_BORN_AFTERFATHERDEATH, &CDragApp::OnBornAfterfatherdeath)
		ON_UPDATE_COMMAND_UI(ID_BORN_AFTERMOTHERDEATH, &CDragApp::OnUpdateBornAftermotherdeath)
		ON_COMMAND(ID_BORN_AFTERMOTHERDEATH, &CDragApp::OnBornAftermotherdeath)
		ON_UPDATE_COMMAND_UI(ID_CHECK_HUSBANDAGE, &CDragApp::OnUpdateCheckHusbandage)
		ON_COMMAND(ID_CHECK_HUSBANDAGE, &CDragApp::OnCheckHusbandage)
		ON_UPDATE_COMMAND_UI(ID_CHECK_WIFEAGE, &CDragApp::OnUpdateCheckWifeage)
		ON_COMMAND(ID_CHECK_WIFEAGE, &CDragApp::OnCheckWifeage)
		ON_UPDATE_COMMAND_UI(ID_CHECK_GENERATIONCODE, &CDragApp::OnUpdateCheckGenerationcode)
		ON_COMMAND(ID_CHECK_GENERATIONCODE, &CDragApp::OnCheckGenerationcode)
		ON_UPDATE_COMMAND_UI(ID_CHECK_MOTHERINDEX, &CDragApp::OnUpdateCheckMotherindex)
		ON_COMMAND(ID_CHECK_MOTHERINDEX, &CDragApp::OnCheckMotherindex)
		ON_UPDATE_COMMAND_UI(ID_CHECK_SPOUSESEX, &CDragApp::OnUpdateCheckSpousesex)
		ON_COMMAND(ID_CHECK_SPOUSESEX, &CDragApp::OnCheckSpousesex)
		ON_UPDATE_COMMAND_UI(ID_CHECK_FOLYT, &CDragApp::OnUpdateCheckFolyt)
		ON_COMMAND(ID_CHECK_FOLYT, &CDragApp::OnCheckFolyt)
		ON_UPDATE_COMMAND_UI(ID_CSALAD_TORZS, &CDragApp::OnUpdateCsaladTorzs)
		ON_COMMAND(ID_CSALAD_TORZS, &CDragApp::OnCsaladTorzs)
		ON_UPDATE_COMMAND_UI(ID_CHECK_REPEATED_FAMILIES, &CDragApp::OnUpdateCheckRepeatedFamilies)
		ON_COMMAND(ID_CHECK_REPEATED_FAMILIES, &CDragApp::OnCheckRepeatedFamilies)
		ON_UPDATE_COMMAND_UI(ID_MARRIAGE_ALL, &CDragApp::OnUpdateMarriageAll)
		ON_COMMAND(ID_MARRIAGE_ALL, &CDragApp::OnMarriageAll)
		ON_UPDATE_COMMAND_UI(ID_MARRIAGE_NAME, &CDragApp::OnUpdateMarriageName)
		ON_COMMAND(ID_MARRIAGE_NAME, &CDragApp::OnMarriageName)
		ON_UPDATE_COMMAND_UI(ID_MARRIAGE_ROWID, &CDragApp::OnUpdateMarriageRowid)
		ON_COMMAND(ID_MARRIAGE_ROWID, &CDragApp::OnMarriageRowid)
		ON_COMMAND(ID_HELP, &CDragApp::OnHelp)
		ON_UPDATE_COMMAND_UI(ID_UNITE, &CDragApp::OnUpdateUnite)
		ON_COMMAND(ID_UNITE, &CDragApp::OnUnite)
		ON_COMMAND(ID_AVERAGE_LIFESPAN, &CDragApp::OnAverageLifespan)
		ON_COMMAND(ID_MERGE_NAME, &CDragApp::OnMergeName)
		ON_COMMAND(ID_MERGE_NAME3, &CDragApp::OnMergeName3)
		ON_COMMAND(ID_MERGE_NAME2, &CDragApp::OnMergeName2)
		ON_COMMAND(ID_FIRSTNAMES_MERGE, &CDragApp::OnFirstnamesMerge)
		END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragApp::CDragApp() noexcept
{

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("drag.AppID.NoVersion"));

	m_nAppLook = 0;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragApp theApp;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
//		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	AfxInitRichEdit2();

	setlocale(LC_CTYPE, "Hungarian_Hungary.1250");

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey( L"dragon");
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDragonDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDragonView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	pM = (CMainFrame*)AfxGetMainWnd();
	pW = (CDragonView*)pM->GetActiveView();


	pCAT_DATABASE = (CMFCRibbonCategory*)pM->m_wndRibbonBar.GetCategory(CAT_DATABASE);
	pCAT_TABLES = (CMFCRibbonCategory*)pM->m_wndRibbonBar.GetCategory(CAT_TABLES);
	pCAT_CHECK1 = (CMFCRibbonCategory*)pM->m_wndRibbonBar.GetCategory(CAT_CHECK1);
	pCAT_MERGE = (CMFCRibbonCategory*)pM->m_wndRibbonBar.GetCategory(CAT_MERGE);
	pCAT_CHECK2 = (CMFCRibbonCategory*)pM->m_wndRibbonBar.GetCategory(CAT_CHECK2);
	pCAT_GAHTML = (CMFCRibbonCategory*)pM->m_wndRibbonBar.GetCategory(CAT_GAHTML);
	pCAT_GEDCOM = (CMFCRibbonCategory*)pM->m_wndRibbonBar.GetCategory(CAT_GEDCOM);
	pCAT_UTILITIES = (CMFCRibbonCategory*)pM->m_wndRibbonBar.GetCategory(CAT_UTILITIES);

	mainDB = new CSqliteDB;
	systemDB = new CSqliteDB;
	systemDB8 = new CSqliteDB;
	blobDB = new CSqliteDB;

	m_recordset = new CSqliteDBRecordSet;
	m_recordset1 = new CSqliteDBRecordSet;
	m_recordset2 = new CSqliteDBRecordSet;
	m_recordset3 = new CSqliteDBRecordSet;
	m_recordset4 = new CSqliteDBRecordSet;
	m_recordsetSystem = new CSqliteDBRecordSet;
	m_masterTable = new CSqliteDBRecordSet;
	m_recordsetName = new CSqliteDBRecordSet;
	m_recordsetBlob = new CSqliteDBRecordSet;
	m_recSetX = new CSqliteDBRecordSet;
	/***************************************************************************************************************/
	static const char* fileSpec = __FILE__;   // ez az exe fájl specifikációját adja meg!!

	GetModuleFileName(NULL, _startFileSpec, _MAX_PATH);
	splitFilespec(_startFileSpec, &drive, &path, &filename, &ext);
	CString exePath;
	exePath.Format(L"%s:%s", drive, path);
	CString rootDir = exePath.Left(exePath.ReverseFind('\\'));
	CString exeDir = exePath.Mid(exePath.ReverseFind('\\') + 1);
	CString nppinstaller;
	// verzió kiolvasása az exe fájlból ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD dwUnUsed;
	LPTSTR lpVI = NULL;

	DWORD dwLen = GetFileVersionInfoSize(LPCWSTR(_startFileSpec), &dwUnUsed);
	if (dwLen > 0)
	{
		lpVI = (LPTSTR)GlobalAlloc(GPTR, dwLen);
	}
	if (lpVI != NULL)
	{
		VS_FIXEDFILEINFO* lpFfi;
		wchar_t szBuffer[2048];
		UINT uLen = 0;

		GetFileVersionInfo(_startFileSpec, NULL, dwLen, lpVI);

		if (VerQueryValue(lpVI, L"\\", (LPVOID*)&lpFfi, &uLen))
		{
			swprintf_s(szBuffer, L"%d.%d.%d.%d", HIWORD(lpFfi->dwProductVersionMS), LOWORD(lpFfi->dwProductVersionMS),
				HIWORD(lpFfi->dwProductVersionLS), LOWORD(lpFfi->dwProductVersionLS));
			m_version = szBuffer;
			//			m_majorVersion =  HIWORD(lpFfi->dwProductVersionMS);
		}
		GlobalFree((HGLOBAL)lpVI);
	}


	m_version = VERSION;  // mégse azt használom, hanem a version.h-t


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// minden fájlnak a gragon.exe könyvtárában kell lenni, kivéve, ha a Visual Studióban dolgozunk
	if (!exeDir.CompareNoCase(L"Release") || !exeDir.CompareNoCase(L"Debug"))
	{
		m_dragonjpg.Format(L"%s\\SYSTEM\\dragon.jpg", rootDir);
		m_systemDatabaseSpec.Format(L"%s\\SYSTEM\\system.db", rootDir);
		m_systemDatabaseSpec8.Format(L"%s\\SYSTEM\\system8.db", rootDir);
		m_systemDatabaseSpec16.Format(L"%s\\SYSTEM\\system16.db", rootDir);
		m_helpFileSpec.Format(L"%s\\SYSTEM\\DragonHelp.chm", rootDir);
		nppinstaller.Format(L"%s\\SYSTEM\\npp.8.4.8.Installer.x64.exe", rootDir);
		m_editorFolderPath.Format(L"%s\\SYSTEM", rootDir);
		m_versionPathName.Format(L"%s\\SYSTEM\\version.h", rootDir);
	}
	else
	{
		m_dragonjpg.Format(L"%s\\dragon.jpg", exePath);
		m_systemDatabaseSpec.Format(L"%s\\system.db", exePath);
		m_systemDatabaseSpec8.Format(L"%s\\system8.db", exePath);
		m_systemDatabaseSpec16.Format(L"%s\\system16.db", exePath);
		m_helpFileSpec.Format(L"%s\\DragonHelp.chm", exePath);
		nppinstaller.Format(L"%s\\npp.8.4.8.Installer.x64.exe", exePath);
		m_versionPathName.Format(L"%s\\version.h", exePath);
		m_editorFolderPath = exePath;
	}
	/* main database ***********************************************************************************************/
	m_dbPathName = GetProfileString(L"dragon", L"m_dbPathName", L"");
	m_dbFolderPath = GetProfileString(L"dragon", L"m_dbFolderPath", L"");
	m_dbFileName = GetProfileString(L"dragon", L"m_dbFileName", L"");
	m_dbFileTitle = GetProfileString(L"dragon", L"m_dbFileTitle", L"");

	if (!openDatabase(false)) return false;

	// system database ***********keresztnevek*********************************************************************************************
	if (_waccess(m_systemDatabaseSpec, 0))
	{
		str.Format(L"%s fájl nem létezik! Ebbõl baj is lehet!", m_systemDatabaseSpec);
		AfxMessageBox(str);
	}
	if (!openSystemDatabase()) return false;
	/**************************************************************************************************************/
	if (_waccess(m_helpFileSpec, 0))
	{
		str.Format(L"%s fájl nem létezik!", m_helpFileSpec);
		AfxMessageBox(str);
	}
	/**************************************************************************************************************/
	m_editorFileName = L"Notepad++.exe";
	m_editorFolderPath = getNotepadPath();
	//	m_editorFolderPath.Format(L"%s\\SYSTEM", rootDir);
	str.Format(L"%s\\%s", m_editorFolderPath, m_editorFileName);
	if (_waccess(str, 0))
	{
		str = L"A Notepad++.exe program nem található.\nEnélkül a text fájl megjelenítés nem fog mûködni!\nInsatallálni akarod?\nAkkor kattints az Igen-re, ha nem akkor a Nem-re!";
		if ((AfxMessageBox(str, MB_ICONINFORMATION | MB_YESNO)) == IDYES)
			ShellExecute(NULL, L"open", nppinstaller, NULL, NULL, SW_SHOWNORMAL);
	}
	/**************************************************************************************************************/
	m_pszAppName = _tcsdup(_T("Dragon - családfa adatbázis"));

	m_fatherAgeMin = GetProfileInt(L"Settigs", L"fatherAgeMin", 18);
	m_fatherAgeMax = GetProfileInt(L"Settigs", L"fatherAgeMax", 70);

	m_motherAgeMin = GetProfileInt(L"Settigs", L"motherAgeMin", 16);
	m_motherAgeMax = GetProfileInt(L"Settigs", L"motherAgeMax", 50);


	HDC hdc = GetDC(NULL);
	_w = GetDeviceCaps(hdc, HORZRES);
	_h = GetDeviceCaps(hdc, VERTRES);
	ReleaseDC(NULL, hdc);

	// Az ablak helyének és méretének meghatározása., beállítása
	int bmpWidth = 1024;
	int bmpHeight = 678;
	//	bmpWidth = 1916;
	//	bmpHeight = 884;
	int x = (theApp._w - bmpWidth) / 2;
	int y = (theApp._h - bmpHeight) / 2;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->SetWindowPos(m_pMainWnd, x, y, bmpWidth + 16, bmpHeight + 60, SWP_NOREDRAW | SWP_SHOWWINDOW);

	m_pMainWnd->SetWindowText( m_caption );

	m_pMainWnd->UpdateWindow();
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CDragApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDragApp message handlers



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::LoadCustomState()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::SaveCustomState()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::selectDatabase()
{

	str.Format(L"Adatbázis fájl (*.db)|*.db|Minden fájl (*.*)|*.*|");
	CFileDialog dlg(TRUE, L".*", m_dbPathName, OFN_HIDEREADONLY | OFN_EXPLORER, str);
	dlg.m_ofn.lpstrTitle = L"Válaszd ki vagy adj meg egy új családfa adatbázis fájlt!";
	if (dlg.DoModal() == IDCANCEL)  return FALSE;

	m_dbPathName = dlg.GetPathName();
	m_dbFileName = dlg.GetFileName();
	m_dbFileTitle = dlg.GetFileTitle();
	m_dbFolderPath = m_dbPathName.Left(m_dbPathName.ReverseFind('\\'));  // a GetFolderPath nem mûködik!!!

	m_blobFileTitle = m_dbFileTitle;
	m_blobFileName.Format(L"%s_blob.db", m_dbFileTitle);
	m_blobPathName.Format(L"%s\\%s", m_dbFolderPath, m_blobFileName);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az m_databaseSpes-ben magadott adatrbázis fájlt megnyitja a COpenDatabase-el
// Ha a megnyitás sikertelen, akkor jelzi a problémát, majd törli az m_databasSpec-et és annak nyilvántartását. 
// Ha a megnyitás sikeres, akkor
// -  elkészíti az adatbázis útvonaélát, nevét, 
// -  megnyitja a BLOB adatbázis fájt. 
// - a main adatbázis 'files' táblájából kiveszi az adatbázis késztítéséhez felhasznált fájl specifikációját és megõrzi azt
// - a fenti fájl típusa lapján megadja az adatbázis típusát, ami lehet:
//	URES	- ha az adatbázis 'peoples' táblája üres	
//	GAHTML	- ha a 'files' táblában nyilvántartott bementi fájl ga.htm típusú
//	GEDCOM	- ha a 'files' táblában nyilvántartott bemeneti fájl típusa GEDCOM	
//	MANUAL	- ha a 'files' táblában nincs nyilvántartva bementi fájl 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::openDatabase(bool same)
{
	str.Format(L"%s adatbázis megnyitása és ellenõrzése...", m_dbFileName);
	CProgressWnd pWin(NULL, str);
	pWin.GoModal();

	COpenDatabase open;

	if (!same)   // true: a korábban is használt adatbázist nyitja meg, nem választható új
	{
		if (_waccess(m_dbPathName, 0))
		{
			if (!selectDatabase()) exit(0);
		}
	}

	open.m_dbPathName = m_dbPathName;
	open.m_connDB = mainDB;
	open.m_databaseTables = databaseTables;
	open.m_databaseIndexes = databaseIndexes;
	open.m_numberOfTables = numberOfTables;
	open.m_numberOfIndexes = numberOfIndexes;

	while (!open.openDatabase())
	{
		if (!same)
		{
			if (!selectDatabase()) return false;
			open.m_dbPathName = m_dbPathName;
		}
	}

	WriteProfileString(L"dragon", L"m_dbPathName", m_dbPathName); // jó adatbázis, eltenni
	WriteProfileString(L"dragon", L"m_dbFolderPath", m_dbFolderPath);
	WriteProfileString(L"dragon", L"m_dbFileName", m_dbFileName);
	WriteProfileString(L"dragon", L"m_dbFileTitle", m_dbFileTitle);

	m_workingDirectory.Format(L"%s\\lists", m_dbFolderPath);
	if (_waccess(m_workingDirectory, 0))
		_wmkdir(m_workingDirectory);

	if (!query(L"SELECT count() FROM people")) return FALSE;
	m_cntPeople = _wtoi(m_recordset->GetFieldString(0));

	if (!same)
	{
		if (!getDBtype()) return FALSE;  // ha ugyanolyan nevû üres adatbázist nyit meg, akkor nem állítja át a bemeneti fájl változókat
	}

	if (theApp.m_inputMode == URES)
	{
		pM->m_wndRibbonBar.ShowCategory(CAT_DATABASE, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_TABLES, false);
		pM->m_wndRibbonBar.ShowCategory(CAT_CHECK1, false);
		pM->m_wndRibbonBar.ShowCategory(CAT_MERGE, false);
		pM->m_wndRibbonBar.ShowCategory(CAT_CHECK2, false);
		pM->m_wndRibbonBar.ShowCategory(CAT_GAHTML, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_GEDCOM, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_UTILITIES, false);
		
		pM->m_wndRibbonBar.SetActiveCategory(pCAT_DATABASE, false);
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		pM->m_wndRibbonBar.ShowCategory(CAT_TABLES, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_CHECK1, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_MERGE, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_CHECK2, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_GAHTML, true);

		pM->m_wndRibbonBar.ShowCategory(CAT_GEDCOM, false);
		pM->m_wndRibbonBar.SetActiveCategory(pCAT_TABLES, false);

	}
	else if (theApp.m_inputMode == GEDCOM)
	{
		pM->m_wndRibbonBar.ShowCategory(CAT_TABLES, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_CHECK1, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_MERGE, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_CHECK2, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_GAHTML, false);

		pM->m_wndRibbonBar.ShowCategory(CAT_GEDCOM, true);
		pM->m_wndRibbonBar.SetActiveCategory(pCAT_TABLES, false);
	}
	else if (theApp.m_inputMode == MANUAL)
	{
		pM->m_wndRibbonBar.ShowCategory(CAT_TABLES, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_CHECK1, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_MERGE, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_CHECK2, true);
		pM->m_wndRibbonBar.ShowCategory(CAT_GAHTML, false);

		pM->m_wndRibbonBar.ShowCategory(CAT_GEDCOM, false );
		pM->m_wndRibbonBar.SetActiveCategory(pCAT_TABLES, false);
		fillDbChildren();

	}
//	if (theApp.m_inputMode != GAHTML)
	{
		// blbob db fájl megnyitása

		m_blobFileTitle = GetProfileString(L"dragon", L"m_blobFileTitle", L"");
		m_blobFileName = GetProfileString(L"dragon", L"m_blboFileName", L"");
		m_blobPathName = GetProfileString(L"dragon", L"m_blobPathName", L"");

		m_blobFileTitle.Format(L"%s_blob", m_dbFileTitle);
		m_blobFileName.Format(L"%s.db", m_blobFileTitle);
		m_blobPathName.Format(L"%s\\%s", m_dbFolderPath, m_blobFileName);


		open.m_dbPathName = m_blobPathName;
		open.m_connDB = blobDB;
		open.m_databaseTables = blobStructure;
		open.m_databaseIndexes = blobIndexes;
		open.m_numberOfTables = blobNumberOfTables;
		open.m_numberOfIndexes = numberOfBlobIndexes;

		while (!open.openDatabase())
		{
			if (!same)
			{
				if (!selectDatabase()) return false;
				open.m_dbPathName = m_blobPathName;
			}
		}
		WriteProfileString(L"dragon", L"m_blobFileTitle", m_blobFileTitle);
		WriteProfileString(L"dragon", L"m_blobFileName", m_blobFileName);
		WriteProfileString(L"dragon", L"m_blobPathName", m_blobPathName); // jó adatbázis, eltenni
	}





	createCaption();
	pWin.DestroyWindow();
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::insertVersion(CString inputMode)
{
	m_inputVersion = m_version;
	m_inputMode = inputMode;
	m_command.Format(L"INSERT INTO properties (inputVersion, inputMode) VALUES ('%s', '%s')", m_inputVersion, m_inputMode);
	theApp.execute(m_command);
	m_uniteVersion.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::createCaption()
{
	m_caption.Format(L"Dragon v. %s (%s ", m_version, m_dbPathName);
	if (m_inputMode == GAHTML)
		m_caption += m_htmlFileName; 
	else if (m_inputMode == GEDCOM)
		m_caption += m_gedFileName;
	else if (m_inputMode == MANUAL)
		m_caption += L"kézi adatbevitel";
	else if (m_inputMode == URES)
		m_caption += L"üres adatbázis";
	m_caption += L")";

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::openSystemDatabase()
{
	COpenDatabase open;

	open.m_dbPathName = m_systemDatabaseSpec;
	open.m_connDB = systemDB;
	open.m_databaseTables = systemTables;
	open.m_databaseIndexes = systemIndexes;
	open.m_numberOfTables = numberOfSystemTables;
	open.m_numberOfIndexes = numberOfSystemIndexes;

	if (!open.openDatabase()) return false;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDragApp::selectHtml()
{
	CFileDialog dlg(TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"html files (*.htm)|*.htm|(*.html)|*.html|All Files (*.*)|*.*||");
	dlg.m_ofn.lpstrTitle = L"Válaszd ki a kívánt GA.html fájlt";
	dlg.m_ofn.lpstrInitialDir = m_dbFolderPath;
	dlg.m_ofn.Flags = OFN_NOCHANGEDIR;

	if (dlg.DoModal() == IDCANCEL) return false;

	m_htmlPathName = dlg.GetPathName();
	m_htmlFileTitle = dlg.GetFileTitle();
	m_htmlFileName = dlg.GetFileName();
	m_htmlFolderPath = dlg.GetFolderPath();
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDragApp::selectGedcom()
{
	CFileDialog dlg(TRUE, L".", (LPCTSTR)m_gedPathName, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"gedcom files (*.ged)|*.ged|All Files (*.*)|*.*||");

	dlg.m_ofn.lpstrTitle = L"Válaszd ki a kívánt GEDCOM fájlt";
	dlg.m_ofn.lpstrInitialDir = m_dbFolderPath;
	dlg.m_ofn.Flags = OFN_NOCHANGEDIR;
	if (dlg.DoModal() == IDCANCEL) return false;

	//	m_gedFileTitle.Empty();
	//	m_gedFileName.Empty();
	//	m_gedPathName.Empty();

	//	CString gedFolderPath;
	//	while (true)
	//	{
	m_gedFileTitle = dlg.GetFileTitle();
	m_gedFileName = dlg.GetFileName();
	m_gedPathName = dlg.GetPathName();
	m_gedFolderPath = dlg.GetFolderPath();

	m_gedFolderPath = m_gedPathName.Left(m_gedPathName.ReverseFind('\\'));  // a GetFolderPath nem mûködik!!!
	/*
			if (m_gedFolderPath != m_dbFolderPath)
			{
				str.Format(L"Csak a\n%s\nmappából választhatsz GEDCOM fájlt!", m_dbFolderPath);
				AfxMessageBox(str, MB_ICONEXCLAMATION);
				return false;
			}
	*/

	//	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDragApp::clearDatabase()
{
	if (AfxMessageBox(L"Tényleg törölni karod az adatbázis a blob fájjal együtt?", MB_YESNO) == IDNO) return false;

	if (mainDB->IsConnected()) mainDB->Close();
	CFile::Remove(m_dbPathName);
	if (!_waccess(m_blobPathName, 0))
	{
		if (blobDB->IsConnected()) blobDB->Close();
		CFile::Remove(m_blobPathName);
	}

	if (!openDatabase(true)) return false;	// true: jelzi az openDatabase-nek, hogy ne kérdezzen rá az adatbázis nevére,
	// ugyanolyan névvel nyissa meg az adatbázist. Ebben az esetben egy üreset,
	// m_inputMode = URES beállítással. Input fájl változók törölve lesznek.
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDragApp::getDBtype()
{
	CString filename;
	CString filespec;
	CString ext;
	CString created;
	CString warning;
	CFileStatus status;
	CString ctime;
	int pos;

	m_htmlFileTitle.Empty();
	m_htmlFileName.Empty();
	m_htmlPathName.Empty();

	m_gedFileTitle.Empty();
	m_gedFileName.Empty();
	m_gedPathName.Empty();

	m_inputVersion.Empty();
	m_uniteVersion.Empty();
	m_inputMode.Empty();

	if (!m_cntPeople)
	{
		m_inputVersion.Empty();
		m_uniteVersion.Empty();
		m_inputMode = URES;
		return true;
	}
	else
	{
		if (!query(L"SELECT rowid, * FROM properties")) return FALSE;
		m_inputVersion = m_recordset->GetFieldString(PROP_INPUTVERSION);
		m_uniteVersion = m_recordset->GetFieldString(PROP_UNITEVERSION);
		m_inputMode = m_recordset->GetFieldString(PROP_INPUTMODE);
		if (m_inputMode.IsEmpty())
		{
			m_inputMode = MANUAL;
			return true;
		}
		else
		{
			m_command = L"SELECT filespec, created  FROM filespec WHERE rowid = 1";
			if (!query(m_command)) return false;

			if (!m_recordset->RecordsCount())
			{
				m_inputMode = MANUAL;
				return true;
			}

			filename = m_recordset->GetFieldString(0);
			created = m_recordset->GetFieldString(1);
			if ((pos = filename.ReverseFind('.')) == -1)
			{
				str.Format(L"getDBtype\n%s\n%s\nfájlnév kiterjesztés nélkül!!", theApp.m_dbPathName, filename);
				AfxMessageBox(str, MB_ICONEXCLAMATION);
				return false;
			}
			filespec.Format(L"%s\\%s", m_dbFolderPath, filename);
			if (_waccess(filespec, 0))
			{
				str.Format(L"Az adatbázisban nyilvántartott\n%s\nfájl nem létezik!\nA program azért mûködik, de ezt fájlt kezelõ funkciók nem!", filespec);
				AfxMessageBox(str, MB_ICONEXCLAMATION);
			}
			else if (CFile::GetStatus(filespec, status))
			{
				ctime = status.m_mtime.Format(L"%Y.%m.%d %H:%M:%S");
				if (ctime != created)
				{
					warning = L"Az adatbázis és a htm fájl nem felel meg egymásnak,\nde azért a program mûködik, csak a fájlt kezelõ funkciók hibásak lehetnek!";
					str.Format(L"%s\nfájl módosításának\nnyilvántartott dátuma:\t%s\nmegállapított dátuma:\t%s\n\n%s", filespec, created, ctime, warning);
					AfxMessageBox(str, MB_ICONASTERISK);
				}
			}

			ext = filename.Mid(pos + 1);
			if (ext == L"htm" || ext == L"html" || ext == L"HTM" || ext == L"HTML")
			{
				m_inputMode = GAHTML;
				m_htmlFileName = filename;
				m_htmlFileTitle = filename.Left(pos);
				m_htmlPathName.Format(L"%s\\%s", m_dbFolderPath, m_htmlFileName);
			}
			else if (ext == L"ged" || ext == L"GED")
			{
				m_inputMode = GEDCOM;
				m_gedFileName = filename;
				m_gedFileTitle = filename.Left(pos);
				m_gedPathName.Format(L"%s\\%s", m_dbFolderPath, m_gedFileName);
			}

			if (m_inputMode.IsEmpty())
			{
				str.Format(L"%s\nfájl nem ismert fájltípus!");
				AfxMessageBox(str, MB_ICONEXCLAMATION);
				return false;
			}
		}
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::query(CString command)
{
	if (m_recordset->Query(command, mainDB))
	{
		str.Format(L"%s\n\n%s\n%s", m_dbPathName, command, m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::query1(CString command)
{
	if (m_recordset1->Query(command, mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset1->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::query2(CString command)
{
	if (m_recordset2->Query(command, mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::query3(CString command)
{
	if (m_recordset3->Query(command, mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset3->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::query4(CString command)
{
	if (m_recordset4->Query(command, mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::querySystem(CString command)
{
	if (m_recordsetSystem->Query(command, systemDB))
	{
		str.Format(L"%s\n%s", command, m_recordsetSystem->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::queryBlob(CString command)
{
	if (m_recordsetBlob->Query(command, blobDB))
	{
		str.Format(L"%s\n%s", command, m_recordsetBlob->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::queryName(CString command)
{
	if (m_recordsetName->Query(command, systemDB))
	{
		str.Format(L"%s\n%s", command, m_recordsetName->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::queryX(CSqliteDB* db, CString command)
{
	if (m_recSetX->Query(command, db))
	{
		str.Format(L"%s\n%s", command, db->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::execute(CString command)
{
	if (mainDB->Execute(command))
	{
		str.Format(L"%s\n%s", command, mainDB->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::executeSys(CString command)
{
	if (systemDB->Execute(command))
	{
		str.Format(L"%s\n%s", command, systemDB->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::executeSys8(CString command)
{
	if (systemDB8->Execute(command))
	{
		str.Format(L"%s\n%s", command, systemDB8->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::executeBlob(CString command)
{
	if (blobDB->Execute(command))
	{
		str.Format(L"%s\n%s", command, blobDB->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::executeX(CSqliteDB* db, CString command)
{
	if (db->Execute(command))
	{
		str.Format(L"%s\n%s", command, db->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getNotepadPath()
{
	str = L"SOFTWARE\\Notepad++";
	int tLen = str.GetLength() + 1;
	LPTSTR path = new TCHAR[tLen];
	_tcscpy_s(path, tLen, str.GetBuffer());

	// First call: get a large enough size to allocate destination string buffer
	DWORD keyType = 0;
	DWORD dataSize = 0;
	const DWORD flags = RRF_RT_REG_SZ; // Only read strings (REG_SZ)
	LONG result = ::RegGetValueW(
		HKEY_LOCAL_MACHINE,
		path,
		NULL,
		flags,
		&keyType,
		nullptr, // pvData == nullptr ? Request buffer size
		&dataSize);
	if (result)
	{
		return L"1";
		AfxMessageBox(L"A Notepad++.exe program nem található. Enélkül a text fájl megjelenítés nem fog mûködni!\nHa insatallálni akarod, akkor kattints az OK-ra, ha nem akkor a Cancel-re!", MB_ICONEXCLAMATION);
		//		str.Format(L"Hiba a Notepad++ registry bejegyzés hosszának\nbeolvasása során!\nA Notepad++ programot telepíteni kell!\nSystem error code: %d", result);
		//		AfxMessageBox(str);
		//		AtlThrow(HRESULT_FROM_WIN32(result));
	}

	// Create a CString with large enough internal buffer
	CString text;
	const DWORD bufferLength = dataSize / sizeof(WCHAR); // length in WCHAR's
	WCHAR* const textBuffer = text.GetBuffer(bufferLength);

	// Second call: read string value from the Registry into local string buffer
	result = ::RegGetValue(
		HKEY_LOCAL_MACHINE,
		path,
		NULL,
		flags,
		nullptr,
		textBuffer, // Write string in this destination buffer
		&dataSize);

	if (result)
	{
		return(L"");
		AfxMessageBox(L"A Notepad++.exe programot nem található.\nA text fájl megjelnítés nem fog mûködni!(2)", MB_ICONEXCLAMATION);
		//		str.Format(L"Hiba a Notepad++ útvonalának registry-bõl\n történõ beolvasása során!\nA Notepad++ programot telepíteni kell!\nSystem error code: %d", result);
		//		AfxMessageBox(str);
		//		return L"";
	}
	return text;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateOpen(CCmdUI* pCmdUI)
{

}
void CDragApp::OnOpen()
{
	if (!selectDatabase()) return;

	if (!openDatabase(false)) return;
	createCaption();
	m_pMainWnd->SetWindowText(m_caption);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnAverageLifespan()
{
	typedef struct
	{
		int century;
		int lifeSpan;
		int db;
	}LIFESPAN;
	std::vector<LIFESPAN> vLife;
	LIFESPAN life;
	int birth;
	int death;
	int century;
	int lifeSpan;
	bool uj;
	FILE* flAv;
	CString filePathName;
	filePathName.Format(L"%s\\averageLifeSpan_%s.text", m_workingDirectory, getTimeTag());

	if (!openFileSpec(&flAv, filePathName, L"w+")) return;
	fwprintf(flAv, L"Átlagos élettartam évszázadonként\n\n");

	m_command = L"SELECT birth_date, death_date FROM people WHERE length(birth_date)!=0 AND length(death_date)!=0 ORDER BY birth_date";
	if (!query(m_command)) return;

	for (int i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext())
	{
		str = m_recordset->GetFieldString(0);
		if (!isNumeric(str) || (str.GetLength()) < 4) continue;
		birth = _wtoi( str.Left(4) );
		str = m_recordset->GetFieldString(1);
		if (!isNumeric(str) || (str.GetLength()) < 4) continue;
		death = _wtoi(str.Left(4));
		if (birth > death) continue;
		if (999 < birth && birth < 2024 && 999 < death && death < 2024)
		{
			lifeSpan = death - birth;
			century = birth / 100;

			uj = false;
			for (int j = 0; j < vLife.size(); ++j)
			{
				if (vLife.at(j).century == century)
				{
					vLife.at(j).lifeSpan += lifeSpan;
					++vLife.at(j).db;
					uj = true;
					break;
				}
			}
			if (uj) continue;
			life.century = century;
			life.db = 1;
			life.lifeSpan = lifeSpan;
			vLife.push_back(life);
		}
	}
	fwprintf(flAv, L"%6s %5s %s\n", L"évsz.", L"átlag", L"db");
	for (int i = 0; i < vLife.size(); ++i)
	{
		str.Format(L"%5d. %5d %d\n", vLife.at(i).century + 1, vLife.at(i).lifeSpan / vLife.at(i).db, vLife.at(i).db);
		fwprintf(flAv, str);
	}
	fclose(flAv);
	showFile(filePathName);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::fillDbChildren()
{
	CString rowid;
	int numOfChildren;

	m_command = L"SELECT rowid, father_id FROM people";
	if (!query(m_command)) return;

	execute( L"BEGIN" );
	for (int i = 0; i < m_recordset->RecordsCount(); ++i)
	{
		rowid = m_recordset->GetFieldString(0);
		m_command.Format(L"SELECT rowid FROM people WHERE father_id = '%s'", rowid);
		if (!query1(m_command)) return;
		numOfChildren = m_recordset1->RecordsCount();
		m_command.Format(L"UPDATE people SET numOfChildren = '%d' WHERE rowid = '%s'", numOfChildren, rowid);
		if (!execute(m_command)) return;
		m_recordset->MoveNext();
	}
	execute(L"COMMIT");
}
