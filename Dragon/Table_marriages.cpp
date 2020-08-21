// Table_marriages.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Table_marriages.h"
#include "afxdialogex.h"
#include "html_Edit.h"
#include "Filter.h"
#include "DragonDlg.h"
#include "SelectOne.h"
#include "GetString.h"
#include "html_Lines.h"
#include "GetLastFirst.h"
#include "EditMarriage.h"
#include "windows.h"

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
	LIST_ORDERHUSBAND,
	LIST_SEPARATOR,
	LIST_SPOUSE2_ID,
	LIST_WLASTNAME,
	LIST_WFIRSTNAME,
	LIST_WBIRTHDATE,
	LIST_WDEATHDATE,
	LIST_ORDERWIFE,
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTableMarriages, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableMarriages::CTableMarriages(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableMarriages::IDD, pParent)
{
	m_pParent = pParent;

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
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableMarriages, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(ID_FILTER_HLASTNAME, &CTableMarriages::OnFilterHlastname)
	ON_EN_CHANGE(IDC_SEARCH, &CTableMarriages::OnChangeSearch)
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
	ON_COMMAND(ID_HTML_EDIT, &CTableMarriages::OnHtmlEdit)
	ON_COMMAND(ID_HTML_SHOWS, &CTableMarriages::OnHtmlShows)
	ON_COMMAND(ID_HTML_NOTEPAD, &CTableMarriages::OnHtmlNotepad)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTableMarriages::OnDblclkList)
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
	m_ListCtrl.InsertColumn( LIST_ORDERHUSBAND,	L"# házasság",	LVCFMT_RIGHT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LIST_SEPARATOR,	L"",			LVCFMT_LEFT,	 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_SPOUSE2_ID,	L"spouse2_id",	LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LIST_WLASTNAME,	L"feleség",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_WFIRSTNAME,	L"",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_WBIRTHDATE,	L"született",	LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_WDEATHDATE,	L"meghalt",		LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LIST_ORDERWIFE,	L"# házasság",	LVCFMT_RIGHT,	 70,-1,COL_NUM);
	

	m_columnsCount	= m_ListCtrl.GetHeaderCtrl()->GetItemCount();

	m_orderix = 0;

	enableMenu( MF_GRAYED );

	m_command.Format( L"SELECT count(*) FROM marriages" );
	if( !theApp.query( m_command ) ) return false;

	int cnt = _wtoi(theApp.m_recordset->GetFieldString( 0 ));
	if( cnt && cnt < 10000 )
//	if( theApp.m_cntMarriages && theApp.m_cntMarriages  < 10000 )
		OnUnfilter();


	return TRUE;
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
	
	
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		
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
		push( theApp.m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND ) );
		push( L" - " );
		push( theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID ) );
		push( last_name2 );
		push( theApp.m_recordset2->GetFieldString( 2 ) );
		push( theApp.m_recordset2->GetFieldString( 3 ) );
		push( theApp.m_recordset2->GetFieldString( 4 ) );
		push( theApp.m_recordset->GetFieldString( MARRIAGES_ORDERWIFE ) );

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.SetItemCountEx( theApp.m_recordset->RecordsCount() );
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
void CTableMarriages::OnChangeSearch()
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.search( search, m_orderix,  &m_ListCtrl );
}
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
BOOL CTableMarriages::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
		}
	}
    return CWnd::PreTranslateMessage(pMsg);
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
LRESULT CTableMarriages:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	LIST_LINENUMBER ) );
	if( lineNumber == 0  )
		return TRUE;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_SHOWS, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_NOTEPAD, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	LIST_LINENUMBER ) );


	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	LIST_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
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
		vLines.push_back( m_ListCtrl.GetItemText( nItem, LIST_LINENUMBER ) );
		if( name.Compare( m_ListCtrl.GetItemText( nItem, LIST_HFIRSTNAME ) ) )
		{
			name = m_ListCtrl.GetItemText( nItem, LIST_HFIRSTNAME );
			++cnt;
		}
	

	}

	CHtmlLines dlg;

	if( cnt == 1 )
		dlg.child = name;
	else
		dlg.child = L"";

	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	int nItem = pNMItemActivate->iItem;

	ShowWindow( SW_HIDE );
	CEditMarriage dlg;
	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, LIST_ROWID );
	dlg.DoModal();
	ShowWindow( SW_RESTORE );
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
		if(Menu.LoadMenu( IDR_DROPDOWN_MARRIAGE ))
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
	CHtmlEdit dlg;

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
	CString cLine;
	
	for( int i = 0; i < htmlLineNumber; ++i ) file.ReadString( cLine );

	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
	if( cLine.GetAt(0) != '%' || cLine.Left(3) == L"%%%" )
	{
		cLine = cleanHtmlLine( cLine );
	}
	dlg.m_line = cLine;
	dlg.DoModal();

}