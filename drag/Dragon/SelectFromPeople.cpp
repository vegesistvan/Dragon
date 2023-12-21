// SelectFromPeople.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectFromPeople.h"
#include "afxdialogex.h"


// CSelectFromPeople dialog

IMPLEMENT_DYNAMIC(CSelectFromPeople, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectFromPeople::CSelectFromPeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectFromPeople::IDD, pParent)
	, m_info(_T(""))
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectFromPeople::~CSelectFromPeople()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFromPeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT, m_info);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectFromPeople, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectFromPeople::OnBnClickedOk)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectFromPeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW( m_caption );


	m_ListCtrl.SortByHeaderClick( TRUE );
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( 0,	L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 1,	L"bejegyzés",	LVCFMT_RIGHT,	300,-1,COL_NUM);

	m_columns = L"rowid, tablenumber, sex_id, last_name, first_name, birth_place, birth_date, death_place, death_date, occupation, comment, father_id, mother_id";

	CString people;
	people = getPeople( m_rowid );
	m_ListCtrl.InsertItem( 0, m_rowid );
	m_ListCtrl.SetItemText( 0, 1, people );





	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSelectFromPeople::getPeople( CString rowid )
{
	CString tablenumber;
	CString sexid;
	CString lastname;
	CString firstname;
	CString birthplace;
	CString birthdate;
	CString deathplace;
	CString deathdate;
	CString profession;
	CString comment;
	CString father_id;
	CString mother_id;

	CString birth;
	CString death;

	m_command.Format( L"SELECT %s FROM people WHERE rowid = '%s'", m_columns, rowid );
	if( !theApp.query( m_command ) ) return L"";
	tablenumber	= theApp.m_recordset->GetFieldString( TABLENUMBER );
	sexid		= theApp.m_recordset->GetFieldString( SEXID );
	lastname	= theApp.m_recordset->GetFieldString( LASTNAME );
	firstname	= theApp.m_recordset->GetFieldString( FIRSTNAME );
	birthplace	= theApp.m_recordset->GetFieldString( BIRTHPLACE );
	birthdate	= theApp.m_recordset->GetFieldString( BIRTHDATE );
	deathplace	= theApp.m_recordset->GetFieldString( DEATHPLACE );
	deathdate	= theApp.m_recordset->GetFieldString( DEATHDATE );
	profession	= theApp.m_recordset->GetFieldString( PROFESSION );
	comment		= theApp.m_recordset->GetFieldString( COMMENT );
	father_id	= theApp.m_recordset->GetFieldString( FATHERID );
	mother_id	= theApp.m_recordset->GetFieldString( MOTHERID );

	birth = pack( L"*", birthplace, birthdate );
	death = pack( L"+", deathplace, deathdate );
	
	str.Format( L"%s %s", lastname, firstname );
	str.Format( L"%s %s", (CString)str, birth );
	str.TrimRight() ;
	str.Format( L"%s %s", (CString)str, death );
	str.TrimRight();
	str.Format( L"%s %s", (CString)str, comment ); 
	str.TrimRight();
	str.Format( L"%s %s", (CString)str, profession ); 
	str.TrimRight();


	return  str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSelectFromPeople::pack( CString tag, CString place, CString date )
{
	str.Format( L"%s %s", place, date );
	str.Trim();
	if( !str.IsEmpty() )
		str.Format( L"%s%s", tag, (CString)str );
	return str;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFromPeople::OnBnClickedOk()
{
	int nItem;
	if( (nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) ) == -1 )
	{
		AfxMessageBox( L"Ki kell jelölni valakit!", MB_ICONASTERISK );
		return;
	}
	m_rowidSel = m_ListCtrl.GetItemText( nItem, 0 );
	CDialogEx::OnOK();
}
