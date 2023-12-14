// TableMSubstr.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_Spouses.h"
#include "afxdialogex.h"
#include "inputGA.h"
#include "GetString.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "SelectTableName.h"

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
	L_NAMESUB,
	L_BIRTHSUB,
	L_DEATHSUB,
	L_SEX_ID,
	L_PEER,
	L_TITLE,
	L_TITOLO,
	L_LAST_NAME,
	L_FIRST_NAME,
	L_POSTERIOR,
	L_COMMENT,
	L_BIRTH_PLACE,
	L_BIRTH_DATE,
	L_DEATH_PLACE,
	L_DEATH_DATE,
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
IMPLEMENT_DYNAMIC(CHtmlSpouses, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlSpouses::CHtmlSpouses(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlSpouses::IDD, pParent)
{
	m_pParent = pParent;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlSpouses::~CHtmlSpouses()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_CAPTION, m_colorCaption);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeress);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CHtmlSpouses, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CHtmlSpouses::OnDblclkList)
	ON_COMMAND(ID_MSUBSTR_FILTER, &CHtmlSpouses::OnFilter)
	ON_COMMAND(ID_MSUBSTR_UNFILTER, &CHtmlSpouses::OnUnfilter)
	ON_COMMAND(ID_MSUBSTR_LIST, &CHtmlSpouses::OnList)
	ON_COMMAND(ID_NEWTABLE, &CHtmlSpouses::OnNewtable )

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CHtmlSpouses::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CHtmlSpouses::OnHtmlNotepad)

	ON_STN_CLICKED(IDC_STATIC_KERESS, &CHtmlSpouses::OnClickedKeress)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlSpouses::OnInitDialog()
{
	CDialogEx::OnInitDialog();


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
	m_ListCtrl.InsertColumn( L_NAMESUB,			L"namesub",				LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHSUB,		L"birthsub",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATHSUB,		L"deathsub",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	
	m_ListCtrl.InsertColumn( L_SEX_ID,			L"sex",					LVCFMT_LEFT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_PEER,			L"peer",				LVCFMT_LEFT, 80, -1, COL_TEXT);
	m_ListCtrl.InsertColumn( L_TITLE,			L"title",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_TITOLO,			L"titolo",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LAST_NAME,		L"last_name",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FIRST_NAME,		L"first_name",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_POSTERIOR,		L"posterior",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_COMMENT,			L"leírás",				LVCFMT_LEFT, 100, -1, COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_PLACE,		L"birth",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_DATE,		L"birth",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_PLACE,		L"death",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH_DATE,		L"death",				LVCFMT_LEFT,	 80,-1,COL_TEXT);

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

	colorKeress.SetTextColor( theApp.m_colorClick );
	m_colorCaption.SetTextColor( theApp.m_colorClick );
	m_orderix = 0;
	m_filter.Empty();
	fillTable();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::fillTable( )
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

	
//	PEOPLE p;
//	NAMEBLOCK	name;
	CInputGA split;
//	std::vector<MARRIAGE> vmarriage;
//	CString descendant;


	CString logFile;
	logFile.Format( L"splitSpouses.txt" );
	str.Format( L"\"%s\" betöltési hibák", m_htmlPathName ); 

	CString fileName( L"DESC_SPOUSES" );
	CString fileSpec;

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	fwprintf( fh1, L"%s<br><br>", m_htmlPathName );


//	if (( fh1 = theApp.openLogFile( logFile, str )) == NULL)return;


//	split.m_error_cnt1 = 0;
	split.m_tableAncestry = TRUE;
	split.m_tableNumber	= 0;

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
		str.Format( L"A %d sorban lévõ leszármazott házasságai és azok felbontása", m_lineNumber );
	else if ( m_tableNumber )
		str.Format(L"A %d. %s tábla leszármazottainak házasságai és azok felbontása", m_tableNumber, m_tableName );
	else
		str.Format( L"A fájlban lévõ leszármazottak házasságai és azok felbontása" );

	SetWindowTextW( m_htmlPathName );
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );


	str.Format( L"Leszármazottak házasságainak beolvasása a %s fájlból...", m_htmlFileName );
	CProgressWnd wndP(NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, fileLength );
	wndP.SetPos(0);
	wndP.SetStep(1);

	m_ListCtrl.DeleteAllItems();
	nItem = 0;
	int numberOfTables = 0;

//	m_numberOfTables = tableNumber + 3;
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
			else if( m_tableNumber )
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
				m_ListCtrl.SetItemText( nItem, L_NAMESUB, split.v_marriages.at(i).nameSubstr );
				m_ListCtrl.SetItemText( nItem, L_BIRTHSUB, split.v_marriages.at(i).birthSubstr );
				m_ListCtrl.SetItemText( nItem, L_DEATHSUB, split.v_marriages.at(i).deathSubstr );
	
				m_ListCtrl.SetItemText( nItem, L_SEX_ID, split.v_marriages.at(i).sex_id );
				m_ListCtrl.SetItemText(nItem, L_PEER, split.v_marriages.at(i).peer);
				m_ListCtrl.SetItemText( nItem, L_TITLE, split.v_marriages.at(i).title );
				m_ListCtrl.SetItemText( nItem, L_TITOLO, split.v_marriages.at(i).titolo );
				m_ListCtrl.SetItemText( nItem, L_LAST_NAME, split.v_marriages.at(i).last_name );
				m_ListCtrl.SetItemText( nItem, L_FIRST_NAME, split.v_marriages.at(i).first_name );
				m_ListCtrl.SetItemText( nItem, L_POSTERIOR, split.v_marriages.at(i).posterior );
				m_ListCtrl.SetItemText(	nItem, L_COMMENT, split.v_marriages.at(i).comment);
				m_ListCtrl.SetItemText( nItem, L_BIRTH_PLACE, split.v_marriages.at(i).birth_place );
				m_ListCtrl.SetItemText( nItem, L_BIRTH_DATE, split.v_marriages.at(i).birth_date );
				m_ListCtrl.SetItemText( nItem, L_DEATH_PLACE, split.v_marriages.at(i).death_place );
				m_ListCtrl.SetItemText( nItem, L_DEATH_DATE, split.v_marriages.at(i).death_date );
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
/*
	if( split.m_error_cnt1 )
	{
		ShellExecute(NULL, L"open", fileSpec,NULL, NULL, SW_SHOWNORMAL);
	}
*/
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnFilter()
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
void CHtmlSpouses::OnUnfilter()
{
	m_filter.Empty();
	m_filterText.Format( L"Szûretlen házasságok" );
	fillTable();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnList()
{
	CString	logFile(L"marriagesProblems"); 
	CString	title( L"Problémás házasságok" );
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CHtmlSpouses::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CHtmlSpouses::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = (int)wParam;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINE ) );
	theApp.listHtmlLine( lineNumber );

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnNewtable()
{
//	SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	/*CDragonDlg dlg;
	if( m_lineNumber != 0 )
		OnMarriagesLine();
	else if( m_tableNumber != 0 )
		OnMarriagesTable();
	else 
		OnMarriagesFile();*/
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnMarriagesFile()
{
	CHtmlSpouses dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber	= 0;
	dlg.m_tableName		= L"";
	dlg.DoModal();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnMarriagesTable()
{
	CSelectTableName dlgT;
	int tableNumber;
	CString tableName;
	tableNumber = theApp.GetProfileInt(	L"dragon", L"tableNumber", 0 );

	if( dlgT.DoModal() == IDCANCEL ) return;
	tableNumber = dlgT.m_tableNumberSelected;
	tableName	= dlgT.m_tableNameSelected;
	theApp.WriteProfileInt( L"dragon", L"tableNumber", tableNumber  );

	CHtmlSpouses dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber	= 0;
	dlg.m_tableName		= L"";
	dlg.DoModal();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnMarriagesLine()
{
	CGetString dlgL;
	int lineNumber;
	lineNumber = theApp.GetProfileInt(	L"dragon", L"lineNumber", 0 );
	dlgL.m_string.Format( L"%d", lineNumber ); 

	dlgL.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName );
	if( dlgL.DoModal() == IDCANCEL ) return;

	lineNumber = _wtoi( dlgL.m_string );
	theApp.WriteProfileInt( L"dragon", L"lineNumber", lineNumber  );


	CHtmlSpouses dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber	= lineNumber;
	dlg.DoModal();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CHtmlSpouses::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CHtmlSpouses::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINE ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINE );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( m_htmlPathName, lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlSpouses::PreTranslateMessage(MSG* pMsg)
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnClickedKeress()
{
	keress(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
