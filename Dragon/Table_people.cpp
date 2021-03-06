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
#include "Relatives.h"
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
#include "Relatives.h"
#include "GedcomOut.h"
#include "Message.h"
#include "ProgressWnd.h"
#include "EditPeople.h"
#include "Relatives.h"

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
	G_COMMENT,
};
// GAHTML
enum
{
	L_ROWID = 0,
	L_ROWID_FILE,
	L_FAMILYNUMBER,
	L_TABLENUMBER,
	L_LINENUMBER,
	L_TABLENUMBERROMAN,

	L_UNITED,
	L_SPOUSE,
	L_SPOUSEPARENT,
	L_SPOUSESPOUSE,
	L_SOURCE,
	
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
	L_COMMENT,
	L_AG,
	L_CSALAD,
	L_FOLYT,
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
	N_COMMENT,
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
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTablePeople, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	
	ON_COMMAND(ID_FILTER_UNFILTER, &CTablePeople::OnFilterUnfilter)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTablePeople::OnDblclkList)


	ON_MESSAGE(WM_SYNCRONIZE_PEOPLE, OnSyncronize )
// DROPDOWN men� funkci�k



	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_DB_EDIT, &CTablePeople::OnEditUpdate )
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CTablePeople::OnHtmlNotepadParents )
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CTablePeople::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_HTML_EDIT, &CTablePeople::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CTablePeople::OnHtmlNotepad)	
	ON_COMMAND(ID_HTML_CHILDREN, &CTablePeople::OnHtmlChildren)
	ON_COMMAND(ID_3GENERATIONS, &CTablePeople::On3Generations )

	ON_COMMAND(ID_EDIT_DELETE, &CTablePeople::OnEditDelete)
	ON_COMMAND(ID_EDIT_UPDATE, &CTablePeople::OnEditUpdate )
	ON_COMMAND(ID_EDIT_INSERT, &CTablePeople::OnEditInsert)


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
ON_COMMAND(ID_ASCENDANTS, &CTablePeople::OnAscendants)
ON_COMMAND(ID_INPUT_MANUAL, &CTablePeople::OnInputManual)
ON_WM_NCDESTROY()
ON_COMMAND(ID_DESCENDANTS_INDIVIDUAL, &CTablePeople::OnIndividualDescendants)
ON_COMMAND(ID_ASCENDANTS_CHAIN, &CTablePeople::OnAscendantsChain)
ON_COMMAND(ID_GEDCOM_OUTPUT, &CTablePeople::OnGedcomOutput)
ON_COMMAND(ID_PRIVATE_DESCENDANTS, &CTablePeople::OnPrivateDescendants)
ON_COMMAND(ID_FILTER_BISEX, &CTablePeople::OnFilterBisex)
ON_STN_CLICKED(IDC_STATIC_KERESS, &CTablePeople::OnClickedKeress)

ON_COMMAND(ID_INFO, &CTablePeople::OnInfo)
ON_STN_CLICKED(IDC_NEXT, &CTablePeople::OnClickedNext)
ON_COMMAND(ID_SET_ANCESTOR, &CTablePeople::OnSetAncestor)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
//	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();

	CString caption;
	caption.Format( L"Emberek" );
	SetWindowTextW( caption );

	colorKeres.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );

	ChangeMenu();

	if( !theApp.m_cntPeople )
		enableMenu( MF_GRAYED );
	m_orderix = 0;
	createListColumns();
	
	if( !theApp.m_cntPeople )
		return FALSE;


