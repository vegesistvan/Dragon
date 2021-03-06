// ContractedPeople.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "ContractedPeople.h"
#include "ContractPeople.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "ProgressWnd.h"
#include "ContractInfo.h"
#include "html_EditLines.h"
#include "Relatives.h"
#include "GetString.h"
//#include "FilterLoop.h"

// CContractedPeople dialog
// CListCtrl t�bl�zat oszlopai
enum
{
	L_CNT = 0,
	L_LOOP, 
	L_GROUP,
	L_MATCH,
	L_GROUP2,
	L_STATUS,
	L_RGBCOLOR,
	L_LINENUMBER,
	L_UNITED,
	L_GENERATION,
	L_SOURCE,
	L_ROWID,
	L_NAME,
	L_ROWIDF,
	L_FATHER,
	L_ROWIDM,
	L_MOTHER,
	L_ROWIDS,
	L_SPOUSES,
	L_LINEF,
	L_COLUMNSCOUNT,
};
IMPLEMENT_DYNAMIC(CContractedPeople, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractedPeople::CContractedPeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CContractedPeople::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractedPeople::~CContractedPeople()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeress);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContractedPeople, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CContractedPeople::OnClickedKeress)
	ON_STN_CLICKED(IDC_NEXT, &CContractedPeople::OnClickedNext)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CContractedPeople::OnCustomdrawList)
	ON_COMMAND(ID_INFO, &CContractedPeople::OnInfo)
	ON_COMMAND(ID_FILTER_ALL, &CContractedPeople::OnFilterAll)
	ON_COMMAND(ID_FILTER_1, &CContractedPeople::OnFilter1)
	ON_COMMAND(ID_FILTER_2, &CContractedPeople::OnFilter2)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CContractedPeople::OnDblclkList)
	ON_COMMAND(ID_INPUT_UNITED, &CContractedPeople::OnInputUnited)
	ON_COMMAND(ID_INPUT_DIFFERENT, &CContractedPeople::OnInputDifferent)

