// CCheckParents.cpp : implementation file
//

#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "checkParents.h"
#include "utilities.h"
#include "ProgressWnd.h"

enum
{
	L_ROWID = 0,
	L_LINENUMBER,
	L_POSTERIOR,
	L_LINE,
};

IMPLEMENT_DYNAMIC(CCheckParents, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////
CCheckParents::CCheckParents(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHECK_PARENTS, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////
CCheckParents::~CCheckParents()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParents::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckParents, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCheckParents::OnDblclkList)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckParents::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CString caption = L"Házastársak gyanús szülei - Egy sorra dupla kattintás a bal egérgombbal:ga.html file a notepad++-ben a kérdéses sorra mutatva";
	SetWindowText(caption);

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrl.InsertColumn(L_ROWID, L"rowid", LVCFMT_RIGHT, 80, -1, COL_HIDDEN);
	m_ListCtrl.InsertColumn(L_LINENUMBER, L"linenumber", LVCFMT_RIGHT, 80, -1, COL_NUM);
	m_ListCtrl.InsertColumn(L_POSTERIOR, L"szülõk-további házastársak", LVCFMT_LEFT, 280, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(L_LINE, L"GA.html sor", LVCFMT_LEFT, 1000, -1, COL_TEXT);
	fillTable();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckParents::fillTable()
{
	int nItem = 0;
	int pos;
	CString linenumber;
	CString posterior;
	CString line;
	m_command.Format( L"SELECT rowid, linenumber, posterior FROM people WHERE posterior LIKE '(%c'", '%');
	if (!theApp.query(m_command)) return false;

	if (!theApp.m_recordset->RecordsCount())
	{
		OnCancel();
	}


	theApp.m_inputCode = GetInputCode(theApp.m_htmlPathName);
	if (!notepad.Open(theApp.m_htmlPathName, CFile::modeRead))
		return false;

	CProgressWnd wndP(NULL, L"Házastársak gyanús szülei... ");
	wndP.GoModal();
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() - 1);
	wndP.SetPos(0);

	for (int i = 0; i < theApp.m_recordset->RecordsCount(); ++i)
	{
		posterior = theApp.m_recordset->GetFieldString(L_POSTERIOR);
		if (posterior.Find(L"Nep") != -1) continue;
		if (posterior.Find(L"Quin") != -1) continue;
		if (posterior.Find(L"Ker") != -1) continue;


		if( (posterior.Find('-') != -1) || (posterior.Find( L"f.") != -1 ) )
		{
			nItem = m_ListCtrl.InsertItem(nItem, theApp.m_recordset->GetFieldString(L_ROWID));

			linenumber = theApp.m_recordset->GetFieldString(L_LINENUMBER);
			m_ListCtrl.SetItemText(nItem, L_LINENUMBER, linenumber );
			m_ListCtrl.SetItemText(nItem, L_POSTERIOR, theApp.m_recordset->GetFieldString(L_POSTERIOR));
			line = getHtmlLine(linenumber);
			m_ListCtrl.SetItemText(nItem, L_LINE, line);
		}
		theApp.m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	notepad.Close();
	if (!m_ListCtrl.GetItemCount() )
	{
		OnCancel();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckParents::getHtmlLine(CString lineNumber)
{
	if (lineNumber.IsEmpty()) return L"";
	

	CString cLine;
	int ln = _wtoi(lineNumber);

	for (int i = 0; i < ln - 2; ++i) notepad.ReadString(cLine);  // elõzõ sor elé megy
	notepad.ReadString(cLine);											// elõzõ sor

	notepad.ReadString(cLine);											// kérdéses sor
	cLine.Trim();
	if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi(cLine);

	cLine = cleanHtmlLine(cLine);
	notepad.SeekToBegin();
	return cLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckParents::cleanHtmlLine(CString cLine)
{
	CString htmlLine = cLine;
	int		pos;
	TCHAR	gen;

	if ((pos = cLine.ReverseFind('>')) != -1)
	{
		cLine = cLine.Mid(pos + 1);
		gen = cLine[0];
		if ((pos = cLine.ReverseFind(';')) != -1)
		{
			cLine = cLine.Mid(pos + 1);
			htmlLine.Format(L"%c %s", gen, cLine);
		}
	}
	return htmlLine;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParents::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int nItem = pNMItemActivate->iItem;
	CString linenumber = m_ListCtrl.GetItemText(nItem, L_LINENUMBER);
	theApp.editNotepad(theApp.m_htmlPathName, linenumber);

	*pResult = 0;
}
