// DateFormat.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkDateFormat.h"
#include "afxdialogex.h"
#include "editHtmlLines.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "editHtmlLine.h"
#include "Relatives.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckDateFormat()
//{
//	CDateFormat dlg;
//	dlg.m_massage = true;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	wchar_t* info = L"\
Az al�bbi d�tum form�tumokat fogadja el a program:\r\n\
1848\r\n\
1848.03.\r\n\
1848.03.15\r\n\r\n\
Az �vnek 1000 �s aktu�li s�v k�z�tti sz�mnak kell lenni.\r\n\
A d�tum el�tt a 'kb' m�dos�t� sz� �llhat.\r\n\
A d�tum ut�n a 'kb', 'el�tt', 'ut�n', 'k�r�l' m�dos�t� szavak valamelyike �llhat.\
\r\n\r\n\
Az ezekt�l elt�r� sz�let�si, hal�loz�si �s esk�v�i d�tumokat list�zza a program.\
\r\n\r\n\
A jobb eg�rgombbal egy sorra kattintva, a leg�rd�l� men�b�l v�laszthatunk, \
hogy az adatb�zisban vagy a GA.html f�jlban m�dos�tsuk a hib�s d�tumot.\
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

	if( !m_ListCtrl.GetItemCount() )
	{
		AfxMessageBox( L"Minden d�tum form�tuma helyes!", MB_ICONINFORMATION );
		OnCancel();
	}

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
	m_ListCtrl.InsertColumn( L_DATE,		L"d�tum",	LVCFMT_LEFT,	150,-1,COL_NUM);
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

	CProgressWnd wndP(NULL, L"D�tumok beolvas�sa..." );
	wndP.GoModal();

	command = L"SELECT rowid, linenumber, birth_date, death_date FROM people";
	if( !theApp.query( command ) ) return;
	
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

#ifndef _DEBUG
	wndP.SetText( L"Sz�let�si �s hal�loz�si d�tumok beolvas�sas..." );
#endif


	m_ListCtrl.DeleteAllItems();
	for( UINT i = 0 ; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		datum = theApp.m_recordset->GetFieldString( 2 );
		A.RemoveAll();
		n = splitCString( datum, ' ', false,  &A );
		if( n > 1 )
		{
			if( A[1] == L"kb" || A[1] == L"el�tt" || A[1] == L"ut�n" || A[1] == L"k�r�l" )
				datum = A[0];
			if( A[0] == L"kb" )
				datum = A[1];
		}
		
		

		length = datum.GetLength();
		if( length && !checkDate( datum) )	// sz�let�si d�tum
		{
			nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( 0 ) );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, theApp.m_recordset->GetFieldString( 1 ) );
			m_ListCtrl.SetItemText( nItem, L_DATE, theApp.m_recordset->GetFieldString( 2 ) );
		}


		datum = theApp.m_recordset->GetFieldString( 3 );
		A.RemoveAll();
		n = splitCString( datum, ' ', false, &A );
		if( n > 1 )
		{
			if( A[1] == L"kb" || A[1] == L"el�tt" || A[1] == L"ut�n" || A[1] == L"k�r�l" )
				datum = A[0];
			if( A[0] == L"kb" )
				datum = A[1];
		}


		length = datum.GetLength();
		if( length && !checkDate( datum) )		// hal�loz�si d�tum
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
	wndP.SetText( L"Esk�v�i d�tumok beolvas�sas..." );
#endif
	command = L"SELECT husband_id, linenumber, date FROM marriages";
	if( !theApp.query( command ) ) return;
	
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0 ; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		datum = theApp.m_recordset->GetFieldString( 3 );
		A.RemoveAll();
		n = splitCString( datum, ' ', false, &A );
		if( n > 1 )
		{
			if( A[1] == L"kb" || A[1] == L"el�tt" || A[1] == L"ut�n" || A[1] == L"k�r�l" )
				datum = A[0];
			if( A[0] == L"kb" )
				datum = A[1];
		}
	

		length = datum.GetLength();
		if( length && !checkDate( datum) )		// esk�v� d�tuma
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CDateFormat::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		pPopup->EnableMenuItem(ID_HTML_FATHERANDSIBLINGS, MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_NOTEPAD_PARENTS, MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_CHILDREN, MF_BYCOMMAND | MF_GRAYED);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CEditHtmlLine dlg;
	dlg.m_linenumber	= m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDateFormat::OnUjlista()
{
	fillTable();
}
