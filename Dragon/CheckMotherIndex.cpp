// CheckMotherIndex.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckMotherIndex.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "Relations.h"
#include "html_Lines.h"

enum
{
	L_CNT = 0,
	L_ROWID,
	L_LINENUMBER,
	L_INDEX,
	L_LINE
};

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
	ON_COMMAND(ID_ROKONSAG, &CCheckMotherIndex::OnRokonsag)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckMotherIndex::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT()


	if( theApp.getUserVersion() )
	{
		AfxMessageBox( L"Csak összevonás elõtt van értelme ezt az ellenõrzést elvégezni!" );
		CDialog::OnCancel();
		return false;
	}

	int nItem = 0;
	CString rowid;
	CString name;
	CString lineNumber;
	CString lineNumberP;
	CString lineP;
	CString gaLine;
	CString father_id;
	CString mother_id;
	CString parent_id;
	CString parent;

	int		z;
	int		pos;
	int		sex_id;
	int		source;
	int		parent2Index;
	int		count;
	int		cnt = 0;

	CString info = L"\
Sorpárokat láthatunk az alábbi listában. Az elsõ sor a szülõ, a második a gyermek sora a ga-html fájlban.\
A gyermek annya-indexe és az apa feleségeinek száma között van ellentmondás.\r\n\n\
Csak összevonás elõtt van értelem ezt a listát elemezni, mert összevonás után a GA.htl-ben való sorok száma \
összekeveredik.\r\n\r\n\
Ha leányági leszármazottak gyeremekei is vannak a GA-html-ben, akkor az apa és az anya felcserélõdik. Erre jelenleg nincs felkészülve a program.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	CProgressWnd wndP( NULL, L"Szülõ-indexek ellenõrzése..." );
	wndP.GoModal();

#ifndef _DEBUG
	str.Format( L"GA-html fájl sorindexeinek elkészítése..." );
	wndP.SetText( str );
#endif

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( L_CNT,			L"#",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_INDEX,		L"X",		LVCFMT_RIGHT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINE,		L"név",		LVCFMT_LEFT,	200,-1,COL_TEXT);

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	gafile.Open( theApp.m_htmlFileSpec, CFile::modeRead );
	vPos.clear();
	vPos.push_back(0);
	while( 	gafile.ReadString( cLine ) )
		vPos.push_back( gafile.GetPosition() );

// gyerekek lekérdezése	
	m_command = L"SELECT rowid, lineNumber, parent2Index, father_id, mother_id, first_name, last_name FROM people WHERE source='1' ORDER BY lineNumber";
	if( !theApp.query( m_command ) )
	{
		OnCancel();
//		return false;
	}

#ifndef _DEBUG
	str.Format( L"Gyermek-szülõ rekordok vizsgálata..." );
	wndP.SetText( str );
#endif

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid			= theApp.m_recordset->GetFieldString( 0 );
		lineNumber		= theApp.m_recordset->GetFieldString( 1 );
		parent2Index	= _wtoi( theApp.m_recordset->GetFieldString( 2 ) );
		if( parent2Index == 0 ) goto cont;

		father_id		= theApp.m_recordset->GetFieldString( 3 );
		mother_id		= theApp.m_recordset->GetFieldString( 4 );

		if( mother_id == L"42691" )
			z = 1;
//		name.Format( L"%s %s", theApp.m_recordset->GetFieldString(6), theApp.m_recordset->GetFieldString(5) );
		// a szülõk html sora
		m_command.Format( L"SELECT lineNumber FROM marriages WHERE spouse1_id='%s' AND spouse2_id= '%s'", father_id, mother_id );  // az ember apjának házasságai
		if( !theApp.query1( m_command ) ) OnCancel();
		if( !theApp.m_recordset1->RecordsCount() ) goto cont;

		lineNumberP	= theApp.m_recordset1->GetFieldString( 0 );
		lineP = getHtmlLine( lineNumberP );
		
		// hány házassága van az apának?
		m_command.Format( L"SELECT lineNumber FROM marriages WHERE spouse1_id='%s'", father_id );  // az ember apjának házasságai
		if( !theApp.query1( m_command ) ) OnCancel();
		count =theApp.m_recordset1->RecordsCount();
		if( !count ) goto cont;

		parent_id = father_id;
		parent = getWord( lineP,2, &pos );
		if( (pos = parent.Find( '/' )) != -1 )
			parent = parent.Left( pos );
		parent.Remove( ',' );
		parent.TrimRight();
		sex_id = theApp.isFirstName( parent );
		if( sex_id == -1 ) goto cont;  // nem tudja milyen nemû

		if( sex_id == 2 ) // hány házastársa van az anyának
		{
			m_command.Format( L"SELECT count() FROM marriages WHERE spouse2_id='%s'", mother_id );  // az ember apjának házasságai
			if( !theApp.query1( m_command ) ) OnCancel();
			count = _wtoi( theApp.m_recordset1->GetFieldString( 0 ) );
			parent_id = mother_id;
			if( !count ) goto cont;
		}

		if( parent2Index > count  )
		{
			// szülõ
			++cnt;
			str.Format( L"%d", cnt );
			nItem = m_ListCtrl.InsertItem( nItem, str );
		
			str.Format( L"%d", count );
			m_ListCtrl.SetItemText( nItem, L_INDEX, str );
			m_ListCtrl.SetItemText( nItem, L_ROWID, parent_id );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumberP);
			m_ListCtrl.SetItemText( nItem, L_LINE, lineP );
			++nItem;

			// gyermek 
			str.Format( L"%d-%d", cnt, parent2Index );
			nItem = m_ListCtrl.InsertItem( nItem, str );

			str.Format( L"%d", parent2Index );
			m_ListCtrl.SetItemText( nItem, L_INDEX, str );
			m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );

			str = getHtmlLine( lineNumber );
			m_ListCtrl.SetItemText( nItem, L_LINE, str );
			++nItem;

			nItem = m_ListCtrl.InsertItem( nItem, L"" );
			++nItem;
		}
cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
		m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
	wndP.DestroyWindow();
	vPos.clear();
	gafile.Close();

	if( !m_ListCtrl.GetItemCount() )
	{
		AfxMessageBox( L"Nincs hiba az anya-indexekkel!" );
		CDialog::OnCancel();
	}
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
CString CCheckMotherIndex::getHtmlLine( CString lineNumber )
{
	int ln = _wtoi( lineNumber );

	gafile.Seek( vPos.at( ln - 1 ), SEEK_SET );
	gafile.ReadString( cLine );		// átugrik cnt sort
	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
	cLine = cleanHtmlLine( cLine );
	return cLine;
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

		if( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ).IsEmpty() )
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
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnRokonsag()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 2 );
	CRelations dlg;

	dlg.m_rowid = rowid;
	dlg.DoModal();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CCheckMotherIndex::OnHtmlShows()
{
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int			nItem;
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");

	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		vLines.push_back( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) );
		if( name.Compare( m_ListCtrl.GetItemText( nItem, L_LINE ) ) )
		{
			name = m_ListCtrl.GetItemText( nItem, L_LINE );
			++cnt;
		}
	

	}

	CHtmlLines dlg;

	if( cnt == 1 )
		dlg.child = name;
	else
		dlg.child = L"";

	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
}
*/