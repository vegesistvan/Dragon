// DateFormat.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "checkDateFormat.h"
#include "afxdialogex.h"
#include "html_EditLines.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "Relations.h"
#include "html_EditLine.h"

enum
{
	L_ROWID = 0,
	L_LINENUMBER,
	L_DATE,
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDateFormat, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDateFormat::CDateFormat(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDateFormat::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDateFormat::~CDateFormat()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDateFormat, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CDateFormat::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CDateFormat::OnHtmlNotepad)
	ON_COMMAND(ID_DB_EDIT, &CDateFormat::OnDbEdit)

	ON_COMMAND(ID_UJLISTA, &CDateFormat::OnUjlista)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDateFormat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString info = L"\
Az alábbi dátum formátumokat fogadja el a program:\r\n\
1848\r\n\
1848.03.\r\n\
1848.03.15\r\n\r\n\
Az évnek 1000 és aktuáli sév közötti számnak kell lenni.\r\n\
A dátum elõtt a 'kb' módosító szó állhat.\r\n\
A dátum után a 'kb', 'elõtt', 'után', 'körül' módosító szavak valamelyike állhat.\
\r\n\r\n\
Az ezektõl eltérõ születési, halálozási és esküvõi dátumokat listázza a program.\
\r\n\r\n\
A jobb egérgombbal egy sorra kattintva, a legördülö menübõl választhatunk, \
hogy az adatbázisban vagy a GA.html fájlban módosítsuk a hibás dátumot.\
";

	GetDlgItem( IDC_EDIT )->SetWindowText( info );

/*
	if( AfxMessageBox( info, MB_OKCANCEL|MB_ICONINFORMATION ) == IDCANCEL )
	{
		OnCancel();
		return FALSE;
	}
*/
	createColumns();
	fillTable();

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::createColumns()
{
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",	LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DATE,		L"dátum",	LVCFMT_LEFT,	150,-1,COL_NUM);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::fillTable()
{
	CStringArray A;
	CString command;
	CString datum;
	int n;
	int length;
	int	nItem = 0;

	CProgressWnd wndP(NULL, L"Dátumok beolvasása..." );
	wndP.GoModal();

	command = L"SELECT rowid, linenumber, birth_date, death_date FROM people";
	if( !theApp.query( command ) ) return;
	
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

#ifndef _DEBUG
	wndP.SetText( L"Születési és halálozási dátumok beolvasásas..." );
#endif


	m_ListCtrl.DeleteAllItems();
	for( UINT i = 0 ; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		datum = theApp.m_recordset->GetFieldString( 2 );
		A.RemoveAll();
		n = wordList( &A, datum, ' ', FALSE );
		if( n > 1 )
		{
			if( A[1] == L"kb" || A[1] == L"elõtt" || A[1] == L"után" || A[1] == L"körül" )
				datum = A[0];
			if( A[0] == L"kb" )
				datum = A[1];
		}
		
		

		length = datum.GetLength();
		if( length && !checkDate( datum) )	// születési dátum
		{
			nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( 0 ) );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, theApp.m_recordset->GetFieldString( 1 ) );
			m_ListCtrl.SetItemText( nItem, L_DATE, theApp.m_recordset->GetFieldString( 2 ) );
		}


		datum = theApp.m_recordset->GetFieldString( 3 );
		A.RemoveAll();
		n = wordList( &A, datum, ' ', FALSE );
		if( n > 1 )
		{
			if( A[1] == L"kb" || A[1] == L"elõtt" || A[1] == L"után" || A[1] == L"körül" )
				datum = A[0];
			if( A[0] == L"kb" )
				datum = A[1];
		}


		length = datum.GetLength();
		if( length && !checkDate( datum) )		// halálozási dátum
		{
			nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( 0 ) );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, theApp.m_recordset->GetFieldString( 1 ) );
			m_ListCtrl.SetItemText( nItem, L_DATE, theApp.m_recordset->GetFieldString( 3 ) );
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

#ifndef _DEBUG
	wndP.SetText( L"Esküvõi dátumok beolvasásas..." );
#endif
	command = L"SELECT spouse1_id, linenumber, date FROM marriages";
	if( !theApp.query( command ) ) return;
	
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0 ; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		datum = theApp.m_recordset->GetFieldString( 3 );
		A.RemoveAll();
		n = wordList( &A, datum, ' ', FALSE );
		if( n > 1 )
		{
			if( A[1] == L"kb" || A[1] == L"elõtt" || A[1] == L"után" || A[1] == L"körül" )
				datum = A[0];
			if( A[0] == L"kb" )
				datum = A[1];
		}
	

		length = datum.GetLength();
		if( length && !checkDate( datum) )		// esküvõ dátuma
		{
			nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( 0 ) );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, theApp.m_recordset->GetFieldString( 1 ) );
			m_ListCtrl.SetItemText( nItem, L_DATE, theApp.m_recordset->GetFieldString( 2 ) );
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;
	CString rowid	= m_ListCtrl.GetItemText( nItem, L_ROWID );

	CRelations dlg;
	dlg.nItem		= nItem;
	dlg.m_rowid		= rowid;
	if( dlg.DoModal() == IDCANCEL ) return;

	*pResult = 0;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CDateFormat::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		pPopup->EnableMenuItem(ID_HTML_FAMILY, MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_PARENTS, MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_NOTEPAD_PARENTS, MF_BYCOMMAND | MF_GRAYED);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CHtmlEditLine dlg;
	dlg.m_linenumber	= m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelations dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnUjlista()
{
	fillTable();
}
