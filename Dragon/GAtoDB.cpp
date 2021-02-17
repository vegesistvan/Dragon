// GaToDb.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "GAtoDB.h"
#include "afxdialogex.h"
#include "utilities.h"

IMPLEMENT_DYNAMIC(CGaToDb, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaToDb::CGaToDb(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGaToDb::IDD, pParent)
{
	m_recordset		= new CSqliteDBRecordSet;
	REFRESH = FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaToDb::~CGaToDb()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaToDb::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMENT, colorComment);
	DDX_Control(pDX, IDC_STATIC_HTML, colorHtml);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGaToDb, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_EMPTY, &CGaToDb::OnClickedButtonEmpty)
	ON_STN_CLICKED(IDC_STATIC_HTML, &CGaToDb::OnClickedStaticHtml)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaToDb::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	colorComment.SetTextColor( RED );
	colorHtml.SetTextColor( theApp.m_colorClick );

	if( m_inputMode == GAHTML )
	 {
		SetWindowTextW( L"GA.html fájl beolvasása adatbázisba" );
		GetDlgItem( IDC_STATIC_HTML )->SetWindowTextW( L"GA.html:" );
		GetDlgItem( IDC_HTML )->SetWindowTextW( theApp.m_htmlFileSpec );
	 }
	else if( m_inputMode == GEDCOM )
	{
		SetWindowTextW( L"GEDCOM fájl beolvasása adatbázisba" );
		GetDlgItem( IDC_STATIC_HTML )->SetWindowTextW( L"GEDCOM:" );
	 	GetDlgItem( IDC_HTML )->SetWindowTextW( theApp.m_gedFileSpec );
	 }
	setDB();
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaToDb::setDB()
{
	GetDlgItem( IDC_DATABASE )->SetWindowTextW( theApp.m_databaseSpec );
	m_command = L"SELECT count() FROM people";
	if( !query( m_command ) ) return FALSE;

	int n = _wtoi( m_recordset->GetFieldString( 0 ) );
	if( n )
	{
		str.Format( L"Az adatbázisban %d ember van nyilvántartva!", n );
		GetDlgItem( IDC_BUTTON_EMPTY )->EnableWindow( TRUE );
	}
	else
	{
		m_command = L"SELECT count() FROM people";
		if( !query( m_command ) ) return FALSE;
		if( _wtoi( m_recordset->GetFieldString( 0 ) ) == 0 )
			str = L"Az adatbázis üres.";
		else
			str = L"törölni kell az adatbázis!";
	}
	GetDlgItem( IDC_COMMENT )->SetWindowTextW( str );
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaToDb::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az adatbázis törlése
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaToDb::OnClickedButtonEmpty()
{
	GetDlgItem( IDC_BUTTON_EMPTY )->EnableWindow( FALSE );
	GetDlgItem( IDOK )->EnableWindow( FALSE );
	GetDlgItem( IDCANCEL )->EnableWindow( FALSE );

	theApp.clearDatabase();
	setCreationTime( theApp.m_databaseSpec );

	GetDlgItem( IDOK )->EnableWindow( TRUE );
	GetDlgItem( IDCANCEL )->EnableWindow( TRUE );
	GetDlgItem( IDOK )->EnableWindow( TRUE );
	str = L"Az adatbázis üres.";
	GetDlgItem( IDC_COMMENT )->SetWindowTextW( str );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GA.html vagy GEDCOM fájl kiválasztása
void CGaToDb::OnClickedStaticHtml()
{
	if( m_inputMode == GAHTML )
	{
		if( theApp.selectHtml( FALSE ) )
			GetDlgItem( IDC_HTML )->SetWindowTextW( theApp.m_htmlFileSpec );
	}
	else
	{
		if( theApp.selectGedcom( FALSE) )
			GetDlgItem( IDC_HTML )->SetWindowTextW( theApp.m_gedFileSpec );
	}
	setDB();
}
