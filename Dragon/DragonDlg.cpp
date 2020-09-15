
// FaDlg.cpp : implementation file
//
#pragma once

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"
#include "winuser.h"
#include "build_defs.h"
#include "about.h"
#include "SelectOne.h"
#include "SelectedFiles.h"
#include "SelectHtmlFile.h"
#include "SelectCodesystem.h"
#include "ProgressWnd.h"
#include "Table_titles.h"
#include "Table_html.h"
#include "Info_db.h"
#include "Table_marriages.h"
#include "Table_tables.h"
#include "Table_blobs.h"
#include "GAtoDB.h"

#include "Table_files.h"
#include "GA_input.h"
#include "Table_GaTables.h"
#include "GetString.h"
#include "SelectTableName.h"
#include "textfile.h"
#include "Table_people.h"
#include "Setup.h"
#include "Filter.h"
#include "utilities.h"
#include <atlimage.h>
#include "html_Edit.h"
#include "FirstLetter.h"
#include "html_Brackets.h"
#include "EditPeople.h"
#include "gedcomin.h"

#include "checkDateFormat.h"
#include "checkGenerations.h"
#include "checkLifespan.h"
#include "CheckMotherDeath.h"
#include "CheckFatherDeath9.h"
#include "CheckSpousesDiff.h"
#include "CheckParentChild.h"
#include "CheckSpouseAge.h"
#include "CheckSpousesSex.h"
#include "CheckNames.h"
#include "CheckMotherIndex.h"
#include "checkSameNameAnd.h"
#include "checkSameSpouses.h"

#include "listPeopleAndSpouses.h"
#include "listPeopleAndFamily.h"

#include "SamePeople.h"
#include "GA_ascendantsChain.h"

#include "version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragonDlg::CDragonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDragonDlg::IDD, pParent)
{
	theApp._dlg = pParent;

	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_FA);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragonDlg::~CDragonDlg()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDragonDlg, CDialogEx)
//ON_WM_SYSCOMMAND()
//ON_WM_SIZE()

ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)

//Descendants
ON_COMMAND(ID_DESCENDANT_FILE, &CDragonDlg::OnDescendantFile)
ON_COMMAND(ID_DESCENDANT_TABLE, &CDragonDlg::OnDescendantTable)
ON_COMMAND(ID_DESCENDANT_LINE, &CDragonDlg::OnDescendantLine)

// házasságok
ON_COMMAND(ID_MARRIAGES_FILE, &CDragonDlg::OnMarriagesFile)
ON_COMMAND(ID_MARRIAGES_TABLE, &CDragonDlg::OnMarriagesTable)
ON_COMMAND(ID_MARRIAGES_LINE, &CDragonDlg::OnMarriagesLine)

// házastársak
ON_COMMAND(ID_SPOUSES_FILE, &CDragonDlg::OnSpousesFile)
ON_COMMAND(ID_SPOUSES_TABLE, &CDragonDlg::OnSpousesTable)
ON_COMMAND(ID_SPOUSES_LINE, &CDragonDlg::OnSpousesLine)

// házastársak házasságai
ON_COMMAND(ID_SS_FILE, &CDragonDlg::OnSsFile)
ON_COMMAND(ID_SS_TABLE, &CDragonDlg::OnSsTable)
ON_COMMAND(ID_SS_LINE, &CDragonDlg::OnSsLine)
ON_COMMAND(ID_SELECTED_FILES, &CDragonDlg::OnSelectedFiles)
ON_COMMAND(ID_TITLES, &CDragonDlg::OnTitles)
//ON_COMMAND(ID_GAHTML, &CDragonDlg::OnGahtml)

