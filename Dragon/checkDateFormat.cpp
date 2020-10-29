// DateFormat.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "checkDateFormat.h"
#include "afxdialogex.h"
#include "html_Lines.h"
#include "ProgressWnd.h"
#include "utilities.h"

enum
{
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
//	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CDateFormat::OnHtmlEdit)
	ON_COMMAND(ID_HTML_SHOWS, &CDateFormat::OnHtmlShows)
	ON_COMMAND(ID_HTML_NOTEPAD, &CDateFormat::OnHtmlNotepad)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CDateFormat::OnClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CDateFormat::OnDblclkList)
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
A dátum elõtt a 'kb' módosító szó állhat.\r\n\
A dátum után a 'kb', 'elõtt', 'után', 'körül' módosító szavak valamelyike állhat.\
\r\n\r\n\
Az ezektõl eltérõ születési, halálozási és esküvõi dátumokat listázza a program.\
\r\n\r\n\
A bal egérgombbal egy sorra kattintva, a html fájl megfelelõ sora szerkeszthetõ.\
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

	command = L"SELECT linenumber, birth_date, death_date FROM people";
	if( !theApp.query( command ) ) return;
	
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

#ifndef _DEBUG
	wndP.SetText( L"Születési és halálozási dátumok beolvasásas..." );
#endif



	for( UINT i = 0 ; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		datum = theApp.m_recordset->GetFieldString( 1 );
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

//		if( length != 0 && length != 4 && length != 8 && length != 10 )
		{
			nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( 0 ) );
			m_ListCtrl.SetItemText( nItem, L_DATE, theApp.m_recordset->GetFieldString( 1 ) );
		}



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
		if( length && !checkDate( datum) )		// halálozási dátum
//		if( length != 0 && length != 4 && length != 8  && length != 10 )
		{
			nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( 0 ) );
			m_ListCtrl.SetItemText( nItem, L_DATE, theApp.m_recordset->GetFieldString( 2 ) );
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

#ifndef _DEBUG
	wndP.SetText( L"Esküvõi dátumok beolvasásas..." );
#endif
	command = L"SELECT linenumber, date FROM marriages";
	if( !theApp.query( command ) ) return;
	
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0 ; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		datum = theApp.m_recordset->GetFieldString( 1 );
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
//		if( length != 0 && length != 4 && length != 8 && length != 10 )
		{
			nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( 0 ) );
			m_ListCtrl.SetItemText( nItem, L_DATE, theApp.m_recordset->GetFieldString( 1 ) );
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	OnHtmlEdit();

	*pResult = 0;
}
void CDateFormat::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnHtmlEdit();
	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDateFormat::OnHtmlShows()
{
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int			nItem;
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");

	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		vLines.push_back( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) );
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnHtmlNotepad()
{

	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
