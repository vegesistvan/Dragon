// SameName.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SameName.h"
#include "afxdialogex.h"

enum
{
	N_ROWID = 0,
	N_LAST_NAME,
	N_FIRST_NAME,
	N_BIRTH_PLACE,
	N_BIRTH_DATE,
	N_DEATH_PLACE,
	N_DEATH_DATE,
	N_COMMENT,
	N_ROWID_FATHER,
	N_FATHER,
	N_BIRTH_DATE_FATHER,
	N_ROWID_MOTHER,
	N_MOTHER,
	N_BIRTH_DATE_MOTHER,
};


// CSameName dialog

IMPLEMENT_DYNAMIC(CSameName, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameName::CSameName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSameName::IDD, pParent)
{
	m_rowid.Empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameName::~CSameName()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSameName, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CSameName::OnCustomdrawList)
	ON_BN_CLICKED(IDOK, &CSameName::OnBnClickedOk)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDOK,		ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );
	EASYSIZE_ADD( IDCANCEL,	ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );

	EASYSIZE_INIT();

	CString name;
	name.Format( L"%s %s", m_last_name, m_first_name );
	name.Trim();
	m_caption.Format( L"Több %s nevû ember van az adatbázisban! %s'", name, (CString) m_caption );
	SetWindowText( m_caption );

	str.Format( L"Az alábbi listán sárga színnel az általunk megadni kívánt új ember adatai vannak, amelyet \
az adatbázisban már léterzõ azonos nevû emberek vannak felsorolva.\
\r\n\r\n\
Ha bizos vagy benne, hogy új embert adtál meg, tehát egyik felsorolt emberrel sem egyezik meg, akkor az 'Új ember' \
gombra kattints.\
\r\n\
Ekkor az ój embert az adatbázisba teszi és létrehozza a kijelölt rokoniu kapcsolatot.\
\r\n\r\n\
Ha az általad megadott ember megegyezik valamelyik felsorolt emberrel, akkor jelöld ki azt, majd kattins a \
'Kijelölt ember' bombra.\
\r\n\
Ekkor nem tesz az adatbázisba új embert, csak a kívánt rokoni kapcsolatot létrehozza a ezzel az emberrel,\
\r\n\r\n\
Ha nem vagy biztos egyik fenti esetben sem, akkor kattints a 'Cancel' gombra.\
" );

	GetDlgItem( IDC_EDIT )->SetWindowTextW( str );

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );

	m_ListCtrl.InsertColumn( N_ROWID,				L"rowid",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( N_LAST_NAME,			L"családnév",		LVCFMT_LEFT,	110,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_FIRST_NAME,			L"utónév",			LVCFMT_LEFT,	110,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_BIRTH_PLACE,			L"születés",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_BIRTH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_DEATH_PLACE,			L"elhalálozás",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_DEATH_DATE,			L"ideje",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_COMMENT,				L"megjegyzés",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_ROWID_FATHER,		L"apa_rowid",		LVCFMT_RIGHT,	50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( N_FATHER,				L"apja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_BIRTH_DATE_FATHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_ROWID_MOTHER,		L"anya_rowid",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_MOTHER,				L"anyja",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( N_BIRTH_DATE_MOTHER,	L"birth",			LVCFMT_LEFT,	80,-1,COL_TEXT);

	fillTable();


	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameName::fillTable()
{
	int nItem = 0;
	CString rowid;
	CString last_name;
	CString first_name;

	nItem = m_ListCtrl.InsertItem( nItem, L"" ); 
	m_ListCtrl.SetItemText( nItem, N_LAST_NAME, m_last_name );
	m_ListCtrl.SetItemText( nItem, N_FIRST_NAME, m_first_name );
	m_ListCtrl.SetItemText( nItem, N_BIRTH_PLACE, m_birth_place );
	m_ListCtrl.SetItemText( nItem, N_BIRTH_DATE, m_birth_date );
	m_ListCtrl.SetItemText( nItem, N_DEATH_PLACE, m_death_place );
	m_ListCtrl.SetItemText( nItem, N_DEATH_DATE, m_death_date );
	m_ListCtrl.SetItemText( nItem, N_COMMENT, m_comment );
	m_ListCtrl.SetItemData( nItem, 1 );
	++nItem;
	for( UINT i = 0; i < theApp.m_recordset2->RecordsCount(); ++i, theApp.m_recordset2->MoveNext() )
	{
		rowid		= theApp.m_recordset2->GetFieldString( PEOPLE_ROWID );
		last_name	= theApp.m_recordset2->GetFieldString( PEOPLE_LAST_NAME );
		first_name	= theApp.m_recordset2->GetFieldString( PEOPLE_FIRST_NAME );
		nItem = m_ListCtrl.InsertItem( nItem, rowid );
		m_ListCtrl.SetItemText( nItem, N_LAST_NAME, last_name );
		m_ListCtrl.SetItemText( nItem, N_FIRST_NAME, first_name );
		m_ListCtrl.SetItemText( nItem, N_BIRTH_PLACE, theApp.m_recordset2->GetFieldString( PEOPLE_BIRTH_PLACE ) );
		m_ListCtrl.SetItemText( nItem, N_BIRTH_DATE, theApp.m_recordset2->GetFieldString( PEOPLE_BIRTH_DATE ) );
		m_ListCtrl.SetItemText( nItem, N_DEATH_PLACE, theApp.m_recordset2->GetFieldString( PEOPLE_DEATH_PLACE ) );
		m_ListCtrl.SetItemText( nItem, N_DEATH_DATE, theApp.m_recordset2->GetFieldString( PEOPLE_DEATH_DATE ) );
		m_ListCtrl.SetItemText( nItem, N_COMMENT, theApp.m_recordset2->GetFieldString( PEOPLE_COMMENT ) );

		m_ListCtrl.SetItemData( nItem, 0 );
	}
	m_ListCtrl.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameName::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSameName::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameName::OnBnClickedOk()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == - 1 )
	{
		theApp.message( L"", L"Nincs kijelölve ember!" );
		return;
	} 
	m_rowid = m_ListCtrl.GetItemText( nItem, N_ROWID );
	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameName::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	int iData;

	*pResult = 0;

	switch( pLVCD->nmcd.dwDrawStage )
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		nItem	= pLVCD->nmcd.dwItemSpec;
		nCol	= pLVCD->iSubItem;
		iData	= m_ListCtrl.GetItemData( nItem );
		if( iData  )
		{
			pLVCD->clrTextBk = YELLOW;
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
