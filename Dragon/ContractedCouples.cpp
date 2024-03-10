// ContractedCouples.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "ContractedCouples.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "ContractCouples.h"
#include "uniteInfo.h"
#include "html_EditLines.h"
#include "Relatives.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnContractedCouples()
{
	CContractedCouples dlg;
	dlg.DoModal();
	mainTitle();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeress);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContractedCouples, CDialogEx)
	ON_COMMAND(IDC_STATIC_KERESS, &CContractedCouples::OnClickedKeress)
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

	// IDR_DROPDOWN_HTML funkci�k	
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
	m_ListCtrl.InsertColumn( L_WEDDING,			L"esk�v�",		LVCFMT_LEFT,	 70,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( L_STATUSH,			L"st",			LVCFMT_RIGHT,	 20,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_LINENUMBERH,		L"line#",		LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATIONH,		L"G",			LVCFMT_RIGHT,	 20,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SOURCEH,			L"S",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_UNITEDH,			L"U",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDH,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_HUSBAND,			L"f�rj",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHH,			L"sz�let�s",	LVCFMT_LEFT,	 70,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_DEATHH,			L"hal�loz�s",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHERH,			L"apja",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHERH,			L"anyja",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_STATUSW,			L"st",			LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_LINENUMBERW,		L"line#",		LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATIONW,		L"G",			LVCFMT_RIGHT,	 20,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SOURCEW,			L"S",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_UNITEDW,			L"U",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDW,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_WIFE,			L"feles�g",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHW,			L"sz�let�s",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATHW,			L"hal�l",		LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHERW,			L"apa",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHERW,			L"anya",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_columnsCount	= m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	
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
	str.Format( L"Azonos nev� h�zasp�rok, akik nem vonhat�ak �ssze" );
	if( !inputFile( COUPLESD_TEXT_FILE ) )return;

	menu.EnableMenuItem( ID_INPUT_UNITED, MF_BYCOMMAND | MF_ENABLED);
	menu.EnableMenuItem( ID_INPUT_DIFFERENT, MF_BYCOMMAND | MF_GRAYED);
	UNITED = false;

	
	SetWindowTextW( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnInputUnited()
{
	str.Format( L"Azonos nev� h�zasp�rok, akik k�z�tt �sszevon�sok t�rt�ntek (%d azonos nev� h�zasp�r )", m_numOfGroups );
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

	// Ha nincs a "files" t�bl�ban a keresett f�jl, akkor el�sz�r �sszevon�st v�gez
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

	CStdioFile file( filespec, CFile::modeRead);   // input csv f�jl
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
	//	n = splitCString( cLine, '\t', true, &A );
		n = wordList(&A, cLine, '\t', true);
		if( n != L_COLUMNSCOUNT )
		{
			str.Format( L"Az %d. sorban az elemek sz�ma %d.\n%d kellen lenni.", cnt, n, L_COLUMNSCOUNT );
			AfxMessageBox( str );
			break;
			return false;
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
		str = L"Azonos nev� h�zast�rsak, akik k�z�tt �sszevon�sok t�rt�ntek";
	else
		str.Format( L"Azonos nev� h�zasp�rok, akik k�z�tt �sszevon�sok t�rt�ntek (%d azonos nev� h�zasp�r )", m_numOfGroups );
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
		str = L"Azonos nev� h�zast�rsdak, akik k�z�tt �sszevon�sok t�rt�ntek az 1. ciklusban";
	else
		str = L"Azonos nev� h�zasp�rok, akik nem vonhat�k �ssze az 1. ciklusban";
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
		str = L"Azonos nev� h�zast�rsdak, akik k�z�tt �sszevon�sok t�rt�ntek a 2. ciklusban";
	else
		str = L"Azonos nev� h�zasp�rok, akik nem vonhat�k �ssze a 2. ciklusban";
	SetWindowTextW( str );
	m_ListCtrl.SetItemCountEx( vFiltered.size() + 1  );
	m_ListCtrl.AttachDataset( &vFiltered );	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S � S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractedCouples::PreTranslateMessage(MSG* pMsg)
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
void CContractedCouples::OnClickedKeress()
{
	keress( 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress( nItem + 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, L_HUSBAND, start );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnInfo()
{
	CString info = L"\
Az azonos nev� h�zasp�rok f�rfi ill. n� tagjainak azonoss�g�t vizsg�ljuk, �s az azonosnak tal�lt bejegyz�sket �sszevonjuk.\r\n\
\r\n\
Az �sszehasonl�t�shoz az al�bbi adatokat vizsg�ljuk:\r\n\
\r\n\
- az ember sz�let�si d�tuma\r\n\
- az ember hal�loz�si d�tuma\r\n\
- az ember apja neve\r\n\
- az ember anyja neve\r\n\\r\n\
. a h�zast�rs neve (a vizsg�lt emberek alap felt�tele )\r\n\
- a h�zast�rs sz�let�si d�tuma\r\n\
- a h�zast�rs hal�loz�si d�tuma\r\n\
- a h�zast�rs apja neve\r\n\
- a h�zast�rs anyja neve\r\n\
\r\n\
A vizsg�lt emberek kiv�lasz�s�nal a h�zast�rs nev�nek azonoss�g�t is el��rjuk, teh�t ez egy eleve teljes�l� krit�rium.\n\r\
Az azonoss�gi krit�riumok teh�t gyeng�bbek, mint az azonos nev� emberek �sszevon�s�n�l haszn�lt krit�riumok, mert nem vizsg�ljuk\
az apa �s anya sz�let�si �s hal�loz�si d�tum�t. Ugyanakkor a h�zast�rs nev�nek azonoss�ga szigr�bb felt�tel, mert az el�bbin�l \
megengedj�k, hogy a h�zast�rsak egyyike legyen, nem azonos.\r\n\
";


	CUniteInfo dlg;
	dlg.m_title = L"Azonos nev� h�zast�rsak h�zasp�rjaink azonoss�gi vizsg�lata �s bejegyz�sek �sszevon�sa"; 
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
		str.Format( L"%s\nf�jl nem l�tezik!", filespec );
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
		title.Format( L"%s a ga.html f�jlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_HUSBAND ), m_ListCtrl.GetItemText( nItem, L_LINENUMBERH )  );
	else
		title.Format( L"%d kijel�lt sor a ga.html f�jlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBERH, title );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBERH );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
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
	dlg.m_title.Format( L"%s sz�lei �s testv�rei", m_ListCtrl.GetItemText( nItem, L_HUSBAND ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWIDH );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
