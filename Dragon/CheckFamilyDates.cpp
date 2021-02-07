// CCheckFamilyDates.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "CheckFamilyDates.h"
#include "utilities.h"
#include "ContractInfo.h"
#include "html_EditLines.h"
#include "Relations.h"

// SELECT  FROM people father
enum
{
	X_ROWID = 0,
	X_LINENUMBER,
	X_FIRST_NAME,
	X_LAST_NAME,
	X_BIRTH,
	X_DEATH,
};

// SELECT from people mother
enum
{
	Y_ROWID = 0,
	Y_LINENUMBER,
	Y_FIRST_NAME,
	Y_LAST_NAME,
	Y_BIRTH,
	Y_DEATH,
};


// SELECT FROM people child
enum
{
	Z_ROWID = 0,
	Z_LINENUMBER,
	Z_FIRST_NAME,
	Z_LAST_NAME,
	Z_BIRTH,
	Z_DEATH,
	Z_MOTHER_ID,
	Z_MOTHERINDEX,
};

// m_ListCtrl oszlopok
enum
{
	Q_CNT = 0,
	Q_LINENUMBER,
	Q_MARRIAGE,
	Q_INDEX,
	Q_ROWID,
	Q_NAME,
	Q_BIRTH,
	Q_DEATH,
	Q_COMMENT,
	Q_NUMOFCOLUMNS
};



// CCheckFamilyDates dialog

IMPLEMENT_DYNAMIC(CCheckFamilyDates, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFamilyDates::CCheckFamilyDates(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckFamilyDates::IDD, pParent)
{
	m_colors.Add( L"hotpink" );
	m_colors.Add( L"aquamarine" );		//127,255,212
	m_colors.Add( L"yellow" );			//255,255,0
	m_colors.Add( L"deepskyblue" );		//0,191,255
	m_colors.Add( L"greenyellow" );		//173,255,47
	m_colors.Add( L"thistle" );			//216,191,216
	m_colors.Add( L"hotpink" );			//255,105,180
	m_colors.Add( L"lightyellow" );		//255,255,224
	m_colors.Add( L"aqua" );			//0,255,255
	m_colors.Add( L"lightGray" );		//211,211,211

	m_rgb[0] = RGB( 255, 105, 180 );
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFamilyDates::~CCheckFamilyDates()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_KERES, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckFamilyDates, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_STN_CLICKED(IDC_KERES, &CCheckFamilyDates::OnClickedKeres)
	ON_STN_CLICKED(IDC_NEXT, &CCheckFamilyDates::OnClickedNext)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckFamilyDates::OnCustomdrawList)
	ON_COMMAND(ID_INFO, &CCheckFamilyDates::OnInfo)

// IDR_DROPDOWN_HTML funkciók	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckFamilyDates::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckFamilyDates::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_FAMILY, &CCheckFamilyDates::OnHtmlFamily)
	ON_COMMAND(ID_DB_EDIT, &CCheckFamilyDates::OnDbEdit)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();

	colorKeres.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );

	SetWindowTextW( L"Családok, amelyekben a férjnek több házassága volt, és a dátumok valamelyike ellentmondásos" ); 

	CString fileName = L"childrenFAmily";
	m_filespec = theApp.openTextFile( &textF, fileName, L"w+" );

	m_ageDiffM = 14;


	str.Format( L"Az anya %d éves kora után született gyerekeket fogadj el a program helyesnek.\n\n", m_ageDiffM ); 
	fwprintf( textF, str );


	createColumns();
	collectFamily();

	m_ListCtrl.SetItemCountEx( vList.size() + 1  );
	m_ListCtrl.AttachDataset( &vList );


	fclose( textF );
	ShowWindow( SW_MAXIMIZE );
