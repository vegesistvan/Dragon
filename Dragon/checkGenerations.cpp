// CheckGenerations.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckGenerations.h"
#include "afxdialogex.h"
#include "html_EditLines.h"
#include "Relations.h"
#include "ProgressWnd.h"

enum 
{
	L_CNT = 0,
	L_TABLEID,
	L_TABLE,
	L_LINE,
	L_G,
	L_ROWID,
	L_NAME,
};
enum
{
	G_TABLENUMBER = 0,
	G_LINENUMBER,
	G_GENERATION,
	G_ROWID,
	G_LAST_NAME,
	G_FIRST_NAME
};


IMPLEMENT_DYNAMIC(CCheckGenerations, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckGenerations::CCheckGenerations(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckGenerations::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckGenerations::~CCheckGenerations()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckGenerations::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckGenerations, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckGenerations::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckGenerations::OnHtmlNotepad)
	ON_COMMAND(ID_DB_EDIT, &CCheckGenerations::OnDbEdit)

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckGenerations::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT()

	CString info = L"\
A program az adatbázisban lévõ táblák egymást követõ leszármazottainak generációs kódjait ellenõrzi. Az alábbi esetekben jelez hibát:\r\n\r\n\
-ha  egy leszármazott generációs kódja kisebb, mint a tábla õsének generációs kódja,\r\n\
-ha egy leszármazott generációs kódja nem egyet lépett a szülõhöz képest.\r\n\r\n\
A hiba lehet a html fájlban vagy oka lehet az emberek hibás azonosítása miatt tévesen egyesített emerek.\
\r\n\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );
	createColumns();
	fillTable();

	return TRUE;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckGenerations::createColumns()
{
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,		L"cnt",		LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_TABLEID,	L"table#",	LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLE,	L"table",	LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LINE,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_G,		L"G",		LVCFMT_LEFT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ROWID,	L"rowid",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,	L"name",	LVCFMT_LEFT,	200,-1,COL_TEXT);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckGenerations::fillTable()
{
	CProgressWnd wndProgress(NULL, L"Generációs kódok ellenõrzése..." ); 
	wndProgress.GoModal();
#ifndef _DEBUG
	wndProgress.SetText( L"Generációs kódok beolvasása folyik..." );
#endif
	m_command = L"SELECT tableNumber, lineNumber, generation, rowid, last_name, first_name FROM people WHERE source == 1 ORDER BY lineNumber";
	if( !theApp.query( m_command ) ) return;

	wndProgress.SetRange(0, theApp.m_recordset->RecordsCount()*2);
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);
#ifndef _DEBUG
	wndProgress.SetText( L"Generációs kódok ellenõrzése..." );
#endif

	CString tableName;
	CString tableNumber(L"");

	CString firstRowid;
	CString firstLineNumber;
	CString firstGeneration(L"");

	CString rowid;
	CString	lineNumber(L"");
	CString generation;

	CString name1;
	CString name2;

	CString prevRowid;
	CString prevLineNumber;
	CString prevGeneration;

	BOOL FIRST = TRUE;

	int diff;
	int nItem = 0;
	m_cnt = 0;
	
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid	= theApp.m_recordset->GetFieldString( G_ROWID );
		lineNumber	= theApp.m_recordset->GetFieldString( G_LINENUMBER );
		generation	= theApp.m_recordset->GetFieldString( G_GENERATION );
		name2.Format( L"%s %s", theApp.m_recordset->GetFieldString(G_LAST_NAME), theApp.m_recordset->GetFieldString(G_FIRST_NAME) );

		if( tableNumber.Compare( theApp.m_recordset->GetFieldString( G_TABLENUMBER ) ) )  // új tábla kezdõdik
		{
			tableNumber	= theApp.m_recordset->GetFieldString( G_TABLENUMBER );
			m_command.Format( L"SELECT tableheader FROM tables WHERE rowid == '%s'", tableNumber );
			if( !theApp.query1( m_command ) ) return;
			tableName	= theApp.m_recordset1->GetFieldString( 0 );

			firstLineNumber	= theApp.m_recordset->GetFieldString( G_LINENUMBER );
			firstRowid	= theApp.m_recordset->GetFieldString( G_ROWID );
			name1.Format( L"%s %s", theApp.m_recordset->GetFieldString(G_LAST_NAME), theApp.m_recordset->GetFieldString(G_FIRST_NAME) );
			firstGeneration	= generation;
		}
		else
		{
			if( generation < firstGeneration )
			{
				str.Format( L"%d", nItem + 1 );
				nItem = m_ListCtrl.InsertItem( nItem, str );
				m_ListCtrl.SetItemText( nItem, L_TABLEID, tableNumber );
				m_ListCtrl.SetItemText( nItem, L_TABLE, tableName );
				m_ListCtrl.SetItemText( nItem, L_LINE, firstLineNumber );
				m_ListCtrl.SetItemText( nItem, L_G, firstGeneration );
				m_ListCtrl.SetItemText( nItem, L_ROWID, firstRowid );
				m_ListCtrl.SetItemText( nItem, L_NAME, name1 );
				++nItem;
				++m_cnt;


//				fwprintf( fh1, L"%-30s %8sT %8sL %8sR %s %s\n", tableName.Left(30), tableNumber, firstLineNumber, firstRowid, firstGeneration, name1  );
//				fwprintf( fh1, L"%49sL %8sR %s %s<br><br>",  lineNumber, rowid, generation, name2 );
				
				nItem = m_ListCtrl.InsertItem( nItem, L"" );
				m_ListCtrl.SetItemText( nItem, L_LINE, lineNumber );
				m_ListCtrl.SetItemText( nItem, L_G, generation );
				m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
				m_ListCtrl.SetItemText( nItem, L_NAME, name2 );
				++nItem;
				++m_cnt;
				nItem = m_ListCtrl.InsertItem( nItem, str );
				++nItem;
			}
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	nItem = m_ListCtrl.InsertItem( nItem, L"" );
	++nItem;

	// generációs kód egyesével csökken?
	theApp.m_recordset->MoveFirst();
	tableNumber.Empty();

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid	= theApp.m_recordset->GetFieldString( G_ROWID );
		lineNumber	= theApp.m_recordset->GetFieldString( G_LINENUMBER );
		generation	= theApp.m_recordset->GetFieldString( G_GENERATION );
		
		if( tableNumber.Compare( theApp.m_recordset->GetFieldString( G_TABLENUMBER ) ) )  // új tábla kezdõdik
		{
			tableNumber	= theApp.m_recordset->GetFieldString( G_TABLENUMBER );
			m_command.Format( L"SELECT tableheader FROM tables WHERE rowid == '%s'", tableNumber );
			if( !theApp.query1( m_command ) ) return;
			tableName	= theApp.m_recordset1->GetFieldString( 0 );
		}
		else
		{
			if( generation > prevGeneration )
			{
				diff = (int)generation[0] - (int)prevGeneration[0];
				if( diff != 1 && diff != 7  )   // diff = 7, ha Z-a generációs kódok követik egymást
				{
					str.Format( L"%d", nItem + 1 );
					nItem = m_ListCtrl.InsertItem( nItem, str );
					m_ListCtrl.SetItemText( nItem, L_TABLEID, tableNumber );
					m_ListCtrl.SetItemText( nItem, L_TABLE, tableName );
					m_ListCtrl.SetItemText( nItem, L_LINE, prevLineNumber );
					m_ListCtrl.SetItemText( nItem, L_ROWID, prevRowid );
					m_ListCtrl.SetItemText( nItem, L_G, prevGeneration );
					
					++nItem;
					nItem = m_ListCtrl.InsertItem( nItem, L"" );
					m_ListCtrl.SetItemText( nItem, L_LINE, lineNumber );
					m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
					m_ListCtrl.SetItemText( nItem, L_G, generation );
					++nItem;
					++m_cnt;

					nItem = m_ListCtrl.InsertItem( nItem, L"" );
					++nItem;

//					fwprintf( fh1, L"%-30s %8sT ", tableName.Left(30), tableNumber );
//					fwprintf( fh1, L"%8sL %8sR %s<br>", prevLineNumber, prevRowid, prevGeneration );
//					fwprintf( fh1, L"%49sL %8sR %s<br><br>", lineNumber, rowid, generation );




				}
			}
		}
		prevRowid		= rowid;
		prevLineNumber	= lineNumber;
		prevGeneration	= generation;

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	if( !m_cnt )
	{
		AfxMessageBox( L"Nem találtam hibás generációs kódot!" );
		CDialog::OnCancel();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckGenerations::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCheckGenerations::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckGenerations:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckGenerations::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINE );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckGenerations::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_NAME ), m_ListCtrl.GetItemText( nItem, L_LINE )  );
	else
		title.Format( L"%d kijelölt sor a ga.html fájlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINE, title );


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckGenerations::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelations dlg;

	dlg.m_rowid = rowid;
	dlg.DoModal();
}

