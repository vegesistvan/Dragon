// CCompareEntries.cpp : implementation file
//

#include "pch.h"
#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "compareEntries.h"
#include <algorithm>


/*
// CCompareEntries dialog
enum
{
	S = 0,
	X,  // source
	L,	// linenumber			emberünk
	R,	// rowid
	N,	// név
	B,	// születés
	D,	// halál

	LF,	// lineneumber			apja
	RF,	// rowid
	NF,	// apa neve
	BF,	// születés
	DF,	// halál

	LM,	// linenumber			anyja
	RM,	// rowid
	NM,	// anya neve
	BM,	// születés
	DM,	// halál
};
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortBySpouseX(const NONUNITED& v1, const NONUNITED& v2)
{
	if (v1.name > v2.name)
		return false;
	if (v1.name == v2.name)
	{
		if (v1.sorszam < v2.sorszam)
			return true;
		else
			return false;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortByXSpouse(const NONUNITED& v1, const NONUNITED& v2)
{
	if (v1.sorszam < v2.sorszam)
		return false;
	if (v1.sorszam == v2.sorszam)
	{
		if (v1.name < v2.name)
			return true;
		else
			return false;
	}
	return true;
}
IMPLEMENT_DYNAMIC(CCompareEntries, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCompareEntries::CCompareEntries(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMPARE_ENTRIES, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCompareEntries::~CCompareEntries()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl1);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl2);
	DDX_Control(pDX, IDC_LIST3, m_ListCtrl3);
	DDX_Control(pDX, IDC_LIST4, m_ListCtrl4);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCompareEntries, CDialogEx)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CCompareEntries::OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CCompareEntries::OnDblclkList2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, &CCompareEntries::OnDblclkList3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST4, &CCompareEntries::OnDblclkList4)
	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CCompareEntries::OnCustomdrawList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &CCompareEntries::OnCustomdrawList2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST3, &CCompareEntries::OnCustomdrawList3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST4, &CCompareEntries::OnCustomdrawList4)
	ON_COMMAND(ID_ROKONOK, &CCompareEntries::OnRokonok)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCompareEntries::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	str.Format(L"Azonos nevû, de nem összevonható bejegyzések.            Bármely névre dupla kattintás a bal egérgombbal és a notepad++ ban javíthatjuk a hibát!");
	SetWindowTextW(str);

	createScreen();

	vS.clear();
	if (!getRelatives(L"1", rowid1, &n1)) OnCancel();
	if (!getRelatives(L"2", rowid2, &n2)) OnCancel();
	displayData();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCompareEntries::createScreen()
{
	m_ListCtrl1.DeleteAllItems();

	m_ListCtrl1.KeepSortOrder(TRUE);
	m_ListCtrl1.SetExtendedStyle(m_ListCtrl1.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrl1.InsertColumn(S, L"", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrl1.InsertColumn(X, L"s", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrl1.InsertColumn(L, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN );
	m_ListCtrl1.InsertColumn(R, L"rowid", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl1.InsertColumn(N, L"", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl1.InsertColumn(B, L"", LVCFMT_LEFT, 75, -1, COL_TEXT);
	m_ListCtrl1.InsertColumn(D, L"", LVCFMT_LEFT,  80, -1, COL_TEXT);

	m_ListCtrl1.InsertColumn(LF, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl1.InsertColumn(RF, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl1.InsertColumn(NF, L"apja", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl1.InsertColumn(BF, L"", LVCFMT_LEFT, 75, -1, COL_NUM);
	m_ListCtrl1.InsertColumn(DF, L"", LVCFMT_LEFT, 80, -1, COL_NUM);

	m_ListCtrl1.InsertColumn(LM, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl1.InsertColumn(RM, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl1.InsertColumn(NM, L"anyja", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl1.InsertColumn(BM, L"", LVCFMT_LEFT, 75, -1, COL_NUM);
	m_ListCtrl1.InsertColumn(DM, L"", LVCFMT_LEFT, 80, -1, COL_NUM);


	m_ListCtrl2.KeepSortOrder(TRUE);
	m_ListCtrl2.SetExtendedStyle(m_ListCtrl2.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrl2.InsertColumn(S, L"", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrl2.InsertColumn(X, L"", LVCFMT_RIGHT, 20, -1, COL_NUM);

	m_ListCtrl2.InsertColumn(L, L"", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl2.InsertColumn(R, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl2.InsertColumn(N, L"", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl2.InsertColumn(B, L"", LVCFMT_LEFT, 75, -1, COL_TEXT);
	m_ListCtrl2.InsertColumn(D, L"", LVCFMT_LEFT, 80, -1, COL_TEXT);

	m_ListCtrl2.InsertColumn(LF, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl2.InsertColumn(RF, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl2.InsertColumn(NF, L"apai nagyapa", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl2.InsertColumn(BF, L"", LVCFMT_LEFT, 75, -1, COL_NUM);
	m_ListCtrl2.InsertColumn(DF, L"", LVCFMT_LEFT, 80, -1, COL_NUM);

	m_ListCtrl2.InsertColumn(LM, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl2.InsertColumn(RM, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl2.InsertColumn(NM, L"apai nagyanya", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl2.InsertColumn(BM, L"", LVCFMT_LEFT, 75, -1, COL_NUM);
	m_ListCtrl2.InsertColumn(DM, L"", LVCFMT_LEFT, 80, -1, COL_NUM);

	m_ListCtrl3.KeepSortOrder(TRUE);
	m_ListCtrl3.SetExtendedStyle(m_ListCtrl3.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrl3.InsertColumn(S, L"", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrl3.InsertColumn(X, L"", LVCFMT_RIGHT, 20, -1, COL_NUM);

	m_ListCtrl3.InsertColumn(L, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl3.InsertColumn(R, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl3.InsertColumn(N, L"", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl3.InsertColumn(B, L"", LVCFMT_LEFT, 75, -1, COL_TEXT);
	m_ListCtrl3.InsertColumn(D, L"", LVCFMT_LEFT, 80, -1, COL_TEXT);

	m_ListCtrl3.InsertColumn(LF, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl3.InsertColumn(RF, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl3.InsertColumn(NF, L"anyai nagyapa", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl3.InsertColumn(BF, L"", LVCFMT_LEFT, 75, -1, COL_NUM);
	m_ListCtrl3.InsertColumn(DF, L"", LVCFMT_LEFT, 80, -1, COL_NUM);

	m_ListCtrl3.InsertColumn(LM, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl3.InsertColumn(RM, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl3.InsertColumn(NM, L"anyai nagyanya", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl3.InsertColumn(BM, L"", LVCFMT_LEFT, 75, -1, COL_NUM);
	m_ListCtrl3.InsertColumn(DM, L"", LVCFMT_LEFT, 80, -1, COL_NUM);







	m_ListCtrl4.KeepSortOrder(FALSE);
	m_ListCtrl4.SetExtendedStyle(m_ListCtrl4.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrl4.DeleteAllItems();
	m_ListCtrl4.InsertColumn(S, L"", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrl4.InsertColumn(X, L"s", LVCFMT_RIGHT, 20, -1, COL_NUM);

	m_ListCtrl4.InsertColumn(L, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl4.InsertColumn(R, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl4.InsertColumn(N, L"házastárs", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl4.InsertColumn(B, L"", LVCFMT_LEFT, 75, -1, COL_NUM);
	m_ListCtrl4.InsertColumn(D, L"", LVCFMT_LEFT,  80, -1, COL_NUM);

	m_ListCtrl4.InsertColumn(LF, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl4.InsertColumn(RF, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl4.InsertColumn(NF, L"apja", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl4.InsertColumn(BF, L"", LVCFMT_LEFT, 75, -1, COL_NUM);
	m_ListCtrl4.InsertColumn(DF, L"", LVCFMT_LEFT, 80, -1, COL_NUM);

	m_ListCtrl4.InsertColumn(LM, L"line", LVCFMT_RIGHT, 70, -1, COL_HIDDEN);
	m_ListCtrl4.InsertColumn(RM, L"", LVCFMT_LEFT, 60, -1, COL_TEXT);
	m_ListCtrl4.InsertColumn(NM, L"anyja", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrl4.InsertColumn(BM, L"", LVCFMT_LEFT, 75, -1, COL_NUM);
	m_ListCtrl4.InsertColumn(DM, L"", LVCFMT_LEFT, 80, -1, COL_NUM);
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCompareEntries::getRelatives(CString sorszam, CString rowid, NONUNITED* n)
{
	NONUNITED nu;
	CString spouse_id;
	CString name;


	clearN( n);
	// emberünk
	if (!getPeople(rowid))
	{
		str.Format(L"%s rowid nincs az adatbázisban!", rowid);
		AfxMessageBox(str);
		return false;
	}
	n->sorszam = sorszam;
	n->source = p.source;
	n->line = p.line;
	n->rowid = p.rowid;
	n->name = p.name;
	n->birth = p.birth;
	n->death = p.death;
	n->rowidF = p.father_id;
	n->rowidM = p.mother_id;

	
	// apja
	getPeople(n->rowidF);
	n->lineF = p.line;
	n->rowidF = p.rowid;
	n->father = p.name;
	n->birthF = p.birth;
	n->deathF = p.death;
	n->rowidGF = p.father_id;
	n->rowidGM = p.mother_id;


	// anyja
	getPeople(n->rowidM);
	n->lineM = p.line;
	n->rowidM = p.rowid;
	n->mother = p.name;
	n->birthM = p.birth;
	n->deathM = p.death;
	n->rowidGF2 = p.father_id;
	n->rowidGM2 = p.mother_id;


	// apai nagyapa
	getPeople(n->rowidGF);
	n->lineGF = p.line;
	n->rowidGF = p.rowid;
	n->grandfather = p.name;
	n->birthGF = p.birth;
	n->deathGF = p.death;

	// apai nagyanya
	getPeople(n->rowidGM);
	n->lineGM = p.line;
	n->rowidGM = p.rowid;
	n->grandmother = p.name;
	n->birthGM = p.birth;
	n->deathGM = p.death;


	// anyai nagyapa
	getPeople(n->rowidGF2);
	n->lineGF2 = p.line;
	n->rowidGF2 = p.rowid;
	n->grandfather2 = p.name;
	n->birthGF2 = p.birth;
	n->deathGF2 = p.death;


	// anyai nagyanya
	getPeople(n->rowidGM2);
	n->lineGM2 = p.line;
	n->rowidGM2 = p.rowid;
	n->grandmother2 = p.name;
	n->birthGM2 = p.birth;
	n->deathGM2 = p.death;



	// házastársai

	m_command.Format(L"SELECT husband_id, wife_id FROM marriages WHERE husband_id='%s' OR wife_id = '%s'", rowid, rowid );
	if (!theApp.query(m_command)) return false;

	
	for (UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i)
	{
		spouse_id = theApp.m_recordset->GetFieldString(0);
		if( spouse_id == rowid )
			spouse_id = theApp.m_recordset->GetFieldString(1);

		getPeople(spouse_id);
		nu.sorszam = sorszam;
		nu.source = p.source;
		nu.rowid = p.rowid;
		nu.line = p.line;
		nu.name = p.name;
		nu.birth = p.birth;
		nu.death = p.death;
		nu.rowidF = p.father_id;
		nu.rowidM = p.mother_id;

		getPeople(nu.rowidF);
		nu.lineF	= p.line;
		nu.rowidF	= p.rowid;
		nu.father	= p.name;
		nu.birthF = p.birth;
		nu.deathF = p.death;

		getPeople(nu.rowidM);
		nu.lineM = p.line;
		nu.rowidM = p.rowid;
		nu.mother = p.name;
		nu.birthM = p.birth;
		nu.deathM = p.death;


		nu.printed = false;
		vS.push_back(nu);
		theApp.m_recordset->MoveNext();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::clearN(NONUNITED* n)
{
	n->birth.Empty();
	n->source.Empty();
	n->birthF.Empty();
	n->birthGF.Empty();
	n->birthGF2.Empty();
	n->birthGM.Empty();
	n->birthGM2.Empty();
	n->birthM.Empty();
	n->death.Empty();
	n->deathF.Empty();
	n->deathGF.Empty();
	n->deathGF2.Empty();
	n->deathGM.Empty();
	n->deathGM2.Empty();
	n->deathM.Empty();
	n->father.Empty();
	n->grandfather.Empty();
	n->grandfather2.Empty();
	n->grandmother.Empty();
	n->grandmother2.Empty();
	n->line.Empty();
	n->lineF.Empty();
	n->lineGF.Empty();
	n->lineGF2.Empty();
	n->lineGM.Empty();
	n->lineGM2.Empty();
	n->lineM.Empty();
	n->mother.Empty();
	n->name.Empty();
	n->printed = false;
	n->rowid.Empty();
	n->rowidF.Empty();
	n->rowidGF.Empty();
	n->rowidGF2.Empty();
	n->rowidGM.Empty();
	n->rowidGM2.Empty();
	n->rowidM.Empty();
	n->sorszam.Empty();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCompareEntries::getPeople( CString rowid )
{
	p.line.Empty();
	p.source.Empty();
	p.rowid.Empty();
	p.name.Empty();
	p.birth.Empty();
	p.death.Empty();
	p.father_id.Empty();
	p.mother_id.Empty();

//	if (rowid.IsEmpty() || rowid == L"0") return true;
	
	m_command.Format(L"SELECT linenumber, last_name, first_name, birth_date, death_date, father_id, mother_id, source FROM people WHERE rowid='%s'", rowid );
	if (!theApp.query1(m_command)) return false;

	if (!theApp.m_recordset1->RecordsCount()) return false;

	p.rowid = rowid;
	p.line = theApp.m_recordset1->GetFieldString(0);

	p.name.Format( L"%s %s",theApp.m_recordset1->GetFieldString(1), sepFirstName(theApp.m_recordset1->GetFieldString(2)) );
	p.name.Trim();

	p.birth = theApp.m_recordset1->GetFieldString(3);
	p.death = theApp.m_recordset1->GetFieldString(4);
	p.father_id = theApp.m_recordset1->GetFieldString(5);
	p.mother_id = theApp.m_recordset1->GetFieldString(6);
	p.source = theApp.m_recordset1->GetFieldString(7);
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::displayData()
{
	nItemS = 0;
	listPeople(&n1, 0);
	listPeople(&n2, 1);

	listSpouses();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::listPeople(NONUNITED * n, int nItem)
{
	m_ListCtrl1.InsertItem( nItem, n->sorszam);
	m_ListCtrl1.SetItemText(nItem, X, n->source); 
	m_ListCtrl1.SetItemText(nItem, L, n->line);

	m_col = 0;
	if (nItem == 1 && n->source == L"1")

		m_col += 1 << X;

	if( n->rowid != L"0")
		m_ListCtrl1.SetItemText( nItem, R, n->rowid);
	m_ListCtrl1.SetItemText( nItem, N, n->name);

	setItem( &m_ListCtrl1, nItem, B, n->birth, '*');
	setItem(&m_ListCtrl1, nItem, D, n->death, '+');

	m_ListCtrl1.SetItemText(nItem, LF, n->lineF);
	if (n->rowidF != L"0")
		m_ListCtrl1.SetItemText(nItem, RF, n->rowidF);
	setItem(&m_ListCtrl1, nItem, NF, n->father, ' ');
	setItem(&m_ListCtrl1, nItem, BF, n->birthF, '*');
	setItem(&m_ListCtrl1, nItem, DF, n->deathF, '+');

	m_ListCtrl1.SetItemText(nItem, LM, n->lineM);
	if (n->rowidM != L"0")
		m_ListCtrl1.SetItemText(nItem, RM, n->rowidM);
	setItem(&m_ListCtrl1, nItem, NM, n->mother, ' ');
	setItem(&m_ListCtrl1, nItem, BM, n->birthM, '*');
	setItem(&m_ListCtrl1, nItem, DM, n->deathM, '+');

	m_ListCtrl1.SetItemData( nItem, m_col);


	m_col = 0;
	m_ListCtrl2.InsertItem(nItem, n->sorszam);
	m_ListCtrl2.SetItemText(nItem, LF, n->lineGF);
	if (n->rowidGF != L"0")
		m_ListCtrl2.SetItemText(nItem, RF, n->rowidGF);
	setItem( &m_ListCtrl2,nItem, NF, n->grandfather, ' ');
	setItem(&m_ListCtrl2, nItem, BF, n->birthGF, '*');
	setItem(&m_ListCtrl2, nItem, DF, n->deathGF, '+');

	m_ListCtrl2.SetItemText(nItem, LM, n->lineGM);
	if (n->rowidGM != L"0")
		m_ListCtrl2.SetItemText(nItem, RM, n->rowidGM);
	setItem(&m_ListCtrl2, nItem, NM, n->grandmother, ' ');
	setItem(&m_ListCtrl2, nItem, BM, n->birthGM, '*');
	setItem(&m_ListCtrl2, nItem, DM, n->deathGM, '+');
	m_ListCtrl2.SetItemData(nItem, m_col);


	m_col = 0;
	m_ListCtrl3.InsertItem(nItem, n->sorszam);
	m_ListCtrl3.SetItemText(nItem, LF, n->lineGF2);
	if (n->rowidGF2 != L"0")
		m_ListCtrl3.SetItemText(nItem, RF, n->rowidGF2);
	setItem(&m_ListCtrl3, nItem, NF, n->grandfather2, ' ');
	setItem(&m_ListCtrl3, nItem, BF, n->birthGF2, '*');
	setItem(&m_ListCtrl3, nItem, DF, n->deathGF2, '+');

	m_ListCtrl3.SetItemText(nItem, LM, n->lineGM2);
	if (n->rowidGM2 != L"0")
		m_ListCtrl3.SetItemText(nItem, RM, n->rowidGM2);
	setItem(&m_ListCtrl3, nItem, NM, n->grandmother2, ' ');
	setItem(&m_ListCtrl3, nItem, BM, n->birthGM2, '*');
	setItem(&m_ListCtrl3, nItem, DM, n->deathGM2, '+');
	m_ListCtrl3.SetItemData(nItem, m_col);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::setItem(CListCtrlEx* lc, int nItem, int X, CString item, TCHAR kar)
{
	item.Trim();
	if (!item.IsEmpty())
	{
		if (kar != ' ') item.Format(L"%c%s", kar, (CString)item);
		if (nItem > 0 )
		{
			str = lc->GetItemText( nItem-1, X);
			if (!str.IsEmpty() && item != str)
				m_col = m_col | 1 << X;
		}
		lc->SetItemText( nItem, X, item );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::listSpouses()
{
	std::sort(vS.begin(), vS.end(), sortBySpouseX);
//	std::sort(vS.begin(), vS.end(), sortByXSpouse);
	int i;
	CString name1;
	CString name2;

	CString father1;
	CString father2;
	CString birthF1;
	CString birthF2;
	CString deathF1;
	CString deathF2;

	CString mother1;
	CString mother2;
	CString birthM1;
	CString birthM2;
	CString deathM1;
	CString deathM2;
	int sorszam;
	CString name;
	CString lastName;
	bool	check;


	for (i = 0; i < vS.size(); ++i)
	{
		m_col = 0;
		name2 = vS.at(i).name;
		if (i && name2 != name1)
		{
			m_ListCtrl4.InsertItem(nItemS, L"");
			++nItemS;
		}
		name1 = name2;
		sorszam = _wtoi(vS.at(i).sorszam);

		// házastárs

		m_ListCtrl4.InsertItem(nItemS, vS.at(i).sorszam);
		m_ListCtrl4.SetItemText(nItemS, X, vS.at(i).source);
		m_ListCtrl4.SetItemText(nItemS, L, vS.at(i).line);
		if (vS.at(i).rowid != L"0")
			m_ListCtrl4.SetItemText(nItemS, R, vS.at(i).rowid);
		name = vS.at(i).name;
		if (i && vS.at(i - 1).name == name)
			check = true;
		else
			check = false;
		m_ListCtrl4.SetItemText(nItemS, N, vS.at(i).name);
		str.Empty();


		if (check) 
			setItem(&m_ListCtrl4, i, B, vS.at(i).birth, '*');
		else if (!vS.at(i).birth.IsEmpty())
		{
			str.Format(L"*%s", vS.at(i).birth);
			m_ListCtrl4.SetItemText(nItemS, B, str);
		}
		str.Empty();
		
		if (check)
			setItem(&m_ListCtrl4, i, D, vS.at(i).death, '+');
		else if (!vS.at(i).death.IsEmpty())
		{
			str.Format(L"+%s", vS.at(i).death);
			m_ListCtrl4.SetItemText(nItemS, D, str);
		}
		str.Empty();

		// apa
		m_ListCtrl4.SetItemText(nItemS, LF, vS.at(i).lineF);
		if (vS.at(i).rowidF != L"0")
			m_ListCtrl4.SetItemText(nItemS, RF, vS.at(i).rowidF);

		father2 = vS.at(i).father;
		if (!father2.IsEmpty())
		{
			m_ListCtrl4.SetItemText(nItemS, NF, father2);
			if (!father1.IsEmpty() && father1 != father2 && sorszam != 1)
				m_col = m_col | 1 << NF;
		}
		father1 = father2;

		str.Empty();
		if (check)
			setItem(&m_ListCtrl4, i, BF, vS.at(i).birthF, '*');
		else if (!vS.at(i).birthF.IsEmpty())
		{
			str.Format(L"*%s", vS.at(i).birthF);
			m_ListCtrl4.SetItemText(nItemS, BF, str);
		}
		str.Empty();

		if (check)
			setItem(&m_ListCtrl4, i, DF, vS.at(i).deathF, '+');
		else if (!vS.at(i).deathF.IsEmpty())
		{
			str.Format(L"+%s", vS.at(i).deathF);
			m_ListCtrl4.SetItemText(nItemS, DF, str);
		}
		str.Empty();


		// anya
		m_ListCtrl4.SetItemText(nItemS, LM, vS.at(i).lineM);
		if (vS.at(i).rowidF != L"0")
			m_ListCtrl4.SetItemText(nItemS, RM, vS.at(i).rowidM);

		str.Empty();
		if (check)
			setItem(&m_ListCtrl4, i, NM, vS.at(i).mother, ' ');
		else
		{
			m_ListCtrl4.SetItemText(nItemS, NM, vS.at(i).mother);
		}
		str.Empty();


		str.Empty();
		if (check)
			setItem(&m_ListCtrl4, i, BM, vS.at(i).birthM, '*');
		else if (!vS.at(i).birthM.IsEmpty())
		{
			str.Format(L"*%s", vS.at(i).birthM);
			m_ListCtrl4.SetItemText(nItemS, BM, str);
		}
		str.Empty();

		if (check)
			setItem(&m_ListCtrl4, i, DM, vS.at(i).deathM, '+');
		else if( !vS.at(i).deathM.IsEmpty() )
		{

			str.Format(L"+%s", vS.at(i).deathM);
			m_ListCtrl4.SetItemText(nItemS, DM, str);
		}
		str.Empty();

		m_ListCtrl4.SetItemData(nItemS, m_col);
		++nItemS;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::emptyLine()
{
	m_ListCtrl4.InsertItem(nItemS, L" ");
	++nItemS;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	int	nCol = pNMItemActivate->iSubItem;

	CString name;
	CString line;
	if (nCol == N || nCol == NF || nCol == NM)
	{
		name = m_ListCtrl1.GetItemText(nItem, nCol);
		line = m_ListCtrl1.GetItemText(nItem, nCol - 2);
		theApp.editNotepad(theApp.m_htmlPathName, line);
	}
	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	int	nCol = pNMItemActivate->iSubItem;

	CString name;
	CString line;
	if (nCol == N || nCol == NF || nCol == NM)
	{
		name = m_ListCtrl2.GetItemText(nItem, nCol);
		line = m_ListCtrl2.GetItemText(nItem, nCol - 2);
		theApp.editNotepad(theApp.m_htmlPathName, line);
	}

	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::OnDblclkList3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	int	nCol = pNMItemActivate->iSubItem;

	CString name;
	CString line;
	if (nCol == N || nCol == NF || nCol == NM)
	{
		name = m_ListCtrl3.GetItemText(nItem, nCol);
		line = m_ListCtrl3.GetItemText(nItem, nCol - 2);
		theApp.editNotepad(theApp.m_htmlPathName, line);
	}

	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::OnDblclkList4(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;
	int	nCol = pNMItemActivate->iSubItem;

	CString name;
	CString line;
	if (nCol == N || nCol == NF || nCol == NM )
	{
		name = m_ListCtrl4.GetItemText(nItem, nCol);
		line = m_ListCtrl4.GetItemText(nItem, nCol-2);
		theApp.editNotepad(theApp.m_htmlPathName, line);
	}
	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::OnCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int nCol;
	int iData;
	UINT mask;

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
		if (nCol == N)
			pLVCD->clrText = LIGHTRED;
		else
		{
			mask = 1 << nCol;
			iData = m_ListCtrl1.GetItemData(nItem);
			if (iData & mask)
			{
				pLVCD->clrTextBk = RGB(255, 255, 0);
			}
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::OnCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int nCol;
	int iData;
	UINT mask;

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
		if (nCol == N)
			pLVCD->clrText = LIGHTRED;
		else
		{
			mask = 1 << nCol;
			iData = m_ListCtrl2.GetItemData(nItem);
			if (iData & mask)
			{
				pLVCD->clrTextBk = RGB(255, 255, 0);
			}
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::OnCustomdrawList3(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int nCol;
	int iData;
	UINT mask;

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
		if (nCol == N)
			pLVCD->clrText = LIGHTRED;
		else
		{
			mask = 1 << nCol;
			iData = m_ListCtrl3.GetItemData(nItem);
			if (iData & mask)
			{
				pLVCD->clrTextBk = RGB(255, 255, 0);
			}
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::OnCustomdrawList4(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int nCol;
	int iData;
	UINT mask;

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
		mask = 1 << nCol;
		iData = m_ListCtrl4.GetItemData(nItem);
		if (iData & mask)
		{
			pLVCD->clrTextBk = RGB(255, 255, 0);
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// D R O P D O W N   F U N C I Ó K  /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////û
LRESULT CCompareEntries::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup;

	if (Menu.LoadMenu(IDR_DROPDOWN_2ROKONOK))
	{
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareEntries::OnRokonok()
{
	int cnt = m_ListCtrl2.GetSelectedCount();
	if (cnt != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni");
		return;
	}
	POSITION	pos = m_ListCtrl2.GetFirstSelectedItemPosition();

	int nItem1 = m_ListCtrl2.GetNextSelectedItem(pos);
	int nItem2 = m_ListCtrl2.GetNextSelectedItem(pos);

	CCompareEntries dlg;
	dlg.rowid1 = m_ListCtrl2.GetItemText(nItem1, RF);
	dlg.rowid2 = m_ListCtrl2.GetItemText(nItem2, RF);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 létezõ bejegyzést jelölj meg!");
		return;
	}
	dlg.DoModal();
}
