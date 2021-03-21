// ContractedCouples.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "ContractedCouples.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "ContractCouples.h"
#include "ContractInfo.h"
#include "html_EditLines.h"
//#include "html_Edit2Lines.h"
//#include "html_EditLine.h"
#include "Relations.h"

enum
{
	L_LOOP = 0,
	L_GROUP,
	L_SUBGROUP,
	L_COLORCODE,
	L_MARRIAGE_ID,
	L_WEDDING,
	L_STATUSH,
	L_LINENUMBERH,
	L_GENERATIONH, 
	L_SOURCEH,
	L_UNITEDH,
	L_ROWIDH,
	L_HUSBAND,
	L_BIRTHH,
	L_DEATHH,
	L_FATHERH,
	L_MOTHERH,
	L_STATUSW,
	L_LINENUMBERW,
	L_GENERATIONW, 
	L_SOURCEW,
	L_UNITEDW,
	L_ROWIDW,
	L_WIFE,
	L_BIRTHW,
	L_DEATHW,
	L_FATHERW,
	L_MOTHERW,
	L_COLUMNSCOUNT,
};

// CContractedCouples dialog

IMPLEMENT_DYNAMIC(CContractedCouples, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractedCouples::CContractedCouples(CWnd* pParent /*=NULL*/)
	: CDialogEx(CContractedCouples::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractedCouples::~CContractedCouples()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_KERESS, colorKeress);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContractedCouples, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(IDC_KERESS, &CContractedCouples::OnClickedKeress)
	ON_STN_CLICKED(IDC_NEXT, &CContractedCouples::OnClickedNext)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CContractedCouples::OnCustomdrawList)
	ON_COMMAND(ID_INPUT_DIFFERENT, &CContractedCouples::OnInputDifferent)
	ON_COMMAND(ID_INPUT_UNITED, &CContractedCouples::OnInputUnited)
	ON_COMMAND(ID_FILTER_ALL, &CContractedCouples::OnFilterAll)
	ON_COMMAND(ID_FILTER_1, &CContractedCouples::OnFilter1)
	ON_COMMAND(ID_FILTER_2, &CContractedCouples::OnFilter2)

	ON_COMMAND(ID_INFO, &CContractedCouples::OnInfo)
	ON_COMMAND(ID_HTML_1_D, &CContractedCouples::OnHtml1D)
	ON_COMMAND(ID_HTML_1_U, &CContractedCouples::OnHtml1U)
	ON_COMMAND(ID_HTML_2_D, &CContractedCouples::OnHtml2D)
	ON_COMMAND(ID_HTML_2_U, &CContractedCouples::OnHtml2U)

	// IDR_DROPDOWN_HTML funkciók	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CContractedCouples::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CContractedCouples::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CContractedCouples::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CContractedCouples::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_DB_EDIT, &CContractedCouples::OnDbEdit)


