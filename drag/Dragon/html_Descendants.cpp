// Descendant.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_Descendants.h"
#include "afxdialogex.h"
#include "inputGA.h"
#include "editHtmlLine.h"
#include "ProgressWnd.h"
#include "utilities.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	PS_ID=0,
	PS_LINENUMBER,
	PS_TABLENUMBER,
	PS_PEOPLE,
	PS_NAME_STR,
	PS_BIRTH_STR,
	PS_DEATH_STR,
	PS_COMMENT,
	PS_PEER,
	PS_TITOLO,
	PS_TITLE,
	PS_LAST_NAME,
	PS_FIRST_NAME,
	PS_POSTERIOR,
	PS_MOTHER_INDEX,
	PS_MINDEX_CALC,
	PS_SEX_ID,
	PS_BPLACE,
	PS_BDATE,
	PS_DPLACE,
	PS_DDATE,
	PS_BRANCH,
	PS_ARM,
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CHtmlDescendants, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlDescendants::CHtmlDescendants(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlDescendants::IDD, pParent)
{
	m_pParent = pParent;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlDescendants::~CHtmlDescendants()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlDescendants::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_CAPTION, colorCaption);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CHtmlDescendants, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CHtmlDescendants::OnDblclkList)
	ON_COMMAND(ID_NEWTABLE, &CHtmlDescendants::OnNewtable)
	ON_COMMAND(ID_D_LIST, &CHtmlDescendants::OnDList)
	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CHtmlDescendants::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CHtmlDescendants::OnHtmlNotepad)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CHtmlDescendants::OnClickedKeress)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlDescendants::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

