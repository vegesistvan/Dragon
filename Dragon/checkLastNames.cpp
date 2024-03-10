// CheckNames.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckLastNames.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "Relatives.h"
#include "html_EditLines.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckNames()
{
	CCheckLastNames dlg;
	dlg.m_message = true;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	L_CNT = 0,
	L_LINENUMBER,
	L_ROWID,
	L_NAME,
	L_PEOPLE,
};

IMPLEMENT_DYNAMIC(CCheckLastNames, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckLastNames::CCheckLastNames(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckLastNames::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckLastNames::~CCheckLastNames()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckLastNames::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckLastNames, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckLastNames::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckLastNames::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckLastNames::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckLastNames::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_DB_EDIT, &CCheckLastNames::OnDbEdit)


	ON_COMMAND(ID_LIST, &CCheckLastNames::OnList)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckLastNames::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString caption;
	caption.Format( L"Név problémák az adatbázisban amely %s GA.htm fájlból lett feltöltve", theApp.m_htmlFileName );
	SetWindowTextW( caption );

	CString info = L"\
Alább olyan sorokat láthatunk a GA.html fájlból, amelyekbõl valamelyik ember család vagy keresztnevét nem sikerült \
kibányászni. A hiányzó családnevet X, a hiányzó keresztnevet Y karakter jelzi a név oszlopban. Nekünk kell kitalánunk, \
hogy melyik ember neve okozhatott problémát a programnak.\r\n\
\r\n\
A hiba okának megállapításában segítségünkre lehet a 'GA.html fájl ellenõrzése' menüpont, ahol egyes sorok \
felbontását is kérhetjük, anélkül, hogy az az adatbázisba kerülne.\r\n\
\r\n\
A hibákat többnyire az értelmezhetetlen keresztnevek okozzák, de más oka is lehet. Ha felismerjük a hibát és az javítható. akkor a \
a jobb egérgombbal a kiválasztott sorra kattintva egy legördülõ menü jelenik meg, amibõl választhatjuk 'A sor szerkesztése' \n\
ill. 'A sor a Notepad-ben' funkciókat. Bármelyikban javíthatjuk a ga.html fájl megfelelõ sorát.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( L_CNT,			L"#",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_PEOPLE,		L"GA.html sor",	LVCFMT_LEFT,   2000,-1,COL_TEXT);

	m_command = L"SELECT rowid, * FROM people WHERE last_name= '' OR first_name='' ORDER BY lineNumber";
	if( !theApp.query( m_command ) ) return false;

	int nItem = 0;
	CString rowid;
	CString lineNumber;
	CString gaLine;
	CString cntS;
	CString people;
	CString name;
	int z;

//	theApp.m_inputCode = GetInputCode( theApp.m_htmlPathName );

//	gafile.Open( theApp.m_htmlPathName, CFile::modeRead );



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
		if( lineNumber == L"22" )
			z = 1;

		gaLine = theApp.getHtmlLine( lineNumber );

		
		
		rowid	= theApp.m_recordset->GetFieldString( PEOPLE_ROWID );
		name	= getName();
		people	= getPeople();

		nItem = m_ListCtrl.InsertItem( nItem, cntS );
		m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
		m_ListCtrl.SetItemText( nItem, L_NAME, name );
		m_ListCtrl.SetItemText( nItem, L_PEOPLE, people );
//		++nItem;

//		nItem = m_ListCtrl.InsertItem( nItem, L"" );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );
		m_ListCtrl.SetItemText( nItem, L_PEOPLE, gaLine );
		++nItem;

//		nItem = m_ListCtrl.InsertItem( nItem, L"" );
		++nItem;

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
		m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
	wndP.DestroyWindow();

	if( !m_ListCtrl.GetItemCount() )
	{
		if( m_message )
			AfxMessageBox( L"Minden ember család és keresztneve meg van adva!" );
		OnCancel();
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckLastNames::cleanHtmlLine( CString cLine )
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
CString CCheckLastNames::getPeople()
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
CString CCheckLastNames::getName()
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
LRESULT CCheckLastNames:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	if( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ).IsEmpty() ) return FALSE;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);

		pPopup->EnableMenuItem(ID_HTML_NOTEPAD_PARENTS, MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_FATHERANDSIBLINGS, MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_DB_EDIT, MF_BYCOMMAND | MF_GRAYED);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckLastNames::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckLastNames::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijelölt sor a ga.html fájlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckLastNames::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckLastNames::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s szülei és testvérei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckLastNames::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckLastNames::OnList()
{
	CString	logFile(L"nameproblems"); 
	theApp.exportAll( logFile, L"Név problémák", &m_ListCtrl );
}
