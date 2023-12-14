// CheckNames.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkNames.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "editHtmlLines.h"
#include "Table_firstnames.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckNames()
//{
//	CCheckNames dlg;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	L_CNT = 0,
//	L_ROWID,
	L_LINENUMBER,
	L_SOURCE,
	L_LASTNAME,
	L_FIRSTNAME,
	L_GAHTML,
};
IMPLEMENT_DYNAMIC(CCheckNames, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckNames::CCheckNames(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckNames::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckNames::~CCheckNames()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_ATTENTION, colorAttention);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckNames, CDialogEx)
	ON_COMMAND(ID_LIST, &CCheckNames::OnList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCheckNames::OnDblclkList)
	ON_COMMAND(ID_FIRSTNAMES, &CCheckNames::OnFirstnames)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckNames::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString info = L"\
Az 'S' oszlopban van megadva a problémás név 'rangja', ami alapján azonosíthatjuk a problémát.\
\r\n\
Ha valószínûsíthetõ, hogy a program azárt nem ismerte fel a keresztnevet, mert az nem szerepel \
a nyilvántartásában, akkor a 'Keresztnevek' funkcióval belenézhetünk a nyilvántartásba és \
és szükség esetén megadhatunk új neveket.\
\r\n\
Ha a problémát a ga.htm fájl sorának szintaktikai hibája vagy elgépelés okozta, akkor a \
hibás sort javíthatjuk. A bal egérgombbal kétszer kattintva a kiválasztott sorra, megjelenik a fájl \
a notepad ++ ban a kijelölt sorra mutatva, amiben módosításokat végezhetünk.\
\r\n\
A nevek téves azonosítását program hiba is okozhatja, amely esetben a programozót kell értesíteni.\
";
	SetWindowText(L"A beovasott ga.html azonm sorai, amelyekbõl valamely család- vagy keresztnevet \
nem sikerült meghatározni");
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	CString attention = L"Az alább felsorolt hibákat egyesítés elõtt mindenképpen ki kell javítani! Legjobb lenne most azonnal!";
	colorAttention.SetTextColor(LIGHTRED);
	GetDlgItem(IDC_ATTENTION)->SetWindowTextW(attention);


	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(FALSE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( L_CNT,			L"#",			LVCFMT_RIGHT,	 60, -1, COL_HIDDEN );
//	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60, -1, COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60, -1, COL_NUM );
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 40, -1, COL_NUM);
	m_ListCtrl.InsertColumn( L_LASTNAME,	L"családnév",	LVCFMT_LEFT,	150, -1, COL_TEXT);
	m_ListCtrl.InsertColumn( L_FIRSTNAME,	L"keresztnév",	LVCFMT_LEFT,     75, -1, COL_TEXT);
	m_ListCtrl.InsertColumn( L_GAHTML,		L"GA.html sor",	LVCFMT_LEFT,   2000, -1, COL_TEXT);

	m_command = L"SELECT rowid, linenumber, last_name, first_name, source FROM people WHERE last_name= '' OR first_name='' ORDER BY lineNumber";
	if( !theApp.query( m_command ) ) return false;

	int nItem = 0;
	CString rowid;
	CString lineNumber;
	CString gaLine;
	CString cntS;
	CString lastname;
	CString firstname;
	CString source;
	int z;

	CProgressWnd wndP( NULL, L"Meghatározhatatlan nevû bejegyzések beolvasása folyik..." );
	wndP.GoModal();
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid		= theApp.m_recordset->GetFieldString(0);
		lineNumber = theApp.m_recordset->GetFieldString(1);
		lastname	= theApp.m_recordset->GetFieldString(2);
		firstname	= theApp.m_recordset->GetFieldString(3);
		source		= theApp.m_recordset->GetFieldString(4);

		if (lineNumber == L"22")
			z = 1;
		gaLine = theApp.getHtmlLine(lineNumber);
		cntS.Format(L"%d", i + 1);

		nItem = m_ListCtrl.InsertItem( nItem, cntS );
		m_ListCtrl.SetItemText(nItem, L_LINENUMBER, lineNumber);
		m_ListCtrl.SetItemText(nItem, L_SOURCE, source);
		m_ListCtrl.SetItemText( nItem, L_LASTNAME, lastname);
		m_ListCtrl.SetItemText(nItem, L_FIRSTNAME, firstname);
		m_ListCtrl.SetItemText( nItem, L_GAHTML, gaLine );
		++nItem;
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
		m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
	wndP.DestroyWindow();
	ShowWindow(SW_MAXIMIZE);

	if( !m_ListCtrl.GetItemCount() )
	{
		OnCancel();
		AfxMessageBox( L"Minden ember család- és keresztnevét sikerült meghatározni!" );
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnList()
{
	CString	logFile(L"nameproblems"); 
	theApp.exportAll( logFile, L"Név problémák", &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	CString linenumber = m_ListCtrl.GetItemText(nItem, L_LINENUMBER);
	if (!linenumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, linenumber);
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnFirstnames()
{
	CTableFirstnames dlg;
	dlg.DoModal();
}