//	menu();
	
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.SortByHeaderClick(TRUE);

	m_ListCtrl.InsertColumn( PS_ID,				L"#",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( PS_LINENUMBER,		L"line#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( PS_TABLENUMBER,	L"table#",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( PS_PEOPLE,			L"leszármazott",	LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_NAME_STR,		L"nameSubString",	LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_BIRTH_STR,		L"birthSubstring",	LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_DEATH_STR,		L"deathSubstring",	LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_COMMENT,		L"leírás",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_PEER,			L"peer",			LVCFMT_LEFT, 50, -1, COL_TEXT);
	m_ListCtrl.InsertColumn( PS_TITOLO,			L"elõnév",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_TITLE,			L"title",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_LAST_NAME,		L"családnév",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_FIRST_NAME,		L"utónév",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_POSTERIOR,		L"utótag",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_MOTHER_INDEX,	L"anyaIx",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_MINDEX_CALC,	L"anyaIx2",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_SEX_ID,			L"sex",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_BPLACE,			L"b.hely",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_BDATE,			L"b.dátum",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_DPLACE,			L"d.hely",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_DDATE,			L"d.dátum",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_BRANCH,			L"elágazás",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_ARM,			L"ág",				LVCFMT_LEFT,	 80,-1,COL_TEXT);

	colorCaption.SetTextColor( theApp.m_colorClick );
	m_orderix = 0;
	fillTable();

	m_orderix = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlDescendants::fillTable( )
{
	CString cLine;
	CString pLine;
	CString familyName;
	CString nameSubstring;
	CString birthBlock;
	CString deathBlock;
	CString folyt;
	CString table_name;
	int nItem = 0;
	std::vector<MARRIAGE> v_marriages;
	PEOPLE p;
	NAMEBLOCK	name;
	std::vector<MARRIAGE> vmarriage;
	CString descendant;
	

	CString logFile;
	logFile.Format( L"psplit.txt" );
	str.Format( L"\"%s\" betöltési hibák", m_htmlPathName ); 
	CString fileName( L"DESCENDANTS" );
	CString fileSpec;

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	fwprintf( fh1, L"%s<br><br>", m_htmlPathName ); 


	CInputGA split;
	split.m_tableAncestry = TRUE;

	

	int fileLength;
	int position;

	theApp.m_inputCode = GetInputCode( m_htmlPathName );
	CStdioFile file( m_htmlPathName, CFile::modeRead); 
	fileLength = (int)file.GetLength();

	split.m_rollToLine		= m_lineNumber;
	split.m_rollToTable		= m_tableNumber;
	split.m_rollToFamily	= 0;
	

	if( !split.rollFile( &file ) ) return;

	int lineNumber		= split.m_lineNumber;  
	int tableNumber		= split.m_tableNumber;


	if( m_lineNumber )
	{
		str.Format( L"A fájl %d sorában lévõ leszármazott és annak felbontása", m_lineNumber );
	}
	else if ( m_tableNumber )
		str.Format(L"A fájl %d. %s táblájának leszármazottai és azok felbontása", m_tableNumber, m_tableName );
	else
		str.Format( L"A fájlban lévõ leszármazottak és azok felbontása" );

	SetWindowTextW( m_htmlPathName );
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );


	str.Format( L"%s fájlban lévõ leszármazottak beolvasása...", m_htmlFileName );
	CProgressWnd wndP(NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, fileLength );
	wndP.SetPos(0);
	wndP.SetStep(1);


	m_ListCtrl.DeleteAllItems();
	nItem = 0;
	int numberOfTables = 0;
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
		else if( cLine.Left( 1) != L"%" )
		{
			cLine = cleanHtmlLine( cLine );
			if( cLine.IsEmpty() ) continue;

			split.splitLine( cLine );		// 

			str.Format( L"%d", nItem+1 );
			m_ListCtrl.InsertItem( nItem, str );
			str.Format( L"%d", lineNumber );
			m_ListCtrl.SetItemText( nItem, PS_LINENUMBER, str );
			str.Format( L"%d", tableNumber );
			m_ListCtrl.SetItemText( nItem, PS_TABLENUMBER, str );

			m_ListCtrl.SetItemText( nItem, PS_PEOPLE, split.m_descendant );

			m_ListCtrl.SetItemText( nItem, PS_NAME_STR, split.descS.nameSubstr );
			m_ListCtrl.SetItemText( nItem, PS_BIRTH_STR, split.descS.birthSubstr );
			m_ListCtrl.SetItemText( nItem, PS_DEATH_STR, split.descS.deathSubstr );
			m_ListCtrl.SetItemText( nItem, PS_COMMENT, split.d.comment );
			m_ListCtrl.SetItemText(nItem,  PS_PEER, split.d.peer );
			m_ListCtrl.SetItemText(nItem, PS_TITOLO, split.d.titolo);
			m_ListCtrl.SetItemText( nItem, PS_TITLE, split.d.title );
			m_ListCtrl.SetItemText( nItem, PS_LAST_NAME, split.d.last_name );
			m_ListCtrl.SetItemText( nItem, PS_FIRST_NAME, split.d.first_name );
			m_ListCtrl.SetItemText( nItem, PS_POSTERIOR, split.d.posterior );
			str.Format( L"%d", split.d.parentIndex );
			if( !str.Compare( L"0" ) ) str.Empty();
			m_ListCtrl.SetItemText( nItem, PS_MOTHER_INDEX, str );

			str.Format( L"%d", split.d.parentIndex );
			if( !str.Compare( L"0" ) ) str.Empty();
			m_ListCtrl.SetItemText( nItem, PS_MINDEX_CALC, str );
			m_ListCtrl.SetItemText( nItem, PS_SEX_ID, split.d.sex_id );
			m_ListCtrl.SetItemText( nItem, PS_BPLACE, split.d.birth_place );
			m_ListCtrl.SetItemText( nItem, PS_BDATE, split.d.birth_date );
			m_ListCtrl.SetItemText( nItem, PS_DPLACE, split.d.death_place );
			m_ListCtrl.SetItemText( nItem, PS_DDATE, split.d.death_date );
			m_ListCtrl.SetItemText( nItem, PS_BRANCH, split.d.folyt );
			m_ListCtrl.SetItemText( nItem, PS_ARM, split.d.arm );

			++nItem;

			position = (int)file.GetPosition();
			wndP.SetPos( position );
			wndP.StepIt();
			wndP.PeekAndPump();
			if (wndP.Cancelled()) break;
		}
		if( m_lineNumber ) break;
	}
	wndP.DestroyWindow();
	fclose( fh1 );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CHtmlDescendants::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CHtmlDescendants::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlDescendants::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	PS_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );



//	listHtmlLine();
	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlDescendants::listHtmlLine()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( nItem == - 1 )
	{
		AfxMessageBox( L"No selected file!" );
		return;
	}
	
	if( _waccess( m_htmlPathName, 0 ) )
	{
		AfxMessageBox( L"file does not exist!" );
		return;
	}

	int htmlLineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	PS_LINENUMBER ) );
	CEditHtmlLine dlg;

	theApp.m_inputCode = GetInputCode( m_htmlPathName );
	CStdioFile file( m_htmlPathName, CFile::modeRead); 
	CString cLine;
	
	for( int i = 0; i < htmlLineNumber; ++i ) file.ReadString( cLine );

	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
	if( cLine.GetAt(0) != '%' || cLine.Left(3) == L"%%%" )
	{
		cLine = cleanHtmlLine( cLine );
	}
	dlg.m_line = cLine;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlDescendants::OnDList()
{
	CString	logFile(L"decendants"); 
	CString	title( L"Kijelölt leszármazottak" );
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlDescendants::OnNewtable()
{
	//CDragonDlg dlg;
	//if( m_lineNumber != 0 )
	//	dlg.OnDescendantLine();
	//else if( m_tableNumber != 0 )
	//	dlg.OnDescendantTable();
	//else 
	//	dlg.OnDescendantFile();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CHtmlDescendants::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CHtmlDescendants::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	PS_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlDescendants::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, PS_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( m_htmlPathName, lineNumber );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlDescendants::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress();
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlDescendants::OnClickedKeress()
{
	keress();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlDescendants::keress()
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, 0, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////