/*
	ON_COMMAND(ID_HTML_1_D, &CContractedPeople::OnHtml1D)
	ON_COMMAND(ID_HTML_1_U, &CContractedPeople::OnHtml1U)
	ON_COMMAND(ID_HTML_2_D, &CContractedPeople::OnHtml2D)
	ON_COMMAND(ID_HTML_2_U, &CContractedPeople::OnHtml2U)
*/
	// IDR_DROPDOWN_HTML funkci�k	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CContractedPeople::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CContractedPeople::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CContractedPeople::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CContractedPeople::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_HTML_CHILDREN, &CContractedPeople::OnHtmlChildren)
	ON_COMMAND(ID_DB_EDIT, &CContractedPeople::OnDbEdit)
	ON_COMMAND(ID_3GENERATIONS, &CContractedPeople::On3Generations )


	ON_COMMAND(ID_FILTER_3, &CContractedPeople::OnFilter3)
	ON_COMMAND(ID_FILTER_4, &CContractedPeople::OnFilter4)
	ON_COMMAND(ID_FILTER_BYNAME, &CContractedPeople::OnFilterByname)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractedPeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();

	menu.LoadMenu( IDR_CONTRACTED_PEOPLE );
	SetMenu(&menu);
	
	colorKeress.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );

	m_azonos = theApp.getUserVersion() && 0XFF;

	createColumns();
	OnInputUnited();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnInputDifferent()
{
	inputFile( DIFFERENT_FILE );

	UNITED = false;
	menu.EnableMenuItem( ID_INPUT_UNITED, MF_BYCOMMAND | MF_ENABLED);
	menu.EnableMenuItem( ID_INPUT_DIFFERENT, MF_BYCOMMAND | MF_GRAYED);

	str.Format( L"Azonos nev� emberek bejegyz�sei, akik nem vonhat�ak �ssze (�sszevon�shoz el��rt adat-azonoss�gok sz�ma: %d )", m_azonos );
	SetWindowTextW( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnInputUnited()
{
	inputFile( UNITED_FILE );

	UNITED = true;
	menu.EnableMenuItem( ID_INPUT_UNITED, MF_BYCOMMAND | MF_GRAYED);
	menu.EnableMenuItem( ID_INPUT_DIFFERENT, MF_BYCOMMAND | MF_ENABLED);

	str.Format( L"Azonos nev� emberek bejegyz�sei, amik r�szben �sszevon�sra ker�ltek (�sszevon�shoz el��rt adat-azonoss�gok sz�ma: %d)", m_azonos );
	SetWindowTextW( str );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::inputFile( int type )
{
	CString filespec;

	m_command.Format( L"SELECT filespec FROM filespec WHERE type=%d", type );
	if( !theApp.query( m_command ) );
	filespec = theApp.m_recordset->GetFieldString( 0 );
	if( filespec.IsEmpty() || _waccess( filespec, 0 ) )
	{
		CContractPeople dlg;
		if( dlg.DoModal() == IDCANCEL )
		{
			CDialogEx::OnCancel();
			return;
		}
		if( type == UNITED_FILE )
			filespec = dlg.m_fileSpecTextU;
		else
			filespec = dlg.m_fileSpecTextD;
	}

	CStdioFile file( filespec, CFile::modeRead);   // input csv f�jl
	int fileLength = (int)file.GetLength();

	file.ReadString( cLine );
	m_azonos		= _wtoi( getLastWord( cLine ) );

	CStringArray A;
	int n;
	int cnt = 0;
	m_numOfGroups = 0;
	
	vPeople.clear();
	vFiltered.clear();
	CProgressWnd wndP( NULL, str ); 
	wndP.GoModal();
	wndP.SetRange( 0, fileLength );
	wndP.SetPos(0 );
	while( file.ReadString( cLine ) )
	{
		++cnt;
		n = wordList( &A, cLine, '\t', true );
		if( n != L_COLUMNSCOUNT )
		{
			str.Format( L"Az %d. sorban az elemek sz�ma %d.\n%d kellen lenni.", cnt, n, L_COLUMNSCOUNT );
			AfxMessageBox( str );
			CDialog::OnCancel();
		}

		if( A[1].IsEmpty() ) ++m_numOfGroups;		// azonos snev� emberek sz�ma

		for( UINT i = 0; i < A.GetSize(); ++i )
		{
			push( A[i] );
		}
		wndP.SetPos( file.GetPosition() );
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	file.Close();
	if( !vPeople.size() )
	{
		AfxMessageBox( L"Nincs �sszevonhat� bejegyz�s az adatb�zisban!", MB_ICONINFORMATION );
		OnOK();
		return;
	}

	m_ListCtrl.SetItemCountEx( vPeople.size() + 1  );
	m_ListCtrl.AttachDataset( &vPeople );

//	m_azonos = theApp.getUserVersion() && 0XFF;

//	str.Format( L"Az �sszevon�shoz megk�vetelt egyez�sek sz�ma: %d", m_azonos );
//	GetDlgItem( IDC_CAPTION )->SetWindowTextW( str );

	ShowWindow( SW_MAXIMIZE );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::push( CString item )
{

	int tLen;
	TCHAR* sT;
	tLen	= item.GetLength()+1;
	sT		= new TCHAR[tLen];
	_tcscpy_s( sT, tLen, item.GetBuffer() );
	vPeople.push_back( sT );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S � S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractedPeople::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az als� return-re debug m�dban hib�t jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress( nItem + 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnClickedKeress()
{
	keress( 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, L_NAME, start );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::createColumns()
{
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( L_CNT,				L"xyz",			LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_LOOP,			L"loop",		LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GROUP,			L"gr",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MATCH,			L"m#",			LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_GROUP2,			L"gr2",			LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_STATUS,			L"st",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_RGBCOLOR,		L"color",		LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_LINENUMBER,		L"line#",		LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_UNITED,			L"U",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_SOURCE,			L"S",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATION,		L"G",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	
	m_ListCtrl.InsertColumn( L_ROWID,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,			L"n�v",			LVCFMT_LEFT,	250,-1,COL_TEXT );
	
	m_ListCtrl.InsertColumn( L_ROWIDF,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,			L"apa",			LVCFMT_LEFT,	250,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( L_ROWIDM,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,			L"anya",		LVCFMT_LEFT,	250,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( L_ROWIDS,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SPOUSES,			L"h�zast�rsak",	LVCFMT_LEFT,	500,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_LINEF,			L"line#F",		LVCFMT_LEFT,	 60,-1,COL_HIDDEN );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;

	*pResult = 0;

	switch( pLVCD->nmcd.dwDrawStage )
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		nItem	= pLVCD->nmcd.dwItemSpec;
		nCol	= pLVCD->iSubItem;

		if( vFiltered.size() )
			pLVCD->clrTextBk = _wtoi( vFiltered.at( nItem*L_COLUMNSCOUNT + L_RGBCOLOR ) );
		else
			pLVCD->clrTextBk = _wtoi( vPeople.at( nItem*L_COLUMNSCOUNT + L_RGBCOLOR ) );

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnInfo()
{
	CContractInfo dlg;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnFilterAll()
{
	if( UNITED )
		str = L"Azonos nev� emberek, akik k�z�tt �sszevon�sok t�rt�ntek";
	else
		str = L"Nem �sszevonhat� azonos nev� emberek";
	SetWindowTextW( str );
	vFiltered.clear();
	m_ListCtrl.SetItemCountEx( vPeople.size() + 1  );
	m_ListCtrl.AttachDataset( &vPeople );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnFilter1()
{
	filter( 1 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnFilter2()
{
	filter( 2 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnFilter3()
{
	filter( 3 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnFilter4()
{
	filter( 4 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::filter( int iter )
{
	int loop;
	vFiltered.clear();
	for( UINT i = 0; i < vPeople.size()- L_COLUMNSCOUNT+1; i += L_COLUMNSCOUNT)
	{
		loop = _wtoi( vPeople.at(i+1) );
		if( loop == iter )
		{
			for( UINT j = 0; j < L_COLUMNSCOUNT; ++j ) // ix-1 a cnt-re mutat
			{
				vFiltered.push_back( vPeople.at( i + j  ) );
			}
		}
	}
	if( UNITED )
		str.Format( L"Azonos nev� emberek, akik k�z�tt �sszevon�sok t�rt�ntek az %d. ciklusban", iter );
	else
		str.Format( L"A nem �sszevonhat� azonos nev� emberek az %d. ciklusban", iter );
	SetWindowTextW( str );
	m_ListCtrl.SetItemCountEx( vFiltered.size() + 1  );
	m_ListCtrl.AttachDataset( &vFiltered );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnFilterByname()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijel�lve sor!" );
		return;
	}

	
	CString name;
	CString nev;
	int pos;
	name = m_ListCtrl.GetItemText( nItem, L_NAME );
	if( (pos = name.Find( '(' ) ) != -1 )
		name = name.Left( pos );
	name.Trim();

	UINT i;
	UINT j;
	bool pushed = false;

	UINT length = name.GetLength();

	vFiltered.clear();
	for( i = 0; i < vPeople.size()- L_COLUMNSCOUNT+1; i += L_COLUMNSCOUNT )
	{
		nev		= vPeople.at(i +L_NAME );
		if( nev.Left( length ) == name  ) 
		{
			pushLine( i );
			pushed = true;
		}
		else if( pushed )
		{
			pushLine( i );		// szepar�l� �res sor
			pushed = false;
		}
	}

	if( UNITED )
		str.Format( L"%s nev� emberek, akik k�z�tt �sszevon�sok t�rt�ntek", name );
	else
		str.Format( L"%s nev� emeberek, akik nem lettek �sszevonva", name );
	SetWindowTextW( str );
	m_ListCtrl.SetItemCountEx( vFiltered.size() + 1  );
	m_ListCtrl.AttachDataset( &vFiltered );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::pushLine( int i )
{
	for( UINT j = 0; j < L_COLUMNSCOUNT; ++j ) // ix-1 a cnt-re mutat
	{
		vFiltered.push_back( vPeople.at( i + j  ) );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;
	if( m_ListCtrl.GetItemText( nItem, L_STATUS ) == L"-1" )
	{
		AfxMessageBox( L"Ez �sszevont bejegyz�s, nincs m�g�tte m�r ember!" );
		return;
	}

	CString rowid	= m_ListCtrl.GetItemText( nItem, L_ROWID );

	CRelatives  dlg;
//	dlg.nItem		= nItem;
	dlg.m_rowid		= rowid;
	if( dlg.DoModal() == IDCANCEL ) return;

	*pResult = 0;
}
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnHtml1D()
{
	getFileSpec( DIFFERENT1_HTML_FILE );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnHtml1U()
{
	getFileSpec( UNITED1_HTML_FILE );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnHtml2D()
{
	getFileSpec( DIFFERENT2_HTML_FILE );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnHtml2U()
{
	getFileSpec( UNITED2_HTML_FILE );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::getFileSpec( int type )
{
	CString filespec;
	m_command.Format( L"SELECT filespec FROM filespec WHERE type=%d", type );
	if( !theApp.query( m_command ) );
	filespec = theApp.m_recordset->GetFieldString( 0 );
	if( filespec.IsEmpty() || _waccess( filespec, 0 ) )
	{
		str.Format( L"%s\nf�jl nem l�tezik!", filespec );
		AfxMessageBox( str );
	}
	theApp.showHtmlFile( filespec );
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CContractedPeople:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString name;
	CString nameS;
	name = m_ListCtrl.GetItemText( nItem, L_NAME );
	nameS = m_ListCtrl.GetItemText( nItem, L_SPOUSES );
	if( selectedCount == 1 )
		title.Format( L"%s-%s a ga.html f�jlban (%s. sor)", name, nameS, m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%s-%s �s m�sok a ga.html f�jlban (%s. sor)", name, nameS, m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );


	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s sz�lei �s testv�rei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnHtmlChildren()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s �s gyermekei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_CHILDREN";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::On3Generations()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );

	CRelatives dlg;
	dlg.m_rowid = rowid;

	dlg.DoModal();
//	ShowWindow( SW_RESTORE );

}
