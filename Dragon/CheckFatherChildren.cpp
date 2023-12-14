// CheckFatherChildren.cpp : implementation file
// Azonos nevû emeberek bejegyzééseinek listája, akik az egyesítési kritérium valamely adatában megegyeznek, de mégsem egyesített a bejegyzésük

#include "stdafx.h"
#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "CheckFatherChildren.h"
#include "ProgressWnd.h"
#include "editHtmlLines.h"
#include "Relatives.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnFatherSamechildren()
//{
//	CCheckFatherChildren dlg;
//	dlg.DoModal();
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	C_LINE = 0,
	C_ROWID,
	C_NAME,
	C_BIRTH,
	C_DEATH,
	C_ROWIDF,
	C_FATHER,
	C_BIRTHF,
	C_DEATHF,
	C_ROWIDM,
	C_MOTHER,
	C_BIRTHM,
	C_DEATHM,
};

// CCheckFatherChildren dialog

IMPLEMENT_DYNAMIC(CCheckFatherChildren, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFatherChildren::CCheckFatherChildren(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHECK_FATHERCHILDREN, pParent)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFatherChildren::~CCheckFatherChildren()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Text(pDX, IDC_SEARCH, m_search);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckFatherChildren, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckFatherChildren::OnCustomdrawList)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCheckFatherChildren::OnDblclkList)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckFatherChildren::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckFatherChildren::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckFatherChildren::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckFatherChildren::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_HTML_CHILDREN, &CCheckFatherChildren::OnHtmlChildren)
	ON_COMMAND(ID_DB_EDIT, &CCheckFatherChildren::OnDbEdit)

