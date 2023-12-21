// EditFirstName.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "EditFirstName.h"
#include "afxdialogex.h"
#include "SelectOne.h"
#include "Languages.h"


// CEditFirstName dialog

IMPLEMENT_DYNAMIC(CEditFirstName, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditFirstName::CEditFirstName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditFirstName::IDD, pParent)
	, m_RadioSex(0)
	, m_firstname(_T(""))
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditFirstName::~CEditFirstName()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditFirstName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANGUAGE, colorLanguage);
	DDX_Control(pDX, IDC_STATIC_LANGUAGE, colorStatic);
	DDX_Radio(pDX, IDC_RADIO, m_RadioSex);
	DDX_Text(pDX, IDC_FIRSTNAME, m_firstname);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEditFirstName, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditFirstName::OnBnClickedOk)
	ON_STN_CLICKED(IDC_LANGUAGE, &CEditFirstName::OnClickedLanguage)
	ON_STN_CLICKED(IDC_STATIC_LANGUAGE, &CEditFirstName::OnClickedStaticLanguage)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditFirstName::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString title;

	m_command.Format( L"SELECT rowid, * FROM firstnames WHERE rowid = '%s'", m_rowid );
	if( !theApp.querySystem( m_command ) ) return FALSE;

	if (!m_firstname.IsEmpty())
	{
		GetDlgItem(IDC_FIRSTNAME)->SetWindowTextW(m_firstname);
	}
	else
	{
		CString first_name;
		first_name = theApp.m_recordsetSystem->GetFieldString(SEX_FIRST_NAME);
		GetDlgItem(IDC_FIRSTNAME)->SetWindowTextW(first_name);
	}

	CString origin;
	origin = theApp.m_recordsetSystem->GetFieldString( SEX_ORIGIN );
	GetDlgItem( IDC_ORIGIN )->SetWindowTextW( origin );

	CString meaning;
	meaning = theApp.m_recordsetSystem->GetFieldString( SEX_MEANING );
	GetDlgItem( IDC_MEANING )->SetWindowTextW( meaning );

	CString comment;
	comment = theApp.m_recordsetSystem->GetFieldString( SEX_COMMENT );
	GetDlgItem( IDC_COMMENT )->SetWindowTextW( comment );

	int sexId =	_wtoi( theApp.m_recordsetSystem->GetFieldString( SEX_SEX_ID ) );


	if( m_rowid.IsEmpty() )
	{
		title = L"Új név bevitele a nyilvántartásba";
		m_RadioSex = m_sexLast;
		m_uj = TRUE;

	}
	else
	{
		title = L"A nyilvántartásban lévõ név módosítása";
		m_RadioSex = sexId;
		m_uj = FALSE;
	}
	SetWindowText(title);

	UpdateData( TOSCREEN );

	CString language;
	m_language_id	= _wtoi( theApp.m_recordsetSystem->GetFieldString( SEX_LANGUAGE_ID ) );
	language	= theApp.getLanguage( m_language_id );
	GetDlgItem( IDC_LANGUAGE )->SetWindowTextW( language );

	colorLanguage.SetTextColor( theApp.m_colorClick );
	colorStatic.SetTextColor( theApp.m_colorClick );

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditFirstName::OnBnClickedOk()
{
	CString first_name;
	CString language;
	CString origin;
	CString meaning;
	CString comment;

	UpdateData( FROMSCREEN );

	GetDlgItem( IDC_FIRSTNAME )->GetWindowTextW( first_name );
	if( first_name.IsEmpty() )
	{
		AfxMessageBox( L"A nevet meg kell adni!" );
		return;
	}
	GetDlgItem( IDC_ORIGIN )->GetWindowTextW( origin );
	GetDlgItem( IDC_MEANING )->GetWindowTextW( meaning );
	GetDlgItem( IDC_COMMENT )->GetWindowTextW( comment );

	m_sexLast = m_RadioSex;
	
	CString columns;
	columns = L"first_name,sex_id,language_id, origin,meaning, comment ";
	CString fields;
	fields.Format( L"'%s','%d','%d','%s','%s', '%s'", first_name, m_sexLast, m_language_id, origin, meaning, comment ); 
	CString values;
	values.Format( L"first_name='%s',sex_id='%d', language_id = '%d', origin='%s',meaning='%s', comment='%s'", first_name, m_sexLast, m_language_id, origin,meaning, comment );
	
	if( m_rowid.IsEmpty() )
	{
		m_command.Format( L"SELECT rowid FROM firstnames WHERE first_name='%s'", first_name );
		if( !theApp.querySystem( m_command ) ) return;
		if( theApp.m_recordsetSystem->RecordsCount() )
		{
			str.Format( L"Már van '%s' keresztnév az adatbázisban!", first_name );
			AfxMessageBox( str );
			return;
		}
		m_command.Format( L"INSERT INTO firstnames (%s) VALUES (%s)", columns, fields );
		if( !theApp.executeSys( m_command ) ) return;

		m_command = L"SELECT last_insert_rowid() FROM languages";	
		if( !theApp.querySystem( m_command ) ) return;
		m_rowid = theApp.m_recordsetSystem->GetFieldString( 0 );
	}
	else
	{
		m_command.Format( L"UPDATE firstnames SET %s WHERE rowid='%s'", values, m_rowid );
		if( !theApp.executeSys( m_command ) ) return;
	}
	CDialogEx::OnOK();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditFirstName::OnClickedLanguage()
{
	CLanguages dlg;

	if(dlg.DoModal() == IDCANCEL ) return;
	GetDlgItem( IDC_LANGUAGE )->SetWindowTextW( dlg.m_language );
	m_language_id = dlg.m_language_id;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditFirstName::OnClickedStaticLanguage()
{
	OnClickedLanguage();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
