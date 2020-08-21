// SelectFamily.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectFamily.h"
#include "afxdialogex.h"
#include "GA_input.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSelectFamily, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectFamily::CSelectFamily(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectFamily::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectFamily::~CSelectFamily()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFamily::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST, m_ListBox);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectFamily, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_BN_CLICKED(IDOK, &CSelectFamily::OnBnClickedOk)
	ON_EN_CHANGE(IDC_SEARCH, &CSelectFamily::OnChangeSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CSelectFamily::OnDblclkList)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectFamily::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CGaInput		split;
	CString		cLine;
	CString		familyNamePrev( L"" );
	int			position;
	int			nItem = 0;

	m_familyNameSelected	= theApp.GetProfileString( L"Settings", L"familyName", L"" );
	m_familyNumberSelected	= theApp.GetProfileInt( L"Settings", L"familyNumber", 1 );

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 

	str.Format( L"Családok a %s fájlban", theApp.m_htmlFileName );
	SetWindowTextW( str );

	CProgressWnd wndP(NULL, L"Családok listázása..."  );
	wndP.GoModal();
	wndP.GoModal();
	wndP.SetRange(0, (int)file.GetLength() );
	wndP.SetPos(0);

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( 0,		L"család#",	LVCFMT_RIGHT,	  50,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( 1,		L"család#",	LVCFMT_RIGHT,	  50,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 2,		L"család neve",	LVCFMT_LEFT, 500,-1,COL_TEXT );

	



	m_familyNumber = 0;
	while(file.ReadString(cLine))
	{
		cLine.Trim();
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
		cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
		if( cLine.Left(3) == L"%%%" )
		{
			++m_familyNumber;
			split.splitTableHeader( cLine );
			str.Format( L"%d", m_familyNumber );

			nItem = m_ListCtrl.InsertItem( 0, L"");
			m_ListCtrl.SetItemText( nItem, 1, str );
			str = split.m_tableHeader.tableHeader;
			str = str.Mid( 4 );
			m_ListCtrl.SetItemText( nItem, 2, str );
		}
		position = (int)file.GetPosition();
		wndP.SetPos( position );
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();

//	m_orderix = 2;
//	theApp.search( m_familyNameSelected, m_orderix,  &m_ListCtrl );

	m_orderix = 1;
	m_ListCtrl.SetSortOrder( 1, m_orderix+1 );
	wndP.DestroyWindow();

	str.Format( L"%d",  m_familyNumberSelected );
	theApp.search( str, m_orderix,  &m_ListCtrl );

	return TRUE;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFamily::OnChangeSearch()
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.search( search, m_orderix,  &m_ListCtrl );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSelectFamily::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CSelectFamily::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFamily::OnBnClickedOk()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	m_familyNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 1 ) );
	m_familyName = m_ListCtrl.GetItemText( nItem, 2 );

	theApp.WriteProfileStringW( L"Settings", L"familyName", m_familyName );
	theApp.WriteProfileInt( L"Settings", L"familyNumber", m_familyNumber );


	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CSelectFamily::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnBnClickedOk();

	*pResult = 0;
}
