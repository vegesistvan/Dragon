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
		
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE husband_id ='%s' AND wife_id='%s'", m_rowidH, m_rowidW );
		if( !theApp.query( m_command ) ) return false;
		m_rowid = theApp.m_recordset->GetFieldString( DBM_ROWID );
		if( m_rowid.IsEmpty() )
		{
			AfxMessageBox( L"Nincs házasság!" );
			OnCancel();
			return false;
		}
		
	}

	if( !m_rowid.IsEmpty() )
	{
		m_command.Format( L"SELECT rowid,* FROM marriages WHERE rowid ='%s'", m_rowid );
		if( !theApp.query( m_command ) ) return false;
		
		m_rowidH = theApp.m_recordset->GetFieldString( DBM_HUSBAND_ID );
		m_rowidW = theApp.m_recordset->GetFieldString( DBM_WIFE_ID );
		m_place = theApp.m_recordset->GetFieldString( DBM_PLACE );
		m_date = theApp.m_recordset->GetFieldString( DBM_DATE );

		GetDlgItem( IDC_EDIT_PLACE )->SetWindowTextW( m_place );
		GetDlgItem( IDC_EDIT_DATE )->SetWindowTextW( m_date );

		m_whichWife		= theApp.m_recordset->GetFieldString( DBM_WHICHWIFE );
		m_whichHusband	= theApp.m_recordset->GetFieldString( DBM_WHICHHUSBAND );

		GetDlgItem( IDC_EDIT_ORDERWIFE )->SetWindowTextW( m_whichWife );
		GetDlgItem( IDC_EDIT_ORDERHUSBAND )->SetWindowTextW( m_whichHusband );
	}

	GetDlgItem( IDC_HUSBAND )->SetWindowTextW( getSpouse( m_rowidH ) );
	GetDlgItem( IDC_WIFE )->SetWindowTextW( getSpouse( m_rowidW ) );


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
	
	str.Format( L"%s %s", theApp.m_recordset1->GetFieldString( DBP_BIRTH_PLACE ), theApp.m_recordset1->GetFieldString( DBP_BIRTH_DATE ) );
	str.Trim();
	if( !str.IsEmpty() ) birth.Format( L"*%s", str );
	
	str.Format( L"%s %s", theApp.m_recordset1->GetFieldString( DBP_DEATH_PLACE ), theApp.m_recordset1->GetFieldString( DBP_DEATH_DATE ) );
	str.Trim();
	if( !str.IsEmpty() ) death.Format( L"+%s", str );

	birth_death.Format( L"%s %s", birth, death ); 

	spouse.Format( L"%s %s %s", theApp.m_recordset1->GetFieldString( DBP_LAST_NAME ), theApp.m_recordset1->GetFieldString( DBP_FIRST_NAME ), birth_death );
	return spouse;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditMarriage::OnBnClickedOk()
{

	GetDlgItem( IDC_EDIT_ORDERWIFE )->GetWindowTextW( m_whichWife );
	GetDlgItem( IDC_EDIT_ORDERHUSBAND )->GetWindowTextW( m_whichHusband );
	GetDlgItem( IDC_EDIT_PLACE )->GetWindowTextW( m_place );
	GetDlgItem( IDC_EDIT_DATE )->GetWindowTextW( m_date );

	if( m_rowid.IsEmpty() )
		m_command.Format( L"INSERT INTO marriages (place, date, husband_id,wife_id, whichHusband, whichWife )\
		VALUES ('%s','%s', '%s', '%s','%s', '%s')", m_place, m_date, m_rowidH, m_rowidW, m_whichHusband, m_whichWife );
	else
		m_command.Format( L"UPDATE marriages SET whichHusband='%s', whichWife='%s', place='%s', date='%s' WHERE rowid='%s'", m_whichHusband, m_whichWife, m_place, m_date, m_rowid ); 
	if( !theApp.execute( m_command ) ) return;

	CDialogEx::OnOK();
}
