// TableIndividuals.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GEDCOM table

#include "stdafx.h"
#include "dragon.h"
#include "utilities.h"
#include "Table_people.h"
#include "afxdialogex.h"
#include "Filter.h"
#include "Relatives.h"
#include "SelectOne.h"
#include "Table_files.h"
#include "Table_tables.h"
#include "GetString.h"
#include "Commctrl.h"
#include "SameParams.h"
#include "editHtmlLine.h"
#include "editHtmlLines.h"
#include "GetLastFirst.h"
#include "EditPeople.h"
#include "pictures.h"
#include "Relatives.h"
#include "Gedcom_out.h"
#include "Message.h"
#include "ProgressWnd.h"
#include "EditPeople.h"
#include "Relatives.h"
#include "Table_people_columns.h"
#include "utilities_dragon.h"
#include "compareEntries.h"
#include "compareAscendants.h"
#include "ascendants.h"
#include "CDescendants.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySource(const UNITE_ENTRIES& v1, const UNITE_ENTRIES& v2)
{
	return(v1.source < v2.source);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTablePeople, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTablePeople::CTablePeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTablePeople::IDD, pParent)
{
//	m_dlg = pParent;

	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTablePeople::~CTablePeople()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
	DDX_Control(pDX, IDC_CAPTION, captionCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTablePeople, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)

	ON_COMMAND(ID_FILTER_UNFILTER, &CTablePeople::OnFilterUnfilter)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTablePeople::OnDblclkList)
	ON_MESSAGE(WM_SYNCRONIZE_PEOPLE, OnSyncronize )
// DROPDOWN menü funkciók
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_DB_EDIT, &CTablePeople::OnEditUpdate )
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CTablePeople::OnHtmlNotepadParents )
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CTablePeople::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_HTML_EDIT, &CTablePeople::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CTablePeople::OnHtmlNotepad)	
	ON_COMMAND(ID_HTML_CHILDREN, &CTablePeople::OnHtmlChildren)
	ON_COMMAND(ID_COMPARE_2PEOPLE, &CTablePeople::OnCompare2People)
	ON_COMMAND(ID_COMPARE_ASC, &CTablePeople::OnCompareAsc)


	ON_COMMAND(ID_EDIT_DELETE, &CTablePeople::OnEditDelete)
	ON_COMMAND(ID_EDIT_UPDATE, &CTablePeople::OnEditUpdate )
	ON_COMMAND(ID_EDIT_INSERT, &CTablePeople::OnEditInsert)


	ON_COMMAND(ID_FILTER_MEN, &CTablePeople::OnFilterMen)
	ON_COMMAND(ID_FILTER_NODESCENDANT, &CTablePeople::OnFilterNodescendant)
	ON_COMMAND(ID_FILTER_NOPARENTS, &CTablePeople::OnFilterNoparents)
	ON_COMMAND(ID_FILTER_NOSPOUSE, &CTablePeople::OnFilterNospouse)
	ON_COMMAND(ID_FILTER_WOMEN, &CTablePeople::OnFilterWomen)
	ON_COMMAND(ID_FILTER_ANCESTORS, &CTablePeople::OnFilterAncestors)
	ON_COMMAND(ID_FILTER_FIRSTNAME, &CTablePeople::OnFilterFirstname)
	ON_COMMAND(ID_FILTER_LASTNAME, &CTablePeople::OnFilterLastname)
	ON_COMMAND(ID_FILTER_FAMILY, &CTablePeople::OnFilterFamily)
	ON_COMMAND(ID_FILTER_TABLE, &CTablePeople::OnFilterTable)
	ON_COMMAND(ID_FILTER_INDIVIDUAL, &CTablePeople::OnFilterIndividual)
	ON_COMMAND(ID_EXPORT_SELECTED, &CTablePeople::OnExportSelected)
	ON_COMMAND(ID_EXPORT_ALL, &CTablePeople::OnExportAll)
ON_COMMAND(ID_FILTER_FOLYT, &CTablePeople::OnFilterFolyt)
ON_WM_CLOSE()
ON_COMMAND(ID_AZONOSEMBER, &CTablePeople::OnAzonosemberIdent)
ON_COMMAND(ID_GROUPBY_BIRTHPLACE, &CTablePeople::OnGroupbyBirthplace)
ON_COMMAND(ID_GROUPBY_DEATHPLACE, &CTablePeople::OnGroupbyDeathplace)
ON_COMMAND(ID_GROUPBY_FIRSTNAME, &CTablePeople::OnGroupbyFirstname)
ON_COMMAND(ID_GROUPBY_LASTNAME, &CTablePeople::OnGroupbyLastname)
ON_COMMAND(ID_GROUPBY_TITOLO, &CTablePeople::OnGroupbyTitolo)
ON_COMMAND(ID_FILTER_DESCENDANTS, &CTablePeople::OnFilterDescendants)
ON_COMMAND(ID_FILTER_FATHERS, &CTablePeople::OnFilterFathers)
ON_COMMAND(ID_FILTER_MOTHERS, &CTablePeople::OnFilterMothers)
ON_COMMAND(ID_FILTER_SPOUSES, &CTablePeople::OnFilterSpouses)
ON_COMMAND(ID_FILTER_SPOUSESPOUSES, &CTablePeople::OnFilterSpousespouses)
ON_WM_SHOWWINDOW()
ON_COMMAND(ID_GROUPBY_BIRTHDATE, &CTablePeople::OnGroupbyBirthDate)
ON_COMMAND(ID_GROUPBY_DEATHDATE, &CTablePeople::OnGroupbyDeathdate)
ON_COMMAND(ID_GROUPBY_COMMENT, &CTablePeople::OnGroupbyComment)

ON_COMMAND(ID_GIVENNAME, &CTablePeople::OnGivenname)
ON_COMMAND(ID_INPUT_MANUAL, &CTablePeople::OnInputManual)
ON_WM_NCDESTROY()
ON_COMMAND(ID_GEDCOM_OUTPUT, &CTablePeople::OnGedcomOutput)
ON_COMMAND(ID_FILTER_BISEX, &CTablePeople::OnFilterBisex)
ON_STN_CLICKED(IDC_STATIC_KERESS, &CTablePeople::OnClickedKeress)

ON_COMMAND(ID_INFO, &CTablePeople::OnInfo)
ON_STN_CLICKED(IDC_NEXT, &CTablePeople::OnClickedNext)
ON_COMMAND(ID_SET_ANCESTOR, &CTablePeople::OnSetAncestor)
ON_COMMAND(ID_GEDCOM_INDI, &CTablePeople::OnGedcomIndi)
ON_COMMAND(ID_FILTER_TABLE_ANCESTORS, &CTablePeople::OnFilterTableAncestors)
ON_COMMAND(ID_FILTER_FAMILY_ANCESTORS, &CTablePeople::OnFilterFamilyAncestors)
ON_COMMAND(ID_GROUPBY_POSTERIOR, &CTablePeople::OnGroupbyPosterior)
ON_COMMAND(ID_UNITE, &CTablePeople::OnUnite)
ON_COMMAND(ID_KEKULE, &CTablePeople::OnKekule)
ON_COMMAND(ID_DESCENDANTS_NEW, &CTablePeople::OnDescendants)
ON_COMMAND(ID_EDIT_NOTEPAD, &CTablePeople::OnEditNotepad)
ON_COMMAND(ID_ROKONOK, &CTablePeople::OnRokonok)
ON_COMMAND(ID_BIRTH_DEATH, &CTablePeople::OnBirthDeath)
ON_COMMAND(ID_DESCENDANTS, &CTablePeople::OnDescendants)
ON_COMMAND(ID_GEDCOM_DELETE, &CTablePeople::OnGedcomDelete)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(theApp.m_caption);

	captionCtrl.GetSelectionCharFormat(cf);
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = CFE_BOLD;
	cf.crTextColor = LIGHTRED;
	cf.yHeight = 300;

	captionCtrl.SetSel(0, -1);
	captionCtrl.Clear();
	captionCtrl.SetWindowTextW( L"Emberek" );
	captionCtrl.SetSel(0, -1);
	captionCtrl.SetSelectionCharFormat(cf);
	captionCtrl.SetBackgroundColor(false, RGB(240, 240, 240));

	colorKeres.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );

	ChangeMenu();

