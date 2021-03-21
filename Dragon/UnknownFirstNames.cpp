// UnknownFirstNames.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "UnknownFirstNames.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "html_EditLines.h"
#include "html_EditLine.h"
#include "ProgressWnd.h"
#include "Relations.h"
// CUnknownFirstNames dialog

enum
{
	L_ROWID = 0,
	L_LINENUMBER,
	L_NAME,
	L_COLUMNSCOUNT,
};

IMPLEMENT_DYNAMIC(CUnknownFirstNames, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnknownFirstNames::CUnknownFirstNames(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnknownFirstNames::IDD, pParent)
{

	m_info = L"\
Egy beolvasott keresztnév ismeretelen lehet azért, mert\r\n\
\r\n\
- a keresztnév helyes, de nincs a keresztnevek adatbázisában\r\n\
- a keresztnév hibásan van írva,\r\n\
- hibás adatstruktúra miatt a program hibásan oldotta fel a szöveget\r\n\
- program hiba, hiányosság miatt oldotta fel hibásan a szöveget.\r\n\
\r\n\
A jobb egérgombbal klikkelve a névre, a hibásan írt keresztnevet javíthatjuk az adatbázisban vagy a ga.html fájlban.\
A hibás adatstruktúrát is javíthatjuk a GA.html fájlban.\r\n\
Ha név helyesen van írva, csak nincs a nyilvántartásban, akkor beírhatjuk azt férfi, nõi vagy bisex névként.\
";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnknownFirstNames::~CUnknownFirstNames()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnknownFirstNames::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUnknownFirstNames, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CUnknownFirstNames::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CUnknownFirstNames::OnHtmlNotepad)
	ON_COMMAND(ID_DB_EDIT, &CUnknownFirstNames::OnDbEdit)
	ON_COMMAND(ID_SAVE_AS_MAN, &CUnknownFirstNames::OnSaveAsMen )
	ON_COMMAND(ID_SAVE_AS_WOMAN, &CUnknownFirstNames::OnSaveAsWomen )
	ON_COMMAND(ID_SAVE_AS_BISEX, &CUnknownFirstNames::OnSaveAsBisex )
	ON_COMMAND(ID_LISTA, &CUnknownFirstNames::OnLista)
	ON_COMMAND(ID_INFO, &CUnknownFirstNames::OnInfo)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUnknownFirstNames::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"linenumber",	LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	180,-1,COL_TEXT);
	

	findFirstNames();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CUnknownFirstNames::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_UNKNOWN ))
    {
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnknownFirstNames::OnHtmlEdit()
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnknownFirstNames::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber.Trim() );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnknownFirstNames::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelations dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnknownFirstNames::OnSaveAsMen()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name = m_ListCtrl.GetItemText( nItem, L_NAME );
	insertName( name, 1 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnknownFirstNames::OnSaveAsWomen()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name = m_ListCtrl.GetItemText( nItem, L_NAME );
	insertName( name, 2 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnknownFirstNames::OnSaveAsBisex()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name = m_ListCtrl.GetItemText( nItem, L_NAME );
	insertName( name, 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnknownFirstNames::insertName( CString name, int sex_id )
{
	m_command.Format( L"SELECT rowid FROM firstnames WHERE first_name = '%s'", name );
	if( !theApp.querySystem( m_command ) ) return false;
	if( theApp.m_recordsetSystem->RecordsCount() )
	{
		str.Format( L"'%s' keresztnév már van az adatbázisban!", name );
		AfxMessageBox( str );
		return false;
	}

	m_command.Format( L"INSERT INTO firstnames ( first_name, sex_id ) VALUES( '%s', '%d' )", name, sex_id );
	if( !theApp.executeSys( m_command ) ) return false;
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnknownFirstNames::findFirstNames()
{
	m_command.Format( L"SELECT rowid, linenumber, first_name FROM people ORDER BY first_name" );
	if( !theApp.query( m_command ) ) return false;

	CString rowid;
	CString linenumber;
	CString first_name;
	int sex_id;
	int nItem = 0;
	CProgressWnd wndP( NULL, L"Keresztnevek ellenõrzése... "); 
	wndP.GoModal();
	wndP.SetRange( 0, theApp.m_recordset->RecordsCount() -1 );
	wndP.SetPos(0 );

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid		= theApp.m_recordset->GetFieldString( 0 );
		first_name	= theApp.m_recordset->GetFieldString( 2 );
		if( (sex_id = theApp.isFirstName( first_name ) ) == -1 )
		{
			linenumber = theApp.m_recordset->GetFieldString( 1 );
			nItem = m_ListCtrl.InsertItem( nItem, rowid );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, linenumber );
			m_ListCtrl.SetItemText( nItem, L_NAME, first_name );
			++nItem;
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	if( nItem == 0 )
		OnCancel();
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnknownFirstNames::OnLista()
{
	CString	logFile(L"unknownfirstnames"); 
	CString	title(L"Ismeretlen keresznevek");;
	
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnknownFirstNames::OnInfo()
{
	theApp.m_pszAppName = _tcsdup( L"Ismeretlen keresztnevek" );
	AfxMessageBox( m_info, MB_ICONINFORMATION );
}
