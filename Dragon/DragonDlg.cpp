
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
#include "SelectHtmlFile.h"
#include "ProgressWnd.h"
#include "Table_html.h"
#include "Info_db.h"
#include "Table_marriages.h"
#include "Table_tables.h"
#include "Table_blobs.h"

#include "Table_files.h"
#include "Table_GaTables.h"
#include "GetString.h"
#include "SelectTableName.h"

#include "Table_people.h"
#include "Setup.h"
#include "Filter.h"
#include "utilities.h"
#include <atlimage.h>
#include "editHtmlLine.h"
#include "html_Brackets.h"
#include "EditPeople.h"

#include "checkDateFormat.h"
#include "CheckMotherDeath.h"
#include "CheckFatherDeath.h"
#include "CheckSpousesDiff.h"
#include "CheckParentChild.h"
#include "CheckSpouseAge.h"
#include "CheckSpousesSex.h"
#include "checkNames.h"
#include "CheckMotherIndex.h"
#include "checkSameNameAnd.h"
#include "checkSameSpouses.h"
#include "checkParents.h"

#include "ConnectCsaladTorzs.h"

#include "checkLifespan.h"

#include "checkGenerations.h"
#include "CheckFamilyDates.h"
#include "CheckFatherChildren.h"
#include "checkOrderWife.h"
#include "checkNumOfSpouses.h"
#include "checkNumOfChildren.h"
#include "checkUnreferencedItems.h"
#include "checkIntegrity.h"

#include "SelectFamilyName.h"

#include "inputGA.h"
#include "GEDCOM_input.h"
#include "inputGEDCOM.h"
#include "GEDCOM_file.h"
#include "Brackets.h"
#include "html_File.h"
#include "CDialogRichText.h"
#include "utilities_dragon.h"

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
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_FA);

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

ON_MESSAGE(WM_MAIN_TITLE, OnMainTitle )

// házasságok
ON_COMMAND(ID_MARRIAGES_FILE, &CDragonDlg::OnMarriagesFile)
ON_COMMAND(ID_MARRIAGES_TABLE, &CDragonDlg::OnMarriagesTable)
ON_COMMAND(ID_MARRIAGES_LINE, &CDragonDlg::OnMarriagesLine)


// házastársak házasságai
ON_COMMAND(ID_SS_FILE, &CDragonDlg::OnSsFile)
ON_COMMAND(ID_SS_TABLE, &CDragonDlg::OnSsTable)
ON_COMMAND(ID_SS_LINE, &CDragonDlg::OnSsLine)

ON_WM_HSCROLL()
ON_COMMAND(ID_OPEN_DB, &CDragonDlg::OnOpenDb)
ON_COMMAND(ID_DATABASE_INFORMATION, &CDragonDlg::OnDatabaseInformation)
ON_COMMAND(ID_DISPLAY_MARRIAGES, &CDragonDlg::OnDisplayMarriages)
ON_COMMAND(ID_DISPLAY_FILESTABLE, &CDragonDlg::OnDisplayFilestable)
ON_COMMAND(ID_INFO_ONPROGRAM, &CDragonDlg::OnInfoOnprogram)
ON_COMMAND(ID_DISPLAY_INDIVIDUALS, &CDragonDlg::OnDisplayIndividuals)
ON_COMMAND(ID_DISPLAY_TABLES, &CDragonDlg::OnDisplayTables)

ON_WM_PAINT()
ON_COMMAND(ID_CSALAD_TORZS, &CDragonDlg::OnCsaladTorzs)  // Private menübõl ( Család kapcsolatok )		listáz és összeköt

