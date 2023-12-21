// TwoIdent.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SameParams.h"
#include "afxdialogex.h"

enum
{
	L_HIDDEN = 0,
	L_CNT,
	L_ROWID,
	L_FAMILYNUMBER,
	L_TABLENUMBER,
	L_LINENUMBER,
	L_SOURCE,
	L_GENERATION,
	L_BIRTH,
	L_DEATH,
	L_FATHER,
	L_MOTHER,
	L_SPOUSES,
};

IMPLEMENT_DYNAMIC(CSameParams, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameParams::CSameParams(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSameParams::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameParams::~CSameParams()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameParams::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSameParams, CDialogEx)
	ON_MESSAGE(WM_SET_COMBO, OnCreateComboList )
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CSameParams::OnDblclkList)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameParams::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//  SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	CString	rowid;
	CString father_id;
	CString mother_id;
	CString father;
	CString mother;
	CString spouse;
	SPOUSES2 sp;

	str.Format( L"%s %s nevû emberek és az azonosításhoz felhasznált adatai", m_lastName, m_firstName );
	SetWindowTextW( str );

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_HIDDEN,		L"cnt",			LVCFMT_RIGHT,	 20,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"family#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_LEFT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halálozás",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,		L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHER,		L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SPOUSES,		L"házastársak",	LVCFMT_LEFT,	200,-1,COL_COMBO );

	m_command.Format( L"SELECT rowid, * FROM people WHERE first_name='%s' AND last_name='%s'", m_firstName, m_lastName );
	if( !theApp.query( m_command ) ) return FALSE;

	m_sex_id = theApp.m_recordset->GetFieldString( DBP_SEX_ID );
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		str.Format( L"%d", i+1 );
		m_ListCtrl.InsertItem( i, str );
		m_ListCtrl.SetItemText( i, L_CNT, str );
		rowid = theApp.m_recordset->GetFieldString( DBP_ROWID );
		m_ListCtrl.SetItemText( i, L_ROWID, rowid );
		m_ListCtrl.SetItemText( i, L_FAMILYNUMBER, theApp.m_recordset->GetFieldString( DBP_FAMILYNUMBER ) );
		m_ListCtrl.SetItemText( i, L_TABLENUMBER, theApp.m_recordset->GetFieldString( DBP_TABLENUMBER ) );
		m_ListCtrl.SetItemText( i, L_LINENUMBER, theApp.m_recordset->GetFieldString( DBP_LINENUMBER ) );
		m_ListCtrl.SetItemText( i, L_SOURCE, theApp.m_recordset->GetFieldString( DBP_SOURCE ) );
		m_ListCtrl.SetItemText( i, L_GENERATION, theApp.m_recordset->GetFieldString( DBP_GENERATION ) );
		m_ListCtrl.SetItemText( i, L_BIRTH, theApp.m_recordset->GetFieldString( DBP_BIRTH_DATE ) );
		m_ListCtrl.SetItemText( i, L_DEATH, theApp.m_recordset->GetFieldString( DBP_DEATH_DATE ) );

		father_id	= theApp.m_recordset->GetFieldString( DBP_FATHER_ID );
		mother_id	= theApp.m_recordset->GetFieldString( DBP_MOTHER_ID );

		m_command.Format( L"SELECT first_name, last_name FROM people WHERE rowid='%s'", father_id );
		if( !theApp.query1( m_command ) ) return FALSE;
		father.Format( L"%s %s", theApp.m_recordset1->GetFieldString( 1 ), theApp.m_recordset1->GetFieldString( 0 ) );
		m_ListCtrl.SetItemText( i, L_FATHER, father );
		
		m_command.Format( L"SELECT first_name, last_name FROM people WHERE rowid='%s'", mother_id );
		if( !theApp.query1( m_command ) ) return FALSE;
		mother.Format( L"%s %s", theApp.m_recordset1->GetFieldString( 1 ), theApp.m_recordset1->GetFieldString( 0 ) );
		m_ListCtrl.SetItemText( i, L_MOTHER, mother );

		
		
		theApp.getSpouses( rowid, m_sex_id, &theApp.v_spouses, &sp );
		if( theApp.v_spouses.size() )
		{
			str.Format( L"%s", theApp.v_spouses.at( 0 ).name );
			if( theApp.v_spouses.size() > 1 )
				str += L"...";
			m_ListCtrl.SetItemText( i, L_SPOUSES, str );
		}
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSameParams::OnCreateComboList(WPARAM wParam, LPARAM lParam) //wparam az oszlop sorszama, lparam: a COMBODATA stuktura
{
	COMBODATA* pComboData=(COMBODATA*) lParam;
	SPOUSES2 sp;

	if( wParam==L_SPOUSES )
	{
		int		nItem = pComboData->nItem;
		CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

		theApp.getSpouses( rowid, m_sex_id, &theApp.v_spouses, &sp );

		for( UINT i = 0; i < theApp.v_spouses.size(); ++i )
		{
			pComboData->m_strList.AddTail( theApp.v_spouses.at(i).name ); 
		}
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameParams::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );

	*pResult = 0;
}