END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFatherChildren::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	createListColumns();
	fatherSameChildren();


	return TRUE;
}
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::createListColumns()
{
	m_ListCtrl.DeleteAllItems();
	for (int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i--)
	{
		m_ListCtrl.DeleteColumn(i);
	}
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);


	m_ListCtrl.InsertColumn(C_LINE, L"line", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrl.InsertColumn(C_ROWID, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrl.InsertColumn(C_NAME, L"name", LVCFMT_LEFT, 200, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(C_BIRTH, L"birth", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(C_DEATH, L"death", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(C_ROWIDF, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrl.InsertColumn(C_FATHER, L"apa", LVCFMT_LEFT, 200, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(C_BIRTHF, L"birth", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(C_DEATHF, L"death", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(C_ROWIDM, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrl.InsertColumn(C_MOTHER, L"anya", LVCFMT_LEFT, 200, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(C_BIRTHM, L"birth", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(C_DEATHM, L"death", LVCFMT_RIGHT, 80, -1, COL_NUM);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::fatherSameChildren()
{
	CString linenumber;
	CString rowid;
	CString father_id;
	CString firstname1;
	CString firstname2;
	CString filespec;
	CString father;
	CString birthF;
	CString deathF;
	CString child;
	CString birthC1;
	CString deathC1;
	CString birthC2;
	CString deathC2;
	bool first;
	int nItem = 0;

//	filespec.Format(L"%s\\childrenSameName.txt", theApp.m_workingDirectory);
//	filespec = theApp.openTextFile(&theApp.fl, L"childrenSameName", L"w+");

//	fwprintf(theApp.fl, L"Apák azonos nevû gyerekei\n\n");

	CProgressWnd wndP(NULL, L"Azonos nevû emberek, akik az egyesítési kritérium valamely adatában megegyeznek...");
	wndP.GoModal();
	
	m_command.Format(L"SELECT rowid, last_name, first_name, birth_date, death_date, lineNumber FROM people WHERE sex_id=1 ORDER BY last_name, first_name");
	if (!theApp.query(m_command)) return;
	wndP.SetRange(0, theApp.m_recordset->RecordsCount());
	wndP.SetPos(0);
	wndP.SetStep(1);


 	for (UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i)
	{
		first = true;
		father_id = theApp.m_recordset->GetFieldString(0);
		m_command.Format(L"SELECT rowid, last_name, first_name, birth_date, death_date, lineNumber FROM people WHERE father_id = '%s' ORDER BY first_name, lineNumber", father_id);
		if (!theApp.query1(m_command)) return;
		if (theApp.m_recordset1->RecordsCount())
		{
			firstname2 = L"WWWW";
			for (UINT j = 0; j < theApp.m_recordset1->RecordsCount() - 1; ++j)
			{
				firstname1 = theApp.m_recordset1->GetFieldString(2);
				birthC1 = theApp.m_recordset1->GetFieldString(3);
				deathC1 = theApp.m_recordset1->GetFieldString(4);

				if (!birthC1.IsEmpty() && birthC1 == birthC2)
				{
					if (!deathC1.IsEmpty() && deathC1 == deathC2)
					{
						if ((!firstname1.IsEmpty() && firstname1 == firstname2))
						{
							if (first)
							{
								if (nItem)
								{
									m_ListCtrl.InsertItem(nItem, L"");
									++nItem;
								}
								setRow(theApp.m_recordset, nItem, 1);
								++nItem;
								theApp.m_recordset1->MovePrevious();
								setRow(theApp.m_recordset1, nItem, 0);
								++nItem;
								first = false;
								theApp.m_recordset1->MoveNext();
							}
							setRow(theApp.m_recordset1, nItem, 0);
							++nItem;
						}
					}
				}
				birthC2 = birthC1;
				deathC2 = deathC1;
				firstname2 = firstname1;
				theApp.m_recordset1->MoveNext();
			}
		}
		theApp.m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
//	fclose(theApp.fl);
//	theApp.showFile(filespec);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::setRow(CSqliteDBRecordSet* rec, int nItem, int data )
{
	CString linenumber;
	CString rowid;
	CString last_name;
	CString first_name;
	CString name;
	CString birth;
	CString death;

	
	rowid	= rec->GetFieldString(0);
	last_name = rec->GetFieldString(1);
	first_name = rec->GetFieldString(2);
	birth = rec->GetFieldString(3);
	death = rec->GetFieldString(4);
	linenumber = rec->GetFieldString(5);
	name.Format(L"%s %s", last_name, first_name);

	m_ListCtrl.InsertItem(nItem, linenumber);
	m_ListCtrl.SetItemText(nItem, C_ROWID, rowid);
	m_ListCtrl.SetItemText(nItem, C_NAME, name);
	m_ListCtrl.SetItemText(nItem, C_BIRTH, birth);
	m_ListCtrl.SetItemText(nItem, C_DEATH, death);
	m_ListCtrl.SetItemData(nItem, data );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int nCol;
	int iData;

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
		if (iData )
		{
			pLVCD->clrTextBk = RGB(255, 255, 0);
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFatherChildren::PreTranslateMessage(MSG* pMsg)
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::keress(int start)
{
	CString	search;
	GetDlgItem(IDC_SEARCH)->GetWindowText(search);
	theApp.keress(search, &m_ListCtrl, C_NAME, start, true);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckFatherChildren::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup;


	if (Menu.LoadMenu(IDR_DROPDOWN_HTML))
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
void CCheckFatherChildren::OnHtmlEditLines()
{
	CString title;
	int selectedCount = m_ListCtrl.GetSelectedCount();
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name;
	CString nameS;
	name = m_ListCtrl.GetItemText(nItem, C_NAME);
//	nameS = m_ListCtrl.GetItemText(nItem, C_SPOUSES);
	if (selectedCount == 1)
		title.Format(L"%s-%s a ga.html fájlban (%s. sor)", name, nameS, m_ListCtrl.GetItemText(nItem, C_LINE));
	else
		title.Format(L"%s-%s és mások a ga.html fájlban (%s. sor)", name, nameS, m_ListCtrl.GetItemText(nItem, C_LINE));


	theApp.htmlEditLines(&m_ListCtrl, C_LINE, title);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText(nItem, C_LINE);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText(nItem, C_ROWID);

	theApp.HtmlNotepadParents(rowid);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText(nItem, C_ROWID);
	CEditHtmlLines dlg;
	dlg.m_title.Format(L"%s szülei és testvérei", m_ListCtrl.GetItemText(nItem, C_NAME));
	dlg.m_type = L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::OnHtmlChildren()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText(nItem, C_ROWID);
	CEditHtmlLines dlg;
	dlg.m_title.Format(L"%s és gyermekei", m_ListCtrl.GetItemText(nItem, C_NAME));
	dlg.m_type = L"F_CHILDREN";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText(nItem, C_ROWID);
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherChildren::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;
	
	CString rowid = m_ListCtrl.GetItemText(nItem, C_ROWID);

	CRelatives  dlg;
	dlg.m_rowid = rowid;
	if (dlg.DoModal() == IDCANCEL) return;

	*pResult = 0;
}