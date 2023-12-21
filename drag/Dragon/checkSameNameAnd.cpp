// TableSameBirth.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkSameNameAnd.h"
#include "afxdialogex.h"
#include "editHtmlLines.h"
#include "Relatives.h"
#include "CheckParam0.h"
#include "GetLastFirst.h"
#include "ProgressWnd.h"
#include "utilities.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnSameNameAndBirth()
//{
//	CcheckSameNameAnd dlg;
//	dlg. and = L"birth";
//	dlg.DoModal();
//}
//void CDragonDlg::OnSameNameAndDeath()
//{
//	CcheckSameNameAnd dlg;
//	dlg. and = L"death";
//	dlg.DoModal();
//}
//void CDragonDlg::OnSameNameAndFather()
//{
//	CcheckSameNameAnd dlg;
//	dlg. and = L"father";
//	dlg.DoModal();
//}
//void CDragonDlg::OnSameNameAndMother()
//{
//	CcheckSameNameAnd dlg;
//	dlg. and = L"mother";
//	dlg.DoModal();
//}
//void CDragonDlg::OnSamenameandspouse()
//{
//	CcheckSameNameAnd dlg;
//	dlg. and = L"spouse";
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





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
	L_LINENUMBER,
	L_TABLENUMBER,
	L_UNITED,
	L_GENERATION,
	L_SOURCE,
	L_ROWID, 
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_SF,
	L_ROWIDF,
	L_FATHER,
	L_SM,
	L_ROWIDM,
	L_MOTHER,
	L_ROWIDS,
	L_SPOUSES,
	L_LINENUMBERF,
	L_LINENUMBERM,
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sameClear( SAME * same )
{
	same->index = 0;
	same->repleceBy.Empty();
	same->rowid.Empty();
	same->same = ' ';
	same->group = 0;
	same->identical = L"";
	same->timeConflict = L"";
}
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

	_lastname	= L"";
	_firstname	= L"";
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
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CcheckSameNameAnd, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CcheckSameNameAnd::OnCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CcheckSameNameAnd::OnDblclkList)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	

	ON_COMMAND(ID_HTML_EDIT, &CcheckSameNameAnd::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CcheckSameNameAnd::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CcheckSameNameAnd::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_DB_EDIT, &CcheckSameNameAnd::OnDbEdit)

	ON_STN_CLICKED(IDC_STATIC_KERESS, &CcheckSameNameAnd::OnClickedKeress)
	ON_STN_CLICKED(IDC_NEXT, &CcheckSameNameAnd::OnClickedNext)
	ON_COMMAND(ID_INFO, &CcheckSameNameAnd::OnInfo)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckSameNameAnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
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

	_fullname.Format( L"%s %s", _lastname, _firstname );
	_fullname.Trim();

	CString caption;

	if( !and.Compare( L"birth" ) )
	{
		if( _fullname.IsEmpty() )
			caption = L"Azonos nevû és azonos születési idejû emberek bejegyzései";
		else
			caption.Format( L"%s nevû emberek bejegyzései, akiknek azonos a születési idejük", _fullname );
	}
	else if( !and.Compare( L"death" ) )
	{
		if( _fullname.IsEmpty() )
			caption = L"Azonos nevû és azonos halálozási idejû emberek bejegyzései";
		else
			caption.Format( L"%s nevû emberek, akiknek azonos a halálozási idejük", _fullname );
	}
	else if( !and.Compare( L"father" ) )
	{
		if( _fullname.IsEmpty() )
			caption = L"Azonos nevû emberek bejegyzései, akiknek az apja neve is azonos";
		else
			caption.Format( L"%s nevû emberek bejegyzései, akiknek az apja neve is azonos", _fullname );
	}
	else if( !and.Compare( L"mother" ) )
	{
		if( _fullname.IsEmpty() )
			caption = L"Azonos nevû emberek bejegyzései, akiknek az anyja neve is azonos";
		else
			caption.Format( L"%s nevû emberek bejegyzései, akiknek az anyja neve is azonos", _fullname );
	}

	str.Format( L"%s (%s)", caption, L", de valamely fontos adatuk mégis különbözõ", theApp.m_dbFileName );
	SetWindowTextW( str );


	colorNext.SetTextColor( theApp.m_colorClick );
	colorKeres.SetTextColor( theApp.m_colorClick );

	createColumns();

	if( !and.Compare( L"birth" ) )
		SameNameAndBirth();
	else if( !and.Compare( L"death" ) )
		SameNameAndDeath();
	else if( !and.Compare( L"father" ) )
		SameNameAndFather();
	else if( !and.Compare( L"mother" ) )
		SameNameAndMother();
	if( !m_ListCtrl.GetItemCount() )
	{
		AfxMessageBox( L"Nem találtam a feltételeknek megfelelõ bejegyzéseket." );
		OnCancel();
	}
	return TRUE;
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

	if( !_lastname.IsEmpty() )
	{
		if( _firstname.IsEmpty() )
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, mother_id, source", _lastname,'%' );
		else 
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name = '%s' AND first_name = '%s' ORDER BY last_name, first_name, mother_id, source", _lastname, _firstname );
	}
	else
		m_command = L"SELECT rowid,* FROM people ORDER BY last_name, first_name, mother_id, source";

