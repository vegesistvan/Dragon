// CheckMotherIndex.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "CheckMotherIndex.h"
#include "afxdialogex.h"

// CCheckMotherIndex dialog

IMPLEMENT_DYNAMIC(CCheckMotherIndex, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckMotherIndex::CCheckMotherIndex(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckMotherIndex::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckMotherIndex::~CCheckMotherIndex()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckMotherIndex, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckMotherIndex::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckMotherIndex::OnHtmlNotepad)

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckMotherIndex::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT()

	int nItem = 0;
	CString rowid;
	CString lineNumber;
	CString lineNumberF;
	CString gaLine;
	CString cntS;
	CString name;
	CString father_id;
	int		mother_index;
	int		count;
	int		cnt = 0;

	CString info = L"\
Sorpárokat láthatunk az alábbi listában. Az elsõ sor a szülõ, a második a gyermek sora a ga-html fájlban.\
\r\n\
A gyermek annya-indexe és az apa feleségeinek száma között van ellentmondás.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );


	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( 0,	L"#",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 2,	L"név",		LVCFMT_LEFT,	200,-1,COL_TEXT);

	CProgressWnd wndP( NULL, L"Anya-indexek ellenõrzése..." );
	wndP.GoModal();

	m_command = L"SELECT rowid, lineNumber, mother_index, father_id, first_name, last_name FROM people ORDER BY lineNumber";
	if( !theApp.query( m_command ) )
	{
		OnCancel();
		return false;
	}

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	gafile.Open( theApp.m_htmlFileSpec, CFile::modeRead );

	
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	m_lineNumberPrev = 1;
	for( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		lineNumber		= theApp.m_recordset->GetFieldString( 1 );
		mother_index	= _wtoi( theApp.m_recordset->GetFieldString( 2 ) );
		father_id		= theApp.m_recordset->GetFieldString( 3 );

		m_command.Format( L"SELECT lineNumber FROM marriages WHERE spouse1_id='%s'", father_id );
		if( !theApp.query1( m_command ) ) 
		{
			OnCancel();
			return false;
		}
		count = theApp.m_recordset1->RecordsCount();		//  _wtoi( theApp.m_recordset1->GetFieldString( 0 ) );
		
		if( count && mother_index > count )
		{
			++cnt;
			cntS.Format( L"%d", cnt );
			lineNumberF	= theApp.m_recordset1->GetFieldString( 0 );

			str = getHtmlLine( lineNumberF );
			nItem = m_ListCtrl.InsertItem( nItem, cntS );
			m_ListCtrl.SetItemText( nItem, 1, lineNumberF);
			m_ListCtrl.SetItemText( nItem, 2, str );
			++nItem;

			str = getHtmlLine( lineNumber );
			nItem = m_ListCtrl.InsertItem( nItem, cntS );
			m_ListCtrl.SetItemText( nItem, 1, lineNumber);
			m_ListCtrl.SetItemText( nItem, 2, str );
			++nItem;

			nItem = m_ListCtrl.InsertItem( nItem, L"" );
			++nItem;
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
		m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
	wndP.DestroyWindow();

	gafile.Close();


	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckMotherIndex::getHtmlLine( CString lineNumberS )
{
	int lineNumber = _wtoi( lineNumberS );
	int cnt;

	cnt = lineNumber - m_lineNumberPrev;
	if( cnt < 0 )
		return cLine;

	for( int i = 0; i < cnt; ++i )
		gafile.ReadString( cLine );		// átugrik cnt sort
	gafile.ReadString( cLine );			// beolvassa a kért sort
	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );

	m_lineNumberPrev = lineNumber + 1;

	cLine = cleanHtmlLine( cLine );
	return cLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckMotherIndex::cleanHtmlLine( CString cLine )
{
	CString htmlLine = cLine;
	int		pos;
	TCHAR	gen;

	if( ( pos = cLine.ReverseFind('>') ) != -1 )
	{
		cLine = cLine.Mid( pos+1 );
		gen = cLine[0];
		if( ( pos = cLine.ReverseFind(';') ) != -1 )
		{
			cLine = cLine.Mid( pos+1 );
			htmlLine.Format( L"%c %s", gen, cLine ); 
		}
	}
	return htmlLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckMotherIndex:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	if(Menu.LoadMenu( IDR_DROPDOWN_HTML_EDIT ))
    {
		pPopup = Menu.GetSubMenu(0);

		if( m_ListCtrl.GetItemText( nItem, 1 ).IsEmpty() )
		{
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_NOTEPAD, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	1 ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	1 );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
