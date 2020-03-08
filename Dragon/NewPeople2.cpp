// Bemenõ adatok:
// m_caption		Az ablak felirata
// m_first_name		Az új ember vezetékneve, if any
//
// Bekéri az új ember adatait és ellenõrzi, hogy van-e már ilyen nevû ember az adatbázisban.
// Ha van, akkor felkéri a felhasználót, hogy válasszon az új és a régi emberek küzül.
// Ha az újat választotta, akkor insertálja az adatbázisba az embert.
// A megadott/választott ember rowid-ját adja vissz az m_rowid változóban.
// Ha a felhasználó Cancel-el mégsem akar új embert adni, akkor az m_rowid empty lesz. 

#include "stdafx.h"
#include "Fa.h"
#include "NewPeople2.h"
#include "afxdialogex.h"
#include "SameName.h"


const TCHAR* modiDate[] =
{
	L"",
	L"kb",
	L"elõtt",
	L"után"
};
const UINT modiDateCnt = sizeof( modiDate ) / sizeof( TCHAR* );


IMPLEMENT_DYNAMIC(CNewPeople, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNewPeople::CNewPeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewPeople::IDD, pParent)
	, m_last_name(_T(""))
	, m_first_name(_T(""))
	, m_titolo(_T(""))
	, m_comment(_T(""))
	, m_death_date(_T(""))
	, m_death_place(_T(""))
	, m_birth_date(_T(""))
	, m_birth_place(_T(""))
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNewPeople::~CNewPeople()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TITLE, comboTitle);
	DDX_Control(pDX, IDC_BIRTH_COMBO, comboBirth);
	DDX_Control(pDX, IDC_DEATH_COMBO, comboDeath);
	DDX_Control(pDX, IDC_COMBO_SEX, comboSex);
	DDX_Text(pDX, IDC_LAST_NAME, m_last_name);
	DDX_Text(pDX, IDC_FIRST_NAME, m_first_name);
	DDX_Text(pDX, IDC_TITOLO, m_titolo);
	DDX_Text(pDX, IDC_COMMENT, m_comment);
	DDX_Text(pDX, IDC_DEATH_DATE, m_death_date);
	DDX_Text(pDX, IDC_DEATH_PLACE, m_death_place);
	DDX_Text(pDX, IDC_BIRTH_DATE, m_birth_date);
	DDX_Text(pDX, IDC_BIRTH_PLACE, m_birth_place);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CNewPeople, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewPeople::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_BIRTH_DATE, &CNewPeople::OnKillfocusBirthDate)
	ON_EN_KILLFOCUS(IDC_DEATH_DATE, &CNewPeople::OnKillfocusDeathDate)
	ON_EN_KILLFOCUS(IDC_FIRST_NAME, &CNewPeople::OnKillfocusFirstName)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CNewPeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText( m_caption );

	comboSex.ResetContent();
	comboSex.AddString( L"" );
	comboSex.AddString( L"ffi" );
	comboSex.AddString( L"nõ" );
	comboSex.SetCurSel( 0);

	m_command = L"SELECT title FROM titles ORDER BY title";
	if( ! theApp.querySystem( m_command ) ) return false;
	comboTitle.AddString( L"" );
	for( UINT i = 0; i < theApp.m_recordsetSystem->RecordsCount(); ++i )
	{
		comboTitle.AddString( theApp.m_recordsetSystem->GetFieldString( 0 ) );
		theApp.m_recordsetSystem->MoveNext();
	}
	comboTitle.SetCurSel( 0 );

	for( UINT i = 0; i < modiDateCnt; ++i )
	{
		comboBirth.AddString( modiDate[i] );
		comboDeath.AddString( modiDate[i] );
	}
	comboBirth.SetCurSel( 0 );
	comboDeath.SetCurSel( 0 );

	m_inserted = false;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );
	if( m_last_name.IsEmpty() )
	{
		AfxMessageBox( L"Családnevet meg kell adni!" );
		return;
	}

	int ix = comboTitle.GetCurSel();
	comboTitle.GetLBText( ix, m_title );
	m_sex_id = comboSex.GetCurSel();

