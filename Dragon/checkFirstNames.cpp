// UnknownFirstNames.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkFirstNames.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "html_EditLines.h"
#include "html_EditLine.h"
#include "ProgressWnd.h"
#include "Relatives.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckFirstnames()
{
	CCheckFirstNames dlg;
	dlg.m_message = true;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	L_ROWID = 0,
	L_LINENUMBER,
	L_NAME,
	L_COLUMNSCOUNT,
};

IMPLEMENT_DYNAMIC(CCheckFirstNames, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFirstNames::CCheckFirstNames(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckFirstNames::IDD, pParent)
{

	m_info = L"\
Egy beolvasott keresztn�v ismeretelen lehet az�rt, mert\r\n\
\r\n\
- a keresztn�v helyes, de nincs a keresztnevek adatb�zis�ban\r\n\
- a keresztn�v hib�san van �rva,\r\n\
- hib�s adatstrukt�ra miatt a program hib�san oldotta fel a sz�veget\r\n\
- program hiba, hi�nyoss�g miatt oldotta fel hib�san a sz�veget.\r\n\
\r\n\
A jobb eg�rgombbal klikkelve a n�vre, a hib�san �rt keresztnevet jav�thatjuk az adatb�zisban vagy a ga.html f�jlban.\
A hib�s adatstrukt�r�t is jav�thatjuk a GA.html f�jlban.\r\n\
Ha n�v helyesen van �rva, csak nincs a nyilv�ntart�sban, akkor be�rhatjuk azt f�rfi, n�i vagy bisex n�vk�nt.\
";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFirstNames::~CCheckFirstNames()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFirstNames::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckFirstNames, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckFirstNames::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckFirstNames::OnHtmlNotepad)
	ON_COMMAND(ID_DB_EDIT, &CCheckFirstNames::OnDbEdit)
	ON_COMMAND(ID_SAVE_AS_MAN, &CCheckFirstNames::OnSaveAsMen )
	ON_COMMAND(ID_SAVE_AS_WOMAN, &CCheckFirstNames::OnSaveAsWomen )
	ON_COMMAND(ID_SAVE_AS_BISEX, &CCheckFirstNames::OnSaveAsBisex )
	ON_COMMAND(ID_LISTA, &CCheckFirstNames::OnLista)
	ON_COMMAND(ID_INFO, &CCheckFirstNames::OnInfo)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFirstNames::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"linenumber",	LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,		L"n�v",			LVCFMT_LEFT,	180,-1,COL_TEXT);
	

	findFirstNames();
	if( !m_ListCtrl.GetItemCount() )
	{
		if( m_message )
			AfxMessageBox( L"Minden keresztn�v szerepel a n�v-adatb�zisban.", MB_ICONINFORMATION );
		OnCancel();

	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckFirstNames::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CCheckFirstNames::OnHtmlEdit()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html f�jlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijel�lt sor a ga.html f�jlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFirstNames::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber.Trim() );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFirstNames::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFirstNames::OnSaveAsMen()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name = m_ListCtrl.GetItemText( nItem, L_NAME );
	insertName( name, 1 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFirstNames::OnSaveAsWomen()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name = m_ListCtrl.GetItemText( nItem, L_NAME );
	insertName( name, 2 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFirstNames::OnSaveAsBisex()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name = m_ListCtrl.GetItemText( nItem, L_NAME );
	insertName( name, 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCheckFirstNames::insertName( CString name, int sex_id )
{
	m_command.Format( L"SELECT rowid FROM firstnames WHERE first_name = '%s'", name );
	if( !theApp.querySystem( m_command ) ) return false;
	if( theApp.m_recordsetSystem->RecordsCount() )
	{
		str.Format( L"'%s' keresztn�v m�r van az adatb�zisban!", name );
		AfxMessageBox( str );
		return false;
	}

	m_command.Format( L"INSERT INTO firstnames ( first_name, sex_id ) VALUES( '%s', '%d' )", name, sex_id );
	if( !theApp.executeSys( m_command ) ) return false;
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCheckFirstNames::findFirstNames()
{
	m_command.Format( L"SELECT rowid, linenumber, first_name FROM people ORDER BY first_name" );
	if( !theApp.query( m_command ) ) return false;

	CString rowid;
	CString linenumber;
	CString first_name;
	int nItem = 0;
	int sex_id;

	CProgressWnd wndP( NULL, L"Keresztnevek ellen�rz�se... "); 
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
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFirstNames::OnLista()
{
	CString	logFile(L"unknownfirstnames"); 
	CString	title(L"Ismeretlen keresznevek");;
	
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFirstNames::OnInfo()
{
	theApp.m_pszAppName = _tcsdup( L"Ismeretlen keresztnevek" );
	AfxMessageBox( m_info, MB_ICONINFORMATION );
}
