// Table_marriages.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Table_marriages.h"
#include "afxdialogex.h"
#include "editHtmlLine.h"
#include "Filter.h"
#include "SelectOne.h"
#include "GetString.h"
#include "editHtmlLines.h"
#include "GetLastFirst.h"
#include "EditMarriage.h"
#include "windows.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "Relatives.h"
#include "compareEntries.h"
#include "displayInfo.h"

#include "checkParam0.h"
#include <algorithm>

#include "ListCtrlColoredHeader.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnDisplayMarriages()
//{
////	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
//	CTableMarriages dlg;
//	dlg.DoModal();
//
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
	LIST_ROWID = 0,
	LIST_SOURCE,
	LIST_LINE,
	LIST_MPLACE,
	LIST_MDATE,

	LIST_LINENUMBER1,
	LIST_ROWID1,
	LIST_NAME1,
	LIST_BIRTH1,
	LIST_DEATH1,
	LIST_WHICHWIFE,

	LIST_LINENUMBER2,
	LIST_ROWID2,
	LIST_NAME2,
	LIST_BIRTH2,
	LIST_DEATH2,
	LIST_WHICHHUSBAND,
};

//bool sortByNameX(const SPOUSE11 &v1, const SPOUSE11 &v2);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTableMarriages, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableMarriages::CTableMarriages(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableMarriages::IDD, pParent)
{
//	m_pParent = pParent;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableMarriages::~CTableMarriages()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_RICHEDIT, m_RichEditCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableMarriages, CDialogEx)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)

	ON_COMMAND(ID_EXPORT_ALL, &CTableMarriages::OnExportAll)
	ON_COMMAND(ID_EXPORT_SELECTED, &CTableMarriages::OnExportSelected)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CTableMarriages::OnCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTableMarriages::OnDblclkList)

	ON_STN_CLICKED(IDC_STATIC_KERESS, &CTableMarriages::OnClickedKeress)
	ON_STN_CLICKED(IDC_NEXT, &CTableMarriages::OnClickedNext)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_ROKONSAG, &CTableMarriages::OnRelatives)
	ON_COMMAND(ID_HTML_HUSBAND, &CTableMarriages::OnHtmlHusband)
	ON_COMMAND(ID_HTML_WIFES, &CTableMarriages::OnHtmlWifes)
	ON_COMMAND(ID_NOTEPAD_HUSBAND, &CTableMarriages::OnNotepadHusband)
	ON_COMMAND(ID_NOTPAD_WIFE, &CTableMarriages::OnNotpadWife)
	ON_COMMAND(ID_COMPARE_HUSBANDS, &CTableMarriages::OnCompareHusbands)
	ON_COMMAND(ID_COMPARE_WIFES, &CTableMarriages::OnCompareWifes)

	ON_COMMAND(ID_EDIT_UPDATE, &CTableMarriages::OnEditUpdate)
	ON_COMMAND(ID_EDIT_DELETE, &CTableMarriages::OnEditDelete)

