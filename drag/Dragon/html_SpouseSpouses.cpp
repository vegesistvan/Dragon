// TableSS.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_SpouseSpouses.h"
#include "afxdialogex.h"
#include "InputGA.h"
#include "editHtmlLine.h"
#include "ProgressWnd.h"
#include "utilities.h"

enum
{
	L_ID = 0,
	L_LINE_NUMBER,
	L_TABLE_NUMBER,
	L_TITLE,
	L_TITOLO,
	L_FAMILYNAME,
	L_FIRST_NAME,
	L_BIRTH_PLACE,
	L_BIRTH_DATE,
	L_SPOUSES,
	L_ORDERNUMBER,
	L_ROWID,
	L_TITLE_SP,
	L_TITOLO_SP,
	L_LAST_NAME_SP,
	L_FIRST_NAME_SP,
	L_BIRTH_PLACE_SP,
	L_BIRTH_DATE_SP,
	L_DEATH_PLACE_SP,
	L_DEATH_DATE_SP,
	L_COMMENT 
};

IMPLEMENT_DYNAMIC(CSpouseSpouses, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
CSpouseSpouses::CSpouseSpouses(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpouseSpouses::IDD, pParent)
	, m_search(_T(""))
{
	m_pParent = pParent;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
CSpouseSpouses::~CSpouseSpouses()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
void CSpouseSpouses::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Text(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_CAPTION, m_colorCaption);

	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeress);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
BEGIN_MESSAGE_MAP(CSpouseSpouses, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CSpouseSpouses::OnDblclkList)
	ON_COMMAND(ID_NEWTABLE, &CSpouseSpouses::OnNewtable)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CSpouseSpouses::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CSpouseSpouses::OnHtmlNotepad)

	ON_STN_CLICKED(IDC_STATIC_KERESS, &CSpouseSpouses::OnClickedKeress)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
