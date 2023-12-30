// CheckNames.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkNames.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "editHtmlLines.h"
#include "Table_firstnames.h"
#include "EditFirstName.h"
#include "html_Spouses.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckNames()
//{
//	CCheckNames dlg;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	L_CNT = 0,
	L_LINENUMBER,
	L_SOURCE,
	L_LASTNAME,
	L_GAHTML,
};
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
	DDX_Control(pDX, IDC_ATTENTION, colorAttention);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckNames, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_EDIT_KERSZTNEV, &CCheckNames::OnEditKersztnev)
	ON_COMMAND(ID_EDIT_NOTEPAD, &CCheckNames::OnEditNotepad)
	ON_COMMAND(ID_EDIT_SPLIT, &CCheckNames::OnEditSplit)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckNames::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString info = L"\
Az 'S' oszlopban van megadva a problémás név 'rangja', ami alapján azonosíthatjuk a problémát.\
\r\n\
A jobb egérgomb kattintására egy legördülõ menübõl választhatunk a keresztnév nyilvántartásba vétele \
és a ga.htm sor Notepad++-ban való megjelenítése között.\r\n\
\r\n\
Ha valószínûsíthetõ, hogy a program azért nem ismerte fel a keresztnevet, mert az nem szerepel \
a nyilvántartásában, akkor az elsõ menüponttal bevihetjük a nyilvántartásbantartásba. \
Ha az S==1, akkor a beviteli ablakba beteszi a leszármazott problémás keresztnevét. \
Ezt elfogadhatjuk vagy módosíthatjuk. Ha S!=0, akkor nekünk kell megadni a bevinni kívánt keresztnevet. \
Ha nem akarunk billentyûzni, akkor a bal egérgombbal kattintva a ga.htm sorra, kijelölhetjük az egérrel \
a kérdéses keresztnevet és azt a bal egérgomb Másolás->Beillesztés funkcióivalk átvihetjük a beviteli ablakba. \
\r\n\
\r\n\
Ha a problémát a ga.htm fájl sorának szintaktikai hibája vagy elgépelés okozta, akkor a \
hibás sort javíthatjuk a Notepad++-ban.\
\r\n\
\r\n\
A nevek téves azonosítását program hiba is okozhatja, amely esetben a programozót kell értesíteni.\
";
	SetWindowText(L"A beovasott ga.html azonm sorai, amelyekbõl valamely család- vagy keresztnevet \
nem sikerült meghatározni");
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	CString attention = L"Az alább felsorolt hibákat egyesítés elõtt mindenképpen ki kell javítani! Legjobb lenne most azonnal!";
	colorAttention.SetTextColor(LIGHTRED);
	GetDlgItem(IDC_ATTENTION)->SetWindowTextW(attention);


	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(FALSE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( L_CNT,			L"#",			LVCFMT_RIGHT,	 60, -1, COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60, -1, COL_NUM );
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 40, -1, COL_NUM);
	m_ListCtrl.InsertColumn( L_LASTNAME,	L"családnév",	LVCFMT_LEFT,	150, -1, COL_TEXT);
	m_ListCtrl.InsertColumn( L_GAHTML,		L"GA.html sor",	LVCFMT_LEFT,   2000, -1, COL_EDIT);

	m_command = L"SELECT linenumber, last_name, source FROM people WHERE last_name= '' OR first_name='' ORDER BY source";
	if( !theApp.query( m_command ) ) return false;

	int nItem = 0;
	CString lineNumber;
	CString gaLine;
	CString cntS;
	CString lastname;
	CString source;
	int z;

	CProgressWnd wndP( NULL, L"Meghatározhatatlan nevû bejegyzések beolvasása folyik..." );
	wndP.GoModal();
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		lineNumber = theApp.m_recordset->GetFieldString(0);
		lastname	= theApp.m_recordset->GetFieldString(1);
		source		= theApp.m_recordset->GetFieldString(2);

		gaLine = theApp.getHtmlLine(lineNumber);
		cntS.Format(L"%d", i + 1);

		nItem = m_ListCtrl.InsertItem( nItem, cntS );
		m_ListCtrl.SetItemText(nItem, L_LINENUMBER, lineNumber);
		m_ListCtrl.SetItemText(nItem, L_SOURCE, source);
		m_ListCtrl.SetItemText( nItem, L_LASTNAME, lastname);
		m_ListCtrl.SetItemText( nItem, L_GAHTML, gaLine );
		++nItem;
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
		m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
//	m_ListCtrl.SetSortOrder(1, 3);
	wndP.DestroyWindow();
	ShowWindow(SW_MAXIMIZE);

	if( !m_ListCtrl.GetItemCount() )
	{
		OnCancel();
		AfxMessageBox( L"Minden ember család- és keresztnevét sikerült meghatározni!" );
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CCheckNames::OnList()
//{
//	CString	logFile(L"nameproblems"); 
//	theApp.exportAll( logFile, L"Név problémák", &m_ListCtrl );
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CCheckNames::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	CString linenumber = m_ListCtrl.GetItemText(nItem, L_LINENUMBER);
	if (!linenumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, linenumber);
	*pResult = 0;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//void CCheckNames::OnFirstnames()
//{
//	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
//	CString source = m_ListCtrl.GetItemText(nItem, L_SOURCE);
//	CString cLine;
//	CString firstname;
//	if (source == L"1")
//	{
//		cLine = m_ListCtrl.GetItemText(nItem, L_GAHTML);
//		firstname = getSecondWord(cLine);
//	}
//
//	CTableFirstnames dlg;
//	dlg.DoModal();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckNames::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup;

	if (Menu.LoadMenu(IDR_DROPDOWN_CHECKNAMES))
	{
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnEditKersztnev()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString source = m_ListCtrl.GetItemText(nItem, L_SOURCE);
	CString cLine;
	CString firstname;
	CString firstname2;
	int pos;
	int cnt = 1;
	if (source == L"1")
	{
		cLine = m_ListCtrl.GetItemText(nItem, L_GAHTML);
		str = getSecondWord(cLine);
		if ((pos = str.Find('/')) != -1)
			firstname = str.Left(pos);
		else if ((pos = str.Find('_')) != -1)
		{
			firstname = str.Left(pos+1);
			firstname2 = str.Mid(pos+1);
		}
		else
			firstname = str;
	}

/*
	// Set the hot cursor to be the system app starting cursor.
	HCURSOR hCursor = ::LoadCursor(NULL, IDC_APPSTARTING);
	m_ListCtrl.SetHotCursor(hCursor);
	ASSERT(m_myListCtrl.GetHotCursor() == hCursor);
*/

	CEditFirstName dlg;
	dlg.m_firstname = firstname;
	dlg.m_sexLast = 1;
	dlg.DoModal();
	if (!firstname2.IsEmpty())
	{
		dlg.m_firstname = firstname2;
		dlg.DoModal();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnEditNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString linenumber = m_ListCtrl.GetItemText(nItem, L_LINENUMBER);
	if (!linenumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, linenumber);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnEditSplit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText(nItem, L_LINENUMBER);
	if (lineNumber.IsEmpty()) return;

	CHtmlSpouses dlg;
	dlg.m_lineNumber = _wtoi(lineNumber);
	dlg.DoModal();
}