END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractedCouples::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();

	menu.LoadMenu( IDR_CONTRACTED_COUPLES );
	SetMenu(&menu);

	colorKeress.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );

	createColumns();
	OnInputUnited();
	
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::push( CString item )
{

	int tLen;
	TCHAR* sT;
	tLen	= item.GetLength()+1;
	sT		= new TCHAR[tLen];
	_tcscpy_s( sT, tLen, item.GetBuffer() );
	vCouples.push_back( sT );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::createColumns()
{
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( L_LOOP,			L"L",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GROUP,			L"GR",			LVCFMT_RIGHT,	 30,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_SUBGROUP,		L"gr",			LVCFMT_RIGHT,	 25,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_COLORCODE,		L"color",		LVCFMT_RIGHT,	 20,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_MARRIAGE_ID,		L"m_id",		LVCFMT_RIGHT,	 80,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_WEDDING,			L"esküvõ",		LVCFMT_LEFT,	 70,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( L_STATUSH,			L"st",			LVCFMT_RIGHT,	 20,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_LINENUMBERH,		L"line#",		LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATIONH,		L"G",			LVCFMT_RIGHT,	 20,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SOURCEH,			L"S",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_UNITEDH,			L"U",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDH,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_HUSBAND,			L"férj",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHH,			L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_DEATHH,			L"halálozás",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHERH,			L"apja",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHERH,			L"anyja",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_STATUSW,			L"st",			LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_LINENUMBERW,		L"line#",		LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATIONW,		L"G",			LVCFMT_RIGHT,	 20,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SOURCEW,			L"S",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_UNITEDW,			L"U",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDW,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_WIFE,			L"feleség",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHW,			L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATHW,			L"halál",		LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHERW,			L"apa",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHERW,			L"anya",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_columnsCount	= m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CContractedCouples::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	int iData;
	ULONG mask;
	CString	statusH;
	CString	statusW;
	bool husband;
	bool wife;

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
		husband = L_STATUSH <= nCol && nCol <= L_MOTHERH;
		wife	= L_STATUSW <= nCol && nCol <= L_MOTHERW;
		if( UNITED )
		{
			statusH = m_ListCtrl.GetItemText( nItem, L_STATUSH );
			statusW = m_ListCtrl.GetItemText( nItem, L_STATUSW );
			if( statusH == L"1" && husband )
				pLVCD->clrTextBk = LIGHTGREEN;
			else if( statusW == L"1" && wife ) 
				pLVCD->clrTextBk = YELLOW;
			else if( statusH == L"-1" && husband)
				pLVCD->clrTextBk = GRAY;
			else if( statusW == L"-1" && wife )
				pLVCD->clrTextBk = GRAY;
			else if( statusW == L"0" && (wife || husband ))
				pLVCD->clrTextBk = RGB( 255, 204, 204 );
		}
		else
		{
			mask = 1 << nCol;
			if( mask & _wtol( m_ListCtrl.GetItemText( nItem, L_COLORCODE  ) ) )
				pLVCD->clrTextBk = YELLOW;
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnInputDifferent()
{
	str.Format( L"Azonos nevû házaspárok, akik nem vonhatóak össze" );
	if( !inputFile( COUPLESD_TEXT_FILE ) )return;

	menu.EnableMenuItem( ID_INPUT_UNITED, MF_BYCOMMAND | MF_ENABLED);
	menu.EnableMenuItem( ID_INPUT_DIFFERENT, MF_BYCOMMAND | MF_GRAYED);
	UNITED = false;

	
	SetWindowTextW( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnInputUnited()
{
	str.Format( L"Azonos nevû házaspárok, akik között összevonások történtek (%d azonos nevû házaspár )", m_numOfGroups );
	if( !inputFile( COUPLESU_TEXT_FILE ) )return;

	UNITED = true;
	menu.EnableMenuItem( ID_INPUT_UNITED, MF_BYCOMMAND | MF_GRAYED);
	menu.EnableMenuItem( ID_INPUT_DIFFERENT, MF_BYCOMMAND | MF_ENABLED);

	SetWindowTextW( str );
	ShowWindow( SW_MAXIMIZE );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CContractedCouples::inputFile( int type )
{
	CString filespec;

	// Ha nincs a "files" táblában a keresett fájl, akkor elõször összevonást végez
	while( true )
	{
		m_command.Format( L"SELECT filespec FROM filespec WHERE type=%d", type );
		if( !theApp.query( m_command ) );
		filespec = theApp.m_recordset->GetFieldString( 0 );
		if( filespec.IsEmpty() || _waccess( filespec, 0 ) )
		{
			CContractCouples cc;
			cc.contractCouples();
		}
		else
			break;
	}

	CStdioFile file( filespec, CFile::modeRead);   // input csv fájl
	int fileLength = (int)file.GetLength();

	CStringArray A;
	int n;
	int cnt = 0;
	m_numOfGroups = 0;
	
	vCouples.clear();
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
			str.Format( L"Az %d. sorban az elemek száma %d.\n%d kellen lenni.", cnt, n, L_COLUMNSCOUNT );
			AfxMessageBox( str );
			break;
			return false;
		}

		if( A[1].IsEmpty() ) ++m_numOfGroups;		// azonos snevû emberek száma

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
	m_ListCtrl.SetItemCountEx( vCouples.size() + 1  );
	m_ListCtrl.AttachDataset( &vCouples );

	GetDlgItem( IDC_CAPTION )->SetWindowTextW( filespec );
	ShowWindow( SW_MAXIMIZE );
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnFilterAll()
{
	if( UNITED )
		str = L"Azonos nevû házastársak, akik között összevonások történtek";
	else
		str.Format( L"Azonos nevû házaspárok, akik között összevonások történtek (%d azonos nevû házaspár )", m_numOfGroups );
	SetWindowTextW( str );
	vFiltered.clear();
	m_ListCtrl.SetItemCountEx( vCouples.size() + 1  );
	m_ListCtrl.AttachDataset( &vCouples );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnFilter1()
{
	int loop;
	vFiltered.clear();
	for( UINT i = 0; i < vCouples.size()- m_columnsCount+1; i += m_columnsCount )
	{
		loop = _wtoi( vCouples.at(i) );
		if( loop == 1 )
		{
			for( UINT j = 0; j < m_columnsCount; ++j ) // ix-1 a cnt-re mutat
			{
				vFiltered.push_back( vCouples.at( i + j  ) );
			}
		}
	}
	if( UNITED )
		str = L"Azonos nevû házastársdak, akik között összevonások történtek az 1. ciklusban";
	else
		str = L"Azonos nevû házaspárok, akik nem vonhatók össze az 1. ciklusban";
	SetWindowTextW( str );
	m_ListCtrl.SetItemCountEx( vFiltered.size() + 1  );
	m_ListCtrl.AttachDataset( &vFiltered );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnFilter2()
{
	int loop;
	vFiltered.clear();
	for( UINT i = 0; i < vCouples.size()- m_columnsCount+1; i += m_columnsCount )
	{
		loop = _wtoi( vCouples.at(i) );
		if( loop == 2 )
		{
			for( UINT j = 0; j < m_columnsCount; ++j ) // ix-1 a cnt-re mutat
			{
				vFiltered.push_back( vCouples.at( i + j  ) );
			}
		}
	}
	if( UNITED )
		str = L"Azonos nevû házastársdak, akik között összevonások történtek a 2. ciklusban";
	else
		str = L"Azonos nevû házaspárok, akik nem vonhatók össze a 2. ciklusban";
	SetWindowTextW( str );
	m_ListCtrl.SetItemCountEx( vFiltered.size() + 1  );
	m_ListCtrl.AttachDataset( &vFiltered );	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnClickedKeress()
{
	keress( 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -0 )
	{
		AfxMessageBox( L"Nincs kijelölve sor!" );
		return;
	}
	keress( nItem + 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	if( search.IsEmpty() )
	{
		AfxMessageBox( L"Meg kell adni a keresendõ stringet!");
		return;
	}

	CProgressWnd wndProgress(NULL, L"Folyik a keresés.." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, m_ListCtrl.GetItemCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);



	int		itemCnt	= m_ListCtrl.GetItemCount();
	int		length	= search.GetLength();
	int		nItem;
	int		topIndex = m_ListCtrl.GetTopIndex();
	CString	str;

	theApp.unselectAll( &m_ListCtrl );

	for( nItem = start; nItem < itemCnt-1; ++nItem )
	{
		str = m_ListCtrl.GetItemText( nItem, L_HUSBAND );
		str = str.Left(length);						// az aktuális search string hosszával azonos hosszúság leválasztása
		if( str == search )	break;
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	if( nItem < itemCnt-1 )			// megtalálta a keresett embert,. aki az nItem-1 sorban van
	{
		m_ListCtrl.EnsureVisible( nItem, FALSE );

		if( nItem > topIndex )   // lefele megy, fel kell hozni a tábla tetejére a megtalált sort
		{
			int countPP = m_ListCtrl.GetCountPerPage();
			int nItemEV	= nItem - 1 + countPP;			// alaphelyzet: a kijelölt sor az ablak tetején

			if( nItemEV > itemCnt - 1 )					// már nem lehet az ablak tetejére hozni, mert nincs annyi adat
				nItemEV = itemCnt - 1;

			m_ListCtrl.EnsureVisible( nItemEV, FALSE );
		}
		m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		Invalidate( false );
	}
	else
	{
		str.Format( L"%s nevû embert nem találtam!", search );
		AfxMessageBox( str );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////44
BOOL CContractedCouples::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
			GetDlgItem( IDC_SEARCH )->GetWindowTextW( str );
			if( str.GetLength() ) 
			OnClickedKeress();
			break;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnInfo()
{
	CString info = L"\
Az azonos nevû házaspárok férfi ill. nõ tagjainak azonosságát vizsgáljuk, és az azonosnak talált bejegyzésket összevonjuk.\r\n\
\r\n\
Az összehasonlításhoz az alábbi adatokat vizsgáljuk:\r\n\
\r\n\
- az ember születési dátuma\r\n\
- az ember halálozási dátuma\r\n\
- az ember apja neve\r\n\
- az ember anyja neve\r\n\\r\n\
. a házastárs neve (a vizsgált emberek alap feltétele )\r\n\
- a házastárs születési dátuma\r\n\
- a házastárs halálozási dátuma\r\n\
- a házastárs apja neve\r\n\
- a házastárs anyja neve\r\n\
\r\n\
A vizsgált emberek kiválaszásánal a házastárs nevének azonosságát is elõírjuk, tehát ez egy eleve teljesülû kritérium.\n\r\
Az azonossági kritériumok tehát gyengébbek, mint az azonos nevû emberek összevonásánál használt kritériumok, mert nem vizsgáljuk\
az apa és anya születési és halálozási dátumát. Ugyanakkor a házastárs nevének azonossága szigrúbb feltétel, mert az elõbbinél \
megengedjük, hogy a házastársak egyyike legyen, nem azonos.\r\n\
";


	CContractInfo dlg;
	dlg.m_title = L"Azonos nevû házastársak házaspárjaink azonossági vizsgálata és bejegyzések összevonása"; 
	dlg.m_info = info;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnHtml1D()
{
	getFileSpec( COUPLESD1_HTML_FILE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnHtml1U()
{
	getFileSpec( COUPLESU1_HTML_FILE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnHtml2D()
{
	getFileSpec( COUPLESD2_HTML_FILE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnHtml2U()
{
	getFileSpec( COUPLESU2_HTML_FILE );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::getFileSpec( int type )
{
	CString filespec;
	m_command.Format( L"SELECT filespec FROM filespex WHERE type=%d" , type );
	if( !theApp.query( m_command ) );
	filespec = theApp.m_recordset->GetFieldString( 0 );
	if( filespec.IsEmpty() || _waccess( filespec, 0 ) )
	{
		str.Format( L"%s\nfájl nem létezik!", filespec );
		AfxMessageBox( str );
	}
	theApp.showHtmlFile( filespec );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CContractedCouples:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_HUSBAND ), m_ListCtrl.GetItemText( nItem, L_LINENUMBERH )  );
	else
		title.Format( L"%d kijelölt sor a ga.html fájlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBERH, title );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBERH );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWIDH );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWIDH );
	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s szülei és testvérei", m_ListCtrl.GetItemText( nItem, L_HUSBAND ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWIDH );
	CRelations dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
