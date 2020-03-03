// NewPeople.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "NewPeople.h"
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
{
	m_last_name.Empty();
	m_rowidExists.Empty();
	m_rowidMain.Empty();
	m_orderFather = 0;
	m_orderMother = 0;
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
	CString command;
	CString caption;

	// a megklikkelt ember: 
//	if( !m_rowidClick.IsEmpty() && m_newPeople != CHILD ) 
	if( !m_rowidClick.IsEmpty() ) 
	{
		command.Format( L"SELECT rowid, * FROM people WHERE rowid='%s'", m_rowidClick );
		if( !theApp.query( command ) ) return false;
		
		m_title			= theApp.m_recordset->GetFieldString( PEOPLE_TITLE );
		m_titolo		= theApp.m_recordset->GetFieldString( PEOPLE_TITOLO );
		m_last_name		= theApp.m_recordset->GetFieldString( PEOPLE_LAST_NAME );
		m_first_name	= theApp.m_recordset->GetFieldString( PEOPLE_FIRST_NAME );
		m_father_id		= theApp.m_recordset->GetFieldString( PEOPLE_FATHER_ID );
		m_mother_id		= theApp.m_recordset->GetFieldString( PEOPLE_MOTHER_ID );
//		m_mother_index	= _wtoi( theApp.m_recordset->GetFieldString( PEOPLE_MOTHER_INDEX ) );

		m_sex_id		= _wtoi( theApp.m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
		m_birth_place	= theApp.m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE );
		m_birth_date	= theApp.m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
		m_death_place	= theApp.m_recordset->GetFieldString( PEOPLE_DEATH_PLACE );
		m_death_date	= theApp.m_recordset->GetFieldString( PEOPLE_DEATH_DATE );
		m_comment		= theApp.m_recordset->GetFieldString( PEOPLE_COMMENT );

		m_name.Format( L"%s %s", m_last_name, m_first_name ); 

		comboSex.ResetContent();
		comboSex.AddString( L"" );
		comboSex.AddString( L"ffi" );
		comboSex.AddString( L"nõ" );
		comboSex.SetCurSel( m_sex_id );
	}

	if( m_rowidMain.IsEmpty() )			// a megklikkelt ember editálása lesz!!
	{
		m_command = L"SELECT title FROM titles ORDER BY title";
		if( ! theApp.querySystem( m_command ) ) return false;

		CString title;
		comboTitle.AddString( L"" );
		for( UINT i = 0; i < theApp.m_recordsetSystem->RecordsCount(); ++i )
		{
			title =  theApp.m_recordsetSystem->GetFieldString( 0 ); 
			comboTitle.AddString( title );
			if( title == m_title ) 
				m_titleX = i+1;
			theApp.m_recordsetSystem->MoveNext();
		}
		comboTitle.SetCurSel( m_titleX );

		GetDlgItem( IDC_TITOLO )->SetWindowTextW( m_titolo );
		GetDlgItem( IDC_LAST_NAME )->SetWindowTextW( m_last_name );
		GetDlgItem( IDC_FIRST_NAME )->SetWindowTextW( m_first_name );
		GetDlgItem( IDC_BIRTH_PLACE )->SetWindowTextW( m_birth_place );
		GetDlgItem( IDC_BIRTH_DATE )->SetWindowTextW( m_birth_date );
		GetDlgItem( IDC_DEATH_PLACE )->SetWindowTextW( m_death_place );
		GetDlgItem( IDC_DEATH_DATE )->SetWindowTextW( m_death_date );
		GetDlgItem( IDC_COMMENT )->SetWindowTextW( m_comment );
		
	}

	else
	{
//		if( m_newPeople != CHILD )
		{
			m_command.Format( L"SELECT rowid, * FROM people WHERE rowid='%s'", m_rowidMain );
			if( !theApp.query1( m_command ) ) return false;
				m_sexIdMain		= _wtoi( theApp.m_recordset1->GetFieldString( PEOPLE_SEX_ID ) );
			m_lastNameMain	= theApp.m_recordset1->GetFieldString( PEOPLE_LAST_NAME );
			m_firstNameMain	= theApp.m_recordset1->GetFieldString( PEOPLE_FIRST_NAME );
			m_fatherIdMain	= theApp.m_recordset1->GetFieldString( PEOPLE_FATHER_ID );
			m_motherIdMain	= theApp.m_recordset1->GetFieldString( PEOPLE_MOTHER_ID );
			m_nameMain.Format( L"%s %s", m_lastNameMain, m_firstNameMain ); 

			comboSex.ResetContent();
			comboSex.AddString( L"" );
			comboSex.AddString( L"ffi" );
			comboSex.AddString( L"nõ" );
			comboSex.SetCurSel( m_sexIdMain);
		}
	}


	

	command = L"SELECT title FROM titles ORDER BY title";
	if( ! theApp.querySystem( command ) ) return false;

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

	switch( m_newPeople )			// milyen rokont akarunk megadni?
	{
		case -1:					// senkiét, csak egy új embert akarunk bevinni
			newPeople();
			break;
		case CHILD:
			newChild();
			break;
		case FATHER:
			newFather();
			break;
		case MOTHER:
			newMother();
			break;
		case SIBLING:
			newSibling();
			break;
		case SPOUSE:
			newSpouse();
			break;
	}


	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::newPeople()
{
	SetWindowText( L"Addj meg egy új embert!" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::newChild()
{
	CString caption;
	caption.Format( L"Add meg %s és %s gyerekét!", m_name, m_nameMain );
	SetWindowText( caption );
	if( m_sexIdMain == MAN )
	{
		GetDlgItem( IDC_LAST_NAME )->SetWindowTextW( m_lastNameMain );
	}
	else
	{
		GetDlgItem( IDC_LAST_NAME )->SetWindowTextW( m_last_name );   // megklikkelt ember neve
	}
	keybd_event(VK_TAB,0x8f,0 , 0);					// Tab Press
	keybd_event(VK_TAB,0x8f, KEYEVENTF_KEYUP,0);	// Tab Release
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::newFather()
{
	CString caption;
	caption.Format( L"Add meg %s apját!", m_nameMain );
	SetWindowText( caption );
	GetDlgItem( IDC_LAST_NAME )->SetWindowTextW( m_lastNameMain );
	keybd_event(VK_TAB,0x8f,0 , 0);					// Tab Press
	keybd_event(VK_TAB,0x8f, KEYEVENTF_KEYUP,0);	// Tab Release
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::newMother()
{
	CString caption;
	caption.Format( L"Add meg %s anyját!", m_nameMain );
	SetWindowText( caption );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::newSibling()
{
	CString caption;
	caption.Format( L"Add meg %s testvérét!", m_nameMain );
	SetWindowText( caption );

	GetDlgItem( IDC_LAST_NAME )->SetWindowTextW( m_lastNameMain );
	keybd_event(VK_TAB,0x8f,0 , 0);					// Tab Press
	keybd_event(VK_TAB,0x8f, KEYEVENTF_KEYUP,0);	// Tab Release
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::newSpouse()
{
	CString caption;
	caption.Format( L"Add meg %s házastársát!", m_nameMain );
	SetWindowText( caption );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnBnClickedOk()
{
	GetDlgItem( IDC_LAST_NAME )->GetWindowText( m_lastNameNew ); 


	if( m_lastNameNew.IsEmpty() )
	{
		AfxMessageBox( L"Családnevet meg kell adni!" );
		return;
	}

	int n = comboTitle.GetCurSel();
	comboTitle.GetLBText( comboTitle.GetCurSel(), m_titleNew );

	GetDlgItem( IDC_TITOLO )->GetWindowText( m_titoloNew ); 
	GetDlgItem( IDC_FIRST_NAME )->GetWindowText( m_firstNameNew ); 
	GetDlgItem( IDC_BIRTH_PLACE )->GetWindowText( m_birthPlaceNew );
	GetDlgItem( IDC_BIRTH_DATE )->GetWindowText( m_birthDateNew );
	GetDlgItem( IDC_DEATH_PLACE )->GetWindowText( m_deathPlaceNew );
	GetDlgItem( IDC_DEATH_DATE )->GetWindowText( m_deathDateNew );
	GetDlgItem( IDC_COMMENT )->GetWindowText( m_commentNew );

	m_nameNew.Format( L"%s %s", m_lastNameNew, m_firstNameNew );


	m_rowidExists = m_rowidClick;	
	if( !m_rowidMain.IsEmpty() )		// létezõ ember editálása, nem kell azonosítani!!
	{
		if( peopleExists() ) return;
	}


	if( m_rowidExists.IsEmpty() )  // nincs ilyen ember
	{
		switch( m_newPeople )
		{
		case -1:
			insertPeople();					// csak az embert visszük be
			break;
		case SIBLING:
			if( !insertSibling() ) return;
			break;
		case FATHER:
			if( !insertFather() ) return;
			break;
		case MOTHER:
			if( !insertMother() ) return;
			break;
		case CHILD:
			if( !insertChild() ) return;
			break;
		case SPOUSE:
			if( !insertSpouse() ) return;
			break;
		}
	}
	else						// van ilyen ember
	{
		switch( m_newPeople )
		{
		case -1:
			updatePeople();
			break;
		case SIBLING:
			if( !existingSibling() ) return;
			break;
		case FATHER:
			if( !existingFather() ) return;
			break;
		case MOTHER:
			if( !existingMother() ) return;
			break;
		case CHILD:
			if( !existingChild() ) return;
			break;
		case SPOUSE:
			if( !existingSpouse() ) return;
			break;
		}
	}
	if( !m_rowidExists.IsEmpty() )
		m_rowidClick = m_rowidExists;
	else
		m_rowidClick = m_rowidNew;

	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::insertChild()
{
	if( m_sexIdMain == MAN )
	{
		m_father_id = m_rowidMain;
		m_mother_id = m_rowidClick;
	}
	else
	{
		m_father_id = m_rowidClick;
		m_mother_id = m_rowidMain;
	}
	return( insertPeople() );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// m_rowidClickExist már létezõ ember, aki az m_rowidClick ember 
bool CNewPeople::existingChild()
{
	if( m_sexIdMain == MAN )
	{
		m_father_id = m_rowidMain;
		m_mother_id = getMotherId();
	}
	else
	{
		m_father_id = getFatherId();
		m_mother_id = m_rowidMain;
	}
	m_command.Format( L"UPDATE people SET father_id='%s', mother_id = '%s', orderFather='%d', orderMother='%d', mother_index='%d', m_mother_index2 = '%d' WHERE rowid = '%s'", m_father_id, m_mother_id, m_orderFather, m_orderMother, m_mother_index, m_mother_index, m_rowidExists );
	if( !theApp.execute( m_command ) ) return false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::insertSibling()
{
	if( !insertPeople() ) return false;

	m_command.Format( L"UPDATE people SET father_id = '%s', mother_id='%s' WHERE rowid='%s'", m_fatherIdMain, m_motherIdMain, m_rowidNew );
	if( !theApp.execute( m_command ) ) return false;
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// m_rowidClickExist már létezõ ember, aki az m_rowidClick ember testvére
// tehát 
bool CNewPeople::existingSibling()
{
	CString command;
	CString father_id;
	CString mother_id;
	CString name;

	command.Format( L"SELECT father_id, mother_id, last_name, first_name FROM people  WHERE rowid = '%s'", m_rowidExists );
	if( !theApp.query2( command ) ) return false;
	name.Format( L"%s %s", theApp.m_recordset2->GetFieldString( 2 ), theApp.m_recordset2->GetFieldString( 3 ) );
	father_id = theApp.m_recordset2->GetFieldString( 0 ); 
	if( !father_id.IsEmpty() )
	{
		if( father_id == m_father_id )
			str.Format( L"%s már testvéred!", name );
		else
			str.Format( L"%snak  más az apja!", name );
		AfxMessageBox( str );
		return false;
	}
	mother_id = theApp.m_recordset2->GetFieldString( 1 ); 
	if( !mother_id.IsEmpty() )
	{
		str.Format( L"%snak van más az anyja!", name );
		AfxMessageBox( str );
		return false;
	}

	command.Format( L"UPDATE people SET father_id='%s', mother_id='%s' WHERE rowid = '%s'", m_fatherIdMain, m_motherIdMain, m_rowidExists );
	theApp.execute( command );
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::insertFather()
{
	m_father_id.Empty();
	m_mother_id.Empty();

	if( !insertPeople() ) return false;
	m_command.Format( L"UPDATE people SET father_id='%s' WHERE rowid = '%s'", m_rowidNew, m_rowidMain  );
	theApp.execute( m_command );
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::existingFather()
{
	CString command;
	command.Format( L"UPDATE people SET father_id='%s' WHERE rowid = '%s'", m_rowidExists, m_rowidMain );
	theApp.execute( command );
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::insertMother()
{
	m_father_id.Empty();
	m_mother_id.Empty();
	if( !insertPeople() ) return false;

	m_command.Format( L"UPDATE people SET mother_id='%s' WHERE rowid = '%s'", m_rowidNew, m_rowidMain  );
	theApp.execute( m_command );
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::existingMother()
{
	CString command;
	command.Format( L"UPDATE people SET mother_id='%s' WHERE rowid = '%s'", m_rowidExists, m_rowidMain  );
	theApp.execute( command );
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::insertSpouse()
{
	if( m_sexIdMain == m_sexIdNew )
	{
		AfxMessageBox( L"A házaspár azonos nemû!" );
		return( false );
	}
	if( m_firstNameNew.IsEmpty() ) 
	{
		AfxMessageBox( L"Keresztnevet meg kell adni!" );
		return false;
	}
	m_father_id.Empty();
	m_mother_id.Empty();

	if( !insertPeople() ) return false;

	if( m_sexIdMain == MAN )
		m_command.Format( L"INSERT INTO marriages ( spouse1_id, spouse2_id ) VALUES ('%s', '%s')", m_rowidMain, m_rowidNew );
	else
		m_command.Format( L"INSERT INTO marriages ( spouse1_id, spouse2_id ) VALUES ('%s', '%s')", m_rowidNew, m_rowidMain );
	theApp.execute( m_command );

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::existingSpouse()
{
	CString command;
	if( m_sexIdMain == MAN )
		command.Format( L"INSERT INTO marriages ( spouse1_id, spouse2_id ) VALUES ('%s', '%s')", m_rowidMain, m_rowidExists );
	else
		command.Format( L"INSERT INTO marriages ( spouse1_id, spouse2_id ) VALUES ('%s', '%s')", m_rowidExists, m_rowidMain );
	theApp.execute( command );
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::insertPeople()
{
	CString fields;
	CString values;
	CString command;
	int	sex = comboSex.GetCurSel();

	m_titleX = comboTitle.GetCurSel();
	comboTitle.GetLBText( m_titleX, m_titleNew );


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
comment,\
father_id,\
mother_id,\
orderFather,\
orderMother,\
mother_index\
" );
	values.Format( L"'%s','%s','%d','%s','%s','%s','%s','%s','%s','%s', '%s', '%s', '%d', '%d', '%d' ",
m_titleNew,\
m_titoloNew,\
sex,\
m_firstNameNew,\
m_lastNameNew,\
m_birthPlaceNew,\
m_birthDateNew,\
m_deathPlaceNew,\
m_deathDateNew,\
m_commentNew,\
m_father_id,\
m_mother_id,\
m_orderFather,\
m_orderMother,\
m_mother_index\
);

	m_rowidNew.Empty();
	command.Format( L"INSERT INTO people (%s) VALUES (%s)", fields, values );
	if( theApp.execute( command ) )
	{
		command = L"SELECT last_insert_rowid() FROM people";
		if( theApp.query( command ) )
			m_rowidNew = theApp.m_recordset->GetFieldString(0);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::updatePeople()
{
	m_titleX = comboTitle.GetCurSel();
	comboTitle.GetLBText( m_titleX, m_titleNew );
	int sex = comboSex.GetCurSel();

	CString set;
	set.Format( L"\
title='%s',\
titolo='%s',\
sex_id='%d',\
first_name='%s',\
last_name='%s',\
birth_place='%s',\
birth_date='%s',\
death_place='%s',\
death_date='%s',\
comment='%s'\
",\
m_titleNew,\
m_titoloNew,\
sex,\
m_firstNameNew,\
m_lastNameNew,\
m_birthPlaceNew,\
m_birthDateNew,\
m_deathPlaceNew,\
m_deathDateNew,\
m_commentNew\
);
	m_command.Format( L"UPDATE people SET %s WHERE rowid='%s'", set, m_rowidClick );
	if( !theApp.execute( m_command ) ) return false;
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
	GetDlgItem( IDC_BIRTH_DATE )->GetWindowText( m_birthDateNew ); 
	if( !m_birthDateNew.IsEmpty() && !checkDate( m_birthDateNew ) )
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
	GetDlgItem( IDC_FIRST_NAME )->GetWindowText( m_firstNameNew ); 
	if( m_firstNameNew.IsEmpty() ) return;


	m_sexIdNew = theApp.isFirstName( m_firstNameNew );
	if( m_sexIdNew == -1 )
	{
		str.Format( L"%s keresztnév nincs a nyilvántartásban!", m_firstNameNew );
		AfxMessageBox( str );
		m_first_name.Empty();
		GetDlgItem( IDC_FIRST_NAME )->SetWindowText( L"" );
		GetDlgItem( IDC_FIRST_NAME )->SetFocus();
		m_sexIdNew = 0;
	}
	comboSex.ResetContent();
	comboSex.AddString( L"" );
	comboSex.AddString( L"ffi" );
	comboSex.AddString( L"nõ" );
	comboSex.SetCurSel( m_sexIdNew );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CNewPeople::getMotherId()
{
	CString spouse2_id;
	if( m_sexIdMain == MAN )
		m_command.Format( L"SELECT spouse2_id FROM marriages WHERE spouse1_id='%s' AND spouse2_id = '%s'", m_rowidMain, m_rowidClick );
	else
		m_command.Format( L"SELECT spouse2_id FROM marriages WHERE spouse1_id='%s' AND spouse2_id = '%s'", m_rowidClick, m_rowidMain );
	if( !theApp.query1( m_command ) ) return( L"");
	spouse2_id = theApp.m_recordset1->GetFieldString( 0 );
	return spouse2_id;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CNewPeople::getFatherId()
{
	CString spouse1_id;
	if( m_sexIdMain == MAN )
		m_command.Format( L"SELECT spouse1_id FROM marriages WHERE spouse1_id='%s' AND spouse2_id = '%s'", m_rowidMain, m_rowidClick );
	else
		m_command.Format( L"SELECT spouse1_id FROM marriages WHERE spouse1_id='%s' AND spouse2_id = '%s'", m_rowidClick, m_rowidMain );
	if( !theApp.query1( m_command ) ) return( L"");
	spouse1_id = theApp.m_recordset1->GetFieldString( 0 );
	return spouse1_id;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::peopleExists()
{
	CString people;
	CString caption;
	people.Format( L"%s %s %s %s", m_titleNew, m_titoloNew, m_lastNameNew, m_firstNameNew );
	people.Trim();

	m_rowidExists.Empty();
	switch( m_newPeople )
	{
		case SIBLING:
			caption.Format( L"Válaszd ki %s testvérét!", m_name ); 
			break;
		case FATHER:
			caption.Format( L"Válaszd ki %s apját!", m_name ); 
			break;
		case MOTHER:
			caption.Format( L"Válaszd ki %s anyját!", m_name ); 
			break;
		case CHILD:
			caption.Format( L"Vélaszd ki %s gyerekét!", m_name ); 
			break;
		case SPOUSE:
			caption.Format( L"Vélaszd ki %s házastársát!", m_name ); 
			break;
	}

	CString birth;
	CString death;
	birth.Format( L"%s %s", m_birthPlaceNew, m_birthDateNew );
	death.Format( L"%s %s", m_deathPlaceNew, m_deathDateNew );
	birth.Trim();
	death.Trim();
	if( !birth.IsEmpty() )
		people.Format( L"%s *%s", (CString)people, birth );
	if( !death.IsEmpty() )
		people.Format( L"%s +%s", (CString)people, death );



	if( m_firstNameNew.IsEmpty() )
		m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c'", m_lastNameNew,'%' );
	else
		m_command.Format( L"SELECT rowid, * FROM people WHERE first_name LIKE '%s%c' AND last_name LIKE '%s%c'", m_firstNameNew, '%', m_lastNameNew, '%' );

//	m_command.Format( L"SELECT rowid,* FROM people WHERE last_name='%s' AND first_name = '%s'", m_lastNameNew,	m_firstNameNew );
	if( !theApp.query2( m_command ) ) return false;
	if( !theApp.m_recordset2->RecordsCount() ) return false;

	CSameName dlg;

	dlg.m_caption		= caption;
	dlg.m_last_name		= m_lastNameNew;
	dlg.m_first_name	= m_firstNameNew;
	dlg.m_birth_place	= m_birthPlaceNew;
	dlg.m_birth_date	= m_birthDateNew;
	dlg.m_death_place	= m_deathPlaceNew;
	dlg.m_death_date	= m_deathDateNew;
	dlg.m_comment		= m_commentNew;

	dlg.m_people = people.Trim();
	if( dlg.DoModal() == IDCANCEL ) return true;
	m_rowidExists = dlg.m_rowid;
	return false;
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
