// unitedEntriesX.cpp : implementation file
//

#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "unitedEntriesX.h"
#include "unitedEntries.h"
#include "Relatives.h"
#include "compareEntries.h"
#include "compareAscendants.h"
#include "utilities_dragon.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CUnitedEntriesX, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CUnitedEntriesX::CUnitedEntriesX(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNITED_ENTRIES_X, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnitedEntriesX::~CUnitedEntriesX()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_X, m_ListCtrlX);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUnitedEntriesX, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_X, &CUnitedEntriesX::OnDblclkList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_X, &CUnitedEntriesX::OnCustomdrawList)
	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_NOTEPAD, &CUnitedEntriesX::OnHtmlNotepad)
	ON_COMMAND(ID_COMPARE_2PEOPLE, &CUnitedEntriesX::OnCompare2People)
	ON_COMMAND(ID_COMPARE_ASCENDANTS, &CUnitedEntriesX::OnCompareAscendants)
	ON_COMMAND(ID_FILTER_BY_NAME, &CUnitedEntriesX::OnFilterByName)
	ON_COMMAND(ID_UNITE2, &CUnitedEntriesX::OnUnite)

END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUnitedEntriesX::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_rgbX[0] = RGB(255, 255, 204);
	m_rgbX[1] = RGB(204,255,255);
	m_rgbX[2] = RGB(229,204,255);
	m_rgbX[3] = RGB(255,204,204);
	m_rgbX[4] = RGB(204,204,255);


	pParent = GetParent();

	m_ListCtrlX.KeepSortOrder(TRUE);
	m_ListCtrlX.SetExtendedStyle(m_ListCtrlX.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrlX.InsertColumn(U_DUMMY, L"", LVCFMT_RIGHT, 35, -1, COL_NUM);
	m_ListCtrlX.InsertColumn(U_LOOP, L"loop", LVCFMT_RIGHT, 35, -1, COL_NUM);
	m_ListCtrlX.InsertColumn(U_GROUP, L"gr", LVCFMT_RIGHT, 30, -1, COL_NUM);
	m_ListCtrlX.InsertColumn(U_MATCH, L"m#", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlX.InsertColumn(U_STATUS, L"st", LVCFMT_RIGHT, 30, -1, COL_NUM);

	m_ListCtrlX.InsertColumn(U_UNITED, L"u", LVCFMT_LEFT, 20, -1, COL_NUM);
	m_ListCtrlX.InsertColumn(U_SOURCE, L"s", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrlX.InsertColumn(U_INDEX, L"i", LVCFMT_RIGHT, 20, -1, COL_NUM);

	m_ListCtrlX.InsertColumn(U_LINE, L"line#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlX.InsertColumn(U_ROWID, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlX.InsertColumn(U_NAME, L"ember", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlX.InsertColumn(U_SEX_ID, L"sex", LVCFMT_LEFT, 30, -1, COL_TEXT);
	m_ListCtrlX.InsertColumn(U_BIRTH, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlX.InsertColumn(U_DEATH, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlX.InsertColumn(U_LINEF, L"line", LVCFMT_RIGHT, 50, -1, COL_HIDDEN);
	m_ListCtrlX.InsertColumn(U_ROWIDF, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlX.InsertColumn(U_FATHER, L"apa", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlX.InsertColumn(U_BIRTHF, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlX.InsertColumn(U_DEATHF, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlX.InsertColumn(U_LINEM, L"", LVCFMT_RIGHT, 50, -1, COL_HIDDEN);
	m_ListCtrlX.InsertColumn(U_ROWIDM, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlX.InsertColumn(U_MOTHER, L"anya", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlX.InsertColumn(U_BIRTHM, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlX.InsertColumn(U_DEATHM, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlX.InsertColumn(U_SPOUSES, L"házastársak", LVCFMT_LEFT, 1000, -1, COL_TEXT);
	m_ListCtrlX.InsertColumn(U_CHILDREN, L"gyerekek", LVCFMT_LEFT, 1500, -1, COL_TEXT);

	inputFile();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitedEntriesX::inputFile()
{
	CString fileSpec;
	CString filename;

	m_command.Format(L"SELECT filespec FROM filespec WHERE type=%d", X_FILE );
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
		str.Format(L"%s fájl nem létezik!", fileSpec);
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

	str = L"Kérdéses bejegyzések beolvasása...";

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

	m_ListCtrlX.DeleteAllItems();
	m_ListCtrlX.SetItemCountEx(vPeople.size() + 1);
	m_ListCtrlX.AttachDataset(&vPeople);
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::push(int i)
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
void CUnitedEntriesX::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int rgbcolor = WHITE;

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
		rgbcolor = _wtoi(m_ListCtrlX.GetItemText(nItem, U_DUMMY));
		if (rgbcolor )
		{
			rgbcolor = m_rgbX[rgbcolor];
			pLVCD->clrTextBk = rgbcolor;
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::atvett()
{
	CString code = L"^^^";
	int nItemFirst;
	int nItem = m_ListCtrlX.GetTopIndex() + 1;

	theApp.unselectAll(&m_ListCtrlX);
	for (; nItem < m_ListCtrlX.GetItemCount(); ++nItem)
	{
		for (int i = 3; i < U_COLUMNSCOUNT; ++i)
		{
			if (m_ListCtrlX.GetItemText(nItem, i).Find(code) != -1)
			{

				nItemFirst = nItem + m_ListCtrlX.GetCountPerPage() - 1;
				if (nItemFirst > m_ListCtrlX.GetItemCount() - 1) nItemFirst = m_ListCtrlX.GetItemCount() - m_ListCtrlX.GetCountPerPage() - 1;

				m_ListCtrlX.EnsureVisible(nItemFirst, false);
				m_ListCtrlX.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				return;
			}
		}
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
/*
	CString status = m_ListCtrlX.GetItemText(nItem, U_STATUS);
	if (status.IsEmpty()) return;  // üres sor

	if (status == L"0")
	{
		AfxMessageBox(L"A bejegyzést már töröltük!");
		return;
	}
*/

	CRelatives  dlg;
	dlg.m_rowid = m_ListCtrlX.GetItemText(nItem, U_ROWID);
	if (dlg.DoModal() == IDCANCEL) return;

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// D R O P D O W N   F U N C I Ó K  /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////û
LRESULT CUnitedEntriesX::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CUnitedEntriesX::OnHtmlEditLines()
{
	CString title;
	int selectedCount = m_ListCtrlX.GetSelectedCount();
	int nItem = m_ListCtrlX.GetNextItem(-1, LVNI_SELECTED);
	if (selectedCount == 1)
		title.Format(L"%s a ga.html fájlban (%s. sor)", m_ListCtrlX.GetItemText(nItem, U_NAME), m_ListCtrlX.GetItemText(nItem, U_LINE));
	else
		title.Format(L"%d kijelölt sor a ga.html fájlban", selectedCount);

	theApp.htmlEditLines(&m_ListCtrlX, U_LINE, title);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::OnHtmlNotepad()
{
	int nItem = m_ListCtrlX.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlX.GetItemText(nItem, U_LINE);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::OnCompare2People()
{
	int cnt = m_ListCtrlX.GetSelectedCount();
	if (cnt != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni");
		return;
	}
	POSITION	pos = m_ListCtrlX.GetFirstSelectedItemPosition();

	int nItem1 = m_ListCtrlX.GetNextSelectedItem(pos);
	int nItem2 = m_ListCtrlX.GetNextSelectedItem(pos);

	CCompareEntries dlg;
	dlg.rowid1 = m_ListCtrlX.GetItemText(nItem1, U_ROWID);
	dlg.rowid2 = m_ListCtrlX.GetItemText(nItem2, U_ROWID);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 létezõ bejegyzést jelölj meg!");
		return;
	}
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::OnFilterByName()
{
	int nItem = m_ListCtrlX.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(L"Nincs kijelölve egy sor !");
		return;
	}


	CString name;
	CString nev;
	int pos;
	name = m_ListCtrlX.GetItemText(nItem, U_NAME);
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
	m_ListCtrlX.SetItemCountEx(vFiltered.size() + 1);
	m_ListCtrlX.AttachDataset(&vFiltered);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::OnFilterByMother()
{
	int nItem = m_ListCtrlX.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(L"Nincs kijelölve egy sor !");
		return;
	}


	CString name;
	CString nev;
	int pos;
	name = m_ListCtrlX.GetItemText(nItem, U_MOTHER);
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
	m_ListCtrlX.SetItemCountEx(vFiltered.size() + 1);
	m_ListCtrlX.AttachDataset(&vFiltered);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::pushLine(int i)
{
	for (UINT j = 0; j < U_COLUMNSCOUNT; ++j) // ix-1 a cnt-re mutat
	{
		vFiltered.push_back(vPeople.at(i + j));
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::OnUnite()
{
	POSITION pos = m_ListCtrlX.GetFirstSelectedItemPosition();
	UNITE_ENTRIES unite;
	vUnite.clear();
	int nItem;

	while (pos)
	{
		nItem = m_ListCtrlX.GetNextSelectedItem(pos);
		unite.nItem = nItem;
		unite.rowid = m_ListCtrlX.GetItemText(nItem, U_ROWID);
		unite.sex_id = m_ListCtrlX.GetItemText(nItem, U_SEX_ID);
		unite.source = _wtoi(m_ListCtrlX.GetItemText(nItem, U_SOURCE));
		vUnite.push_back(unite);
	}
	if (vUnite.size() < 2)
	{
		AfxMessageBox(L"Legalább 2 azonos nevû bejegyzést ki kel jelölnöd!");
		return;
	}
	std::sort(vUnite.begin(), vUnite.end(), sortCBySource);

	// ez elsõ bejegyzést kell megtartani
	CString rowidKeep = vUnite.at(0).rowid;
	int nItem0 = vUnite.at(0).nItem;
	CString sex_id = vUnite.at(0).sex_id;

	CString rowidDel;
	CString rowidH;
	CString rowidW;
	CString rowidC;
	int ix;
	int base;

	for (int i = 1; i < vUnite.size(); ++i)
	{
		nItem = vUnite.at(i).nItem;
		rowidDel = vUnite.at(i).rowid;
		// gyerekek szüleinek update-elése
		if (sex_id == MAN)
		{
			m_command.Format(L"UPDATE people SET father_id = '%s' WHERE father_id='%s'", rowidKeep, rowidDel);
			if (!theApp.execute(m_command)) return;
		}
		else
		{
			m_command.Format(L"UPDATE people SET mother_id = '%s' WHERE mother_id='%s'", rowidKeep, rowidDel);
			if (!theApp.execute(m_command)) return;
		}
		// házassági bejegyzések törlése
		if (sex_id == MAN)
			m_command.Format(L"DELETE FROM marriages WHERE husband_id ='%s'", rowidDel);
		else
			m_command.Format(L"DELETE FROM marriages WHERE wife_id ='%s'", rowidDel);
		if (!theApp.execute(m_command)) return;

		// törlendõ bejegyzés törlése
		m_command.Format(L"DELETE FROM people WHERE rowid ='%s'", rowidDel);
		if (!theApp.execute(m_command)) return;

		// gyerekek átvitele a másik házastárshoz
		if (sex_id == MAN)
		{
			m_command.Format(L"SELECT wife_id FROM marriages WHERE husband_id = '%s'", rowidKeep);
			if (!theApp.query(m_command)) return;
			if (theApp.m_recordset->RecordsCount() == 1)
			{
				rowidW = theApp.m_recordset->GetFieldString(0);		// házastárs rowidja
				// gyerekek rowid-ja
				m_command.Format(L"SELECT rowid FROM people WHERE father_id == '%s'", rowidKeep);
				if (!theApp.query(m_command)) return;
				for (int j = 0; j < theApp.m_recordset->RecordsCount(); ++j)
				{
					rowidC = theApp.m_recordset->GetFieldString(0);
					m_command.Format(L"UPDATE people SET mother_id = '%s' WHERE rowid= '%s'", rowidW, rowidC);
					if (!theApp.execute(m_command)) return;
					theApp.m_recordset->MoveNext();
				}
			}
		}
		else
		{
			m_command.Format(L"SELECT husband_id FROM marriages WHERE wife_id = '%s'", rowidKeep);
			if (!theApp.query(m_command)) return;
			if (theApp.m_recordset->RecordsCount() == 1)
			{
				rowidH = theApp.m_recordset->GetFieldString(0);		// házastárs rowidja
				// gyerekek rowid-ja
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id == '%s'", rowidKeep);
				if (!theApp.query(m_command)) return;
				for (int j = 0; j < theApp.m_recordset->RecordsCount(); ++j)
				{
					rowidC = theApp.m_recordset->GetFieldString(0);
					m_command.Format(L"UPDATE people SET father_id = '%s' WHERE rowid= '%s'", rowidH, rowidC );
					if (!theApp.execute(m_command)) return;
					theApp.m_recordset->MoveNext();
				}
			}
		}


		// törölt bejegyzés törlése
		base = nItem * U_COLUMNSCOUNT;
		for (int j = 0; j < U_COLUMNSCOUNT; ++j)
		{
			ix = base + j;
			vPeople.at(ix) = L" ";
		}
	}
	m_ListCtrlX.DeleteAllItems();
	m_ListCtrlX.SetItemCountEx(vPeople.size() + 1);
	m_ListCtrlX.AttachDataset(&vPeople);

	m_ListCtrlX.EnsureVisible(nItem0 + m_ListCtrlX.GetCountPerPage() - 1, false);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesX::OnCompareAscendants()
{
	int db = m_ListCtrlX.GetSelectedCount();
	if (db != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni!");
		return;
	}

	POSITION	pos = m_ListCtrlX.GetFirstSelectedItemPosition();
	int nItem;

	CString rowid1;
	CString rowid2;

	nItem = m_ListCtrlX.GetNextSelectedItem(pos);
	rowid1 = m_ListCtrlX.GetItemText(nItem, U_ROWID);

	nItem = m_ListCtrlX.GetNextSelectedItem(pos);
	rowid2 = m_ListCtrlX.GetItemText(nItem, U_ROWID);

	CCompareAsc dlg;
	dlg.rowid1 = rowid1;
	dlg.rowid2 = rowid2;
	dlg.DoModal();
}