BOOL CSpouseSpouses::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	menu();

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_colorCaption.SetTextColor( theApp.m_colorClick );

	SetWindowTextW( theApp.m_htmlPathName );
	str.Format( L"A leszármazott házastársának további házasságai" );
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( L_ID,				L"#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINE_NUMBER,		L"line#",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLE_NUMBER,	L"table#",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TITLE,			L"rang",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_TITOLO,			L"elõnév",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FAMILYNAME,		L"családnév",	LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FIRST_NAME,		L"utónév",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_PLACE,		L"született",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_DATE,		L"dátum",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SPOUSES,			L"házastársak",	LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ORDERNUMBER,		L"#",			LVCFMT_RIGHT,	 40,-1,COL_NUM );
	m_ListCtrl.InsertColumn(L_ROWID, L"rowid", LVCFMT_RIGHT, 40, -1, COL_NUM);
	m_ListCtrl.InsertColumn( L_TITLE_SP,		L"házastárs",	LVCFMT_LEFT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_TITOLO_SP,		L"elõnév",		LVCFMT_LEFT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LAST_NAME_SP,	L"családnév",	LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FIRST_NAME_SP,	L"utóneve",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_PLACE_SP,	L"születés",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_DATE_SP,	L"ideje",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_PLACE_SP,	L"meghalt",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_DATE_SP,	L"ideje",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_COMMENT,			L"leírás",		LVCFMT_LEFT,	 80,-1,COL_TEXT);

	m_orderix = 0;

	fillTable();


	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSpouseSpouses::fillTable( )
{

	typedef struct
	{
	int		number;
	int		sex_id;
	CString title;
	CString titolo;
	CString last_name;
	CString first_name;
	CString comment;
	}SPOUSES1;

	//CGaInput split;
	CInputGA split;
	PEOPLE p;

	CString cLine;
	CString table_name;
	CString spouses;
	CString relatives;
	CString spouse;
	CString condition(L"");
	CString familyName;
	int nItem	= 0;

	CString logFile( L"spsp.txt" );

	CString descendant;
	int fileLength;
	int position;

	theApp.m_inputCode = GetInputCode( m_htmlPathName );
	CStdioFile file( m_htmlPathName, CFile::modeRead); 
	fileLength = (int)file.GetLength();

	split.m_rollToLine		= m_lineNumber;
	split.m_rollToTable		= m_tableNumber;
	split.m_rollToFamily	= m_familyNumber;

	if( !split.rollFile( &file ) ) return;

	int lineNumber		= split.m_lineNumber;  
	int tableNumber		= split.m_tableNumber;


	if( m_lineNumber )
		str.Format( L"A fájl %d sorában lévõ leszármazott házastársainak további házassságai és azok felbontása", m_lineNumber );
	else if ( m_tableNumber )
		str.Format(L"A fájl %d. %s táblájában lévõ leszármazottak házastársainak további házastársai és azok felbontása", m_tableNumber, m_tableName );
	else
		str.Format( L"A fájlban lévõ leszármazottak házastársainak további házasságai és azok felbontása" );
	

	CString fileName( L"SPOUSE_SPOUSES" );
	CString fileSpec;


	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	fwprintf( fh1, L"%s<br><br>", m_htmlPathName );

//	if ((fh1 = theApp.openLogFile( logFile, str )) == NULL)return;

	SetWindowTextW( m_htmlPathName );
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );
	
	str.Format( L"%s fájból a házastársak házastársainak beolvasása", theApp.m_htmlFileName );
	CProgressWnd  wndProgress(NULL, str );
	wndProgress.GoModal();
	wndProgress.SetRange(0, fileLength );
	wndProgress.SetPos(0);

	int numberOfTables = 0;
//	split.m_error_cnt1 = 0;
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
			if (m_numberOfTables)
			{
				if (numberOfTables == m_numberOfTables) break;
				++numberOfTables;
			}
			else if (m_tableNumber)
			{
				if (tableNumber > m_tableNumber) break;
			}
		}
		else if( cLine.Left( 1 ) != L"%" )
		{
			cLine = cleanHtmlLine( cLine );
			if( cLine.IsEmpty() ) continue;
			split.splitLine( cLine );

			UINT i = 0;

			for( UINT j = 0; j < split.v_spouseSpouses.size(); ++j )
			{
				i = split.v_spouseSpouses.at(j).spouseIndex;

				str.Format( L"%d", nItem+1 );
				m_ListCtrl.InsertItem(nItem, str);
				str.Format(L"%d", lineNumber );
				m_ListCtrl.SetItemText( nItem, L_LINE_NUMBER, str );
				str.Format( L"%d", tableNumber );
				m_ListCtrl.SetItemText( nItem, L_TABLE_NUMBER, str );

				
//				if( i > (split.v_marriages.size() - 1) )
//					int x = 12;


				m_ListCtrl.SetItemText( nItem, L_TITLE, split.v_marriages.at(i).title );
				m_ListCtrl.SetItemText( nItem, L_TITOLO, split.v_marriages.at(i).titolo );
				m_ListCtrl.SetItemText( nItem, L_FAMILYNAME, split.v_marriages.at(i).last_name );
				m_ListCtrl.SetItemText( nItem, L_FIRST_NAME, split.v_marriages.at(i).first_name );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_PLACE, split.v_marriages.at(i).birth_place );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_DATE, split.v_marriages.at(i).birth_date );
				m_ListCtrl.SetItemText( nItem, L_SPOUSES, split.v_marriages.at(i).spouses );
		
				str.Format( L"%d", split.v_spouseSpouses.at(j).order );
				m_ListCtrl.SetItemText( nItem, L_ORDERNUMBER, str );
				m_ListCtrl.SetItemText(nItem, L_ROWID, split.v_spouseSpouses.at(j).rowid);
				m_ListCtrl.SetItemText( nItem, L_TITLE_SP, split.v_spouseSpouses.at(j).title );
				m_ListCtrl.SetItemText( nItem, L_TITOLO_SP, split.v_spouseSpouses.at(j).titolo );
				m_ListCtrl.SetItemText( nItem, L_LAST_NAME_SP, split.v_spouseSpouses.at(j).last_name );
				m_ListCtrl.SetItemText( nItem, L_FIRST_NAME_SP, split.v_spouseSpouses.at(j).first_name );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_PLACE_SP, split.v_spouseSpouses.at(j).birth_place );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_DATE_SP, split.v_spouseSpouses.at(j).birth_date );
				m_ListCtrl.SetItemText( nItem, L_DEATH_PLACE_SP, split.v_spouseSpouses.at(j).death_place );
				m_ListCtrl.SetItemText( nItem, L_DEATH_DATE_SP, split.v_spouseSpouses.at(j).death_date );
				m_ListCtrl.SetItemText( nItem, L_COMMENT, split.v_spouseSpouses.at(j).comment );

				++nItem;
			}

		}

		if( m_lineNumber ) break;
		position = (int)file.GetPosition();
		wndProgress.SetPos( position );
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	file.Close();
	wndProgress.DestroyWindow();
	fclose( fh1 );
/*
	if( split.m_error_cnt1 )
		ShellExecute(NULL, L"open", fileSpec,NULL, NULL, SW_SHOWNORMAL);
//		theApp.showLogFile();
*/
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSpouseSpouses::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CSpouseSpouses::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = (int)wParam;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSpouseSpouses::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINE_NUMBER ) );
	theApp.listHtmlLine( lineNumber );

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSpouseSpouses::OnNewtable()
{
//	DestroyWindow();
	/*CDragonDlg dlg;
	if( m_lineNumber != 0 )
		dlg.OnSsLine();
	else if( m_tableNumber != 0 )
		dlg.OnSsTable();
	else 
		dlg.OnSsFile();*/
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSpouseSpouses::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR;

	MENU_IDR = IDR_DROPDOWN_HTML;
	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);

		pPopup->EnableMenuItem(ID_DB_EDIT,MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_NOTEPAD_PARENTS,MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_FATHERANDSIBLINGS,MF_BYCOMMAND | MF_GRAYED);
		
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSpouseSpouses::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINE_NUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSpouseSpouses::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINE_NUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( m_htmlPathName, lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSpouseSpouses::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSpouseSpouses::OnClickedKeress()
{
	keress(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSpouseSpouses::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////