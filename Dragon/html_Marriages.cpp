// TableMSubstr.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "html_Marriages.h"
#include "afxdialogex.h"
#include "FaDlg.h"
#include "GA_input.h"
#include "GetString.h"


enum
{
	L_ORD = 0,
	L_FAMILY,
	L_TABLE,
	L_LINE,
	L_ORDER,
	L_MARRIAGE,
	L_PLACE,
	L_DATE,
	L_NAME_STR,
	L_BIRTH_STR,
	L_DEATH_STR,
	L_COMMENT,
	L_BRACE,
	L_ARM,
};
IMPLEMENT_DYNAMIC(CMarriages, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMarriages::CMarriages(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMarriages::IDD, pParent)
{
	m_pParent = pParent;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMarriages::~CMarriages()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_CAPTION, m_colorCaption);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMarriages, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)

	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CMarriages::OnDblclkList)
	ON_EN_CHANGE(IDC_SEARCH, &CMarriages::OnChangeSearch)
//	ON_COMMAND(ID_MSUBSTR_NEWTABLE, &CMarriages::OnNewtable)
	ON_COMMAND(ID_MSUBSTR_FILTER, &CMarriages::OnFilter)
	ON_COMMAND(ID_MSUBSTR_UNFILTER, &CMarriages::OnUnfilter)
	ON_COMMAND(ID_MSUBSTR_LIST, &CMarriages::OnList)
	ON_COMMAND(ID_NEWTABLE, &CMarriages::OnNewtable )
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMarriages::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT()

