// contractEntriesUnited.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "unitedEntries.h"
#include "unitedEntriesUnited.h"
#include "Relatives.h"
#include "compareEntries.h"
#include "compareAscendants.h"
#include "utilities_dragon.h"
#include <algorithm>

IMPLEMENT_DYNAMIC(CUnitedEntriesUnited, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnitedEntriesUnited::CUnitedEntriesUnited(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNITED_ENTRIES_UNITED, pParent)
{
	m_rgb[0] = YELLOW;
	m_rgb[1] = LIGHTGREEN;
	m_rgb[2] = RGB(129, 190, 250);
	m_rgb[3] = LIGHTRED;
	m_rgb[4] = RGB(173, 255, 47);
	m_rgb[5] = RGB(246, 252, 192);
	m_rgb[6] = RGB(111, 188, 243);
	m_rgb[7] = RGB(255, 0, 255);
	m_rgb[8] = RGB(198, 198, 205);	// szürke, törlendõ
	m_numOfColors = 9;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnitedEntriesUnited::~CUnitedEntriesUnited()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UNITED, m_ListCtrlU);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUnitedEntriesUnited, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_UNITED, &CUnitedEntriesUnited::OnDblclkList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_UNITED, &CUnitedEntriesUnited::OnCustomdrawList)


	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CUnitedEntriesUnited::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CUnitedEntriesUnited::OnHtmlNotepad)
	ON_COMMAND(ID_COMPARE_2PEOPLE, &CUnitedEntriesUnited::OnCompare2People)
	ON_COMMAND(ID_FILTER_BY_NAME, &CUnitedEntriesUnited::OnFilterByName)
	ON_COMMAND(ID_UNITE2, &CUnitedEntriesUnited::OnUnite2)
	ON_COMMAND(ID_COMPARE_ASCENDANTS, &CUnitedEntriesUnited::OnCompareAscendants)
	ON_COMMAND(ID_FILTER_BY_MOTHER, &CUnitedEntriesUnited::OnFilterByMother)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CUnitedEntriesUnited::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pParent = GetParent();

	if (pParent == NULL)
	{
		AfxMessageBox( L"alma" );
	}


	m_ListCtrlU.KeepSortOrder(TRUE);
	m_ListCtrlU.SetExtendedStyle(m_ListCtrlU.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrlU.InsertColumn(U_DUMMY, L"", LVCFMT_RIGHT, 35, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_LOOP, L"loop", LVCFMT_RIGHT, 35, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_GROUP, L"gr", LVCFMT_RIGHT, 30, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_MATCH, L"m#", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_STATUS, L"st", LVCFMT_RIGHT, 30, -1, COL_NUM);

	m_ListCtrlU.InsertColumn(U_UNITED, L"u", LVCFMT_LEFT, 20, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_SOURCE, L"s", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_INDEX, L"i", LVCFMT_RIGHT, 20, -1, COL_NUM);

	m_ListCtrlU.InsertColumn(U_LINE, L"line#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_ROWID, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_NAME, L"ember", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_SEX_ID, L"sex", LVCFMT_LEFT, 30, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_BIRTH, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_DEATH, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlU.InsertColumn(U_LINEF, L"line", LVCFMT_RIGHT, 50, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_ROWIDF, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_FATHER, L"apa", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_BIRTHF, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_DEATHF, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlU.InsertColumn(U_LINEM, L"", LVCFMT_RIGHT, 50, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_ROWIDM, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_MOTHER, L"anya", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_BIRTHM, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_DEATHM, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlU.InsertColumn(U_SPOUSES, L"házastársak", LVCFMT_LEFT, 1000, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_CHILDREN, L"gyerekek", LVCFMT_LEFT, 1500, -1, COL_TEXT);

	inputFile();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitedEntriesUnited::inputFile()
{
	CString fileSpec;
	CString filename;

	m_command.Format(L"SELECT filespec FROM filespec WHERE type=%d", UNITED_FILE);
	if (!theApp.query(m_command)) return false;
	fileSpec = theApp.m_recordset->GetFieldString(0);
	if (fileSpec.IsEmpty())
	{
		str.Format(L"%s fájl nincs specifikálva!", fileSpec);
		AfxMessageBox(str, MB_ICONEXCLAMATION);
		return false;
	}
	filename = fileSpec.Mid(fileSpec.ReverseFind('\\') + 1);
	fileSpec.Format(L"%s\\%s", theApp.m_dbFolderPath, filename);

	if (_waccess(fileSpec, 0))
	{
		str.Format(L"CUnitedEntriesUnited\n%s fájl nem létezik!", fileSpec);
		AfxMessageBox(str, MB_ICONERROR);
		return false;
	}

	CStdioFile file(fileSpec, CFile::modeRead);   // input csv fájl
	int fileLength = (int)file.GetLength();

	file.ReadString(str);  // az elsõ cím sor átugrása

	int n;
	int m;
	int cnt = 0;
	UINT col;
	CString w1;
	CString w2;
	CString cLine;

	vPeople.clear();

	str = L"Egyesített bejegyzések beolvasása...";

	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, fileLength);
	wndP.SetPos(0);

	while (file.ReadString(cLine))
	{
		++cnt;
		A.RemoveAll();
		n = wordList(&A, cLine, '\t', true);
		if (n != U_COLUMNSCOUNT)
		{
			str.Format(L"A %s\nfájl %d. sorban az elemek száma %d.\n%d kellen lenni.", fileSpec, cnt, n, U_COLUMNSCOUNT);
			AfxMessageBox(str, MB_ICONERROR);
			return false;
		}

		for (UINT i = 0; i < A.GetSize(); ++i)
		{
			push(i);
		}

		wndP.SetPos(file.GetPosition());
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	file.Close();
	
	m_ListCtrlU.DeleteAllItems();
	m_ListCtrlU.SetItemCountEx(vPeople.size() + 1);
	m_ListCtrlU.AttachDataset(&vPeople);
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::push(int i)
{
	CString item = A[i];
	int tLen;
	TCHAR* sT;
	tLen = item.GetLength() + 1;
	sT = new TCHAR[tLen];
	_tcscpy_s(sT, tLen, item.GetBuffer());
	vPeople.push_back(sT);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int colorIndex;
	UINT rgbColor;
	int group;
	int status;

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
		group = _wtoi(m_ListCtrlU.GetItemText(nItem, U_GROUP));
		status= _wtoi(m_ListCtrlU.GetItemText(nItem, U_STATUS));
		if (group == 0) rgbColor = WHITE;			// változatlan marad
		else if (status == 0) rgbColor = GRAY;		// RGB(224, 224, 224); 		// törlendõ
		else										// egyesített bejegyzés, színe ciklikusan változik a néven belül
		{
			--group;
			colorIndex = group  % m_numOfColors;
			rgbColor = m_rgb[colorIndex];
		}
		pLVCD->clrTextBk = rgbColor;
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::atvett()
{
	CString code = L"^^^";
	int nItemFirst;
	int nItem = m_ListCtrlU.GetTopIndex() + 1;

	theApp.unselectAll(&m_ListCtrlU);
	for (; nItem < m_ListCtrlU.GetItemCount(); ++nItem)
	{
		for (int i = 3; i < U_COLUMNSCOUNT; ++i)
		{
			if (m_ListCtrlU.GetItemText(nItem, i).Find( code) != - 1 )
			{

				nItemFirst = nItem + m_ListCtrlU.GetCountPerPage() - 1;
				if (nItemFirst > m_ListCtrlU.GetItemCount() - 1) nItemFirst = m_ListCtrlU.GetItemCount() - m_ListCtrlU.GetCountPerPage() - 1;

				m_ListCtrlU.EnsureVisible(nItemFirst, false);
				m_ListCtrlU.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				return;
			}
		}
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;

	CString status = m_ListCtrlU.GetItemText(nItem, U_STATUS);
	if (status.IsEmpty()) return;  // üres sor

	if (status == L"0")
	{
		AfxMessageBox(L"A bejegyzést már töröltük!");
		return;
	}


	CRelatives  dlg;
	dlg.m_rowid = m_ListCtrlU.GetItemText(nItem, U_ROWID);
	if (dlg.DoModal() == IDCANCEL) return;

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// D R O P D O W N   F U N C I Ó K  /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////û
LRESULT CUnitedEntriesUnited::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup;

	if (Menu.LoadMenu(IDR_DROPDOWN_UNITED_UNITED))
	{
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::OnHtmlEditLines()
{
	CString title;
	int selectedCount = m_ListCtrlU.GetSelectedCount();
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	if (selectedCount == 1)
		title.Format(L"%s a ga.html fájlban (%s. sor)", m_ListCtrlU.GetItemText(nItem, U_NAME), m_ListCtrlU.GetItemText(nItem, U_LINE));
	else
		title.Format(L"%d kijelölt sor a ga.html fájlban", selectedCount);

	theApp.htmlEditLines(&m_ListCtrlU, U_LINE, title);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::OnHtmlNotepad()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlU.GetItemText(nItem, U_LINE);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::OnCompare2People()
{
	int cnt = m_ListCtrlU.GetSelectedCount();
	if (cnt != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni");
		return;
	}
	POSITION	pos = m_ListCtrlU.GetFirstSelectedItemPosition();

	int nItem1 = m_ListCtrlU.GetNextSelectedItem(pos);
	int nItem2 = m_ListCtrlU.GetNextSelectedItem(pos);

	CCompareEntries dlg;
	dlg.rowid1 = m_ListCtrlU.GetItemText(nItem1, U_ROWID);
	dlg.rowid2 = m_ListCtrlU.GetItemText(nItem2, U_ROWID);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 létezõ bejegyzést jelölj meg!");
		return;
	}
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::OnFilterByName()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(L"Nincs kijelölve egy sor !");
		return;
	}


	CString name;
	CString nev;
	int pos;
	name = m_ListCtrlU.GetItemText(nItem, U_NAME);
	if ((pos = name.Find('(')) != -1)
		name = name.Left(pos);
	name.Trim();

	UINT i;
	UINT j;
	bool pushed = false;

	UINT length = name.GetLength();

	vFiltered.clear();
	for (i = 0; i < vPeople.size() - U_COLUMNSCOUNT + 1; i += U_COLUMNSCOUNT)
	{
		nev = vPeople.at(i + U_NAME);
		if (nev.Left(length) == name)
		{
			pushLine(i);
			pushed = true;
		}
		else if (pushed)
		{
			pushLine(i);		// szeparáló üres sor
			pushed = false;
		}
	}
	m_ListCtrlU.SetItemCountEx(vFiltered.size() + 1);
	m_ListCtrlU.AttachDataset(&vFiltered);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::OnFilterByMother()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(L"Nincs kijelölve egy sor !");
		return;
	}


	CString name;
	CString nev;
	int pos;
	name = m_ListCtrlU.GetItemText(nItem, U_MOTHER);
	if ((pos = name.Find('(')) != -1)
		name = name.Left(pos);
	name.Trim();

	UINT i;
	UINT j;
	UINT length = name.GetLength();

	vFiltered.clear();
	for (i = 0; i < vPeople.size() - U_COLUMNSCOUNT + 1; i += U_COLUMNSCOUNT)
	{
		nev = vPeople.at(i + U_MOTHER);
		if (nev.Left(length) == name)
			pushLine(i);
	}
	m_ListCtrlU.SetItemCountEx(vFiltered.size() + 1);
	m_ListCtrlU.AttachDataset(&vFiltered);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::pushLine(int i)
{
	for (UINT j = 0; j < U_COLUMNSCOUNT; ++j) // ix-1 a cnt-re mutat
	{
		vFiltered.push_back(vPeople.at(i + j));
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::OnUnite2()
{
	POSITION pos = m_ListCtrlU.GetFirstSelectedItemPosition();
	UNITE_ENTRIES unite;
	vUnite.clear();
	int nItem;

	while (pos)
	{
		nItem = m_ListCtrlU.GetNextSelectedItem(pos);
		unite.nItem = nItem;
		unite.rowid = m_ListCtrlU.GetItemText(nItem, U_ROWID);
		unite.sex_id = m_ListCtrlU.GetItemText(nItem, U_SEX_ID);
		unite.source = _wtoi(m_ListCtrlU.GetItemText(nItem, U_SOURCE));
		vUnite.push_back(unite);
	}
	if (vUnite.size() < 2)
	{
		AfxMessageBox(L"Legalább 2 azonos nevû bejegyzést ki kel jelölnöd!");
		return;
	}
	std::sort(vUnite.begin(), vUnite.end(), sortCBySource);
	
	// ez elsõ bejegyzést kell megtartani
	CString rowidKeep	= vUnite.at(0).rowid;
	int nItem0			= vUnite.at(0).nItem;
	CString sex_id		= vUnite.at(0).sex_id;

	CString rowidDel;
	int ix;
	int base;
	
	for (int i = 1; i < vUnite.size(); ++i)
	{
		nItem = vUnite.at(i).nItem;
		rowidDel = vUnite.at(i).rowid;
		// gyerekek szüleinek update-elése
		if (sex_id == MAN)
			m_command.Format(L"UPDATE people SET father_id = '%s' WHERE father_id='%s'", rowidKeep, rowidDel);
		else
			m_command.Format(L"UPDATE people SET mother_id = '%s' WHERE mother_id='%s'", rowidKeep, rowidDel);
		if (!theApp.execute(m_command)) return;

		// házassági bejegyzések törlése
		if (sex_id == MAN)
			m_command.Format(L"DELETE FROM marriages WHERE husband_id ='%s'", rowidDel);
		else
			m_command.Format(L"DELETE FROM marriages WHERE wife_id ='%s'", rowidDel);
		if (!theApp.execute(m_command)) return;

		// törlendõ bejegyzés törlése
		m_command.Format(L"DELETE FROM people WHERE rowid ='%s'", rowidDel);
		if (!theApp.execute(m_command)) return;

		// törölt bejegyzés törlése
		base = nItem * U_COLUMNSCOUNT;
		for (int j = 0; j < U_COLUMNSCOUNT; ++j)
		{
			ix = base + j;
			vPeople.at(ix) = L" ";
		}
	}
	m_ListCtrlU.DeleteAllItems();
	m_ListCtrlU.SetItemCountEx(vPeople.size() + 1);
	m_ListCtrlU.AttachDataset(&vPeople);

	m_ListCtrlU.EnsureVisible(nItem0 + m_ListCtrlU.GetCountPerPage() - 1, false);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesUnited::OnCompareAscendants()
{
	int db = m_ListCtrlU.GetSelectedCount();
	if (db != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni!");
		return;
	}

	POSITION	pos = m_ListCtrlU.GetFirstSelectedItemPosition();
	int nItem;

	CString rowid1;
	CString rowid2;

	nItem = m_ListCtrlU.GetNextSelectedItem(pos);
	rowid1 = m_ListCtrlU.GetItemText(nItem, U_ROWID);

	nItem = m_ListCtrlU.GetNextSelectedItem(pos);
	rowid2 = m_ListCtrlU.GetItemText(nItem, U_ROWID);
	
	CCompareAsc dlg;
	dlg.rowid1 = rowid1;
	dlg.rowid2 = rowid2;
	dlg.DoModal();
}
