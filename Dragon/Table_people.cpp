// TableIndividuals.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GEDCOM table

#include "stdafx.h"
#include "Fa.h"
#include "FaDlg.h"
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
#include "html_Edit.h"
#include "html_Lines.h"
#include "GA_ascendants.h"
#include "GetLastFirst.h"
#include "EditPeople.h"
#include "GA_descendants.h"
#include "GA_ascendants.h"
#include "GA_ascendantsChain.h"
#include "SamePeople.h"
#include "pictures.h"
#include "Relations.h"
#include "GedcomOut.h"
#include "NewPeople.h"
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
	H_ROWID = 0,
	H_ROWID_FILE,
	H_FAMILYNUMBER,
	H_TABLENUMBER,
	H_LINENUMBER,
	H_SOURCE,
	H_SPOUSE,
	H_SPOUSEPARENT,
	H_SPOUSESPOUSE,
	H_UNITED,
	H_TABLENUMBERROMAN,
	H_GENERATION,
	H_SEX,
	H_TITLE,
	H_TITOLO,
	H_LAST_NAME,
	H_KNOWN_AS,
	H_FIRST_NAME,
	H_BIRTH_PLACE,
	H_BIRTH_DATE,
	H_DEATH_PLACE,
	H_DEATH_DATE,
	H_AGE,
	H_ORDERFATHER,
	H_ORDERMOTHER,
	H_COMMENT,
	H_OCCUPATION,
	H_ROWID_FATHER,
//	H_TABLE_ID_FATHER,
	H_FATHER,
	H_BIRTH_DATE_FATHER,
	H_ROWID_MOTHER,
//	H_TABLE_ID_MOTHER,
	H_MOTHER,
	H_BIRTH_DATE_MOTHER,
	H_FOLYT,
	H_ROW,
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
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTablePeople, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_FILTER_UNFILTER, &CTablePeople::OnFilterUnfilter)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTablePeople::OnDblclkList)
	ON_COMMAND(ID_EDIT_DELETE, &CTablePeople::OnEditDelete)
	ON_COMMAND(ID_EDIT_INSERT, &CTablePeople::OnEditInsert)
	ON_COMMAND(ID_EDIT_UPDATE, &CTablePeople::OnEditUpdate)
	ON_COMMAND(ID_RELATIONS, &CTablePeople::OnRelations)
	ON_EN_CHANGE(IDC_SEARCH, &CTablePeople::OnChangeSearch)
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
ON_COMMAND(ID_AZONOSEMBER, &CTablePeople::OnAzonosember)
ON_WM_CLOSE()
ON_COMMAND(ID_AZONOSEMBER_CONC, &CTablePeople::OnAzonosemberConc)
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
ON_COMMAND(ID_HTML_PARENTS, &CTablePeople::OnHtmlParents)
ON_COMMAND(ID_HTML_LINES, &CTablePeople::OnHtmlLines)
ON_COMMAND(ID_EDIT_NOTEPAD, &CTablePeople::OnEditNotepad)
ON_COMMAND(ID_GIVENNAME, &CTablePeople::OnGivenname)
ON_COMMAND(ID_ASCENDANTS, &CTablePeople::OnAscendants)
ON_COMMAND(ID_INPUT_MANUAL, &CTablePeople::OnInputManual)
ON_WM_NCDESTROY()
ON_MESSAGE(WM_EDIT_INDIVIDUAL, OnUpdateRecord )
ON_COMMAND(ID_DESCENDANTS_INDIVIDUAL, &CTablePeople::OnIndividualDescendants)
ON_COMMAND(ID_ASCENDANTS_CHAIN, &CTablePeople::OnAscendantsChain)
ON_COMMAND(ID_GEDCOM_OUTPUT, &CTablePeople::OnGedcomOutput)
ON_COMMAND(ID_PRIVAT_DESCENDANTS, &CTablePeople::OnPrivatDescendants)
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


	ChangeMenu();

	if( !theApp.m_cntPeople )
		enableMenu( MF_GRAYED );
	m_orderix = 0;
	createListColumns();
	