ON_COMMAND(ID_CHECK_LIFESPAN, &CDragonDlg::OnCheckLifespan)
ON_COMMAND(ID_CHECK_DATEFORMAT, &CDragonDlg::OnCheckDateFormat)
ON_COMMAND(ID_INPUT_MANUAL, &CDragonDlg::OnInputManual)
ON_COMMAND(ID_GEDCOM_TAGS, &CDragonDlg::OnCheckTags)
ON_COMMAND(ID_GEDCOM_PEOPLE, &CDragonDlg::OnListPeople)
ON_COMMAND(ID_GEDCOM_INPUT, &CDragonDlg::OnGedcomInput)
ON_COMMAND(ID_GEDCOM_FAMILIES, &CDragonDlg::OnListFamilyByName)
ON_COMMAND(ID_GEDCOM_VINDI, &CDragonDlg::OnGedcomVindi)

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
ON_COMMAND(ID_CHECK_NAMES, &CDragonDlg::OnCheckNames)
ON_COMMAND(ID_CHECK_MOTHERINDEX, &CDragonDlg::OnMotherIndex)
ON_COMMAND(ID_DISPLAY_BLOB, &CDragonDlg::OnDisplayBlob)
ON_COMMAND(ID_GEDCOM_INDIFAMS, &CDragonDlg::OnGedcomINDIFAMS)
ON_WM_CLOSE()
ON_COMMAND(ID_DISPLAY_FAMILIES, &CDragonDlg::OnDisplayFamilies)
ON_COMMAND(ID_CHECK_WEDDINGDATE, &CDragonDlg::OnCheckWeddingDate)
ON_COMMAND(ID_SPOUSES_SEX, &CDragonDlg::OnSpousesSex)
ON_COMMAND(ID_UNREFERENCED_ITEMS, &CDragonDlg::OnUnreferencedItems)
ON_COMMAND(ID_CHECK_FOLYT, &CDragonDlg::OnCheckFolyt)
ON_COMMAND(ID_UNITE_ENTRIES, &CDragonDlg::OnUniteEntries)
ON_COMMAND(ID_FATHER_SAMECHILDREN, &CDragonDlg::OnFatherSamechildren)
ON_COMMAND(ID_CHECK_NUMOFSPOUSES, &CDragonDlg::OnCheckNumofspouses)
ON_COMMAND(ID_CHECK_ORDERWIFE, &CDragonDlg::OnCheckOrderwife)
ON_COMMAND(ID_CHECK_NUMOFCHILDREN, &CDragonDlg::OnCheckNumofchildren)
ON_COMMAND(ID_INPUTGA, &CDragonDlg::OnInputGA)
ON_COMMAND(ID_OL_LIST, &CDragonDlg::OnOlList)
ON_COMMAND(ID_CHECK_FAMILYDATES, &CDragonDlg::OnCheckFamilyDates)
ON_COMMAND(ID_CHECK_GENERATIONS, &CDragonDlg::OnCheckGenerations)
ON_COMMAND(ID_CHECK_INEGRITY, &CDragonDlg::OnCheckIntegrity)
ON_COMMAND(ID_CHECK_MOTHERDEATH, &CDragonDlg::OnCheckMotherDeath)
ON_COMMAND(ID_INPUT_GEDCOM, &CDragonDlg::OnInputGedcom)
ON_COMMAND(ID_LIST_HEAD, &CDragonDlg::OnListHEAD)
ON_COMMAND(ID_LIST_ZEROLEVEL, &CDragonDlg::OnListZero)
ON_COMMAND(ID_LIST_FILE, &CDragonDlg::OnListFile)
ON_COMMAND(ID_GEDCOM_VFAM, &CDragonDlg::OnGedcomVFAM)
ON_COMMAND(ID_LIST_CHAIN, &CDragonDlg::OnListChain)
ON_COMMAND(ID_LIST_FILE_TAB, &CDragonDlg::OnListFileTab)
ON_COMMAND(ID_FILTER_IRREGULAR, &CDragonDlg::OnFilterIrregular)
ON_COMMAND(ID_FOLYT_LINES, &CDragonDlg::OnFolytLines)
ON_COMMAND(ID_DESCENDANT_FILE, &CDragonDlg::OnDescendantFile)
ON_COMMAND(ID_DESCENDANT_LINE, &CDragonDlg::OnDescendantLine)
ON_COMMAND(ID_DESCENDANT_TABLE, &CDragonDlg::OnDescendantTable)
ON_COMMAND(ID_GA_EDITOR, &CDragonDlg::OnGaEditor)
ON_COMMAND(ID_GA_OPEN, &CDragonDlg::OnGaOpen)
ON_COMMAND(ID_REPEATED_FAMILIES, &CDragonDlg::OnRepeatedFamilies)
ON_COMMAND(ID_CHECK_PARENTS, &CDragonDlg::OnCheckParents)
ON_COMMAND(ID_RICHEDIT, &CDragonDlg::OnRichEdit)
//ON_COMMAND(ID_GEDCOM_SELECT, &CDragonDlg::OnGedcomSelect)
ON_COMMAND(ID_BRACKETS_ROUND, &CDragonDlg::OnBracketsRound)
ON_COMMAND(ID_BRACKETS_SQUARE, &CDragonDlg::OnBracketsSquare)
ON_COMMAND(ID_BRACKETS_BRACES, &CDragonDlg::OnBracketsBraces)
//ON_COMMAND(ID_MARRIAGES2, &CDragonDlg::OnMarriages2)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	BUILT.Format( L"%s - %s", theApp.m_version, MultiToUnicode( LPCSTR( BUILD ) ) );

	// modless dialogs