//	theApp.showFile( m_filespec );
	return TRUE; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnClickedKeres()
{
	keress( 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnClickedNext()
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
void CCheckFamilyDates::keress( int start )
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
		str = m_ListCtrl.GetItemText( nItem, Q_NAME );
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
BOOL CCheckFamilyDates::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
			GetDlgItem( IDC_SEARCH )->GetWindowTextW( str );
			if( str.GetLength() ) 
			OnClickedKeres();
			break;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCheckFamilyDates::collectFamily()
{
	wndP.Create( NULL, L"Családok lekérdezése..." );
	wndP.GoModal();

	vList.clear();
	m_cnt = 1;

	m_command =L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date FROM people ORDER BY last_name, first_name";
	if( !theApp.query( m_command ) ) return false;

	
	wndP.SetRange( 0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	int z;
	UINT j;


	
	CString name;
	bool push;

	// férjek cikluse
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		h.rowid = theApp.m_recordset->GetFieldString( X_ROWID );

		m_command.Format( L"SELECT rowid,* FROM marriages WHERE spouse1_id='%s'", h.rowid );
		if( !theApp.query1( m_command ) ) break;
		if( theApp.m_recordset1->RecordsCount() < 2 ) goto cont;  // nincs vagy csak 1 felesége van
		h.numOfSpouses	= theApp.m_recordset1->RecordsCount();

		//gyerekek lekérdezése
		m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date, mother_id, parentIndexCalc FROM people WHERE father_id ='%s' ORDER BY parentIndexCalc", h.rowid );
		if( !theApp.query2( m_command ) ) break;
		if( !theApp.m_recordset2->RecordsCount() ) goto cont;		// nincsenek gyerekek
		
		h.linenumber	= theApp.m_recordset->GetFieldString( X_LINENUMBER );
		h.name.Format( L"%s %s", theApp.m_recordset->GetFieldString( X_LAST_NAME), getFirstWord( theApp.m_recordset->GetFieldString( X_FIRST_NAME) ) );
		h.birth			= theApp.m_recordset->GetFieldString( X_BIRTH );
		h.death			= theApp.m_recordset->GetFieldString( X_DEATH );
	
//		vHusband.push_back( h );

		vChildren.clear();
		for( UINT k = 0; k < theApp.m_recordset2->RecordsCount() ; ++k, theApp.m_recordset2->MoveNext() )
		{
			c.name.Format( L"%s %s", theApp.m_recordset2->GetFieldString( Z_LAST_NAME), getFirstWord( theApp.m_recordset2->GetFieldString( Z_FIRST_NAME) ) );

			c.linenumber	= theApp.m_recordset2->GetFieldString( Z_LINENUMBER );
			c.rowid			= theApp.m_recordset2->GetFieldString( Z_ROWID );
			c.birth			= theApp.m_recordset2->GetFieldString( Z_BIRTH );
			c.death			= theApp.m_recordset2->GetFieldString( Z_DEATH );
			c.father_id		= h.rowid;
			c.mother_id		= theApp.m_recordset2->GetFieldString( Z_MOTHER_ID );
			c.mother_index	= _wtoi( theApp.m_recordset2->GetFieldString( Z_MOTHERINDEX ) );
			

			vChildren.push_back( c );
		}

		// házasságok lekérdezése
		vWifes.clear();
		for( j = 0; j < theApp.m_recordset1->RecordsCount() ; ++j, theApp.m_recordset1->MoveNext() )
		{
			w.rowidH	= h.rowid;
			w.rowid		= theApp.m_recordset1->GetFieldString( MARRIAGES_SPOUSE2_ID );
			w.marriage	= theApp.m_recordset1->GetFieldString( MARRIAGES_DATE );

			// feleség lekérdezése
			m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date FROM people WHERE rowid='%s'", w.rowid );
			if( !theApp.query3( m_command ) ) break;

			w.linenumber	= theApp.m_recordset3->GetFieldString( Y_LINENUMBER );
			w.name.Format( L"%s %s", theApp.m_recordset3->GetFieldString( Y_LAST_NAME), getFirstWord( theApp.m_recordset3->GetFieldString( Y_FIRST_NAME) ) );
			w.birth			= theApp.m_recordset3->GetFieldString( Y_BIRTH );
			w.death			= theApp.m_recordset3->GetFieldString( Y_DEATH );
		
			w.motherOrder	= j+1;

			vWifes.push_back( w );
		}
		if( j < theApp.m_recordset1->RecordsCount() ) break;



		checkFamily();





cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::createColumns()
{

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( Q_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( Q_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( Q_MARRIAGE,	L"esküvõ",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( Q_INDEX,		L"ix",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( Q_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( Q_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( Q_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( Q_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( Q_COMMENT,		L"hiba",		LVCFMT_LEFT,	500,-1,COL_TEXT);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::checkFamily()
{
	WIFES		w;
	CHILDREN	c;
	int	mx;
	int age;
	bool hiba = false;;
	
	CString dateC;
	CString dateW;
	CString dateM;
	CString msg;

	_int64 birthH = 0;
	_int64 deathH = 0;
	_int64 birthW;
	_int64 deathW;

	_int64 marriage;
	_int64 birthC;
	_int64 deathC;

	// férj idõ-adatainak ellenõrzése
	if( checkDate( h.birth ) )
	{
		h.birth = roundDate( h.birth );
		birthH	= theApp.getDateI( h.birth, 0 );
	}
	if( checkDate( h.death ) )
	{
		h.death = roundDate( h.death );
		deathH	= theApp.getDateI( h.death, 0 );
	}
	if( deathH && birthH && deathH < birthH )
	{
		h.message = L"halála < születése";
		hiba = true;
	}

	// feleségek idõ-adatainak ellenõrzése
	for( UINT i = 0; i < vWifes.size(); ++i )
	{
		w = vWifes.at(i);
		birthW = 0;
		deathW = 0;
		marriage = 0;

		if( checkDate( w.birth ) )
		{
			w.birth = roundDate( w.birth );
			birthW	= theApp.getDateI( w.birth, 0 );
		}
		if( checkDate( w.death ) )
		{
			w.death = roundDate( w.death );
			deathW	= theApp.getDateI( w.death, 0 );
		}
		if( deathW && birthW && deathW < birthW )
		{
			vWifes.at(i).message = L"halála < születése";
			hiba = true;
		}


		if( checkDate( w.marriage ) )
		{
			w.marriage = roundDate( w.marriage );
			marriage	= theApp.getDateI( w.marriage, -10*12 );

			if( birthW && marriage < birthW )
			{
				vWifes.at(i).message = L" 10 éves kora elõtt házasodott";
				hiba = true;
			}
		}

		if( marriage && h.birth && marriage < birthH )
		{
			h.message += L" 10 éves kora elõtt házasodott";
			hiba = true;
		}
	}
	// gyerekek idõ adatainak ellenõrzése
	for( UINT i = 0; i < vChildren.size(); ++i )
	{
		c = vChildren.at(i);


		birthC = 0;
		deathC = 0;
		if( checkDate( c.birth ) )
		{
			c.birth = roundDate( c.birth );
			birthC	= theApp.getDateI( c.birth, 0 );
		}
		if( checkDate( c.death ) )
		{
			c.death = roundDate( c.death );
			deathC	= theApp.getDateI( c.death, 0 );
		}
		if( deathC && birthC && deathC < birthC )
		{
			vChildren.at(i).message = L"halála < születése";
			hiba = true;
		}
	}


	// gyerekek születési dátumának összehasonlítása anya születési dátumával és annak házasságkötási dátumával
	for( UINT i = 0; i < vChildren.size(); ++i )
	{
		c = vChildren.at(i);

		birthC = 0;
		deathC = 0;
		birthW = 0;
		deathW = 0;
		marriage = 0;
		if( checkDate( c.birth ) )
		{
			c.birth = roundDate( c.birth );
			birthC	= theApp.getDateI( c.birth, 0 );
		}
		if( checkDate( c.death ) )
		{
			c.death = roundDate( c.death );
			deathC	= theApp.getDateI( c.death, 0 );
		}

		mx = c.mother_index -1;

		// kritikus hibák, utána return;!!!
		if( mx + 1 > vWifes.size() )
		{
			vChildren.at(i).message.Format( L"Anya index nagyobb, mint %d", vWifes.size() );
			printFamily();
			break;
		}
		if( mx < 0 )
		{
			vChildren.at(i).message.Format( L"Anya index %d", mx+1 );
			printFamily();
			break;
		}

		// a mother indexxel kijelölt anyának dátumaival egyeztet
		w = vWifes.at(mx);
		if( checkDate( w.birth ) )
		{
			w.birth = roundDate( w.birth );
			birthW	= theApp.getDateI( w.birth, 0 );
		}
		if( checkDate( w.death ) )
		{
			w.death = roundDate( w.death );
			deathW	= theApp.getDateI( w.death, 0 );
		}
		if( checkDate( w.marriage ) )
		{
			w.marriage	= roundDate( w.marriage );
			marriage	= theApp.getDateI( w.marriage, 0 );
		}


		if( marriage && birthC && birthC < marriage )
		{
			vChildren.at(i).message += L" házasságkötés elõtt született";
			hiba = true;
		}

		// gyerek születési dátum és anya születési dátum + 16 év

		if( birthW )
		{
			birthW	= theApp.getDateI( w.birth, m_ageDiffM *12 );
			if( birthC && birthW && birthC < birthW )
			{
				age = _wtoi( c.birth.Left(4) ) - _wtoi( w.birth.Left(4) );
				str.Format( L" anyja %d éves korában született!", age ); 
				vChildren.at(i).message = str;
				hiba = true;
			}
		}

		// gyerek születési dátum és apa halálozási + 8 hónap

		if( deathH )
		{
			deathH	= theApp.getDateI( h.death, 9 );
			if( birthC && deathH < birthC )
			{
				str = L" apja halála után több mint 9 hónapra született"; 
				vChildren.at(i).message = str;
				hiba = true;
			}
		}
	}
	if( hiba )	printFamily();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::printFamily()
{
	WIFES		w;
	CHILDREN	c;

	fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s %s\n", h.linenumber, L"", h.numOfSpouses, h.rowid, h.name, h.birth, h.death, h.message );

	for( UINT j = 0; j < vWifes.size(); ++j )
	{
		w = vWifes.at(j);
		fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s %s\n", w.linenumber, w.marriage, w.motherOrder, w.rowid, w.name, w.birth, w.death, w.message );
	}
	for( UINT k = 0; k < vChildren.size(); ++k )
	{
		c = vChildren.at(k );
		fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s %s", c.linenumber, L"", c.mother_index, c.rowid, c.name, c.birth, c.death, c.message );
		fwprintf( textF, L"\n" );
	}
	fwprintf( textF, L"\n" );
	fflush( textF );


	str.Format( L"%d", m_cnt );
	push( str );
	push( h.linenumber );
	push( L"" );							// esküvõ
	push( L"" );							// index
	push( h.rowid );
	push( h.name );
	push( h.birth );
	push( h.death );
	push( h.message );

	for( UINT j = 0; j < vWifes.size(); ++j )
	{
		w = vWifes.at(j);
		push( L"" );							// cnt
		push( w.linenumber );
		push( w.marriage );
		str.Format( L"%d", w.motherOrder );
		push( str );
		push( w.rowid );
		push( w.name );
		push( w.birth );
		push( w.death );
		push( w.message );
	}
	for( UINT k = 0; k < vChildren.size(); ++k )
	{
		c = vChildren.at(k );
		push( L"" );							// cnt
		push( c.linenumber );
		push( L"" );							// marriage
		str.Format( L"%d", c.mother_index );
		push( str );
		push( c.rowid );
		push( c.name );
		push( c.birth );
		push( c.death );
		push( c.message );
	}
	emptyLine();
	++m_cnt;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::push( CString item )
{

	int tLen;
	TCHAR* sT;
	tLen	= item.GetLength()+1;
	sT		= new TCHAR[tLen];
	_tcscpy_s( sT, tLen, item.GetBuffer() );
	vList.push_back( sT );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::emptyLine()
{
	for( UINT i = 0; i < Q_NUMOFCOLUMNS; i++ ) push( L"" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	int index;
	UINT mask;

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
		mask	= 1 << nCol;
		if( _wtoi( vList.at( nItem * Q_NUMOFCOLUMNS + Q_LINENUMBER ))  )
		{
			index = _wtoi( vList.at( nItem * Q_NUMOFCOLUMNS + Q_INDEX ) );
//			if( index == 0) 
//				pLVCD->clrTextBk = YELLOW;
//			else
				pLVCD->clrTextBk = m_rgb[index];
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnInfo()
{
	CString info = L"\
Azon férfiak családját -feleségeit és gyerekeit- listázzuk, akiknek több felesége volt, és a dátumok elleenõrzése vaalmilyen \
problénára hívta fel a figyelmet.\r\n\
";



	CContractInfo dlg;
	dlg.m_title = L"Családok, amelyekben a férjnek több házassága volt, és a dátumok valamelyike ellentmondásos"; 
	dlg.m_info = info;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckFamilyDates:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


//	if(Menu.LoadMenu( IDR_DROPDOWN_HTML_P ))
	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnHtmlEdit()
{
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int			nItem;
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");

	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		vLines.push_back( m_ListCtrl.GetItemText( nItem, Q_LINENUMBER ) );
		if( name.Compare( m_ListCtrl.GetItemText( nItem, Q_NAME ) ) )
		{
			name = m_ListCtrl.GetItemText( nItem, Q_NAME );
			++cnt;
		}
	}

	CHtmlEditLines dlg;

	dlg.m_title = L"Kijelölt sorok a htm fájlban";  
	dlg.vLines = &vLines;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	Q_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnHtmlFamily()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	Q_ROWID );
	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s szülei és testvérei", m_ListCtrl.GetItemText( nItem, Q_NAME ) );
	dlg.m_type	= L"FATHERMOTHERHE";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	Q_ROWID );
	CRelations dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
