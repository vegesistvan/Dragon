// Brackets.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "Brackets.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnBracketsRound()
//{
//
//	CString htmlPathName = theApp.m_htmlPathName;
//	if (htmlPathName.IsEmpty())
//	{
//		theApp.selectHtml();
//		htmlPathName = theApp.m_htmlPathName;
//		if (htmlPathName.IsEmpty()) return;
//	}
//
//	CBrackets dlg;
//	dlg.m_bracketCode = ROUND;
//	dlg.m_htmlPathName = htmlPathName;
//	dlg.DoModal();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnBracketsSquare()
//{
//	CString htmlPathName = theApp.m_htmlPathName;
//	if (htmlPathName.IsEmpty())
//	{
//		theApp.selectHtml();
//		htmlPathName = theApp.m_htmlPathName;
//		if (htmlPathName.IsEmpty()) return;
//	}
//	CBrackets dlg;
//	dlg.m_bracketCode = SQUARE;
//	dlg.m_htmlPathName = htmlPathName;
//	dlg.DoModal();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnBracketsBraces()
//{
//	CString htmlPathName = theApp.m_htmlPathName;
//	if (htmlPathName.IsEmpty())
//	{
//		theApp.selectHtml();
//		htmlPathName = theApp.m_htmlPathName;
//		if (htmlPathName.IsEmpty()) return;
//	}
//
//	CBrackets dlg;
//	dlg.m_bracketCode = BRACES;
//	dlg.m_htmlPathName = htmlPathName;
//	dlg.DoModal();
//}


// CBrackets dialog