ON_WM_HSCROLL()
ON_COMMAND(ID_OPEN_DB, &CDragonDlg::OnOpenDb)
ON_COMMAND(ID_DATABASE_INFORMATION, &CDragonDlg::OnDatabaseInformation)
ON_COMMAND(ID_DISPLAY_MARRIAGES, &CDragonDlg::OnDisplayMarriages)
ON_COMMAND(ID_DISPLAY_FILESTABLE, &CDragonDlg::OnDisplayFilestable)
ON_COMMAND(ID_INFO_ONPROGRAM, &CDragonDlg::OnInfoOnprogram)
ON_COMMAND(ID_DISPLAY_INDIVIDUALS, &CDragonDlg::OnDisplayIndividuals)
ON_COMMAND(ID_DISPLAY_TABLES, &CDragonDlg::OnDisplayTables)
ON_COMMAND(ID_INPUT_GAHTML_FILE, &CDragonDlg::OnInputGahtmlFile)
ON_COMMAND(ID_INPUT_GAHTML_FAMILY, &CDragonDlg::OnInputGahtmlFamily)
ON_COMMAND(ID_INPUT_GAHTML_FROMLINE, &CDragonDlg::OnInputGahtmlFromline)
ON_COMMAND(ID_INPUT_GAHTML_LINE, &CDragonDlg::OnInputGahtmlLine)
ON_COMMAND(ID_INPUT_GAHTML_TABLE, &CDragonDlg::OnInputGahtmlTable)
ON_COMMAND(ID_CONNECT_BRANCHES, &CDragonDlg::OnConnectBranches)
ON_COMMAND(ID_INPUT_KEYBOARD, &CDragonDlg::OnInputKeyboard)
ON_WM_PAINT()
ON_COMMAND(ID_APP_EXIT, &CDragonDlg::OnAppExit)
ON_COMMAND(ID_BRACKETS_SQUARE, &CDragonDlg::OnBracketsSquare)
ON_COMMAND(ID_BRACKETS_ROUND, &CDragonDlg::OnBracketsRound)
ON_COMMAND(ID_BRACES, &CDragonDlg::OnBraces)
ON_COMMAND(ID_GENERATIONCODE, &CDragonDlg::OnGenerationcode)
ON_COMMAND(ID_FROMTABLE, &CDragonDlg::OnFromtable)
ON_COMMAND(ID_FROMFAMILY, &CDragonDlg::OnFromfamily)
ON_COMMAND(ID_CONNECT_CSALAD, &CDragonDlg::OnConnectCsalad)
ON_COMMAND(ID_CSALAD, &CDragonDlg::OnCsalad)
ON_COMMAND(ID_CSALAD_TORZS, &CDragonDlg::OnCsaladTorzs)
ON_COMMAND(ID_MOTHERDEATH, &CDragonDlg::OnMotherDeath)
ON_COMMAND(ID_CHECK_LIFESPAN, &CDragonDlg::OnCheckLifespan)
ON_COMMAND(ID_DATEFORMAT, &CDragonDlg::OnDateFormat)
ON_COMMAND(ID_INPUT_MANUAL, &CDragonDlg::OnInputManual)
ON_COMMAND(ID_GEDCOM_TAGS, &CDragonDlg::OnCheckTags)
ON_COMMAND(ID_GEDCOM_FAM, &CDragonDlg::OnGedcomFam)
ON_COMMAND(ID_GEDCOM_INDI, &CDragonDlg::OnGedcomIndi)
ON_COMMAND(ID_GEDCOM_PEOPLE, &CDragonDlg::OnListPeople)
ON_COMMAND(ID_GEDCOM_FILE, &CDragonDlg::OnGedcomFile)
ON_COMMAND(ID_GEDCOM_0LEVEL, &CDragonDlg::OnGedcom0level)
ON_COMMAND(ID_GEDCOM_HEAD, &CDragonDlg::OnGedcomHead)
ON_COMMAND(ID_GEDCOM_INPUT, &CDragonDlg::OnGedcomInput)
ON_COMMAND(ID_GEDCOM_FAMILIES, &CDragonDlg::OnListFamilyByName)
ON_COMMAND(ID_GEDCOM_VINDI, &CDragonDlg::OnGedcomVindi)
ON_COMMAND(ID_NEW_FAMILIES, &CDragonDlg::OnNewFamilies)
ON_WM_CLOSE()
ON_COMMAND(ID_CHECK_FATHER9, &CDragonDlg::OnCheckFather9)
ON_COMMAND(ID_CHECK_SPOUSEDIFF, &CDragonDlg::OnCheckSpousesDiff)
ON_COMMAND(ID_FATHERCHILD_GREATER, &CDragonDlg::OnFatherchildGreater)
ON_COMMAND(ID_FATHERCHILD_LESS, &CDragonDlg::OnFatherchildLess)
ON_COMMAND(ID_MOTHERCHILD_GREATER, &CDragonDlg::OnMotherchildGreater)
ON_COMMAND(ID_MOTHERCHILD_LESS, &CDragonDlg::OnMotherchildLess)
ON_COMMAND(ID_HUSBAND_AGE_G, &CDragonDlg::OnHusbandAgeG)
ON_COMMAND(ID_HUSBAND_AGE_L, &CDragonDlg::OnHusbandAgeL)
ON_COMMAND(ID_WIFE_AGE_G, &CDragonDlg::OnWifeAgeG)
ON_COMMAND(ID_WIFE_AGE_L, &CDragonDlg::OnWifeAgeL)
ON_COMMAND(ID_CHECK_SPOUSES_SEX, &CDragonDlg::OnCheckSpousesSex)
ON_COMMAND(ID_SAMENAMEANDBIRTH2, &CDragonDlg::OnSameNameAndBirth)
ON_COMMAND(ID_SAMENAMEANDDEATH2, &CDragonDlg::OnSameNameAndDeath)
ON_COMMAND(ID_SAMENAMEANDFATHER2, &CDragonDlg::OnSameNameAndFather)
ON_COMMAND(ID_SAMENAMEANDMOTHER2, &CDragonDlg::OnSameNameAndMother)
ON_COMMAND(ID_CHECK_SAMESPOUSENAME, &CDragonDlg::OnCheckSameSpouseName)
ON_COMMAND(ID_CHECK_SAMENAMES_SPOUSES, &CDragonDlg::OnCheckSameNamesSpouses)
ON_COMMAND(ID_LIST_PEOPLEANDFAMILY, &CDragonDlg::OnListPeopleAndFamily)
ON_COMMAND(ID_SAME_PEOPLE, &CDragonDlg::OnSamePeople)
ON_COMMAND(ID_SAME_PEOPLE_CONTRACT, &CDragonDlg::OnSamePeopleContract)
ON_COMMAND(ID_NAME_PROBLEMS, &CDragonDlg::OnNameProblems)
ON_COMMAND(ID_MOTHER_INDEX, &CDragonDlg::OnMotherIndex)
ON_COMMAND(ID_DISPLAY_BLOB, &CDragonDlg::OnDisplayBlob)
ON_COMMAND(ID_GEDCOM_TAGTABLE, &CDragonDlg::OnGedcomTagtable)
ON_COMMAND(ID_GEDCOM_INDIFAMS, &CDragonDlg::OnGedcomINDIFAMS)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
//	EASYSIZE_ADD( IDC_PICTURE,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
//	EASYSIZE_INIT()

	BUILT.Format( L"%s - %s", VERSION, MultiToUnicode( LPCSTR( build ) ) );
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

