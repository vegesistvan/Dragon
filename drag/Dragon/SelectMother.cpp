// SelectMother.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectMother.h"
#include "afxdialogex.h"
#include "GetPeople.h"

enum {
	L_DUMMY = 0,
	L_ROWID,
	L_MOTHER,
	L_BIRTH,
	L_DEATH
};
IMPLEMENT_DYNAMIC(CSelectMother, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectMother::CSelectMother(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectMother::IDD, pParent)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectMother::~CSelectMother()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectMother::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectMother, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectMother::OnBnClickedOk)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectMother::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	m_ListCtrl.SortByHeaderClick( TRUE );
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( L_DUMMY,	L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_ROWID,	L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,	L"apa",			LVCFMT_LEFT,	170,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH,	L"születés",	LVCFMT_LEFT,	 75,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH,	L"halál",		LVCFMT_LEFT,	 75,-1,COL_TEXT);

	int nItem = 0;
	CString rowid;
	CString mother;
	CString birth;
	CString death;

	m_rowidM.Empty();

	// ha az apa nem ismert
	if( m_rowidF.IsEmpty() || !m_rowidF.Compare( L"0" ) )
	{
		ShowWindow(SW_HIDE);
		CGetPeople dlg;
		dlg.m_caption = L"Választ ki az anyát!";
		dlg.m_sex_id = WOMAN;
		if( dlg.DoModal() == IDCANCEL) 
			OnCancel();

		m_rowidM	= dlg.m_rowid;
		m_mother	= dlg.m_people;

		m_command.Format( L"SELECT husband_id FROM marriages WHERE wife_id='%s'", m_rowidM );
		if( !theApp.query( m_command ) ) return FALSE;

		m_numberOfWifes = theApp.m_recordset->RecordsCount();

		if( theApp.m_recordset->RecordsCount() < 2 )
		{
			m_rowidF = theApp.m_recordset->GetFieldString( 0 );
			CDialogEx::OnOK();
		}	
		else
		{
			for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
			{
				m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'", theApp.m_recordset->GetFieldString( 0 ) );
				if( !theApp.query2( m_command ) ) return FALSE;

				nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset2->GetFieldString( DBP_ROWID ) );
				m_ListCtrl.SetItemText( nItem, L_ROWID, theApp.m_recordset2->GetFieldString( DBP_ROWID ) );
				mother.Format( L"%s %s", theApp.m_recordset2->GetFieldString( DBP_LAST_NAME ), theApp.m_recordset2->GetFieldString( DBP_FIRST_NAME ) );
				m_ListCtrl.SetItemText( nItem, L_MOTHER, mother );
				birth = theApp.m_recordset2->GetFieldString( DBP_BIRTH_DATE );
				m_ListCtrl.SetItemText( nItem, L_BIRTH, birth );
				death = theApp.m_recordset2->GetFieldString( DBP_DEATH_DATE );
				m_ListCtrl.SetItemText( nItem, L_DEATH, death );
			}
			m_ListCtrl.SetSortOrder( 2, 3,4 );
		}
//		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		m_command.Format( L"SELECT wife_id FROM marriages WHERE husband_id='%s'", m_rowidF );
		if( !theApp.query( m_command ) ) return FALSE;

		m_numberOfWifes = theApp.m_recordset->RecordsCount();

		if( theApp.m_recordset->RecordsCount() < 2 )
		{
			m_rowidM = theApp.m_recordset->GetFieldString( 0 );
			CDialogEx::OnOK();
		}	
		else
		{
			for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
			{
				m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'", theApp.m_recordset->GetFieldString( 0 ) );
				if( !theApp.query2( m_command ) ) return FALSE;

				nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset2->GetFieldString( DBP_ROWID ) );
				m_ListCtrl.SetItemText( nItem, L_ROWID, theApp.m_recordset2->GetFieldString( DBP_ROWID ) );
				mother.Format( L"%s %s", theApp.m_recordset2->GetFieldString( DBP_LAST_NAME ), theApp.m_recordset2->GetFieldString( DBP_FIRST_NAME ) );
				m_ListCtrl.SetItemText( nItem, L_MOTHER, mother );
				birth = theApp.m_recordset2->GetFieldString( DBP_BIRTH_DATE );
				m_ListCtrl.SetItemText( nItem, L_BIRTH, birth );
				death = theApp.m_recordset2->GetFieldString( DBP_DEATH_DATE );
				m_ListCtrl.SetItemText( nItem, L_DEATH, death );
			}
			m_ListCtrl.SetSortOrder( 2, 3,4 );
		}
//		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectMother::OnBnClickedOk()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	m_rowidM = m_ListCtrl.GetItemText( nItem, L_ROWID );
	if( m_numberOfWifes == 1 ) 
		m_mother_index = 0;
	else
		m_mother_index = nItem + 1;

	CDialogEx::OnOK();
}
