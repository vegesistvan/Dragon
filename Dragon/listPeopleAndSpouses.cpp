// CheckSameNamesSpouses.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "listPeopleAndSpouses.h"
#include "afxdialogex.h"
#include "CheckParam.h"
#include "GetLastFirst.h"
#include "html_Lines.h"
#include "Relations.h"

// ListCtrl oszlopok
enum
{
	L_FILE = 0,
	L_CNT,
	L_ROWID, 
	L_LINENUMBER,
	L_TABLENUMBER,
	L_GENERATION,
	L_SOURCE,
	L_UNITED,
	L_ORDER,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_SOURCEF,
	L_FATHER,
	L_SOURCEM,
	L_MOTHER,
	L_LINENUMBERF,
};

IMPLEMENT_DYNAMIC(CListPeopleAndSpouses, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListPeopleAndSpouses::CListPeopleAndSpouses(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListPeopleAndSpouses::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListPeopleAndSpouses::~CListPeopleAndSpouses()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CListPeopleAndSpouses, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CListPeopleAndSpouses::OnCustomdrawList)
	ON_COMMAND(ID_EDITNOTEPAD, &CListPeopleAndSpouses::OnEditnotepad)
//	ON_COMMAND(ID_EDIT_NOTPAD_PARENTS, &CListPeopleAndSpouses::OnEditNotpadParents)
	ON_COMMAND(ID_LIST_PEOPLE, &CListPeopleAndSpouses::OnListPeople)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CListPeopleAndSpouses::OnDblclkList)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CListPeopleAndSpouses::OnHtmlEdit)
	ON_COMMAND(ID_HTML_SHOWS, &CListPeopleAndSpouses::OnHtmlShows)
	ON_COMMAND(ID_HTML_PEOPLEFATHER, &CListPeopleAndSpouses::OnHtmlPeoplefather)
	ON_COMMAND(ID_HTML_NOTEPAD, &CListPeopleAndSpouses::OnHtmlNotepad)
	ON_COMMAND(ID_HTML, &CListPeopleAndSpouses::OnHtml)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CListPeopleAndSpouses::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();

	CString caption = L"Adott nevû emberek és azok házastársai";

	SetWindowTextW( caption );

	CString attention = L"Ezt a mûveletet az azonos emberek összevonása után érdemes alkalmazni, hogy csak a gyanús eseteket tartalmazza.\nAkarod, hogy mégis elkészítsük a listát?";
	if( !theApp._iterationCount )
	{
		if( AfxMessageBox( attention, MB_YESNO ) == IDNO )
		{
			OnCancel();
			return FALSE;
		}
	}


	CString info = L"\
Megadott nevû embereket és azok házastársait listázza a program  azonosító adataikkal együtt.\
\r\n\
Nem csak az aktuális adatbázisból, hanem az összevonás nélküli és minden létezõ összevonással keletkezõ adatbázisból \
listázza az embereket.\
\r\n\r\n\
Összevonás elõtt készült listán minden ember különbözõ embernek tekintendõ és házastársaik csak a saját html-sorában \
megadott házastársak.\
\r\n\
Az összevonás eredményeként minden összevont ember házastársait megkapja az eredményül kapott ember.\
\r\n\r\n\
Hibás adatok kibogozásához nyújt segítséget ez a funkció, ha hibát gyanítunk egy embernél. Összevonás elõtt és után \
elkészítve ezt a listát, könnyen áttekinthetjük, hogy hogy jöttek létre a hibásnak vélt házastársak. \
Ennek oka lehet a html fájl hibája.\
";

	CCheckParam dlg;

//	dlg.m_title	= caption;
//	dlg._info		= info;

