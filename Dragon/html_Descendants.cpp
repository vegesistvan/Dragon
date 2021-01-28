// Descendant.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_Descendants.h"
#include "afxdialogex.h"
#include "GA_input.h"
#include "html_Edit.h"
#include "DragonDlg.h"
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
	PS_TITOLO,
	PS_TITLE,
	PS_LAST_NAME,
	PS_FIRST_NAME,
	PS_MOTHER_INDEX,
	PS_MINDEX_CALC,
	PS_POSTERIOR,
	PS_SEX_ID,
	PS_BPLACE,
	PS_BDATE,
	PS_DPLACE,
	PS_DDATE,
	PS_COMMENT,
	PS_BRANCH,
	PS_ARM,
	PS_ROWIDF,
	PS_ROWIDM,
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDescendant, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendant::CDescendant(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDescendant::IDD, pParent)
{
	m_pParent = pParent;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendant::~CDescendant()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_CAPTION, colorCaption);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendant, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CDescendant::OnDblclkList)
	ON_EN_CHANGE(IDC_SEARCH, &CDescendant::OnChangeSearch)
	ON_COMMAND(ID_NEWTABLE, &CDescendant::OnNewtable)
	ON_COMMAND(ID_D_LIST, &CDescendant::OnDList)
	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CDescendant::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CDescendant::OnHtmlNotepad)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendant::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT()

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
	m_ListCtrl.InsertColumn( PS_TITOLO,			L"elõnév",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_TITLE,			L"title",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_LAST_NAME,		L"családnév",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_FIRST_NAME,		L"utónév",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_MOTHER_INDEX,	L"anyaIx",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_MINDEX_CALC,	L"anyaIx2",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_POSTERIOR,		L"utótag",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_SEX_ID,			L"sex",				LVCFMT_LEFT,	 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_BPLACE,			L"b.hely",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_BDATE,			L"b.dátum",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_DPLACE,			L"d.hely",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_DDATE,			L"d.dátum",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_COMMENT,		L"leírás",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_BRANCH,			L"elágazás",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_ARM,			L"ág",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_ROWIDF,			L"rowidF",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( PS_ROWIDM,			L"rowidM",			LVCFMT_LEFT,	 80,-1,COL_TEXT);

	colorCaption.SetTextColor( theApp.m_colorClick );
	m_orderix = 0;
	fillTable();

	m_orderix = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::fillTable( )
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
	CGaInput split;
	std::vector<MARRIAGE> vmarriage;
	CString descendant;
	


	CString logFile;
	logFile.Format( L"psplit.txt" );
	str.Format( L"\"%s\" betöltési hibák", theApp.m_htmlFileSpec ); 
	CString fileName( L"DESCENDANTS" );
	CString fileSpec;

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	fwprintf( fh1, L"%s<br><br>", theApp.m_htmlFileSpec ); 

	split.m_error_cnt1 = 0;
	split.m_tableAncestry = TRUE;

	int fileLength;
	int position;

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
	fileLength = (int)file.GetLength();

	split.m_error_cnt1 = 0;

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

	SetWindowTextW( theApp.m_htmlFileSpec );
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );


	str.Format( L"%s fájlban lévõ leszármazottak beolvasása...", theApp.m_htmlFileName );
	CProgressWnd wndP(NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, fileLength );
	wndP.SetPos(0);
	wndP.SetStep(1);

	split.m_error_cnt1 = 0;
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

			m_ListCtrl.SetItemText( nItem, PS_NAME_STR, split._descNameSS );
			m_ListCtrl.SetItemText( nItem, PS_BIRTH_STR, split._descBirthSS );
			m_ListCtrl.SetItemText( nItem, PS_DEATH_STR, split._descDeathSS );

			m_ListCtrl.SetItemText( nItem, PS_TITOLO, split.m_titolo );
			m_ListCtrl.SetItemText( nItem, PS_TITLE, split.d.title );
			m_ListCtrl.SetItemText( nItem, PS_LAST_NAME, split.d.last_name );
