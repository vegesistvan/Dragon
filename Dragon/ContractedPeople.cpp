// ContractedPeople.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "ContractedPeople.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "ProgressWnd.h"
#include "SamePeopleInfo.h"
#include "FilterLoop.h"

// CContractedPeople dialog
// CListCtrl táblázat oszlopai
enum
{
	S_CNT = 0,
	S_LOOP, 
	S_GROUP,
	S_MATCH,
	S_GROUP2,
	S_STATUS,
	S_RGBCOLOR,
	S_LINE,
	S_UNITED,
	S_GENERATION,
	S_SOURCE,
	S_ROWID,
	S_NAME,
	S_BIRTH,
	S_DEATH,
	S_ROWIDF,
	S_FATHER,
	S_BIRTHF,
	S_DEATHF,
	S_ROWIDM,
	S_MOTHER,
	S_BIRTHM,
	S_DEATHM,
	S_ROWIDS,
	S_SPOUSES,
	S_LINEF,
};
IMPLEMENT_DYNAMIC(CContractedPeople, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractedPeople::CContractedPeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CContractedPeople::IDD, pParent)
{
	m_rgb[0] = RGB( 255,25,255 );
	m_rgb[1] = RGB( 127, 255, 212 );
	m_rgb[2] = RGB( 255, 255, 0 );
	m_rgb[3] = RGB( 0, 191, 255 );
	m_rgb[4] = RGB( 173, 255, 47 );
	m_rgb[5] = RGB( 216,191,216 );
	m_rgb[6] = RGB( 255, 105,180 );
	m_rgb[7] = RGB( 255, 255, 224 );
	m_rgb[8] = RGB( 0,255,255 );
	m_rgb[9] = RGB( 211,211,211 );
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
	DDX_Control(pDX, IDC_KERESS, colorKeress);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContractedPeople, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_STN_CLICKED(IDC_KERESS, &CContractedPeople::OnClickedKeress)
	ON_STN_CLICKED(IDC_NEXT, &CContractedPeople::OnClickedNext)
	ON_COMMAND(ID_INPUT_CSVTEXT, &CContractedPeople::OnInputCsvtext)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CContractedPeople::OnCustomdrawList)
	ON_COMMAND(ID_INFO, &CContractedPeople::OnInfo)
	ON_COMMAND(ID_FILTER_ALL, &CContractedPeople::OnFilterAll)
	ON_COMMAND(ID_FILTER_1, &CContractedPeople::OnFilter1)
	ON_COMMAND(ID_FILTER_2, &CContractedPeople::OnFilter2)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractedPeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();


	colorKeress.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );
	createColumns();

	OnInputCsvtext();
	return TRUE;
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -0 )
	{
		AfxMessageBox( L"Nincs kijelölve sor!" );
		return;
	}
	keress( nItem + 1 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////44
BOOL CContractedPeople::PreTranslateMessage(MSG* pMsg)
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
//	return CDialogEx::PreTranslateMessage(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
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
		str = m_ListCtrl.GetItemText( nItem, S_NAME );
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::createColumns()
{
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( S_CNT,				L"cnt",			LVCFMT_RIGHT,	 20,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( S_LOOP,			L"loop",		LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_GROUP,			L"gr",			LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( S_MATCH,			L"m#",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_GROUP2,			L"gr2",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_STATUS,			L"st",			LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( S_RGBCOLOR,		L"color",		LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_LINE,			L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_UNITED,			L"U",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( S_SOURCE,			L"S",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_GENERATION,		L"G",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_ROWID,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_NAME,			L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( S_BIRTH,			L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_DEATH,			L"halálozás",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_ROWIDF,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_FATHER,			L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( S_BIRTHF,			L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_DEATHF,			L"halál",		LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_ROWIDM,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_MOTHER,			L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( S_BIRTHM,			L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_DEATHM,			L"halál",		LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_ROWIDS,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_SPOUSES,			L"házastársak",	LVCFMT_LEFT,	500,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( S_LINEF,			L"line#F",		LVCFMT_LEFT,	 60,-1,COL_HIDDEN );
	m_columnsCount	= m_ListCtrl.GetHeaderCtrl()->GetItemCount();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnInputCsvtext()
{

//	 m_fileSpec = L"d:\\FA_DATABASES\\2021.01.04\\lists\\PeopleUnited_202101141456.text";

	 m_command.Format( L"SELECT filespec FROM contracted WHERE code1 = 1" );
	 if( !theApp.query( m_command ) ) return;
	 m_fileSpec = theApp.m_recordset->GetFieldString( 0 );


	 if( _waccess( m_fileSpec, 0 ) )
	 {
		 str.Format( L"%s\nfájl nem létezik", m_fileSpec );
		 AfxMessageBox( str );
		 return;
	 }


	 CStdioFile file( m_fileSpec, CFile::modeRead);   // input csv fájl
	 int fileLength = (int)file.GetLength();

	CStringArray A;
	int		n;
	int		cnt = 0;
	int		items = 0;
	int		pos;
	int		loop;

	m_loop = 0;
	vLoops.clear();
	vColor.clear();
	vTablePeople.clear();
	CProgressWnd wndP( NULL, L"Text fájl beolvasása..." ); 
	wndP.GoModal();
	wndP.SetRange( 0, fileLength );
	wndP.SetPos(0 );
	while( file.ReadString( cLine ) )
	{
		++cnt;
		n = wordList( &A, cLine, '\t', true );
		if( n != m_columnsCount )
		{
			str.Format( L"Az %d. sorban az elemek száma %d.\n%d kellen lenni.", cnt, n, m_columnsCount );
			AfxMessageBox( str );
			break;
			return;
		}
		if( A[0].IsEmpty() ) ++items;		// azonoi snevû emberek száma
		

		loop = _wtoi( A[1] );
		if( vLoops.empty() || vLoops.at( vLoops.size() -1 ) != loop ) 
			vLoops.push_back( loop );
		
		vColor.push_back( _wtoi( A[0] ) );
		str.Format( L"%d", cnt );
		push( str );
		for( UINT i = 1; i < A.GetSize(); ++i )
		{
			push( A[i] );
		}
		wndP.SetPos( file.GetPosition() );
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	file.Close();
	m_ListCtrl.SetItemCountEx( vTablePeople.size() + 1  );
	m_ListCtrl.AttachDataset( &vTablePeople );

	str.Format( L"Azonos nevû emberek, akik között összevonások történtek (%d azonos nevû ember ) %s", items, m_fileSpec );
	SetWindowTextW( str );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::push( CString item )
{

	int tLen;
	TCHAR* sT;
	tLen	= item.GetLength()+1;
	sT		= new TCHAR[tLen];
	_tcscpy_s( sT, tLen, item.GetBuffer() );
	vTablePeople.push_back( sT );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
//	int iData;

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
		/*
		iData   = vColor.at(nItem);
		iData   = _wtoi( vTablePeople.at( nItem*m_columnsCount + S_STATUS ) );
		if( iData  )
		{
			pLVCD->clrTextBk = m_rgb[iData];
		}
		*/

		if( vFiltered.size() )
			pLVCD->clrTextBk = _wtoi( vFiltered.at( nItem*m_columnsCount + S_RGBCOLOR ) );
		else
			pLVCD->clrTextBk = _wtoi( vTablePeople.at( nItem*m_columnsCount + S_RGBCOLOR ) );
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnInfo()
{
	CSamePeopleInfo dlg;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnFilterAll()
{
	vFiltered.clear();
	m_ListCtrl.SetItemCountEx( vTablePeople.size() + 1  );
	m_ListCtrl.AttachDataset( &vTablePeople );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnFilter1()
{
	int loop;
	vFiltered.clear();
	for( UINT i = 0; i < vTablePeople.size()- m_columnsCount+1; i += m_columnsCount )
	{
		loop = _wtoi( vTablePeople.at(i+1) );
		if( loop == 1 )
		{
			for( UINT j = 0; j < m_columnsCount; ++j ) // ix-1 a cnt-re mutat
			{
				vFiltered.push_back( vTablePeople.at( i + j  ) );
			}
		}
	}
	m_ListCtrl.SetItemCountEx( vFiltered.size() + 1  );
	m_ListCtrl.AttachDataset( &vFiltered );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedPeople::OnFilter2()
{
	int loop;
	vFiltered.clear();
	for( UINT i = 0; i < vTablePeople.size()- m_columnsCount+1; i += m_columnsCount )
	{
		loop = _wtoi( vTablePeople.at(i+1) );
		if( loop == 2 )
		{
			for( UINT j = 0; j < m_columnsCount; ++j ) // ix-1 a cnt-re mutat
			{
				vFiltered.push_back( vTablePeople.at( i + j  ) );
			}
		}
	}
	m_ListCtrl.SetItemCountEx( vFiltered.size() + 1  );
	m_ListCtrl.AttachDataset( &vFiltered );
}