//	menu();

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.SortByHeaderClick(TRUE);

	m_ListCtrl.InsertColumn( L_ORD,				L"#",					LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FAMILY,			L"family#",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLE,			L"table#",				LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINE,			L"line#",				LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ORDER,			L"#",					LVCFMT_LEFT,	 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MARRIAGE,		L"házasság",			LVCFMT_LEFT,    600,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_PLACE,			L"helye",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DATE,			L"ideje",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_NAME_STR,		L"házastárs",			LVCFMT_LEFT,	400,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_STR,		L"született",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_STR,		L"meghalt",				LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_COMMENT,			L"megjegyzés",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BRACE,			L"rokonok",				LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ARM,				L"ág",					LVCFMT_LEFT,	200,-1,COL_TEXT);

	m_colorCaption.SetTextColor( theApp.m_colorClick );
	m_orderix = 0;
	m_filter.Empty();
	fillTable();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::fillTable( )
{
	CString cLine;
	CString pLine;
	CString familyName;
	CString nameSubstring;
	CString marriage;
	CString birthBlock;
	CString deathBlock;
	CString table_name;
	CString order;
	int nItem = 0;
	int	pos;

	
	PEOPLE p;
	NAMEBLOCK	name;
	CGaInput split;
	std::vector<MARRIAGE> vmarriage;
	CString descendant;


	CString logFile;
	logFile.Format( L"splitSpouses.txt" );
	str.Format( L"\"%s\" betöltési hibák", theApp.m_htmlFileSpec ); 

	CString fileName( L"DESC_SPOUSES" );
	CString fileSpec;

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	fwprintf( fh1, L"%s<br><br>", theApp.m_htmlFileSpec );


//	if (( fh1 = theApp.openLogFile( logFile, str )) == NULL)return;


	split.m_error_cnt1 = 0;
	split.m_tableAncestry = TRUE;
	split.m_tableNumber	= 0;

	int fileLength;
	int position;

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
	fileLength = (int)file.GetLength();

	split.m_rollToLine		= m_lineNumber;
	split.m_rollToTable		= m_tableNumber;
	split.m_rollToFamily	= m_familyNumber;

	if( !split.rollFile( &file ) ) return;

	int lineNumber		= split.m_lineNumber;  
	int tableNumber		= split.m_tableNumber;

	if( m_lineNumber )
		str.Format( L"A %d sorban lévõ leszármazott házasságai és azok felbontása", m_lineNumber );
	else if ( m_tableNumber )
		str.Format(L"A %d. %s tábla leszármazottainak házasságai és azok felbontása", m_tableNumber, m_tableName );
	else
		str.Format( L"A fájlban lévõ leszármazottak házasságai és azok felbontása" );

	SetWindowTextW( theApp.m_htmlFileSpec );
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );


	str.Format( L"Leszármazottak házasságainak beolvasása a %s fájlból...", theApp.m_htmlFileName );
	CProgressWnd wndP(NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, fileLength );
	wndP.SetPos(0);
	wndP.SetStep(1);

	m_ListCtrl.DeleteAllItems();
	nItem = 0;

	while(file.ReadString(cLine)) 
	{
		cLine.Trim();
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
		cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
		++lineNumber;
		if(  cLine.Left( 2 ) == L"%%" )
		{
			++tableNumber;
			split.splitTableHeader( cLine );
			if( m_tableNumber )
			{
				if( tableNumber > m_tableNumber ) break;
			}
		}
		if( cLine.Left( 1 ) != L"%" )
		{
			cLine = cleanHtmlLine( cLine );
			if( cLine.IsEmpty() ) continue;
			split.splitLine( cLine );

			for( UINT i = 0; i < split.v_marriages.size(); ++i )
			{
				marriage = split.v_marriages.at(i).marriageSubstr;
				if( m_filter.IsEmpty() || (pos = marriage.Find( m_filter ) ) != -1  )
				{
				
				str.Format( L"%d", nItem+1 );
				m_ListCtrl.InsertItem( nItem, str );
				str.Format( L"%d", lineNumber  );
				m_ListCtrl.SetItemText( nItem, L_LINE, str );
				str.Format( L"%d", tableNumber );
				m_ListCtrl.SetItemText( nItem, L_TABLE, str );

				order.Format( L"%d", split.v_marriages.at(i).order );
				if( !order.Compare( L"0" ) ) order.Empty();
				m_ListCtrl.SetItemText( nItem, L_ORDER, order );
				m_ListCtrl.SetItemText( nItem, L_MARRIAGE, marriage );
				m_ListCtrl.SetItemText( nItem, L_PLACE, split.v_marriages.at(i).place );
				m_ListCtrl.SetItemText( nItem, L_DATE, split.v_marriages.at(i).date );
				m_ListCtrl.SetItemText( nItem, L_NAME_STR, split.v_marriages.at(i).spouseFullname );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_STR, split.v_marriages.at(i).birthSubstr );
				m_ListCtrl.SetItemText( nItem, L_DEATH_STR, split.v_marriages.at(i).deathSubstr );
				m_ListCtrl.SetItemText( nItem, L_COMMENT, split.v_marriages.at(i).comment );
				m_ListCtrl.SetItemText( nItem, L_BRACE, split.v_marriages.at(i).brace );
				m_ListCtrl.SetItemText( nItem, L_ARM, split.v_marriages.at(i).arm );

				++nItem;
				}
			}
		}
		position = (int)file.GetPosition();
		wndP.SetPos( position );
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

		if( m_lineNumber ) break;
	}
	wndP.DestroyWindow();
	fclose( fh1 );
	if( split.m_error_cnt1 )
	{
		ShellExecute(NULL, L"open", fileSpec,NULL, NULL, SW_SHOWNORMAL);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::OnFilter()
{
	CGetString dlg;
	
//	SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	dlg.m_caption = L"Add meg a házasságban keresendõ substringet!";
	if( dlg.DoModal() == IDCANCEL ) return;
	m_filter = dlg.m_string;
	m_filterText.Format( L"A '%s' substringet tartalmazó házasságok", m_filter );
	fillTable();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::OnUnfilter()
{
	m_filter.Empty();
	m_filterText.Format( L"Szûretlen házasságok" );
	fillTable();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::OnList()
{
	CString	logFile(L"marriagesProblems"); 
	CString	title( L"Problémás házasságok" );
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMarriages::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CMarriages::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = (int)wParam;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINE ) );
	theApp.listHtmlLine( lineNumber );

	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::OnChangeSearch()
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.search( search, m_orderix,  &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMarriages::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
//			listHtmlLine();
			return TRUE;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::OnNewtable()
{
//	SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	CFaDlg dlg;
	if( m_lineNumber != 0 )
		dlg.OnMarriagesLine();
	else if( m_tableNumber != 0 )
		dlg.OnMarriagesTable();
	else 
		dlg.OnMarriagesFile();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if(m_pParent)
	{		
		((CFaDlg*)m_pParent)->m_pMarriagesF = NULL;
		((CFaDlg*)m_pParent)->m_pMarriagesT = NULL;
		((CFaDlg*)m_pParent)->m_pMarriagesL = NULL;
	}
	delete this;
}


