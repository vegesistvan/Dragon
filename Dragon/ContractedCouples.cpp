// ContractedCouples.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "ContractedCouples.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"

enum
{
	L_LOOP = 0,
	L_COLORCODE,
	L_MARRIAGE_ID,
	L_WEDDING,
	L_GROUP,
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
	L_WIFW,
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
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContractedCouples, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(IDC_KERESS, &CContractedCouples::OnKeress)
	ON_STN_CLICKED(IDC_NEXT, &CContractedCouples::OnClickedNext)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractedCouples::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();

	 m_command.Format( L"SELECT filespec FROM contracted WHERE %s", m_filter );
	 if( !theApp.query( m_command ) ) return false;
	 m_fileSpec = theApp.m_recordset->GetFieldString( 0 );

	 if( m_fileSpec.IsEmpty() )
	 {
		 AfxMessageBox( L"Elõször el kel végezni az összevonásokat!" );
		 CDialog::OnOK();
		 return false;
	 }

	 if( _waccess( m_fileSpec, 0 ) )
	 {
		 str.Format( L"%s\nfájl nem létezik", m_fileSpec );
		 AfxMessageBox( str );
		 CDialog::OnOK();
		 return false;
	 }

	 	 CStdioFile file( m_fileSpec, CFile::modeRead);   // input csv fájl
	 int fileLength = (int)file.GetLength();

	CStringArray A;
	int		n;
	int		cnt = 0;
	int		items = 0;
	int		pos;
	int		loop;

	vCouples.clear();
	
	if( m_contracted )
		str = L"Összevont házaspárok táblázatának elkészítése...";
	else
		str = L"A nem összevonható házaspárok táblázatának elkészítése...";

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

		if( A[0].IsEmpty() ) ++items;		// azonos snevû emberek száma

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
	/*
	m_ListCtrl.InsertColumn( S_CNT,				L"cnt",			LVCFMT_RIGHT,	 20,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( S_LOOP,			L"loop",		LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_GROUP,			L"gr",			LVCFMT_RIGHT,	 30,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( S_MATCH,			L"m#",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_GROUP2,			L"gr2",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_STATUS,			L"st",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
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
	m_ListCtrl.InsertColumn( S_LINENUMBERMF,	L"line#MF",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_columnsCount	= m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	*/
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
void CContractedCouples::OnKeress()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractedCouples::OnClickedNext()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////