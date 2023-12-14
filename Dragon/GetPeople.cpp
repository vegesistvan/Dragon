// GetPeople.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "GetPeople.h"
#include "afxdialogex.h"
#include "GetLastFirst.h"
#include "ProgressWnd.h"

enum
{
	L_ROWID = 0,
	L_GENERATION,
	L_LAST_NAME,
	L_FIRST_NAME,
	L_BIRTH_PLACE,
	L_BIRTH_DATE,
	L_DEATH_PLACE,
	L_DEATH_DATE,
	L_COMMENT,
};
IMPLEMENT_DYNAMIC(CGetPeople, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGetPeople::CGetPeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGetPeople::IDD, pParent)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGetPeople::~CGetPeople()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGetPeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGetPeople, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGetPeople::OnBnClickedOk)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGetPeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CGetLastFirst dlg;
	if( dlg.DoModal() ==  IDCANCEL )
	{
		OnCancel();
		return false;
	}

	m_fullname		= dlg._fullname;
	m_last_name		= dlg._lastname;
	m_first_name	= dlg._firstname;


	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_RIGHT,	40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_LAST_NAME,	L"családnév",	LVCFMT_LEFT,	100,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_FIRST_NAME,	L"keresztnév",	LVCFMT_LEFT,	100,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH_PLACE,	L"szültés",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_DATE,	L"dátum",		LVCFMT_RIGHT,	70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH_PLACE,	L"halál",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_DATE,	L"dátum",		LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_COMMENT,		L"leírás",	LVCFMT_LEFT,	100,-1,COL_TEXT);


//	m_sex_id		= dlg._sex_id;

	str.Format( L"Nevek beolvasása folyik..." );
	CProgressWnd wndP( NULL, str );
	wndP.GoModal();
	
	if( m_fullname.IsEmpty() )
		m_command.Format( L"SELECT rowid,* FROM people WHERE sex_id='%s'",  m_sex_id );
	else if( m_first_name.IsEmpty() )
		m_command.Format( L"SELECT rowid,* FROM people WHERE last_name LIKE '%s%c' AND sex_id='%s'", m_last_name,'%', m_sex_id );
	else
		m_command.Format( L"SELECT rowid,* FROM people WHERE last_name='%s' AND first_name='%s'", m_last_name, m_first_name );

	if( !theApp.query2( m_command ) ) return FALSE ;

	wndP.SetRange( 0, theApp.m_recordset2->RecordsCount() );
	wndP.SetPos( 0 );
	wndP.SetStep( 1 );

	int nItem = 0;
	for( UINT i = 0; i < theApp.m_recordset2->RecordsCount(); ++i, theApp.m_recordset2->MoveNext() )
	{
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

		nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset2->GetFieldString( DBP_ROWID ) );
		m_ListCtrl.SetItemText( nItem, L_GENERATION, theApp.m_recordset2->GetFieldString( DBP_GENERATION ) );
		m_ListCtrl.SetItemText( nItem, L_LAST_NAME, theApp.m_recordset2->GetFieldString( DBP_LAST_NAME ) );
		m_ListCtrl.SetItemText( nItem, L_FIRST_NAME, theApp.m_recordset2->GetFieldString( DBP_FIRST_NAME ) );
		m_ListCtrl.SetItemText( nItem, L_BIRTH_PLACE, theApp.m_recordset2->GetFieldString( DBP_BIRTH_PLACE ) );
		m_ListCtrl.SetItemText( nItem, L_BIRTH_DATE, theApp.m_recordset2->GetFieldString( DBP_BIRTH_DATE ) );
		m_ListCtrl.SetItemText( nItem, L_DEATH_PLACE, theApp.m_recordset2->GetFieldString( DBP_DEATH_PLACE ) );
		m_ListCtrl.SetItemText( nItem, L_DEATH_DATE, theApp.m_recordset2->GetFieldString( DBP_DEATH_DATE ) );
		m_ListCtrl.SetItemText( nItem, L_COMMENT, theApp.m_recordset2->GetFieldString( DBP_COMMENT ) );
	}
	m_ListCtrl.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	SetWindowTextW( m_caption );
//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGetPeople::OnBnClickedOk()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	m_rowid			= m_ListCtrl.GetItemText( nItem, L_ROWID );
	m_generation	= m_ListCtrl.GetItemText( nItem, L_GENERATION );
	m_last_name		= m_ListCtrl.GetItemText( nItem, L_LAST_NAME );
	m_first_name	= m_ListCtrl.GetItemText( nItem, L_FIRST_NAME );
	m_birth_place	= m_ListCtrl.GetItemText( nItem, L_BIRTH_PLACE );
	m_birth_date	= m_ListCtrl.GetItemText( nItem, L_BIRTH_DATE );
	m_death_place	= m_ListCtrl.GetItemText( nItem, L_DEATH_PLACE );
	m_death_date	= m_ListCtrl.GetItemText( nItem, L_DEATH_DATE );
	m_comment		= m_ListCtrl.GetItemText( nItem, L_COMMENT );

	m_people.Format( L"%s %s", m_last_name, m_first_name );
	if( !m_birth_date.IsEmpty() )
	{
		m_people += " *";
		m_people += m_birth_date;
	}
	if( !m_death_date.IsEmpty() )
	{
		m_people += " +";
		m_people += m_death_date;
	}
	m_people += " ";
	m_people += m_comment;

	m_people.TrimRight();

	CDialogEx::OnOK();
}
