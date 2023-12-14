// checkNumOfSpouses.cpp : implementation file
//

#include "pch.h"
#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "checkNumOfChildren.h"
#include "Relatives.h"
#include "editHtmlLines.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckNumofchildren()
//{
//	CCheckNumOfChildren dlg;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


enum
{
	LINE = 0,
	SOURCE,
	UNITED,
	ROWIDN,
	NAME,
	NUM1,
	NUM2,
};


IMPLEMENT_DYNAMIC(CCheckNumOfChildren, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckNumOfChildren::CCheckNumOfChildren(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHECK_NUMOFSPOUSES, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckNumOfChildren::~CCheckNumOfChildren()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNumOfChildren::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckNumOfChildren, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckNumOfChildren::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckNumOfChildren::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckNumOfChildren::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckNumOfChildren::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_HTML_CHILDREN, &CCheckNumOfChildren::OnHtmlChildren)
	ON_COMMAND(ID_ROKONSAG, &CCheckNumOfChildren::OnDbEdit)

	ON_STN_CLICKED(IDC_NEXT, &CCheckNumOfChildren::OnClickedNext)

	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckNumOfChildren::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString filename = L"checkNumOfSpouses";
	CString filespec;
	CString line;
	CString rowid;
	CString name;
	CString source;
	CString united;
	int cnt = 0;
	CString num1;
	CString num2;
	int nItem = 0;

	colorNext.SetTextColor(theApp.m_colorClick);
	createColumns();

//	filespec = theApp.openTextFile(&fl, filename, L"w+");
//	fwprintf(fl, L"Házastársak számának változása az összevonás következtében");

	m_command= L"SELECT rowid, linenumber, source, last_name, first_name, numofchildren, united FROM people ORDER BY last_name, first_name";
	if (!theApp.query(m_command)) return false;

	CProgressWnd wndP(NULL, L"Gyerekek száma összevonás után ...");
	wndP.GoModal();
	wndP.SetRange(0, theApp.m_recordset->RecordsCount());
	wndP.SetPos(0);
	wndP.SetStep(1);

	for (UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext())
	{
		num1 = theApp.m_recordset->GetFieldString(5);
		rowid = theApp.m_recordset->GetFieldString(0);
		m_command.Format(L"SELECT count() FROM people WHERE father_id='%s'", rowid );
		if (!theApp.query1(m_command)) return false;
		num2 = theApp.m_recordset1->GetFieldString(0);

		if (num1 != num2)
		{
			++cnt;
			line = theApp.m_recordset->GetFieldString(1);
			source = theApp.m_recordset->GetFieldString(2);
			name.Format(L"%s %s", theApp.m_recordset->GetFieldString(3), theApp.m_recordset->GetFieldString(4));
			united = theApp.m_recordset->GetFieldString(6);
			
			name.Trim();
			m_ListCtrl.InsertItem(nItem, line);
			m_ListCtrl.SetItemText(nItem, SOURCE, source);
			m_ListCtrl.SetItemText(nItem, UNITED, united);
			m_ListCtrl.SetItemText(nItem, ROWIDN, rowid);
			m_ListCtrl.SetItemText(nItem, NAME, name);
			m_ListCtrl.SetItemText(nItem, NUM1, num1);
			m_ListCtrl.SetItemText(nItem, NUM2, num2);
			++nItem;


		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNumOfChildren::createColumns()
{
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrl.InsertColumn(LINE, L"line", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(SOURCE, L"s", LVCFMT_LEFT, 40, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(UNITED, L"u", LVCFMT_LEFT, 40, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(ROWIDN, L"rowid", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(NAME, L"név", LVCFMT_LEFT, 250, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(NUM1, L"htm fájlban", LVCFMT_LEFT, 40, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(NUM2, L"adatbázisban", LVCFMT_LEFT, 40, -1, COL_TEXT);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckNumOfChildren::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup;

	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
//	if (Menu.LoadMenu(IDR_DROPDOWN_UNITED))
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
void CCheckNumOfChildren::OnHtmlEditLines()
{
	CString title;
	int selectedCount = m_ListCtrl.GetSelectedCount();
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name;

	name = m_ListCtrl.GetItemText(nItem, NAME);
	if (selectedCount == 1)
		title.Format(L"%s a ga.html fájlban (%s. sor)", name,  m_ListCtrl.GetItemText(nItem, LINE));
	else
		title.Format(L"%s és mások a ga.html fájlban (%s. sor)", name, m_ListCtrl.GetItemText(nItem, LINE));


	theApp.htmlEditLines(&m_ListCtrl, LINE, title);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNumOfChildren::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText(nItem, LINE);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNumOfChildren::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText(nItem, ROWIDN);

	theApp.HtmlNotepadParents(rowid);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNumOfChildren::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText(nItem, ROWIDN);
	CEditHtmlLines dlg;
	dlg.m_title.Format(L"%s szülei és testvérei", m_ListCtrl.GetItemText(nItem, NAME));
	dlg.m_type = L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNumOfChildren::OnHtmlChildren()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText(nItem, ROWIDN);
	CEditHtmlLines dlg;
	dlg.m_title.Format(L"%s és gyermekei", m_ListCtrl.GetItemText(nItem, NAME));
	dlg.m_type = L"F_CHILDREN";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNumOfChildren::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText(nItem, ROWIDN);
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckNumOfChildren::PreTranslateMessage(MSG* pMsg)
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
void CCheckNumOfChildren::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress(nItem + 1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNumOfChildren::keress(int start)
{
	CString	search;
	GetDlgItem(IDC_SEARCH)->GetWindowText(search);
	theApp.keress(search, &m_ListCtrl, m_orderix, start, true);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckNumOfChildren::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor = (CELLCOLOR*)lParam;//a COLUMNCOLOR struktura deklaralva van a ListCtrlEx.h -ban.

	if (m_orderix == (int)wParam)
	{
		cColor->bg = RGB(250, 250, 250);
		cColor->tx = RGB(250, 0, 0);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckNumOfChildren::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = (int)wParam;
	return TRUE;
}
