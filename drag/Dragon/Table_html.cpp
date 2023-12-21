// Table_html.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Table_html.h"
#include "afxdialogex.h"
#include "GetString.h"
#include "ProgressWnd.h"
#include "utilities.h"

enum
{
	M_ID=0,
	M_TABLA_ID,
	M_ROW
};


// CTableHtml dialog
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTableHtml, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableHtml::CTableHtml(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableHtml::IDD, pParent)
{
	m_pParent = pParent;
	m_fileSpec = theApp.m_htmlPathName;
	m_fileName = theApp.m_htmlFileName;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableHtml::~CTableHtml()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTHTML, m_ListCtrl);
	DDX_Control(pDX, IDC_CAPTION, colorCaption);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableHtml, CDialogEx)
	ON_COMMAND(ID_FILTER_UNFILTER, &CTableHtml::OnFilterUnfilter)
	ON_COMMAND(ID_OPEN_HTML, &CTableHtml::OnOpenHtml)
	ON_COMMAND(ID_FILTER_SUBSTRING, &CTableHtml::OnFilterSubstring)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CTableHtml::OnClickedKeress)
	ON_STN_CLICKED(IDC_NEXT, &CTableHtml::OnClickedNext)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTHTML, &CTableHtml::OnDblclkListhtml)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)


END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableHtml::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

//	colorCaption.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );
	colorKeres.SetTextColor( theApp.m_colorClick );


	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( M_ID,			L"line#",	LVCFMT_RIGHT,	  40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( M_TABLA_ID,	L"table#",	LVCFMT_LEFT,	  40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( M_ROW,			L"line",	LVCFMT_LEFT,	1300,-1,COL_TEXT);

	title();

	m_substring.Empty();
	fillTable();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::title()
{
	str.Format( L"A %s fájl sorainak listája", m_fileName );
	SetWindowTextW( str );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::fillTable()
{

	CString cLine;
//	int nItem = 0;
	int n = 0;
	int pos;
	CString utonev;
	int lineNumber = 0;
	int tableNumber = 0;

//	BOOL ownerdata = TRUE;

	CString item1;
	CString item2;
	CString item3;

	if( m_substring.IsEmpty() )
		str = L"Szûretlen tábla";
	else
		str.Format( L"'%s' substrinegt tartalmazó sorok", m_substring );
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );

	CString generationFirst;
	CStdioFile file( m_fileSpec, CFile::modeRead); 
	while(file.ReadString(cLine)) ++n;

	CProgressWnd wndProgress(NULL,L"A html fájl beolvasása ..." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0,n);
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	v_table_html.clear();
	m_familyName.Empty();
	m_ListCtrl.DeleteAllItems();
	file.SeekToBegin();
	while(file.ReadString(cLine)) 
	{
		item1.Empty();
		item2.Empty();
		item3.Empty();
		if( cLine.GetAt(0) != '%' || cLine.Left(3) == L"%%%" )
		{
			cLine = cleanHtmlLine( cLine );
		}
		if( cLine.Left(2) == L"%%" )	++tableNumber;
		++lineNumber;


		if( !m_substring.IsEmpty() )						// ha szûrés van substring-re
		{
			if( ( pos = cLine.Find( m_substring ) ) == -1 )
				continue;
		}
		

		item1.Format( L"%d", lineNumber );
		item2.Format( L"%d", tableNumber ); 
		item3 = cLine;
		fillRow( item1, item2, item3 );
		


		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.SetItemCountEx( v_table_html.size()/3);
	m_ListCtrl.AttachDataset( &v_table_html );
	m_ListCtrl.ReSort();

	wndProgress.DestroyWindow();
	file.Close();
	m_orderix = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::fillRow( CString item1, CString item2, CString item3 )
{
	int tLen;
	LPTSTR sT;
	CStringW sw;

	tLen=item1.GetLength()+1;
	sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,item1.GetBuffer());
	v_table_html.push_back( sT );

	tLen=item2.GetLength()+1;
	sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,item2.GetBuffer());
	v_table_html.push_back( sT );

	tLen=item3.GetLength()+1;
	sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,item3.GetBuffer());
	v_table_html.push_back( sT );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::OnFilterUnfilter()
{
	m_substring.Empty();
	fillTable();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::OnFilterSubstring()
{
	CGetString dlg;

	dlg.m_caption = L"Add meg a stringet, amire szûrni akarsz!";
	if( dlg.DoModal() == IDCANCEL ) return;
	m_substring = dlg.m_string;

	fillTable();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::OnOpenHtml()
{
	theApp.selectHtml();
	m_substring.Empty();
	title();
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableHtml::isAncestor()
{
	BOOL GEN( FALSE );
//	BOOL ANCESTOR( TRUE );		// alapállapotban van õs a fájlban
//
	CString cLine;
	CString generationFirst;
	CStdioFile file( m_fileSpec, CFile::modeRead); 
	
	while(file.ReadString(cLine))		// nincs õs, ha van az elsõvel azonos generáció
	{
		cLine = cleanCline( cLine );
		str = cLine.Left(1);
		if( !cLine.IsEmpty() )
		{
			if( !GEN )
			{
				generationFirst = str;
				GEN = TRUE;
			}
			else
			{
				if( str == generationFirst ) return FALSE;
			}
		}
	}
	file.Close();
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::OnDblclkListhtml(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	if( nItem < 0 )
	{
		AfxMessageBox(L"Nincs kijelölve semmi!"); 
		return;
	}
	

	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableHtml::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor= (CELLCOLOR*) lParam;//a COLUMNCOLOR struktura deklaralva van a ListCtrlEx.h -ban.

	if(m_orderix==(int)wParam)
	{
		cColor->bg	= RGB(250,250,250);
		cColor->tx	= RGB(250,0,0);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableHtml::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableHtml::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::OnClickedKeress()
{
	keress( 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress( nItem + 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableHtml::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////