//			m_ListCtrl.SetItemText( nItem, PS_LAST_NAME, split.m_tableHeader.familyName );
			m_ListCtrl.SetItemText( nItem, PS_FIRST_NAME, split.d.first_name );
			str.Format( L"%d", split.d.parent2Index );
			if( !str.Compare( L"0" ) ) str.Empty();
			m_ListCtrl.SetItemText( nItem, PS_MOTHER_INDEX, str );

				str.Format( L"%d", split.d.parent2IndexCalc );
			if( !str.Compare( L"0" ) ) str.Empty();
			m_ListCtrl.SetItemText( nItem, PS_MINDEX_CALC, str );

			m_ListCtrl.SetItemText( nItem, PS_POSTERIOR, split.d.posterior );
	

			str.Format( L"%d", split.d.sex_id );
			m_ListCtrl.SetItemText( nItem, PS_SEX_ID, str );

			m_ListCtrl.SetItemText( nItem, PS_BPLACE, split.d.birth_place );
			m_ListCtrl.SetItemText( nItem, PS_BDATE, split.d.birth_date );

			m_ListCtrl.SetItemText( nItem, PS_DPLACE, split.d.death_place );
			m_ListCtrl.SetItemText( nItem, PS_DDATE, split.d.death_date );

			m_ListCtrl.SetItemText( nItem, PS_COMMENT, split.d.comment );
			m_ListCtrl.SetItemText( nItem, PS_BRANCH, split.d.folyt );

			m_ListCtrl.SetItemText( nItem, PS_ARM, split.d.arm );

			str.Format( L"%d", split.d.father_id );
			m_ListCtrl.SetItemText( nItem, PS_ROWIDF, str );

			str.Format( L"%d", split.d.mother_id );
			m_ListCtrl.SetItemText( nItem, PS_ROWIDM, str );

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
	if( split.m_error_cnt1 )
	{
		ShellExecute(NULL, L"open", fileSpec,NULL, NULL, SW_SHOWNORMAL);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CDescendant::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CDescendant::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
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
void CDescendant::listHtmlLine()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( nItem == - 1 )
	{
		AfxMessageBox( L"No selected file!" );
		return;
	}
	
	if( _waccess( theApp.m_htmlFileSpec, 0 ) )
	{
		AfxMessageBox( L"file does not exist!" );
		return;
	}

	int htmlLineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	PS_LINENUMBER ) );
	CHtmlEdit dlg;

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendant::PreTranslateMessage(MSG* pMsg)
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
void CDescendant::OnDList()
{
	CString	logFile(L"decendants"); 
	CString	title( L"Kijelölt leszármazottak" );
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::OnChangeSearch()
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.search( search, m_orderix,  &m_ListCtrl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::OnNewtable()
{
//	SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	CDragonDlg dlg;
	if( m_lineNumber != 0 )
		dlg.OnDescendantLine();
	else if( m_tableNumber != 0 )
		dlg.OnDescendantTable();
	else 
		dlg.OnDescendantFile();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CDescendant::menu()
{
	CMenu* pMenu = GetMenu();

	UINT cnt = pMenu->GetMenuItemCount();

	for( int i=cnt; i > -1; --i )
		pMenu->DeleteMenu( i, MF_BYPOSITION );

	pMenu->AppendMenuW( MF_STRING, ID_NEWTABLE_D,	L"Új lekérdezés" );
	pMenu->AppendMenuW( MF_STRING, ID_D_LIST,		L"Listázás" );
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if(m_pParent)
	{		
		((CDragonDlg*)m_pParent)->m_pDescendantsF = NULL;
		((CDragonDlg*)m_pParent)->m_pDescendantsT = NULL;
		((CDragonDlg*)m_pParent)->m_pDescendantsL = NULL;
	}
	delete this;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CDescendant::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR;

	MENU_IDR = IDR_DROPDOWN_HTML_EDIT;
	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
//			pPopup->EnableMenuItem(ID_RELATIONS,MF_BYCOMMAND | MF_GRAYED);
//			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	PS_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendant::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, PS_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


