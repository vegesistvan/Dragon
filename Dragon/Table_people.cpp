// TableIndividuals.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GEDCOM table

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "utilities.h"
#include "Table_people.h"
#include "afxdialogex.h"
#include "Filter.h"
#include "Relations.h"
#include "SelectOne.h"
#include "Table_files.h"
#include "Table_tables.h"
#include "GetString.h"
#include "Commctrl.h"
#include "SameParams.h"
#include "html_EditLine.h"
#include "html_EditLines.h"
#include "GA_ascendants.h"
#include "GetLastFirst.h"
#include "EditPeople.h"
#include "GA_descendants.h"
#include "GA_ascendants.h"
#include "GA_ascendantsChain.h"
#include "pictures.h"
#include "Relations.h"
#include "GedcomOut.h"
#include "NewPeople.h"
#include "Message.h"
#include "ProgressWnd.h"

// GEDCOM
enum
{
	G_ROWID = 0,
	G_ROWID_FILE,
	G_FAMILYNUMBER,
	G_TABLENUMBER,
	G_SEX,
	G_TITLE,
	G_TITOLO,
	G_LAST_NAME,
	G_FIRST_NAME,
	G_BIRTH_PLACE,
	G_BIRTH_DATE,
	G_DEATH_PLACE,
	G_DEATH_DATE,
	G_AGE,
	G_ORDERFATHER,
	G_ORDERMOTHER,
	G_COMMENT,
	G_OCCUPATION,
	G_ROWID_FATHER,
	G_FATHER,
	G_BIRTH_DATE_FATHER,
	G_ROWID_MOTHER,
	G_MOTHER,
	G_BIRTH_DATE_MOTHER,
};
// GAHTML
enum
{
	L_ROWID = 0,
	L_ROWID_FILE,
	L_FAMILYNUMBER,
	L_TABLENUMBER,
	L_LINENUMBER,
	L_SOURCE,

	L_SPOUSE,
	L_SPOUSEPARENT,
	L_SPOUSESPOUSE,
	L_UNITED,
	L_TABLENUMBERROMAN,
	L_GENERATION,

	L_SEX,
	L_TITLE,
	L_TITOLO,
	L_LAST_NAME,
	L_KNOWN_AS,
	L_FIRST_NAME,
	L_POSTERIOR,
	L_BIRTH_PLACE,
	L_BIRTH_DATE,
	L_DEATH_PLACE,
	L_DEATH_DATE,
	L_AGE,
	L_ORDERFATHER,
	L_ORDERMOTHER,
	L_COMMENT,
	L_OCCUPATION,
	L_ROWID_FATHER,
	L_FATHER,
	L_BIRTH_DATE_FATHER,
	L_ROWID_MOTHER,
	L_MOTHER,
	L_BIRTH_DATE_MOTHER,
	L_FOLYT,
	L_ROW,
};
// MANUAL

enum
{
	N_ROWID = 0,
	N_FAMILYNUMBER,
	N_TABLENUMBER,
	N_TABLE_ANCESTRY,
	N_SEX,
	N_LAST_NAME,
	N_FIRST_NAME,
	N_BIRTH_PLACE,
	N_BIRTH_DATE,
	N_DEATH_PLACE,
	N_DEATH_DATE,
	N_AGE,
	N_ORDERFATHER,
	N_ORDERMOTHER,
	N_COMMENT,
	N_OCCUPATION,
	N_ROWID_FATHER,
	N_FATHER,
	N_BIRTH_DATE_FATHER,
	N_ROWID_MOTHER,
	N_MOTHER,
	N_BIRTH_DATE_MOTHER,
};


IMPLEMENT_DYNAMIC(CTablePeople, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTablePeople::CTablePeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTablePeople::IDD, pParent)
{
	m_dlg = pParent;

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
	DDX_Control(pDX, IDC_KERES, colorKeres);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTablePeople, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	
	ON_COMMAND(ID_FILTER_UNFILTER, &CTablePeople::OnFilterUnfilter)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTablePeople::OnDblclkList)

// DROPDOWN menü funkciók
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_DB_EDIT, &CTablePeople::OnDbEdit)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CTablePeople::OnHtmlNotepadParents )
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CTablePeople::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_HTML_EDIT, &CTablePeople::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CTablePeople::OnHtmlNotepad)	
	ON_COMMAND(ID_HTML_CHILDREN, &CTablePeople::OnHtmlChildren)


	ON_COMMAND(ID_EDIT_DELETE, &CTablePeople::OnEditDelete)
	ON_COMMAND(ID_EDIT_UPDATE, &CTablePeople::OnEditUpdate)
	ON_COMMAND(ID_EDIT_INSERT, &CTablePeople::OnEditInsert)
	ON_COMMAND(ID_3GENERATIONS, &CTablePeople::On3Generations )


	ON_COMMAND(ID_FILTER_FILE, &CTablePeople::OnFilterFile)
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
	ON_COMMAND(ID_GALINE, &CTablePeople::OnGaLine)
	ON_COMMAND(ID_EXPORT_SELECTED, &CTablePeople::OnExportSelected)
	ON_COMMAND(ID_EXPORT_ALL, &CTablePeople::OnExportAll)