//	m_htmlDlg		= NULL;
	m_pIndividuals	= NULL;
	m_pMarriages	= NULL;

	m_pMarriagesF	= NULL;
	m_pMarriagesT	= NULL;
	m_pMarriagesL	= NULL;

	m_pDescendantsF	= NULL;
	m_pDescendantsT	= NULL;
	m_pDescendantsL	= NULL;

	m_pSpousesF	= NULL;
	m_pSpousesT	= NULL;
	m_pSpousesL	= NULL;

	mainTitle();

	ChangeMenu();

//	m_orderix = 1;
//	createListColumns();
//	OnFilterUnfilter();

	CWnd wndBottom;
// Az ablak helyének és méretének meghatározása., beállítása
	int bmpWidth	= 1024;
	int bmpHeight	= 678;
	int x = ( theApp._w - bmpWidth )/2;
	int y = ( theApp._h - bmpHeight )/2;

//	SetWindowPos( &wndBottom, x,y, bmpWidth + 16, bmpHeight + 60, SWP_NOREDRAW|SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos( &wndBottom, x,y, bmpWidth + 16, bmpHeight + 60, SWP_NOREDRAW|SWP_NOACTIVATE );
//	SetWindowPos( HWND_BOTTOM, x,y, bmpWidth + 16, bmpHeight + 60, SWP_NOREDRAW|SWP_NOACTIVATE | SWP_NOZORDER);

