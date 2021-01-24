// TableMSubstr.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_Marriages.h"
#include "afxdialogex.h"
#include "DragonDlg.h"
#include "GA_input.h"
#include "GetString.h"
#include "ProgressWnd.h"
#include "utilities.h"

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
	L_SEX_ID,
	L_TITLE,
	L_TITOLO,
	L_LAST_NAME,
	L_FIRST_NAME,
	L_POSTERIOR,
	L_BIRTH_PLACE,
	L_BIRTH_DATE,
	L_DEATH_PLACE,
	L_DEATH_DATE,
	L_COMMENT,
	L_PARENTS,
	L_FATHER,
	L_LAST_NAMEF,
	L_FIRST_NAMEF,
	L_BIRTH_PLACEF,
	L_BIRTH_DATEF,
	L_DEATH_PLACEF,
	L_DEATH_DATEF,
	L_MOTHER,
	L_LAST_NAMEM,
	L_FIRST_NAMEM,
	L_BIRTH_PLACEM,
	L_BIRTH_DATEM,
	L_DEATH_PLACEM,
	L_DEATH_DATEM,
	L_MORESPOUSES,
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

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CMarriages::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CMarriages::OnHtmlNotepad)

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
	m_ListCtrl.InsertColumn( L_SEX_ID,			L"sex",					LVCFMT_LEFT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_TITLE,			L"title",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_TITOLO,			L"titolo",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LAST_NAME,		L"last_name",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FIRST_NAME,		L"first_name",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_POSTERIOR,		L"posterior",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_PLACE,		L"birth",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_DATE,		L"birth",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_PLACE,		L"death",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_DATE,		L"death",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_COMMENT,			L"leírás",				LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_PARENTS,			L"szülõk",				LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FATHER,			L"apa",					LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LAST_NAMEF,		L"családnév",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FIRST_NAMEF,		L"utónév",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_PLACEF,	L"születés",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_DATEF,		L"születés",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_PLACEF,	L"halál",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_DATEF,		L"halál",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHER,			L"anya",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LAST_NAMEM,		L"családnév",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FIRST_NAMEM,		L"utónév",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_PLACEM,	L"születés",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_DATEM,		L"születés",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_PLACEM,	L"halál",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_DATEM,		L"halál",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MORESPOUSES,		L"házastársak",			LVCFMT_LEFT,	 80,-1,COL_TEXT);

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

				str.Format( L"%d", split.v_marriages.at(i).sex_id );
				m_ListCtrl.SetItemText( nItem, L_SEX_ID, str );
				m_ListCtrl.SetItemText( nItem, L_TITLE, split.v_marriages.at(i).title );
				m_ListCtrl.SetItemText( nItem, L_TITOLO, split.v_marriages.at(i).titolo );
				m_ListCtrl.SetItemText( nItem, L_LAST_NAME, split.v_marriages.at(i).last_name );
				m_ListCtrl.SetItemText( nItem, L_FIRST_NAME, split.v_marriages.at(i).first_name );
				m_ListCtrl.SetItemText( nItem, L_POSTERIOR, split.v_marriages.at(i).posterior );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_PLACE, split.v_marriages.at(i).birth_place );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_DATE, split.v_marriages.at(i).birth_date );
				m_ListCtrl.SetItemText( nItem, L_DEATH_PLACE, split.v_marriages.at(i).death_place );
				m_ListCtrl.SetItemText( nItem, L_DEATH_DATE, split.v_marriages.at(i).death_date );
				m_ListCtrl.SetItemText( nItem, L_COMMENT, split.v_marriages.at(i).comment );
				m_ListCtrl.SetItemText( nItem, L_PARENTS, split.v_marriages.at(i).parents );
				
				str.Format( L"%s %s %s %s %s",\
split.v_marriages.at(i).titleF, split.v_marriages.at(i).titoloF, split.v_marriages.at(i).lastNameF, 
split.v_marriages.at(i).firstNameF,split.v_marriages.at(i).posteriorF ); 					
				str.Trim();
				m_ListCtrl.SetItemText( nItem, L_FATHER, str );

				m_ListCtrl.SetItemText( nItem, L_LAST_NAMEF, split.v_marriages.at(i).lastNameF );
				m_ListCtrl.SetItemText( nItem, L_FIRST_NAMEF, split.v_marriages.at(i).firstNameF );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_PLACEF, split.v_marriages.at(i).birthPlaceF );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_DATEF, split.v_marriages.at(i).birthDateF );
				m_ListCtrl.SetItemText( nItem, L_DEATH_PLACEF, split.v_marriages.at(i).deathPlaceF );
				m_ListCtrl.SetItemText( nItem, L_DEATH_DATEF, split.v_marriages.at(i).deathDateF );

				str.Format( L"%s %s %s %s %s",\
split.v_marriages.at(i).titleM, split.v_marriages.at(i).titoloM, split.v_marriages.at(i).lastNameM, 
split.v_marriages.at(i).firstNameM,split.v_marriages.at(i).posteriorM ); 					
				str.Trim();
				m_ListCtrl.SetItemText( nItem, L_MOTHER, str );
				m_ListCtrl.SetItemText( nItem, L_LAST_NAMEM, split.v_marriages.at(i).lastNameM );
				m_ListCtrl.SetItemText( nItem, L_FIRST_NAMEM, split.v_marriages.at(i).firstNameM );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_PLACEM, split.v_marriages.at(i).birthPlaceM );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_DATEM, split.v_marriages.at(i).birthDateM );
				m_ListCtrl.SetItemText( nItem, L_DEATH_PLACEM, split.v_marriages.at(i).deathPlaceM );
				m_ListCtrl.SetItemText( nItem, L_DEATH_DATEM, split.v_marriages.at(i).deathDateM );

				m_ListCtrl.SetItemText( nItem, L_MORESPOUSES, split.v_marriages.at(i).moreSpouses );

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
	CDragonDlg dlg;
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
		((CDragonDlg*)m_pParent)->m_pMarriagesF = NULL;
		((CDragonDlg*)m_pParent)->m_pMarriagesT = NULL;
		((CDragonDlg*)m_pParent)->m_pMarriagesL = NULL;
	}
	delete this;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMarriages::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CMarriages::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINE ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriages::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINE );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