IMPLEMENT_DYNAMIC(CBrackets, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CBrackets::CBrackets(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BRACKETS, pParent)
{
	m_caption2 = L"Bal egérgomb dupla kattintás: notepd++";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CBrackets::~CBrackets()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_STATIC_KERES, colorKeres);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CBrackets, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CBrackets::OnDblclkList)
	ON_COMMAND(ID_UNFILTERED, &CBrackets::OnUnfiltered)
	ON_COMMAND(ID_WRONG_BRACKETS, &CBrackets::OnWrongBrackets)
	ON_COMMAND(ID_BRACKETS_FAMILY, &CBrackets::OnBracketsFamily)
	ON_COMMAND(ID_BRACKETS_TABLE, &CBrackets::OnBracketsTable)
	ON_COMMAND(ID_BRACKETS_GOOD, &CBrackets::OnBracketsGood)
	ON_COMMAND(ID_BRACKETS_TORZS, &CBrackets::OnBracketsTorzs)
	ON_COMMAND(ID_BRACKETS_CSALAD, &CBrackets::OnBracketsCsalad)
	ON_COMMAND(ID_BRACKETS_AG, &CBrackets::OnBracketsAg)
	ON_COMMAND(ID_BRACKETS_DESC, &CBrackets::OnBracketsDesc)
	ON_STN_CLICKED(IDC_STATIC_KERES, &CBrackets::OnClickedStaticKeres)

	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CBrackets::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	switch (m_bracketCode)
	{
	case ROUND:
		m_bracket1 = '(';
		m_bracket2 = ')';
		m_brackets = L"kerek";
		m_Brackets = L"Kerek";
		break;
	case SQUARE:
		m_bracket1 = '[';
		m_bracket2 = ']'; 
		m_brackets = L"szögletes";
		m_Brackets = L"Szögletes";
		break;
	case BRACES:
		m_bracket1 = '{';
		m_bracket2 = '}';
		m_brackets = L"kapcsos";
		m_Brackets = L"Kapcsos";
		break;
	}

	theApp.m_inputCode = GetInputCode(m_htmlPathName);
	file.Open(m_htmlPathName, CFile::modeRead);
	m_fileLength = (int)file.GetLength();
	

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES);
	m_ListCtrl.SortByHeaderClick(TRUE);

	m_ListCtrl.InsertColumn(ERRORX, L"error", LVCFMT_RIGHT, 60, -1, COL_HIDDEN);
	m_ListCtrl.InsertColumn(LINENUMBER, L"line#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrl.InsertColumn(LINE, L"ga.htm sor", LVCFMT_LEFT, 2000, -1, COL_TEXT);


	fillVector();
	file.Close();
	if (!vBrackets.size())
	{
		str.Format(L"%s zárójelek nincsenek a fájlban", m_brackets );
		AfxMessageBox(str);
		OnCancel();
	}
	m_orderix = 1;
	OnUnfiltered();
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::fillVector()
{
	CString cLine;
	CString percent;
	BRACKETS br;



	CProgressWnd wndP(NULL, L"Zárójelek beolvasása....");
	wndP.GoModal();
	wndP.SetRange(0, m_fileLength);
	wndP.SetPos(0);
	wndP.SetStep(1);

	m_ListCtrl.DeleteAllItems();

	int pos;
	int nItem = 0;

	m_linenumber = 0;
	vBrackets.clear();
	int start;
	while (file.ReadString(cLine))
	{
		if (cLine.IsEmpty()) continue;
		cLine.Trim();


		if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi(cLine);
		++m_linenumber;
		br.linenumber = m_linenumber;
		percent.Empty();
		if (cLine.Left(3) == L"%%%")
		{
			percent = L"%%%";
			cLine = cLine.Mid(4);
		}
		else if (cLine.Left(2) == L"%%")
		{
			percent = L"%%";
			cLine = cLine.Mid(3);
		}
		cLine = cleanHtmlTags(cLine);
		if (findBrackets(cLine))
		{
			str.Format(L"%s %s", percent, m_line);
			str.TrimLeft();
			br.line = str;
			br.error = m_error;
			push(br);
		}
		pos = (int)file.GetPosition();
		wndP.SetPos( pos );
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::push( BRACKETS br )
{
	CString str;

	str.Format(L"%d", br.error);
	pushStr(str);

	str.Format(L"%d", br.linenumber);
	pushStr(str);

	pushStr(br.line);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::pushStr(CString str)
{
	const TCHAR* pC = (LPCTSTR)str;
	int tLen = str.GetLength() + 1;
	LPTSTR sT = new TCHAR[tLen];
	_tcscpy_s(sT, tLen, str.GetBuffer());
	vBrackets.push_back(sT);
	str.ReleaseBuffer();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CBrackets::findBrackets( CString cLine )
{
	int i;
	int jobb;
	int bal;
	int tol = 0;
	int pos;
	int db1 = 0;
	int db2 = 0;
	CString line = cLine;;


	m_error = 0;
	m_line = cLine;
	while((pos = line.ReverseFind(m_bracket1)) != -1 )  // megszámolja a végzárójelek számát
	{
		++db1;
		line = line.Left(pos);
		bal = pos;										// a bal-ban lesz az elsõ végzárójel
	}
	while((pos = cLine.Find(m_bracket2, tol)) != -1)	// megszámolja a kezdõ zárójelek számát
	{
		++db2;
		tol = pos + 1;
		jobb = pos;										// a jobb-ben lesz az utolsó kezdõ zárójel
	}

	if (!db1 && !db2) return false;  // nincs kezdõ és vég végzárójel sem, tzehát nem listázzuk
	if (db1 != db2)					// a kezdõ- és végzárójelek száma nem egyforma
	{
		m_error = 1;
	}
//	else
//	{
//		m_line = cLine.Mid(bal, jobb - bal + 1);	// csak a zárójelet listázza
//	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// < ol > <li>L & diams; János * 1683 + 1683
CString CBrackets::cleanHtmlTags(CString cLine)
{
	CString clean = cLine;
	CString str1;
	CString str2;
	int	pos;
	int pos1 = 0;
	int pos2 = 0;

	if ((pos = clean.Find(';')) != -1)   // az elejérõl levesz mident a ';' karakterig
		clean = clean.Mid(pos+1);
	
// <xxx> -ek kiszedése a sor belsejébõl és végérõl
	while ((pos = clean.Find('<')) != -1)
	{
		str1 = clean.Left(pos);				// a < elõtti karakterek
		if ((pos = clean.Find('>')) != -1)
		{
			str2 = clean.Mid(pos+1);		// a > utáni karakterek
			clean = str1 + str2;
		}
		else
		{
			m_error = 2;
			return cLine;
		}
	}
	

//		if (pos != clean.GetLength())
//			cLine = dropUntil(cLine, ';');

//	str.Empty();
//	if (!cLine.IsEmpty())
//		if (cLine.GetAt(0) != '%')
//			str.Format(L"%s %s", tag.Left(1), cLine);
//		else
	return clean;
}////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::fillTable()
{
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.AttachDataset(&vB);
	str.Format(L"%-100s%s", m_caption, m_caption2);
	SetWindowText(str);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString linenumber = m_ListCtrl.GetItemText(nItem, LINENUMBER);
	if (!linenumber.IsEmpty())
		theApp.editNotepad(m_htmlPathName, linenumber);
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnUnfiltered()
{
	vB.clear();
	for (int i = 0; i < vBrackets.size(); ++i)
	{
		vB.push_back(vBrackets.at(i));
	}
	m_caption.Format( L"Az összes %s zárójelet tartalmazó sor", m_brackets );
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnBracketsGood()
{
	vB.clear();
	for (int i = 0; i < vBrackets.size(); i += RECORDSIZE)
	{
		if (!_wtoi(vBrackets.at(i + ERRORX)))
		{
			for (int j = 0; j < RECORDSIZE; ++j)
				vB.push_back(vBrackets.at(i+j));
		}
	}
	m_caption.Format(L"Szabályos %s zárójeleket tartalmazó sorok", m_brackets);
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnWrongBrackets()
{
	vB.clear();
	for (int i = 0; i < vBrackets.size(); i += RECORDSIZE )
	{
		if (_wtoi(vBrackets.at(i + ERRORX )) )
		{

			for (int j = 0; j < RECORDSIZE; ++j)
				vB.push_back(vBrackets.at(i+j));
		}
	}
	m_caption.Format( L"Szabálytalan %s zárójeleket tartalmazó sorok", m_brackets);
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnBracketsFamily()
{
	vB.clear();
	for (int i = 0; i < vBrackets.size(); i += RECORDSIZE)
	{
		str = vBrackets.at(i + LINE);
		if ( getFirstWord(str) == L"\%\%\%")
		{
			for (int j = 0; j < RECORDSIZE; ++j)
				vB.push_back(vBrackets.at(i + j));
		}
	}
	m_caption.Format(L"%s zárójeleket tartalmazó család tábla fejlécek", m_Brackets);
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnBracketsTable()
{
	vB.clear();
	for (int i = 0; i < vBrackets.size(); i += RECORDSIZE)
	{
		str = vBrackets.at(i + LINE);
		if (getFirstWord(str) == L"\%\%")
		{
			for (int j = 0; j < RECORDSIZE; ++j)
				vB.push_back(vBrackets.at(i + j));
		}
	}
	m_caption.Format(L"%s zárójeleket tartalmazó folytatás tábla fejlécek", m_Brackets);
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnBracketsDesc()
{
	vB.clear();
	for (int i = 0; i < vBrackets.size(); i += RECORDSIZE)
	{
		str = vBrackets.at(i + LINE);
		if ( str.GetAt(0) != '\%' )
		{
			for (int j = 0; j < RECORDSIZE; ++j)
				vB.push_back(vBrackets.at(i + j));
		}
	}
	m_caption.Format( L"%s zárójeleket tartalmazó leszármazotti sorok", m_Brackets );
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnBracketsTorzs()
{
	vB.clear();

	for (int i = 0; i < vBrackets.size(); i += RECORDSIZE)
	{
		str = vBrackets.at(i + LINE);
		if ( str.Find( L"[törzs:" ) != -1 )
		{
			for (int j = 0; j < RECORDSIZE; ++j)
				vB.push_back(vBrackets.at(i + j));
		}
	}
	m_caption = L"[törzs: X Y]-et tartalmazó sorok";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnBracketsCsalad()
{
	vB.clear();

	for (int i = 0; i < vBrackets.size(); i += RECORDSIZE)
	{
		str = vBrackets.at(i + LINE);
		if (str.Find(L"család]") != -1)
		{
			for (int j = 0; j < RECORDSIZE; ++j)
				vB.push_back(vBrackets.at(i + j));
		}
	}
	m_caption = L"[X Y család] -ot tartalmazó sorok";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnBracketsAg()
{
	vB.clear();

	for (int i = 0; i < vBrackets.size(); i += RECORDSIZE)
	{
		str = vBrackets.at(i + LINE);
		if (str.Find(L"ág]") != -1)
		{
			for (int j = 0; j < RECORDSIZE; ++j)
				vB.push_back(vBrackets.at(i + j));
		}
	}
	m_caption = L"'X ág]'-ot tartalmazó sorok";
	fillTable();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CBrackets::PreTranslateMessage(MSG* pMsg)
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::OnClickedStaticKeres()
{
	keress(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrackets::keress(int start)
{
	CString	search;
	GetDlgItem(IDC_SEARCH)->GetWindowText(search);
	theApp.keress(search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CBrackets::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CBrackets::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
