// EditMarriage.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "EditMarriage.h"
#include "afxdialogex.h"


// CEditMarriage dialog

IMPLEMENT_DYNAMIC(CEditMarriage, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditMarriage::CEditMarriage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditMarriage::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditMarriage::~CEditMarriage()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditMarriage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEditMarriage, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditMarriage::OnBnClickedOk)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditMarriage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if( m_rowid.IsEmpty() )
	{
		m_command.Format( L"SELECT rowid FROM marriages WHERE spouse1_id ='%s' AND spouse2_id='%s'", m_rowidH, m_rowidW );
		if( !theApp.query( m_command ) ) return false;
		m_rowid = theApp.m_recordset->GetFieldString( 0 );
		if( m_rowid.IsEmpty() )
		{
			AfxMessageBox( L"Nincs hátzasság!" );
			return false;
		}
	}







	m_command.Format( L"SELECT rowid,* FROM marriages WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return false;

	CString rowidH;
	CString rowidW;

	rowidH = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
	rowidW = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );

	GetDlgItem( IDC_HUSBAND )->SetWindowTextW( getSpouse( rowidH ) );
	GetDlgItem( IDC_WIFE )->SetWindowTextW( getSpouse( rowidW ) );

	m_place = theApp.m_recordset->GetFieldString( MARRIAGES_PLACE );
	m_date = theApp.m_recordset->GetFieldString( MARRIAGES_DATE );

	GetDlgItem( IDC_EDIT_PLACE )->SetWindowTextW( m_place );
	GetDlgItem( IDC_EDIT_DATE )->SetWindowTextW( m_date );

	m_orderWife		= theApp.m_recordset->GetFieldString( MARRIAGES_ORDERWIFE );
	m_orderHusband	= theApp.m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND );

	GetDlgItem( IDC_EDIT_ORDERWIFE )->SetWindowTextW( m_orderWife );
	GetDlgItem( IDC_EDIT_ORDERHUSBAND )->SetWindowTextW( m_orderHusband );

	GetDlgItem( IDOK )->SetFocus();

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditMarriage::getSpouse( CString rowid )
{
	CString spouse;
	CString birth;
	CString death;
	CString birth_death;

	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'", rowid );
	if( !theApp.query1( m_command ) ) return false;
	
	str.Format( L"%s %s", theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_PLACE ), theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_DATE ) );
	str.Trim();
	if( !str.IsEmpty() ) birth.Format( L"*%s", str );
	
	str.Format( L"%s %s", theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_PLACE ), theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_DATE ) );
	str.Trim();
	if( !str.IsEmpty() ) death.Format( L"+%s", str );

	birth_death.Format( L"%s %s", birth, death ); 

	spouse.Format( L"%s %s %s", theApp.m_recordset1->GetFieldString( PEOPLE_LAST_NAME ), theApp.m_recordset1->GetFieldString( PEOPLE_FIRST_NAME ), birth_death );
	return spouse;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditMarriage::OnBnClickedOk()
{
	CString orderHusband;
	CString orderWife;
	CString place;
	CString date;

	GetDlgItem( IDC_EDIT_ORDERWIFE )->GetWindowTextW( orderWife );
	GetDlgItem( IDC_EDIT_ORDERHUSBAND )->GetWindowTextW( orderHusband );
	GetDlgItem( IDC_EDIT_PLACE )->GetWindowTextW( place );
	GetDlgItem( IDC_EDIT_DATE )->GetWindowTextW( date );

	m_command.Format( L"UPDATE marriages SET orderHusband='%s', orderWife='%s', place='%s', date='%s' WHERE rowid='%s'", orderHusband, orderWife, place, date, m_rowid ); 
	if( !theApp.execute( m_command ) ) return;

	CDialogEx::OnOK();
}
