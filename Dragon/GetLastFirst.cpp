// GetLastFirst.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "GetLastFirst.h"
#include "afxdialogex.h"


// CGetLastFirst dialog

IMPLEMENT_DYNAMIC(CGetLastFirst, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGetLastFirst::CGetLastFirst(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGetLastFirst::IDD, pParent)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGetLastFirst::~CGetLastFirst()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGetLastFirst::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGetLastFirst, CDialogEx)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGetLastFirst::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString info( L"\
Az alábbiak szerint adhatunk meg nevet:\r\n\r\n\
1. Üresen hagyjuk a nevet, ami az összes ember elõírását jelenit.\r\n\r\n\
2. Családnév tetszõleges számú kezdõbetûit adjuk meg\r\n\
3. A pontos családnevet adjuk meg\r\n\r\n\
4. Családnevet és egy vagy két keresztnevet adunk meg.\r\n\
");



	GetDlgItem( IDC_INFO )->SetWindowTextW( info );

	_fullname = theApp.GetProfileString( L"Settings", L"fullname", L"Mórocz József" );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( _fullname );

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGetLastFirst::OnOK()
{
	_lastname.Empty();
	_firstname.Empty();

	GetDlgItem( IDC_EDIT )->GetWindowTextW( _fullname );
	if( !_fullname.IsEmpty() )
	{
		CProgressWnd wndProgress(NULL,L"Név ellenõrzése...." ); 
		wndProgress.GoModal();


		CStringArray A;
		int n;

		_sex_id = 0;

		n = wordList( &A, _fullname, ' ',FALSE );

		_lastname	= A[0];
		_firstname.Empty();
		if( n > 1 )
		{
			_sex_id = theApp.isFirstName( A[1] );
			if( _sex_id == -1 )
			{
				str.Format( L"'%s' nem keresztnév!", A[1] );
				AfxMessageBox( str );
				wndProgress.DestroyWindow();
				return;
			}
			_firstname	= A[1];

			if( n > 2 )
			{
				_sex_id = theApp.isFirstName( A[2] );
				if( _sex_id == -1 )
				{
					str.Format( L"'%s' nem keresztnév!", A[2] );
					AfxMessageBox( str );
					wndProgress.DestroyWindow();
					return;
				}
				_firstname += L" ";
				_firstname += A[2];
			}
		}
		if( _firstname.IsEmpty() )
		{
			m_command.Format( L"SELECT count() FROM people WHERE last_name LIKE '%s%c'", _lastname,'%' );
		}
		else
		{
			m_command.Format( L"SELECT count() FROM people WHERE first_name='%s' AND last_name='%s'", _firstname, _lastname );
		}
		if( !theApp.query( m_command ) ) return;

		int count = _wtoi( theApp.m_recordset->GetFieldString(0) );
		if( !count )
		{
			str.Format( L"'%s' nevû ember nincs az adatbázisban!", _fullname );
			AfxMessageBox( str );
			wndProgress.DestroyWindow();
			return;
		}
		theApp.WriteProfileStringW( L"Settings", L"fullname", _fullname );
		wndProgress.DestroyWindow();
	}
	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////