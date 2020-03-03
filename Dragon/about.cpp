#include "stdafx.h"
#include "Fa.h"
#include "FaDlg.h"
#include <stdio.h>
#include "about.h"
#include "version_num.h"
#include "build_defs.h"

// want something like: 1.4.1432.2234

const unsigned char completeVersion[] =
{
    VERSION_MAJOR_INIT,
    '.',
    VERSION_MINOR_INIT,
    '-', 'V', '-',
    BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
    '-',
    BUILD_MONTH_CH0, BUILD_MONTH_CH1,
    '-',
    BUILD_DAY_CH0, BUILD_DAY_CH1,
    ' ',
    BUILD_HOUR_CH0, BUILD_HOUR_CH1,
    ':',
    BUILD_MIN_CH0, BUILD_MIN_CH1,
    ':',
    BUILD_SEC_CH0, BUILD_SEC_CH1,
    '\0'
};
/*
const unsigned char build[] =
{
    BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
    '-',
    BUILD_MONTH_CH0, BUILD_MONTH_CH1,
    '-',
    BUILD_DAY_CH0, BUILD_DAY_CH1,
    ' ',
    BUILD_HOUR_CH0, BUILD_HOUR_CH1,
    ':',
    BUILD_MIN_CH0, BUILD_MIN_CH1,
    ':',
    BUILD_SEC_CH0, BUILD_SEC_CH1,
    '\0'
};
*/




CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(ID_USERMANUAL, &CAboutDlg::OnClickedUsermanual)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString BUILT;
	CString command;
	CString sqliteSourceId;
	CString sqliteVersion;

	static const char *built = __DATE__" "__TIME__;
	str = MultiToUnicode( built );
	CString	year;
	CString month;
	CString day;
	CString time;
	CStringArray A;

	wordList( &A, str, ' ', TRUE );
/*
	month	= getWord( str, 1 );
	day		= getWord( str, 2 );
	year	= getWord( str, 3 );
	time	= getWord( str, 4 );
*/

//	BUILT.Format( L"%s. %s. %s  %s", A[2], A[0], A[1], A[3] );


	command = L"SELECT sqlite_source_id()";
	if( !theApp.query( command ) ) return FALSE;
	sqliteSourceId = theApp.m_recordset->GetFieldString(0);
	command = L"SELECT sqlite_version()";
	if( !theApp.query( command ) ) return FALSE;
	sqliteVersion	= theApp.m_recordset->GetFieldString(0);
	str.Format( L"%s - %s", sqliteVersion, sqliteSourceId );

	BUILT.Format( L"%s - %s", VERSION, MultiToUnicode( LPCSTR( build ) ) );
//	BUILT = VERSION;
//	BUILT += L" - ";
//	BUILT += LPCSTR(build);
//	CString str1;
//	str1 = completeVersion;
//	GetDlgItem( IDC_STATIC_BUILT )->SetWindowText( str1 );
	BUILT.Trim();
	GetDlgItem( IDC_STATIC_BUILT )->SetWindowText( BUILT );
	GetDlgItem( IDC_STATIC_SQLITE )->SetWindowText( str );
	GetDlgItem( IDC_PROGRAM )->SetWindowText( theApp._startFileSpec );
	GetDlgItem( IDC_PLATFORM )->SetWindowText( PLATFORM );

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnClickedUsermanual()
{
	if( _waccess( theApp.m_helpFileSpec, 0 ) )
	{
		AfxMessageBox( L"Nincs a faHelp.chm fájl a program könyvtárában!" );
		return;
	}
	else
	{
		ShowWindow( SW_HIDE );
		ShellExecute(NULL, L"open", theApp.m_helpFileSpec,NULL, NULL, SW_SHOWNORMAL);
		ShowWindow( SW_RESTORE );
	}



}
