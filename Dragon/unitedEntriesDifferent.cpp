// contractEntriesDifferent.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "unitedEntries.h"
#include "unitedEntriesDifferent.h"
#include "relatives.h"
#include "compareEntries.h"
#include "compareAscendants.h"
#include "utilities_dragon.h"
#include <algorithm>
// CUnitedEntriesDifferent dialog

IMPLEMENT_DYNAMIC(CUnitedEntriesDifferent, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnitedEntriesDifferent::CUnitedEntriesDifferent(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNITED_ENTRIES_DIFFERENT, pParent)
{


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnitedEntriesDifferent::~CUnitedEntriesDifferent()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesDifferent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIFFERENT, m_ListCtrlD);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUnitedEntriesDifferent, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DIFFERENT, &CUnitedEntriesDifferent::OnDblclkList)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_NOTEPAD, &CUnitedEntriesDifferent::OnHtmlNotepad)
	ON_COMMAND(ID_COMPARE_2PEOPLE, &CUnitedEntriesDifferent::OnCompare2People)
	ON_COMMAND(ID_COMPARE_ASCENDANTS, &CUnitedEntriesDifferent::OnCompareAscendants)
	ON_COMMAND(ID_FILTER_BY_NAME, &CUnitedEntriesDifferent::OnFilterByName)
	ON_COMMAND(ID_UNITE2, &CUnitedEntriesDifferent::OnUnite)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUnitedEntriesDifferent::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ListCtrlD.KeepSortOrder(TRUE);
	m_ListCtrlD.SetExtendedStyle(m_ListCtrlD.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrlD.InsertColumn(U_DUMMY, L"", LVCFMT_RIGHT, 35, -1, COL_HIDDEN);
	m_ListCtrlD.InsertColumn(U_LOOP, L"loop", LVCFMT_RIGHT, 35, -1, COL_NUM);
	m_ListCtrlD.InsertColumn(U_GROUP, L"gr", LVCFMT_RIGHT, 30, -1, COL_NUM);
	m_ListCtrlD.InsertColumn(U_MATCH, L"m#", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlD.InsertColumn(U_STATUS, L"st", LVCFMT_RIGHT, 30, -1, COL_NUM);

	m_ListCtrlD.InsertColumn(U_UNITED, L"u", LVCFMT_LEFT, 20, -1, COL_NUM);
	m_ListCtrlD.InsertColumn(U_SOURCE, L"s", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrlD.InsertColumn(U_INDEX, L"i", LVCFMT_RIGHT, 20, -1, COL_NUM);

	m_ListCtrlD.InsertColumn(U_LINE, L"line#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlD.InsertColumn(U_ROWID, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlD.InsertColumn(U_NAME, L"ember", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlD.InsertColumn(U_SEX_ID, L"sex", LVCFMT_LEFT, 30, -1, COL_TEXT);
	m_ListCtrlD.InsertColumn(U_BIRTH, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlD.InsertColumn(U_DEATH, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlD.InsertColumn(U_LINEF, L"line", LVCFMT_RIGHT, 50, -1, COL_HIDDEN);
	m_ListCtrlD.InsertColumn(U_ROWIDF, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlD.InsertColumn(U_FATHER, L"apa", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlD.InsertColumn(U_BIRTHF, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlD.InsertColumn(U_DEATHF, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlD.InsertColumn(U_LINEM, L"", LVCFMT_RIGHT, 50, -1, COL_HIDDEN);
	m_ListCtrlD.InsertColumn(U_ROWIDM, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlD.InsertColumn(U_MOTHER, L"anya", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlD.InsertColumn(U_BIRTHM, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlD.InsertColumn(U_DEATHM, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlD.InsertColumn(U_SPOUSES, L"h�zast�rsak", LVCFMT_LEFT, 1000, -1, COL_TEXT);
	m_ListCtrlD.InsertColumn(U_CHILDREN, L"gyerekek", LVCFMT_LEFT, 1500, -1, COL_TEXT);

	inputFile();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitedEntriesDifferent::inputFile()
{
	CString fileSpec;
	CString filename;

	m_command.Format(L"SELECT filespec FROM filespec WHERE type=%d", DIFFERENT_FILE);
	if (!theApp.query(m_command)) return false;
	fileSpec = theApp.m_recordset->GetFieldString(0);
	if (fileSpec.IsEmpty())
	{
		str = L"A nme �sszevonhat� bejegyz�seket tartalmaz� f�jl nincs specifik�lva!";
		AfxMessageBox(str, MB_ICONEXCLAMATION);
		return false;
	}
	filename = fileSpec.Mid(fileSpec.ReverseFind('\\') + 1);
	fileSpec.Format(L"%s\\%s", theApp.m_dbFolderPath, filename);

	if (_waccess(fileSpec, 0))
	{
		str.Format(L"CUnitedEntriesDifferent\n%s f�jl nem l�tezik!", fileSpec);
		AfxMessageBox(str, MB_ICONERROR);
		return false;
	}

	CStdioFile file(fileSpec, CFile::modeRead);   // input csv f�jl
	int fileLength = (int)file.GetLength();

	file.ReadString(str);  // az els� felrat sor beolvas�sa
	

	str = L"Nem egyes�thet� bejegyz�sek beolvas�sa...";
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, fileLength);
	wndP.SetPos(0);
	
	int n;
	int cnt = 0;
	CString cLine;
	vPeople.clear();
	while (file.ReadString(cLine))
	{
		++cnt;
		A.RemoveAll();
		n = wordList(&A, cLine, '\t', true);
		if (n != U_COLUMNSCOUNT)
		{
			str.Format(L"A %s\nf�jl %d. sorban az elemek sz�ma %d.\n%d kellen lenni.", fileSpec, cnt, n, U_COLUMNSCOUNT);
			AfxMessageBox(str, MB_ICONERROR);
			return false;
		}

		for (UINT i = 0; i < A.GetSize(); ++i)
			push(i);

		wndP.SetPos(file.GetPosition());
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	file.Close();

	m_ListCtrlD.DeleteAllItems();
	m_ListCtrlD.SetItemCountEx(vPeople.size() + 1);
	m_ListCtrlD.AttachDataset( &vPeople);

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesDifferent::push(int i)
{
	CString item = A[i];
	int tLen;
	TCHAR* sT;
	tLen = item.GetLength() + 1;
	sT = new TCHAR[tLen];
	_tcscpy_s(sT, tLen, item.GetBuffer());
	vPeople.push_back(sT);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesDifferent::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;

	CString status = m_ListCtrlD.GetItemText(nItem, U_STATUS);
	if (status.IsEmpty()) return;  // �res sor

	if (status == L"0")
	{
		AfxMessageBox(L"A bejegyz�st m�r t�r�lt�k!");
		return;
	}


	CRelatives  dlg;
	dlg.m_rowid = m_ListCtrlD.GetItemText(nItem, U_ROWID);
	if (dlg.DoModal() == IDCANCEL) return;

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// D R O P D O W N   F U N C I � K  /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�
LRESULT CUnitedEntriesDifferent::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup;

	if (Menu.LoadMenu(IDR_DROPDOWN_UNITED_DIFFERENT ))
	{
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesDifferent::OnHtmlNotepad()
{
	int nItem = m_ListCtrlD.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlD.GetItemText(nItem, U_LINE);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesDifferent::OnCompare2People()
{
	int cnt = m_ListCtrlD.GetSelectedCount();
	if (cnt != 2)
	{
		AfxMessageBox(L"2 bejegyz�st kell kijel�lni");
		return;
	}
	POSITION	pos = m_ListCtrlD.GetFirstSelectedItemPosition();

	int nItem1 = m_ListCtrlD.GetNextSelectedItem(pos);
	int nItem2 = m_ListCtrlD.GetNextSelectedItem(pos);

	CCompareEntries dlg;
	dlg.rowid1 = m_ListCtrlD.GetItemText(nItem1, U_ROWID);
	dlg.rowid2 = m_ListCtrlD.GetItemText(nItem2, U_ROWID);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 l�tez� bejegyz�st jel�lj meg!");
		return;
	}
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesDifferent::OnCompareAscendants()
{
	int db = m_ListCtrlD.GetSelectedCount();
	if (db != 2)
	{
		AfxMessageBox(L"2 bejegyz�st kell kijel�lni!");
		return;
	}

	POSITION	pos = m_ListCtrlD.GetFirstSelectedItemPosition();
	int nItem;

	CString rowid1;
	CString rowid2;

	nItem = m_ListCtrlD.GetNextSelectedItem(pos);
	rowid1 = m_ListCtrlD.GetItemText(nItem, U_ROWID);

	nItem = m_ListCtrlD.GetNextSelectedItem(pos);
	rowid2 = m_ListCtrlD.GetItemText(nItem, U_ROWID);

	CCompareAsc dlg;
	dlg.rowid1 = rowid1;
	dlg.rowid2 = rowid2;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesDifferent::OnFilterByName()
{
	int nItem = m_ListCtrlD.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(L"Nincs kijel�lve egy sor !");
		return;
	}


	CString name;
	CString nev;
	int pos;
	name = m_ListCtrlD.GetItemText(nItem, U_NAME);
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
			pushLine(i);		// szepar�l� �res sor
			pushed = false;
		}
	}
	m_ListCtrlD.SetItemCountEx(vFiltered.size() + 1);
	m_ListCtrlD.AttachDataset(&vFiltered);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesDifferent::pushLine(int i)
{
	for (UINT j = 0; j < U_COLUMNSCOUNT; ++j) // ix-1 a cnt-re mutat
	{
		vFiltered.push_back(vPeople.at(i + j));
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tetsz�leges sz�m� kijel�lt bejegyz�s k�zi egyes�t�se
//	Az els� kijel�lt bejkegyz�st tartja meg. 
// 
//
void CUnitedEntriesDifferent::OnUnite()
{
	POSITION pos = m_ListCtrlD.GetFirstSelectedItemPosition();
	UNITE_ENTRIES unite;
	std::vector<UNITE_ENTRIES> vUnite;
	vUnite.clear();
	int nItem;
	int base;

	while (pos)
	{
		nItem = m_ListCtrlD.GetNextSelectedItem(pos);
		unite.nItem = nItem;
		unite.rowid = m_ListCtrlD.GetItemText(nItem, U_ROWID);
		unite.sex_id = m_ListCtrlD.GetItemText(nItem, U_SEX_ID);
		unite.source = _wtoi(m_ListCtrlD.GetItemText(nItem, U_SOURCE));
		vUnite.push_back(unite);
	}
	if (vUnite.size() < 2)
	{
		AfxMessageBox(L"Legal�bb 2 azonos nev� bejegyz�st ki kel jel�ln�d!");
		return;
	}

// ez els� bejegyz�st (legkisebb n�v-t�pus�) kell megtartani
	std::sort(vUnite.begin(), vUnite.end(), sortCBySource);
	CString rowidKeep = vUnite.at(0).rowid;
	int nItem0 = vUnite.at(0).nItem;
	CString sex_id = vUnite.at(0).sex_id;

	CString rowidDel;
	int ix;

	for (int i = 1; i < vUnite.size(); ++i)
	{
		nItem = vUnite.at(i).nItem;
		rowidDel = vUnite.at(i).rowid;
		saveData(rowidKeep, rowidDel);
		// gyerekek sz�leinek update-el�se
		if (sex_id == MAN)
			m_command.Format(L"UPDATE people SET father_id = '%s' WHERE father_id='%s'", rowidKeep, rowidDel);
		else
			m_command.Format(L"UPDATE people SET mother_id = '%s' WHERE mother_id='%s'", rowidKeep, rowidDel);
		if (!theApp.execute(m_command)) return;

		// h�zass�gi bejegyz�sek t�rl�se
		if (sex_id == MAN)
			m_command.Format(L"DELETE FROM marriages WHERE husband_id ='%s'", rowidDel);
		else
			m_command.Format(L"DELETE FROM marriages WHERE wife_id ='%s'", rowidDel);
		if (!theApp.execute(m_command)) return;

		// t�rlend� bejegyz�s t�rl�se
		m_command.Format(L"DELETE FROM people WHERE rowid ='%s'", rowidDel);
		if (!theApp.execute(m_command)) return;

		m_command.Format(L"UPDATE people SET united=united+1 WHERE rowid ='%s'", rowidKeep);
		if (!theApp.execute(m_command)) return;


		// t�r�lt bejegyz�s t�rl�se
		base = nItem * U_COLUMNSCOUNT;
		for (int j = 0; j < U_COLUMNSCOUNT; ++j)
		{
			ix = base + j;
			vPeople.at(ix) = L" ";
		}

		m_ListCtrlD.DeleteAllItems();
		m_ListCtrlD.SetItemCountEx(vPeople.size() + 1);
		m_ListCtrlD.AttachDataset(&vPeople);

		m_ListCtrlD.EnsureVisible(nItem0 + m_ListCtrlD.GetCountPerPage() - 1, false);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesDifferent::saveData(CString rowidKeep, CString rowidDel)
{
/*
	itemI = vSameNames.at(i).peer;
	itemJ = vSameNames.at(j).peer;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET peer='%s' WHERE rowid = '%s'", itemJ, rowidI);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).peer += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET peer='%s' WHERE rowid = '%s'", itemI, rowidJ);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(j).peer += mark;
	}

	itemI = vSameNames.at(i).titolo;
	itemJ = vSameNames.at(j).titolo;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET titolo='%s' WHERE rowid = '%s'", itemJ, rowidI);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).titolo += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET titolo='%s' WHERE rowid = '%s'", itemI, rowidJ);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(j).titolo += mark;
	}

	if (vSameNames.at(i).rowidF.IsEmpty() && !vSameNames.at(j).rowidF.IsEmpty())
	{
		m_command.Format(L"UPDATE people SET father_id='%s' WHERE rowid = '%s'", vSameNames.at(j).rowidF, rowidI);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).father += mark;
	}
	else if (!vSameNames.at(i).rowidF.IsEmpty() && vSameNames.at(j).rowidF.IsEmpty())
	{
		m_command.Format(L"UPDATE people SET father_id='%s' WHERE rowid = '%s'", vSameNames.at(i).rowidF, rowidJ);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(j).father += mark;
	}

	if (vSameNames.at(i).rowidM.IsEmpty() && !vSameNames.at(j).rowidM.IsEmpty())
	{
		m_command.Format(L"UPDATE people SET mother_id='%s' WHERE rowid = '%s'", vSameNames.at(j).rowidM, rowidI);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).mother += mark;
	}
	else if (!vSameNames.at(i).rowidM.IsEmpty() && vSameNames.at(j).rowidM.IsEmpty())
	{
		m_command.Format(L"UPDATE people SET mother_id='%s' WHERE rowid = '%s'", vSameNames.at(i).rowidM, rowidJ);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).mother += mark;
	}


	itemI = vSameNames.at(i).birth;
	itemJ = vSameNames.at(j).birth;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET birth_date='%s' WHERE rowid = '%s'", itemJ, rowidI);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).birth += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET birth_date='%s' WHERE rowid = '%s'", itemI, rowidJ);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).birth += mark;
	}



	itemI = vSameNames.at(i).death;
	itemJ = vSameNames.at(j).death;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET death_date='%s' WHERE rowid = '%s'", itemJ, rowidI);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).death += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET death_date='%s' WHERE rowid = '%s'", itemI, rowidJ);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).death += mark;
	}


	itemI = vSameNames.at(i).birthPlace;
	itemJ = vSameNames.at(j).birthPlace;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET birth_place='%s' WHERE rowid = '%s'", itemJ, rowidI);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).birthPlace += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET birth_place='%s' WHERE rowid = '%s'", itemI, rowidJ);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).birthPlace += mark;
	}



	itemI = vSameNames.at(i).deathPlace;
	itemJ = vSameNames.at(j).deathPlace;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET death_place='%s' WHERE rowid = '%s'", itemJ, rowidI);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).deathPlace += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET death_place='%s' WHERE rowid = '%s'", itemI, rowidJ);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).deathPlace += mark;
	}

	itemI = vSameNames.at(i).comment;
	itemJ = vSameNames.at(j).comment;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET comment='%s' WHERE rowid = '%s'", itemJ, rowidI);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).comment += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET comment='%s' WHERE rowid = '%s'", itemI, rowidJ);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).comment += mark;
	}


	itemI = vSameNames.at(i).birthF;
	itemJ = vSameNames.at(j).birthF;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET birth_date='%s' WHERE rowid = '%s'", itemJ, vSameNames.at(i).rowidF);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).birthF += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET birth_date='%s' WHERE rowid = '%s'", itemI, vSameNames.at(j).rowidF);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).birthF += mark;
	}

	itemI = vSameNames.at(i).deathF;
	itemJ = vSameNames.at(j).deathF;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET death_date='%s' WHERE rowid = '%s'", itemJ, vSameNames.at(i).rowidF);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).deathF += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET death_date='%s' WHERE rowid = '%s'", itemI, vSameNames.at(j).rowidF);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).deathF += mark;
	}

	itemI = vSameNames.at(i).birthM;
	itemJ = vSameNames.at(j).birthM;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET birth_date='%s' WHERE rowid = '%s'", itemJ, vSameNames.at(i).rowidM);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).birthM += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET birth_date='%s' WHERE rowid = '%s'", itemI, vSameNames.at(j).rowidM);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).birthM += mark;
	}

	itemI = vSameNames.at(i).deathM;
	itemJ = vSameNames.at(j).deathM;
	if (itemI.GetLength() < itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET death_date='%s' WHERE rowid = '%s'", itemJ, vSameNames.at(i).rowidM);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).deathM += mark;
	}
	else if (itemI.GetLength() > itemJ.GetLength())
	{
		m_command.Format(L"UPDATE people SET death_date='%s' WHERE rowid = '%s'", itemI, vSameNames.at(j).rowidM);
		if (!theApp.execute(m_command)) return false;
		vSameNames.at(i).deathM += mark;
	}

	spouseIndexI = vSameNames.at(i).spouseIndex;
	spouseIndexJ = vSameNames.at(j).spouseIndex;
	if (!spouseIndexI && spouseIndexJ)
	{
		m_command.Format(L"UPDATE people SET spouseIndex='%d' WHERE rowid = '%s'", spouseIndexJ, vSameNames.at(i).rowidM);
		if (!theApp.execute(m_command)) return false;
	}
	else if (spouseIndexI && !spouseIndexJ)
	{
		m_command.Format(L"UPDATE people SET spouseIndex='%d' WHERE rowid = '%s'", spouseIndexI, vSameNames.at(j).rowidM);
		if (!theApp.execute(m_command)) return false;
	}
*/
}