//	if( dlg.DoModal() == IDCANCEL ) return FALSE;

	if( AfxMessageBox( info, MB_OKCANCEL|MB_ICONINFORMATION ) ==  IDCANCEL )
	{
		OnCancel();
		return FALSE;
	}
	CGetLastFirst dlg1;
	if( dlg1.DoModal() == IDCANCEL )
	{
		OnCancel();
		return FALSE;
	}

	_fullname	= dlg1._fullname;
	_firstname	= dlg1._firstname;
	_lastname	= dlg1._lastname;
	_sex_id		= dlg1._sex_id;

	_contractions = theApp.contractions();

	nItem = 0;

	createColumns();
	peopleMarriages();

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::peopleMarriages()
{

	CString order;
	CString people_id;
	CString spouse_id;
	CString people_id_prev;
	
	int sex_id;
	CString rowid;
	int sorsz;
	int	pos;
	int pos2;


	CString title;
	title.Format( L"'%s' nevû emberek és házastársaik", _fullname );

	CProgressWnd wndProgress(NULL, title ); 
	wndProgress.GoModal();
#ifndef _DEBUG
	str.Format( L"%s nevû emberek lekérdezése...", _fullname );
	wndProgress.SetText( str );
#endif

	CString fileName( L"peopleSpouses" );
	fileName += _contractions;

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	printHeader( title );


	int cnt = 0;
	theApp.getNumberOfDb( &vE );
	for( UINT idb = 0; idb < vE.size(); ++ idb )
	{
		theApp.m_databaseSpec = vE.at( idb );
		theApp.openDatabase();

		str = theApp.m_databaseSpec;
		if( ( pos = str.Find( '+' ) ) != -1 )
		{
			if( ( pos2 = str.ReverseFind( '.' ) ) != -1 )
			{
				m_fileId = str.Mid( pos, pos2-pos );
			}
		}
		else
			m_fileId.Empty();



#ifndef _DEBUG
	wndProgress.SetText( theApp.m_databaseSpec );
#endif

		fwprintf( fh1, L"<br><br>Database: %s<br><br>", theApp.m_databaseSpec );
		str.Format( L"<font color='blue'>%12s %6s %6s %1s %1s %1s %1s %-25s %-15s %-15s %-25s %-25s</font>", L"rowid", L"line#", L"table#", L"G", L"S", L"U", L"O", L"név", L"születés", L"halál", L"apja neve", L"anyja neve" );
		fwprintf( fh1, str );

		if( _firstname.IsEmpty( ) )
			m_command.Format( L"SELECT rowid,* FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name", _lastname,'%' );
		else
			m_command.Format( L"SELECT rowid,* FROM people WHERE last_name '%s' AND first_name='%s' ORDER BY last_name, first_name", _lastname, _firstname );
		if( !theApp.query( m_command ) ) return;

		
		sorsz = 1;
		cnt += theApp.m_recordset->RecordsCount();
		wndProgress.SetRange( 0, theApp.m_recordset->RecordsCount() );
		wndProgress.SetPos(0);
		wndProgress.SetStep( 1 );

		for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
		{
			sex_id	= _wtoi( theApp.m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
			rowid	= theApp.m_recordset->GetFieldString( PEOPLE_ROWID );

			if( sex_id == MAN )
				m_command.Format( L"SELECT spouse1_id, spouse2_id, orderWife FROM marriages WHERE spouse1_id ='%s'", rowid );
			else
				m_command.Format( L"SELECT spouse2_id, spouse1_id, orderHusband FROM marriages WHERE spouse2_id ='%s'", rowid );
			if( !theApp.query2( m_command ) ) return;

			for( UINT j = 0; j < theApp.m_recordset2->RecordsCount(); ++j, theApp.m_recordset2->MoveNext() )
			{
				spouse_id	= theApp.m_recordset2->GetFieldString( 1 );
				order		= theApp.m_recordset2->GetFieldString( 2 );

				if( j == 0 )
				{
					printPeople( rowid, order, TRUE, sorsz );
					++cnt;
					++sorsz;
				}
				printPeople( spouse_id, order, FALSE, cnt  );
			}
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
			if (wndProgress.Cancelled()) break;
		}
	}
	wndProgress.DestroyWindow();

	if( cnt )
	{
		fwprintf( fh1, L"</pre>" );
		fclose( fh1 );
//		theApp.showHtmlFile( fileSpec );
	}
	else
	{
		fclose( fh1 );
		AfxMessageBox( L"Nincsenek házastársak!" );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::printHeader( CString title )
{
	fwprintf( fh1, L"<HEAD>\n" );
	fwprintf( fh1, L"<style>\n" );
	fwprintf( fh1, L"</style>\n" );
	fwprintf( fh1, L"</HEAD>\n" );
	fwprintf( fh1, L"<BODY>\n" );
	fwprintf( fh1, L"<center>%s</center><br><br>\n\n", title );
	fwprintf( fh1, _explanation );
	fwprintf( fh1, L"<pre>" );
	fwprintf( fh1, L"<br>%s %s<br><br>", L"lista készült:", theApp.getPresentDateTime() );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::printPeople( CString rowid, CString order, BOOL people, int cnt  )
{
	CString name;

	CString lineNumber;
	CString tableNumber;
	CString generation;
	CString source;
	CString united;
	CString birth_date;
	CString death_date;
	CString father_id;
	CString mother_id;
	CString father;
	CString mother;

	CString sourceF;
	CString	sourceM;
	CString lineNumberF;

	CString fileId;
	int		col;


	m_command.Format( L"SELECT \
lineNumber,\
tableNumber,\
generation,\
source,\
united,\
birth_date,\
death_date,\
father_id,\
mother_id,\
last_name,\
first_name \
FROM people WHERE rowid = '%s'", rowid );
	if( !theApp.query1( m_command ) ) return;

	lineNumber	= theApp.m_recordset1->GetFieldString( 0 );
	tableNumber	= theApp.m_recordset1->GetFieldString( 1 );
	generation	= theApp.m_recordset1->GetFieldString( 2 );
	source		= theApp.m_recordset1->GetFieldString( 3 );
	united		= theApp.m_recordset1->GetFieldString( 4 );

	birth_date	= theApp.m_recordset1->GetFieldString( 5 );
	death_date	= theApp.m_recordset1->GetFieldString( 6 );
	father_id	= theApp.m_recordset1->GetFieldString( 7 );
	mother_id	= theApp.m_recordset1->GetFieldString( 8 );

	name.Format( L"%s %s", theApp.m_recordset1->GetFieldString( 9 ), theApp.m_recordset1->GetFieldString( 10 ) );


	m_command.Format( L"SELECT lineNumber, source, last_name, first_name FROM people WHERE rowid = '%s'", father_id );
	if( !theApp.query1( m_command ) ) return;
	lineNumberF	= theApp.m_recordset1->GetFieldString( 0 );
	sourceF		= theApp.m_recordset1->GetFieldString( 1 );
	father.Format( L"%s %s", theApp.m_recordset1->GetFieldString( 2 ), theApp.m_recordset1->GetFieldString( 3 ) );

	m_command.Format( L"SELECT source, last_name, first_name FROM people WHERE rowid = '%s'", mother_id );
	if( !theApp.query1( m_command ) ) return;
	sourceM		= theApp.m_recordset1->GetFieldString( 0 );
	mother.Format( L"%s %s", theApp.m_recordset1->GetFieldString( 1 ), theApp.m_recordset1->GetFieldString( 2 ) );

	if( people ) order.Empty();

	str.Format( L"\n%6s %6s %6s %1s %1s %1s %1s %-25s %-15s %-15s %-25s %-25s<br>\n", L"rowid", L"line#", L"table#", L"G", L"S", L"U", L"O", L"name", L"születés", L"halál", L"apja neve", L"anyja neve" );
	str.Format( L"%6s %6s %6s %1s %1s %1s %1s %-25s %-15s %-15s %-25s %-25s", rowid, lineNumber, tableNumber, generation, source, united, order, name, birth_date, death_date, father, mother );

	if( people )
		fwprintf( fh1, L"<br><font color='red'>%4d. %s</font><br>", cnt, str );
	else
		fwprintf( fh1, L"%5s %s<br>", L" ", str );

	if( people )
	{
		nItem = m_ListCtrl.InsertItem( nItem, L" " );
		++nItem;
	}
	nItem = m_ListCtrl.InsertItem( nItem, m_fileId );

	if( people )
		str.Format( L"%d", cnt );
	else
		str.Empty();
	m_ListCtrl.SetItemText( nItem, L_CNT,  str );
	m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );
	m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, tableNumber );
	m_ListCtrl.SetItemText( nItem, L_UNITED, united );
	m_ListCtrl.SetItemText( nItem, L_GENERATION, generation );
	m_ListCtrl.SetItemText( nItem, L_SOURCE, source );
	m_ListCtrl.SetItemText( nItem, L_ORDER, order );
	m_ListCtrl.SetItemText( nItem, L_NAME, name );

	m_ListCtrl.SetItemText( nItem, L_BIRTH, birth_date );
	m_ListCtrl.SetItemText( nItem, L_DEATH, death_date );
	m_ListCtrl.SetItemText( nItem, L_SOURCEF, sourceF );
	m_ListCtrl.SetItemText( nItem, L_FATHER, father );
	m_ListCtrl.SetItemText( nItem, L_SOURCEM, sourceM );
	m_ListCtrl.SetItemText( nItem, L_MOTHER, mother );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBERF,lineNumberF );

	col = 0;
	if( people )
		m_ListCtrl.SetItemData( nItem, 1 );
	else
		m_ListCtrl.SetItemData( nItem, 0 );
	++nItem;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::createColumns()
{
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_FILE,		L"fil",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ORDER,		L"O",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCEF,		L"s",			LVCFMT_LEFT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,		L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SOURCEM,		L"s",			LVCFMT_LEFT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,		L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LINENUMBERF,	L"line#F",		LVCFMT_RIGHT,	 60,-1,COL_NUM );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
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
		if( iData == 1 )
		{
			pLVCD->clrText	 = RGB( 255,0,0 );
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnEditnotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnEditNotpadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	L_LINENUMBERF );
	if( !lineNumber.IsEmpty() ) theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnListPeople()
{
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int			nItem;
	CString		lineNumber;
	std::vector<CString> vLines;

	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		lineNumber	= m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
		if( !lineNumber.IsEmpty() ) vLines.push_back( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) );
	}

	CHtmlLines dlg;

	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
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
LRESULT CListPeopleAndSpouses:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML_P ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_SHOWS, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnHtmlShows()
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

	CHtmlLines dlg;

	if( cnt == 1 )
		dlg.child = name;
	else
		dlg.child = L"";

	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnHtmlPeoplefather()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	CString lineNumberF	= m_ListCtrl.GetItemText( nItem, 	L_LINENUMBERF );
	if( lineNumberF.IsEmpty() )
	{
		AfxMessageBox( L"A kijelölt embernek nem ismerjük az apját!" );
		return;
	}


	std::vector<CString> vLines;

	vLines.push_back( lineNumberF );
	if( lineNumber != lineNumberF )	vLines.push_back( lineNumber );

	CHtmlLines dlg;
	dlg._what = 2;
	dlg.parents.Format( L"%s - %s",  m_ListCtrl.GetItemText( nItem,L_FATHER ), m_ListCtrl.GetItemText( nItem,L_MOTHER ) );
	dlg.child	= m_ListCtrl.GetItemText( nItem,L_NAME );
	dlg.vLines	= &vLines;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndSpouses::OnHtml()
{
	theApp.showHtmlFile( fileSpec );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////