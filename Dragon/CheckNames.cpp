// CheckNames.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckNames.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "Relations.h"

IMPLEMENT_DYNAMIC(CCheckNames, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckNames::CCheckNames(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckNames::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckNames::~CCheckNames()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckNames, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_LINE, &CCheckNames::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckNames::OnHtmlNotepad)
	ON_COMMAND(ID_DB_EDIT, &CCheckNames::OnRokonsag)

END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckNames::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT()
/*
	CString title = L"A ga.html fájl beolvasásakor elõfordulhatnak olyan emberek, akiknek a vezeték vagy keresztnevét nem tudja a \
program megállapítani, ezért vezeték vagy keresztneve üres marad. Ezeket az eseteket vizsgálhatjuk meg ezzel a funkcióval.\
";
	if( AfxMessageBox( title, MB_ICONINFORMATION || MB_OKCANCEL ) == IDCANCEL ) 
	{
		OnCancel();
		return false;
	}
*/
	CString caption;
	caption.Format( L"Név problémák az adatbázisban amely %s GA.htm fájlból lett feltöltve", theApp.m_htmlFileName );
	SetWindowTextW( caption );

	CString info = L"\
Alább sor-párokat láthatunk felsorolva. A sor-pár elsõ sorában olyan ember személyleírását találjuk, akinek a vezeték és/vagy \
keresztneve hiányzik. Ennek többnyire az az oka, hogy a program valami miatt nem tudta ezt/ezeket meghatározni a \
második sorban ábrázolt ga.html sorból.\
A hiányzó vezeték nevet X, a hiányzó keresztnevet Y karakter jelzi a kérdéses ember személyleírásában.\
Az ember személyleírása segít azonosítani, hogy hozzá tartozó ga.html sorban melyik emberrõl is van szó.\
\r\n\r\n\
A hibákat többnyire az értelmezhetetlen keresztnevek okozzák, de más oka is lehet. Ha felismerjük a hibát és az javítható. akkor a \
a jobb egérgombbal a kiválasztott sorra kattintva egy legördülõ menü jelenik meg, amibõl választhatjuk 'A sor szerkesztése' \n\
ill. 'A sor a Notepad-ben' funkciókat. Bármelyikban javíthatjuk a ga.html fájl megfelelõ sorát.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( 0,	L"#",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 2,	L"rowid",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 3,	L"név",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( 4,	L"",		LVCFMT_LEFT,   2000,-1,COL_TEXT);

	


	m_command = L"SELECT rowid, * FROM people WHERE last_name= '' OR first_name='' ORDER BY lineNumber";
	if( !theApp.query( m_command ) ) return false;

	int nItem = 0;
	CString rowid;
	CString lineNumber;
	CString gaLine;
	CString cntS;
	CString people;
	CString name;


	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	gafile.Open( theApp.m_htmlFileSpec, CFile::modeRead );



	CProgressWnd wndP( NULL, L"Üres nevek beolvasása folyik..." );
	wndP.GoModal();
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	m_lineNumberPrev = 1;
	for( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		cntS.Format( L"%d", i+1 );
		lineNumber = theApp.m_recordset->GetFieldString( PEOPLE_LINENUMBER );
		gaLine = getHtmlLine( lineNumber );

		
		
		rowid	= theApp.m_recordset->GetFieldString( PEOPLE_ROWID );
		name	= getName();
		people	= getPeople();

		nItem = m_ListCtrl.InsertItem( nItem, cntS );
		m_ListCtrl.SetItemText( nItem, 2, rowid );
		m_ListCtrl.SetItemText( nItem, 3, name );
		m_ListCtrl.SetItemText( nItem, 4, people );
		++nItem;

		nItem = m_ListCtrl.InsertItem( nItem, L"" );
		m_ListCtrl.SetItemText( nItem, 1, lineNumber );
		m_ListCtrl.SetItemText( nItem, 4, gaLine );
		++nItem;

		nItem = m_ListCtrl.InsertItem( nItem, L"" );
		++nItem;

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
		m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
	wndP.DestroyWindow();

	gafile.Close();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
void CCheckNames::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckNames::getHtmlLine( CString lineNumberS )
{
	int lineNumber = _wtoi( lineNumberS );
	int cnt;

	cnt = lineNumber - m_lineNumberPrev;
	if( cnt < 0 )
		return cLine;

	for( int i = 0; i < cnt; ++i )
		gafile.ReadString( cLine );		// átugrik cnt sort
	gafile.ReadString( cLine );			// beolvassa a kért sort
	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM  ) cLine = Utf8ToAnsi( cLine );


	m_lineNumberPrev = lineNumber + 1;

	cLine = cleanHtmlLine( cLine );
	return cLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckNames::cleanHtmlLine( CString cLine )
{
	CString htmlLine = cLine;
	int		pos;
	TCHAR	gen;

	if( ( pos = cLine.ReverseFind('>') ) != -1 )
	{
		cLine = cLine.Mid( pos+1 );
		gen = cLine[0];
		if( ( pos = cLine.ReverseFind(';') ) != -1 )
		{
			cLine = cLine.Mid( pos+1 );
			htmlLine.Format( L"%c %s", gen, cLine ); 
		}
	}
	return htmlLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckNames::getPeople()
{
	CString people;
	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
	CString comment;

	birth_place	= theApp.m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE );
	birth_date	= theApp.m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
	death_place	= theApp.m_recordset->GetFieldString( PEOPLE_DEATH_PLACE );
	death_date	= theApp.m_recordset->GetFieldString( PEOPLE_DEATH_DATE );
	comment		= theApp.m_recordset->GetFieldString( PEOPLE_COMMENT );

	people.Empty();
	if( !birth_place.IsEmpty() )
	{
		people += L"*";
		people += birth_place;
		if( !birth_date.IsEmpty() )
		{
			people += L" ";
			people += birth_date;
		}
	}
	else if( !birth_date.IsEmpty() )
	{
		people += L"*";
		people += birth_date;
	}
	
	if( !death_place.IsEmpty() )
	{
		people += L" +";
		people += death_place;
		if( !death_date.IsEmpty() )
		{
			people += L" ";
			people += death_date;
		}
	}
	else if( !death_date.IsEmpty() )
	{
		people += L" +";
		people += death_date;
	}
	people += L" ";
	people += comment;
	return ( people );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckNames::getName()
{
	CString name;
	CString last_name;
	CString first_name;

	last_name	= theApp.m_recordset->GetFieldString( PEOPLE_LAST_NAME );
	first_name	= theApp.m_recordset->GetFieldString( PEOPLE_FIRST_NAME );

	if( last_name.IsEmpty() ) last_name		= L"X";
	if( first_name.IsEmpty() ) first_name	= L"Y";

	name = last_name;
	name += L" ";
	name += first_name;
	return name;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckNames:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	if(Menu.LoadMenu( IDR_DROPDOWN_HTML_EDIT ))
    {
		pPopup = Menu.GetSubMenu(0);

		if( m_ListCtrl.GetItemText( nItem, 1 ).IsEmpty() )  // linenumber
		{
			pPopup->EnableMenuItem(ID_HTML_LINE, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_NOTEPAD, MF_BYCOMMAND | MF_GRAYED);
		}
		if( m_ListCtrl.GetItemText( nItem, 2 ).IsEmpty() )  // rowid
		{
			pPopup->EnableMenuItem(ID_DB_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	1 ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	1 );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnRokonsag()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 2 );
	CRelations dlg;

	dlg.m_rowid = rowid;
	dlg.DoModal();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