//	if( !theApp.m_cntPeople )
	enableMenu( MF_GRAYED );

	createListColumns();
	
	if( !theApp.m_cntPeople )
		return FALSE;


	if (!m_ListCtrl.GetItemCount())
	{
		GetDlgItem(IDC_STATIC_KERESS)->EnableWindow(false);
		GetDlgItem(IDC_SEARCH)->EnableWindow(false);
		GetDlgItem(IDC_NEXT)->EnableWindow(false);
	}
	
/*
	menu.EnableMenuItem( 1,MF_BYPOSITION| MF_GRAYED );
	menu.EnableMenuItem( 2,MF_BYPOSITION| MF_GRAYED );
	menu.EnableMenuItem( 3,MF_BYPOSITION| MF_GRAYED );
	menu.EnableMenuItem( 4,MF_BYPOSITION| MF_GRAYED );
*/


	switch (m_list)
	{
	case ALL:
		OnFilterUnfilter();
		break;
	case ROWIDPEOPLE:
		OnFilterIndividual();
		break;
	case FULLNAME:
		OnGivenname();
		break;
	case LASTNAME:
		OnFilterLastname();
		break;
	case FIRSTNAME:
		OnFilterFirstname();
		break;


	};


	//if( theApp.m_cntPeople && theApp.m_cntPeople  < 10000 )
	//	OnFilterUnfilter();	

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);   // ez volt!!

	ShowWindow( SW_MAXIMIZE );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTablePeople::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CTablePeople::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::createListColumns()
{
	m_ListCtrl.DeleteAllItems();
	for (int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i--)
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

	if (theApp.m_inputMode == GEDCOM)
	{

		m_ListCtrl.InsertColumn(G_ROWID, L"rowid", LVCFMT_RIGHT, 40, -1, COL_NUM);
		m_ListCtrl.InsertColumn(G_ROWID_FILE, L"fájl#", LVCFMT_RIGHT, 40, -1, COL_HIDDEN);
		m_ListCtrl.InsertColumn(G_FAMILYNUMBER, L"family#", LVCFMT_RIGHT, 40, -1, COL_HIDDEN);
		m_ListCtrl.InsertColumn(G_TABLENUMBER, L"tábla#", LVCFMT_RIGHT, 40, -1, COL_HIDDEN);
		m_ListCtrl.InsertColumn(G_LINENUMBER, L"line#", LVCFMT_RIGHT, 40, -1, COL_NUM);
		m_ListCtrl.InsertColumn(G_SEX, L"nem", LVCFMT_LEFT, 40, -1, COL_HIDDEN);
		m_ListCtrl.InsertColumn(G_TITLE, L"tit", LVCFMT_LEFT, 30, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_TITOLO, L"elõnév", LVCFMT_LEFT, 100, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_LAST_NAME, L"családnév", LVCFMT_LEFT, 110, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_FIRST_NAME, L"utónév", LVCFMT_LEFT, 110, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_POSTERIOR, L"utótag", LVCFMT_LEFT, 40, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_BIRTH_PLACE, L"születés", LVCFMT_LEFT, 120, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_BIRTH_DATE, L"ideje", LVCFMT_LEFT, 70, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_DEATH_PLACE, L"elhalálozás", LVCFMT_LEFT, 120, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_DEATH_DATE, L"ideje", LVCFMT_LEFT, 70, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_COMMENT, L"leírás", LVCFMT_LEFT, 600, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_FATHER_ID, L"father_id", LVCFMT_LEFT, 50, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(G_MOTHER_ID, L"mother_id", LVCFMT_LEFT, 50, -1, COL_TEXT);
	}
	else if (theApp.m_inputMode == GAHTML)
	{

		m_ListCtrl.InsertColumn(H_ROWID, L"rowid", LVCFMT_RIGHT, 40, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_ROWID_FILE, L"fájl#", LVCFMT_RIGHT, 40, -1, COL_HIDDEN);
		m_ListCtrl.InsertColumn(H_FAMILYNUMBER, L"család#", LVCFMT_RIGHT, 40, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_TABLENUMBER, L"tábla#", LVCFMT_RIGHT, 40, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_LINENUMBER, L"line#", LVCFMT_RIGHT, 40, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_TABLENUMBERROMAN, L"tábla", LVCFMT_RIGHT, 40, -1, COL_TEXT);

		m_ListCtrl.InsertColumn(H_UNITED, L"U", LVCFMT_RIGHT, 19, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_SOURCE, L"R", LVCFMT_RIGHT, 19, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_GENERATION, L"G", LVCFMT_LEFT, 19, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_SEX, L"X", LVCFMT_LEFT, 19, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_PEER, L"peer", LVCFMT_LEFT, 30, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_TITLE, L"tit", LVCFMT_LEFT, 30, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_TITOLO, L"elõnév", LVCFMT_LEFT, 100, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_LAST_NAME, L"családnév", LVCFMT_LEFT, 110, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_FIRST_NAME, L"utónév", LVCFMT_LEFT, 110, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_POSTERIOR, L"utótag", LVCFMT_LEFT, 60, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_BIRTH_PLACE, L"születés", LVCFMT_LEFT, 100, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_BIRTH_DATE, L"ideje", LVCFMT_LEFT, 70, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_DEATH_PLACE, L"elhalálozás", LVCFMT_LEFT, 100, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_DEATH_DATE, L"ideje", LVCFMT_LEFT, 70, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_COMMENT, L"leírás", LVCFMT_LEFT, 600, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_AG, L"ág", LVCFMT_LEFT, 200, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_CSALAD, L"család õse", LVCFMT_LEFT, 100, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_FOLYT, L"folyt", LVCFMT_LEFT, 20, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(H_FATHER_ID, L"father_id", LVCFMT_RIGHT, 50, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_MOTHER_ID, L"mother_id", LVCFMT_RIGHT, 50, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_WHICHHUSBAND, L"wh", LVCFMT_RIGHT, 50, -1, COL_NUM);
		m_ListCtrl.InsertColumn(H_WHICHWIFE, L"ww", LVCFMT_RIGHT, 50, -1, COL_NUM);
	}
	else if (theApp.m_inputMode == MANUAL)
	{
		m_ListCtrl.InsertColumn(N_ROWID, L"rowid", LVCFMT_RIGHT, 40, -1, COL_NUM);
		m_ListCtrl.InsertColumn(N_FAMILYNUMBER, L"család#", LVCFMT_RIGHT, 40, -1, COL_NUM);
		m_ListCtrl.InsertColumn(N_TABLENUMBER, L"tábla#", LVCFMT_RIGHT, 40, -1, COL_NUM);
		m_ListCtrl.InsertColumn(N_TABLE_ANCESTRY, L"õs", LVCFMT_RIGHT, 20, -1, COL_HIDDEN);
		m_ListCtrl.InsertColumn(N_SEX, L"nem", LVCFMT_RIGHT, 20, -1, COL_HIDDEN);
		m_ListCtrl.InsertColumn(N_LAST_NAME, L"családnév", LVCFMT_LEFT, 110, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(N_FIRST_NAME, L"utónév", LVCFMT_LEFT, 110, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(N_POSTERIOR, L"utótag", LVCFMT_LEFT, 40, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(N_BIRTH_PLACE, L"születés", LVCFMT_LEFT, 120, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(N_BIRTH_DATE, L"ideje", LVCFMT_LEFT, 70, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(N_DEATH_PLACE, L"elhalálozás", LVCFMT_LEFT, 120, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(N_DEATH_DATE, L"ideje", LVCFMT_LEFT, 70, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(N_COMMENT, L"leírás", LVCFMT_LEFT, 600, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(N_FATHER_ID, L"father_id", LVCFMT_LEFT, 50, -1, COL_TEXT);
		m_ListCtrl.InsertColumn(N_MOTHER_ID, L"mother_id", LVCFMT_LEFT, 50, -1, COL_TEXT);
	}
	m_columnsCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// F I L T E R /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterUnfilter()
{
	m_filterText	= L"Az adatbázisban lévõ emberek"; 
	m_orderix		= 0;
	m_filter		= L"people";
	menu.EnableMenuItem( ID_DESCENDANTS_ALL, MF_BYCOMMAND | MF_GRAYED);  //GEDCOM és MANUAL esetén kell
	fillTable( 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterFamily()
{
	CTableTables dlg;

	dlg.m_filter	= L"WHERE percent='%%%'";
	dlg.m_select_id = L"familyNumber";
	dlg.m_caption = L"Válaszd ki a családot!";
	dlg.m_select  = 1;
	if(dlg.DoModal() == IDCANCEL ) return;


	m_filterText.Format(L"%s család táblázataiban lévõ emberek", dlg.m_last_name );
	m_filter.Format(  L"people WHERE familyNumber = %s", dlg.m_familyNumber);
	m_orderix = H_LAST_NAME;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterTable()
{
	CTableTables dlg;

	dlg.m_filter.Empty();
	dlg.m_select_id = L"tableNumber";
	dlg.m_caption = L"Válaszd ki a táblát!";
	dlg.m_select  = 1;


	if(dlg.DoModal() == IDCANCEL ) return;

	m_filterText.Format(L"A %s táblázatban lévõ emberek", dlg.m_tableHeader );
	m_filter.Format(L" people WHERE tableNumber='%s'", dlg.m_id);

	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterAncestors()
{
	m_filterText = L"Õsök - férfiak, akiknek nincsenek szülei, de van gyereke";
	m_filter = L"people WHERE sex_id = 1 AND (typeof(father_id) = 'null' OR father_id == 0 OR father_id = '' )\
 AND ( rowid IN (SELECT father_id FROM people ) OR rowid IN (SELECT mother_id FROM people) )";
	menu.EnableMenuItem( ID_DESCENDANTS_ALL, MF_BYCOMMAND | MF_ENABLED);
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterTableAncestors()
{
	m_filterText.Format(L"Táblák elsõ emberei - õsõk");
	m_filter = L"people WHERE tableAncestry==1";
	m_orderix = H_LAST_NAME;
	menu.EnableMenuItem(ID_DESCENDANTS_ALL, MF_BYCOMMAND | MF_ENABLED);
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterFamilyAncestors()
{
	m_filterText.Format(L"Család-táblák elsõ emberei");
	m_filter = L"(people,tables) WHERE people.tableNumber=tables.rowid AND people.tableAncestry=1 AND tables.percent == '%%%'";
//	m_filter = L"(people,tables) WHERE people.rowid==tables.ancesestor AND people.tableAncestry=1 AND table.percent=='%%%'";
	m_orderix = H_LAST_NAME;
	menu.EnableMenuItem(ID_DESCENDANTS_ALL, MF_BYCOMMAND | MF_ENABLED);
	fillTable(0);
	m_ListCtrl.SetSortOrder(1, m_orderix+1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterIndividual()
{
	CGetString dlg;

	int rowid;
	rowid = theApp.GetProfileInt(	L"dragon", L"rowid", 0 );
	dlg.m_string.Format( L"%d", rowid ); 
	dlg.m_caption.Format( L"Add meg a kívánt ember rowid-ját!" );
	if( dlg.DoModal() == IDCANCEL ) return;
	rowid = _wtoi( dlg.m_string );
	theApp.WriteProfileInt( L"dragon", L"rowid", rowid  );

	
	m_filterText.Format(L"rowid = %s azonosítójú bejegyzés", dlg.m_string);
	m_filter.Format(L"people WHERE rowid=%s", dlg.m_string);
	fillTable(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterLastname()
{
	CSelectOne dlg;

	dlg.m_caption	= L"Válasszd ki a családnevet!"; 
	dlg.m_table		= L"people"; 
	dlg.m_column	= L"last_name"; 
	dlg.m_filter	= L"WHERE last_name LIKE '%' GROUP BY last_name";

	if(dlg.DoModal() == IDCANCEL ) return;

	m_filterText.Format(L"%s családnevû bejegyzések", dlg.m_selected);
	m_filter.Format(L"people WHERE last_name='%s'",dlg.m_selected );
	
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterFirstname()
{
	CSelectOne dlg;

	dlg.m_caption	= L"Válasszd ki az utónevet!"; 
	dlg.m_table		= L"people"; 
	dlg.m_column	= L"first_name"; 
	dlg.m_filter	= L"WHERE first_name LIKE '%' GROUP BY first_name";

	if(dlg.DoModal() == IDCANCEL ) return;

	m_filter.Format(L"people WHERE first_name='%s'",dlg.m_selected );
	m_filterText.Format(L"%s keresztnevû bejegyzések", dlg.m_selected);
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGivenname()
{
	CGetLastFirst dlg;
	if( dlg.DoModal() == IDCANCEL ) return;

	if( dlg._firstname.IsEmpty() )
	{
		m_filterText.Format( L"'%s'-el kezdõdõ családnevû emberek", dlg._fullname );
		m_filter.Format( L"people WHERE last_name LIKE'%s%c'", dlg._lastname, '%' );
	}
	else
	{
		m_filterText.Format( L"%s nevû bejegyzések", dlg._fullname );
		m_filter.Format( L"people WHERE first_name='%s' AND last_name='%s'", dlg._firstname, dlg._lastname );
	}
	fillTable( 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnBirthDeath()
{
	m_filterText = L"Születési és halálozási dátum is meg van adva";
	m_filter = L"people WHERE length(birth_date) != 0 AND length(death_date) != 0";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterNodescendant()
{
	m_filterText = L"Emberek, akiknek nincsenek leszármazottai";
	m_filter = L"people WHERE rowid NOT IN (SELECT father_id FROM people WHERE father_id != '0') AND rowid NOT IN (SELECT mother_id FROM people WHERE mother_id!='0') AND rowid != '0'";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterNoparents()
{
	m_filterText = L"Emberek, akiknek nincsenek szülei";
	m_filter = L"people WHERE (father_id=0 OR typeof( father_id ) = 'null' ) AND ( mother_id=0 OR typeof( mother_id ) = 'null' ) ";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterNospouse()
{
	m_filterText = L"Emberek , akiknek nincs házastársa";
	m_filter = L"people WHERE rowid NOT IN (SELECT husband_id FROM marriages) AND rowid NOT IN (SELECT wife_id FROM marriages )";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterMen()
{
	m_filterText = L"Az adatbázisban lévõ férfiak";
	m_filter.Format(  L"people WHERE sex_id=1" );
	fillTable(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterWomen()
{
	m_filterText = L"Az adatbázisban lévõ nõk";
	m_filter.Format(  L"people WHERE sex_id=2" );
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterBisex()
{
	m_filterText = L"Az adatbázisban lévõ bisex emberek";
	m_filter.Format(  L"people WHERE sex_id=0" );
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterFolyt()
{
	m_filterText = L"Elágazások";
	m_filter.Format(  L"people WHERE folyt != ''" );
	fillTable(0);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterDescendants()
{
	m_filterText	= L"Leszármazottak";
	m_filter		= L"people WHERE source=1"; 
	fillTable(0);
}
void CTablePeople::OnFilterSpouses()
{
	m_filterText	= L"Leszármazottak házastársai";
	m_filter		= L"people WHERE source=2"; 
	fillTable(0);
}
void CTablePeople::OnFilterFathers()
{
	m_filterText	= L"Leszármazottak házastársainak apjai";
	m_filter		= L"people WHERE source=3 AND sex_id=1"; 
	fillTable(0);
}

void CTablePeople::OnFilterMothers()
{
	m_filterText	= L"Leszármazottak házastársainak anyjai";
	m_filter		= L"people WHERE source=3 AND sex_id==2"; 
	fillTable(0);
}

void CTablePeople::OnFilterSpousespouses()
{
	m_filterText	= L"Leszármazottak házastársainak további házastársai";
	m_filter		= L"people WHERE source=4"; 
	fillTable(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// G R O U P   B Y  ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyTitolo()
{
	m_filter.Format(L"people GROUP BY titolo");
	m_filterText.Format(L"Különbözõ elõnevek" );
	m_orderix = H_TITOLO;
	fillTable(0);
}
void CTablePeople::OnGroupbyPosterior()
{
	m_filter.Format(L"people GROUP BY posterior");
	m_filterText.Format(L"Különbözõ utótagok");
	m_orderix = H_POSTERIOR;
	fillTable(0);
}
void CTablePeople::OnGroupbyBirthplace()
{
	m_filter.Format(L"people GROUP BY birth_place");
	m_filterText.Format(L"Különbözõ születési helyek" );
	m_orderix = H_BIRTH_PLACE;
	fillTable(0);

}
void CTablePeople::OnGroupbyDeathplace()
{
	m_filter.Format(L"people GROUP BY death_place");
	m_filterText.Format(L"Különbözõ halálozási helyek" );
	m_orderix = H_DEATH_PLACE;
	fillTable(0);
}
void CTablePeople::OnGroupbyFirstname()
{
	m_filter.Format(L"people GROUP BY first_name");
	m_filterText.Format(L"Különbözõ keresztnevek" );
	m_orderix = H_FIRST_NAME;
	fillTable(0);
}
void CTablePeople::OnGroupbyLastname()
{
	m_filter.Format(L"people GROUP BY last_name");
	m_filterText.Format(L"Különbözõ családnevek" );
	m_orderix = H_LAST_NAME;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyBirthDate()
{
	m_filter.Format(L"people GROUP BY birth_date");
	m_filterText.Format(L"Különbözõ születési idõk" );
	m_orderix = H_BIRTH_DATE;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyDeathdate()
{
	m_filter.Format(L"people GROUP BY death_date");
	m_filterText.Format(L"Különbözõ halálozási idõk" );
	m_orderix = H_DEATH_DATE;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyComment()
{
	m_filter.Format(L"people GROUP BY comment");
	m_filterText.Format(L"Különbözõ megjegyzések" );
	m_orderix = H_COMMENT;
	fillTable(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::fillTable( UINT nItem )
{
	CFilter dlg;
	int		ret;
	CString	relation;
	CString command;
	CString	str;

	CProgressWnd wndProgress(NULL, m_filterText ); 
	wndProgress.GoModal();
	m_command.Format(L"SELECT people.rowid,* FROM %s", m_filter);

	if( !query( m_command ) ) return;

	m_ListCtrl.DeleteAllItems();

	wndProgress.SetRange(0, m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	v_individuals.clear();
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i )
	{
		addEntry();
		m_recordset->MoveNext();

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.AttachDataset( &v_individuals );
	m_ListCtrl.ReSort();
	m_ListCtrl.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	int cntPerPage = m_ListCtrl.GetCountPerPage();
//	if( nItem >= 0 )
//	{
//		theApp.showItem( 0, &m_ListCtrl );
//		nItem += cntPerPage/2;
//		if( nItem > m_recordset->RecordsCount() )
//			nItem = m_recordset->RecordsCount() - 1;
//		m_ListCtrl.EnsureVisible( nItem, FALSE );
//	}
	enableMenu( MF_ENABLED );
	str.Format( L"%s (%d)", m_filterText, m_ListCtrl.GetItemCount() );

	captionCtrl.SetSel(0, -1);
	captionCtrl.Clear();
	captionCtrl.SetWindowTextW( str);
	captionCtrl.SetSel(0, -1);
	captionCtrl.SetSelectionCharFormat(cf);




//	captionCtrl.SetWindowTextW( str );
	SetForegroundWindow();

	if( m_ListCtrl.GetItemCount() )
	{
		GetDlgItem(IDC_STATIC_KERESS)->EnableWindow(true);
		GetDlgItem(IDC_SEARCH)->EnableWindow(true);
		GetDlgItem(IDC_NEXT)->EnableWindow(true);

		menu.EnableMenuItem( 1,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 2,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 3,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 4,MF_BYPOSITION| MF_ENABLED );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnInputManual()
{
	CString message;
	CString captiopn;
	message = L"Már vannak emberek az adtbázisban.\nÚj ember csak akkor tanácsos megadni, ha nincs rokoni kapcsolatban \
az adatbázisban már létezõ személyekkel.\n\
Egyébként a rokonhoz kapcsolódóan lehet megadni.\n\n\
Mégis megad új személyt?";

	if( AfxMessageBox( message, MB_YESNO ) == IDNO ) return;

	CEditPeople dlg;
	dlg.m_rowid.Empty();

	if( dlg.DoModal() == IDCANCEL ) return;
	m_rowid = dlg.m_rowid;

	m_command.Format( L"SELECT rowid, * FROM people WHERE rowid ='%s'", m_rowid );
	if( !query( m_command ) ) return;
	
	addEntry();

	int nItem  = m_ListCtrl.GetItemCount();
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.AttachDataset( &v_individuals );
	m_ListCtrl.ReSort();

	for( nItem = 0; nItem < v_individuals.size(); ++nItem )
	{
		if( m_ListCtrl.GetItemText( nItem, 0 ) == m_rowid )
			break;
	}
	m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible( nItem, FALSE );

	CRelatives dlgR;
	dlgR.m_rowid = m_rowid;
	if( dlgR.DoModal() == IDCANCEL ) return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::addEntry()
{
	CString rowid;
	CString lastName;
	CString firstName;
//	int tLen;


	if( theApp.m_inputMode == GEDCOM )
	{
		rowid		= m_recordset->GetFieldString( DBP_ROWID );
		lastName	= m_recordset->GetFieldString( DBP_LAST_NAME );
		lastName.Replace( '|', '\'' );
		firstName	= m_recordset->GetFieldString( DBP_FIRST_NAME );

		push( rowid );
		push( m_recordset->GetFieldString( DBP_FILENUMBER ) );
		push( L"1" );													// familyNumber
		push( m_recordset->GetFieldString( DBP_TABLENUMBER ) );
		push( m_recordset->GetFieldString( DBP_LINENUMBER ) );

		push( m_recordset->GetFieldString( DBP_SEX_ID ) );
		push( m_recordset->GetFieldString( DBP_TITLE ));
		push( m_recordset->GetFieldString( DBP_TITOLO ));

		push( lastName );
		push( firstName );
		push(m_recordset->GetFieldString(DBP_POSTERIOR));
		push( m_recordset->GetFieldString( DBP_BIRTH_PLACE ) );
		push( m_recordset->GetFieldString( DBP_BIRTH_DATE ) );
		push( m_recordset->GetFieldString( DBP_DEATH_PLACE ) );
		push( m_recordset->GetFieldString( DBP_DEATH_DATE ) );
		push( m_recordset->GetFieldString( DBP_COMMENT ) );
		push(m_recordset->GetFieldString(DBP_FATHER_ID));
		push(m_recordset->GetFieldString(DBP_MOTHER_ID));
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		CString united = m_recordset->GetFieldString( DBP_UNITED );
		if( united == L"1" ) united.Empty();

		
		push( m_recordset->GetFieldString( DBP_ROWID ) );
		push( m_recordset->GetFieldString( DBP_FILENUMBER ) );
		push( m_recordset->GetFieldString( DBP_FAMILYNUMBER ) );
		push( m_recordset->GetFieldString( DBP_TABLENUMBER ) );
		push( m_recordset->GetFieldString( DBP_LINENUMBER ) );
		push( m_recordset->GetFieldString( DBP_TABLEROMAN ) );

		push( united );
		push( m_recordset->GetFieldString( DBP_SOURCE ) );

		push( m_recordset->GetFieldString( DBP_GENERATION ) );
		push( m_recordset->GetFieldString( DBP_SEX_ID ) );
		push(m_recordset->GetFieldString( DBP_PEER ) );
		push( m_recordset->GetFieldString( DBP_TITLE ));
		push( m_recordset->GetFieldString( DBP_TITOLO ));
		lastName = m_recordset->GetFieldString( DBP_LAST_NAME );
		lastName.Replace( '|', '\'' );
		push( lastName );
		push( m_recordset->GetFieldString( DBP_FIRST_NAME ) );
		push( m_recordset->GetFieldString( DBP_POSTERIOR ) );
		push( m_recordset->GetFieldString( DBP_BIRTH_PLACE ) );
		push( m_recordset->GetFieldString( DBP_BIRTH_DATE ) );
		push( m_recordset->GetFieldString( DBP_DEATH_PLACE ) );
		push( m_recordset->GetFieldString( DBP_DEATH_DATE ) );
		push( m_recordset->GetFieldString( DBP_COMMENT ) );
		push( m_recordset->GetFieldString( DBP_ARM ) );
		push( m_recordset->GetFieldString( DBP_CSALAD ) );
		push( m_recordset->GetFieldString( DBP_FOLYT ) );
		push(m_recordset->GetFieldString(DBP_FATHER_ID));
		push(m_recordset->GetFieldString(DBP_MOTHER_ID));
		push(m_recordset->GetFieldString(DBP_WHICHHUSBAND));
		push(m_recordset->GetFieldString(DBP_WHICHWIFE));
	}
	else if( theApp.m_inputMode == MANUAL )
	{
		push( m_recordset->GetFieldString( DBP_ROWID ) );
		push( m_recordset->GetFieldString( DBP_FAMILYNUMBER ) );
		push( m_recordset->GetFieldString( DBP_TABLENUMBER ) );
		str = m_recordset->GetFieldString( DBP_TABLEANCESTRY );
		if( str == L"1" )
			push( m_recordset->GetFieldString( DBP_TABLEANCESTRY ) );
		else
			push( L"" );
		push( m_recordset->GetFieldString( DBP_SEX_ID ) );
		lastName = m_recordset->GetFieldString( DBP_LAST_NAME );
		lastName.Replace( '|', '\'' );
		push( lastName );
		push( m_recordset->GetFieldString( DBP_FIRST_NAME ) );
		push(m_recordset->GetFieldString(DBP_POSTERIOR));
		push( m_recordset->GetFieldString( DBP_BIRTH_PLACE ) );
		push( m_recordset->GetFieldString( DBP_BIRTH_DATE ) );
		push( m_recordset->GetFieldString( DBP_DEATH_PLACE ) );
		push( m_recordset->GetFieldString( DBP_DEATH_DATE ) );
		push( m_recordset->GetFieldString( DBP_COMMENT ) );
		push(m_recordset->GetFieldString(DBP_FATHER_ID));
		push(m_recordset->GetFieldString(DBP_MOTHER_ID));
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::push( CString str )
{
	const TCHAR* pC = (LPCTSTR) str;
	int tLen=str.GetLength()+1;
	LPTSTR sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,str.GetBuffer());
	v_individuals.push_back( sT);
	str.ReleaseBuffer();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a v_individuals ész m_ListCtrl renezettsége eltérhet!!! 
// Ezért az elõbbiben az m_rowid, az utóbbibal az m_nItem a szerkesztendõ/szerkestett bejegyzés azonosítója!
// 
void CTablePeople::OnEditUpdate()
{
	m_nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED); 

	CEditPeople dlg;
	CString name;

	if( theApp.m_inputMode == GEDCOM )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( m_nItem, G_LAST_NAME ),  m_ListCtrl.GetItemText( m_nItem, G_FIRST_NAME ) );
		m_rowid	= m_ListCtrl.GetItemText( m_nItem, G_ROWID );

	}
	else if( theApp.m_inputMode == GAHTML )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( m_nItem, H_LAST_NAME ),  m_ListCtrl.GetItemText( m_nItem, H_FIRST_NAME ) );
		m_rowid	= m_ListCtrl.GetItemText( m_nItem, H_ROWID );
	}
	else if( theApp.m_inputMode == MANUAL )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( m_nItem, N_LAST_NAME ),  m_ListCtrl.GetItemText( m_nItem, N_FIRST_NAME ) );
		m_rowid	= m_ListCtrl.GetItemText( m_nItem, N_ROWID );
	}
	dlg.m_rowid = m_rowid;
	if( dlg.DoModal() != IDCANCEL )
	updateEntry();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a v_individuals rowid azonosítójú bejegyzését updateli és megjeleníti az egész listát
//
void CTablePeople::updateEntry()
{
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid='%s'", m_rowid );
	if( !query( m_command ) ) return;
	if( !m_recordset->RecordsCount() ) return;	

	// a v_individuals-ban megkeresi a módosítandó bejegyzést az m_rowid alapján

	int nItem;
	for( nItem = 0; nItem < v_individuals.size(); ++nItem )
	{
		if( v_individuals.at( nItem * m_columnsCount + N_ROWID ) == m_rowid )
			break;
	}

	m_ListCtrl.DetachDataset();
	if( theApp.m_inputMode == GEDCOM )
	{
		updateField( nItem, G_SEX, m_recordset->GetFieldString( DBP_SEX_ID ) );
		updateField( nItem, G_TITLE, m_recordset->GetFieldString( DBP_TITLE ) );
		updateField( nItem, G_TITOLO, m_recordset->GetFieldString( DBP_TITOLO ) );
		updateField( nItem, G_LAST_NAME, m_recordset->GetFieldString( DBP_LAST_NAME ) );
		updateField( nItem, G_FIRST_NAME, m_recordset->GetFieldString( DBP_FIRST_NAME ) );
		updateField( nItem, G_POSTERIOR, m_recordset->GetFieldString(DBP_POSTERIOR));
		updateField( nItem, G_BIRTH_PLACE, m_recordset->GetFieldString( DBP_BIRTH_PLACE ) );
		updateField( nItem, G_BIRTH_DATE, m_recordset->GetFieldString( DBP_BIRTH_DATE ) );
		updateField( nItem, G_DEATH_PLACE, m_recordset->GetFieldString( DBP_DEATH_PLACE ) );
		updateField( nItem, G_DEATH_DATE, m_recordset->GetFieldString( DBP_DEATH_DATE ) );
		updateField( nItem, G_COMMENT, m_recordset->GetFieldString( DBP_COMMENT ) );
		updateField(nItem, G_FATHER_ID, m_recordset->GetFieldString(DBP_FATHER_ID));
		updateField(nItem, G_MOTHER_ID, m_recordset->GetFieldString(DBP_MOTHER_ID));
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		updateField( nItem, H_SEX, m_recordset->GetFieldString( DBP_SEX_ID ) );
		updateField(nItem, H_PEER, m_recordset->GetFieldString(DBP_PEER));
		updateField( nItem, H_TITLE, m_recordset->GetFieldString( DBP_TITLE ) );
		updateField( nItem, H_TITOLO, m_recordset->GetFieldString( DBP_TITOLO ) );
		updateField( nItem, H_LAST_NAME, m_recordset->GetFieldString( DBP_LAST_NAME ) );
		updateField( nItem, H_FIRST_NAME, m_recordset->GetFieldString( DBP_FIRST_NAME ) );
		updateField( nItem, H_POSTERIOR, m_recordset->GetFieldString(DBP_POSTERIOR));
		updateField( nItem, H_BIRTH_PLACE, m_recordset->GetFieldString( DBP_BIRTH_PLACE ) );
		updateField( nItem, H_BIRTH_DATE, m_recordset->GetFieldString( DBP_BIRTH_DATE ) );
		updateField( nItem, H_DEATH_PLACE, m_recordset->GetFieldString( DBP_DEATH_PLACE ) );
		updateField( nItem, H_DEATH_DATE, m_recordset->GetFieldString( DBP_DEATH_DATE ) );
		updateField( nItem, H_COMMENT, m_recordset->GetFieldString( DBP_COMMENT ) );
		updateField(nItem, H_FATHER_ID, m_recordset->GetFieldString(DBP_FATHER_ID));
		updateField(nItem, H_MOTHER_ID, m_recordset->GetFieldString(DBP_MOTHER_ID));
	}
	else if( theApp.m_inputMode == MANUAL )
	{
		updateField( nItem, N_TABLE_ANCESTRY, m_recordset->GetFieldString( DBP_TABLEANCESTRY ) );
		updateField( nItem, N_LAST_NAME, m_recordset->GetFieldString( DBP_LAST_NAME ) );
		updateField( nItem, N_FIRST_NAME, m_recordset->GetFieldString( DBP_FIRST_NAME ) );
		updateField( nItem, N_POSTERIOR, m_recordset->GetFieldString(DBP_POSTERIOR));
		updateField( nItem, N_BIRTH_PLACE, m_recordset->GetFieldString( DBP_BIRTH_PLACE ) );
		updateField( nItem, N_BIRTH_DATE, m_recordset->GetFieldString( DBP_BIRTH_DATE ) );
		updateField( nItem, N_DEATH_PLACE, m_recordset->GetFieldString( DBP_DEATH_PLACE ) );
		updateField( nItem, N_DEATH_DATE, m_recordset->GetFieldString( DBP_DEATH_DATE ) );
		updateField( nItem, N_COMMENT, m_recordset->GetFieldString( DBP_COMMENT ) );
		updateField(nItem, N_FATHER_ID, m_recordset->GetFieldString(DBP_FATHER_ID));
		updateField(nItem, N_MOTHER_ID, m_recordset->GetFieldString(DBP_MOTHER_ID));
	}
	m_ListCtrl.DetachDataset();
	m_ListCtrl.AttachDataset( &v_individuals );
	m_ListCtrl.ReSort();
	m_ListCtrl.EnsureVisible( m_nItem, TRUE );
	m_ListCtrl.SetItemState( m_nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::updateField( int nItem, int i, CString str )
{
	int tLen=str.GetLength()+1;
	LPTSTR sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,str.GetBuffer());

	v_individuals.at( (nItem)*m_columnsCount + i ) = sT;
	str.ReleaseBuffer();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::query1( CString command )
{
	if( m_recordset1->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::query3( CString command )
{
	if( m_recordset3->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnEditInsert();
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::ChangeMenu()
{
    if( theApp.m_inputMode == GEDCOM )
    {
        SetMenu(NULL);
        ::DestroyMenu(GetMenu()->GetSafeHmenu());
//        CMenu menu;
        menu.LoadMenuW(IDR_TABLE_PEOPLE_GEDCOM);
        SetMenu(&menu);
		if( wcscmp( theApp.m_username, L"végesistvan" ) )
			menu.EnableMenuItem( 5, MF_BYPOSITION|MF_GRAYED);


    }
    else if( theApp.m_inputMode == GAHTML )
    {
        SetMenu(NULL);
        ::DestroyMenu(GetMenu()->GetSafeHmenu());
 //       CMenu menu;
        menu.LoadMenuW(IDR_TABLE_PEOPLE_HTML );
        SetMenu(&menu);
		if( wcscmp( theApp.m_username, L"végesistvan" ) )
			menu.EnableMenuItem( 6, MF_BYPOSITION|MF_GRAYED);
    }
	else if( theApp.m_inputMode == URES )
    {
        SetMenu(NULL);
        ::DestroyMenu(GetMenu()->GetSafeHmenu());
//        CMenu menu;
        menu.LoadMenuW(IDR_MAIN_EMPTY );
        SetMenu(&menu);
    }
	else if( theApp.m_inputMode == MANUAL )
    {
        SetMenu(NULL);
        ::DestroyMenu(GetMenu()->GetSafeHmenu());
 //       CMenu menu;
        menu.LoadMenuW(IDR_TABLE_PEOPLE_MANUAL );
		SetMenu(&menu);
		if( wcscmp( theApp.m_username, L"végesistvan" ) )
			menu.EnableMenuItem( 11, MF_BYPOSITION|MF_GRAYED);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGaLine()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	H_LINENUMBER ) );

//	ShowWindow( SW_HIDE );
	theApp.listHtmlLine( lineNumber );
//	ShowWindow( SW_RESTORE );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnExportSelected()
{
	CString	logFile(L"projects"); 
	CString	title;
	title.Format( L"%s (selected from %d projects)", m_filterText, m_ListCtrl.GetItemCount() );
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}


void CTablePeople::OnExportAll()
{
	CString	logFile(L"projects"); 
	CString	title;
	title.Format( L"%s", m_filterText );
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::enableMenu( BOOL flag)
{
	if( theApp.m_inputMode == GAHTML )
	{
		menu.EnableMenuItem(1,MF_BYPOSITION| flag );
		menu.EnableMenuItem(2,MF_BYPOSITION| flag );
		menu.EnableMenuItem(3,MF_BYPOSITION| flag );
		menu.EnableMenuItem(4,MF_BYPOSITION| flag );
		menu.EnableMenuItem(5,MF_BYPOSITION| flag);
		menu.EnableMenuItem(6,MF_BYPOSITION| flag);
		menu.EnableMenuItem(7,MF_BYPOSITION| flag);
	}
	DrawMenuBar();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnAzonosemberIdent()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		theApp.message( L"Emberek listája", L"\nNincs kijelölve senki!" );
		return;
	}

	CSameParams dlg;
	
	dlg.m_firstName	= m_ListCtrl.GetItemText( nItem, H_FIRST_NAME );
	dlg.m_lastName	= m_ListCtrl.GetItemText( nItem, H_LAST_NAME );

	dlg.DoModal();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDD_TABLE_INDIVIDUAL sysem menüjének (X) kattintására Ez mûködik!!!
void CTablePeople::OnClose()  
{
	m_ListCtrl.DeleteAllItems();	// erre szükség van?
	m_recordset->Clear();			// erre szükség van?
	

//	((CDragonDlg*)m_dlg )->m_pIndividuals = NULL;
//	theApp.m_pMainWnd->SetForegroundWindow();
	CDialogEx::OnClose();
}//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CTablePeople::OnGedcomOutput()
{
	if( theApp.m_inputMode == MANUAL )
		m_ListCtrl.SetSortOrder( 2, N_LAST_NAME+1, N_FIRST_NAME+1 );
	if( theApp.m_inputMode == GEDCOM )
		m_ListCtrl.SetSortOrder( 2, G_LAST_NAME+1, G_FIRST_NAME+1 );
	if( theApp.m_inputMode == GAHTML )
		m_ListCtrl.SetSortOrder( 2, H_LAST_NAME+1, H_FIRST_NAME+1 );

	CGedcomOut dlg;
	dlg.m_filter = m_filterText;
	dlg.p_ListCtrl = &m_ListCtrl;
	dlg.DoModal();
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnClickedKeress()
{
	keress(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress( nItem + 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////44
LRESULT CTablePeople::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR;

	if (theApp.m_inputMode == GEDCOM)
		MENU_IDR = IDR_DROPDOWN_GEDCOM;
	else if( theApp.m_inputMode == GAHTML )
		MENU_IDR = IDR_DROPDOWN_PEOPLE_GA;
	else if( theApp.m_inputMode == MANUAL )
		MENU_IDR = IDR_DROPDOWN_PEOPLE;

	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);
		if( MENU_IDR == IDR_DROPDOWN_HTML )
		{
			/*
			if( !theApp.m_dbFileExists )
			{
				str.Format( L"Az adatbázis a\n\n%s\n\nfájlból készült, ami nem található!", theApp.m_htmlPathName );
				AfxMessageBox( str, MB_ICONEXCLAMATION );
				return false;
			}
			*/
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText( nItem, H_LAST_NAME ), m_ListCtrl.GetItemText( nItem, H_LINENUMBER )  );
	else
		title.Format( L"%d kijelölt sor a ga.html fájlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, H_LINENUMBER, title );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText(nItem, H_LINENUMBER);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	H_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, H_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CEditHtmlLines dlg;
	dlg.m_title.Format( L"%s %s szülei és testvérei", m_ListCtrl.GetItemText( nItem, H_LAST_NAME ), m_ListCtrl.GetItemText( nItem, H_FIRST_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, 	H_ROWID );
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlChildren()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CEditHtmlLines dlg;
	dlg.m_title.Format( L"%s %s és gyermekei", m_ListCtrl.GetItemText( nItem, H_LAST_NAME ), m_ListCtrl.GetItemText( nItem, H_FIRST_NAME ) );
	dlg.m_type	= L"F_CHILDREN";
	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, 	H_ROWID );

	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGedcomDelete()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText(nItem, G_ROWID);
	CString name;
	name.Format(L"%s %s", m_ListCtrl.GetItemText(nItem, G_LAST_NAME), m_ListCtrl.GetItemText(nItem, G_FIRST_NAME));
	
	str.Format(L"Valóban törölni akarod %s(%s) bejegyzést\nés minden rá való hivatkozást?", name, rowid);
	if (AfxMessageBox(str, MB_YESNO) == IDYES)
	{
		m_ListCtrl.DeleteItem(nItem);
		m_command.Format(L"DELETE FROM people WHERE rowid='%s'", rowid);
		if (!theApp.execute(m_command)) return;
		--theApp.m_cntPeople;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditDelete()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString sex_id;
	CString rowid = m_ListCtrl.GetItemText( nItem, G_ROWID );
	CString name;

	if( theApp.m_inputMode == GEDCOM )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, G_LAST_NAME ), m_ListCtrl.GetItemText( nItem, G_FIRST_NAME ) ); 
		sex_id = m_ListCtrl.GetItemText( nItem, G_SEX );
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, H_LAST_NAME ), m_ListCtrl.GetItemText( nItem, H_FIRST_NAME ) ); 
		sex_id = m_ListCtrl.GetItemText( nItem, H_SEX );
	}
	else
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, N_LAST_NAME ), m_ListCtrl.GetItemText( nItem, N_FIRST_NAME ) ); 
		sex_id = m_ListCtrl.GetItemText( nItem, N_SEX );
	}

	str.Format( L"Valóban törölni akarod %s(%s) bejegyzést\nés minden rá való hivatkozást?", name, rowid );
	if( AfxMessageBox( str, MB_YESNO ) == IDYES )
	{
		m_ListCtrl.DeleteItem( nItem );
		m_command.Format(L"DELETE FROM people WHERE rowid='%s'",rowid );
		if( !theApp.execute( m_command ) ) return;
		--theApp.m_cntPeople;

//		m_command.Format(L"DELETE FROM people WHERE father_id='%s' OR mother_id='%s'",rowid, rowid );
		if( sex_id == MAN )
			m_command.Format( L"UPDATE people SET father_id=0 WHERE father_id = '%s'", rowid );
		else
			m_command.Format( L"UPDATE people SET mother_id=0 WHERE mother_id = '%s'", rowid );
		if( !theApp.execute( m_command ) ) return;
	
		m_command.Format(L"DELETE FROM marriages WHERE husband_id='%s' OR wife_id='%s'",rowid, rowid );
		if( !theApp.execute( m_command ) ) return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditInsert()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	H_ROWID );

	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnInfo()
{
	CString info = L"\
Azt oszlopok jelentése:\
\r\n\r\n\
U - a bejegyzés ennyi bejegyzés egyesítése\r\n\
R - bejegyzés rangja\r\n\
G - generációs kód\r\n\
X - sex\
";
	AfxMessageBox( info, MB_ICONINFORMATION );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnSetAncestor()
{
	CString rowid;
	CString father_id;
	UINT	i;

	m_command.Format( L"SELECT rowid, father_id FROM people WHERE last_name = 'Gardiner'" );
/*
	m_command.Format( L"SELECT rowid, father_id FROM people" );
	if( !query( m_command ) ) return;

	CProgressWnd wndProgress(NULL, L"Õsök keresése" );
	wndProgress.GoModal();
	wndProgress.SetRange(0, m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);


	for( i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
 		rowid		= m_recordset->GetFieldString( 0 );
		father_id	= m_recordset->GetFieldString( 1 );
		if( father_id.IsEmpty() || father_id == L"0" ) 
		{	
			setAncestor( rowid );
		}
		else
		{
			rowid = ancestor( father_id );
			if( !rowid.IsEmpty() )
				setAncestor( rowid );
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
*/
	if( !theApp.execute( L"UPDATE people SET tableAncestry = 0" ) ) return; 
	m_command.Format( L"SELECT rowid FROM people WHERE \
	(father_id == 0 || father_id == '' || typeof( father_id ) == 'null') AND \
	(mother_id == 0 || mother_id == '' || typeof( mother_id ) == 'null') " );
	if( !query( m_command ) ) return;

	CProgressWnd wndProgress(NULL, L"Õsök keresése" );
	wndProgress.GoModal();
	wndProgress.SetRange(0, m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);


	for( i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		setAncestor( m_recordset->GetFieldString( 0 ) );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
// Az father_id-jû bejegyzás õsét keresi meg, azaz akkinek már nincsa apja
CString CTablePeople::ancestor( CString father_id )
{
	CString rowid;
	while( !father_id.IsEmpty() )
	{
		m_command.Format( L"SELECT father_id FROM people WHERE rowid ='%s'", father_id );
		if( !query2( m_command ) ) return 0;
		str = m_recordset2->GetFieldString( 0 );
		if( str.IsEmpty() || str == L"0")
		{
			rowid = father_id;
			break;
		}
		father_id	= str;
	}
	return rowid;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
// Az father_id-jû bejegyzás õsét keresi meg, azaz akkinek már nincsa apja
bool CTablePeople::setAncestor( CString rowid  )
{
	m_command.Format( L"UPDATE people SET tableAncestry = 1 WHERE rowid = '%s'", rowid );
	if( !theApp.execute( m_command ) ) return false;
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////44
LRESULT CTablePeople::OnSyncronize(WPARAM wParam, LPARAM lParam)
{
	m_rowid.Format( L"%d", wParam );
	bool uj = lParam;
	int nItem;
	m_orderix = 0;
	m_ListCtrl.SetSortOrder( 1, 1 );
	if( uj )
	{
		m_command.Format( L"SELECT rowid, * FROM people WHERE rowid ='%s'", m_rowid );
		if( !query( m_command ) ) return false ;
		addEntry();
		nItem = v_individuals.size() -1;
	}
	else
	{
		
		UINT i;
		for( i = 0; i*m_columnsCount - N_ROWID  < v_individuals.size(); ++i )
		{
			if( v_individuals.at( i*m_columnsCount + N_ROWID ) == m_rowid )
				break;
		}
		if( i < v_individuals.size() - m_columnsCount + N_ROWID )  // már létezõ bejegyzés módosítása
			updateEntry();

		nItem = i;
	}
	m_ListCtrl.DetachDataset();
	m_ListCtrl.AttachDataset( &v_individuals );
	theApp.showItem( nItem, &m_ListCtrl );
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGedcomIndi()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	H_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad(theApp.m_gedPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnApai()
{
	directAscendants(true);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnAnyai()
{
	directAscendants(false);
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CTablePeople::directAscendants( bool apai )
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(L"Nincs kijelölve senki!");
		return;
	}

	CString name;
	CString rowid;

	rowid = m_ListCtrl.GetItemText(nItem, G_ROWID);

	if (theApp.m_inputMode == GAHTML)
		name.Format(L"%s %s", m_ListCtrl.GetItemText(nItem, H_LAST_NAME), m_ListCtrl.GetItemText(nItem, H_FIRST_NAME));
	else if (theApp.m_inputMode == MANUAL)
		name.Format(L"%s %s", m_ListCtrl.GetItemText(nItem, N_LAST_NAME), m_ListCtrl.GetItemText(nItem, N_FIRST_NAME));
	else if (theApp.m_inputMode == GEDCOM)
		name.Format(L"%s %s", m_ListCtrl.GetItemText(nItem, G_LAST_NAME), m_ListCtrl.GetItemText(nItem, G_FIRST_NAME));

	CAscendantsDirect asc;
	asc.m_rowid = rowid;
	asc.m_name = name;

	if( apai )
		asc.directFatherAscendants();
	else
		asc.directMotherAscendants();
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////
//void CTablePeople::OnCheckParents()
//{
//	int i;
//	CString other;
//	CString munka;
//	int b;
//	int j;
//	std::vector<CString> v_nItem;
//	for (i = 0; i < m_ListCtrl.GetItemCount(); ++i)
//	{
//		other = m_ListCtrl.GetItemText(i, G_POSTERIOR);
//		if (other[0] == '(')	// a sor elején zárójel, akkor ez (szülõk,házastárs )  ???
//		{
//			other = other.Mid( 1 );
//			munka = other;			// végzárójelt eldobja
//			while (true)
//			{
//				if ((j = munka.ReverseFind(')')) != -1);
//				{
//					if ((b = munka.Find('(', j + 1)) == -1)	// nem talált már benne 
//					{
//						break;
//					}
//					munka.SetAt(j, '_'); // bal zárójel törlése
//					munka.SetAt(b, '_'); // jobb zárójel törlése
//				}
//			}
//			spouseSubstr = other.Left(j - 1);
//			relativesSubstr = other.Mid(j + 1);
//			v_nItem.push_back( i )
//
//		}
//	}
//
//	m_ListCtrl.DeleteAllItems();
//	wndProgress.SetRange(0, m_recordset->RecordsCount());
//	wndProgress.SetPos(0);
//	wndProgress.SetStep(1);
//
//	v_individuals.clear();
//	for (UINT i = 0; i < m_recordset->RecordsCount(); ++i)
//	{
//		addEntry();
//		m_recordset->MoveNext();
//
//		wndProgress.StepIt();
//		wndProgress.PeekAndPump();
//		if (wndProgress.Cancelled()) break;
//	}
//	m_ListCtrl.DeleteAllItems();
//	m_ListCtrl.AttachDataset(&v_individuals);
//	m_ListCtrl.ReSort();
//
//
//
//
//}
/////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnUnite()
{
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	UNITE_ENTRIES unite;
	vUnite.clear();
	int nItem;

	while (pos)
	{
		nItem = m_ListCtrl.GetNextSelectedItem(pos);
		unite.nItem = nItem;
		unite.rowid = m_ListCtrl.GetItemText(nItem, H_ROWID);
		unite.sex_id = m_ListCtrl.GetItemText(nItem, H_SEX);
		unite.source = _wtoi(m_ListCtrl.GetItemText(nItem, H_SOURCE));
		vUnite.push_back(unite);
	}
	if (vUnite.size() < 2)
	{
		AfxMessageBox(L"Legakább 2 azonos nevû bejegyzést ki kel jelölnöd!");
		return;
	}
	std::sort(vUnite.begin(), vUnite.end(), sortBySource);
	contractSameItems();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTablePeople::contractSameItems()
{
	// az elsõ bejegyzés megtartandó
	CString rowidKeep	= vUnite.at(0).rowid;
	int nItem0			= vUnite.at(0).nItem;
	CString sex_id		= vUnite.at(0).sex_id;

	CString rowidDel;
	CString parent_id;
	int		nItem;
	
	for (int i = 1; i < vUnite.size(); ++i)
	{
		nItem		= vUnite.at(i).nItem;
		rowidDel	= vUnite.at(i).rowid;
		// gyerekek szüleinek update-elése
		if (sex_id == MAN)
			m_command.Format(L"UPDATE people SET father_id = '%s' WHERE father_id='%s'", rowidKeep, rowidDel);
		else
			m_command.Format(L"UPDATE people SET mother_id = '%s' WHERE mother_id='%s'", rowidKeep, rowidDel);
		if (!theApp.execute(m_command)) return false;

		// házassági bejegyzések törlése
		if (sex_id == MAN)
			m_command.Format(L"DELETE FROM marriages WHERE husband_id ='%s'", rowidDel);
		else
			m_command.Format(L"DELETE FROM marriages WHERE wife_id ='%s'", rowidDel);
		if (!theApp.execute(m_command)) return false;

		// törlendõ bejegyzés törlése
		m_command.Format(L"DELETE FROM people WHERE rowid ='%s'", rowidDel);
		if (!theApp.execute(m_command)) return false;
	}
//	OnNoidentification();
	m_ListCtrl.EnsureVisible(nItem0 + m_ListCtrl.GetCountPerPage() - 1, false);
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnCompare2People()
{
	int cnt = m_ListCtrl.GetSelectedCount();
	if (cnt != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni");
		return;
	}
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();

	int nItem1 = m_ListCtrl.GetNextSelectedItem(pos);
	int nItem2 = m_ListCtrl.GetNextSelectedItem(pos);

	CCompareEntries dlg;
	dlg.rowid1 = m_ListCtrl.GetItemText(nItem1, H_ROWID);
	dlg.rowid2 = m_ListCtrl.GetItemText(nItem2, H_ROWID);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 létezõ bejegyzést jelölj meg!");
		return;
	}
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnKekule()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(L"Nincs kijelölve senki!");
		return;
	}
	CString rowid;
	rowid = m_ListCtrl.GetItemText(nItem, G_ROWID);
	CAscendants dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnCompareAsc()
{

	int db = m_ListCtrl.GetSelectedCount();
	if (db != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni!");
		return;
	}

	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int nItem;

	CString rowid1;
	CString rowid2;

	nItem = m_ListCtrl.GetNextSelectedItem(pos);
	rowid1 = m_ListCtrl.GetItemText(nItem, H_ROWID);

	nItem = m_ListCtrl.GetNextSelectedItem(pos);
	rowid2 = m_ListCtrl.GetItemText(nItem, H_ROWID);

	CCompareAsc dlg;
	dlg.rowid1 = rowid1;
	dlg.rowid2 = rowid2;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnRokonok()
{
	int cnt = m_ListCtrl.GetSelectedCount();
	if (cnt != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni");
		return;
	}
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();

	int nItem1 = m_ListCtrl.GetNextSelectedItem(pos);
	int nItem2 = m_ListCtrl.GetNextSelectedItem(pos);

	CCompareEntries dlg;
	dlg.rowid1 = m_ListCtrl.GetItemText(nItem1, H_ROWID);
	dlg.rowid2 = m_ListCtrl.GetItemText(nItem2, H_ROWID);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 létezõ bejegyzést jelölj meg!");
		return;
	}
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnDescendants()
{
	int nItem;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (!pos)
	{
		theApp.message(L"Kijelölt õsök listája", L"\nNincs kijelölve senki!");
		return;
	}

	theApp.v_rowid.clear();
	theApp.v_tableNumbers.clear();
	CString rowid;
	CString tableNumber;

	while (pos)
	{
		nItem = m_ListCtrl.GetNextSelectedItem(pos);
		rowid = m_ListCtrl.GetItemText(nItem, G_ROWID);
		m_command.Format(L"SELECT rowid FROM people WHERE father_id ='%s' OR mother_id='%s'", rowid, rowid);
		if (!query(m_command)) return;
		if (!m_recordset->RecordsCount())
		{
			str.Format(L"%s rowid bejegyzésnek nincs leszármoazottja", rowid);
			AfxMessageBox(str);
			return;
		}

		theApp.v_rowid.push_back(rowid);
//		if (theApp.m_inputMode == GAHTML)
		{
			tableNumber = m_ListCtrl.GetItemText(nItem, G_TABLENUMBER);
//			theApp.v_tableNumbers.push_back(tableNumber);
		}
//		else
//			theApp.v_tableNumbers.push_back(L"1");
	}
	CDescendants dlg;
	dlg.DoModal();
}