ON_COMMAND(ID_FILTER_FOLYT, &CTablePeople::OnFilterFolyt)
ON_WM_CLOSE()
ON_COMMAND(ID_AZONOSEMBER_IDENT, &CTablePeople::OnAzonosemberIdent)
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
ON_COMMAND(ID_ASCENDANTS, &CTablePeople::OnAscendants)
ON_COMMAND(ID_INPUT_MANUAL, &CTablePeople::OnInputManual)
ON_WM_NCDESTROY()
ON_MESSAGE(WM_EDIT_INDIVIDUAL, OnUpdateRecord )
ON_COMMAND(ID_DESCENDANTS_INDIVIDUAL, &CTablePeople::OnIndividualDescendants)
ON_COMMAND(ID_ASCENDANTS_CHAIN, &CTablePeople::OnAscendantsChain)
ON_COMMAND(ID_GEDCOM_OUTPUT, &CTablePeople::OnGedcomOutput)
ON_COMMAND(ID_PRIVATE_DESCENDANTS, &CTablePeople::OnPrivateDescendants)
ON_COMMAND(ID_FILTER_BISEX, &CTablePeople::OnFilterBisex)
ON_STN_CLICKED(IDC_KERES, &CTablePeople::OnClickedKeres)

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();

	CString caption;
	caption.Format( L"Emberek" );
	SetWindowTextW( caption );

	colorKeres.SetTextColor( theApp.m_colorClick );

	ChangeMenu();

	if( !theApp.m_cntPeople )
		enableMenu( MF_GRAYED );
	m_orderix = 0;
	createListColumns();
	
	if( !theApp.m_cntPeople )
		return FALSE;
		

	menu.EnableMenuItem( 1,MF_BYPOSITION| MF_GRAYED );
	menu.EnableMenuItem( 2,MF_BYPOSITION| MF_GRAYED );
	menu.EnableMenuItem( 3,MF_BYPOSITION| MF_GRAYED );
	menu.EnableMenuItem( 4,MF_BYPOSITION| MF_GRAYED );

	if( theApp.m_cntPeople && theApp.m_cntPeople  < 10000 )
		OnFilterUnfilter();	

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);   // ez volt!!

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
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
/////////////////////////////////////// F I L T E R /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterUnfilter()
{
	m_filterNew.Empty();
	m_filterTextNew = L"Az adatbázisban lévõ emberek";
	m_filter.Empty();
	m_filterText.Empty();
	m_orderix	= 0;
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

	m_filterTextNew.Format(L"A %s családban lévõ emberek", dlg.m_selected);
	m_filterNew.Format(  L"familyNumber='%s'", dlg.m_id );
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

	m_filterNew.Format(L"tableNumber='%s'", dlg.m_id );
	m_filterTextNew.Format(L"A %s táblázatban lévõ emberek", dlg.m_selected);

	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterAncestors()
{
	if( theApp.m_inputMode != GAHTML )
	{
		m_filterTextNew = L"- õsök - férfiak, akiknek nincsenek szülei, de van gyereke";
		m_filterNew = L"sex_id = 1 AND (typeof(father_id) = 'null' OR father_id == 0 OR father_id = '' )\
AND ( rowid IN (SELECT father_id FROM people ) OR rowid IN (SELECT mother_id FROM people) )";
	}
	else
	{
		m_filterNew.Format(L"tableAncestry!='0'" );
		m_filterNew.Format(L"generation=='A'" );
		m_filterTextNew.Format(L"Az adatbázisban lévõ õsõk");
	}
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterFile()
{
	CTableFiles dlg;
	if( dlg.DoModal() == IDCANCEL ) return;

	m_filterNew.Format(L"fileNumber='%s'", dlg.m_selected_id );
	m_filterTextNew.Format(L"A %s. fájlban lévõ emberek", dlg.m_selected);
	fillTable(0);	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterIndividual()
{
	CGetString dlg;

	int rowid;
	rowid = theApp.GetProfileInt(	L"Settings", L"rowid", 0 );
	dlg.m_string.Format( L"%d", rowid ); 
	dlg.m_caption.Format( L"Add meg a kívánt ember rowid-ját!" );
	if( dlg.DoModal() == IDCANCEL ) return;
	rowid = _wtoi( dlg.m_string );
	theApp.WriteProfileInt( L"Settings", L"rowid", rowid  );

	m_filterTextNew.Format(L"Ember, akineka rowid-ja %s", dlg.m_string );
	m_filterNew.Format(  L"rowid=%s", dlg.m_string );
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

	m_filterNew.Format(L"last_name='%s'",dlg.m_selected );
	m_filterTextNew.Format(L"%s családnevû emberek", dlg.m_selected);
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

	m_filterNew.Format(L"first_name='%s'",dlg.m_selected );
	m_filterTextNew.Format(L"%s utónevû emberek", dlg.m_selected);
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CTablePeople::OnGivenfFamilyName()
//{
//	CString familyName;
//	CGetString dlg;
//
//	dlg.m_string  = theApp.GetProfileString( L"Settings", L"familyName2", L"Vermes" );
//	dlg.m_caption = L"Add meg a keresett családnevet!";
//	if( dlg.DoModal() == IDCANCEL ) return;
//	
//	familyName = dlg.m_string;
//
//	theApp.WriteProfileStringW( L"Settings", L"familyName2", familyName );
//
//	m_filterTextNew.Format( L"%s családnevû emberek az adatbázisban", familyName );
//	m_filterNew.Format( L"last_name='%s'", familyName );
//	fillTable( 0 );
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGivenname()
{
	CGetLastFirst dlg;
	if( dlg.DoModal() == IDCANCEL ) return;

	m_filterTextNew.Format( L"%s nevû emberek az adatbázisban", dlg._fullname );
	if( dlg._firstname.IsEmpty() )
	{
		m_filterTextNew.Format( L"'%s'-el kezdõdõ családnevû emberek az adatbázisban", dlg._fullname );
		m_filterNew.Format( L"last_name LIKE'%s%c'", dlg._lastname, '%' );
	}
	else
	{
		m_filterTextNew.Format( L"%s nevû emberek az adatbázisban", dlg._fullname );
		m_filterNew.Format( L"first_name='%s' AND last_name='%s'", dlg._firstname, dlg._lastname );
	}
	fillTable( 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterNodescendant()
{
	m_filterTextNew = L"Emberek, akiknek nincsenek leszármazottai";
	m_filterNew = L"rowid NOT IN (SELECT father_id FROM people WHERE father_id != '0') AND rowid NOT IN (SELECT mother_id FROM people WHERE mother_id!='0') AND rowid != '0'";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterNoparents()
{
	m_filterTextNew = L"Emberek, akiknek nincsenek szülei";
	m_filterNew = L"(father_id=0 OR typeof( father_id ) = 'null' ) AND ( mother_id=0 OR typeof( mother_id ) = 'null' ) ";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterNospouse()
{
	m_filterTextNew = L"Emberek , akiknek nincs házastársa";
	m_filterNew = L"rowid NOT IN (SELECT spouse1_id FROM marriages) AND rowid NOT IN (SELECT spouse2_id FROM marriages )";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterMen()
{
	m_filterTextNew.Format(L"Az adatbázisban lévõ férfiak");
	m_filterNew.Format(  L"sex_id=1" );
	fillTable(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterWomen()
{
	m_filterTextNew.Format(L"Az adatbázisban lévõ nõk");
	m_filterNew.Format(  L"sex_id=2" );
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterBisex()
{
	m_filterTextNew.Format(L"Az adatbázisban lévõ bisex emberek");
	m_filterNew.Format(  L"sex_id=0" );
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterFolyt()
{
	m_filterTextNew.Format(L"Elágazások");
	m_filterNew.Format(  L"folyt != ''" );
	fillTable(0);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterDescendants()
{
	m_filterTextNew	= L"Leszármazottak";
	m_filterNew		= L"source=1"; 
	fillTable(0);
}
void CTablePeople::OnFilterSpouses()
{
	m_filterTextNew	= L"Leszármazottak házastársai";
	m_filterNew		= L"source=2"; 
	fillTable(0);
}
void CTablePeople::OnFilterFathers()
{
	m_filterTextNew	= L"Leszármazottak házastársainak apjai";
	m_filterNew		= L"source=3 AND sex_id=1"; 
	fillTable(0);
}

void CTablePeople::OnFilterMothers()
{
	m_filterTextNew	= L"Leszármazottak házastársainak anyjai";
	m_filterNew		= L"source=3 AND sex_id==2"; 
	fillTable(0);
}

void CTablePeople::OnFilterSpousespouses()
{
	m_filterTextNew	= L"Leszármazottak házastársainak további házastársai";
	m_filterNew		= L"source=5"; 
	m_filterNew		= L"source=4"; 
	fillTable(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// G R O U P   B Y  ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyTitolo()
{
	m_filterNew.Format(L"GROUP BY titolo");
	m_filterTextNew.Format(L"Különbözõ elõnevek" );
	m_orderix = L_TITOLO;
	fillTable(0);
}
void CTablePeople::OnGroupbyBirthplace()
{
	m_filterNew.Format(L"GROUP BY birth_place");
	m_filterTextNew.Format(L"Különbözõ születési helyek" );
	m_orderix = L_BIRTH_PLACE;
	fillTable(0);

}
void CTablePeople::OnGroupbyDeathplace()
{
	m_filterNew.Format(L"GROUP BY death_place");
	m_filterTextNew.Format(L"Különbözõ halálozási helyek" );
	m_orderix = L_DEATH_PLACE;
	fillTable(0);
}
void CTablePeople::OnGroupbyFirstname()
{
	m_filterNew.Format(L"GROUP BY first_name");
	m_filterTextNew.Format(L"Különbözõ keresztnevek" );
	m_orderix = L_FIRST_NAME;
	fillTable(0);
}
void CTablePeople::OnGroupbyLastname()
{
	m_filterNew.Format(L"GROUP BY last_name");
	m_filterTextNew.Format(L"Különbözõ családnevek" );
	m_orderix = L_LAST_NAME;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyBirthDate()
{
	m_filterNew.Format(L"GROUP BY birth_date");
	m_filterTextNew.Format(L"Különbözõ születési idõk" );
	m_orderix = L_BIRTH_DATE;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyDeathdate()
{
	m_filterNew.Format(L"GROUP BY death_date");
	m_filterTextNew.Format(L"Különbözõ halálozási idõk" );
	m_orderix = L_DEATH_DATE;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyComment()
{
	m_filterNew.Format(L"GROUP BY comment");
	m_filterTextNew.Format(L"Különbözõ megjegyzések" );
	m_orderix = L_COMMENT;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CTablePeople::createListColumns( )
{
	m_ListCtrl.DeleteAllItems();
	for( int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i-- )
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );

	if( theApp.m_inputMode == GEDCOM )
	{

	m_ListCtrl.InsertColumn( G_ROWID,				L"rowid",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( G_ROWID_FILE,			L"fájl#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( G_FAMILYNUMBER,		L"family#",			LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( G_TABLENUMBER,			L"tábla#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( G_SEX,					L"nem",				LVCFMT_LEFT,	 40,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( G_TITLE,				L"tit",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_TITOLO,				L"elõnév",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_LAST_NAME,			L"családnév",		LVCFMT_LEFT,	110,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_FIRST_NAME,			L"utónév",			LVCFMT_LEFT,	110,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_BIRTH_PLACE,			L"születés",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_BIRTH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_DEATH_PLACE,			L"elhalálozás",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_DEATH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_AGE,					L"kor",				LVCFMT_LEFT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( G_ORDERFATHER,			L"apjacnt",			LVCFMT_LEFT,	30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( G_ORDERMOTHER,			L"anyjacnt",		LVCFMT_LEFT,	30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( G_COMMENT,				L"leírás",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_OCCUPATION,			L"foglalkozás",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_ROWID_FATHER,		L"apa_rowid",		LVCFMT_RIGHT,	50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( G_FATHER,				L"apja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_BIRTH_DATE_FATHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_ROWID_MOTHER,		L"anya_rowid",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_MOTHER,				L"anyja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_BIRTH_DATE_MOTHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);
	}
	else if( theApp.m_inputMode == GAHTML )
	{

		m_ListCtrl.InsertColumn( L_ROWID,				L"rowid",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_ROWID_FILE,			L"fájl#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_FAMILYNUMBER,		L"család#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_TABLENUMBER,			L"tábla#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_LINENUMBER,			L"line#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_SOURCE,				L"src",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_SPOUSE,				L"sp",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_SPOUSEPARENT,		L"spp",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_SPOUSESPOUSE,		L"ssp",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_UNITED,				L"egyesített",		LVCFMT_RIGHT,	 40,-1,COL_NUM);

		m_ListCtrl.InsertColumn( L_TABLENUMBERROMAN,	L"tábla",			LVCFMT_RIGHT,	 40,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_GENERATION,			L"G",				LVCFMT_LEFT,	 25,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_SEX,					L"nem",				LVCFMT_LEFT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_TITLE,				L"tit",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_TITOLO,				L"elõnév",			LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_LAST_NAME,			L"családnév",		LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_KNOWN_AS,			L"más",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_FIRST_NAME,			L"utónév",			LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_POSTERIOR,			L"utótag",			LVCFMT_LEFT,	 60,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_BIRTH_PLACE,			L"születés",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_BIRTH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_DEATH_PLACE,			L"elhalálozás",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_DEATH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_AGE,					L"kor",				LVCFMT_LEFT,	 30,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_ORDERFATHER,		L"apjacnt",			LVCFMT_LEFT,	30,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_ORDERMOTHER,		L"anyjacnt",		LVCFMT_LEFT,	30,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_COMMENT,				L"leírás",		LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_OCCUPATION,			L"foglalkozás",		LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_ROWID_FATHER,		L"apa_rowid",		LVCFMT_RIGHT,	50,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_FATHER,				L"apja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_BIRTH_DATE_FATHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_ROWID_MOTHER,		L"anya_rowid",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_MOTHER,				L"anyja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_BIRTH_DATE_MOTHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_FOLYT,				L"elágazás",		LVCFMT_LEFT,	50,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_ROW,					L"record",			LVCFMT_RIGHT,	50,-1,COL_NUM);
	}
	else if( theApp.m_inputMode == MANUAL )
	{
		m_ListCtrl.InsertColumn( N_ROWID,				L"rowid",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_FAMILYNUMBER,		L"család#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_TABLENUMBER,			L"tábla#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_TABLE_ANCESTRY,		L"õs",				LVCFMT_RIGHT,	 20,-1,COL_HIDDEN );
		m_ListCtrl.InsertColumn( N_SEX,					L"nem",				LVCFMT_RIGHT,	 20,-1,COL_HIDDEN);
		m_ListCtrl.InsertColumn( N_LAST_NAME,			L"családnév",		LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_FIRST_NAME,			L"utónév",			LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_BIRTH_PLACE,			L"születés",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_BIRTH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_DEATH_PLACE,			L"elhalálozás",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_DEATH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_AGE,					L"kor",				LVCFMT_LEFT,	 30,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_ORDERFATHER,			L"apjacnt",			LVCFMT_LEFT,	30,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_ORDERFATHER,			L"anyjacnt",		LVCFMT_LEFT,	30,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_COMMENT,				L"leírás",		LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_OCCUPATION,			L"foglalkozás",		LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_ROWID_FATHER,		L"apa_rowid",		LVCFMT_RIGHT,	50,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_FATHER,				L"apja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_BIRTH_DATE_FATHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_ROWID_MOTHER,		L"anya_rowid",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_MOTHER,				L"anyja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_BIRTH_DATE_MOTHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);
	}
	m_columnsCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();

	

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::fillTable( UINT nItem )
{
	CFilter dlg;
	int		ret;
	CString	relation;
	CString command;
	CString	str;

	CProgressWnd wndProgress(NULL, m_filterTextNew ); 
	wndProgress.GoModal();

	if( m_filterNew.IsEmpty() )
	{
		m_filterText = m_filterTextNew;
		m_filter.Empty();
		m_command = L"SELECT rowid,* FROM people";
	
	}
	else if( m_filterNew.Left(5) != L"GROUP" )
	{
		if( !m_filter.IsEmpty() && m_filter.Left(5) != L"GROUP" )					// már van egy szûrés)					// már van egy szûrés
		{
			dlg.m_condition1 = m_filterText; 
			dlg.m_condition2 = m_filterTextNew; 
			ret = dlg.DoModal();
			switch( ret )
			{
			case AND:
				relation = L"AND"; 
				break;
			case OR:
				relation = L"OR"; 
				break;
			case ONLY1:
				return;
			case ONLY2:
				m_filter.Empty();
				break;
			}
			if( !m_filter.IsEmpty() )  // több feltétel szerinti szûrés
			{
				str.Format(L"(%s) %s %s",m_filter, relation, m_filterNew );
				m_filterNew = str;
				str.Format(L"(%s) %s %s", m_filterText, relation, m_filterTextNew );					// a korábbi és az új szûrõfeltétel összeépítésa
				m_filterTextNew = str;
			}
		}
		m_filter		= m_filterNew;
		m_filterText	= m_filterTextNew;
		m_command.Format( L"SELECT rowid,* FROM people WHERE %s", m_filter );
	}
	else  // GROUP BY
	{
		m_filter		= m_filterNew;
		m_filterText	= m_filterTextNew;
		m_command.Format( L"SELECT rowid,* FROM people %s", m_filter );
	}
	
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( m_filterText);
	if( !query( m_command ) ) return;

	m_ListCtrl.DeleteAllItems();

	wndProgress.SetRange(0, m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	v_individuals.clear();
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i )
	{
		fillRow(i);
		m_recordset->MoveNext();

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.AttachDataset( &v_individuals );
//	m_ListCtrl.ReSort();

//	m_ListCtrl.SetSortOrder( 1, m_orderix+1 );

	int cntPerPage = m_ListCtrl.GetCountPerPage();
	if( nItem > 0 )
	{
		m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		nItem += cntPerPage/2;
		if( nItem > m_recordset->RecordsCount() )
			nItem = m_recordset->RecordsCount() - 1;
		m_ListCtrl.EnsureVisible( nItem, FALSE );
	}

//	m_ListCtrl.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	enableMenu( MF_ENABLED );
	str.Format( L"%s (%d)", m_filterText, m_ListCtrl.GetItemCount() );
	SetWindowTextW( str );
	SetForegroundWindow();   //


	if( m_ListCtrl.GetItemCount() )
	{
		menu.EnableMenuItem( 1,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 2,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 3,MF_BYPOSITION| MF_ENABLED );
		menu.EnableMenuItem( 4,MF_BYPOSITION| MF_ENABLED );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::fillRow( UINT i )
{
	int age = 0;
	int birth;
	int death;
	CString last_name;
	CString kor(L"");
	CString m_index(L"");
	CString table_id_father;
	CString table_id_mother;
	CString birth_date_father;
	CString birth_date_mother;
	CString row;
	PARENT father;
	PARENT mother;

	row.Format( L"%d", i );
	str = m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
	birth = _wtoi( str.Left( 4 ) );
	str = m_recordset->GetFieldString( PEOPLE_DEATH_DATE );
	death = _wtoi( str.Left( 4 ) );
	if( birth && death )
	{
			age = death-birth;
			kor.Format( L"%d", age );
	}

	CString father_id = m_recordset->GetFieldString( PEOPLE_FATHER_ID );
	CString mother_id = m_recordset->GetFieldString( PEOPLE_MOTHER_ID );
	parent( father_id, &father );
	parent( mother_id, &mother );
	if( father_id == L"0" ) father_id = L""; 
	if( mother_id == L"0" ) mother_id = L""; 

	int parentIndex = _wtoi( m_recordset->GetFieldString( PEOPLE_MOTHER_INDEX ) );
	if( parentIndex )
	{
		m_index.Format( L"%d", parentIndex );
	}

	CString rowid;
	CString lastName;
	CString firstName;
	int tLen;

	if( theApp.m_inputMode == GEDCOM )
	{
		rowid		= m_recordset->GetFieldString( PEOPLE_ROWID );
		lastName	= m_recordset->GetFieldString( PEOPLE_LAST_NAME );
		lastName.Replace( '|', '\'' );
		firstName	= m_recordset->GetFieldString( PEOPLE_FIRST_NAME );

		push( rowid );
		push( m_recordset->GetFieldString( PEOPLE_FILENUMBER ) );
		push( L"1" );													// familyNumber
		push( m_recordset->GetFieldString( PEOPLE_TABLENUMBER ) );
		push( m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
		push( m_recordset->GetFieldString( PEOPLE_TITLE ));
		push( m_recordset->GetFieldString( PEOPLE_TITOLO ));

		

/*
		tLen=last_name.GetLength()+1;
		lastName = new TCHAR[tLen];
		_tcscpy_s( lastName,tLen,last_name.GetBuffer());

		str = UnicodeToMulti( Utf8ToUnicode( lastName ) );
*/
//		push( str );

		push( lastName );
		push( firstName );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		push( kor );
		push( m_recordset->GetFieldString( PEOPLE_ORDERFATHER ) );
		push( m_recordset->GetFieldString( PEOPLE_ORDERMOTHER ) );

		push( m_recordset->GetFieldString( PEOPLE_COMMENT ) );
		push( m_recordset->GetFieldString( PEOPLE_OCCUPATION ) );
		push( father_id );
		push( father.name );
		push( father.birth_date );
		push( mother_id );
		push( mother.name );
		push( mother.birth_date );
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		CString united = m_recordset->GetFieldString( PEOPLE_UNITED );
		if( united == L"1" ) united.Empty();

		CString spouse = m_recordset->GetFieldString( PEOPLE_SPOUSE );
		if( spouse == L"0" ) spouse.Empty();

		CString spouseparent = m_recordset->GetFieldString( PEOPLE_SPOUSEPARENT );
		if( spouseparent == L"0" ) spouseparent.Empty();

		CString spousespouse = m_recordset->GetFieldString( PEOPLE_SPOUSESPOUSE );
		if( spousespouse == L"0" ) spousespouse.Empty();


		push( m_recordset->GetFieldString( PEOPLE_ROWID ) );
		push( m_recordset->GetFieldString( PEOPLE_FILENUMBER ) );
		push( m_recordset->GetFieldString( PEOPLE_FAMILYNUMBER ) );
		push( m_recordset->GetFieldString( PEOPLE_TABLENUMBER ) );
		push( m_recordset->GetFieldString( PEOPLE_LINENUMBER ) );
		push( m_recordset->GetFieldString( PEOPLE_SOURCE ) );
		push( spouse );
		push( spouseparent );
		push( spousespouse );
		push( united );
		push( m_recordset->GetFieldString( PEOPLE_TABLEROMAN ) );
		push( m_recordset->GetFieldString( PEOPLE_GENERATION ) );
		push( m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
		push( m_recordset->GetFieldString( PEOPLE_TITLE ));
		push( m_recordset->GetFieldString( PEOPLE_TITOLO ));
		last_name = m_recordset->GetFieldString( PEOPLE_LAST_NAME );
		last_name.Replace( '|', '\'' );
		push( last_name );
		push( m_recordset->GetFieldString( PEOPLE_KNOWN_AS ) );
		push( m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		push( m_recordset->GetFieldString( PEOPLE_POSTERIOR ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		push( kor );
		push( m_recordset->GetFieldString( PEOPLE_ORDERFATHER ) );
		push( m_recordset->GetFieldString( PEOPLE_ORDERMOTHER ) );
		push( m_recordset->GetFieldString( PEOPLE_COMMENT ) );
		push( m_recordset->GetFieldString( PEOPLE_OCCUPATION ) );
		push( father_id );
		push( father.name );
		push( father.birth_date );
		push( mother_id );
		push( mother.name );
		push( mother.birth_date );
		push( m_recordset->GetFieldString( PEOPLE_FOLYT ) );
		push( row );
	}
	else if( theApp.m_inputMode == MANUAL )
	{
		push( m_recordset->GetFieldString( PEOPLE_ROWID ) );
		push( m_recordset->GetFieldString( PEOPLE_FAMILYNUMBER ) );
		push( m_recordset->GetFieldString( PEOPLE_TABLENUMBER ) );
		str = m_recordset->GetFieldString( PEOPLE_TABLEANCESTRY );
		if( str == L"1" )
			push( m_recordset->GetFieldString( PEOPLE_TABLEANCESTRY ) );
		else
			push( L"" );
		push( m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
		last_name = m_recordset->GetFieldString( PEOPLE_LAST_NAME );
		last_name.Replace( '|', '\'' );
		push( last_name );
		push( m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		push( kor );
		push( m_recordset->GetFieldString( PEOPLE_ORDERFATHER ) );
		push( m_recordset->GetFieldString( PEOPLE_ORDERMOTHER ) );
		push( m_recordset->GetFieldString( PEOPLE_COMMENT ) );
		push( m_recordset->GetFieldString( PEOPLE_OCCUPATION ) );
		push( father_id );
		push( father.name );
		push( father.birth_date );
		push( mother_id );
		push( mother.name );
		push( mother.birth_date );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::push( CString str )
{
	const TCHAR* pC = (LPCTSTR) str;
//	v_individuals.push_back( (LPTSTR)pC); nem jó!!!

	int tLen=str.GetLength()+1;
	LPTSTR sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,str.GetBuffer());

	v_individuals.push_back( sT);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		theApp.message( L"Query", str );
//		AfxMessageBox(str);
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
//		AfxMessageBox(str);
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
//		AfxMessageBox(str);
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
//		AfxMessageBox(str);
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
//		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	m_rowid		= m_ListCtrl.GetItemText( nItem, N_ROWID );

	str.Format( L"%s %s adatainak módosítása", m_ListCtrl.GetItemText( nItem, N_LAST_NAME ), m_ListCtrl.GetItemText( nItem, N_FIRST_NAME ) );
//	ShowWindow( SW_HIDE );

	OnDbEdit();

	SetForegroundWindow();

//	updateRow( nItem );
	*pResult = 0;
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

	ShowWindow( SW_HIDE );

	CNewPeople dlg;
	dlg.m_caption = L"Adj meg egy új embert!";
	if( dlg.DoModal() == IDCANCEL ) return;
	ShowWindow( SW_RESTORE );

	if( dlg.m_inserted )
	{
		m_command.Format( L"SELECT rowid, * FROM people WHERE rowid ='%s'", dlg.m_rowid );
		if( !query( m_command ) ) return;


		fillRow( 0 );

		int nItem  = m_ListCtrl.GetItemCount();
		m_ListCtrl.DeleteAllItems();
		m_ListCtrl.AttachDataset( &v_individuals );
		m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		m_ListCtrl.EnsureVisible( nItem, FALSE );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CTablePeople::find_nItem( int rowid )
{
	for( int i = 0; i < m_ListCtrl.GetItemCount(); ++i )
	{
		if( _wtoi( m_ListCtrl.GetItemText( i, 0 ) ) == rowid )
			return i;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::insertRow()
{
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid='%s'", m_rowid );
	if( !query( m_command ) ) return;
	if( !m_recordset->RecordsCount() ) return;	
	fillRow(1);

	m_ListCtrl.DeleteAllItems();
//	m_ListCtrl.SetItemCountEx( theApp.m_recordset->RecordsCount() );
	m_ListCtrl.AttachDataset( &v_individuals );
	m_ListCtrl.ReSort();
	int nItem = m_ListCtrl.GetItemCount() -1;
	m_ListCtrl.EnsureVisible( nItem, TRUE );
	m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::updateRow( int nItem )
{
	CString rowid;
	rowid = m_ListCtrl.GetItemText( nItem, G_ROWID );

	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid );
	if( !query( m_command ) ) return;
	if( !m_recordset->RecordsCount() ) return;	


	int birth;
	int death;
	int age;
	CString kor(L"");
	CString rowid_father;
	CString father;
	CString rowid_mother;
	CString mother;
	CString indexM;



	str = m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
	birth = _wtoi( str.Left( 4 ) );
	str = m_recordset->GetFieldString( PEOPLE_DEATH_DATE );
	death = _wtoi( str.Left( 4 ) );
	if( birth && death )
	{
			age = death-birth;
			kor.Format( L"%d", age );
	}

	
	int parentIndex = _wtoi( m_recordset->GetFieldString( PEOPLE_MOTHER_INDEX ) );
	if( parentIndex )
	{
		indexM.Format( L"%d", parentIndex );
	}

	rowid_father = m_recordset->GetFieldString( PEOPLE_FATHER_ID );
	m_command.Format(L"SELECT tableNumber, last_name,first_name, birth_date FROM people WHERE rowid='%s'", rowid_father ); 
	if( !query2( m_command ) ) return;
	father.Format( L"%s %s", m_recordset2->GetFieldString( 1 ), m_recordset2->GetFieldString( 2 ) );

	rowid_mother = m_recordset->GetFieldString( PEOPLE_MOTHER_ID );
	m_command.Format(L"SELECT tableNumber, last_name,first_name, birth_date FROM people WHERE rowid='%s'", rowid_mother ); 
	if( !query2( m_command ) ) return;
	mother.Format( L"%s %s", m_recordset2->GetFieldString( 1 ), m_recordset2->GetFieldString( 2 ));

	m_ListCtrl.DetachDataset();
	if( theApp.m_inputMode == GEDCOM )
	{
		updateField( nItem, G_SEX, m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
		updateField( nItem, G_TITLE, m_recordset->GetFieldString( PEOPLE_TITLE ) );
		updateField( nItem, G_TITOLO, m_recordset->GetFieldString( PEOPLE_TITOLO ) );
		updateField( nItem, G_LAST_NAME, m_recordset->GetFieldString( PEOPLE_LAST_NAME ) );
		updateField( nItem, G_FIRST_NAME, m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		updateField( nItem, G_BIRTH_PLACE, m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		updateField( nItem, G_BIRTH_DATE, m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		updateField( nItem, G_DEATH_PLACE, m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		updateField( nItem, G_DEATH_DATE, m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		updateField( nItem, G_AGE, kor );
		updateField( nItem, G_ORDERFATHER, m_recordset->GetFieldString( PEOPLE_ORDERFATHER ) );
		updateField( nItem, G_ORDERMOTHER, m_recordset->GetFieldString( PEOPLE_ORDERMOTHER ) );
		updateField( nItem, G_COMMENT, m_recordset->GetFieldString( PEOPLE_COMMENT ) );
		updateField( nItem, G_OCCUPATION, m_recordset->GetFieldString( PEOPLE_OCCUPATION ) );
		updateField( nItem, G_ROWID_FATHER, rowid_father );
		updateField( nItem, G_FATHER, father );
		updateField( nItem, G_BIRTH_DATE_FATHER, m_recordset2->GetFieldString( 3) );
		updateField( nItem, G_ROWID_MOTHER, rowid_mother );
		updateField( nItem, G_MOTHER, mother );
		updateField( nItem, G_BIRTH_DATE_MOTHER, m_recordset2->GetFieldString( 3) );
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		updateField( nItem, L_SEX, m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
		updateField( nItem, L_TITLE, m_recordset->GetFieldString( PEOPLE_TITLE ) );
		updateField( nItem, L_TITOLO, m_recordset->GetFieldString( PEOPLE_TITOLO ) );
		updateField( nItem, L_LAST_NAME, m_recordset->GetFieldString( PEOPLE_LAST_NAME ) );
		updateField( nItem, L_FIRST_NAME, m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		updateField( nItem, L_BIRTH_PLACE, m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		updateField( nItem, L_BIRTH_DATE, m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		updateField( nItem, L_DEATH_PLACE, m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		updateField( nItem, L_DEATH_DATE, m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		updateField( nItem, L_AGE, kor );
		updateField( nItem, L_ORDERFATHER, m_recordset->GetFieldString( PEOPLE_ORDERFATHER ) );
		updateField( nItem, L_ORDERMOTHER, m_recordset->GetFieldString( PEOPLE_ORDERMOTHER ) );
		updateField( nItem, L_COMMENT, m_recordset->GetFieldString( PEOPLE_COMMENT ) );
		updateField( nItem, L_OCCUPATION, m_recordset->GetFieldString( PEOPLE_OCCUPATION ) );
		updateField( nItem, L_ROWID_FATHER, rowid_father );
		updateField( nItem, L_FATHER, father );
		updateField( nItem, L_BIRTH_DATE_FATHER, m_recordset2->GetFieldString( 3) );
		updateField( nItem, L_ROWID_MOTHER, rowid_mother );
		updateField( nItem, L_MOTHER, mother );
		updateField( nItem, L_BIRTH_DATE_MOTHER, m_recordset2->GetFieldString( 3) );
	}
	else if( theApp.m_inputMode == MANUAL )
	{
		updateField( nItem, N_TABLE_ANCESTRY, m_recordset->GetFieldString( PEOPLE_TABLEANCESTRY ) );
		updateField( nItem, N_LAST_NAME, m_recordset->GetFieldString( PEOPLE_LAST_NAME ) );
		updateField( nItem, N_FIRST_NAME, m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		updateField( nItem, N_BIRTH_PLACE, m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		updateField( nItem, N_BIRTH_DATE, m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		updateField( nItem, N_DEATH_PLACE, m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		updateField( nItem, N_DEATH_DATE, m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		updateField( nItem, N_AGE, kor );
		updateField( nItem, N_ORDERFATHER, m_recordset->GetFieldString( PEOPLE_ORDERFATHER ) );
		updateField( nItem, N_ORDERMOTHER, m_recordset->GetFieldString( PEOPLE_ORDERMOTHER ) );
		updateField( nItem, N_COMMENT, m_recordset->GetFieldString( PEOPLE_COMMENT ) );
		updateField( nItem, N_OCCUPATION, m_recordset->GetFieldString( PEOPLE_OCCUPATION ) );
		updateField( nItem, N_ROWID_FATHER, rowid_father );
		updateField( nItem, N_FATHER, father );
		updateField( nItem, N_BIRTH_DATE_FATHER, m_recordset2->GetFieldString( 3) );
		updateField( nItem, N_ROWID_MOTHER, rowid_mother );
		updateField( nItem, N_MOTHER, mother );
		updateField( nItem, N_BIRTH_DATE_MOTHER, m_recordset2->GetFieldString( 3) );
	}

	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.AttachDataset( &v_individuals );
	m_ListCtrl.ReSort();
	m_ListCtrl.EnsureVisible( nItem, TRUE );
	m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::updateField( int nItem, int i, CString str )
{
	int tLen=str.GetLength()+1;
	LPTSTR sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,str.GetBuffer());
	v_individuals.at( (nItem) * m_columnsCount + i ) = sT;
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// C R E A T E   G A . H T M L /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnIndividualDescendants()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		theApp.message( L"Emberek listája", L"\nNincs kijelölve senki!" );
		return;
	}

	CGaDescendants dlg;

	dlg.m_numbering	= 0;
	dlg.m_code		= ANSI;
	dlg.m_rowid1 = m_ListCtrl.GetItemText( nItem, G_ROWID );
	dlg.m_source = 0;
	
	if( theApp.m_inputMode == GEDCOM )
	{
		dlg.m_name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, G_LAST_NAME ), m_ListCtrl.GetItemText( nItem, G_FIRST_NAME ) );
		dlg.m_tableNumber	= m_ListCtrl.GetItemText( nItem, G_TABLENUMBER );
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		dlg.m_name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) );
		dlg.m_tableNumber = m_ListCtrl.GetItemText( nItem, L_TABLENUMBER );
		dlg.m_source	= _wtoi( m_ListCtrl.GetItemText( nItem, L_SOURCE ) );
	}
	else
	{
		dlg.m_name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, N_LAST_NAME ), m_ListCtrl.GetItemText( nItem, N_FIRST_NAME ) );
		dlg.m_tableNumber = m_ListCtrl.GetItemText( nItem, N_TABLENUMBER );
	}

	if( dlg.DoModal() == IDCANCEL ) return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnPrivateDescendants()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		theApp.message( L"Emberek listája", L"\nNincs kijelölve senki!" );
		return;
	}

	CGaDescendants dlg;

	dlg.m_numbering	= 1;
	dlg.m_code		= ANSI;
	dlg.m_rowid1 = m_ListCtrl.GetItemText( nItem, G_ROWID );
	dlg.m_source = 0;
	
	if( theApp.m_inputMode == GEDCOM )
	{
		dlg.m_name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, G_LAST_NAME ), m_ListCtrl.GetItemText( nItem, G_FIRST_NAME ) );
		dlg.m_tableNumber	= m_ListCtrl.GetItemText( nItem, G_TABLENUMBER );
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		dlg.m_name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) );
		dlg.m_tableNumber = m_ListCtrl.GetItemText( nItem, L_TABLENUMBER );
		dlg.m_source	= _wtoi( m_ListCtrl.GetItemText( nItem, L_SOURCE ) );
	}
	else
	{
		dlg.m_name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, N_LAST_NAME ), m_ListCtrl.GetItemText( nItem, N_FIRST_NAME ) );
		dlg.m_tableNumber = m_ListCtrl.GetItemText( nItem, N_TABLENUMBER );
	}

	if( dlg.DoModal() == IDCANCEL ) return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGaLine()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );

	ShowWindow( SW_HIDE );
	theApp.listHtmlLine( lineNumber );
	ShowWindow( SW_RESTORE );
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
	}
	DrawMenuBar();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////// K I J E L Ö L T   E M B E R R E L   A Z O N O S   N E V Û E M B E R E K   L I S T Á Z Á S A ///////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CTablePeople::OnAzonosember()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		theApp.message( L"Emberek listája", L"\nNincs kijelölve senki!" );
//		AfxMessageBox( L"Nincs kijelölve senki!" );
//		ShowWindow(SW_RESTORE);
//		SetForegroundWindow();
		return;  // hova megy vissza?
	}
	CString firstName;
	CString lastName;

	firstName	= m_ListCtrl.GetItemText( nItem, L_FIRST_NAME );
	lastName	= m_ListCtrl.GetItemText( nItem, L_LAST_NAME );
	CSameP same;
//	CIdenticalPeople identical;
	same.findSamePeople( firstName, lastName, FALSE );
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////// K I J E L Ö L T   E M B E R R E L   A Z O N O S   E M B E R E K   Ö S S Z E V O N Á S A ///////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CTablePeople::OnAzonosemberConc()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		theApp.message( L"Emberek listája", L"\nNincs kijelölve senki!" );
//		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	CString firstName;
	CString lastName;

	firstName	= m_ListCtrl.GetItemText( nItem, L_FIRST_NAME );
	lastName	= m_ListCtrl.GetItemText( nItem, L_LAST_NAME );

//	CIdenticalPeople identical;
	CSameP identical;
	identical.findSamePeople( firstName, lastName, TRUE );

	
}
*/
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
	
	dlg.m_firstName	= m_ListCtrl.GetItemText( nItem, L_FIRST_NAME );
	dlg.m_lastName	= m_ListCtrl.GetItemText( nItem, L_LAST_NAME );

	dlg.DoModal();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDD_TABLE_INDIVIDUAL sysem menüjének (X) kattintására Ez mûködik!!!
void CTablePeople::OnClose()  
{
	m_ListCtrl.DeleteAllItems();	// erre szükség van?
	m_recordset->Clear();			// erre szükség van?
	((CDragonDlg*)m_dlg )->m_pIndividuals = NULL;
//	theApp.m_pMainWnd->ShowWindow( SW_RESTORE );
	theApp.m_pMainWnd->SetForegroundWindow();
	CDialogEx::OnClose();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDD_FA_DIALOG abalak bezárására Úgy látszik ez nem mûködik!!!
void CTablePeople::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if(m_dlg )
	{		
		((CDragonDlg*)m_dlg )->m_pIndividuals = NULL;
	}
	delete this;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();

	// TODO: Add your message handler code here
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTablePeople::OnUpdateRecord( WPARAM wParam, LPARAM lParam)
{
	CString* rowid = (CString*)wParam;

	AfxMessageBox( *rowid );

	return TRUE;
}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::parent( CString rowid, PARENT* p)
{
	if( !rowid.IsEmpty() )
	{
		m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'",rowid );
		if( !query2( m_command ) ) return;

		p->rowid_table = m_recordset2->GetFieldString( PEOPLE_TABLENUMBER );
		p->name.Format( L"%s %s", m_recordset2->GetFieldString( PEOPLE_LAST_NAME ), m_recordset2->GetFieldString( PEOPLE_FIRST_NAME ) );
		p->name.Replace( '|', '\'' );
		p->birth_date	= m_recordset2->GetFieldString( PEOPLE_BIRTH_DATE );

	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnAscendants()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}

	CString name;
	CString rowid;

	rowid = m_ListCtrl.GetItemText( nItem, G_ROWID );

	int sex_id;
	if( theApp.m_inputMode == GAHTML )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) );
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, L_SEX ) );
	}
	else if( theApp.m_inputMode == GEDCOM )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, G_LAST_NAME ), m_ListCtrl.GetItemText( nItem, G_FIRST_NAME ) );
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, G_SEX ) );
	}
	else
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, N_LAST_NAME ), m_ListCtrl.GetItemText( nItem, N_FIRST_NAME ) );
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, N_SEX ) );
	}

	ShowWindow(SW_HIDE);
	CGaAscendants dlg;
	dlg.m_name	= name;
	dlg.m_rowid		= rowid;
	dlg.m_sex_id	= sex_id;
	dlg.DoModal();
	ShowWindow(SW_RESTORE);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnAscendantsChain()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}

	CString name;
	CString rowid;

	rowid = m_ListCtrl.GetItemText( nItem, G_ROWID );
//	theApp.v_rowids.push_back( rowid );

	int sex_id;
	if( theApp.m_inputMode == GAHTML )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) );
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, L_SEX ) );
	}
	else if( theApp.m_inputMode == GEDCOM )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, G_LAST_NAME ), m_ListCtrl.GetItemText( nItem, G_FIRST_NAME ) );
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, G_SEX ) );
	}
	else
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, N_LAST_NAME ), m_ListCtrl.GetItemText( nItem, N_FIRST_NAME ) );
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, N_SEX ) );
	}

	ShowWindow(SW_HIDE);
	CGaAscendantsChain dlg;
	dlg.m_rowid	= rowid;
	dlg.m_name	= name;
	dlg.DoModal();
	ShowWindow(SW_RESTORE);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGedcomOutput()
{
	if( theApp.m_inputMode == MANUAL )
		m_ListCtrl.SetSortOrder( 2, N_LAST_NAME+1, N_FIRST_NAME+1 );
	if( theApp.m_inputMode == GEDCOM )
		m_ListCtrl.SetSortOrder( 2, G_LAST_NAME+1, G_FIRST_NAME+1 );
	if( theApp.m_inputMode == GAHTML )
		m_ListCtrl.SetSortOrder( 2, L_LAST_NAME+1, L_FIRST_NAME+1 );

	CGedcomOut dlg;
	dlg.m_filter = m_filterText;
	dlg.p_ListCtrl = &m_ListCtrl;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CTablePeople::OnChangeSearch()
//{
//	CString	search;
//	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
//	theApp.search( search, m_orderix,  &m_ListCtrl );
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnClickedKeres()
{
	if( m_orderix == 0 )
	{
		AfxMessageBox( L"Rendezni kell az oszlopot, amelyben keresni akarsz!" );
		return;
	}
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	if( search.IsEmpty() )
	{
		AfxMessageBox( L"Meg kell adni a keresendõ stringet!");
		return;
	}

	int		n				= m_ListCtrl.GetItemCount();
	int		length			= search.GetLength();
	int		nItem;
	CString	str;

	theApp.unselectAll( &m_ListCtrl );
	for( nItem = 0; nItem < n; ++nItem )
	{
		str = m_ListCtrl.GetItemText( nItem, m_orderix);
		str = str.Left(length);						// az aktuális search string hosszával azonos hosszúság leválasztása
		if( str == search )
		{
			m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
			m_ListCtrl.EnsureVisible( nItem, FALSE );
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
			GetDlgItem( IDC_SEARCH )->GetWindowTextW( str );
			if( str.GetLength() ) 
				OnClickedKeres();
			break;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
		}
	}
    return CWnd::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTablePeople::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR;

	if( theApp.m_inputMode == GEDCOM )
		MENU_IDR = IDR_DROPDOWN_EDIT;
	else if( theApp.m_inputMode == GAHTML )
		MENU_IDR = IDR_DROPDOWN_HTML;
	else if( theApp.m_inputMode == MANUAL )
		MENU_IDR = IDR_DROPDOWN_PEOPLE;

	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);
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
		title.Format( L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijelölt ember a ga.html fájlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s %s szülei és testvérei", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlChildren()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s %s és gyermekei", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) );
	dlg.m_type	= L"F_CHILDREN";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );

	CEditPeople dlg;
	dlg.m_caption.Format( L"%s %s adatainak szerkesztése", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) );
	dlg.m_rowid = rowid;

	ShowWindow( SW_HIDE );
	dlg.DoModal();
	ShowWindow( SW_RESTORE );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::On3Generations()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );

	CRelations dlg;
	dlg.m_rowid = rowid;

	ShowWindow( SW_HIDE );
	dlg.DoModal();
	ShowWindow( SW_RESTORE );

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditUpdate()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED); 
	if( nItem == -1 ) return;

	CString name;
	CString first_name;
	CString last_name;

	first_name = m_ListCtrl.GetItemText( nItem, N_FIRST_NAME );
 	last_name = m_ListCtrl.GetItemText( nItem, N_LAST_NAME );
	name.Format( L"%s %s", last_name, first_name );

	
//	CNewPeople dlg;
//	dlg.m_rowid.Empty();

	CEditPeople dlg;

	dlg.m_rowid	= m_ListCtrl.GetItemText( nItem, N_ROWID );
	dlg.m_caption.Format( L"%s szerkesztése", name );

	ShowWindow( SW_HIDE );
	dlg.DoModal();	
//	if( dlg.DoModal() == IDCANCEL ) return;
	ShowWindow( SW_RESTORE );

	fillTable( nItem );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditDelete()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int sex_id;
	CString rowid = m_ListCtrl.GetItemText( nItem, G_ROWID );
	CString name;

	ShowWindow( SW_HIDE );
	if( theApp.m_inputMode == GEDCOM )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, G_LAST_NAME ), m_ListCtrl.GetItemText( nItem, G_FIRST_NAME ) ); 
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, G_SEX ) );
	}
	else if( theApp.m_inputMode == GAHTML )
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) ); 
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, L_SEX ) );
	}
	else
	{
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, N_LAST_NAME ), m_ListCtrl.GetItemText( nItem, N_FIRST_NAME ) ); 
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, N_SEX ) );
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
	
		m_command.Format(L"DELETE FROM marriages WHERE spouse1_id='%s' OR spouse2_id='%s'",rowid, rowid );
		if( !theApp.execute( m_command ) ) return;
	}
	ShowWindow( SW_RESTORE );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditInsert()
{
/*
	int	nItem;
	CString name;
	CString first_name;
	CString last_name;

	
	nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED); 
	if( nItem == -1 ) return;

	first_name = m_ListCtrl.GetItemText( nItem, N_FIRST_NAME );
 	last_name = m_ListCtrl.GetItemText( nItem, N_LAST_NAME );
	name.Format( L"%s %s", last_name, first_name );


	CNewPeople dlg;
	dlg.m_rowid.Empty();



	ShowWindow( SW_HIDE );
	if( dlg.DoModal() == IDCANCEL ) return;
	ShowWindow( SW_RESTORE );
	
	fillTable( nItem );
*/

	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );

	CRelations dlg;
	dlg.m_rowid = rowid;
	
	ShowWindow( SW_HIDE );
	dlg.DoModal();
	ShowWindow( SW_RESTORE );

}
