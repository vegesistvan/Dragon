// Identical.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "checkParam4.h"
#include "afxdialogex.h"


// CCheckParam4 dialog
enum
{
	L_SORSZ = 0,
	L_DATE,
	L_ALGORITHM,
	L_INPUTP,
	L_UNITEDP,
	L_DELETEDP,
	L_MARRIAGESD
};
IMPLEMENT_DYNAMIC(CCheckParam4, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParam4::CCheckParam4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckParam4::IDD, pParent)
{
	_azonos = 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParam4::~CCheckParam4()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_RADIO_LIST, m_listCtrl);
	DDX_Control(pDX, IDC_RADIO_ALL, m_allCtrl);
	DDX_Control(pDX, IDC_RADIO_LOOP, m_loopCtrl);
	DDX_Control(pDX, IDC_COMBO, m_Combo);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckParam4, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCheckParam4::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_ALL, &CCheckParam4::OnClickedRadioAll)
	ON_COMMAND(IDC_RADIO_ALL1, &CCheckParam4::OnRadioAll1)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckParam4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString caption;

	GetDlgItem( IDC_INFO )->SetWindowText( _info );
	
	caption.Format( L"Emberek azonosságának megállapítása a %s adatbázisban", theApp.m_dbFileName );
	SetWindowTextW( caption );
		

	m_allCtrl.SetCheck( TRUE );								// minden embert vizsgálunk
	m_listCtrl.SetCheck( TRUE );							// html lista
	m_loopCtrl.SetCheck( TRUE );							// ciklus amíg van változás


	GetDlgItem( IDC_RADIO_LIST1 )->EnableWindow( FALSE );  // összevonáskor csak html lista választható
	
	

	m_ListCtrl.SortByHeaderClick(TRUE);

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );
	m_ListCtrl.InsertColumn( L_SORSZ,		L"",					LVCFMT_RIGHT, 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DATE,		L"dátum",				LVCFMT_RIGHT, 90,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ALGORITHM,	L"azonosság száma",		LVCFMT_RIGHT, 110,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_INPUTP,		L"bemeneti emberek",	LVCFMT_RIGHT, 110,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_UNITEDP,		L"összevont emberek",	LVCFMT_RIGHT, 110,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DELETEDP,	L"törölt emberek",		LVCFMT_RIGHT, 85,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MARRIAGESD,	L"törölt házasságok",	LVCFMT_RIGHT, 100,-1,COL_NUM);	


	fillTable();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam4::fillTable()
{
	m_command = L"SELECT rowid,* FROM united";
	if( !theApp.query( m_command ) ) return;

	int nItem;
	
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i )
	{
		str.Format( L"%d", i + 1 );
		nItem = m_ListCtrl.InsertItem( 0, str );
		m_ListCtrl.SetItemText( nItem, L_DATE, theApp.m_recordset->GetFieldString( UNITED_DATE ) );
		m_ListCtrl.SetItemText( nItem, L_ALGORITHM, theApp.m_recordset->GetFieldString( UNITED_MODE ) );
		m_ListCtrl.SetItemText( nItem, L_INPUTP, theApp.m_recordset->GetFieldString( UNITED_INPUT ) );
		m_ListCtrl.SetItemText( nItem, L_UNITEDP, theApp.m_recordset->GetFieldString( UNITED_UNITED ) );
		m_ListCtrl.SetItemText( nItem, L_DELETEDP, theApp.m_recordset->GetFieldString( UNITED_DELETED ) );
		m_ListCtrl.SetItemText( nItem, L_MARRIAGESD, theApp.m_recordset->GetFieldString( UNITED_MARRIAGES ) );
		theApp.m_recordset->MoveNext();
	}

	m_Combo.AddString( L"1" );
	m_Combo.AddString( L"2" );
	m_Combo.AddString( L"3" );
	m_Combo.AddString( L"4" );
	m_Combo.AddString( L"5" );

	m_Combo.SetCurSel( _azonos-1 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam4::OnBnClickedOk()
{
	m_all	= m_allCtrl.GetCheck();
	m_list	= m_listCtrl.GetCheck();
	m_loop	= m_loopCtrl.GetCheck();

	_azonos = m_Combo.GetCurSel() + 1;

	CDialogEx::OnOK();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam4::OnClickedRadioAll()			// minden embert vizsgálunk
{
	CheckRadioButton(IDC_RADIO_LIST, IDC_RADIO_LIST1, IDC_RADIO_LIST);  // html fájl beállítása
	GetDlgItem( IDC_RADIO_LIST )->EnableWindow( FALSE );				// és ezt nem változtathatjuk meg
	GetDlgItem( IDC_RADIO_LIST1 )->EnableWindow( FALSE );				// és ezt nem változtathatjuk meg
	GetDlgItem( IDC_RADIO_LOOP )->EnableWindow( TRUE );					// ciklusok száma választható
}
void CCheckParam4::OnRadioAll1()				// csak a megadott embert vizsgáljuk
{
	GetDlgItem( IDC_RADIO_LIST )->EnableWindow( TRUE );				// mindent választhatunk
	GetDlgItem( IDC_RADIO_LIST1 )->EnableWindow( TRUE );
	GetDlgItem( IDC_RADIO_LOOP )->EnableWindow( TRUE );
}
