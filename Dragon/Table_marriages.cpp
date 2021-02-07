// Table_marriages.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Table_marriages.h"
#include "afxdialogex.h"
#include "html_EditLine.h"
#include "Filter.h"
#include "DragonDlg.h"
#include "SelectOne.h"
#include "GetString.h"
#include "html_EditLines.h"
#include "GetLastFirst.h"
#include "EditMarriage.h"
#include "windows.h"
#include "ProgressWnd.h"
#include "utilities.h"

#include "checkParam0.h"
#include <algorithm>


enum
{
	LIST_ROWID = 0,
	LIST_LINENUMBER,
	LIST_SOURCE,
	LIST_MPLACE,
	LIST_MDATE,
	LIST_SPOUSE1_ID,
	LIST_HLASTNAME,
	LIST_HFIRSTNAME,
	LIST_HBIRTHDATE,
	LIST_HDEATHDATE,
	LIST_ORDERWIFE,
	LIST_SEPARATOR,
	LIST_SPOUSE2_ID,
	LIST_WLASTNAME,
	LIST_WFIRSTNAME,
	LIST_WBIRTHDATE,
	LIST_WDEATHDATE,
	LIST_ORDERHUSBAND,
};

enum
{
	P_ROWID = 0,
	P_FIRST_NAME,
	P_LAST_NAME,
	P_SEX_ID,
	P_LINENUMBER,
	P_TABLENUMBER,
	P_GENERATION,
	P_SOURCE,
	P_BIRTH_DATE,
	P_DEATH_DATE,
	P_FATHER_ID,
	P_MOTHER_ID,
	P_UNITED,
};

// ListCtrl oszlopok a több házastárs
enum
{
	L_LINENUMBER = 0,
	L_ORDER, 
	L_DATE,
	L_SOURCEM,
	L_SOURCE,
	L_ROWID,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_ROWIDF,
	L_FATHER,
	L_BIRTHF,
	L_DEATHF,
	L_ROWIDM,
	L_MOTHER,
	L_BIRTHM,
	L_DEATHM,
};
bool sortByNameX(const SPOUSE11 &v1, const SPOUSE11 &v2);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTableMarriages, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableMarriages::CTableMarriages(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableMarriages::IDD, pParent)
{
	m_pParent = pParent;
	m_explanation = L"Azokat az embereket listázzuk, akiknek több házastársa van. A lista elsõsorban arra szolgál, \
hogy ellenõrizzük a házasságok sorszámát.\
<br><br>\
Az alábbi oszlopok vannak a listában:\
<pre>\
<br>\
line#	a leszármazott/házastárs ssorszáma aGA.html fájlban.<br>\
O	a házasság sorszáma<br>\
esküvõ	az esküvõ dátuma<br>\
S	a házatársak hierachia sorszáma. ( 1-5 )<br>\
</pre>\
a további oszlopok magától érthetõek.<br>\
";

	p_fields = L"\
rowid,\
first_name,\
last_name,\
sex_id,\
lineNumber,\
tableNumber,\
generation,\
source,\
birth_date,\
death_date,\
father_id,\
mother_id,\
united\
";

	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableMarriages::~CTableMarriages()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_KERES, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableMarriages, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(ID_FILTER_HLASTNAME, &CTableMarriages::OnFilterHlastname)
	ON_COMMAND(ID_EXPORT_ALL, &CTableMarriages::OnExportAll)
	ON_COMMAND(ID_EXPORT_SELECTED, &CTableMarriages::OnExportSelected)
	ON_COMMAND(ID_AZONOS_MARRIAGELIST, &CTableMarriages::OnAzonosMarriagelist)
	ON_COMMAND(ID_UNFILTER, &CTableMarriages::OnUnfilter)
	ON_COMMAND(ID_FILTER_WLASTNAME, &CTableMarriages::OnFilterWlastname)
	ON_WM_CLOSE()
	ON_COMMAND(ID_GROUPBY_PLACE, &CTableMarriages::OnGroupbyPlace)
	ON_COMMAND(ID_MARRIAGES_DESCENDANTS, &CTableMarriages::OnMarriagesDescendants)
	ON_COMMAND(ID_MARRIAGES_SPOUSES, &CTableMarriages::OnMarriagesSpouses)
	ON_COMMAND(ID_MARRIAGES_PARENTS, &CTableMarriages::OnMarriagesParents)
	ON_COMMAND(ID_ROWID_MARRIAGES, &CTableMarriages::OnRowidMarriages)
	ON_COMMAND(ID_NAMEMARRIAGES, &CTableMarriages::OnNameMarriages)
	ON_COMMAND(ID_HTML_LINE, &CTableMarriages::OnHtmlEdit)
	ON_COMMAND(ID_HTML_EDIT, &CTableMarriages::OnHtmlShows)
	ON_COMMAND(ID_HTML_NOTEPAD, &CTableMarriages::OnHtmlNotepad)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTableMarriages::OnDblclkList)
	ON_STN_CLICKED(IDC_KERES, &CTableMarriages::OnClickedKeres)
	ON_COMMAND(ID_MOREMARRIAGES, &CTableMarriages::OnMoreMarriages)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CTableMarriages::OnCustomdrawList)
	ON_STN_CLICKED(IDC_NEXT, &CTableMarriages::OnClickedNext)
	ON_COMMAND(ID_MAN_MORESPOUSES, &CTableMarriages::OnManMorespouses)