//	m_command = L"UPDATE people SET source = 1";
//	if( !theApp.execute( m_command ) ) return false;



	menu.EnableMenuItem( 1,MF_BYPOSITION| MF_GRAYED );
	menu.EnableMenuItem( 2,MF_BYPOSITION| MF_GRAYED );
	menu.EnableMenuItem( 3,MF_BYPOSITION| MF_GRAYED );
	menu.EnableMenuItem( 4,MF_BYPOSITION| MF_GRAYED );

	if( theApp.m_cntPeople && theApp.m_cntPeople  < 10000 )
		OnFilterUnfilter();	

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);   // ez volt!!

	ShowWindow( SW_MAXIMIZE );
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
	m_filterTextNew = L"Az adatb�zisban l�v� emberek";
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
	dlg.m_caption = L"V�laszd ki a csal�dot!";
	dlg.m_select  = 1;


	if(dlg.DoModal() == IDCANCEL ) return;

	m_filterTextNew.Format(L"A %s csal�dban l�v� emberek", dlg.m_selected);
	m_filterNew.Format(  L"familyNumber='%s'", dlg.m_id );
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterTable()
{
	CTableTables dlg;

	dlg.m_filter.Empty();
	dlg.m_select_id = L"tableNumber";
	dlg.m_caption = L"V�laszd ki a t�bl�t!";
	dlg.m_select  = 1;


	if(dlg.DoModal() == IDCANCEL ) return;

	m_filterNew.Format(L"tableNumber='%s'", dlg.m_id );
	m_filterTextNew.Format(L"A %s t�bl�zatban l�v� emberek", dlg.m_selected);

	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterAncestors()
{
	if( theApp.m_inputMode != GAHTML )
	{
		m_filterTextNew = L"- �s�k - f�rfiak, akiknek nincsenek sz�lei, de van gyereke";
		m_filterNew = L"sex_id = 1 AND (typeof(father_id) = 'null' OR father_id == 0 OR father_id = '' )\
AND ( rowid IN (SELECT father_id FROM people ) OR rowid IN (SELECT mother_id FROM people) )";
	}

	else
	{
		m_filterNew.Format( L"tableAncestry == 1" );
//		m_filterNew.Format(L"generation=='A'" );
		m_filterTextNew.Format(L"Az adatb�zisban l�v� �s�k");
	}
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterFile()
{
	CTableFiles dlg;
	if( dlg.DoModal() == IDCANCEL ) return;

	m_filterNew.Format(L"fileNumber='%s'", dlg.m_selected_id );
	m_filterTextNew.Format(L"A %s. f�jlban l�v� emberek", dlg.m_selected);
	fillTable(0);	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterIndividual()
{
	CGetString dlg;

	int rowid;
	rowid = theApp.GetProfileInt(	L"Settings", L"rowid", 0 );
	dlg.m_string.Format( L"%d", rowid ); 
	dlg.m_caption.Format( L"Add meg a k�v�nt ember rowid-j�t!" );
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

	dlg.m_caption	= L"V�lasszd ki a csal�dnevet!"; 
	dlg.m_table		= L"people"; 
	dlg.m_column	= L"last_name"; 
	dlg.m_filter	= L"WHERE last_name LIKE '%' GROUP BY last_name";

	if(dlg.DoModal() == IDCANCEL ) return;

	m_filterNew.Format(L"last_name='%s'",dlg.m_selected );
	m_filterTextNew.Format(L"%s csal�dnev� emberek", dlg.m_selected);
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterFirstname()
{
	CSelectOne dlg;

	dlg.m_caption	= L"V�lasszd ki az ut�nevet!"; 
	dlg.m_table		= L"people"; 
	dlg.m_column	= L"first_name"; 
	dlg.m_filter	= L"WHERE first_name LIKE '%' GROUP BY first_name";

	if(dlg.DoModal() == IDCANCEL ) return;

	m_filterNew.Format(L"first_name='%s'",dlg.m_selected );
	m_filterTextNew.Format(L"%s ut�nev� emberek", dlg.m_selected);
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGivenname()
{
	CGetLastFirst dlg;
	if( dlg.DoModal() == IDCANCEL ) return;

	m_filterTextNew.Format( L"%s nev� emberek az adatb�zisban", dlg._fullname );
	if( dlg._firstname.IsEmpty() )
	{
		m_filterTextNew.Format( L"'%s'-el kezd�d� csal�dnev� emberek az adatb�zisban", dlg._fullname );
		m_filterNew.Format( L"last_name LIKE'%s%c'", dlg._lastname, '%' );
	}
	else
	{
		m_filterTextNew.Format( L"%s nev� emberek az adatb�zisban", dlg._fullname );
		m_filterNew.Format( L"first_name='%s' AND last_name='%s'", dlg._firstname, dlg._lastname );
	}
	fillTable( 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterNodescendant()
{
	m_filterTextNew = L"Emberek, akiknek nincsenek lesz�rmazottai";
	m_filterNew = L"rowid NOT IN (SELECT father_id FROM people WHERE father_id != '0') AND rowid NOT IN (SELECT mother_id FROM people WHERE mother_id!='0') AND rowid != '0'";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterNoparents()
{
	m_filterTextNew = L"Emberek, akiknek nincsenek sz�lei";
	m_filterNew = L"(father_id=0 OR typeof( father_id ) = 'null' ) AND ( mother_id=0 OR typeof( mother_id ) = 'null' ) ";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterNospouse()
{
	m_filterTextNew = L"Emberek , akiknek nincs h�zast�rsa";
	m_filterNew = L"rowid NOT IN (SELECT spouse1_id FROM marriages) AND rowid NOT IN (SELECT spouse2_id FROM marriages )";
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterMen()
{
	m_filterTextNew.Format(L"Az adatb�zisban l�v� f�rfiak");
	m_filterNew.Format(  L"sex_id=1" );
	fillTable(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterWomen()
{
	m_filterTextNew.Format(L"Az adatb�zisban l�v� n�k");
	m_filterNew.Format(  L"sex_id=2" );
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterBisex()
{
	m_filterTextNew.Format(L"Az adatb�zisban l�v� bisex emberek");
	m_filterNew.Format(  L"sex_id=0" );
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterFolyt()
{
	m_filterTextNew.Format(L"El�gaz�sok");
	m_filterNew.Format(  L"folyt != ''" );
	fillTable(0);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnFilterDescendants()
{
	m_filterTextNew	= L"Lesz�rmazottak";
	m_filterNew		= L"source=1"; 
	fillTable(0);
}
void CTablePeople::OnFilterSpouses()
{
	m_filterTextNew	= L"Lesz�rmazottak h�zast�rsai";
	m_filterNew		= L"source=2"; 
	fillTable(0);
}
void CTablePeople::OnFilterFathers()
{
	m_filterTextNew	= L"Lesz�rmazottak h�zast�rsainak apjai";
	m_filterNew		= L"source=3 AND sex_id=1"; 
	fillTable(0);
}

void CTablePeople::OnFilterMothers()
{
	m_filterTextNew	= L"Lesz�rmazottak h�zast�rsainak anyjai";
	m_filterNew		= L"source=3 AND sex_id==2"; 
	fillTable(0);
}

void CTablePeople::OnFilterSpousespouses()
{
	m_filterTextNew	= L"Lesz�rmazottak h�zast�rsainak tov�bbi h�zast�rsai";
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
	m_filterTextNew.Format(L"K�l�nb�z� el�nevek" );
	m_orderix = L_TITOLO;
	fillTable(0);
}
void CTablePeople::OnGroupbyBirthplace()
{
	m_filterNew.Format(L"GROUP BY birth_place");
	m_filterTextNew.Format(L"K�l�nb�z� sz�let�si helyek" );
	m_orderix = L_BIRTH_PLACE;
	fillTable(0);

}
void CTablePeople::OnGroupbyDeathplace()
{
	m_filterNew.Format(L"GROUP BY death_place");
	m_filterTextNew.Format(L"K�l�nb�z� hal�loz�si helyek" );
	m_orderix = L_DEATH_PLACE;
	fillTable(0);
}
void CTablePeople::OnGroupbyFirstname()
{
	m_filterNew.Format(L"GROUP BY first_name");
	m_filterTextNew.Format(L"K�l�nb�z� keresztnevek" );
	m_orderix = L_FIRST_NAME;
	fillTable(0);
}
void CTablePeople::OnGroupbyLastname()
{
	m_filterNew.Format(L"GROUP BY last_name");
	m_filterTextNew.Format(L"K�l�nb�z� csal�dnevek" );
	m_orderix = L_LAST_NAME;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyBirthDate()
{
	m_filterNew.Format(L"GROUP BY birth_date");
	m_filterTextNew.Format(L"K�l�nb�z� sz�let�si id�k" );
	m_orderix = L_BIRTH_DATE;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyDeathdate()
{
	m_filterNew.Format(L"GROUP BY death_date");
	m_filterTextNew.Format(L"K�l�nb�z� hal�loz�si id�k" );
	m_orderix = L_DEATH_DATE;
	fillTable(0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGroupbyComment()
{
	m_filterNew.Format(L"GROUP BY comment");
	m_filterTextNew.Format(L"K�l�nb�z� megjegyz�sek" );
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
	m_ListCtrl.InsertColumn( G_ROWID_FILE,			L"f�jl#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( G_FAMILYNUMBER,		L"family#",			LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( G_TABLENUMBER,			L"t�bla#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( G_SEX,					L"nem",				LVCFMT_LEFT,	 40,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( G_TITLE,				L"tit",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_TITOLO,				L"el�n�v",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_LAST_NAME,			L"csal�dn�v",		LVCFMT_LEFT,	110,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_FIRST_NAME,			L"ut�n�v",			LVCFMT_LEFT,	110,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_BIRTH_PLACE,			L"sz�let�s",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_BIRTH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_DEATH_PLACE,			L"elhal�loz�s",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_DEATH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( G_COMMENT,				L"le�r�s",			LVCFMT_LEFT,	600,-1,COL_TEXT);
	}
	else if( theApp.m_inputMode == GAHTML )
	{

		m_ListCtrl.InsertColumn( L_ROWID,				L"rowid",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_ROWID_FILE,			L"f�jl#",			LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
		m_ListCtrl.InsertColumn( L_FAMILYNUMBER,		L"csal�d#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_TABLENUMBER,			L"t�bla#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_LINENUMBER,			L"line#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_TABLENUMBERROMAN,	L"t�bla",			LVCFMT_RIGHT,	 40,-1,COL_TEXT);

		m_ListCtrl.InsertColumn( L_UNITED,				L"U",				LVCFMT_RIGHT,	 19,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_SPOUSE,				L"2",				LVCFMT_RIGHT,	 19,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_SPOUSEPARENT,		L"3",				LVCFMT_RIGHT,	 19,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_SPOUSESPOUSE,		L"4",				LVCFMT_RIGHT,	 19,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_SOURCE,				L"R",				LVCFMT_RIGHT,	 19,-1,COL_NUM);

		m_ListCtrl.InsertColumn( L_GENERATION,			L"G",				LVCFMT_LEFT,	 19,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_SEX,					L"X",				LVCFMT_LEFT,	 19,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_TITLE,				L"tit",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_TITOLO,				L"el�n�v",			LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_LAST_NAME,			L"csal�dn�v",		LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_KNOWN_AS,			L"m�s",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_FIRST_NAME,			L"ut�n�v",			LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_POSTERIOR,			L"ut�tag",			LVCFMT_LEFT,	 60,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_BIRTH_PLACE,			L"sz�let�s",		LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_BIRTH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_DEATH_PLACE,			L"elhal�loz�s",		LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_DEATH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_COMMENT,				L"le�r�s",			LVCFMT_LEFT,	600,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_AG,					L"�g",				LVCFMT_LEFT,	200,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_CSALAD,				L"csal�d �se",		LVCFMT_LEFT,	100,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_FOLYT,				L"folyt",			LVCFMT_LEFT,	 20,-1,COL_TEXT);
	}
	else if( theApp.m_inputMode == MANUAL )
	{
		m_ListCtrl.InsertColumn( N_ROWID,				L"rowid",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_FAMILYNUMBER,		L"csal�d#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_TABLENUMBER,			L"t�bla#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
		m_ListCtrl.InsertColumn( N_TABLE_ANCESTRY,		L"�s",				LVCFMT_RIGHT,	 20,-1,COL_HIDDEN );
		m_ListCtrl.InsertColumn( N_SEX,					L"nem",				LVCFMT_RIGHT,	 20,-1,COL_HIDDEN);
		m_ListCtrl.InsertColumn( N_LAST_NAME,			L"csal�dn�v",		LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_FIRST_NAME,			L"ut�n�v",			LVCFMT_LEFT,	110,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_BIRTH_PLACE,			L"sz�let�s",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_BIRTH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_DEATH_PLACE,			L"elhal�loz�s",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_DEATH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( N_COMMENT,				L"le�r�s",		LVCFMT_LEFT,		600,-1,COL_TEXT);
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
		if( !m_filter.IsEmpty() && m_filter.Left(5) != L"GROUP" )					// m�r van egy sz�r�s)					// m�r van egy sz�r�s
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
			if( !m_filter.IsEmpty() )  // t�bb felt�tel szerinti sz�r�s
			{
				str.Format(L"(%s) %s %s",m_filter, relation, m_filterNew );
				m_filterNew = str;
				str.Format(L"(%s) %s %s", m_filterText, relation, m_filterTextNew );					// a kor�bbi �s az �j sz�r�felt�tel �ssze�p�t�sa
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

	int cntPerPage = m_ListCtrl.GetCountPerPage();
	if( nItem >= 0 )
	{
		theApp.showItem( nItem, &m_ListCtrl );
/*
		m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		nItem += cntPerPage/2;
		if( nItem > m_recordset->RecordsCount() )
			nItem = m_recordset->RecordsCount() - 1;
		m_ListCtrl.EnsureVisible( nItem, FALSE );
*/
	}
	enableMenu( MF_ENABLED );
	str.Format( L"%s (%d)", m_filterText, m_ListCtrl.GetItemCount() );
	SetWindowTextW( str );
	SetForegroundWindow();

	if( m_ListCtrl.GetItemCount() )
	{
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
	message = L"M�r vannak emberek az adtb�zisban.\n�j ember csak akkor tan�csos megadni, ha nincs rokoni kapcsolatban \
az adatb�zisban m�r l�tez� szem�lyekkel.\n\
Egy�bk�nt a rokonhoz kapcsol�d�an lehet megadni.\n\n\
M�gis megad �j szem�lyt?";

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

		push( lastName );
		push( firstName );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		push( m_recordset->GetFieldString( PEOPLE_COMMENT ) );
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
		push( m_recordset->GetFieldString( PEOPLE_TABLEROMAN ) );

		push( united );
		push( spouse );
		push( spouseparent );
		push( spousespouse );
		push( m_recordset->GetFieldString( PEOPLE_SOURCE ) );

		push( m_recordset->GetFieldString( PEOPLE_GENERATION ) );
		push( m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
		push( m_recordset->GetFieldString( PEOPLE_TITLE ));
		push( m_recordset->GetFieldString( PEOPLE_TITOLO ));
		lastName = m_recordset->GetFieldString( PEOPLE_LAST_NAME );
		lastName.Replace( '|', '\'' );
		push( lastName );
		push( m_recordset->GetFieldString( PEOPLE_KNOWN_AS ) );
		push( m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		push( m_recordset->GetFieldString( PEOPLE_POSTERIOR ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		push( m_recordset->GetFieldString( PEOPLE_COMMENT ) );
		push( m_recordset->GetFieldString( PEOPLE_ARM ) );
		push( m_recordset->GetFieldString( PEOPLE_CSALAD ) );
		push( m_recordset->GetFieldString( PEOPLE_FOLYT ) );
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
		lastName = m_recordset->GetFieldString( PEOPLE_LAST_NAME );
		lastName.Replace( '|', '\'' );
		push( lastName );
		push( m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ) );
		push( m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		push( m_recordset->GetFieldString( PEOPLE_COMMENT ) );
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
// a v_individuals �sz m_ListCtrl renezetts�ge elt�rhet!!! 
// Ez�rt az el�bbiben az m_rowid, az ut�bbibal az m_nItem a szerkesztend�/szerkestett bejegyz�s azonos�t�ja!
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
		name.Format( L"%s %s", m_ListCtrl.GetItemText( m_nItem, L_LAST_NAME ),  m_ListCtrl.GetItemText( m_nItem, L_FIRST_NAME ) );
		m_rowid	= m_ListCtrl.GetItemText( m_nItem, L_ROWID );
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
// a v_individuals rowid azonos�t�j� bejegyz�s�t updateli �s megjelen�ti az eg�sz list�t
//
void CTablePeople::updateEntry()
{
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid='%s'", m_rowid );
	if( !query( m_command ) ) return;
	if( !m_recordset->RecordsCount() ) return;	

	// a v_individuals-ban megkeresi a m�dos�tand� bejegyz�st az m_rowid alapj�n

	int nItem;
	for( nItem = 0; nItem < v_individuals.size(); ++nItem )
	{
		if( v_individuals.at( nItem * m_columnsCount + N_ROWID ) == m_rowid )
			break;
	}

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
		updateField( nItem, G_COMMENT, m_recordset->GetFieldString( PEOPLE_COMMENT ) );
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
		updateField( nItem, L_COMMENT, m_recordset->GetFieldString( PEOPLE_COMMENT ) );
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
		updateField( nItem, N_COMMENT, m_recordset->GetFieldString( PEOPLE_COMMENT ) );
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
		if( wcscmp( theApp.m_username, L"v�gesistvan" ) )
			menu.EnableMenuItem( 5, MF_BYPOSITION|MF_GRAYED);
    }
    else if( theApp.m_inputMode == GAHTML )
    {
        SetMenu(NULL);
        ::DestroyMenu(GetMenu()->GetSafeHmenu());
 //       CMenu menu;
        menu.LoadMenuW(IDR_TABLE_PEOPLE_HTML );
        SetMenu(&menu);
		if( wcscmp( theApp.m_username, L"v�gesistvan" ) )
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
		if( wcscmp( theApp.m_username, L"v�gesistvan" ) )
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
		theApp.message( L"Emberek list�ja", L"\nNincs kijel�lve senki!" );
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
		theApp.message( L"Emberek list�ja", L"\nNincs kijel�lve senki!" );
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
	}
	DrawMenuBar();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnAzonosemberIdent()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		theApp.message( L"Emberek list�ja", L"\nNincs kijel�lve senki!" );
		return;
	}

	CSameParams dlg;
	
	dlg.m_firstName	= m_ListCtrl.GetItemText( nItem, L_FIRST_NAME );
	dlg.m_lastName	= m_ListCtrl.GetItemText( nItem, L_LAST_NAME );

	dlg.DoModal();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDD_TABLE_INDIVIDUAL sysem men�j�nek (X) kattint�s�ra Ez m�k�dik!!!
void CTablePeople::OnClose()  
{
	m_ListCtrl.DeleteAllItems();	// erre sz�ks�g van?
	m_recordset->Clear();			// erre sz�ks�g van?
//	((CDragonDlg*)m_dlg )->m_pIndividuals = NULL;
//	theApp.m_pMainWnd->SetForegroundWindow();
	CDialogEx::OnClose();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnAscendants()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijel�lve senki!" );
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

//	ShowWindow(SW_HIDE);
	CGaAscendants dlg;
	dlg.m_name	= name;
	dlg.m_rowid		= rowid;
	dlg.m_sex_id	= sex_id;
	dlg.DoModal();
//	ShowWindow(SW_RESTORE);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnAscendantsChain()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijel�lve senki!" );
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

//	ShowWindow(SW_HIDE);
	CGaAscendantsChain dlg;
	dlg.m_rowid	= rowid;
	dlg.m_name	= name;
	dlg.DoModal();
//	ShowWindow(SW_RESTORE);
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S � S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePeople::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az als� return-re debug m�dban hib�t jelez
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
	theApp.keress( search, &m_ListCtrl, m_orderix, start );
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
		title.Format( L"%s a ga.html f�jlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijel�lt sor a ga.html f�jlban", selectedCount );

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

	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s %s sz�lei �s testv�rei", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnHtmlChildren()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s %s �s gyermekei", m_ListCtrl.GetItemText( nItem, L_LAST_NAME ), m_ListCtrl.GetItemText( nItem, L_FIRST_NAME ) );
	dlg.m_type	= L"F_CHILDREN";
	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );

	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::On3Generations()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CRelatives dlg;
	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );

	dlg.DoModal();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditDelete()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int sex_id;
	CString rowid = m_ListCtrl.GetItemText( nItem, G_ROWID );
	CString name;

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

	str.Format( L"Val�ban t�r�lni akarod %s(%s) bejegyz�st\n�s minden r� val� hivatkoz�st?", name, rowid );
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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnEditInsert()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );

	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnInfo()
{
	CString info = L"\
Azt oszlopok jelent�se:\
\r\n\r\n\
U - a bejegyz�s ennyi bejegyz�s egyes�t�se\r\n\
2 - a 2. rend� bejegyz�sek sz�ma az egyes�tett bejegyz�sben\r\n\
3 - a 3. rend� bejegyz�sek sz�ma az egyes�tett bejegyz�sben\r\n\
4 - a 4. rend� bejegyz�sek sz�ma az egyes�tett bejegyz�sben\r\n\
R - bejegyz�s rangja\r\n\
G - gener�ci�s k�d\
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

	CProgressWnd wndProgress(NULL, L"�s�k keres�se" );
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

	CProgressWnd wndProgress(NULL, L"�s�k keres�se" );
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
// Az father_id-j� bejegyz�s �s�t keresi meg, azaz akkinek m�r nincsa apja
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
// Az father_id-j� bejegyz�s �s�t keresi meg, azaz akkinek m�r nincsa apja
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
		if( i < v_individuals.size() - m_columnsCount + N_ROWID )  // m�r l�tez� bejegyz�s m�dos�t�sa
			updateEntry();

		nItem = i;
	}
	m_ListCtrl.DetachDataset();
	m_ListCtrl.AttachDataset( &v_individuals );
	theApp.showItem( nItem, &m_ListCtrl );
	return true;
}
