// GetLastFirst.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "GetLastFirst.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include <string.h>
#include <vector>

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
Az al�bbiak szerint adhatunk meg nevet:\r\n\r\n\
1. �resen hagyjuk a nevet, ami az �sszes ember el��r�s�t jelenit.\r\n\r\n\
2. Csal�dn�v tetsz�leges sz�m� kezd�bet�it adjuk meg\r\n\
3. A pontos csal�dnevet adjuk meg\r\n\r\n\
4. Csal�dnevet �s egy vagy k�t keresztnevet adunk meg.\r\n\
");



	GetDlgItem( IDC_INFO )->SetWindowTextW( info );

	_fullname = theApp.GetProfileString( L"dragon", L"fullname", L"M�rocz J�zsef" );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( _fullname );

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGetLastFirst::OnOK()
{
	_lastname.Empty();
	_firstname.Empty();

	CString alma( L"Aba  �stv�n  �rzse" );

	std::vector< CString > nevek;

	GetDlgItem( IDC_EDIT )->GetWindowTextW( _fullname );
	if( !_fullname.IsEmpty() )
	{
		CProgressWnd wndProgress(NULL,L"N�v ellen�rz�se...." ); 
		wndProgress.GoModal();


		CStringArray A;
		int n;

		_sex_id.Empty();

		n = splitCString( _fullname, ' ', false, &A );

		splitLine( alma, ' ', false, &nevek );


		_lastname	= A[0];
		_firstname.Empty();
		if( n > 1 )
		{
			_sex_id = theApp.isFirstName( A[1] );
			if( _sex_id.IsEmpty() )
			{
				str.Format( L"'%s' nem keresztn�v!", A[1] );
				AfxMessageBox( str );
				wndProgress.DestroyWindow();
				return;
			}
			_firstname	= A[1];

			if( n > 2 )
			{
				_sex_id = theApp.isFirstName( A[2] );
				if( _sex_id.IsEmpty() )
				{
					str.Format( L"'%s' nem keresztn�v!", A[2] );
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
			str.Format( L"'%s' nev� ember nincs az adatb�zisban!", _fullname );
			AfxMessageBox( str );
			wndProgress.DestroyWindow();
			return;
		}
		theApp.WriteProfileStringW( L"dragon", L"fullname", _fullname );
		wndProgress.DestroyWindow();
	}
	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
