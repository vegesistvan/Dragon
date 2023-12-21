// Date.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Date.h"
#include "afxdialogex.h"


// CDate dialog

IMPLEMENT_DYNAMIC(CDate, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDate::CDate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDate::IDD, pParent)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDate::~CDate()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimeCtrl);
	//  DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_datum);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDate, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDate::OnClickedButtonClear)
	ON_BN_CLICKED(IDOK, &CDate::OnBnClickedOk)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDate::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText( m_caption );
	int	year = 1800;
	int	month = 1;
	int	day =1;

	if( m_datum.GetLength() == 10 )
	{
		year = _wtoi( m_datum.Left(4) );
		month= _wtoi( m_datum.Mid( 5,2 ) );
		day	 = _wtoi( m_datum.Right(2 ) );
	}
		

	
	memset(&sysTime, 0, sizeof(sysTime));
	sysTime.wYear	= WORD(year);
	sysTime.wMonth	= WORD(month);
	sysTime.wDay	= WORD(day);
	VERIFY(m_DateTimeCtrl.SetTime(&sysTime));

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDate::OnClickedButtonClear()
{
	m_datum = L"";
	OnOK();
}
void CDate::OnBnClickedOk()
{
	m_DateTimeCtrl.GetTime(&sysTime);
	m_datum.Format( L"%4d.%02d.%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay );
	CDialogEx::OnOK();
}
