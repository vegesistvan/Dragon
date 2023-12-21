// UNreferencedItems.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "checkUnreferencedItems.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "Relatives.h"

//LPTSTR push(CString str);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragApp::OnUnreferencedItems()
//{
//	CUnreferencedItems dlg;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CUnreferencedItems, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnreferencedItems::CUnreferencedItems(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnreferencedItems::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnreferencedItems::~CUnreferencedItems()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnreferencedItems::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUnreferencedItems, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_NOTEPAD, &CUnreferencedItems::OnHtmlNotepad)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CUnreferencedItems::OnDblclkList)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUnreferencedItems::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_DUMMY,		L"dummy",		LVCFMT_RIGHT,	 65,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 65,-1,COL_NUM );
	if( theApp.m_inputMode == GAHTML )
		m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line",	LVCFMT_RIGHT,	 65,-1,COL_NUM );
	else
		m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line",	LVCFMT_RIGHT,	 65,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn(L_UNITED, L"U", LVCFMT_LEFT, 20, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(L_SOURCE, L"S", LVCFMT_LEFT, 20, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(L_ANCESTRY, L"A", LVCFMT_LEFT, 20, -1, COL_TEXT);
	m_ListCtrl.InsertColumn( L_PEOPLE, Utf8ToAnsi(L"bejegyzés"),	LVCFMT_LEFT,	 700,-1,COL_TEXT );

	CString filespec;
	CString filename = L"unreferenced";



// nekik nincs sem apjuk sem anyjuk
	m_command.Format( L"SELECT rowid FROM people WHERE \
( father_id='0' OR father_id= '' OR typeof(father_id)='null' ) AND \
( mother_id='0' OR mother_id= '' OR typeof(mother_id)='null' ) ");
	if(!theApp.query( m_command ) ) return false;
	int n = theApp.m_recordset->RecordsCount();

	int nItem = 0;
	CString rowid;
	CString people;
	CString	linenumber;
	CString ancestry;
	CString source = L"4";
	int cnt = 0;
	str = L"Nem kapcsolódó bejegyzések keresése...";
	str = Utf8ToAnsi(str);
//	str = UnicodeToUtf8(str);
//	str = UnicodeToMulti(str);

//	LPCTSTR szo = CStringToLPCTSTR(str);
//	str = MultiToUnicode((LPTSTR)szo);

//	LPTSTR szo = push(str);
//	CStringA alma = theApp.MultiToUtf8(szo);

	CProgressWnd wndProgress(NULL, str );
	wndProgress.GoModal();
	wndProgress.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

// Havatkoznak rá mint szülő vagy házastárs?
	for( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid = theApp.m_recordset->GetFieldString( 0 );
		m_command.Format( L"SELECT rowid FROM people WHERE father_id = '%s' OR mother_id= '%s'", rowid, rowid );   
		if( !theApp.query1( m_command ) ) return false;

		m_command.Format( L"SELECT rowid FROM marriages WHERE husband_id = '%s' OR wife_id= '%s'", rowid, rowid );   
		if( !theApp.query2( m_command ) ) return false;

		if( !theApp.m_recordset1->RecordsCount() && !theApp.m_recordset2->RecordsCount() )
		{
			++cnt;
			people = selectPeople( rowid );
			m_ListCtrl.InsertItem( nItem, rowid );
			m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
			m_ListCtrl.SetItemText(nItem, L_LINENUMBER, m_linenumber);
			m_ListCtrl.SetItemText(nItem, L_SOURCE, m_source);
			m_ListCtrl.SetItemText(nItem, L_ANCESTRY, m_ancestry);
			m_ListCtrl.SetItemText( nItem, L_PEOPLE, people );
			++nItem;
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	if( !cnt )
	{
		AfxMessageBox( L"Nincs olyan bejegyzés, amire ne történne hivatkozás!" );
		OnCancel();
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CUnreferencedItems:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	if( theApp.m_inputMode != GAHTML ) return true;

	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_NOTEBOOK ))
    {
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnreferencedItems::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////
CString CUnreferencedItems::selectPeople(CString rowid )
{
	CString sexid;
	CString lastname;
	CString firstname;
	CString birthplace;
	CString birthdate;
	CString deathplace;
	CString deathdate;
	CString profession;
	CString comment;
	CString father_id;
	CString mother_id;
	
	CString birth;
	CString death;

	CString columns = L"rowid, linenumber, sex_id, source, last_name, first_name, birth_place, birth_date, death_place, death_date, occupation, comment, father_id, mother_id, united, tableAncestry";

	m_command.Format(L"SELECT %s FROM people WHERE rowid = '%s'", columns, rowid);
	if (!theApp.query4(m_command)) return L"";
	m_linenumber = theApp.m_recordset4->GetFieldString(1);
	m_source = theApp.m_recordset4->GetFieldString(3);

	lastname = theApp.m_recordset4->GetFieldString(4);
	firstname = theApp.m_recordset4->GetFieldString(5);
	birthplace = theApp.m_recordset4->GetFieldString(6);
	birthdate = theApp.m_recordset4->GetFieldString(7);
	deathplace = theApp.m_recordset4->GetFieldString(8);
	deathdate = theApp.m_recordset4->GetFieldString(9);
	profession = theApp.m_recordset4->GetFieldString(10);
	comment = theApp.m_recordset4->GetFieldString(11);
	father_id = theApp.m_recordset4->GetFieldString(12);
	mother_id = theApp.m_recordset4->GetFieldString(13);
	m_united = theApp.m_recordset4->GetFieldString(14);
	m_ancestry = theApp.m_recordset4->GetFieldString(15);

	birth = pack(L"*", birthplace, birthdate);
	death = pack(L"+", deathplace, deathdate);


	str.Format(L"%s %s", lastname, firstname);
	str.Format(L"%s %s", (CString)str, birth);
	str.TrimRight();
	str.Format(L"%s %s", (CString)str, death);
	str.TrimRight();
	str.Format(L"%s %s", (CString)str, comment);
	str.TrimRight();
	str.Format(L"%s %s", (CString)str, profession);
	str.TrimRight();

	return  str;
}
//////////////////////////////////////////////////////////////////////////////////////////////////4
void CUnreferencedItems::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

/*
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText(nItem, L_LINENUMBER);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
*/

	CRelatives dlg;
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText(nItem, L_ROWID);

	dlg.m_rowid = rowid;
	if (dlg.DoModal() == IDCANCEL) return;

	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
LPTSTR push(CString str)
{
	const TCHAR* pC = (LPCTSTR)str;
	int tLen = str.GetLength() + 1;
	LPTSTR sT = new TCHAR[tLen];
	_tcscpy_s(sT, tLen, str.GetBuffer());
	str.ReleaseBuffer();
	return sT;
}
*/