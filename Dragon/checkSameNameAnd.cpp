// TableSameBirth.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkSameNameAnd.h"
#include "afxdialogex.h"
#include "html_EditLines.h"
#include "Relations.h"
#include "CheckParam0.h"
#include "GetLastFirst.h"
#include "ProgressWnd.h"
#include "utilities.h"

// txt fájl oszlopok
//	fwprintf( theApp.fl, L"%d\t%s\t%s\t%s\t%s\t%s\t%s\t", m_cnt, rowid, lineNumber, tableNumber, source, united, namePrint );
//	fwprintf( theApp.fl, L"%s\t%s\t%s\t%s\t%s\t%s\t", birthDate, deathDate,fatherSource,fatherNamePrint,motherName2,spouses2 ); 

enum
{
	_CNT = 0,
	_ROWID,
	_LINENUMBER,
	_TABLENUMBER,
	_SOURCE,
	_UNITED,
	_GENERATION,
	_NAME,
	_BIRTH_DATE,
	_DEATH_DATE,
	_SF,
	_FATHER,
	_SM,
	_MOTHER,
	_SPOUSES,
	_LINENUMBERF,
	_LINENUMBERM,
	_COLUMNS
};


// ListCtrl oszlopok
enum
{
	L_CNT = 0,
	L_ROWID, 
	L_LINENUMBER,
	L_TABLENUMBER,
	L_SOURCE,
	L_UNITED,
	L_GENERATION,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_SF,
	L_ROWIDF,
	L_FATHER,
	L_SM,
	L_ROWIDM,
	L_MOTHER,
	L_SPOUSES,
	L_LINENUMBERF,
	L_LINENUMBERM,
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CcheckSameNameAnd, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CcheckSameNameAnd::CcheckSameNameAnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcheckSameNameAnd::IDD, pParent)
{
	
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;

	nItem = 0;
	_contractions = theApp.contractions();

	_extension = L"\
\r\n\
A másik lehetõség, hogy azonos emberek lehetnek, csak az összevonási eljárás adathiba miatt nem találta õket azonosnak.\r\n\r\n\
A listában felsoroljuk az azonosítási vizsgálatban felhasznált adatokat, születési és halálozási dátum, apa, anya, házastársak  \
neve, ami segíthet a probléma okának meghatározásban.\
\r\n\r\n\
Alább választhatunk, hogy az összes embert vizsgáljuk-e, vagy csak azokat, akiknek a családneve a megadandó karakter sorozattal kezdõdõdik.\
\r\n\
";
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CcheckSameNameAnd::~CcheckSameNameAnd()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CcheckSameNameAnd, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CcheckSameNameAnd::OnCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CcheckSameNameAnd::OnDblclkList)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_EDIT2LINES, &CcheckSameNameAnd::OnEdit2lines)
	ON_COMMAND(ID_HTML_EDIT, &CcheckSameNameAnd::OnHtmlShows)
	ON_COMMAND(ID_HTML_PEOPLEFATHER, &CcheckSameNameAnd::OnHtmlPeoplefather)
	ON_COMMAND(ID_HTML_NOTEPAD, &CcheckSameNameAnd::OnHtmlNotepad)

	ON_COMMAND(ID_HTML, &CcheckSameNameAnd::OnHtml)
	ON_COMMAND(ID_EDIT_NOTEPAD_PARENTS, &CcheckSameNameAnd::OnEditNotepadParents)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MOTHERANDSIBLING, &CcheckSameNameAnd::OnMotherandsibling)
	ON_COMMAND(ID_FATHERANDSIBLINGS, &CcheckSameNameAnd::OnFatherandsiblings)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckSameNameAnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();
	
	int iter = theApp.getUserVersion();
	CString attention = L"Ezt a mûveletet az azonos emberek összevonása után érdemes alkalmazni, hogy csak a gyanús eseteket tartalmazza.\nAkarod, hogy mégis elkészítsük a listát?";
	if( !iter )
	{
		if( AfxMessageBox( attention, MB_YESNO ) == IDNO )
		{
			OnCancel();
			return false;
		}
	}

	createColumns();
	CString caption( L"");

	_cnt = 0;
	if( !and.Compare( L"birth" ) )
	{
		SameNameAndBirth();
		caption = L"Azonos nevû és azonos születési idejû emberek";
	}
	else if( !and.Compare( L"death" ) )
	{
		SameNameAndDeath();
		caption = L"Azonos nevû és azonos halálozási idejû emberek";
	}
	else if( !and.Compare( L"father" ) )
	{
		SameNameAndFather();
		caption = L"Azonos nevû emberek, akiknek az apja is azonos";
	}
	else if( !and.Compare( L"mother" ) )
	{
		SameNameAndMother();
		caption = L"Azonos nevû emberek, akiknek az anyja is azonos";
	}
	if( !_cnt ) OnCancel();


	str.Format( L"%s (%s)", caption, theApp.m_databaseName );
	SetWindowTextW( str );

	
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// A Z O N O S   N E V Û   E M B E R   É S   A Z O N O S   A N Y A ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::SameNameAndMother()
{
	CString fileName( L"sameNameAndMother" );
	CString title;

	CString begin;

	CString caption = L"Azonos nevû emberek, akiknek az anyjuk neve is megegyezik.";
	CString info = L"\
Azon azonos nevû emberek listája készül el, akiknek az anyjuk neve is azonos, de nem testvérek.\r\n\
Valószínûtlen, hogy ilyen emberek lennének, bár nem kizárt. Az a valószínûbb, hogy adathiba van.\r\n\r\n\
Az egyik lehetõség, hogy az emberünk és/vagy annak anyja neve hibás.\
";
	CCheckParam0 dlg;

	dlg._caption	= caption;
	dlg._info		= info + _extension;
	if( dlg.DoModal() == IDCANCEL ) return;

	if( dlg._all )
	{
		title = L"Azonos nevû emberek, akiknek azonos nevû anyjuk van";
		m_command = L"SELECT rowid,* FROM people ORDER BY last_name, first_name, mother_id, source";
	}
	else
	{
		CGetLastFirst dlg1;
		if( dlg1.DoModal() == IDCANCEL ) return;
		_fullname	= dlg1._fullname;
		_firstname	= dlg1._firstname;
		_lastname	= dlg1._lastname;

		if( _firstname.IsEmpty() )
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, mother_id, source", _lastname,'%' );
		else
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name = '%s' AND first_name = '%s' ORDER BY last_name, first_name, mother_id, source", _lastname, _firstname );

		title.Format( L"'%s' nevû emberek, akiknek azonos nevû anyjuk van", _fullname );
		fileName += "_";
		fileName += _fullname;
	}

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	init( title );


	CString lastName;
	SAME same;
	sameClear( &same );
	vSame.clear();

// azonos nevû emberek indexeinek kigyûjtése a vSame vektorba
	CProgressWnd wndProgress(NULL, title ); 
	wndProgress.GoModal();
#ifndef _DEBUG
	wndProgress.SetText( L"Emberek beolvasása folyik..." );
#endif
	if( !query( m_command ) ) return;


	wndProgress.SetRange(0, m_recordset->RecordsCount());
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);
#ifndef _DEBUG
	wndProgress.SetText( L"Emberek azonosítása..." );
