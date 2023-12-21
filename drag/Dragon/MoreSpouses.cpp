// MoreSpouses.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "MoreSpouses.h"
#include "afxdialogex.h"


// házastársak
enum
{
	LISTM_LINENUMBER = 0,
	LISTM_ROWIDM,				// házasság azonosító
	LISTM_ORDER,
	LISTM_ROWID,				// házastárs azonosító
	LISTM_NAME,					
	LISTM_BIRTH,
	LISTM_DEATH,
	LISTM_COMMENT,
	LISTM_MARRIAGE
};


// CMoreSpouses dialog

IMPLEMENT_DYNAMIC(CMoreSpouses, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMoreSpouses::CMoreSpouses(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMoreSpouses::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMoreSpouses::~CMoreSpouses()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMoreSpouses::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMoreSpouses, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMoreSpouses::OnBnClickedOk)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMoreSpouses::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString caption;
	if( m_sex_id == MAN )
		caption = L"Válaszd ki a gyerek anyját!";
	else
		caption = L"Válaszd ki a gyerek apját!";

	SetWindowTextW( caption );

	createScreen();
	fillTable();


	return TRUE;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMoreSpouses::createScreen()
{
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );	

	m_ListCtrl.InsertColumn( LISTM_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( LISTM_ROWIDM,		L"házasság",	LVCFMT_RIGHT,	 50,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( LISTM_ORDER,		L"#",			LVCFMT_RIGHT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( LISTM_ROWID,		L"rowid",		LVCFMT_RIGHT,	 50,-1,COL_NUM );
	m_ListCtrl.InsertColumn( LISTM_NAME,		L"név",			LVCFMT_LEFT,	170,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LISTM_BIRTH,		L"született",	LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LISTM_DEATH,		L"meghalt",		LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LISTM_COMMENT,		L"leírás",	LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LISTM_MARRIAGE,	L"házasság",	LVCFMT_LEFT,	120,-1,COL_TEXT);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMoreSpouses::fillTable()
{
	CString lineNumber;
	CString rowidM;
	CString order1;
	CString order2;
	CString rowidS;
	CString	name;
	CString	birth;
	CString	death;
	CString comment;
	CString marriage;
	
	CString husband_id;
	CString	wife_id;
	CString	whichHusband;
	CString	whichWife;
	CString order;
	
	
	CString parentIndex;

	if( m_sex_id == MAN )
		m_command.Format( L"SELECT rowid,* FROM marriages WHERE husband_id=%s ORDER BY whichHusband", m_rowid );
	else
		m_command.Format( L"SELECT rowid,* FROM marriages WHERE wife_id=%s ORDER BY whichWife", m_rowid );
	if( !theApp.query( m_command ) ) return;

	m_ListCtrl.DeleteAllItems();
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		
		rowidM			= theApp.m_recordset->GetFieldString( DBM_ROWID );
		husband_id		= theApp.m_recordset->GetFieldString( DBM_HUSBAND_ID );
		wife_id		= theApp.m_recordset->GetFieldString( DBM_WIFE_ID );
		whichHusband	= theApp.m_recordset->GetFieldString( DBM_WHICHHUSBAND );
		whichWife		= theApp.m_recordset->GetFieldString( DBM_WHICHWIFE );


		if( m_rowid == husband_id )
		{
			m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'",wife_id );
			order = whichWife;
		}
		else
		{
			m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'",husband_id );
			order = whichHusband;
		}
		lineNumber		= theApp.m_recordset->GetFieldString( DBP_LINENUMBER );
		if( !theApp.query2( m_command ) ) return;
		
		
		rowidS		= theApp.m_recordset2->GetFieldString( DBP_ROWID );
		name.Format( L"%s %s", theApp.m_recordset2->GetFieldString( DBP_LAST_NAME ), theApp.m_recordset2->GetFieldString( DBP_FIRST_NAME ) );
		name.Trim();

		birth.Format( L"%s %s", theApp.m_recordset2->GetFieldString( DBP_BIRTH_PLACE ), theApp.m_recordset2->GetFieldString( DBP_BIRTH_DATE ) );
		death.Format( L"%s %s", theApp.m_recordset2->GetFieldString( DBP_DEATH_PLACE ), theApp.m_recordset2->GetFieldString( DBP_DEATH_DATE ) );
		marriage.Format( L"%s %s", theApp.m_recordset->GetFieldString( DBM_PLACE ), theApp.m_recordset->GetFieldString( DBM_DATE ) );
			
		comment		= theApp.m_recordset2->GetFieldString( DBP_COMMENT );

		m_ListCtrl.InsertItem( i, lineNumber );
		m_ListCtrl.SetItemText( i, LISTM_ROWIDM, rowidM );
		m_ListCtrl.SetItemText( i, LISTM_ORDER, order );
		m_ListCtrl.SetItemText( i, LISTM_ROWID, rowidS );
		m_ListCtrl.SetItemText( i, LISTM_NAME, name );
		m_ListCtrl.SetItemText( i, LISTM_BIRTH, birth );
		m_ListCtrl.SetItemText( i, LISTM_DEATH, death );
		m_ListCtrl.SetItemText( i, LISTM_MARRIAGE, marriage );
		m_ListCtrl.SetItemText( i, LISTM_COMMENT, comment );
	}
	m_ListCtrl.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
//	m_order1 = m_ListCtrl.GetItemCount() + 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMoreSpouses::OnBnClickedOk()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	m_rowid		= m_ListCtrl.GetItemText( nItem, LISTM_ROWID );
	m_mother_index = nItem + 1;
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////