// szûrések
	ON_COMMAND(ID_ALLMARRIAGES, &CTableMarriages::OnAllMarriages)
	ON_COMMAND(ID_1WIFE, &CTableMarriages::On1wife)
	ON_COMMAND(ID_2WIFES, &CTableMarriages::On2wifes)
	ON_COMMAND(ID_3WIFES, &CTableMarriages::On3wifes)
	ON_COMMAND(ID_4WIFES, &CTableMarriages::On4wifes)
	ON_COMMAND(ID_5WIFES, &CTableMarriages::On5wifes)
	ON_COMMAND(ID_6WIFES, &CTableMarriages::On6wifes)
	ON_COMMAND(ID_7WIFES, &CTableMarriages::On7wifes)
	ON_COMMAND(ID_8WIFES, &CTableMarriages::On8wifes)
	ON_COMMAND(ID_9WIFES, &CTableMarriages::On9wifes)
	ON_COMMAND(ID_10WIFES, &CTableMarriages::On10wifes)
	ON_COMMAND(ID_ROWIDMARRIAGES, &CTableMarriages::OnRowidMarriages)
	ON_COMMAND(ID_MARRIAGES_MAN, &CTableMarriages::OnMarriagesMan)
	ON_COMMAND(ID_MARRIAGES_ALL, &CTableMarriages::OnMarriagesAll)
	ON_COMMAND(ID_MARRIAGES_WOMAN, &CTableMarriages::OnMarriagesWoman)
	ON_COMMAND(ID_MARRIAGES_DESCENDANTS, &CTableMarriages::OnMarriagesDescendants)
	ON_COMMAND(ID_MARRIAGES_PARENTS, &CTableMarriages::OnMarriagesParents)
	ON_COMMAND(ID_MARRIAGES_SPOUSES, &CTableMarriages::OnMarriagesSpouses)
	ON_COMMAND(ID_INFO, &CTableMarriages::OnInfo)
	ON_COMMAND(ID_MARRIGE_GEDCOM, &CTableMarriages::OnMarrigeGedcom)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(theApp.m_caption);



	p_fields = L"\
rowid,\
first_name,\
last_name,\
sex_id,\
lineNumber,\
tableNumber,\
generation,\
source,\
birth_date,\
death_date,\
father_id,\
mother_id,\
united\
";

	m_rsetM = new CSqliteDBRecordSet;
	m_rsetP = new CSqliteDBRecordSet;

	m_ListCtrl.m_ctlHeader.h11 = 5;
	m_ListCtrl.m_ctlHeader.h12 = 10;
	m_ListCtrl.m_ctlHeader.h21 = 11;
	m_ListCtrl.m_ctlHeader.h22 = 17;

	m_RichEditCtrl.GetSelectionCharFormat(cf);
	m_RichEditCtrl.SetBackgroundColor(false, RGB(240, 240, 240));

	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = CFE_BOLD;
	cf.crTextColor = LIGHTRED;
	cf.yHeight = 300;

	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.Clear();
	m_RichEditCtrl.SetWindowTextW( L"Házasságok");
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);
	m_orderix = 0;

	enableMenu( MF_GRAYED );

	colorKeres.SetTextColor( theApp.m_colorClick ); 
	colorNext.SetTextColor( theApp.m_colorClick );

	m_ListCtrl.DeleteAllItems();
	for (int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i--)
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrl.InsertColumn(LIST_ROWID, L"rowid", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrl.InsertColumn(LIST_SOURCE, L"s", LVCFMT_RIGHT, 25, -1, COL_NUM);
	m_ListCtrl.InsertColumn(LIST_LINE, L"line#", LVCFMT_LEFT, 50, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(LIST_MPLACE, L"hk helye", LVCFMT_LEFT, 110, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(LIST_MDATE, L"hk ideje", LVCFMT_LEFT, 90, -1, COL_TEXT);
		
	m_ListCtrl.InsertColumn(LIST_LINENUMBER1, L"line#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrl.InsertColumn(LIST_ROWID1, L"rowid", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrl.InsertColumn(LIST_NAME1, L"férj", LVCFMT_LEFT, 140, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(LIST_BIRTH1, L"született", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(LIST_DEATH1, L"meghalt", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(LIST_WHICHWIFE, L"házasság", LVCFMT_RIGHT, 60, -1, COL_NUM);

	m_ListCtrl.InsertColumn(LIST_LINENUMBER2, L"line#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrl.InsertColumn(LIST_ROWID2, L"rowid", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrl.InsertColumn(LIST_NAME2, L"feleség", LVCFMT_LEFT, 140, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(LIST_BIRTH2, L"született", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(LIST_DEATH2, L"meghalt", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(LIST_WHICHHUSBAND, L"házasság", LVCFMT_RIGHT, 60, -1, COL_NUM);
	

	m_columnsCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();



	m_command.Format( L"SELECT count(*) FROM marriages" );
	if( !queryM( m_command ) ) return false;

	int cnt = _wtoi(m_rsetM->GetFieldString( 0 ));
	if (cnt && cnt < 10000)
		OnAllMarriages();

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::enableMenu( BOOL flag)
{
	CMenu* menu;

	menu = GetMenu();

	menu->EnableMenuItem(1, MF_BYPOSITION| flag );
	DrawMenuBar();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// S Z Û R É S E K ///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::OnAllMarriages()
{
	m_filter.Empty();
	m_filterText = L"Összes házasság";
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On1wife()
{
	m_numOfSpouses = 1;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On2wifes()
{
	m_numOfSpouses = 2;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On3wifes()
{
	m_numOfSpouses = 3;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On4wifes()
{
	m_numOfSpouses = 4;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On5wifes()
{
	m_numOfSpouses = 5;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On6wifes()
{
	m_numOfSpouses = 6;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On7wifes()
{
	m_numOfSpouses = 7;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On8wifes()
{
	m_numOfSpouses = 8;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On9wifes()
{
	m_numOfSpouses = 9;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::On10wifes()
{
	m_numOfSpouses = 10;
	m_filter.Format(L"WHERE p.numOfSpouses='%d'", m_numOfSpouses);
	m_filterText.Format(L"%d házasságot kötött férfiak", m_numOfSpouses);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::OnRowidMarriages()
{
	CGetString dlg;

	int rowid;
	rowid = theApp.GetProfileInt(L"dragon", L"rowid", 0);
	dlg.m_string.Format(L"%d", rowid);
	dlg.m_caption.Format(L"Add meg a kívánt ember rowid-ját!");
	if (dlg.DoModal() == IDCANCEL) return;
	rowid = _wtoi(dlg.m_string);
	theApp.WriteProfileInt(L"dragon", L"rowid", rowid);

	m_last_name_h.Empty();
	m_last_name_w.Empty();
	m_filterText.Format(L"%s rowid házasságai", dlg.m_string);
	m_filter.Format(L"WHERE husband_id=%s OR wife_id=%s", dlg.m_string, dlg.m_string);
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::OnMarriagesAll()
{
	CGetLastFirst dlg;
	if (dlg.DoModal() == IDCANCEL) return;

	m_filterText.Format(L"%s nevû emberek házasságai", dlg._fullname);

	if (dlg._firstname.IsEmpty())
		m_filter.Format(L"WHERE husband_id IN (SELECT rowid FROM people WHERE last_name='%s') OR wife_id IN (SELECT rowid FROM people WHERE last_name='%s')", dlg._lastname, dlg._lastname);
	else
		m_filter.Format(L"WHERE husband_id IN (SELECT rowid FROM people WHERE first_name='%s' AND last_name='%s') OR wife_id IN (SELECT rowid FROM people WHERE first_name='%s' AND last_name='%s')", dlg._firstname, dlg._lastname, dlg._firstname, dlg._lastname);

	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::OnMarriagesMan()
{
	CGetLastFirst dlg;
	if (dlg.DoModal() == IDCANCEL) return;

	m_filterText.Format(L"%s nevû férfiak házasságai", dlg._fullname);

	if (dlg._firstname.IsEmpty())
		m_filter.Format(L"WHERE husband_id IN (SELECT rowid FROM people WHERE last_name='%s')", dlg._lastname);
	else
		m_filter.Format(L"WHERE husband_id IN (SELECT rowid FROM people WHERE first_name='%s' AND last_name='%s')", dlg._firstname, dlg._lastname);

	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::OnMarriagesWoman()
{
	CGetLastFirst dlg;
	if (dlg.DoModal() == IDCANCEL) return;

	m_filterText.Format(L"%s nevû nõk házasságai", dlg._fullname);

	if (dlg._firstname.IsEmpty())
		m_filter.Format(L"WHERE wife_id IN (SELECT rowid FROM people WHERE last_name='%s')", dlg._lastname);
	else
		m_filter.Format(L"WHERE wife_id IN (SELECT rowid FROM people WHERE first_name='%s' AND last_name='%s')", dlg._firstname, dlg._lastname);

	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::OnMarriagesDescendants()
{
	m_last_name_h.Empty();
	m_last_name_w.Empty();
	m_filterText = L"Leszármazottak házasságai";
	m_filter = L"WHERE p.source=1";
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::OnMarriagesParents()
{
	m_last_name_h.Empty();
	m_last_name_w.Empty();
	m_filterText = L"Leszármazottak házastársai szüleinek házasságai";
	m_filter = L"WHERE p.source=3";
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
void CTableMarriages::OnMarriagesSpouses()
{
	m_last_name_h.Empty();
	m_last_name_w.Empty();
	m_filterText = L"Leszármazottak házastársai szüleinek házasságai";
	m_filter = L"WHERE p.source=3";
	listMarriages();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::listMarriages()
{
	// mrsetM fields
	enum
	{
		ROWIDM = 0,
		HUSBAND_ID,
		WIFE_ID,
		WHICHHUSBAND,
		WHICHWIFE,
		PLACE,
		DATE,
		SOURCE,
		LINE,

		ROWID1,
		LINENUMBER1,
		LAST_NAME1,
		FIRST_NAME1,
		BIRTH1,
		DEATH1
	};
	// mrsetP fields
	enum
	{
		ROWID2 = 0,
		LINENUMBER2,
		LAST_NAME2,
		FIRST_NAME2,
		BIRTH2,
		DEATH2
	};
	CString columnsM;
	CString columnsP;

	CString rowidM;
	CString husband_id;
	CString wife_id;
	CString whichHusband;
	CString whichWife;
	CString place;
	CString date;
	CString source;
	CString line;

	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString death;

	CProgressWnd wndP(NULL, L"Házasságok beolvasása...");
	wndP.GoModal();

	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.DetachDataset();
	v_tableMarriages.clear();


	columnsM = L"m.rowid, m.husband_id, m.wife_id, m.whichHusband, m.whichWife, m.place, m.date, m.source, m.linenumber";
	columnsP = L"p.rowid, p.lineNumber, p.last_name, p.first_name, p.birth_date, p.death_date";
	m_command.Format(L"SELECT %s, %s FROM marriages m LEFT JOIN people p ON m.husband_id=p.rowid %s ORDER BY p.last_name, p.first_name", columnsM, columnsP, m_filter);
	if (!queryM(m_command)) return;

	if (!m_rsetM->RecordsCount())
	{
		wndP.DestroyWindow();
		AfxMessageBox(L"Nincs a feltételnek megfelelõ házasság az adatbázisban!");
		return;
	}



	wndP.SetRange(0, m_rsetM->RecordsCount());
	wndP.SetPos(0);
	wndP.SetStep(1);

	for (int i = 0; i < m_rsetM->RecordsCount(); ++i)
	{
		rowidM = m_rsetM->GetFieldString(ROWIDM);
		husband_id = m_rsetM->GetFieldString(HUSBAND_ID);
		wife_id = m_rsetM->GetFieldString(WIFE_ID);
		whichHusband = m_rsetM->GetFieldString(WHICHHUSBAND);
		whichWife = m_rsetM->GetFieldString(WHICHWIFE);
		place = m_rsetM->GetFieldString(PLACE);
		date = m_rsetM->GetFieldString(DATE);
		source = m_rsetM->GetFieldString(SOURCE);
		line = m_rsetM->GetFieldString(LINE);

		rowid = m_rsetM->GetFieldString(ROWID1);
		linenumber = m_rsetM->GetFieldString(LINENUMBER1);
		name.Format(L"%s %s", m_rsetM->GetFieldString(LAST_NAME1), m_rsetM->GetFieldString(FIRST_NAME1));
		birth = m_rsetM->GetFieldString(BIRTH1);
		death = m_rsetM->GetFieldString(DEATH1);


		// házasságkötés
		push(rowidM);
		push(source);
		push(line);
		push(place);
		push(date);

		// férj
		push(linenumber);
		push(husband_id);
		push(name);
		push(birth);
		push(death);
		if (!whichWife.IsEmpty())
			whichWife += ".";
		push(whichWife);

		// feleség
		m_command.Format(L"SELECT %s FROM people p WHERE rowid = '%s'", columnsP, wife_id);
		if (!queryP(m_command)) return;

		rowid = m_rsetP->GetFieldString(ROWID2);
		linenumber = m_rsetP->GetFieldString(LINENUMBER2);
		name.Format(L"%s %s", m_rsetP->GetFieldString(LAST_NAME2), m_rsetP->GetFieldString(FIRST_NAME2));
		birth = m_rsetP->GetFieldString(BIRTH2);
		death = m_rsetP->GetFieldString(DEATH2);

		push(linenumber);
		push(wife_id);
		push(name);
		push(birth);
		push(death);
		if (!whichHusband.IsEmpty())
			whichHusband += ".";
		push(whichHusband);

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
		m_rsetM->MoveNext();
	}

	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.SetItemCountEx(v_tableMarriages.size());
	m_ListCtrl.AttachDataset(&v_tableMarriages);
	m_ListCtrl.ReSort();

//	wndP.DestroyWindow();

	m_orderix = LIST_NAME1;

	int cntPerPage = m_ListCtrl.GetCountPerPage();
	if (nItem > 0)
	{
		m_ListCtrl.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		nItem += cntPerPage / 2;
		if (nItem > m_rsetM->RecordsCount())
			nItem = m_rsetM->RecordsCount() - 1;
		m_ListCtrl.EnsureVisible(nItem, FALSE);
	}
	enableMenu(MF_ENABLED);
	str.Format(L"%s ( %d)", m_filterText, m_ListCtrl.GetItemCount());

	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.Clear();
	m_RichEditCtrl.SetWindowTextW(str);
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::push( CString str )
{
	int tLen=str.GetLength()+1;
	LPTSTR sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,str.GetBuffer());
	v_tableMarriages.push_back(sT);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	OnEditUpdate();
}

void CTableMarriages::OnEditUpdate()
{
	if (theApp.m_inputMode == GAHTML) return;

	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CEditMarriage dlg;
	m_rowid = m_ListCtrl.GetItemText( nItem, LIST_ROWID );

	dlg.m_rowid = m_rowid;
	if( dlg.DoModal() == IDCANCEL ) return;

	updateEntry( dlg.m_whichHusband, dlg.m_whichWife, dlg.m_place, dlg.m_date );

	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.SetItemCountEx( v_tableMarriages.size() );
	m_ListCtrl.AttachDataset( &v_tableMarriages );
	m_ListCtrl.ReSort();

	m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible( nItem, FALSE );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::updateEntry( CString whichHusband, CString whichWife, CString place, CString date )
{
	int nItem;
	for( nItem = 0; nItem < v_tableMarriages.size(); ++nItem )
	{
		if( v_tableMarriages.at( nItem * m_columnsCount + LIST_ROWID ) == m_rowid )
			break;
	}

	updateField( nItem, LIST_WHICHHUSBAND, whichHusband );
	updateField( nItem, LIST_WHICHWIFE, whichWife );
	updateField( nItem, LIST_MPLACE, place );
	updateField( nItem, LIST_MDATE, date );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::updateField( int nItem, int i, CString str )
{
	int tLen=str.GetLength()+1;
	LPTSTR sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,str.GetBuffer());

	v_tableMarriages.at( nItem * m_columnsCount + i ) = sT;
	str.ReleaseBuffer();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableMarriages::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup = NULL;


	if (theApp.m_inputMode == GAHTML)
	{
		if (Menu.LoadMenu(IDR_DROPDOWN_MARRIAGES_GA))
		{
			pPopup = Menu.GetSubMenu(0);
		}
	}
	else if (theApp.m_inputMode == GEDCOM)
	{
		if (Menu.LoadMenu(IDR_DROPDOWN_MARRIAGES_GEDCOM))
		{
			pPopup = Menu.GetSubMenu(0);
		}
	}
	else 
	{
		if (Menu.LoadMenu(IDR_DROPDOWN_EDIT))
		{
			pPopup = Menu.GetSubMenu(0);
			pPopup->EnableMenuItem(ID_EDIT_INSERT, MF_BYCOMMAND | MF_GRAYED);
			if (m_ListCtrl.GetNextItem(-1, LVNI_SELECTED) < 0)
			{
				pPopup->EnableMenuItem(ID_EDIT_UPDATE, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_EDIT_DELETE, MF_BYCOMMAND | MF_GRAYED);
			}
		}
	}
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnEditDelete()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	CString	rowid;
	rowid = m_ListCtrl.GetItemText(nItem, LIST_ROWID);

	str.Format(L"Tényleg törölni akarod a rowid=%s házasságot?", rowid);
	if (AfxMessageBox(str, MB_YESNO) == IDNO) return;


	m_command.Format(L"DELETE FROM marriages WHERE rowid='%s'", rowid);
	if (!theApp.execute(m_command)) return;

	m_ListCtrl.DeleteItem(nItem);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnRelatives()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText(nItem, LIST_ROWID1);
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnHtmlHusband()
{
	CString title;
	int selectedCount = m_ListCtrl.GetSelectedCount();
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (selectedCount == 1)
		title.Format(L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText(nItem, LIST_NAME1), m_ListCtrl.GetItemText(nItem, LIST_LINENUMBER1));
	else
		title.Format(L"%d kijelölt sor a ga.html fájlban", selectedCount);

	theApp.htmlEditLines(&m_ListCtrl, LIST_LINENUMBER1, title);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnHtmlWifes()
{
	CString title;
	int selectedCount = m_ListCtrl.GetSelectedCount();
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (selectedCount == 1)
		title.Format(L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText(nItem, LIST_NAME2), m_ListCtrl.GetItemText(nItem, LIST_LINENUMBER2));
	else
		title.Format(L"%d kijelölt sor a ga.html fájlban", selectedCount);

	theApp.htmlEditLines(&m_ListCtrl, LIST_LINENUMBER2, title);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnNotepadHusband()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText(nItem, LIST_LINENUMBER1);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnNotpadWife()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText(nItem, LIST_LINENUMBER2);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnCompareHusbands()
{
	int cnt = m_ListCtrl.GetSelectedCount();
	if (cnt != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni");
		return;
	}
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();

	int nItem1 = m_ListCtrl.GetNextSelectedItem(pos);
	int nItem2 = m_ListCtrl.GetNextSelectedItem(pos);

	CCompareEntries dlg;
	dlg.rowid1 = m_ListCtrl.GetItemText(nItem1, LIST_ROWID1);
	dlg.rowid2 = m_ListCtrl.GetItemText(nItem2, LIST_ROWID1);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 létezõ bejegyzést jelölj meg!");
		return;
	}
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnCompareWifes()
{
	int cnt = m_ListCtrl.GetSelectedCount();
	if (cnt != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni");
		return;
	}
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();

	int nItem1 = m_ListCtrl.GetNextSelectedItem(pos);
	int nItem2 = m_ListCtrl.GetNextSelectedItem(pos);

	CCompareEntries dlg;
	dlg.rowid1 = m_ListCtrl.GetItemText(nItem1, LIST_ROWID2);
	dlg.rowid2 = m_ListCtrl.GetItemText(nItem2, LIST_ROWID2);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 létezõ bejegyzést jelölj meg!");
		return;
	}
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableMarriages::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CTableMarriages::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = (int)wParam;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnExportAll()
{
	CString	logFile(L"marriages"); 
	CString	title(L"Minden házasság");;
	
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
void CTableMarriages::OnExportSelected()
{
	CString	logFile(L"marriages"); 
	CString	title( L"Kijelölt házasságok" );
	//title.Format( L"%s (selected from %d projects)", m_filterText, m_ListCtrl.GetItemCount() );
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::PreTranslateMessage(MSG* pMsg)
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress( nItem + 1 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnClickedKeress()
{
	keress( 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::queryM( CString command )
{
	if( m_rsetM->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_rsetM->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableMarriages::queryP( CString command )
{
	if( m_rsetP->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_rsetP->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByNameX(const SPOUSE11 &v1, const SPOUSE11 &v2) 
{ 
    return( v1.name < v2.name ); 
} 
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	int nCol = pLVCD->iSubItem;
	pLVCD->clrTextBk = WHITE;
	*pResult = 0;
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		if (LIST_LINENUMBER1 <= nCol && nCol <= LIST_WHICHWIFE)
			pLVCD->clrText = LIGHTBLUE;  // kék
		else if (LIST_LINENUMBER2 <= nCol && nCol <= LIST_WHICHHUSBAND)
			pLVCD->clrText = LIGHTRED;	// rózsaszín
		else
			pLVCD->clrText = BLACK;
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnInfo()
{
	CString info = L"\
A HÁZASSÁGOK LISTÁJA\r\n\
\r\n\
A GA.htm fájlban három helyen fordulhatnak elõ házasságok:\r\n\
\r\n\
1. A leszármazott házassága\r\n\
2. A leszármazott házastársának további házasságai\r\n\
3. A leszármazott házastársa szüleinek házassága\r\n\
\r\n\
Ezek a sorszámok találhatóak az 's' oszlopban.\r\n\
\r\n\
Az összevonási eljárás során a házasságok bejegyzései, éppúgy mint az emberek bejegyzései összevonásra kerülnek. \
A hzasságban megnevezett férjek és feleségek is összevont bejegyzések.\r\n\
GEDCOM bemenet ill. kézi adatbevitel esetén ezeknek természetesen semmi jelentése nincs.\r\n\
\r\n\
A férjek kékkel, a feleségek piros színnel vannak listázva, mindkettõjükrõl azonos információkkal.\r\n\
A 'házasság' oszlopokban az van megadva, hogy a feleség hagyadik házastársa a férjnek és fordítva.\r\n\
\r\n\
Egy házasságra kattintva szerkesztjóhetjük azt, a jobb egérgombbal kattintva egy legördülû menübõl választhatunk különbözõ megjelenítési lehetõségeket.\r\n\
A keresés ablakban a férjre kereshetünk rá, a nevébõl akárhány karaktert megadva.\r\n\
";
	CDisplayInfo dlg;
	dlg.m_text = &info;
	dlg.m_title = L"Házasságok listája";
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableMarriages::OnMarrigeGedcom()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText(nItem, LIST_LINE);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_gedPathName, lineNumber);

}