//	comboBirth.GetLBText( comboBirth.GetCurSel(), str );
//	comboDeath.GetLBText( comboBirth.GetCurSel(), str );

	m_rowid.IsEmpty();
	if( peopleExists() )
	{
		if( m_rowid.IsEmpty() )	insertPeople();
	}

	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////44////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::insertPeople()
{
	CString fields;
	CString values;
	CString command;
	int	sex = comboSex.GetCurSel();



	fields.Format( L"\
title,\
titolo,\
sex_id,\
first_name,\
last_name,\
birth_place,\
birth_date,\
death_place,\
death_date,\
comment\
" );
	values.Format( L"'%s','%s','%d','%s','%s','%s','%s','%s','%s','%s'",
m_title,\
m_titolo,\
sex,\
m_first_name,\
m_last_name,\
m_birth_place,\
m_birth_date,\
m_death_place,\
m_death_date,\
m_comment\
);

	m_rowid.Empty();
	command.Format( L"INSERT INTO people (%s) VALUES (%s)", fields, values );
	if( theApp.execute( command ) )
	{
		command = L"SELECT last_insert_rowid() FROM people";
		if( theApp.query( command ) )
			m_rowid = theApp.m_recordset->GetFieldString(0);
	}
	m_inserted = true;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnKillfocusBirthDate()
{
	GetDlgItem( IDC_BIRTH_DATE )->GetWindowText( m_birth_date ); 
	if( !m_birth_date.IsEmpty() && !checkDate( m_birth_date ) )
	{
		GetDlgItem( IDC_BIRTH_DATE )->SetWindowText( L"" ); 
		GetDlgItem( IDC_BIRTH_DATE )->SetFocus();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnKillfocusDeathDate()
{
	GetDlgItem( IDC_DEATH_DATE )->GetWindowText( m_death_date ); 
	if( !m_birth_date.IsEmpty() && !checkDate( m_birth_date ) )
	{
		GetDlgItem( IDC_DEATH_DATE )->SetWindowText( L"" ); 
		GetDlgItem( IDC_DEATH_DATE )->SetFocus();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnKillfocusFirstName()
{
	GetDlgItem( IDC_FIRST_NAME )->GetWindowText( m_first_name ); 
	if( m_first_name.IsEmpty() ) return;


	m_sex_id = theApp.isFirstName( m_first_name );
	if( m_sex_id == -1 )
	{
		str.Format( L"%s keresztnév nincs a nyilvántartásban!", m_first_name );
		AfxMessageBox( str );
		m_first_name.Empty();
		GetDlgItem( IDC_FIRST_NAME )->SetWindowText( L"" );
		GetDlgItem( IDC_FIRST_NAME )->SetFocus();
		m_sex_id = 0;
	}
	comboSex.ResetContent();
	comboSex.AddString( L"" );
	comboSex.AddString( L"ffi" );
	comboSex.AddString( L"nõ" );
	comboSex.SetCurSel( m_sex_id );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::peopleExists()
{
	CString people;
	people.Format( L"%s %s %s %s", m_title, m_titolo, m_last_name, m_first_name );
	people.Trim();
	
	CString birth;
	CString death;
	birth.Format( L"%s %s", m_birth_place, m_birth_date );
	death.Format( L"%s %s", m_death_place, m_death_date );
	birth.Trim();
	death.Trim();
	if( !birth.IsEmpty() )
		people.Format( L"%s *%s", (CString)people, birth );
	if( !death.IsEmpty() )
		people.Format( L"%s +%s", (CString)people, death );
	people.Trim();

	if( m_first_name.IsEmpty() )
		m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c'", m_last_name,'%' );
	else
		m_command.Format( L"SELECT rowid, * FROM people WHERE first_name LIKE '%s%c' AND last_name LIKE '%s%c'", m_first_name, '%', m_last_name, '%' );
	if( !theApp.query2( m_command ) ) return false;
	if( theApp.m_recordset2->RecordsCount() )
	{
		CSameName dlg;
		dlg.m_last_name		= m_last_name;
		dlg.m_first_name	= m_first_name;
		dlg.m_birth_place	= m_birth_place;
		dlg.m_birth_date	= m_birth_date;
		dlg.m_death_place	= m_death_place;
		dlg.m_death_date	= m_death_date;
		dlg.m_comment		= m_comment;
		dlg.m_people		= people;

		if( dlg.DoModal() == IDCANCEL ) return false;
		m_rowid = dlg.m_rowid;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::checkDate( CString date )
{
	CString error;
	error.Format( L"%s illegális dátum!\n\nmegengedett:\nYYYY\nYYYY.MM.DD ahol MM/DD '??' vagy XX szám", date ); 
	int length = date.GetLength();
	if( length != 4 && length != 10 )
	{
		AfxMessageBox( error );
		return false;
	}
	
	int year	= _wtoi( date.Left( 4 ) );
	int month;
	int day;
	if( year < 1000 || 2500 < year )
	{
		str.Format( L"%d évszám nem megengedett!", year );
		AfxMessageBox( str );
		return false;
	}
	
	if( length == 4 ) return true;
	if( length != 10 )
	{
		AfxMessageBox( error );
		return false;
	}
	if( date.GetAt( 4 ) != '.' || date.GetAt( 7 ) != '.' )
	{
		AfxMessageBox( error );
		return false;
	}

	CString monthS	= date.Mid( 5, 2 );
	if( monthS.Compare( L"??" ) )
	{
		month	= _wtoi( monthS );
		if( month < 1 || 12 < month )
		{
			AfxMessageBox( error );
			return false;
		}
	}

	CString dayS	= date.Mid( 8 );
	if( dayS.Compare( L"??" ) )
	{
		day	= _wtoi( dayS );
		if( day < 1 || 31 < day )
		{
			AfxMessageBox( error );
			return false;
		}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
