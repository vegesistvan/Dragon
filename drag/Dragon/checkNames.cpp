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
Az 'S' oszlopban van megadva a probl�m�s n�v 'rangja', ami alapj�n azonos�thatjuk a probl�m�t.\
\r\n\
Ha val�sz�n�s�thet�, hogy a program az�rt nem ismerte fel a keresztnevet, mert az nem szerepel \
a nyilv�ntart�s�ban, akkor a 'Keresztnevek' funkci�val belen�zhet�nk a nyilv�ntart�sba �s \
�s sz�ks�g eset�n megadhatunk �j neveket.\
\r\n\
Ha a probl�m�t a ga.htm f�jl sor�nak szintaktikai hib�ja vagy elg�pel�s okozta, akkor a \
hib�s sort jav�thatjuk. A bal eg�rgombbal k�tszer kattintva a kiv�lasztott sorra, megjelenik a f�jl \
a notepad ++ ban a kijel�lt sorra mutatva, amiben m�dos�t�sokat v�gezhet�nk.\
\r\n\
A nevek t�ves azonos�t�s�t program hiba is okozhatja, amely esetben a programoz�t kell �rtes�teni.\
";
	SetWindowText(L"A beovasott ga.html azonm sorai, amelyekb�l valamely csal�d- vagy keresztnevet \
nem siker�lt meghat�rozni");
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	CString attention = L"Az al�bb felsorolt hib�kat egyes�t�s el�tt mindenk�ppen ki kell jav�tani! Legjobb lenne most azonnal!";
	colorAttention.SetTextColor(LIGHTRED);
	GetDlgItem(IDC_ATTENTION)->SetWindowTextW(attention);


	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(FALSE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( L_CNT,			L"#",			LVCFMT_RIGHT,	 60, -1, COL_HIDDEN );
//	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60, -1, COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60, -1, COL_NUM );
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 40, -1, COL_NUM);
	m_ListCtrl.InsertColumn( L_LASTNAME,	L"csal�dn�v",	LVCFMT_LEFT,	150, -1, COL_TEXT);
	m_ListCtrl.InsertColumn( L_FIRSTNAME,	L"keresztn�v",	LVCFMT_LEFT,     75, -1, COL_TEXT);
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

	CProgressWnd wndP( NULL, L"Meghat�rozhatatlan nev� bejegyz�sek beolvas�sa folyik..." );
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
		AfxMessageBox( L"Minden ember csal�d- �s keresztnev�t siker�lt meghat�rozni!" );
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckNames::OnList()
{
	CString	logFile(L"nameproblems"); 
	theApp.exportAll( logFile, L"N�v probl�m�k", &m_ListCtrl );
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
