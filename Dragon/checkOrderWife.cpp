// checkOrderWife.cpp : implementation file
//
#include "stdafx.h"
#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "checkOrderWife.h"
#include "ProgressWnd.h"
#include "Relatives.h"
#include "editHtmlLines.h"
#include "ColorStatic.h"

enum
{
	WLINE = 0,
	WCNT,
	WSOURCE,
	WUNITED,
	WNUM1,
	WNUM2,
	WROWID,
	WNAME,
	WBIRTH,
	WDEATH,
	WROWIDF,
	WFATHER,
	WBIRTHF,
	WDEATHF,
	WROWIDM,
	WMOTHER,
	WBIRTHM,
	WDEATHM,
};

// CCheckOrderWife dialog
IMPLEMENT_DYNAMIC(CCheckOrderWife, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckOrderWife::CCheckOrderWife(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHECK_ORDERWIFE, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckOrderWife::~CCheckOrderWife()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_STATIC_KERES, colorKeres);
	DDX_Control(pDX, IDC_STATIC_NEXT, colorNext);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckOrderWife, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckOrderWife::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckOrderWife::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckOrderWife::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckOrderWife::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_HTML_CHILDREN, &CCheckOrderWife::OnHtmlChildren)
	ON_COMMAND(ID_ROKONSAG, &CCheckOrderWife::OnDbEdit)

	ON_STN_CLICKED(IDC_STATIC_NEXT, &CCheckOrderWife::OnClickedStaticNext)
	ON_COMMAND(IDC_STATIC_KERESS, &CCheckOrderWife::OnStaticKeress)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckOrderWife::OnCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCheckOrderWife::OnDblclkList)
	ON_COMMAND(ID_INFO, &CCheckOrderWife::OnInfo)
	ON_COMMAND(ID_UNITE_DATA, &CCheckOrderWife::OnUniteData)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckOrderWife::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	str = L"Férjek, akiknek több azonos sorszámú felesége volt.";
	SetWindowTextW(str);

	colorKeres.SetTextColor(theApp.m_colorClick);
	colorNext.SetTextColor(theApp.m_colorClick);

	createColumns();
	fillTable();




	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::createColumns()
{
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrl.InsertColumn(WLINE, L"line", LVCFMT_RIGHT, 60, -1, COL_HIDDEN);
	m_ListCtrl.InsertColumn(WCNT, L"cnt", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WSOURCE, L"s", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WUNITED, L"u", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WNUM1, L"#házastárs", LVCFMT_RIGHT,80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WNUM2, L"sorrend", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WROWID, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WNAME, L"name", LVCFMT_LEFT, 200, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(WBIRTH, L"birth", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WDEATH, L"death", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WROWIDF, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WFATHER, L"apa", LVCFMT_LEFT, 200, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(WBIRTHF, L"birth", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WDEATHF, L"death", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WROWIDM, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WMOTHER, L"anya", LVCFMT_LEFT, 200, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(WBIRTHM, L"birth", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(WDEATHM, L"death", LVCFMT_RIGHT, 80, -1, COL_NUM);
	

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCheckOrderWife::fillTable()
{
	nItem = 0;

	CProgressWnd wndP(NULL, L"A férj azonos sorszámú feleségei... ");
	wndP.GoModal();

	m_command.Format(L"SELECT husband_id, wife_id, whichHusband, whichWife FROM marriages ORDER BY husband_id, whichWife" );
	if (!theApp.query(m_command)) return false;
	UINT i;
	CString spouse1_id1;
	CString spouse1_id2;
	CString orderWife1;
	CString orderWife2;
	m_cnt = 0;

	wndP.SetRange(0, theApp.m_recordset->RecordsCount());
	wndP.SetPos(0);
	wndP.SetStep(1);
	for (i = 0; i < theApp.m_recordset->RecordsCount(); ++i)
	{
		spouse1_id2 = theApp.m_recordset->GetFieldString(0);
		orderWife2 = theApp.m_recordset->GetFieldString(3);
		if (spouse1_id2 == spouse1_id1 && orderWife2 == orderWife1)  // azonios férj, azonos sorszámú feleség
		{
			listCouple(i, orderWife1);
		}
		orderWife1 = orderWife2;
		spouse1_id1 = spouse1_id2;
		theApp.m_recordset->MoveNext();


		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCheckOrderWife::listCouple(int i1, CString whichWife )
{
	theApp.m_recordset->MovePrevious();
	
	CString husband_id	= theApp.m_recordset->GetFieldString(0);
	CString wife_id	= theApp.m_recordset->GetFieldString(1);
	CString order		= theApp.m_recordset->GetFieldString(3);
	int col;
	PEOPLESTRUCT p1;

	
	getPeople(husband_id);
	
	col = 0;
	m_ListCtrl.InsertItem(nItem, p.line);
	++m_cnt;
	str.Format(L"%d", m_cnt);
	m_ListCtrl.SetItemText(nItem, WCNT, str);
	m_ListCtrl.SetItemText(nItem, WSOURCE, p.source);
	m_ListCtrl.SetItemText(nItem, WUNITED, p.united);
	m_ListCtrl.SetItemText(nItem, WNUM1, p.numOfSpouses);
	m_ListCtrl.SetItemText(nItem, WNUM2, L"");
	m_ListCtrl.SetItemText(nItem, WROWID, p.rowid);
	m_ListCtrl.SetItemText(nItem, WNAME, p.name);
	m_ListCtrl.SetItemText(nItem, WBIRTH, p.birth);
	m_ListCtrl.SetItemText(nItem, WDEATH, p.death);
	m_ListCtrl.SetItemData(nItem, col);
	++nItem;

	col = 0;
	getPeople(wife_id);
	p1 = p;
	m_ListCtrl.InsertItem(nItem, p.line);
	m_ListCtrl.SetItemText(nItem, WSOURCE, p.source);
	m_ListCtrl.SetItemText(nItem, WUNITED, p.united);
	m_ListCtrl.SetItemText(nItem, WNUM1, p.numOfSpouses);
	m_ListCtrl.SetItemText(nItem, WNUM2, whichWife);
	m_ListCtrl.SetItemText(nItem, WROWID, p.rowid);
	m_ListCtrl.SetItemText(nItem, WNAME, p.name);
	m_ListCtrl.SetItemText(nItem, WBIRTH, p.birth);
	m_ListCtrl.SetItemText(nItem, WDEATH, p.death);
	m_ListCtrl.SetItemData(nItem, col);
	++nItem;

	theApp.m_recordset->MoveNext();
	wife_id = theApp.m_recordset->GetFieldString(1);

	col = 0;
	getPeople(wife_id);
	m_ListCtrl.InsertItem(nItem, p.line);
	m_ListCtrl.SetItemText(nItem, WSOURCE, p.source);
	m_ListCtrl.SetItemText(nItem, WUNITED, p.united);
	m_ListCtrl.SetItemText(nItem, WNUM1, p.numOfSpouses);
	m_ListCtrl.SetItemText(nItem, WNUM2, whichWife);
	m_ListCtrl.SetItemText(nItem, WROWID, p.rowid);

	if (p1.name != p.name)
		col += 1 << WNAME;
	m_ListCtrl.SetItemText(nItem, WNAME, p.name);

	if ( (same( p1.birth, p.birth)) == -1 )
		col += 1 << WBIRTH;
	m_ListCtrl.SetItemText(nItem, WBIRTH, p.birth);

	if ((same(p1.death, p.death)) == -1)
		col += 1 << WDEATH;
	m_ListCtrl.SetItemText(nItem, WDEATH, p.death);

	m_ListCtrl.SetItemData(nItem, col);
	++nItem;

	col = 0;
	m_ListCtrl.InsertItem(nItem, L" ");
	m_ListCtrl.SetItemData(nItem, col);
	++nItem;
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCheckOrderWife::getPeople( CString rowid )
{
	if (rowid.IsEmpty()) return true;
	m_command.Format(L"SELECT linenumber, source, united, last_name, first_name, birth_date, death_date, father_id, mother_id, numOfSpouses, numOfChildren FROM people WHERE rowid='%s'", rowid);
	if (!theApp.query1(m_command)) return false;
	p.rowid = rowid;
	p.line = theApp.m_recordset1->GetFieldString(0);
	p.source = theApp.m_recordset1->GetFieldString(1);
	p.united = theApp.m_recordset1->GetFieldString(2);
	p.name.Format(L"%s %s", theApp.m_recordset1->GetFieldString(3), sepFirstName(theApp.m_recordset1->GetFieldString(4)));
	p.name.Trim();

	p.birth = theApp.m_recordset1->GetFieldString(5);
	p.death = theApp.m_recordset1->GetFieldString(6);
	p.father_id = theApp.m_recordset1->GetFieldString(7);
	p.mother_id = theApp.m_recordset1->GetFieldString(8);
	p.numOfSpouses = theApp.m_recordset1->GetFieldString(9);
	p.numOfChildren = theApp.m_recordset1->GetFieldString(10);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckOrderWife::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup;
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	str = m_ListCtrl.GetItemText(nItem, 0);
	if (m_ListCtrl.GetItemText(nItem, 0) == L" ")
		return TRUE;

	//	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
	if (Menu.LoadMenu(IDR_DROPDOWN_SPOUSEORDER))
	{
		pPopup = Menu.GetSubMenu(0);
		if (m_ListCtrl.GetNextItem(-1, LVNI_SELECTED) < 0)
		{
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnHtmlEditLines()
{
	CString title;
	int selectedCount = m_ListCtrl.GetSelectedCount();
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name;
	CString nameS;
	name = m_ListCtrl.GetItemText(nItem, WNAME);
//	nameS = m_ListCtrl.GetItemText(nItem, WSPOUSES);
	if (selectedCount == 1)
		title.Format(L"%s a ga.html fájlban (%s. sor)", name, m_ListCtrl.GetItemText(nItem, WLINE));
	else
		title.Format(L"%s és mások a ga.html fájlban (%s. sor)", name,m_ListCtrl.GetItemText(nItem, WLINE));


	theApp.htmlEditLines(&m_ListCtrl, WLINE, title);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText(nItem, WLINE);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText(nItem, WROWID);

	theApp.HtmlNotepadParents(rowid);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText(nItem, WROWID);
	CEditHtmlLines dlg;
	dlg.m_title.Format(L"%s szülei és testvérei", m_ListCtrl.GetItemText(nItem, WNAME));
	dlg.m_type = L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnHtmlChildren()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText(nItem, WROWID);
	CEditHtmlLines dlg;
	dlg.m_title.Format(L"%s és gyermekei", m_ListCtrl.GetItemText(nItem, WNAME));
	dlg.m_type = L"F_CHILDREN";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText(nItem, WROWID);
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckOrderWife::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnClickedStaticNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress(nItem + 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnStaticKeress()
{
	keress(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::keress(int start)
{
	CString	search;
	GetDlgItem(IDC_SEARCH)->GetWindowText(search);
	theApp.keress(search, &m_ListCtrl, WNAME, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int nCol;
	int iData;
	UINT mask;
	UINT color;

	*pResult = 0;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		nItem = pLVCD->nmcd.dwItemSpec;
		nCol = pLVCD->iSubItem;
		iData = m_ListCtrl.GetItemData(nItem);
		mask = 1 << nCol;
		if (iData & mask)
		{
			pLVCD->clrTextBk = RGB(255, 255, 0);
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;

	CString rowid = m_ListCtrl.GetItemText(nItem, WROWID);
	if (rowid.IsEmpty()) return;

	CRelatives  dlg;
	dlg.m_rowid = rowid;
	if (dlg.DoModal() == IDCANCEL) return;

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnInfo()
{
	AfxMessageBox(m_info, MB_ICONINFORMATION);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckOrderWife::OnUniteData()
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
	dlg.rowid1 = m_ListCtrl.GetItemText(nItem1, WROWID);
	dlg.rowid2 = m_ListCtrl.GetItemText(nItem2, WROWID);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 létezõ bejegyzést jelölj meg!");
		return;
	}
	dlg.DoModal();
}