//	ShowWindow( SW_MAXIMIZE );
	

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnInfoOnprogram()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CDragonDlg::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor= (CELLCOLOR*) lParam;//a COLUMNCOLOR struktura deklaralva van a ListCtrlEx.h -ban.

	if(m_orderix==(int)wParam)
	{
		cColor->bg	= RGB(250,250,250);
		cColor->tx	= RGB(250,0,0);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CDragonDlg::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonDlg::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonDlg::query1( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonDlg::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonDlg::query3( CString command )
{
	if( m_recordset3->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonDlg::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnSelectedFiles()
{
	CString drive;
	CString	dir;
	CString	fname;
	CString	ext;
	CString database;
	
	database = theApp.m_databaseSpec;

	CSelectedFiles dlg;
	if( dlg.DoModal() == IDCANCEL ) return;

	theApp.m_texteditor = dlg.m_texteditor;
	theApp.WriteProfileStringW( L"Settings", L"texteditor", theApp.m_texteditor );

	splitFilespec( theApp.m_texteditor, &drive,&dir,&fname,&ext );
	theApp.m_editorName.Format( L"%s.%s", fname, ext );
	theApp.m_editorFolder.Format( L"%s:%s", drive, dir );

	if( database != theApp.m_databaseSpec )
	{
		theApp.openDatabase();
	}
	mainTitle();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::mainTitle( )
{
	CString caption;

	caption.Format( L"Dragon v. %s - %s || Családok nyilvántartása || adatbázis: %s || %s ||", BUILT, PLATFORM, theApp.m_databaseSpec, theApp.m_inputMode );
	SetWindowText( caption );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnTitles()
{
	CTitles dlg;
	if( dlg.DoModal() == IDCANCEL ) return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonDlg::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
//			CDialogEx::OnOK();	// exit
			return TRUE;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDatabaseInformation()
{
	CInfoDb dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// T Á B L Á Z A T O K   M E G N Y I T Á S A ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDisplayIndividuals()
{
	if( m_pIndividuals != NULL )
	{
		m_pIndividuals->ShowWindow(SW_RESTORE);		// néha kell!!
		m_pIndividuals->SetForegroundWindow();
	}
	else
	{
		m_pIndividuals = new CTablePeople(this);
//		m_pIndividuals->Create(CTablePeople::IDD,GetDesktopWindow());
		m_pIndividuals->Create( IDD_TABLE_PEOPLE,GetDesktopWindow());
		m_pIndividuals->ShowWindow(SW_SHOW);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDisplayMarriages()
{
	if( m_pMarriages )
	{
		m_pMarriages->ShowWindow(SW_RESTORE);
		m_pMarriages->SetForegroundWindow();
	}
	else
	{
		m_pMarriages = new CTableMarriages(this);
		m_pMarriages->Create(CTableMarriages::IDD,GetDesktopWindow());
		m_pMarriages->ShowWindow(SW_SHOW);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDisplayTables()
{
//	OnTabletables();
//}
//void CDragonDlg::OnTabletables()
//{
	CTableTables dlg;
	dlg.m_select = false;
	dlg.m_caption = L"Az adatbázisban lévõ táblák";
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDisplayFilestable()
{
//	OnFiles();
//}
//void CDragonDlg::OnFiles()
//{
	CTableFiles dlg;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnListUtf8()
{
	CString filespec;
	filespec = L"f:\\fa\\samlpefile.txt";
	//Create file. Use UTF-8 to encode the file
	CTextFileWrite myfile( filespec, CTextFileWrite::UTF_8 );

	
	ASSERT(myfile.IsOpen());

	//Write some text
	myfile << "Using 8 bit characters as input";
	myfile.WriteEndl();
	myfile << L"Using 16-bit characters. The following character is alfa: \x03b1";
	myfile.WriteEndl();
	CString temp = _T("Using CString.");
	myfile << temp;
	myfile.WriteEndl();
	myfile << L"öüóõúéáûí\nÖÜÓÕÚÉÁÛÍ";
	myfile.Close();
	theApp.showFile( filespec );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnOpenDb()
{
	if( m_pIndividuals != NULL )
		m_pIndividuals->OnClose();
	if( m_pMarriages )
		m_pMarriages->OnClose();

	if( !theApp.selectDatabase() ) return;
	
	while( !theApp.openDatabase() )
	{
		theApp.selectDatabase();
	}


	mainTitle();
	ChangeMenu();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::ChangeMenu()
{
	if( theApp.m_inputMode == GEDCOM )
	{
		OnMenuGedcom();
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		OnMenuGahtml();
	}
	else if( theApp.m_inputMode == URES )
	{
		OnMenuEmpty();
	}
	else if( theApp.m_inputMode == MANUAL )
	{
		OnMenuManual();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMenuEmpty()
{
	SetMenu(NULL);
	::DestroyMenu(GetMenu()->GetSafeHmenu());
	CMenu menu;
	menu.LoadMenuW(IDR_MAIN_EMPTY );
	SetMenu(&menu);	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMenuGahtml()
{
	SetMenu(NULL);
	::DestroyMenu(GetMenu()->GetSafeHmenu());
	CMenu menu;
	menu.LoadMenuW(IDR_MAIN_GAHTML );
	SetMenu(&menu);

	if( wcscmp( theApp.m_username, L"végesistvan" ) )
		menu.EnableMenuItem( 12, MF_BYPOSITION|MF_GRAYED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMenuGedcom()
{
	SetMenu(NULL);
	::DestroyMenu(GetMenu()->GetSafeHmenu());
	CMenu menu;
	menu.LoadMenuW(IDR_MAIN_GEDCOM);
	SetMenu(&menu);

	if( wcscmp( theApp.m_username, L"végesistvan" ) )
			menu.EnableMenuItem( 12, MF_BYPOSITION|MF_GRAYED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMenuManual()
{
	SetMenu(NULL);
	::DestroyMenu(GetMenu()->GetSafeHmenu());
	CMenu menu;
	menu.LoadMenuW(IDR_MAIN_MANUAL );
	theApp.m_inputMode = MANUAL;
	SetMenu(&menu);

	if( !theApp.m_cntPeople )
	{
		menu.EnableMenuItem( 3,MF_BYPOSITION| MF_GRAYED );
		menu.EnableMenuItem( 4,MF_BYPOSITION| MF_GRAYED );
		menu.EnableMenuItem( 5,MF_BYPOSITION| MF_GRAYED );
		menu.EnableMenuItem( 6,MF_BYPOSITION| MF_GRAYED );
		menu.EnableMenuItem( 7,MF_BYPOSITION| MF_GRAYED );
		menu.EnableMenuItem( 8,MF_BYPOSITION| MF_GRAYED );
	}
	else
	{
		menu.EnableMenuItem( 3,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 4,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 5,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 6,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 7,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 8,MF_BYPOSITION| MF_ENABLED );
	}
	if( wcscmp( theApp.m_username, L"végesistvan" ) )
		menu.EnableMenuItem( 12, MF_BYPOSITION|MF_GRAYED);
	DrawMenuBar();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnConnectBranches()
{
	CGaInput split;
	split.connectBranches();
	split.setDummyFather();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnInputKeyboard()
{
	OnMenuManual();
	if( !theApp.m_cntPeople )
		OnDisplayIndividuals();
	if( !theApp.m_cntPeople )
		OnMenuManual();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	displayTree();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::displayTree()
{
//	HBITMAP hBmp = (HBITMAP)LoadImage( NULL, theApp.m_bitmapSpec, IMAGE_BITMAP, 1024, 678,
//	LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
// "Dragonblood" a fa neva

	HBITMAP hBmp = (HBITMAP)LoadImage(
	AfxGetApp()->m_hInstance,
	MAKEINTRESOURCE(IDB_TREE),
	IMAGE_BITMAP,
	0,0,
	LR_DEFAULTCOLOR
	);

	BITMAP bm;
	::GetObject( hBmp, sizeof( bm ), &bm );
	int bmWidth = bm.bmWidth;
	int bmHeight = bm.bmHeight;


	CRect rect;
	GetClientRect(&rect);
	int x = (rect.Width() - bmWidth + 1) / 2;
	int y = (rect.Height() - bmHeight + 1) / 2;


	CPaintDC dc(this); // device context for painting

	CStatic *pBmp;
	pBmp = (CStatic*)GetDlgItem( IDC_PICTURE );
	pBmp->SetWindowPos( NULL,x,y, bmWidth, bmHeight, SWP_SHOWWINDOW ); 
	pBmp->SetBitmap( hBmp );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnAppExit()
{

	if( m_pIndividuals != NULL ) m_pIndividuals->DestroyWindow();
	if( m_pMarriages )
		m_pMarriages->DestroyWindow();
	
	m_pDescendantsF->DestroyWindow();
	m_pDescendantsT->DestroyWindow();
	m_pDescendantsL->DestroyWindow();
	m_pMarriagesF->DestroyWindow();
	m_pMarriagesT->DestroyWindow();
	m_pMarriagesL->DestroyWindow();

	m_pSpousesF->DestroyWindow();
	m_pSpousesT->DestroyWindow();
	m_pSpousesL->DestroyWindow();

	m_pSsF->DestroyWindow();
	m_pSsT->DestroyWindow();
	m_pSsL->DestroyWindow();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnBracketsSquare()
{
	CHtmlBrackets dlg;
	dlg._square = TRUE;
	dlg._round	= FALSE;
	dlg._braces	= FALSE;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnBracketsRound()
{
	CHtmlBrackets dlg;
	dlg._square = FALSE;
	dlg._round	= TRUE;
	dlg._braces	= FALSE;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnBraces()
{
	CHtmlBrackets dlg;
	dlg._square = FALSE;
	dlg._round	= FALSE;
	dlg._braces	= TRUE;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnManualInputDialog()
{
	OnDisplayIndividuals();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnInputManual()
{
	CEditPeople dlg;

	dlg.m_rowid.Empty();
	if( dlg.DoModal() == IDCANCEL ) return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnListPeople()
{
	CGedcomIn ged;
	ged.listPeople();
}
void CDragonDlg::OnListFamilyByName()
{
	CGedcomIn ged;
	ged.listFamiliesByName();
}

void CDragonDlg::OnCheckTags()
{
	CGedcomIn ged;
	ged.checkTags();

}
void CDragonDlg::OnGedcomTagtable()
{
	CGedcomIn ged;
	ged.tagTable();
}
void CDragonDlg::OnGedcomFam()
{
	CGedcomIn ged;
	ged.listFAM();
}


void CDragonDlg::OnGedcomIndi()
{
	CGedcomIn ged;
	ged.listINDI();
}

void CDragonDlg::OnGedcomFile()
{
	theApp.showFile( theApp.m_gedFileSpec );
}

void CDragonDlg::OnGedcom0level()
{
	CGedcomIn ged;
	ged.zeroLevel();
}


void CDragonDlg::OnGedcomHead()
{
	CGedcomIn ged;
	ged.listHEAD();
}


void CDragonDlg::OnGedcomInput()
{
	CGedcomIn ged;
	if( !ged.gedcomInput() ) return;

	m_pIndividuals = NULL;
	OnDisplayIndividuals();
	mainTitle();
	ChangeMenu();
}

void CDragonDlg::OnGedcomVindi()
{
	CGedcomIn ged;
	ged.listVindi();;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnNewFamilies()
{
	CString fname( L"root" );
	CString title( L"új család-root kapcsolat ellenõrzése" );
	CString attention;
	CString torzs;

	if( theApp.openLogFile( fname,title )==NULL)
		return;

	CString info = L"\
A program ellenõrzi, hogy az [xxx család]->[törzs: XXXX] kapcsolat létezik-e. Természetesen nem hiba ha az egyik\n\
vagy másik deklaráció nem létezik, tehát ez nem egy hibalista, csak egy kimutatás a html fájlban megadott\n\
család->törzs kapcsolatokról. Az alábbi felsorolásokban megtaláljuk a nemlétezõ és létezõ kapcsolatokat is.\n\n";

	fwprintf( theApp.fl, info );

	CString rowid;
	CString lineNumber;
	CString tableNumber;
	CString name;
	CString percent;
	CString csalad;
	CString newFamilies;
	CString familyName;
	CString family;
	CString roman;
	CString birth_date;
	CString death_date;

	CString rowidF;
	CString nameF;
	CString rowidM;
	CString nameM;
	CString children;
	CString rowidC;
	CString nameC;

	fwprintf( theApp.fl, L"Az adatbázisban létezõ család apítói és leszármazottai: ([XY család])\n\n" );
	m_command = L"SELECT rowid, lineNumber, first_name, last_name, csalad, tableNumber, birth_date, death_date FROM people WHERE csalad != '' AND gap == 0 ORDER BY csalad";
	if( !query( m_command ) ) return;


	int cnt = 0;
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		rowid		= m_recordset->GetFieldString( 0 );
		lineNumber	= m_recordset->GetFieldString( 1 );
		tableNumber	= m_recordset->GetFieldString( 5 );
		birth_date	= m_recordset->GetFieldString( 6 );
		death_date	= m_recordset->GetFieldString( 7 );
		name.Format( L"%s %s", m_recordset->GetFieldString( 3 ), m_recordset->GetFieldString( 2 ) );
		csalad		= m_recordset->GetFieldString( 4 );
		csalad.Trim();

		m_command.Format( L"SELECT familyName, tableRoman FROM tables WHERE rowid = '%s'", tableNumber ); // a családalapító ebbena táblában van
		if( !query3( m_command ) ) return;
		familyName	= m_recordset3->GetFieldString( 0 );
		roman		= m_recordset3->GetFieldString( 1 );
		family.Format( L"%s %s", familyName, roman );
		family.Trim();

		m_command.Format( L"SELECT rowid, torzs FROM tables WHERE familyName='%s'", csalad );		// erre a táblára mutat
		if( !query3( m_command ) ) return;

		cnt = 0;
		for( UINT j = 0; j < m_recordset3->RecordsCount(); ++j )  // több azonos nevû család is lehet!!!
		{
			torzs = m_recordset3->GetFieldString( 1 ); 
			torzs.Trim();

			if( !torzs.Compare( family ) )
			{
				fwprintf( theApp.fl, L"%-20s %8s %8s %-30s %-12s %-12s [%s család]\n", family, lineNumber, rowid, name, birth_date, death_date, csalad );

				//gyerekek
				m_command.Format( L"SELECT rowid, first_name, last_name, lineNumber, birth_date, death_date FROM people WHERE father_id ='%s'", rowid );
				if( !query3( m_command ) ) return;
					
				if( m_recordset3->RecordsCount() == 0 )
				{
					fwprintf( theApp.fl, L"!!!!!!!!!!!!!!!!!!!!\n" );
				}
				else
				{
					for( UINT j = 0; j < m_recordset3->RecordsCount(); ++j, m_recordset3->MoveNext() )
					{
						rowidC = m_recordset3->GetFieldString( 0);
						name.Format( L"%s %s", m_recordset3->GetFieldString( 2 ), m_recordset3->GetFieldString( 1 ) );
						lineNumber	= m_recordset3->GetFieldString( 3 );
						birth_date	= m_recordset3->GetFieldString( 4 );
						death_date	= m_recordset3->GetFieldString( 5 );

						attention.Empty();
						if( torzs.Compare( family ) ) 
							attention = L"!!!!!!";
						fwprintf( theApp.fl, L"%-20s %8s %8s %-30s %-12s %-12s [törzs: %s]%s\n", csalad, lineNumber, rowidC, name, birth_date, death_date,  torzs, attention );
					}
				}
				fwprintf( theApp.fl, L"\n" );
				break ;
			}

		}
	}
	fwprintf( theApp.fl, L"\n\n" );

	fwprintf( theApp.fl, L"Az adatbázisban nem létezõ család alapítói: ([XY család õse])\n\n" );

	m_command = L"SELECT rowid, lineNumber, first_name, last_name, csalad, tableNumber FROM people WHERE csalad != '' AND gap == 1 ORDER BY csalad";
	if( !query( m_command ) ) return;

	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		rowid		= m_recordset->GetFieldString( 0 );
		lineNumber	= m_recordset->GetFieldString( 1 );
		tableNumber	= m_recordset->GetFieldString( 5 );
		name.Format( L"%s %s", m_recordset->GetFieldString( 3 ), m_recordset->GetFieldString( 2 ) );
		csalad		= m_recordset->GetFieldString( 4 );
		csalad.Trim();

		m_command.Format( L"SELECT familyName, tableRoman FROM tables WHERE rowid = '%s'", tableNumber ); // a családalapító ebbena táblában van
		if( !query3( m_command ) ) return;
		familyName	= m_recordset3->GetFieldString( 0 );
		roman		= m_recordset3->GetFieldString( 1 );
		family.Format( L"%s %s", familyName, roman );
		family.Trim();



		fwprintf( theApp.fl, L"%-20s %8s %8s %-30s [%s család õse]\n", family, lineNumber, rowid, name, csalad );
	}
	fwprintf( theApp.fl, L"\n\n" );


	fwprintf( theApp.fl, L"Az alábbi [törzs: xxxx] jelzésû családoknak nincs alapítójuk ([xxxx csalad]):\n\n" );

//	m_command = L"SELECT rowid, * FROM tables WHERE root != '' ORDER BY root";
	m_command = L"SELECT rowid, * FROM tables WHERE familyname != '' ORDER BY familyname";
	if( !query1( m_command ) ) return;
	
	for( UINT i = 0; i < m_recordset1->RecordsCount(); ++i, m_recordset1->MoveNext() )
	{
		lineNumber	= m_recordset1->GetFieldString( TABLES_LINENUMBER );
		tableNumber = m_recordset1->GetFieldString( TABLES_ROWID );
		familyName	= m_recordset1->GetFieldString( TABLES_FAMILY_NAME );
		percent		= m_recordset1->GetFieldString( TABLES_PERCENT );
		csalad		= m_recordset1->GetFieldString( TABLES_TORZS );
		csalad.Trim();

		if( !csalad.IsEmpty() )
		{
			m_command.Format( L"SELECT rowid FROM tables WHERE familyName='%s' AND newFamilies LIKE '%c%s%c'", csalad, '%', familyName, '%' );
			if( !query2( m_command ) ) return;
		
			if( !m_recordset2->RecordsCount() )
			{
				fwprintf( fl, L"%6s %-3s %-20s [törzs: %s]\n", lineNumber, percent, familyName, csalad ); 
			}
		}
	}
	fwprintf( theApp.fl, L"\n\n" );


	fclose(theApp.fl);
	theApp.showLogFile();	

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnClose()
{
	if( m_pIndividuals != NULL )
		m_pIndividuals->CloseWindow();
	if( m_pMarriages )
		m_pMarriages->CloseWindow();

	CDialogEx::OnClose();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnGenerationcode()
{
	CcheckGenerations dlg;
	if( dlg.DoModal() == IDCANCEL ) return;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckLifespan()
{
	CLifespanParam dlg;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDateFormat()
{
	CDateFormat dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMotherDeath()
{
	CCheckMotherDeath dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckFather9()
{
	CCheckFatherDeath9 dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckSpousesDiff()
{
	CCheckSpousesDiff dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnFatherchildGreater()
{
	CCheckParentChild dlg;

	dlg.m_function = FCG;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnFatherchildLess()
{
	CCheckParentChild dlg;

	dlg.m_function = FCL;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMotherchildGreater()
{
	CCheckParentChild dlg;

	dlg.m_function = MCG;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMotherchildLess()
{
	CCheckParentChild dlg;

	dlg.m_function = MCL;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnHusbandAgeG()
{
	CCheckSpouseAge dlg;

	dlg.m_function = HG;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnHusbandAgeL()
{
	CCheckSpouseAge dlg;

	dlg.m_function = HL;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnWifeAgeG()
{
	CCheckSpouseAge dlg;

	dlg.m_function = WG;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnWifeAgeL()
{
	CCheckSpouseAge dlg;

	dlg.m_function = WL;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckSpousesSex()
{
	CCheckSpousesSex dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnSameNameAndBirth()
{
	CcheckSameNameAnd dlg;
	dlg.and = L"birth";
	dlg.DoModal();
}
void CDragonDlg::OnSameNameAndDeath()
{
	CcheckSameNameAnd dlg;
	dlg.and = L"death";
	dlg.DoModal();
}
void CDragonDlg::OnSameNameAndFather()
{
	CcheckSameNameAnd dlg;
	dlg.and = L"father";
	dlg.DoModal();
}
void CDragonDlg::OnSameNameAndMother()
{
	CcheckSameNameAnd dlg;
	dlg.and = L"mother";
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckSameSpouseName()
{
	CCheckSameSpouses dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckSameNamesSpouses()
{
	CListPeopleAndSpouses dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnListPeopleAndFamily()
{
	CListPeopleAndFamily dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnSamePeople()
{
	CSamePeople dlg;
	dlg._onlyList = true;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnSamePeopleContract()
{
	CSamePeople dlg;
	dlg._onlyList = false;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnNameProblems()
{
	CCheckNames dlg;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMotherIndex()
{
	CCheckMotherIndex dlg;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDisplayBlob()
{
	CTableBlobs dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CDragonDlg::OnGedcomINDIFAMS()
{
	CGedcomIn ged;
	ged.indiFams();
}
