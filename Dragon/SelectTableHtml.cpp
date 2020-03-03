// SelectTable.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "SelectTableHtml.h"
#include "afxdialogex.h"
#include "GA_input.h"


// CSelectTable dialog

IMPLEMENT_DYNAMIC(CSelectTable, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectTable::CSelectTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectTable::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectTable::~CSelectTable()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectTable, CDialogEx)
	ON_EN_CHANGE(IDC_SEARCH, &CSelectTable::OnChangeSearch)
	ON_BN_CLICKED(IDOK, &CSelectTable::OnBnClickedOk)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CSelectTable::OnDblclkList)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSelectTable::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor= (CELLCOLOR*) lParam;//a COLUMNCOLOR struktura deklaralva van a ListCtrlEx.h -ban.

	if(m_orderix==(int)wParam)
	{
		cColor->bg	= RGB(250,250,250);
		cColor->tx	= RGB(250,0,0);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSelectTable::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString		cLine;
	CString		tableNumber;
	int			position;
	int			cnt = 0;
	int			nItem = 0;
	CGaInput	split;

	m_tableNameSelected		= theApp.GetProfileString( L"Settings", L"tableNameSelected", L"" );
	m_tableNumberSelected	= theApp.GetProfileInt( L"Settings", L"tableNumber", 1 );

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
	


	str.Format( L"Táblák a %s fájlban", theApp.m_htmlFileSpec );
	SetWindowTextW( str );

	CProgressWnd wndP(NULL, L"Táblák listázása..."  );
	wndP.GoModal();
	wndP.GoModal();
	wndP.SetRange(0, (int)file.GetLength() );
	wndP.SetPos(0);

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( 0,		L"",			LVCFMT_RIGHT,	 50,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( 1,		L"table#",		LVCFMT_RIGHT,	 50,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 2,		L"%%",			LVCFMT_RIGHT,	 50,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 3,		L"tábla neve",	LVCFMT_LEFT,	500,-1,COL_TEXT );

	
	
	int pos;
	CString percent;
	while(file.ReadString(cLine))
	{
		cLine.Trim();
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );

		cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
		if( cLine.Left(2) == L"%%" )
		{
			
			++cnt;
			tableNumber.Format( L"%d", cnt );

			nItem = m_ListCtrl.InsertItem( 0,  L"" );
			m_ListCtrl.SetItemText( nItem, 1, tableNumber );

			split.splitTableHeader( cLine ); 
			str = split.m_tableHeader.tableHeader;
			percent = getWord( str, 1, &pos );
			m_ListCtrl.SetItemText( nItem, 2, percent );

			str.Replace( '%', ' ' );
			str.Trim();
			m_ListCtrl.SetItemText( nItem, 3, str );
		}

		position = (int)file.GetPosition();
		wndP.SetPos( position );
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	m_orderix = 1;
	m_ListCtrl.SetSortOrder( 1, m_orderix+1 );
	wndP.DestroyWindow();

	str.Format( L"%d", m_tableNumberSelected );
	theApp.search( str, m_orderix,  &m_ListCtrl );
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectTable::OnChangeSearch()
{

	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.search( search, m_orderix,  &m_ListCtrl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectTable::OnBnClickedOk()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	m_tableNumberSelected	= _wtoi( m_ListCtrl.GetItemText( nItem, 1 ) );
	m_tableNameSelected		= m_ListCtrl.GetItemText( nItem, 3 );

	theApp.WriteProfileStringW( L"Settings", L"tableNameSelected", m_tableNameSelected );
	theApp.WriteProfileInt( L"Settings", L"tableNumber", m_tableNumberSelected );

	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectTable::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	OnBnClickedOk();
	*pResult = 0;
}