// IDR_DROPDOWN_EDIT
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_EDIT_UPDATE, &CTableMarriages::OnEditUpdate)
	ON_COMMAND(ID_EDIT_DELETE, &CTableMarriages::OnEditDelete)
	ON_COMMAND(ID_EDIT_GAHTML, &CTableMarriages::OnEditGahtml)
	
	
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT()

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


	m_orderix = 0;

	enableMenu( MF_GRAYED );

	colorKeres.SetTextColor( theApp.m_colorClick ); 
	colorNext.SetTextColor( theApp.m_colorClick );


	m_command.Format( L"SELECT count(*) FROM marriages" );
	if( !theApp.query( m_command ) ) return false;

	int cnt = _wtoi(theApp.m_recordset->GetFieldString( 0 ));
	if( cnt && cnt < 10000 )
		OnUnfilter();

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::fillVectors()
{

	CString title(L"Házastársak lekérdezése" ); 
	wndP.Create( NULL, title );
	wndP.GoModal();
	
	CString fileName;
	fileName += L"peopleHavingMoreSpouses";
	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	htmlHeader( title );

#ifndef _DEBUG
	str.Format( L"Férjek lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif

	m_command.Format( L"SELECT rowid,* FROM marriages ORDER BY spouse1_id" );
	if( !query( m_command ) ) return false;

	collectHusband();
	
#ifndef _DEBUG
	str.Format( L"(1/6) Feleségek lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif
	m_command.Format( L"SELECT rowid,* FROM marriages ORDER BY spouse2_id" );
	if( !query( m_command ) ) return false;

	collectWife();

	fillSpouse1();
	fillSpouse2();

	std::sort( vSpouse1.begin(), vSpouse1.end(), sortByNameX );
//	listHtml();
	fillTableF();


	wndP.DestroyWindow();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::enableMenu( BOOL flag)
{
	CMenu* menu;

	menu = GetMenu();

	menu->EnableMenuItem(1, MF_BYPOSITION| flag );
	DrawMenuBar();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// S Z Û R É S E K ///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnUnfilter()
{
	m_filterText = L"Szüretlen házasságok";
	m_filter.Empty();
	m_last_name_h.Empty();
	m_last_name_w.Empty();
	fillTable( 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnFilterHlastname()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == - 1)
	{
		theApp.message( L"Házastársak listája", L"\nNincs kijelölve házaspár!" );
		//AfxMessageBox( L"Nincs kijelölve házaspár!" );
		return;
	}

	m_last_name_w.Empty();
	m_last_name_h = m_ListCtrl.GetItemText( nItem, LIST_HLASTNAME );

	m_filterText.Empty();
	m_filterText.Format( L"%s családnevû férjek házasságai", m_last_name_h );
	fillTable( 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnFilterWlastname()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == - 1)
	{
		theApp.message( L"Házastársak listája", L"\nNincs kijelölve házaspár!" );
//		AfxMessageBox( L"Nincs kijelölve házaspár!" );
		return;
	}

	m_last_name_w.Empty();
	m_last_name_w = m_ListCtrl.GetItemText( nItem, LIST_WLASTNAME );

	m_filter.Empty();
	m_filterText.Format( L"%s családnevû feleségek házasságai", m_last_name_w );

	fillTable( 0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnMarriagesDescendants()
{
	m_last_name_h.Empty();
	m_last_name_w.Empty();
	m_filterText = L"Leszármazottak házasságai";
	m_filter = L"WHERE source=1";
	fillTable( 0 );
}
void CTableMarriages::OnMarriagesSpouses()
{
	m_last_name_h.Empty();
	m_last_name_w.Empty();
	m_filterText = L"Leszármazottak házastársainak további házasságai";
	m_filter = L"WHERE source=2";
	fillTable( 0 );
}
void CTableMarriages::OnMarriagesParents()
{
	m_last_name_h.Empty();
	m_last_name_w.Empty();
	m_filterText = L"Leszármazottak házastársai szüleinek házasságai";
	m_filter = L"WHERE source=3";
	fillTable( 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnGroupbyPlace()
{
	m_last_name_h.Empty();
	m_last_name_w.Empty();
	m_filterText	= L"Különbözõ házasságkötési helyek";
	m_filter		= L"GROUP BY place";
	m_orderix = LIST_MPLACE;
	fillTable( 0 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnRowidMarriages()
{
	CGetString dlg;

	int rowid;
	rowid = theApp.GetProfileInt(	L"Settings", L"rowid", 0 );
	dlg.m_string.Format( L"%d", rowid ); 
	dlg.m_caption.Format( L"Add meg a kívánt ember rowid-ját!" );
	if( dlg.DoModal() == IDCANCEL ) return;
	rowid = _wtoi( dlg.m_string );
	theApp.WriteProfileInt( L"Settings", L"rowid", rowid  );

	m_last_name_h.Empty();
	m_last_name_w.Empty();
	m_filterText.Format( L"%s rowid házasságai", dlg.m_string );
	m_filter.Format( L"WHERE spouse1_id=%s OR spouse2_id=%s", dlg.m_string, dlg.m_string );
	fillTable( 0 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnNameMarriages()
{
	CGetLastFirst dlg;
	if( dlg.DoModal() == IDCANCEL ) return;

	m_filterText.Format( L"%s nevû emberek házasságai", dlg._fullname );
	m_filter.Format( L"WHERE spouse1_id IN (SELECT rowid FROM people WHERE first_name='%s' AND last_name='%s') OR spouse2_id IN (SELECT rowid FROM people WHERE first_name='%s' AND last_name='%s') ORDER BY spouse1_id", dlg._firstname, dlg._lastname, dlg._firstname, dlg._lastname  );

	fillTable( 0 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::fillTable( UINT nItem )
{
	m_ListCtrl.DeleteAllItems();
	for( int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i-- )
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( LIST_ROWID,		L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_NUM );
	m_ListCtrl.InsertColumn( LIST_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LIST_SOURCE,		L"src",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LIST_MPLACE,		L"hk helye",	LVCFMT_LEFT,	110,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_MDATE,		L"hk ideje",	LVCFMT_LEFT,	 90,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_SPOUSE1_ID,	L"spouse1_id",	LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LIST_HLASTNAME,	L"férj",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_HFIRSTNAME,	L"",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_HBIRTHDATE,	L"született",	LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_HDEATHDATE,	L"meghalt",		LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_ORDERWIFE,	L"# házasság",	LVCFMT_RIGHT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LIST_SEPARATOR,	L"",			LVCFMT_LEFT,	 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_SPOUSE2_ID,	L"spouse2_id",	LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LIST_WLASTNAME,	L"feleség",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_WFIRSTNAME,	L"",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_WBIRTHDATE,	L"született",	LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_WDEATHDATE,	L"meghalt",		LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_ORDERHUSBAND,	L"# házasság",	LVCFMT_RIGHT,	 70,-1,COL_NUM);

	m_columnsCount	= m_ListCtrl.GetHeaderCtrl()->GetItemCount();


	CProgressWnd wndProgress(NULL, m_filterText ); 
	wndProgress.GoModal();


	if( m_filter.IsEmpty() )
		m_command = L"SELECT rowid,* FROM marriages";
	else
		m_command.Format( L"SELECT rowid,* FROM marriages %s", m_filter );

	GetDlgItem( IDC_CAPTION )->SetWindowTextW( m_filterText);
	if( !theApp.query( m_command ) ) return;

	m_ListCtrl.DeleteAllItems();
	v_tableMarriages.clear();

	wndProgress.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	CString	strBuffer;
	CString id;
	CString rowid_spouse1;
	CString rowid_spouse2;
	CString last_name1;
	CString last_name2;
	int		sex_id;
	

	vColor.clear();
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		vColor.push_back( 0 );
		rowid_spouse1 = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		m_command.Format( L"SELECT sex_id, last_name, first_name, birth_date, death_date FROM people WHERE rowid='%s'", rowid_spouse1 );
		if( !theApp.query1( m_command ) ) return;
		sex_id = _wtoi( theApp.m_recordset1->GetFieldString( 0 ) );
		last_name1 = theApp.m_recordset1->GetFieldString( 1 );
		last_name1.Replace( '|','\'' );
		

		rowid_spouse2 = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
		m_command.Format( L"SELECT sex_id, last_name, first_name, birth_date, death_date FROM people WHERE rowid='%s'", rowid_spouse2 );
		if( !theApp.query2( m_command ) ) return;
		last_name2 = theApp.m_recordset2->GetFieldString( 1 );
		last_name2.Replace( '|','\'' );

		if( !m_last_name_h.IsEmpty() )
		{
			if( sex_id == 1 )
			{
				if( last_name1 != m_last_name_h ) continue;
			}
			else
			{
				if( last_name2 != m_last_name_h ) continue;
			}
		}

		if( !m_last_name_w.IsEmpty() )
		{
			if( sex_id == 2 )
			{
				if( last_name1 != m_last_name_w ) continue;
			}
			else
			{
				if( last_name2 != m_last_name_w ) continue;
			}
		}

		push( theApp.m_recordset->GetFieldString( MARRIAGES_ROWID ) );
		push( theApp.m_recordset->GetFieldString( MARRIAGES_LINENUMBER ) );
		push( theApp.m_recordset->GetFieldString( MARRIAGES_SOURCE ) )	;
		push( theApp.m_recordset->GetFieldString( MARRIAGES_PLACE ) );
		push( theApp.m_recordset->GetFieldString( MARRIAGES_DATE ) );
		push( theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID ) );
		push( last_name1 );
		push( theApp.m_recordset1->GetFieldString( 2 ) );
		push( theApp.m_recordset1->GetFieldString( 3 ) );
		push( theApp.m_recordset1->GetFieldString( 4 ) );
		push( theApp.m_recordset->GetFieldString( MARRIAGES_ORDERWIFE ) );
		push( L" - " );
		push( theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID ) );
		push( last_name2 );
		push( theApp.m_recordset2->GetFieldString( 2 ) );
		push( theApp.m_recordset2->GetFieldString( 3 ) );
		push( theApp.m_recordset2->GetFieldString( 4 ) );
		push( theApp.m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND ) );

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.SetItemCountEx( v_tableMarriages.size() );
	m_ListCtrl.AttachDataset( &v_tableMarriages );
	m_ListCtrl.ReSort();

	wndProgress.DestroyWindow();

	m_ListCtrl.SetSortOrder( 1, m_orderix+1 );

	int cntPerPage = m_ListCtrl.GetCountPerPage();
	if( nItem > 0 )
	{
		m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		nItem += cntPerPage/2;
		if( nItem > theApp.m_recordset->RecordsCount() )
			nItem = theApp.m_recordset->RecordsCount() - 1;
		m_ListCtrl.EnsureVisible( nItem, FALSE );
	}
	enableMenu( MF_ENABLED );
	str.Format( L"%s ( %d)", m_filterText, m_ListCtrl.GetItemCount() );
	SetWindowTextW( str );
	SetForegroundWindow();  // kell, mert szûrés után csak így jön vissz az ablak!!!
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::push( CString str )
{
	int tLen=str.GetLength()+1;
	LPTSTR sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,str.GetBuffer());
	v_tableMarriages.push_back(sT);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableMarriages::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CTableMarriages::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = (int)wParam;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CTableMarriages::OnChangeSearch()
//{
//	CString	search;
//	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
//	theApp.search( search, m_orderix,  &m_ListCtrl );
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CTableMarriages::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	LIST_LINENUMBER ) );

	if( theApp.m_inputMode == GAHTML )
	{
		theApp.listHtmlLine( lineNumber );
	}

	*pResult = 0;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnExportAll()
{
	CString	logFile(L"marriages"); 
	CString	title(L"Minden házasság");;
	
//	title.Format( L"%s", m_filterText );
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
void CTableMarriages::OnExportSelected()
{
	CString	logFile(L"marriages"); 
	CString	title( L"Kijelölt házasságok" );
	//title.Format( L"%s (selected from %d projects)", m_filterText, m_ListCtrl.GetItemCount() );
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnAzonosMarriagelist()
{
	CString rowid;
	CString rowidNext;

	CString husband;
	CString husbandNext;
	CString wife;
	CString wifeNext;
	CString couple;
	CString coupleNext;

	CString printed( L" " );

	int nItem = 0;
	int cnt = 0;

	CString logFile( L"sameMarriages" );
	CString caption( L"Az alábbi nevekkel több házasság szerepel az adatbázisban");

	CProgressWnd wndProgress(NULL,L"Azonos nevû házaspárok keresése ..." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, m_ListCtrl.GetItemCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);


	if(( theApp.fl=theApp.openLogFile( logFile,caption ))==NULL)return;


#ifndef _DEBUG
	wndProgress.SetText( L"Házaspárok azonosítása..." );
#endif

	m_ListCtrl.SetSortOrder( 4, LIST_HLASTNAME + 1, LIST_HFIRSTNAME + 1, LIST_WLASTNAME + 1, LIST_WFIRSTNAME + 1   );

	rowid	= m_ListCtrl.GetItemText( nItem, LIST_ROWID );
	husband.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, LIST_HLASTNAME ), m_ListCtrl.GetItemText( nItem, LIST_HFIRSTNAME ) );
	wife.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, LIST_WLASTNAME ), m_ListCtrl.GetItemText( nItem, LIST_WFIRSTNAME ) );
	couple	= husband+wife;
	if( !theApp.execute( L"BEGIN" ) ) return;
	for( int nItem = 1 ; nItem < m_ListCtrl.GetItemCount(); ++nItem )
	{
		rowidNext	= m_ListCtrl.GetItemText( nItem, LIST_ROWID );
		husbandNext.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, LIST_HLASTNAME ), m_ListCtrl.GetItemText( nItem, LIST_HFIRSTNAME ) );
		wifeNext.Format( L"%s %s", m_ListCtrl.GetItemText( nItem, LIST_WLASTNAME ), m_ListCtrl.GetItemText( nItem, LIST_WFIRSTNAME ) );
		coupleNext	= husbandNext+wifeNext;
		if( !couple.Compare( coupleNext ) )
		{
			printed.Format( L"%8s-%-8s %-30s %s\n", rowid, rowidNext, husband, wife );
			fwprintf( theApp.fl, printed );
			++cnt;
			++nItem;
		}

		husband = husbandNext;
		wife	= wifeNext;
		couple	= coupleNext;
		rowid	= rowidNext;

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	fclose( theApp.fl );
	if( cnt )
	{
		ShowWindow( SW_HIDE );
		theApp.showLogFile();
	}
	else
		theApp.message( L"Házastársak listája", L"\nNem találtam többször elõforduló házasságot!" );
//		AfxMessageBox( L"Nem találtam többször elõforduló házasságot!" );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if(m_pParent)
	{		
		((CDragonDlg*)m_pParent)->m_pMarriages = NULL;
	}
	delete this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnClose()
{
	m_ListCtrl.DeleteAllItems();	// erre szükség van?
	theApp.m_recordset->Clear();	// erre szükség van?
	((CDragonDlg*)m_pParent )->m_pMarriages = NULL;
	theApp.m_pMainWnd->SetForegroundWindow();
	CDialogEx::OnClose();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CTableMarriages::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int linenumber;
	if( m_columnsCount == 18 )
		linenumber = _wtoi( m_ListCtrl.GetItemText( nItem, LIST_LINENUMBER ) );
	else
		linenumber = _wtoi( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) );


	theApp.listHtmlLine( linenumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString linenumber;
	if( m_columnsCount == 18 )
		linenumber = m_ListCtrl.GetItemText( nItem, LIST_LINENUMBER );
	else
		linenumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );

	if( !linenumber.IsEmpty() ) 
		theApp.editNotepad( linenumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnHtmlShows()
{
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int			nItem;
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");

	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		if( m_columnsCount == 18 )
		{
			vLines.push_back( m_ListCtrl.GetItemText( nItem, LIST_LINENUMBER ) );
			if( name.Compare( m_ListCtrl.GetItemText( nItem, LIST_HFIRSTNAME ) ) )
			{
				name = m_ListCtrl.GetItemText( nItem, LIST_HFIRSTNAME );
				++cnt;
			}
		}
		else
		{
			vLines.push_back( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) );
			++cnt;
		}
	}

	CHtmlEditLines dlg;

	dlg.m_title = name;
	dlg.vLines = &vLines;

	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	int nItem = pNMItemActivate->iItem;

	if( m_columnsCount == 18 )
	{
		ShowWindow( SW_HIDE );
		CEditMarriage dlg;
		dlg.m_rowid = m_ListCtrl.GetItemText( nItem, LIST_ROWID );
		dlg.DoModal();
		ShowWindow( SW_RESTORE );
	}
	else
	{
		int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) );
		if( theApp.m_inputMode == GAHTML )
		{
			theApp.listHtmlLine( lineNumber );
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableMarriages::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup = NULL;


	if( theApp.m_inputMode == GAHTML )
	{
//		if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
		if(Menu.LoadMenu( IDR_DROPDOWN_EDIT ))
		{
			pPopup = Menu.GetSubMenu(0);
			if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
			{
				pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_EDIT_GAHTML,MF_BYCOMMAND | MF_GRAYED);
			}
		 }
	}
	else
	{
		if(Menu.LoadMenu( IDR_DROPDOWN_DELETE_UPDATE ))
		{
			pPopup = Menu.GetSubMenu(0);
			if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
			{
				pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
			}

		 }
	}
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnEditUpdate()
{


//	ShowWindow( SW_HIDE );
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CEditMarriage dlg;
	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, LIST_ROWID );
	dlg.DoModal();



//	HWND hWind = ::FindWindow( TEXT( "CTableMarriages" ), NULL );
//	ShowWindow( hWind, SW_RESTORE );

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnEditDelete()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	CString	rowid;
	rowid		= m_ListCtrl.GetItemText( nItem, LIST_ROWID );

	str.Format( L"Tényleg törölni akarod a rowid=%s házasságot?", rowid );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;


	m_command.Format( L"DELETE FROM marriages WHERE rowid='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;

	m_ListCtrl.DeleteItem( nItem );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnEditGahtml()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	if( _waccess( theApp.m_htmlFileSpec, 0 ) )
	{
		AfxMessageBox( L"file does not exist!" );
		return;
	}

	int htmlLineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	LIST_LINENUMBER ) );
	CHtmlEditLine dlg;

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
	CString cLine;
	
	for( int i = 0; i < htmlLineNumber; ++i ) 
		file.ReadString( cLine );

	file.Close();

	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
	if( cLine.GetAt(0) != '%' || cLine.Left(3) == L"%%%" )
	{
		cLine = cleanHtmlLine( cLine );
	}
	dlg.m_line = cLine;
	dlg.DoModal();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -0 )
	{
		AfxMessageBox( L"Nincs kijelölve sor!" );
		return;
	}

	keress( nItem + 1 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnClickedKeres()
{
	keress( 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	if( search.IsEmpty() )
	{
		AfxMessageBox( L"Meg kell adni a keresendõ stringet!");
		return;
	}


	int		itemCnt	= m_ListCtrl.GetItemCount();
	int		length	= search.GetLength();
	int		nItem;
	int		topIndex = m_ListCtrl.GetTopIndex();
	CString	str;

	theApp.unselectAll( &m_ListCtrl );
	if( m_columnsCount == 18 )
	{
		if( m_orderix == 0 )
		{
			AfxMessageBox( L"Rendezni kell az oszlopot, amelyben keresni akarsz!" );
			return;
		}
	

	}
	else
	{
		m_orderix = L_NAME;	
	}
	for( nItem = start; nItem < itemCnt-1; ++nItem )
	{
		str = m_ListCtrl.GetItemText( nItem, m_orderix);
		str = str.Left(length);						// az aktuális search string hosszával azonos hosszúság leválasztása
		if( str == search )	break;
	}

	

	if( nItem < itemCnt-1 )			// megtalálta a keresett embert,. aki az nItem-1 sorban van
	{
		m_ListCtrl.EnsureVisible( nItem, FALSE );

		if( nItem > topIndex )   // lefele megy, fel kell hozni a tábla tetejére a megtalált sort
		{
			int countPP = m_ListCtrl.GetCountPerPage();
			int nItemEV	= nItem - 1 + countPP;			// alaphelyzet: a kijelölt sor az ablak tetején

			if( nItemEV > itemCnt - 1 )					// már nem lehet az ablak tetejére hozni, mert nincs annyi adat
				nItemEV = itemCnt - 1;

			m_ListCtrl.EnsureVisible( nItemEV, FALSE );
		}
		m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		Invalidate( false );
	}
	else
	{
		str.Format( L"%s nevû embert nem találtam!", search );
		AfxMessageBox( str );
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::PreTranslateMessage(MSG* pMsg)
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnMoreMarriages()
{
	enableMenu( MF_GRAYED );
	m_ListCtrl.DeleteAllItems();
	for( int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i-- )
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ORDER,		L"O",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DATE,		L"esküvõ",		LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SOURCEM,		L"M",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_RIGHT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_RIGHT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_ROWIDF,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,		L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHF,		L"születés",	LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATHF,		L"halál",		LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ROWIDM,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,		L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHM,		L"születés",	LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATHM,		L"halál",		LVCFMT_RIGHT,	 80,-1,COL_TEXT);

	m_columnsCount	= m_ListCtrl.GetHeaderCtrl()->GetItemCount();

	nItem = 0;
	m_cnt = 0;

	CString title(L"Emberek, akiknek több házastársuk volt" ); 
	wndP.Create( NULL, title );
	wndP.GoModal();
	
	CString fileName;
	fileName += L"peopleHavingMoreSpouses";
	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	htmlHeader( title );

#ifndef _DEBUG
	str.Format( L"Férjek lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif

	m_command.Format( L"SELECT rowid,* FROM marriages ORDER BY spouse1_id" );
	if( !query( m_command ) ) return;

	collectHusband();
	
#ifndef _DEBUG
	str.Format( L"(1/6) Feleségek lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif
	m_command.Format( L"SELECT rowid,* FROM marriages ORDER BY spouse2_id" );
	if( !query( m_command ) )
		return;

	collectWife();

	fillSpouse1();
	fillSpouse2();

	std::sort( vSpouse1.begin(), vSpouse1.end(), sortByNameX );
//	listHtml();
	fillTableF();


	wndP.DestroyWindow();
	
	if( !vSpouse1.size() )
	{
		str = L"Nem találtam olyan embert, akinek több házassága lenne!";
		AfxMessageBox( str );
		return;
	}


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vSpouse1 vektorban a férj
// vSpouse2 vektorban a feleségek

void CTableMarriages::collectHusband()
{
	CString spouse1_id;
	CString spouse2_id;

	bool	volt = false;  // true: ha a férjt már eltettük vSpouse1-be
	
	SPOUSE11 husband;
	SPOUSE21 wifes;

#ifndef _DEBUG
	str.Format( L"(2/6) Férfiak házasságai..." );
	wndP.SetText( str );
#endif

	wndP.SetRange( 0, m_recordset->RecordsCount()/2-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		spouse1_id = m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		m_recordset->MoveNext();

		// ha volt == false, nem teszi el a férjet, csak akkor, ha a következõ is õ (rowid azonos)
		if( spouse1_id == m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID ) )
		{
			if( !volt )
			{
				husband.rowid	= spouse1_id;
				vSpouse1.push_back( husband );
				volt = true;
			}
			m_recordset->MovePrevious();
			wifes.rowidS	= spouse1_id;   // feleség férjének rowid-je
			wifes.rowid	= m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );

			wifes.date	= m_recordset->GetFieldString( MARRIAGES_DATE );
			wifes.place	= m_recordset->GetFieldString( MARRIAGES_PLACE );
			wifes.order	= m_recordset->GetFieldString( MARRIAGES_ORDERWIFE );
			wifes.sourceM = m_recordset->GetFieldString( MARRIAGES_SOURCE );	
		
			vSpouse2.push_back( wifes );
			m_recordset->MoveNext();
		}
		else
		{
			if( volt )
			{
				m_recordset->MovePrevious();
				wifes.rowidS	= spouse1_id;
				wifes.rowid	= m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );

				wifes.date	= m_recordset->GetFieldString( MARRIAGES_DATE );
				wifes.place	= m_recordset->GetFieldString( MARRIAGES_PLACE );

				wifes.order	= m_recordset->GetFieldString( MARRIAGES_ORDERWIFE );
		
				vSpouse2.push_back( wifes );
				m_recordset->MoveNext();
			}
			volt = false;
		}

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Nõk házasságai:
// vSpouse1 vektorban a nõ
// vSpouse2 vektorban a férjek
void CTableMarriages::collectWife()
{
	CString spouse1_id;
	CString spouse2_id;

	UINT i;
	bool	volt = false;
	
	SPOUSE11 wife;
	SPOUSE21 husband;

#ifndef _DEBUG
	str.Format( L"(3/6) Nõk házasságai..." );
	wndP.SetText( str );
#endif

	wndP.SetRange( 0, m_recordset->RecordsCount()/2-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );
	for( i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		spouse2_id = m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
		m_recordset->MoveNext();
		if( spouse2_id == m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID ) )
		{
			if( !volt )
			{
				wife.rowid	= spouse2_id;
				vSpouse1.push_back( wife );
				volt = true;
			}
			m_recordset->MovePrevious();
			husband.rowidS	= spouse2_id;
			husband.rowid	= m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );

			husband.date	= m_recordset->GetFieldString( MARRIAGES_DATE );
			husband.place	= m_recordset->GetFieldString( MARRIAGES_PLACE );
			husband.order	= m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND );
			husband.sourceM	= m_recordset->GetFieldString( MARRIAGES_SOURCE );
		
			vSpouse2.push_back( husband );
			m_recordset->MoveNext();
		}
		else
		{
			if( volt )
			{
				m_recordset->MovePrevious();
				husband.rowidS	= spouse2_id;
				husband.rowid	= m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );

				husband.date	= m_recordset->GetFieldString( MARRIAGES_DATE );
				husband.place	= m_recordset->GetFieldString( MARRIAGES_PLACE );
				husband.order	= m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND );
		
				vSpouse2.push_back( husband );
				m_recordset->MoveNext();
			}
			volt = false;
		}

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vSpouse1 vektor feltöltése a 'people' táblábólé lekérdezett adatokkal
void CTableMarriages::fillSpouse1()
{
	UINT i;
	CString rowid;
	CString firstName;
	CString lastName;
	CString name;
	CString father_id;
	CString mother_id;

#ifndef _DEBUG
	str.Format( L"(4/6) Emberek adatainak lekérdezése..." );
	wndP.SetText( str );
#endif

	wndP.SetRange( 0, vSpouse1.size()-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	for( i = 0; i < vSpouse1.size(); ++i )
	{
		rowid = vSpouse1.at(i).rowid;
		m_command.Format( L"SELECT last_name,  first_name, birth_date, death_date, source, sex_id, lineNumber, father_id, mother_id FROM people WHERE rowid ='%s'", rowid );
		if( !query( m_command ) ) return;

		lastName		= m_recordset->GetFieldString( 0 );
		firstName		= m_recordset->GetFieldString( 1 );
		name.Format ( L"%s %s", lastName, firstName );
		vSpouse1.at(i).name = name;
		vSpouse1.at(i).birth	= m_recordset->GetFieldString( 2 );
		vSpouse1.at(i).death	= m_recordset->GetFieldString( 3 );
		vSpouse1.at(i).source	= m_recordset->GetFieldString( 4 );
		vSpouse1.at(i).sex_id	= m_recordset->GetFieldString( 5 );
		vSpouse1.at(i).linenumber = m_recordset->GetFieldString( 6 );

		father_id				= m_recordset->GetFieldString( 7 );
		mother_id				= m_recordset->GetFieldString( 8 );


		vSpouse1.at(i).rowidF	= father_id;
		vSpouse1.at(i).rowidM	= mother_id;



		m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid ='%s'", father_id );
		if( !query( m_command ) ) return;
		firstName = getFirstWord( m_recordset->GetFieldString( 1 ) );
		name.Format( L"%s %s", m_recordset->GetFieldString( 0 ), firstName );
		vSpouse1.at(i).father	= name;
		vSpouse1.at(i).birthF	= m_recordset->GetFieldString( 2 );
		vSpouse1.at(i).deathF	= m_recordset->GetFieldString( 3 );
		

		m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid ='%s'", mother_id );
		if( !query( m_command ) ) return;
		firstName = getFirstWord( m_recordset->GetFieldString( 1 ) );
		name.Format( L"%s %s", m_recordset->GetFieldString( 0 ), firstName );
		vSpouse1.at(i).mother	= name;
		vSpouse1.at(i).birthM	= m_recordset->GetFieldString( 2 );
		vSpouse1.at(i).deathM	= m_recordset->GetFieldString( 3 );

		if( vSpouse1.at(i).rowidF == L"0" )
			vSpouse1.at(i).rowidF = L" ";
		if( vSpouse1.at(i).rowidM == L"0" )
			vSpouse1.at(i).rowidM = L" ";

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vSpouse2 vektor feltöltése a 'people' táblábólé lekérdezett adatokkal
void CTableMarriages::fillSpouse2()
{
	UINT i;
	CString rowid;
	CString firstName;
	CString lastName;
	CString name;
	CString father_id;
	CString mother_id;

#ifndef _DEBUG
	str.Format( L"(5/6) Házastársak adatainak lekérdezése..." );
	wndP.SetText( str );
#endif

	wndP.SetRange( 0, vSpouse2.size()-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	for( i = 0; i < vSpouse2.size(); ++i )
	{
		rowid = vSpouse2.at(i).rowid;
		m_command.Format( L"SELECT last_name,  first_name, birth_date, death_date, source, sex_id, lineNumber, father_id, mother_id FROM people WHERE rowid ='%s'", rowid );
		if( !query( m_command ) ) return;

		lastName		= m_recordset->GetFieldString( 0 );
		firstName		= m_recordset->GetFieldString( 1 );
		name.Format ( L"%s %s", lastName, firstName );
		vSpouse2.at(i).name = name;
		vSpouse2.at(i).birth	= m_recordset->GetFieldString( 2 );
		vSpouse2.at(i).death	= m_recordset->GetFieldString( 3 );
		vSpouse2.at(i).source	= m_recordset->GetFieldString( 4 );
		vSpouse2.at(i).sex_id	= m_recordset->GetFieldString( 5 );
		vSpouse2.at(i).linenumber = m_recordset->GetFieldString( 6 );

		father_id				= m_recordset->GetFieldString( 7 );
		mother_id				= m_recordset->GetFieldString( 8 );
		vSpouse2.at(i).rowidF	= father_id;
		vSpouse2.at(i).rowidM	= mother_id;


		m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid ='%s'", father_id );
		if( !query( m_command ) ) return;
		firstName = getFirstWord( m_recordset->GetFieldString( 1 ) );
		name.Format( L"%s %s", m_recordset->GetFieldString( 0 ), firstName );
		vSpouse2.at(i).father	= name;
		vSpouse2.at(i).birthF	= m_recordset->GetFieldString( 2 );
		vSpouse2.at(i).deathF	= m_recordset->GetFieldString( 3 );


		m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid ='%s'", mother_id );
		if( !query( m_command ) ) return;
		firstName = getFirstWord( m_recordset->GetFieldString( 1 ) );
		name.Format( L"%s %s", m_recordset->GetFieldString( 0 ), firstName );
		vSpouse2.at(i).mother	= name;
		vSpouse2.at(i).birthM	= m_recordset->GetFieldString( 2 );
		vSpouse2.at(i).deathM	= m_recordset->GetFieldString( 3 );

		if( vSpouse2.at(i).rowidF == L"0" )
			vSpouse2.at(i).rowidF = L" ";
		if( vSpouse2.at(i).rowidM == L"0" )
			vSpouse2.at(i).rowidM = L" ";

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::listHtml()
{
	CString header;
	CString people;
	CString father;
	CString mother;
	CString spouse1_id;
	CString spouse2_id;
#ifndef _DEBUG
	str.Format( L"Házasságok listázása..." );
	wndP.SetText( str );
#endif
	wndP.SetRange( 0, vSpouse1.size()-1 );
	wndP.SetPos(0 );
	for( int i = 0; i < vSpouse1.size(); ++i )
	{
		spouse2_id = vSpouse1.at(i).rowid;
		header.Format( L"%10s %14s %1s", vSpouse1.at(i).linenumber, L" ", vSpouse1.at(i).source );
		people.Format( L"%8s %-25s %12s %12s", vSpouse1.at(i).rowid, vSpouse1.at(i).name, vSpouse1.at(i).birth, vSpouse1.at(i).death );
		father.Format( L"%8s %-20s %12s %12s", vSpouse1.at(i).rowidF, vSpouse1.at(i).father, vSpouse1.at(i).birthF, vSpouse1.at(i).deathF );
		mother.Format( L"%8s %-20s %12s %12s", vSpouse1.at(i).rowidM, vSpouse1.at(i).mother, vSpouse1.at(i).birthM, vSpouse1.at(i).deathM );
		fwprintf( fh1, L"<font color='red'>%s %s %s %s</font><br>", header, people, father, mother );
//		fwprintf( fh1, L"<span style=\"background:yellow\">%s %s %s %s</span>\n", header, people, father, mother );
		for( int j = 0 ; j< vSpouse2.size(); ++j )
		{
			spouse1_id = vSpouse2.at(j).rowidS;
			if( spouse2_id == spouse1_id )
			{
				spouse1_id = vSpouse2.at(j).rowid;
				header.Format( L"%10s %1s %12s %1s", vSpouse2.at(j).linenumber,  vSpouse2.at(j).order, vSpouse2.at(j).date, vSpouse2.at(j).source );
				people.Format( L"%8s %-25s %12s %12s", vSpouse2.at(j).rowid, vSpouse2.at(j).name, vSpouse2.at(j).birth, vSpouse2.at(j).death );
				father.Format( L"%8s %-20s %12s %12s", vSpouse2.at(j).rowidF, vSpouse2.at(j).father, vSpouse2.at(j).birthF, vSpouse2.at(j).deathF );
				mother.Format( L"%8s %-20s %12s %12s", vSpouse2.at(j).rowidM, vSpouse2.at(j).mother, vSpouse2.at(j).birthM, vSpouse2.at(j).deathM );
				fwprintf( fh1, L"%s %s %s %s\n", header, people, father, mother );
			}
		}
		fwprintf( fh1, L"\n" );
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	fwprintf( fh1, L"</pre>" );
	fclose( fh1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::fillTable()
{
	CString rowid;
	CString rowidS;

#ifndef _DEBUG
	str.Format( L"(6/6)Házaspárok listázása..." );
	wndP.SetText( str );
#endif
	str = L"Emberek, akiknek több házasságuk volt: piros az ember, fekete a házastársa.        O - házasság sorszáma  S - az ember hierarchia száma ";
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );
	
	wndP.SetRange( 0, vSpouse1.size()-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	v_tableMarriages.clear();
	vColor.clear();
	for( int i = 0; i < vSpouse1.size(); ++i )
	{
		rowid = vSpouse1.at(i).rowid;
		push( vSpouse1.at(i).linenumber );
		push( L"" );							// order
		push( L"" );							// date
		push( L"" );							// sourceM
		push( vSpouse1.at(i).source );
		push( rowid );
		push( vSpouse1.at(i).name );
		push( vSpouse1.at(i).birth );
		push( vSpouse1.at(i).death );
		push( vSpouse1.at(i).rowidF );
		push( vSpouse1.at(i).father );
		push( vSpouse1.at(i).birthF );
		push( vSpouse1.at(i).deathF );
		push( vSpouse1.at(i).rowidM );
		push( vSpouse1.at(i).mother );
		push( vSpouse1.at(i).birthM );
		push( vSpouse1.at(i).deathM );
		vColor.push_back(1);
		for( int j = 0 ; j< vSpouse2.size(); ++j )
		{
			rowidS = vSpouse2.at(j).rowidS;
			if( rowidS == rowid )
			{
				push( vSpouse2.at(j).linenumber );
				push( vSpouse2.at(j).order );	
				push( vSpouse2.at(j).date );	
				push( vSpouse2.at(j).sourceM );
				push( vSpouse2.at(j).source );
				push( vSpouse2.at(j).rowid );
				push( vSpouse2.at(j).name );
				push( vSpouse2.at(j).birth );
				push( vSpouse2.at(j).death );
				push( vSpouse2.at(j).rowidF );
				push( vSpouse2.at(j).father );
				push( vSpouse2.at(j).birthF );
				push( vSpouse2.at(j).deathF );
				push( vSpouse2.at(j).rowidM );
				push( vSpouse2.at(j).mother );
				push( vSpouse2.at(j).birthM );
				push( vSpouse2.at(j).deathM );
				vColor.push_back(0);
			}
		}
		emptyRow();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.AttachDataset( &v_tableMarriages );

	SetForegroundWindow();  // kell, mert szûrés után csak így jön vissz az ablak!!!
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::fillTableF()
{
	CString rowid;
	CString rowidS1;
	CString rowidS2;
	CString name1;
	CString name2;

		m_ListCtrl.DeleteAllItems();
	for( int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i-- )
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ORDER,		L"O",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DATE,		L"esküvõ",		LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SOURCEM,		L"M",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_RIGHT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_RIGHT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_ROWIDF,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,		L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHF,		L"születés",	LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATHF,		L"halál",		LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ROWIDM,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,		L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHM,		L"születés",	LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATHM,		L"halál",		LVCFMT_RIGHT,	 80,-1,COL_TEXT);

	m_columnsCount	= m_ListCtrl.GetHeaderCtrl()->GetItemCount();


#ifndef _DEBUG
	str.Format( L"(6/6)Házaspárok listázása..." );
	wndP.SetText( str );
#endif
	str = L"Emberek, akiknek több házasságuk volt: piros az ember, fekete a házastársa.        O - házasság sorszáma  S - az ember hierarchia száma ";
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );
	
	wndP.SetRange( 0, vSpouse1.size()-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	v_tableMarriages.clear();
	vColor.clear();
	for( int i = 0; i < vSpouse1.size(); ++i )
	{
		rowid = vSpouse1.at(i).rowid;
		for( int j = 0 ; j< vSpouse2.size()-1; ++j )
		{
			rowidS1	= vSpouse2.at(j).rowidS;
			name1	= vSpouse2.at(j).name;
			rowidS2	= vSpouse2.at(j+1).rowidS;
			name2	= vSpouse2.at(j+1).name;
			if( rowidS1 == rowid && rowidS2 == rowid && name1 == name2 )
			{
				push( vSpouse1.at(i).linenumber );
				push( L"" );							// order
				push( L"" );							// date
				push( L"" );							// sourceM
				push( vSpouse1.at(i).source );
				push( rowid );
				push( vSpouse1.at(i).name );
				push( vSpouse1.at(i).birth );
				push( vSpouse1.at(i).death );
				push( vSpouse1.at(i).rowidF );
				push( vSpouse1.at(i).father );
				push( vSpouse1.at(i).birthF );
				push( vSpouse1.at(i).deathF );
				push( vSpouse1.at(i).rowidM );
				push( vSpouse1.at(i).mother );
				push( vSpouse1.at(i).birthM );
				push( vSpouse1.at(i).deathM );
				vColor.push_back(1);

				push2( j );
				push2( j + 1);
				emptyRow();
			}
		}
		
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.AttachDataset( &v_tableMarriages );

	SetForegroundWindow();  // kell, mert szûrés után csak így jön vissz az ablak!!!
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::push2( int i )
{	
	push( vSpouse2.at(i).linenumber );
	push( vSpouse2.at(i).order );	
	push( vSpouse2.at(i).date );	
	push( vSpouse2.at(i).sourceM );
	push( vSpouse2.at(i).source );
	push( vSpouse2.at(i).rowid );
	push( vSpouse2.at(i).name );
	push( vSpouse2.at(i).birth );
	push( vSpouse2.at(i).death );
	push( vSpouse2.at(i).rowidF );
	push( vSpouse2.at(i).father );
	push( vSpouse2.at(i).birthF );
	push( vSpouse2.at(i).deathF );
	push( vSpouse2.at(i).rowidM );
	push( vSpouse2.at(i).mother );
	push( vSpouse2.at(i).birthM );
	push( vSpouse2.at(i).deathM );
	vColor.push_back(0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::emptyRow()
{
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	vColor.push_back(0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::query1( CString command )
{
	if( m_recordset1->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::query3( CString command )
{
	if( m_recordset3->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByNameX(const SPOUSE11 &v1, const SPOUSE11 &v2) 
{ 
    return( v1.name < v2.name ); 
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;

	*pResult = 0;

	switch( pLVCD->nmcd.dwDrawStage )
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		nItem	= pLVCD->nmcd.dwItemSpec;
		if( vColor.at(nItem) == 1 )
		{
			pLVCD->clrText = RGB( 255,0,0 );
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::htmlHeader( CString title )
{
	fwprintf( fh1, L"<HEAD>\n" );
	fwprintf( fh1, L"<style>\n" );
	fwprintf( fh1, L"</style>\n" );
	fwprintf( fh1, L"</HEAD>\n" );
	fwprintf( fh1, L"<BODY>\n" );
	fwprintf( fh1, L"<center>%s</center><br><br>\n\n", title );
	fwprintf( fh1, m_explanation );
	fwprintf( fh1, L"<pre>" );
	fwprintf( fh1, L"\n%-20s %s<br>", L"Adatbázis:", theApp.m_databaseSpec );
	fwprintf( fh1, L"%-20s %s<br><br><br>\n", L"lista készült:", theApp.getPresentDateTime() );
	
	str.Format( L"\n%10s %1s %12s %1s %8s %-25s %12s %12s %8s %-20s %12s %12s %8s %-20s %12s %12s<br>\n",\
L"line#", L"O", L"esküvõ",L"S",\
L"rowid", L"name", L"születés", L"halál",\
L"rowid", L"apja", L"születés", L"halál",\
L"rowid", L"anyja", L"születés", L"halál"\
);
	fwprintf( fh1, str );
}




void CTableMarriages::OnManMorespouses()
{
	// TODO: Add your command handler code here
}