//	m_pIndividuals	= NULL;
//	m_pMarriages	= NULL;
//	m_pFamilies		= NULL;

//	m_pMarriagesF	= NULL;
//	m_pMarriagesT	= NULL;
//	m_pMarriagesL	= NULL;

//	m_pDescendantsF	= NULL;
//	m_pDescendantsT	= NULL;
//	m_pDescendantsL	= NULL;

//	m_pSpousesF	= NULL;
//	m_pSpousesT	= NULL;
//	m_pSpousesL	= NULL;


	SetWindowText(theApp.m_caption);
	ChangeMenu();

	CWnd wndBottom;
// Az ablak helyének és méretének meghatározása., beállítása
	int bmpWidth	= 1024;
	int bmpHeight	= 678;
	int x = ( theApp._w - bmpWidth )/2;
	int y = ( theApp._h - bmpHeight )/2;

//	SetWindowPos( &wndBottom, x,y, bmpWidth + 16, bmpHeight + 60, SWP_NOREDRAW|SWP_HIDEWINDOW );
	SetWindowPos(&wndBottom, x, y, bmpWidth + 16, bmpHeight + 60, SWP_NOREDRAW | SWP_SHOWWINDOW);


//	ShowWindow( SW_MAXIMIZE );

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CDragonDlg::OnMainTitle(WPARAM wParam, LPARAM lParam)
{
	mainTitle();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::mainTitle()
{
	theApp.createCaption();
	SetWindowText( theApp.m_caption);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::ChangeMenu()
{
	CString filespec;
	SetMenu(NULL);
	::DestroyMenu(GetMenu()->GetSafeHmenu());
	if (theApp.m_inputMode == GEDCOM)
	{
		menu.LoadMenuW(IDR_MAIN_GEDCOM);
	}
	else if (theApp.m_inputMode == GAHTML)
	{
		menu.LoadMenuW(IDR_MAIN_GAHTML);
	}
	else if (theApp.m_inputMode == URES)
	{
		menu.LoadMenuW(IDR_MAIN_EMPTY);
	}
	else if (theApp.m_inputMode == MANUAL)
	{
		menu.LoadMenuW(IDR_MAIN_MANUAL);
	}
	else
	{
		str.Format(L"%s\nértelmezhetetlen adatbázis típus!", theApp.m_inputMode);
		AfxMessageBox(str);
		CDialog::OnCancel();
		return;
	}
	SetMenu(&menu);

	// nem használható menüpontok lezárása
	if (theApp.m_inputMode == URES)
	{
		if (wcscmp(theApp.m_username, L"végesistvan"))
			menu.EnableMenuItem(11, MF_BYPOSITION | MF_GRAYED);
	}

	if (theApp.m_inputMode == GEDCOM)
	{
		if (wcscmp(theApp.m_username, L"végesistvan"))
			menu.EnableMenuItem(15, MF_BYPOSITION | MF_GRAYED);

		if (_waccess(theApp.m_gedPathName, 0))
			menu.EnableMenuItem(13, MF_BYPOSITION | MF_GRAYED);
	}
	if (theApp.m_inputMode == GAHTML)
	{
		if (wcscmp(theApp.m_username, L"végesistvan"))
			menu.EnableMenuItem(14, MF_BYPOSITION | MF_GRAYED);
	}
	else if (theApp.m_inputMode == MANUAL)
	{
		if (!theApp.m_cntPeople)
		{
			menu.EnableMenuItem(3, MF_BYPOSITION | MF_GRAYED);
			menu.EnableMenuItem(4, MF_BYPOSITION | MF_GRAYED);
			menu.EnableMenuItem(5, MF_BYPOSITION | MF_GRAYED);
			menu.EnableMenuItem(6, MF_BYPOSITION | MF_GRAYED);
			menu.EnableMenuItem(7, MF_BYPOSITION | MF_GRAYED);
			menu.EnableMenuItem(8, MF_BYPOSITION | MF_GRAYED);
		}
		else
		{
			menu.EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
			menu.EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
			menu.EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);
			menu.EnableMenuItem(6, MF_BYPOSITION | MF_ENABLED);
			menu.EnableMenuItem(7, MF_BYPOSITION | MF_ENABLED);
			menu.EnableMenuItem(8, MF_BYPOSITION | MF_ENABLED);
		}
		if (wcscmp(theApp.m_username, L"végesistvan"))
			menu.EnableMenuItem(16, MF_BYPOSITION | MF_GRAYED);
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	HBITMAP hBmp = (HBITMAP)LoadImage(
		AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(IDB_TREE),
		IMAGE_BITMAP,
		0, 0,
		LR_DEFAULTCOLOR
	);

	BITMAP bm;
	::GetObject(hBmp, sizeof(bm), &bm);
	int bmWidth = bm.bmWidth;
	int bmHeight = bm.bmHeight;


	CRect rect;
	GetClientRect(&rect);
	int x = (rect.Width() - bmWidth + 1) / 2;
	int y = (rect.Height() - bmHeight + 1) / 2;

	CStatic* pBmp;
	pBmp = (CStatic*)GetDlgItem(IDC_PICTURE);
	pBmp->SetWindowPos(NULL, x, y, bmWidth, bmHeight, SWP_SHOWWINDOW);
	pBmp->SetBitmap(hBmp);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
			return true;						// hogy ne lépjen ki Enter-re
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	if( m_recordset1->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnOpenDb()
{
/*
	if( m_pIndividuals != NULL )
		m_pIndividuals->OnClose();
	if( m_pMarriages )
		m_pMarriages->OnClose();
//	if( m_pFamilies )
//		m_pFamilies->OnClose();
*/
 	if( !theApp.selectDatabase() ) return;
	
	if( !theApp.openDatabase( false )) return;
	mainTitle();
	ChangeMenu();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnClose()
{
//	if (AfxMessageBox(L"Tényleg be akarod zárni a programot?", MB_YESNO) == IDNO) return;



	//	if( m_pIndividuals != NULL )	m_pIndividuals->CloseWindow();
	//	if( m_pMarriages != NULL )		m_pMarriages->CloseWindow();
	//	if( m_pMarriagesF  != NULL )	m_pMarriagesF->CloseWindow();
	//	if( m_pMarriagesT  != NULL )	m_pMarriagesT->CloseWindow();
	//	if( m_pMarriagesL  != NULL )	m_pMarriagesL->CloseWindow();

	//	if( m_pFamilies  != NULL )		m_pFamilies->CloseWindow();

	//	if( m_pDescendantsF != NULL )	m_pDescendantsF->CloseWindow();
	//	if( m_pDescendantsT != NULL )	m_pDescendantsT->CloseWindow();
	//	if( m_pDescendantsL != NULL )	m_pDescendantsL->CloseWindow();

	//	if( m_pSpousesF != NULL )		m_pSpousesF->CloseWindow();
	//	if( m_pSpousesT != NULL )		m_pSpousesT->CloseWindow();
	//	if( m_pSpousesL != NULL )		m_pSpousesL->CloseWindow();

	//	if( m_pSsF != NULL )			m_pSsF->CloseWindow();
	//	if( m_pSsT != NULL )			m_pSsT->CloseWindow();
	//	if( m_pSsL != NULL )			m_pSsL->CloseWindow();

	CDialogEx::OnClose();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// játék
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnOlList()
{
	{
		CString fileSpec;
		FILE* fl;
		fileSpec.Format(L"d:\\ol.html");
		if (!openFileSpec(&fl, fileSpec, L"w+")) return;

		int c = 0;
		int i;
		int j;
		TCHAR kar;

		for (j = 0; j < 10; ++j)
		{
			for (i = 0 + c; i < 52; ++i)
			{
				if (i > 25)
					kar = TCHAR('A') + 6 + i;
				else
					kar = TCHAR('A') + i;
				fwprintf(fl, L"<ol><li>%c</li>\n", kar);
			}
			for (i = 50; i > -1; --i)
			{
				if (i > 25)
					kar = TCHAR('A') + 6 + i;
				else
					kar = TCHAR('A') + i;
				fwprintf(fl, L"</ol><li>%c</li>\n", kar);
			}
			c = 1;
		}
		fclose(fl);
		theApp.showHtmlFile(fileSpec);
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckParents()
{
	CCheckParents dlg;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CDragonDlg::OnGedcomSelect()
{
	if (!theApp.selectGedcom()) return;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnRichEdit()
{
	CDialogRichText dlg;
	dlg.DoModal();
}