// azonos nevû emberek indexeinek kigyûjtése a vSame vektorba
	wndProgress.Create(NULL, L"Azonos nevû emberek..." ); 
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


	SAME same;
	sameClear( &same );
	vSame.clear();
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i, m_recordset->MoveNext() )
	{
		if( !sameNameParent( DBP_MOTHER_ID ) )
		{
			if( vSame.size() > 1 )
			{
				listSameVector();		// az eddig összegyûjtött azonos nevû emberek listázása, ha 1-nél több van
			}
			vSame.clear();
		}
		same.rowid = m_recordset->GetFieldString( DBP_ROWID );
		same.index = i + 1;
		vSame.push_back( same );
		
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	if( vSame.size() > 1 )
		listSameVector();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// A Z O N O S   N E V Û   E M B E R   É S   A Z O N O S   A P A  ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::SameNameAndFather()
{
	if( !_lastname.IsEmpty() )
	{
		if( _firstname.IsEmpty() )
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, father_id, source", _lastname,'%' );
		else 
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name = '%s' AND first_name = '%s' ORDER BY last_name, first_name, father_id, source", _lastname, _firstname );
	}
	else
		m_command = L"SELECT rowid,* FROM people ORDER BY last_name, first_name, father_id, source";

// azonos nevû emberek indexeinek kigyûjtése a vSame vektorba
	wndProgress.Create(NULL, L"Azonos nevû emberek..." ); 
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

	SAME same;
	sameClear( &same );
	vSame.clear();

	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i, m_recordset->MoveNext() )
	{
		if( !sameNameParent( DBP_FATHER_ID ) )
		{
			if( vSame.size() > 1 )	
			{
				listSameVector();		// az eddig összegyûjtött azonos nevû emberek listázása, ha 1-nél több van
			}
			vSame.clear();
		}
		same.rowid = m_recordset->GetFieldString( DBP_ROWID );
		same.index = i + 1;
		vSame.push_back( same );
		
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	if( vSame.size() > 1 )
		listSameVector();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// S A M E  N A M E   A N D   D E A T H   //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::SameNameAndDeath()
{
	if( !_lastname.IsEmpty() )
	{
		if( _firstname.IsEmpty() )
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, death_date, source", _lastname,'%' );
		else 
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name = '%s' AND first_name = '%s' ORDER BY last_name, first_name, death_date, source", _lastname, _firstname );
	}
	else
		m_command = L"SELECT rowid,* FROM people ORDER BY last_name, first_name, death_date, source";

// azonos nevû emberek indexeinek kigyûjtése a vSame vektorba
	wndProgress.Create(NULL, L"Azonos nevû emberek..." ); 
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


	SAME same;
	sameClear( &same );
	vSame.clear();

	// azonos nevû és halálozási dátumú emberek indexeinek kigyûjtése a vSame vektorba
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i, m_recordset->MoveNext() )
	{
		if( !sameNameDate( DBP_DEATH_DATE ) )
		{
			if( vSame.size() > 1 )
			{
				listSameVector();		// az eddig összegyûjtött azonos nevû emberek listázása, ha 1-nél több van
			}
			vSame.clear();
		}
		same.rowid = m_recordset->GetFieldString( DBP_ROWID );
		same.index = i + 1;
		vSame.push_back( same );

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	if( vSame.size() > 1 )
		listSameVector();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// A Z O N O S   N E V Û   É S   A Z O N O S   S Z Ü L E T É S I   I D E J Û   E M B E R E K //////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::SameNameAndBirth()
{
	if( !_lastname.IsEmpty() )
	{
		if( _firstname.IsEmpty() )
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, birth_date, source", _lastname,'%' );
		else 
			m_command.Format( L"SELECT rowid, * FROM people WHERE last_name = '%s' AND first_name = '%s' ORDER BY last_name, first_name, birth_date, source", _lastname, _firstname );
	}
	else
		m_command = L"SELECT rowid,* FROM people ORDER BY last_name, first_name, birth_date, source";

// azonos nevû emberek indexeinek kigyûjtése a vSame vektorba
	wndProgress.Create(NULL, L"Azonos nevû emberek..." ); 
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


	SAME same;
	sameClear( &same );
	vSame.clear();

	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i, m_recordset->MoveNext() )
	{
		if( !sameNameDate( DBP_BIRTH_DATE ) )
		{
			if( vSame.size() > 1 )
			{
				listSameVector();		// az eddig összegyûjtött azonos nevû emberek listázása, ha 1-nél több van
			}
			vSame.clear();
		}
		same.rowid = m_recordset->GetFieldString( DBP_ROWID );
		same.index = i + 1;
		vSame.push_back( same );
		
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
		wndProgress.DestroyWindow();
	if( vSame.size() > 1 )
		listSameVector();
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

	name.Format( L"%s%s", m_recordset->GetFieldString( DBP_LAST_NAME ), m_recordset->GetFieldString( DBP_FIRST_NAME ) );
	rowidParent = m_recordset->GetFieldString( PARENT_ID );

	if( !name.IsEmpty() && rowidParent != L"0" )
	{
		name1.Format( L"%s%s", name, rowidParent );
		source1 = _wtoi( m_recordset->GetFieldString( DBP_SOURCE ) );

		m_recordset->MoveNext();
		name2.Format( L"%s%s%s", m_recordset->GetFieldString( DBP_LAST_NAME ), m_recordset->GetFieldString( DBP_FIRST_NAME ), m_recordset->GetFieldString( PARENT_ID ) );
		source2 = _wtoi( m_recordset->GetFieldString( DBP_SOURCE ) );
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
	

	name.Format( L"%s%s", m_recordset->GetFieldString( DBP_LAST_NAME ), m_recordset->GetFieldString( DBP_FIRST_NAME ) );
	date = m_recordset->GetFieldString( ID_DATE );

	if( !name.IsEmpty() && !date.IsEmpty() )
	{
		name1.Format( L"%s%s", name, date );

		m_recordset->MoveNext();
		name2.Format( L"%s%s%s", m_recordset->GetFieldString( DBP_LAST_NAME ), m_recordset->GetFieldString( DBP_FIRST_NAME ), m_recordset->GetFieldString( ID_DATE ) );
		m_recordset->MovePrevious();

		if( !name1.Compare( name2 ) ) return TRUE;  //  azonos nevû emberek
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::createColumns()
{

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_SOURCE,		L"s",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SF,			L"s",			LVCFMT_LEFT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDF,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,		L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SM,			L"s",			LVCFMT_LEFT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDM,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,		L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ROWIDS,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SPOUSES,		L"házastársak",	LVCFMT_LEFT,	500,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_LINENUMBERF,	L"line#F",		LVCFMT_LEFT,	 60,-1,COL_HIDDEN );
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
	if( !lineNumber.IsEmpty() ) theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CRelatives dlg;
	int nItem		= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid	= m_ListCtrl.GetItemText( nItem, L_ROWID );

//	dlg.m_insert	= 0;
//	dlg.m_LCtrl		= &m_ListCtrl;
//	dlg.nItem		= nItem;
	dlg.m_rowid		= rowid;
	if( dlg.DoModal() == IDCANCEL ) return;

	*pResult = 0;
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
/*
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
	fwprintf( fh1, L"%-25s %s<br>", L"Adatbázis", theApp.m_dbPathName );
	fwprintf( fh1, L"%-25s %s<br>", L"lista készült", theApp.getPresentDateTime() );
	fwprintf( fh1, L"%-25s %d<br><br><br>", L"Elvégzett összevonások:", theApp.getUserVersion() );

	str.Format( L"\n<font color='red'>%12s %6s %6s %1s %1s %1s %-25s %-15s %-15s %s %-25s %s %-25s %s</font><br>\n", L"rowid", L"line#", L"table#", L"S", L"U",L"G",L"name", L"születés", L"halál", L"S", L"apja neve", L"s", L"anyja neve", L"házastársa" );
	fwprintf( fh1, str );


}
*/
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
	CString fatherNameP(L"");
	CString motherNamePrev( L"" );
	CString motherNameP(L"");
	CString spousesPrev( L"" );
	CString spouses2Prev( L"");

	CString	lineNumberPrev;

	CString birth;
	CString death;

	CString birthF;
	CString deathF;

	CString birthM;
	CString deathM;

	CString birthFP(L"");
	CString deathFP(L"");;

	CString birthMP;
	CString deathMP;


	CString father_id;
	CString mother_id;
	CString spouseId;
	CString spouseName;
	SPOUSES2 sp;

	SPOUSES sNameId;

	int		cnt = 1;
	int		sameName = 1;

	int nSame;
	int bSame;
	int dSame;

	int col;
	for( UINT i = 0; i < vSame.size(); ++i )
	{
		m_recordset->MoveTo( vSame.at(i).index );
	
		sex_id 		= m_recordset->GetFieldString( DBP_SEX_ID );
		rowid		= m_recordset->GetFieldString( DBP_ROWID );
		source		= m_recordset->GetFieldString( DBP_SOURCE );
		united		= m_recordset->GetFieldString( DBP_UNITED );
		generation	= m_recordset->GetFieldString( DBP_GENERATION );

		lineNumber	= m_recordset->GetFieldString( DBP_LINENUMBER );
		tableNumber	= m_recordset->GetFieldString( DBP_TABLENUMBER );
		birthDate	= m_recordset->GetFieldString( DBP_BIRTH_DATE );
		deathDate	= m_recordset->GetFieldString( DBP_DEATH_DATE );
		father_id	= m_recordset->GetFieldString( DBP_FATHER_ID );
		mother_id	= m_recordset->GetFieldString( DBP_MOTHER_ID );
	
		name.Format( L"%s %s", m_recordset->GetFieldString( DBP_LAST_NAME ), m_recordset->GetFieldString( DBP_FIRST_NAME ) );
		name.Trim();
		if( name.GetLength() > 25 )
			namePrint = name.Left( 22 ) + L"..."; 
		else
			namePrint = name.Left(25);


		m_command.Format( L"SELECT lineNumber, first_name, last_name, source, birth_date, death_date FROM people WHERE rowid = '%s'", father_id );
		if( !query2( m_command ) ) return;

		fatherName.Format( L"%s %s", m_recordset2->GetFieldString( 2 ), m_recordset2->GetFieldString( 1 ) );
		fatherName.Trim();
		birthF = m_recordset2->GetFieldString( 4 );
		deathF = m_recordset2->GetFieldString( 5 );
		fatherNamePrint = getNameBD( fatherName, birthF, deathF, L"" );

		fatherSource	= m_recordset2->GetFieldString(3);
		if( fatherSource.IsEmpty() ) fatherSource = " ";
		lineNumberF		= m_recordset2->GetFieldString(0);

		m_command.Format( L"SELECT lineNumber, first_name, last_name, source, birth_date, death_date FROM people WHERE rowid = '%s'", mother_id );
		if( !query2( m_command ) ) return;

		motherName.Format( L"%s %s", m_recordset2->GetFieldString( 2 ), m_recordset2->GetFieldString( 1 ) );
		motherName.Trim();
		birthM = m_recordset2->GetFieldString( 4 );
		deathM = m_recordset2->GetFieldString( 5 );
		motherNamePrint = getNameBD( motherName, birthM, deathM, L"" );

		motherSource	= m_recordset2->GetFieldString(3);
		if( motherSource.IsEmpty() ) motherSource = " ";
		lineNumberM		= m_recordset2->GetFieldString(0);

		spouses = theApp.getSpouses( rowid, sex_id, &theApp.v_spouses, &sp );
		spouses2 = sp.spouses2;

		if( theApp.v_spouses.size() )
			spouseId = theApp.v_spouses.at(0).id;
		else
			spouseId.Empty();
		col = 0;
		if( i )
		{
			if( birthDatePrev != birthDate && !birthDate.IsEmpty() && !birthDatePrev.IsEmpty() )
				col = col | 1 << L_BIRTH;

			if( deathDatePrev != deathDate && !deathDate.IsEmpty() && !deathDatePrev.IsEmpty() )
				col = col | 1 << L_DEATH;

			nSame = same( fatherName, fatherNameP );
			bSame = same( birthF, birthFP );
			dSame = same( deathF, deathFP );

			if( nSame == -1 || bSame == -1 || dSame == -1  )
				col = col | 1 << L_FATHER;

			nSame = same( motherName, motherNameP );
			bSame = same( birthM, birthMP );
			dSame = same( deathM, deathMP );

			if( nSame == -1 || bSame == -1 || dSame == -1  )
				col = col | 1 << L_MOTHER;
		
			if( spouses2 != spouses2Prev )
				col = col | 1 << L_SPOUSES;

			/*
			if( i && !sameSpouses() )
				col = col | 1 << L_SPOUSES;
				*/
		}

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
		m_ListCtrl.SetItemText( nItem, L_ROWIDS, spouseId );
		m_ListCtrl.SetItemText( nItem, L_SPOUSES,spouses2 );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBERF, lineNumberF );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBERM, lineNumberM );
		++cnt;

		m_ListCtrl.SetItemData( nItem, col );

		++nItem;

		birthDatePrev	= birthDate;
		deathDatePrev	= deathDate;
		birthFP			= birthF;
		deathFP			= deathF;
		birthMP			= birthM;
		deathMP			= deathM;

		fatherNamePrev	= fatherNamePrint;
		motherNamePrev	= motherNamePrint;
		motherNameP		= motherName;
		fatherNameP		= fatherName;


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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CcheckSameNameAnd:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijelölt sor a ga.html fájlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CEditHtmlLines dlg;
	dlg.m_title.Format( L"%s szülei és testvérei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckSameNameAnd::PreTranslateMessage(MSG* pMsg)
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
void CcheckSameNameAnd::OnClickedKeress()
{
	keress( 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress( nItem + 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, L_NAME, start, true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckSameNameAnd::OnInfo()
{
	CString info( "\
Az azonos emberek bejegyzéseinek összevonása után maradhatnak az adatbázisban olyan bejegyzések, amelyekben a név azonos, sõt \
van még további megegyezõ adat, pl. születési dátum, halálozási dátum, apja neve/születése/halála, anyja neve/születése/halála, \
házastárs neve/születése/halála, de ellentmondó adatok is találhatók a bejegyzésekben, ezért nem kerültek összevonásra.\
\r\n\r\n\
Joggal merül fel a gyanú, hogy ilyenkor az adatbevitel vagy adatforrás hibája miatt vannak ellentmondó adatok. \
De a megegyezõ adatok is lehetnek hibásak, ezért az azonosság gyanúja is lehet megalapozatlan.\
\r\n\r\n\
Azt kell eldöntenünk a lista alapján, hogy az egyezések vagy a különbségek hibásak vagy az eltérések nem is azért vannak, \
mert valamelyik hibás, hanem valóban különbözõ, rokon vagy névrokon emberekrõl van szó? \
\r\n\r\n\
Ezt alistát tanácsos az azonos emberek összevonása után elkészíteni, hogy a valóban azonos emberek különbözõ bejegyzései már \
ne terheljék a listát. Hiszen minket az eltérérések, a hibák érdekelnek. Egy adat hibájának, helyes értékének megállapítása embert próbáló feladat. \
Ez a lista csak felhívja a figyelmet arra, hogy mely adatoknak kell utána nézni.\
\r\n\r\n\
A hibalistából kihagyjuk azokat az azonos nevû embereket akiknek bár az apja vagy anyja azonos volt, õk azonban testvérek voltak.\
\r\n\r\n\
Az 'S' oszlopban az ember szerepkódja van, az 'U' oszlopban lévõ szám pedig megmutatja, hogy hány embert vont össze az \
egyesítési eljárás.\
\r\n\r\n\
A Ga.html fájlban javítani kell a megállapított hibákat, majd ezt a fájlt újra beolvasni az adatbázisba és megismételni az \
azonos emberek összevonását. Ebbõl az adatbázisból készített hasonló lista, már nem fogja tartalmazni a javított tételeket.\
\r\n\r\n\
A hibák javításához az érintett sor/sorok kijelölése után a bal egérgomb klikkelésére legördülõ menübõl választható \
funkciók nyújtanak segítséget.\
");
	theApp.m_pszAppName =_tcsdup( L"Bejegyzések, amelyekben a név és valami más azonos" );
	AfxMessageBox( info, MB_ICONINFORMATION );
}
