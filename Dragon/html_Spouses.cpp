// Descendant.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_Marriages.h"
#include "afxdialogex.h"
#include "GA_input.h"
#include "html_EditLine.h"
#include "DragonDlg.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "html_Spouses.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	MG_ID=0,
	MG_LINE_NUMBER,
	MG_TABLE_NUMBER,

	MG_ORDER,
	MG_SUBSTRING,

	MG_PLACE,
	MG_DATE,
	
	MG_SPOUSE,
	MG_TITLE,
	MG_TITOLO,
	MG_LAST_NAME,
	MG_FIRST_NAME,
	MG_POSTERIOR,
	MG_SEX_ID,
	MG_BPLACE,
	MG_BDATE,
	MG_DPLACE,
	MG_DDATE,
	MG_COMMENT,

	MG_PARENTS,
	
//	MG_FATHER,
	MG_TITLEF,
	MG_TITOLOF,
	MG_LAST_NAMEF,
	MG_FIRST_NAMEF,
	MG_SEX_IDF,
	MG_BPLACEF,
	MG_BDATEF,
	MG_DPLACEF,
	MG_DDATEF,
	MG_COMMENTF,

//	MG_MOTHER,
	MG_TITLEM,
	MG_TITOLOM,
	MG_LAST_NAMEM,
	MG_FIRST_NAMEM,
	MG_SEX_IDM,
	MG_BPLACEM,
	MG_BDATEM,
	MG_DPLACEM,
	MG_DDATEM,
	MG_COMMENTM,

	MG_SPOUSE_SPOUSES,
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CHtmlSpouses, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlSpouses::CHtmlSpouses(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlSpouses::IDD, pParent)
{
	m_pParent = pParent;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlSpouses::~CHtmlSpouses()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_CAPTION, colorCaption);
	DDX_Control(pDX, IDC_STATIC_SEARCH, colorKeress);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CHtmlSpouses, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CHtmlSpouses::OnDblclkList)
	ON_COMMAND(ID_NEWTABLE, &CHtmlSpouses::OnNewtable)
	ON_STN_CLICKED(IDC_STATIC_SEARCH, &CHtmlSpouses::OnClickedStaticSearch)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlSpouses::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT()

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//menu();

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( MG_ID,				L"#",						LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( MG_LINE_NUMBER,	L"line#",					LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( MG_TABLE_NUMBER,	L"table#",					LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( MG_ORDER,			L"order",					LVCFMT_LEFT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_SUBSTRING,		L"h�zass�g",				LVCFMT_LEFT,    600,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_PLACE,			L"hely",					LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_DATE,			L"id�",						LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_SPOUSE,			L"h�zast�rs",				LVCFMT_LEFT,	500,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_PARENTS,		L"sz�l�k",					LVCFMT_LEFT,	200,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( MG_TITLE,			L"titulus",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_TITOLO,			L"el�n�v",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_LAST_NAME,		L"csal�dn�v",				LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_FIRST_NAME,		L"ut�n�v",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_POSTERIOR,		L"ut�tag",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_SEX_ID,			L"sex",						LVCFMT_LEFT,	30,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( MG_BPLACE,			L"sz�let�s",				LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_BDATE,			L"id�",						LVCFMT_LEFT,	100,-1,COL_TEXT);
	
	m_ListCtrl.InsertColumn( MG_DPLACE,			L"hal�l",					LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_DDATE,			L"id�",						LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_COMMENT,		L"le�r�s",					LVCFMT_LEFT,	80,-1,COL_TEXT);


//	m_ListCtrl.InsertColumn( MG_FATHER,			L"apja",					LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_TITLEF,			L"apja",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_TITOLOF,		L"el�n�v",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_LAST_NAMEF,		L"csal�dn�v",				LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_FIRST_NAMEF,	L"ut�n�v",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_SEX_IDF,		L"sex",						LVCFMT_LEFT,	30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_BPLACEF,		L"sz�let�s",				LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_BDATEF,			L"id�",						LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_DPLACEF,		L"hal�l",					LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_DDATEF,			L"id�",						LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_COMMENTF,		L"le�r�s",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	
//	m_ListCtrl.InsertColumn( MG_MOTHER,			L"anyja",	LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_TITLEM,			L"anyja",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_TITOLOM,		L"el�n�v",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_LAST_NAMEM,		L"csal�dn�v",				LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_FIRST_NAMEM,	L"ut�n�v",					LVCFMT_LEFT,	80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_SEX_IDM,		L"sex",						LVCFMT_LEFT,	30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_BPLACEM,		L"sz�let�s",				LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_BDATEM,			L"id�",						LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_DPLACEM,		L"hal�l",					LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_DDATEM,			L"id�",						LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( MG_COMMENTM,		L"le�r�s",					LVCFMT_LEFT,	80,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( MG_SPOUSE_SPOUSES,	L"h�zast�rs h�zast�rsai",	LVCFMT_LEFT,    200,-1,COL_TEXT);

	colorKeress.SetTextColor( theApp.m_colorClick );
	colorCaption.SetTextColor( theApp.m_colorClick );
	m_orderix = 0;
	fillTable();

	m_orderix = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::fillTable( )
{
	CString cLine;
	CString pLine;
	CString familyName;
	CString nameSubstring;
	CString birthBlock;
	CString deathBlock;
	CString table_name;
	CString order;
	int nItem = 0;

	
	PEOPLE p;
	NAMEBLOCK	name;
	CGaInput split;
	std::vector<MARRIAGE> vmarriage;
	CString descendant;


	CString logFile;
	logFile.Format( L"splitSpouses.txt" );
	str.Format( L"\"%s\" bet�lt�si hib�k", theApp.m_htmlFileSpec ); 
	BOOL out = FALSE;

	CString fileName( L"MARRIAGES" );
	CString fileSpec;

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	fwprintf( fh1, L"%s<br><br>", theApp.m_htmlFileSpec );
	fwprintf( fh1, L"Az al�bbi p�rok neme azonos. Ennek az is lehet az oka, hogy sikertelen volt a h�zass�g blokk felbont�sa �s \
						   t�ves a h�zast�rs nev�nek meghat�roz�sa.<br><br>" );
	fwprintf( fh1, L"<pre>" );

//	split.m_error_cnt1 = 0;
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
		str.Format( L"A f�jl %d sor�ban l�v� lesz�rmazott h�zast�rsai �s annak felbont�sa", m_lineNumber );
	else if ( m_tableNumber )
		str.Format(L"A f�jl %d. %s t�bl�j�nak l�v� lesz�rmazottak h�zast�rsai �s azok felbont�sa", m_tableNumber, m_tableName );
	else
		str.Format( L"A f�jlban l�v� lesz�rmazottak h�zast�rsai �s azok felbont�sa" );

	SetWindowTextW( theApp.m_htmlFileSpec );
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );


	str.Format( L"Lesz�rmazottak h�zast�rsainek beolvas�sa a %s f�jlb�l...", theApp.m_htmlFileName );
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
		cLine.Replace( '\'', '|' );	 // a nevekben el�fordulhat az ' jel, amit az SQLite nem szeret
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
				str.Format( L"%d", nItem+1 );
				m_ListCtrl.InsertItem( nItem, str );
				str.Format( L"%d", lineNumber  );
				m_ListCtrl.SetItemText( nItem, MG_LINE_NUMBER, str );
				str.Format( L"%d", tableNumber );
				m_ListCtrl.SetItemText( nItem, MG_TABLE_NUMBER, str );

				order.Format( L"%d", split.v_marriages.at(i).order );
				if( !order.Compare( L"0" ) ) order.Empty();
				m_ListCtrl.SetItemText( nItem, MG_ORDER, order );
				m_ListCtrl.SetItemText( nItem, MG_SUBSTRING, split.v_marriages.at(i).marriageSubstr );
				m_ListCtrl.SetItemText( nItem, MG_PLACE, split.v_marriages.at(i).place );
				m_ListCtrl.SetItemText( nItem, MG_DATE, split.v_marriages.at(i).date );
				m_ListCtrl.SetItemText( nItem, MG_SPOUSE, split.v_marriages.at(i).spouseFullname );
				str = split.v_marriages.at(i).parents;
				m_ListCtrl.SetItemText( nItem, MG_PARENTS, split.v_marriages.at(i).parents );
				m_ListCtrl.SetItemText( nItem, MG_SPOUSE_SPOUSES, split.v_marriages.at(i).moreSpouses );

				m_ListCtrl.SetItemText( nItem, MG_TITLE, split.v_marriages.at(i).title );
				m_ListCtrl.SetItemText( nItem, MG_TITOLO, split.v_marriages.at(i).titolo );
				m_ListCtrl.SetItemText( nItem, MG_LAST_NAME, split.v_marriages.at(i).last_name );
				m_ListCtrl.SetItemText( nItem, MG_FIRST_NAME, split.v_marriages.at(i).first_name );
				m_ListCtrl.SetItemText( nItem, MG_POSTERIOR, split.v_marriages.at(i).first_name );   //!!!!!!!!!!!!!!!
				str.Format( L"%d", split.v_marriages.at(i).sex_id );
				m_ListCtrl.SetItemText( nItem, MG_SEX_ID, str );

				m_ListCtrl.SetItemText( nItem, MG_BPLACE, split.v_marriages.at(i).birth_place );
				m_ListCtrl.SetItemText( nItem, MG_BDATE, split.v_marriages.at(i).birth_date );

				m_ListCtrl.SetItemText( nItem, MG_DPLACE, split.v_marriages.at(i).death_place );
				m_ListCtrl.SetItemText( nItem, MG_DDATE, split.v_marriages.at(i).death_date );

				m_ListCtrl.SetItemText( nItem, MG_COMMENT, split.v_marriages.at(i).comment );

//				m_ListCtrl.SetItemText( nItem, MG_FATHER, split.v_marriages.at(i).father );
				m_ListCtrl.SetItemText( nItem, MG_TITLEF, split.v_marriages.at(i).titleF );
				m_ListCtrl.SetItemText( nItem, MG_TITOLOF, split.v_marriages.at(i).titoloF );
				m_ListCtrl.SetItemText( nItem, MG_LAST_NAMEF, split.v_marriages.at(i).lastNameF );
				m_ListCtrl.SetItemText( nItem, MG_FIRST_NAMEF, split.v_marriages.at(i).firstNameF );
				str.Format( L"%d", split.v_marriages.at(i).sex_idF );
				if( !str.Compare( L"0" ) ) str.Empty();
				m_ListCtrl.SetItemText( nItem, MG_SEX_IDF, str );
				m_ListCtrl.SetItemText( nItem, MG_BPLACEF, split.v_marriages.at(i).birthPlaceF );
				m_ListCtrl.SetItemText( nItem, MG_BDATEF, split.v_marriages.at(i).birthDateF );
				m_ListCtrl.SetItemText( nItem, MG_DPLACEF, split.v_marriages.at(i).deathPlaceF );
				m_ListCtrl.SetItemText( nItem, MG_DDATEF, split.v_marriages.at(i).deathDateF );
				m_ListCtrl.SetItemText( nItem, MG_COMMENTF, split.v_marriages.at(i).commentF );

//				m_ListCtrl.SetItemText( nItem, MG_MOTHER, split.v_marriages.at(i).mother );
				m_ListCtrl.SetItemText( nItem, MG_TITLEM, split.v_marriages.at(i).titleM);
				m_ListCtrl.SetItemText( nItem, MG_TITOLOM, split.v_marriages.at(i).titoloM );
				m_ListCtrl.SetItemText( nItem, MG_LAST_NAMEM, split.v_marriages.at(i).lastNameM );
				m_ListCtrl.SetItemText( nItem, MG_FIRST_NAMEM, split.v_marriages.at(i).firstNameM );
				str.Format( L"%d", split.v_marriages.at(i).sex_idM );
				if( !str.Compare( L"0" ) ) str.Empty();
				m_ListCtrl.SetItemText( nItem, MG_SEX_IDM, str );
				m_ListCtrl.SetItemText( nItem, MG_BPLACEM, split.v_marriages.at(i).birthPlaceM );
				m_ListCtrl.SetItemText( nItem, MG_BDATEM, split.v_marriages.at(i).birthDateM );
				m_ListCtrl.SetItemText( nItem, MG_DPLACEM, split.v_marriages.at(i).deathPlaceM );
				m_ListCtrl.SetItemText( nItem, MG_DDATEM, split.v_marriages.at(i).deathDateM );
				m_ListCtrl.SetItemText( nItem, MG_COMMENTM, split.v_marriages.at(i).commentM );

				m_ListCtrl.SetItemText( nItem, MG_SPOUSE_SPOUSES, split.v_marriages.at(i).moreSpouses );
				++nItem;

				position = (int)file.GetPosition();
				wndP.SetPos( position );
				wndP.StepIt();
				wndP.PeekAndPump();
				if (wndP.Cancelled())
				{
					out = TRUE;
					break;
				}
			}
		}
		if( m_lineNumber ) break;
		if( out ) break;
	}
	wndP.DestroyWindow();
	fwprintf( fh1, L"</pre>" );
	fclose( fh1 );
/*
	if( split.m_error_cnt1 )
	{
		ShellExecute(NULL, L"open", fileSpec,NULL, NULL, SW_SHOWNORMAL);
//		theApp.showLogFile();
	}
*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	m_orderix = wParam;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	MG_LINE_NUMBER ) );
	theApp.listHtmlLine( lineNumber );

	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlSpouses::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
			keress(0);
			break;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnNewtable()
{
//	SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	CDragonDlg dlg;
	if( m_lineNumber != 0 )
		dlg.OnSpousesLine();
	else if( m_tableNumber != 0 )
		dlg.OnSpousesTable();
	else 
		dlg.OnSpousesFile();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::OnClickedStaticSearch()
{
	keress( 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlSpouses::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	if( search.IsEmpty() )
	{
		AfxMessageBox( L"Meg kell adni a keresend� stringet!");
		return;
	}
	theApp.keress( search, &m_ListCtrl, m_orderix, start );
}