#endif
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i, m_recordset->MoveNext() )
	{
		if( !sameNameParent( PEOPLE_MOTHER_ID ) )
		{
			if( vSame.size() > 1 )
			{
				listSameVector();		// az eddig összegyûjtött azonos nevû emberek listázása, ha 1-nél több van
				++_cnt;
			}
			vSame.clear();
		}
		same.rowid = m_recordset->GetFieldString( PEOPLE_ROWID );
		same.index = i + 1;
		vSame.push_back( same );
		
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	if( vSame.size() > 1 )	listSameVector();
			vSame.clear();

	if( _cnt )
	{
		fwprintf( fh1, L"</pre>" );
		fclose( fh1 );
//		theApp.showHtmlFile( fileSpec );
	}
	else
	{
		fclose( fh1 );
		AfxMessageBox( L"Nem találtam azonos nevû embereket, akiknek az anyjuk neve is azonos lett volna." );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// A Z O N O S   N E V Û   E M B E R   É S   A Z O N O S   A P A  ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::SameNameAndFather()
{
	CString fileName( L"sameNameAndFather" );
	CString title;
	CString begin;

	CString caption = L"Azonos nevû emberek, akiknek az apjuk neve is megegyezik";

	CString info = L"\
Azon azonos nevû emberek listája készül el, akiknek az apjuk neve is azonos, de nem testvérek.\r\n\
Valószínûtlen, hogy ilyen emberek lennének, bár nem kizárt. Az a valószínûbb, hogy adathiba van.\r\n\r\n\
Az egyik lehetõség, hogy az emberünk és/vagy annak apja neve hibás.\
";
	
	CCheckParam0 dlg;

	dlg._caption	= caption;
	dlg._info		= info + _extension;
	if( dlg.DoModal() == IDCANCEL ) return;

	if( dlg._all )
	{
		title = L"Azonos nevû emberek, akiknek azonos nevû apjuk van";
		m_command = L"SELECT rowid,* FROM people ORDER BY last_name, first_name, father_id, source";
	}
	else
	{
		CGetLastFirst dlg1;
		if( dlg1.DoModal() == IDCANCEL ) return;
		_fullname	= dlg1._fullname;
		_firstname	= dlg1._firstname;
		_lastname	= dlg1._lastname;


		if( _firstname.IsEmpty() )
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, father_id, source", _lastname,'%' );
		else
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name = '%s' AND first_name = '%s' ORDER BY last_name, first_name, father_id, source", _lastname, _firstname );

		title.Format( L"'%s' nevû emberek, akiknek azonos apjuk van", _fullname );
		fileName += "_";
		fileName += _fullname;
	}

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	init( title );

	CString lastName;
	SAME same;
	sameClear( &same );
	vSame.clear();

// azonos nevû emberek indexeinek kigyûjtése a vSame vektorba
	CProgressWnd wndProgress(NULL, title ); 
	wndProgress.GoModal();
#ifndef _DEBUG
	wndProgress.SetText( L"Emberek beolvasása folyik..." );
#endif
	if( !query( m_command ) ) return;

	wndProgress.SetRange(0, m_recordset->RecordsCount());
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);
#ifndef _DEBUG
	wndProgress.SetText( L"Emberek azonosítása..." );
#endif

	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i, m_recordset->MoveNext() )
	{
		if( !sameNameParent( PEOPLE_FATHER_ID ) )
		{
			if( vSame.size() > 1 )	
			{
				listSameVector();		// az eddig összegyûjtött azonos nevû emberek listázása, ha 1-nél több van
				++_cnt;
			}
			vSame.clear();
		}
		same.rowid = m_recordset->GetFieldString( PEOPLE_ROWID );
		same.index = i + 1;
		vSame.push_back( same );
		
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	if( vSame.size() > 1 ) listSameVector();
		vSame.clear();

	if( _cnt )
	{
		fwprintf( fh1, L"</pre>" );
		fclose( fh1 );
	}
	else
	{
		fclose( fh1 );
		AfxMessageBox( L"Nem találtam azonos nevû embereket, akiknek az apjuk neve is azonos lett volna." );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// S A M E  N A M E   A N D   D E A T H   //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::SameNameAndDeath()
{
	CString fileName( L"sameNameAndDeath" );
	CString title;
	CString begin;

	CString caption = L"Azonos nevû és azonos halálozási idejû emberek";

	CString info = L"\
Azon azonos nevû emberek listája készül el,akiknek a halálozási idejük is azonos.\r\n\
Valószínûtlen, hogy ilyen emberek lennének, bár nem kizárt. Az a valószínûbb, hogy adathiba van.\r\n\r\n\
Az egyik lehetõség, hogy az emberünk neve és/vagy annak halálozási dátuma hibás.\
";

	CCheckParam0 dlg;


	dlg._caption	= caption;
	dlg._info		= info + _extension;
	if( dlg.DoModal() == IDCANCEL ) return;

	if( dlg._all )
	{
		title = L"Azonos nevû és halálozási dátumú emberek";
		m_command = L"SELECT rowid,* FROM people ORDER BY last_name, first_name, death_date, source";
	}
	else
	{
		CGetLastFirst dlg1;
		if( dlg1.DoModal() == IDCANCEL ) return;
		_fullname	= dlg1._fullname;
		_firstname	= dlg1._firstname;
		_lastname	= dlg1._lastname;

		if( _firstname.IsEmpty() )
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, death_date, source", _lastname,'%' );
		else
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name = '%s' AND first_name = '%s' ORDER BY last_name, first_name, death_date, source", _lastname, _firstname );

		title.Format( L"'%s' nevû emberek, akiknek azonos a halálozási dátuma", _fullname );
		fileName += "_";
		fileName += begin;
	}

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	init( title );

	CString lastName;
	SAME same;
	sameClear( &same );
	vSame.clear();

	CProgressWnd wndProgress(NULL, title ); 
	wndProgress.GoModal();
#ifndef _DEBUG
	wndProgress.SetText( L"Emberek beolvasása folyik..." );
#endif

	if( !query( m_command ) ) return;


	wndProgress.SetRange(0, m_recordset->RecordsCount());
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);
#ifndef _DEBUG
	wndProgress.SetText( L"Emberek azonosítása..." );
#endif

	// azonos nevû és halálozási dátumú emberek indexeinek kigyûjtése a vSame vektorba
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i, m_recordset->MoveNext() )
	{
		if( !sameNameDate( PEOPLE_DEATH_DATE ) )
		{
			if( vSame.size() > 1 )
			{
				listSameVector();		// az eddig összegyûjtött azonos nevû emberek listázása, ha 1-nél több van
				++_cnt;
			}
			vSame.clear();
		}
		same.rowid = m_recordset->GetFieldString( PEOPLE_ROWID );
		same.index = i + 1;
		vSame.push_back( same );

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	if( vSame.size() > 1 ) listSameVector();
	vSame.clear();

	if( _cnt )
	{
		fwprintf( fh1, L"</pre>" );
		fclose( fh1 );
	}
	else
	{
		fclose( fh1 );
		AfxMessageBox( L"Nem találtam azonos nevû és azonos halálozási dátumú embert." );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// A Z O N O S   N E V Û   É S   A Z O N O S   S Z Ü L E T É S I   I D E J Û   E M B E R E K //////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::SameNameAndBirth()
{
	CString fileName( L"sameNameAndBirth" );
	CString title;
	CString begin;
	CString caption = L"Azonos nevû és azonos születési idejû emberek";
	CString info = L"\
Azon azonos nevû emberek listája készül el, akiknek a születési idejük is azonos.\r\n\
Valószínûtlen, hogy ilyen emberek lennének, bár nem kizárt. Az a valószínûbb, hogy adathiba van.\r\n\r\n\
Az egyik lehetõség, hogy az emberünk neve és/vagy annak születési dátuma hibás.\
";
	
	CCheckParam0 dlg;

	dlg._caption	= caption;
	dlg._info		= info + _extension;
	if( dlg.DoModal() == IDCANCEL ) return;

	if( dlg._all )
	{
		title = L"Azonos nevû és születési dátumú emberek";
		m_command = L"SELECT rowid,* FROM people ORDER BY last_name, first_name, birth_date, source";
	}
	else
	{
		CGetLastFirst dlg1;
		if( dlg1.DoModal() == IDCANCEL ) OnCancel();
		_fullname	= dlg1._fullname;
		_firstname	= dlg1._firstname;
		_lastname	= dlg1._lastname;


		if( _firstname.IsEmpty() )
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, birth_date, source", _lastname,'%' );
		else
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name = '%s' AND first_name = '%s' ORDER BY last_name, first_name, birth_date, source", _lastname, _firstname );

		title.Format( L"'%s' nevû emberek, akiknek azonos a születési dátuma", _fullname );
		fileName += "_";
		fileName += _fullname;
	}

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	init( title );

	CString lastName;
	SAME same;
	sameClear( &same );
	vSame.clear();

// azonos nevû emberek indexeinek kigyûjtése a vSame vektorba
	CProgressWnd wndProgress(NULL, title ); 
	wndProgress.GoModal();
#ifndef _DEBUG
	wndProgress.SetText( L"Emberek beolvasása folyik..." );
#endif
	if( !query( m_command ) ) return;
	wndProgress.SetRange(0, m_recordset->RecordsCount());
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);
#ifndef _DEBUG
	wndProgress.SetText( L"Emberek azonosítása..." );
#endif


	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i, m_recordset->MoveNext() )
	{
		if( !sameNameDate( PEOPLE_BIRTH_DATE ) )
		{
			if( vSame.size() > 1 )
			{
				listSameVector();		// az eddig összegyûjtött azonos nevû emberek listázása, ha 1-nél több van
				++_cnt;
			}
			vSame.clear();
		}
		same.rowid = m_recordset->GetFieldString( PEOPLE_ROWID );
		same.index = i + 1;
		vSame.push_back( same );
		
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	if( vSame.size() > 1 )	listSameVector();
	vSame.clear();


	if( _cnt )
	{
		fwprintf( fh1, L"</pre>" );
		fclose( fh1 );
	}
	else
	{
		fclose( fh1 );
		AfxMessageBox( L"Nen találtam azonos nevû és azonos születési dátumú embert." );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::createColumns()
{

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SF,			L"s",			LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDF,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,		L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SM,			L"s",			LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDM,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,		L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SPOUSES,		L"házastársak",	LVCFMT_LEFT,	500,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_LINENUMBERF,	L"line#F",		LVCFMT_LEFT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINENUMBERM,	L"line#M",		LVCFMT_LEFT,	 60,-1,COL_HIDDEN );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	int iData;
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
		iData	= m_ListCtrl.GetItemData( nItem );
		if( iData & mask )
		{
			pLVCD->clrTextBk = RGB(255,255,0);
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnEditNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == - 1 )
	{
		theApp.message( L"", L"Nincs kijelölve ember!" );
		return;
	}
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	L_LINENUMBERF );
	if( !lineNumber.IsEmpty() ) theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CRelations dlg;
	int nItem		= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid	= m_ListCtrl.GetItemText( nItem, L_ROWID );

//	dlg.m_insert	= 0;
//	dlg.m_LCtrl		= &m_ListCtrl;
	dlg.nItem		= nItem;
	dlg.m_rowid		= rowid;
	if( dlg.DoModal() == IDCANCEL ) return;

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CcheckSameNameAnd:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML_P ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_LINE, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnEdit2lines()
{
	theApp.editHtmlLines( &m_ListCtrl, L_LINENUMBER );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == - 1 )
	{
		theApp.message( L"", L"Nincs kijelölve ember!" );
		return;
	}
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnHtmlShows()
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
		if( name.Compare( m_ListCtrl.GetItemText( nItem, L_NAME ) ) )
		{
			name = m_ListCtrl.GetItemText( nItem, L_NAME );
			++cnt;
		}
	

	}

	CHtmlEditLines dlg;

	if( !name.IsEmpty() )
	{
		str.Format( L"%s kijelölt sora a html fájlban", name ); 
		dlg.m_title = str;
	}
	else
		dlg.m_title = L"Kijelölt sorok a htm fájlban";
	dlg.vLines = &vLines;

	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnHtmlPeoplefather()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CHtmlEditLines dlg;
	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );
	dlg.DoModal();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckSameNameAnd::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckSameNameAnd::query1( CString command )
{
	if( m_recordset1->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckSameNameAnd::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckSameNameAnd::query3( CString command )
{
	if( m_recordset3->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckSameNameAnd::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::init( CString title )
{
	CString explanation( "\
Ezen lista elkészítésének célja, hogy hibákat keressünk az adatbázisban, amelyek elgépelés vagy az adatforrás hibája miatt \
keletkezhettek a GA.html fájlban. Azt használjuk ki, hogy egy ember többször szerepelhet a GA.html fájlban ( szerepüket egy \
kódszámmal jelöljük ).\
<br><br>\
Egy ember lehet:  \
<br><br>\
1 = leszármazott<br>\
2 = leszármazott házastársa<br>\
3 = leszármazott házastárásának apja<br>\
4 = leszármazott házastársának anyja<br>\
5 = leszármazott házastársának további házastársa\
<br><br>\
Egy ember különbözõ elõfordulásaiban megadott adatoknak illene azonosnak lenniük, de elõfordulhat, hogy gépelési hiba vagy az \
adatforrás hibája miatt ezek némelyike különbözik. Ha két embernek azonos a neve és még egy fontos adatukban megegyezik \
(születési idõ/halálozási idõ/apja neve/anyja neve), akkor joggal merül fel a gyanú, hogy ezek az emberek azonosak lehetnek, \
még akkor is, ha vannak eltérõ adataik is. Ezek ugyanis lehetnek hibásak. De a megegyezõ adatok is lehetnek hibásak, ezért \
az azonosság gyanúja is lehet megalapozatlan. Éppen azt kell eldöntenünk a lista alapján, hogy az egyezések vagy a különbségek \
hibásak vagy az eltérések nem is azért vannak, mert valamelyik hibás, hanem valóban különbözõ, rokon vagy névrokon emberekrõl \
van szó? Ha valamelyik adatról bebizonyosodik, hogy hibás, akkor azt ki kel javítani a html fájlban.\
<br><br>\
Ezt a mûveletet tanácsos az azonos emberek összevonása után elkészíteni, hogy az azonos emberek már ne terheljék a listát. \
Hiszen minket az eltérérések, a hibák érdekelnek. Egy adat hibájának, helyes értékének megállapítása embert próbáló feladat. \
Ez a lista csak felhívja a figyelmet arra, hogy mely adatoknak kell utána nézni.\
<br><br>\
A hibalistából kihagyjuk azokat az azonos nevû embereket akiknek bár az apja vagy anyja azonos volt, õk azonban testvérek voltak.\
<br><br>\
Az 'S' oszlopban az ember szerepkódja van, az 'U' oszlopban lévõ szám pedig megmutatja, hogy hány embert vont össze az \
gyesítési eljárás.\
<br><br>\
A Ga.html fájlban javítani kell a megállapított hibákat, majd ezt a fájlt újra beolvasni az adatbázisba és megismételni az \
azonos emberek összevonását. Ebbõl az adatbázisból készített hasonló lista, már nem fogja tartalmazni a javított tételeket.\
");


	fwprintf( fh1, L"<HEAD>\n" );
	fwprintf( fh1, L"<style>\n" );
	fwprintf( fh1, L"</style>\n" );
	fwprintf( fh1, L"</HEAD>\n" );
	fwprintf( fh1, L"<BODY>\n" );
	fwprintf( fh1, L"<font color='red'><center>%s</center><br><br></font>\n", title );
	fwprintf( fh1, explanation );
	
	fwprintf( fh1, L"<pre>" );
	fwprintf( fh1, L"%-25s %s<br>", L"Adatbázis", theApp.m_databaseSpec );
	fwprintf( fh1, L"%-25s %s<br>", L"lista készült", theApp.getPresentDateTime() );
	fwprintf( fh1, L"%-25s %d<br><br><br>", L"Elvégzett összevonások:", theApp.getUserVersion() );

	str.Format( L"\n<font color='red'>%12s %6s %6s %1s %1s %1s %-25s %-15s %-15s %s %-25s %s %-25s %s</font><br>\n", L"rowid", L"line#", L"table#", L"S", L"U",L"G",L"name", L"születés", L"halál", L"S", L"apja neve", L"s", L"anyja neve", L"házastársa" );
	fwprintf( fh1, str );


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::listSameVector()
{
	CString	sex_id;
	CString rowid;
	CString generation;
	CString source;
	CString united;
	CString lineNumber;
	CString tableNumber;
	CString name;
	CString birthDate;
	CString deathDate;
	CString rowidF;
	CString fatherName;
	CString rowidM;
	CString motherName;
	CString spouses;
	CString spouses2;
	CString	lineNumberF;
	CString lineNumberM;


	CString namePrint;
	CString fatherNamePrint;
	CString motherNamePrint;

	CString fatherSource;
	CString motherSource;

	CString birthDatePrev( L"" );
	CString deathDatePrev( L"" );
	CString fatherNamePrev( L"" );
	CString motherNamePrev( L"" );
	CString spousesPrev( L"" );
	CString spouses2Prev( L"");

	CString	lineNumberPrev;

	CString father_id;
	CString mother_id;
	CString spouseId;
	CString spouseName;
	SPOUSES2 sp;

	SPOUSES sNameId;

	int		cnt = 1;
	int		sameName = 1;



	int col;
	for( UINT i = 0; i < vSame.size(); ++i )
	{
		m_recordset->MoveTo( vSame.at(i).index );
	
		sex_id 		= m_recordset->GetFieldString( PEOPLE_SEX_ID );
		rowid		= m_recordset->GetFieldString( PEOPLE_ROWID );
		source		= m_recordset->GetFieldString( PEOPLE_SOURCE );
		united		= m_recordset->GetFieldString( PEOPLE_UNITED );
		generation	= m_recordset->GetFieldString( PEOPLE_GENERATION );

		lineNumber	= m_recordset->GetFieldString( PEOPLE_LINENUMBER );
		tableNumber	= m_recordset->GetFieldString( PEOPLE_TABLENUMBER );
		birthDate	= m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
		deathDate	= m_recordset->GetFieldString( PEOPLE_DEATH_DATE );
		father_id	= m_recordset->GetFieldString( PEOPLE_FATHER_ID );
		mother_id	= m_recordset->GetFieldString( PEOPLE_MOTHER_ID );
	
		name.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		name.Trim();
		if( name.GetLength() > 25 )
			namePrint = name.Left( 22 ) + L"..."; 
		else
			namePrint = name.Left(25);


		m_command.Format( L"SELECT lineNumber, first_name, last_name, source FROM people WHERE rowid = '%s'", father_id );
		if( !query2( m_command ) ) return;
		fatherName.Format( L"%s %s", m_recordset2->GetFieldString( 2 ), m_recordset2->GetFieldString( 1 ) );
		fatherName.Trim();
		if( fatherName.GetLength() > 25 )
			fatherNamePrint = fatherName.Left( 22 ) + L"..."; 
		else
			fatherNamePrint = fatherName.Left(25);
		fatherSource	= m_recordset2->GetFieldString(3);
		if( fatherSource.IsEmpty() ) fatherSource = " ";
		lineNumberF		= m_recordset2->GetFieldString(0);

		m_command.Format( L"SELECT lineNumber, first_name, last_name, source FROM people WHERE rowid = '%s'", mother_id );
		if( !query2( m_command ) ) return;
		motherName.Format( L"%s %s", m_recordset2->GetFieldString( 2 ), m_recordset2->GetFieldString( 1 ) );
		motherName.Trim();
		if( motherName.GetLength() > 25 )
			motherNamePrint = motherName.Left( 22 ) + L"..."; 
		else
			motherNamePrint = motherName.Left(25);
		motherSource	= m_recordset2->GetFieldString(3);
		if( motherSource.IsEmpty() ) motherSource = " ";
		lineNumberM		= m_recordset2->GetFieldString(0);
		motherName.Trim();


		spouses = theApp.getSpouses( rowid, sex_id, &theApp.v_spouses, &sp );
		spouses2 = sp.spouses2;

		
		fwprintf( fh1, L"%5d %6s %6s %6s %1s %1s %1s %-25s ", cnt, rowid, lineNumber, tableNumber, source, united, generation, namePrint );

		col = 0;
		if( i && birthDatePrev != birthDate && !birthDate.IsEmpty() && !birthDatePrev.IsEmpty() )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-15s</span> ", birthDate.Left(15) );
			col = col | 1 << L_BIRTH;
		}
		else
			fwprintf( fh1, L"%-15s ", birthDate.Left(25 ) );

		if( i && deathDatePrev != deathDate && !deathDate.IsEmpty() )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-15s</span> ", deathDate.Left(15) );
			col = col | 1 << L_DEATH;
		}
		else
			fwprintf( fh1, L"%-15s ", deathDate.Left(25 ) );

		if( i && fatherNamePrev != fatherName && !fatherNamePrint.IsEmpty() )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%s %-25s</span> ", fatherSource, fatherNamePrint );
			col = col | 1 << L_FATHER;
		}
		else
			fwprintf( fh1, L"%s %-25s ", fatherSource, fatherNamePrint );


		if( i && motherNamePrev != motherName && !motherNamePrint.IsEmpty() )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%s %-25s</span> ", motherSource, motherNamePrint );
			col = col | 1 << L_MOTHER;
		}
		else
			fwprintf( fh1, L"%s %-25s ", motherSource, motherNamePrint );

		if( i && !sameSpouses() )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%s</span>", spouses2 );
			col = col | 1 << L_SPOUSES;
		}
		else
			fwprintf( fh1, L"%s", spouses2 );
		
		fwprintf( fh1, L"\n" );
	

		
	//	str.Format( L"%d", sameName );

		str.Format( L"%d", cnt );
		nItem = m_ListCtrl.InsertItem( nItem, str );

		m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );
		m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, tableNumber );
		m_ListCtrl.SetItemText( nItem, L_UNITED, united );
		m_ListCtrl.SetItemText( nItem, L_SOURCE, source );
		m_ListCtrl.SetItemText( nItem, L_GENERATION, generation );
		m_ListCtrl.SetItemText( nItem, L_NAME, namePrint );

		m_ListCtrl.SetItemText( nItem, L_BIRTH, birthDate );
		m_ListCtrl.SetItemText( nItem, L_DEATH, deathDate );
		m_ListCtrl.SetItemText( nItem, L_SF, fatherSource );
		m_ListCtrl.SetItemText( nItem, L_ROWIDF, father_id );
		m_ListCtrl.SetItemText( nItem, L_FATHER, fatherNamePrint );
		m_ListCtrl.SetItemText( nItem, L_SM, motherSource );
		m_ListCtrl.SetItemText( nItem, L_ROWIDM, mother_id );
		m_ListCtrl.SetItemText( nItem, L_MOTHER, motherNamePrint );
		m_ListCtrl.SetItemText( nItem, L_SPOUSES,spouses2 );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBERF, lineNumberF );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBERM, lineNumberM );
		++cnt;

		m_ListCtrl.SetItemData( nItem, col );

		++nItem;

		birthDatePrev = birthDate;
		deathDatePrev = deathDate;
		fatherNamePrev = fatherName;
		motherNamePrev = motherName;

		theApp.v_spouses2.clear();
		for( UINT k=0; k < theApp.v_spouses.size(); ++ k )
		{
			sNameId.id		= theApp.v_spouses.at(k).id;
			sNameId.name	= theApp.v_spouses.at(k).name;
			sNameId.name2	= theApp.v_spouses.at(k).name2;
			theApp.v_spouses2.push_back( sNameId );
		}

		spousesPrev		= spouses;
		spouses2Prev	= spouses2;
		lineNumberPrev	= lineNumber;
	}
	nItem = m_ListCtrl.InsertItem( nItem, L"" );
	++nItem;
	++sameName;
	fwprintf( fh1, L"<br>" );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az aktuális és az azt követõ rekordban lévõ ember neve+valamelyik szûlõjének azonosítója azonos-e?