//	if( !theApp.m_cntPeople )
//	{
//		OnEditInsert();
//	}
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
		m_filterTextNew = L"- õsök - nincsenek szülei, de van gyereke";
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
	m_filterTextNew	= L"Leszármazottak házastársainak apja";
	m_filterNew		= L"source=3"; 
	fillTable(0);
}
void CTablePeople::OnFilterMothers()
{
	m_filterTextNew	= L"Leszármazottak házastársainak anyja";
	m_filterNew		= L"source=4"; 
	fillTable(0);
}
void CTablePeople::OnFilterSpousespouses()
{
	m_filterTextNew	= L"Leszármazottak házastársainak további házastársai";
	m_filterNew		= L"source=5"; 
	fillTable(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// G R O U P   B Y  ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyTitolo()
{
	m_filterNew.Format(L"GROUP BY titolo");
	m_filterTextNew.Format(L"Különbözõ elõnevek" );
	m_orderix = H_TITOLO;
	fillTable(0);
}
void CTablePeople::OnGroupbyBirthplace()
{
	m_filterNew.Format(L"GROUP BY birth_place");
	m_filterTextNew.Format(L"Különbözõ születési helyek" );
	m_orderix = H_BIRTH_PLACE;
	fillTable(0);

}
void CTablePeople::OnGroupbyDeathplace()
{
	m_filterNew.Format(L"GROUP BY death_place");
	m_filterTextNew.Format(L"Különbözõ halálozási helyek" );
	m_orderix = H_DEATH_PLACE;
	fillTable(0);
}
void CTablePeople::OnGroupbyFirstname()
{
	m_filterNew.Format(L"GROUP BY first_name");
	m_filterTextNew.Format(L"Különbözõ keresztnevek" );
	m_orderix = H_FIRST_NAME;
	fillTable(0);
}
void CTablePeople::OnGroupbyLastname()
{
	m_filterNew.Format(L"GROUP BY last_name");
	m_filterTextNew.Format(L"Különbözõ családnevek" );
	m_orderix = H_LAST_NAME;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyBirthDate()
{
	m_filterNew.Format(L"GROUP BY birth_date");
	m_filterTextNew.Format(L"Különbözõ születési idõk" );
	m_orderix = H_BIRTH_DATE;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyDeathdate()
{
	m_filterNew.Format(L"GROUP BY death_date");
	m_filterTextNew.Format(L"Különbözõ halálozási idõk" );
	m_orderix = H_DEATH_DATE;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyComment()
{
	m_filterNew.Format(L"GROUP BY comment");
	m_filterTextNew.Format(L"Különbözõ megjegyzések" );
	m_orderix = H_COMMENT;
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
	m_ListCtrl.InsertColumn( G_COMMENT,				L"megjegyzés",		LVCFMT_LEFT,	100,-1,COL_TEXT);
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

		m_ListCtrl.InsertColumn( H_ROWID,				L"rowid",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_ROWID_FILE,			L"fájl#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_FAMILYNUMBER,		L"család#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_TABLENUMBER,			L"tábla#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_LINENUMBER,			L"line#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_SOURCE,				L"src",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_SPOUSE,				L"sp",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_SPOUSEPARENT,		L"spp",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_SPOUSESPOUSE,		L"ssp",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_UNITED,				L"egyesített",		LVCFMT_RIGHT,	 40,-1,COL_NUM);

		m_ListCtrl.InsertColumn( H_TABLENUMBERROMAN,	L"tábla",			LVCFMT_RIGHT,	 40,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_GENERATION,			L"G",				LVCFMT_LEFT,	 25,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_SEX,					L"nem",				LVCFMT_LEFT,	 40,-1,COL_HIDDEN);
		m_ListCtrl.InsertColumn( H_TITLE,				L"tit",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_TITOLO,				L"elõnév",			LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_LAST_NAME,			L"családnév",		LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_KNOWN_AS,			L"más",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_FIRST_NAME,			L"utónév",			LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_BIRTH_PLACE,			L"születés",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_BIRTH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_DEATH_PLACE,			L"elhalálozás",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_DEATH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_AGE,					L"kor",				LVCFMT_LEFT,	 30,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_ORDERFATHER,		L"apjacnt",			LVCFMT_LEFT,	30,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_ORDERMOTHER,		L"anyjacnt",		LVCFMT_LEFT,	30,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_COMMENT,				L"megjegyzés",		LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_OCCUPATION,			L"foglalkozás",		LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_ROWID_FATHER,		L"apa_rowid",		LVCFMT_RIGHT,	50,-1,COL_NUM);
		m_ListCtrl.InsertColumn( H_FATHER,				L"apja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_BIRTH_DATE_FATHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_ROWID_MOTHER,		L"anya_rowid",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_MOTHER,				L"anyja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_BIRTH_DATE_MOTHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_FOLYT,				L"elágazás",		LVCFMT_LEFT,	50,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( H_ROW,					L"record",			LVCFMT_RIGHT,	50,-1,COL_NUM);
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
		m_ListCtrl.InsertColumn( N_COMMENT,				L"megjegyzés",		LVCFMT_LEFT,	100,-1,COL_TEXT);
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

	m_ListCtrl.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
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

	int mother_index = _wtoi( m_recordset->GetFieldString( PEOPLE_MOTHER_INDEX ) );
	if( mother_index )
	{
		m_index.Format( L"%d", mother_index );
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTablePeople::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR;

	if( theApp.m_inputMode == GEDCOM )
		MENU_IDR = IDR_DROPDOWN_DELETE_RELATIONS;
	else if( theApp.m_inputMode == GAHTML )
		MENU_IDR = IDR_DROPDOWN_TABLE;
	else if( theApp.m_inputMode == MANUAL )
		MENU_IDR = IDR_DROPDOWN_DELETE_RELATIONS;

	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_RELATIONS,MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
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

	if( theApp.m_inputMode == GAHTML )
	{
		CEditPeople dlg;
		dlg.m_caption = str;
		dlg.m_rowid = m_ListCtrl.GetItemText( nItem, N_ROWID );
		dlg.DoModal();
	}
	else
		OnRelations();

	SetForegroundWindow();

//	updateRow( nItem );
	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnInputManual()
{
	ShowWindow( SW_HIDE );

	CNewPeople dlg;

	dlg.m_newPeople = -1;
	if( dlg.DoModal() == IDCANCEL ) return;

	ShowWindow( SW_RESTORE );

	m_command.Format( L"SELECT rowid, * FROM people WHERE rowid ='%s'", dlg.m_rowidClick );
	if( !query( m_command ) ) return;

	int nItem = m_ListCtrl.GetItemCount();

	

	fillRow( nItem-1 );

	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.AttachDataset( &v_individuals );
	m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible( nItem, FALSE );

/*
	CString table(L"");

	m_ListCtrl.SetSortOrder( 1, 1 );
	while( TRUE )
	{
		ShowWindow( SW_HIDE );
		CEditPeople dlg;
		dlg.m_caption = L"Új ember bevitele";
		dlg.m_rowid.Empty();
		dlg.m_table	= table;
		if( dlg.DoModal() != IDCANCEL )
		{
			m_rowid = dlg.m_m;  // ez érthetetlen! dlg.m_rowid rossz!!!
			table = dlg.m_table;
//			insertRow();
		}
		else
		{
			ShowWindow( SW_RESTORE );
			break;
		}
		ShowWindow( SW_RESTORE );
	}
*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditInsert()
{
	ShowWindow( SW_HIDE );
	CEditPeople dlg;
	dlg.m_caption = L"Új ember bevitele";
	dlg.m_rowid.Empty();
	if( dlg.DoModal() != IDCANCEL )
	{
//		m_rowid = dlg.m_m;  // ez érthetetlen! dlg.m_rowid rossz!!!
//		insertRow();
	}
	ShowWindow( SW_RESTORE );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditUpdate()
{
	int	nItem;
	CString name;
	CString first_name;
	CString last_name;

	CNewPeople dlg;

	dlg.m_rowidMain.Empty();
	nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED); 
	if( nItem == -1 ) return;

	first_name = m_ListCtrl.GetItemText( nItem, N_FIRST_NAME );
	last_name = m_ListCtrl.GetItemText( nItem, N_LAST_NAME );
	name.Format( L"%s %s", last_name, first_name );

	dlg.m_rowidClick	= m_ListCtrl.GetItemText( nItem, N_ROWID );
	str.Format( L"%s szerkesztése", name );

	dlg.m_newPeople = -1;

	ShowWindow( SW_HIDE );
	if( dlg.DoModal() == IDCANCEL ) return;
	ShowWindow( SW_RESTORE );


	
	fillTable( nItem );
//	m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
//	m_ListCtrl.EnsureVisible( nItem, FALSE );
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
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, H_LAST_NAME ), m_ListCtrl.GetItemText( nItem, H_FIRST_NAME ) ); 
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, H_SEX ) );
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnRelations()
{

	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	m_rowid = m_ListCtrl.GetItemText( nItem, G_ROWID );
	m_insert = 0;
	ShowWindow( SW_HIDE );
	CRelations dlg;
	dlg.nItem = nItem;
	dlg.m_rowid = m_rowid;
	if( dlg.DoModal() != IDCANCEL )
	{
		fillTable( nItem );
		updateRow( nItem );
	}
	ShowWindow( SW_RESTORE );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	
	int mother_index = _wtoi( m_recordset->GetFieldString( PEOPLE_MOTHER_INDEX ) );
	if( mother_index )
	{
		indexM.Format( L"%d", mother_index );
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
		updateField( nItem, H_SEX, m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
		updateField( nItem, H_TITLE, m_recordset->GetFieldString( PEOPLE_TITLE ) );
		updateField( nItem, H_TITOLO, m_recordset->GetFieldString( PEOPLE_TITOLO ) );
		updateField( nItem, H_LAST_NAME, m_recordset->GetFieldString( PEOPLE_LAST_NAME ) );
		updateField( nItem, H_FIRST_NAME, m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		updateField( nItem, H_BIRTH_PLACE, m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		updateField( nItem, H_BIRTH_DATE, m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		updateField( nItem, H_DEATH_PLACE, m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		updateField( nItem, H_DEATH_DATE, m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		updateField( nItem, H_AGE, kor );
		updateField( nItem, H_ORDERFATHER, m_recordset->GetFieldString( PEOPLE_ORDERFATHER ) );
		updateField( nItem, H_ORDERMOTHER, m_recordset->GetFieldString( PEOPLE_ORDERMOTHER ) );
		updateField( nItem, H_COMMENT, m_recordset->GetFieldString( PEOPLE_COMMENT ) );
		updateField( nItem, H_OCCUPATION, m_recordset->GetFieldString( PEOPLE_OCCUPATION ) );
		updateField( nItem, H_ROWID_FATHER, rowid_father );
		updateField( nItem, H_FATHER, father );
		updateField( nItem, H_BIRTH_DATE_FATHER, m_recordset2->GetFieldString( 3) );
		updateField( nItem, H_ROWID_MOTHER, rowid_mother );
		updateField( nItem, H_MOTHER, mother );
		updateField( nItem, H_BIRTH_DATE_MOTHER, m_recordset2->GetFieldString( 3) );
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnChangeSearch()
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.search( search, m_orderix,  &m_ListCtrl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
//		case VK_DELETE:
//			OnEditDelete();
//			return TRUE;
		case VK_RETURN:
//			OnEditUpdate();
			return TRUE;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
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

	dlg.m_syntax	= 0;
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
		dlg.m_name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, H_LAST_NAME ), m_ListCtrl.GetItemText( nItem, H_FIRST_NAME ) );
		dlg.m_tableNumber = m_ListCtrl.GetItemText( nItem, H_TABLENUMBER );
		dlg.m_source	= _wtoi( m_ListCtrl.GetItemText( nItem, H_SOURCE ) );
	}
	else
	{
		dlg.m_name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, N_LAST_NAME ), m_ListCtrl.GetItemText( nItem, N_FIRST_NAME ) );
		dlg.m_tableNumber = m_ListCtrl.GetItemText( nItem, N_TABLENUMBER );
	}

	if( dlg.DoModal() == IDCANCEL ) return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnPrivatDescendants()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		theApp.message( L"Emberek listája", L"\nNincs kijelölve senki!" );
		return;
	}

	CGaDescendants dlg;

	dlg.m_syntax	= 1;
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
		dlg.m_name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, H_LAST_NAME ), m_ListCtrl.GetItemText( nItem, H_FIRST_NAME ) );
		dlg.m_tableNumber = m_ListCtrl.GetItemText( nItem, H_TABLENUMBER );
		dlg.m_source	= _wtoi( m_ListCtrl.GetItemText( nItem, H_SOURCE ) );
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
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	H_LINENUMBER ) );

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

	firstName	= m_ListCtrl.GetItemText( nItem, H_FIRST_NAME );
	lastName	= m_ListCtrl.GetItemText( nItem, H_LAST_NAME );
	CSamePeople same;
//	CIdenticalPeople identical;
	same.findSamePeople( firstName, lastName, FALSE );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////// K I J E L Ö L T   E M B E R R E L   A Z O N O S   E M B E R E K   Ö S S Z E V O N Á S A ///////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	firstName	= m_ListCtrl.GetItemText( nItem, H_FIRST_NAME );
	lastName	= m_ListCtrl.GetItemText( nItem, H_LAST_NAME );

//	CIdenticalPeople identical;
	CSamePeople identical;
	identical.findSamePeople( firstName, lastName, TRUE );

	
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
	((CFaDlg*)m_dlg )->m_pIndividuals = NULL;
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
		((CFaDlg*)m_dlg )->m_pIndividuals = NULL;
	}
	delete this;
}




//void CTablePeople::OnWindowPosChanged(WINDOWPOS* lpwndpos)
//{
//	CDialogEx::OnWindowPosChanged(lpwndpos);
//
//	m_pParentWnd->ShowWindow( SW_RESTORE );
//}

/*
void CTablePeople::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
//	SetForegroundWindow();
}
*/
/*
void CTablePeople::listHtmlLine( int lineNumber )
{
	CHtmlEdit dlg;

	ULONGLONG pos1;
	ULONGLONG pos2;

	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
	CString cLine;
	for( int i = 0; i < lineNumber-1; ++i ) file.ReadString( cLine );
	pos1 = file.GetPosition();
	file.ReadString( cLine );
	pos2 = file.GetPosition();
	file.Close();
	CString front;

	front = cLine.Left( cLine.Find( ';' ) + 1 );
	cLine = cLine.Mid( cLine.Find( ';' ) + 1 );
		
	dlg.m_line = cLine;
	dlg.m_lineNumber = lineNumber;
	if( dlg.DoModal() == IDCANCEL ) return;

	CString line;
	line.Format( L"%s%s\r\n", front, dlg.m_line );

	USES_CONVERSION;
	char *pl = W2A((LPCTSTR)line);

	// create new html file with the modified line
	CString newFileSpec;
	newFileSpec.Format( L"%s\\%s_original.htm", theApp.m_htmlPath, theApp.m_htmlFileName );

	CString outputFileSpec;
	outputFileSpec.Format( L"%s\\modifiedFile.html", theApp.m_htmlPath );
	DeleteFile( outputFileSpec );

	CFile input( theApp.m_htmlFileSpec, CFile::modeRead | CFile::typeBinary );
	CFile output( outputFileSpec, CFile::modeCreate | CFile::modeWrite  | CFile::typeBinary );

	ULONGLONG inputLength = input.GetLength();
	int		length = 1000000;
	TCHAR *buffer = new TCHAR[ length ];
	
	int numOfCycles;
	int	remains;

// I. rész másolása
	numOfCycles = int( pos1 / length );		// olvasás-írás ciklusok száma length hosszzal
	remains = int( pos1 % length );			// utolsó olvasás-írás	remains hosszal

	input.SeekToBegin();
	for( int i = 0; i < numOfCycles; ++i )
	{
		input.Read( buffer, length );
		output.Write( buffer, length );
	}
	if( remains )
	{
		input.Read( buffer, remains );
		output.Write( buffer, remains );
	}
// új sor beírása
	output.Write( pl, strlen( pl) );

// II. rész másolása
	ULONGLONG left = input.GetLength() - pos2;


	numOfCycles = int( left / length );		// olvasás-írás ciklusok száma length hosszzal
	remains = int( left % length );			// utolsó olvasás-írás	remains hosszal

	input.Seek( pos2, SEEK_SET );
	for( int i = 0; i < numOfCycles; ++i )
	{
		input.Read( buffer, length );
		output.Write( buffer, length );
	}
	if( remains )
	{
		input.Read( buffer, remains );
		output.Write( buffer, remains );
	}
	input.Close();
	output.Close();

		
	DeleteFile( newFileSpec );
	input.Rename( theApp.m_htmlFileSpec, newFileSpec );
	output.Rename( outputFileSpec, theApp.m_htmlFileSpec );

	theApp.message( L"Emberek listája", L"A html fájl módosítása megtörtént!" );
//	AfxMessageBox( L"A html fájl módisítása megtörtént!" );
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlParents()
{
//	CProgressWnd wndProgress(NULL, L"Gyerek és szülõk html sorainak keresése..." ); 
//	wndProgress.GoModal();

	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumberF;
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	H_LINENUMBER );
	CString father_id	= m_ListCtrl.GetItemText( nItem, 	H_ROWID_FATHER );
	CString parents;


	parents.Format( L"%s - %s", m_ListCtrl.GetItemText( nItem, 	H_FATHER ), m_ListCtrl.GetItemText( nItem, 	H_MOTHER ) );

	m_command.Format( L"SELECT lineNumber FROM people WHERE rowid = '%s'", father_id );
	if( !query( m_command ) ) return;
	lineNumberF = m_recordset->GetFieldString( 0 );
	if( lineNumberF.IsEmpty() )
	{
		AfxMessageBox( L"A kijelölt embernek nem ismerjük az apját!" );
		return;
	}

	std::vector<CString> vLines;

	vLines.push_back( lineNumberF );
	vLines.push_back( lineNumber );

	ShowWindow( SW_HIDE );
	CHtmlLines dlg;
	dlg._what = 2;
	dlg.parents = parents;
	dlg.child.Format( L"%s %s", m_ListCtrl.GetItemText( nItem,H_LAST_NAME ), m_ListCtrl.GetItemText( nItem,H_FIRST_NAME ) ); 
	dlg.vLines	= &vLines;

//	wndProgress.DestroyWindow();

	dlg.DoModal();
	ShowWindow( SW_RESTORE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlLines()
{
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int			nItem;
	std::vector<CString> vLines;

	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		vLines.push_back( m_ListCtrl.GetItemText( nItem, H_LINENUMBER ) );
	}

	ShowWindow( SW_HIDE );
	CHtmlLines dlg;

	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
	ShowWindow( SW_RESTORE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	H_LINENUMBER );

//	ShowWindow( SW_HIDE );
	theApp.editNotepad( lineNumber );
//	ShowWindow( SW_RESTORE );
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

		p->m_rowid_table = m_recordset2->GetFieldString( PEOPLE_TABLENUMBER );
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
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, H_LAST_NAME ), m_ListCtrl.GetItemText( nItem, H_FIRST_NAME ) );
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, H_SEX ) );
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
		name.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, H_LAST_NAME ), m_ListCtrl.GetItemText( nItem, H_FIRST_NAME ) );
		sex_id = _wtoi( m_ListCtrl.GetItemText( nItem, H_SEX ) );
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
		m_ListCtrl.SetSortOrder( 2, H_LAST_NAME+1, H_FIRST_NAME+1 );

	CGedcomOut dlg;
	dlg.m_filter = m_filterText;
	dlg.p_ListCtrl = &m_ListCtrl;
	dlg.DoModal();
}
