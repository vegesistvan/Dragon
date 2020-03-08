// MoreSpouses.cpp : implementation file
//

#include "stdafx.h"
#include "fa.h"
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
	m_ListCtrl.InsertColumn( LISTM_COMMENT,		L"megjegyzés",	LVCFMT_LEFT,	120,-1,COL_TEXT);
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
	
	CString spouse1_id;
	CString	spouse2_id;
	CString	orderHusband;
	CString	orderWife;
	CString order;
	
	
	CString mother_index;

	if( m_sex_id == MAN )
		m_command.Format( L"SELECT rowid,* FROM marriages WHERE spouse1_id=%s ORDER BY orderHusband", m_rowid );
	else
		m_command.Format( L"SELECT rowid,* FROM marriages WHERE spouse2_id=%s ORDER BY orderWife", m_rowid );
	if( !theApp.query( m_command ) ) return;

	m_ListCtrl.DeleteAllItems();
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		
		rowidM			= theApp.m_recordset->GetFieldString( MARRIAGES_ROWID );
		spouse1_id		= theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		spouse2_id		= theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
		orderHusband	= theApp.m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND );
		orderWife		= theApp.m_recordset->GetFieldString( MARRIAGES_ORDERWIFE );


		if( m_rowid == spouse1_id )
		{
			m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'",spouse2_id );
			order = orderWife;
		}
		else
		{
			m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'",spouse1_id );
			order = orderHusband;
		}
		lineNumber		= theApp.m_recordset->GetFieldString( PEOPLE_LINENUMBER );
		if( !theApp.query2( m_command ) ) return;
		
		
		rowidS		= theApp.m_recordset2->GetFieldString( PEOPLE_ROWID );
		name.Format( L"%s %s", theApp.m_recordset2->GetFieldString( PEOPLE_LAST_NAME ), theApp.m_recordset2->GetFieldString( PEOPLE_FIRST_NAME ) );
		name.Trim();

		birth.Format( L"%s %s", theApp.m_recordset2->GetFieldString( PEOPLE_BIRTH_PLACE ), theApp.m_recordset2->GetFieldString( PEOPLE_BIRTH_DATE ) );
		death.Format( L"%s %s", theApp.m_recordset2->GetFieldString( PEOPLE_DEATH_PLACE ), theApp.m_recordset2->GetFieldString( PEOPLE_DEATH_DATE ) );
		marriage.Format( L"%s %s", theApp.m_recordset->GetFieldString( MARRIAGES_PLACE ), theApp.m_recordset->GetFieldString( MARRIAGES_DATE ) );
			
		comment		= theApp.m_recordset2->GetFieldString( PEOPLE_COMMENT );

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