// Ha a név vagy a szülõ nincs megadva, akkor nem vizsgálja, nem tekinti azonosnak
// return TRUE, ha azons
// return FALSE, ha nem azonos
BOOL CcheckSameNameAnd::sameNameParent( int PARENT_ID )
{
	int		source1;
	int		source2;

	CString name;
	CString name1;
	CString name2;

	CString rowidParent;

	name.Format( L"%s%s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
	rowidParent = m_recordset->GetFieldString( PARENT_ID );

	if( !name.IsEmpty() && rowidParent != L"0" )
	{
		name1.Format( L"%s%s", name, rowidParent );
		source1 = _wtoi( m_recordset->GetFieldString( PEOPLE_SOURCE ) );

		m_recordset->MoveNext();
		name2.Format( L"%s%s%s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ), m_recordset->GetFieldString( PARENT_ID ) );
		source2 = _wtoi( m_recordset->GetFieldString( PEOPLE_SOURCE ) );
		m_recordset->MovePrevious();

		// Ha azonos nevû leszármazottak0 apja vagy anyja is azonos, akkor azok testvérek lehetnek, akik közül az egyik már meghalt.
		// Ezeket nem listázzuk.
		if( source1 == 1 && source2 == 1 ) return FALSE;

		if( !name1.Compare( name2 ) )
			return TRUE;  //  azonos nevû emberek
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az aktuális és az azt követõ rekordban lévõ ember neve+halálozási dátuma azonos-e?
// Ha a név vagy a halálozási dátum nincs megadva, akkor nem vizsgálja, nem tekinti azonosnak
// return TRUE, ha azons
// return FALSE, ha nem azonos
BOOL CcheckSameNameAnd::sameNameDate( int ID_DATE )
{
	CString name;
	CString name1;
	CString name2;
	
	CString date;
	

	name.Format( L"%s%s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
	date = m_recordset->GetFieldString( ID_DATE );

	if( !name.IsEmpty() && !date.IsEmpty() )
	{
		name1.Format( L"%s%s", name, date );

		m_recordset->MoveNext();
		name2.Format( L"%s%s%s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ), m_recordset->GetFieldString( ID_DATE ) );
		m_recordset->MovePrevious();

		if( !name1.Compare( name2 ) ) return TRUE;  //  azonos nevû emberek
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckSameNameAnd::sameSpouses()
{
	CString spouse1;
	CString spouse2;

	// ha van közös házastársuk, akkor azonos embernek vesszük

	if( theApp.v_spouses.size() == 0 || theApp.v_spouses2.size() == 0 ) return TRUE; // az azonosítás nem végezhetõ el, folytassuk

	for( UINT i = 0; i < theApp.v_spouses.size(); ++i )
	{
		spouse1 = theApp.v_spouses.at(i).name;
		for( UINT j = 0; j < theApp.v_spouses2.size(); ++j )
		{
			spouse2 = theApp.v_spouses2.at(j).name;
			if( !spouse1.Compare( spouse2 ) )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnHtml()
{
	theApp.showHtmlFile( fileSpec );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnMotherandsibling()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CHtmlEditLines dlg;
	dlg.m_type = L"SIBLINGS";

	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnFatherandsiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CHtmlEditLines dlg;

	str.Format( L"%s és gyermekei a GA.html listában", m_ListCtrl.GetItemText( nItem, L_FATHER ) );
	dlg.m_title = str;
	dlg.m_type = L"F_SIBLINGS";

	dlg.m_rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );
	dlg.DoModal();

}
