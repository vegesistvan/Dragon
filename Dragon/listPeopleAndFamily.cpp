// TableNameFamily.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "listPeopleAndFamily.h"
#include "afxdialogex.h"
#include "html_Lines.h"
#include "Relations.h"
#include "checkParam1.h"
#include "GetLastFirst.h"

enum {
_ROWID = 0,
_LINENUMBER,
_TABLENUMBER,
_GENERATION,
_SOURCE,
_UNITED,
_SEX_ID,
_LAST_NAME,
_FIRST_NAME,
_BIRTH_DATE,
_DEATH_DATE,
_FATHER_ID,
_MOTHER_ID,
_COMMENT
};

/*
// txt fájl oszlopok
enum
{
	_CNT = 0,
	_WHO,
	_ROWID,
	_LINENUMBER,
	_TABLENUMBER,
	_GENERATION,
	_SOURCE,
	_UNITED,
	_NAME,
	_BIRTH_DATE,
	_DEATH_DATE,
	_COMMENT,
	_COLUMNS
};
*/

// ListCtrl oszlopok
enum
{
	L_FILE = 0,
	L_CNT,
	L_WHO,
	L_ROWID, 
	L_LINENUMBER,
	L_TABLENUMBER,
	L_GENERATION,
	L_SOURCE,
	L_UNITED,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_COMMENT
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CListPeopleAndFamily, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListPeopleAndFamily::CListPeopleAndFamily(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListPeopleAndFamily::IDD, pParent)
{
	peopleColumns =
"SELECT \
rowid,\
lineNumber,\
tableNumber,\
generation,\
source,\
united,\
sex_id,\
last_name,\
first_name,\
birth_date,\
death_date,\
father_id,\
mother_id, \
comment \
FROM people \
";

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListPeopleAndFamily::~CListPeopleAndFamily()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CListPeopleAndFamily, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CListPeopleAndFamily::OnCustomdrawList)

	ON_COMMAND(ID_EDITNOTEPAD, &CListPeopleAndFamily::OnEditnotepad)
	ON_COMMAND(ID_LIST_PEOPLE, &CListPeopleAndFamily::OnListPeople)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CListPeopleAndFamily::OnDblclkList)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CListPeopleAndFamily::OnHtmlEdit)
	ON_COMMAND(ID_HTML_SHOWS, &CListPeopleAndFamily::OnHtmlShows)
	ON_COMMAND(ID_HTML_NOTEPAD, &CListPeopleAndFamily::OnHtmlNotepad)
	ON_COMMAND(ID_HTML, &CListPeopleAndFamily::OnHtml)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CListPeopleAndFamily::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();

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
Magadott nevû embereket és azok közvetlen családtagjait listázza a program  azonosító adataikkal együtt.\
\r\n\
Nem csak az aktuális adatbázisból, hanem az összevonás nélküli és minden létezõ összevonással keletkezõ adatbázisból \
listázza az embereket.\
\r\n\r\n\
Piros színnel az emberünk és házastársaui, kék színnel a szülei és zöld színnel a gyermekeik vannak listázva.\
\r\n\r\n\
Hibás adatok kibogozásához nyújt segítséget ez a funkció, ha hibát gyanítunk egy embernél.\
\r\n\r\n\
Az azonos emberek összevonásának ciklusaiban nyomon követhetjük az összevonásokat.\
";


	if( AfxMessageBox( info, MB_ICONINFORMATION | MB_OKCANCEL ) == IDCANCEL )
	{
		OnCancel();
		return false;
	}

	CGetLastFirst dlg1;
	if( dlg1.DoModal() == IDCANCEL )
	{
		OnCancel();
		return FALSE;
	}


	fullname	= dlg1._fullname;
	first_name	= dlg1._firstname;
	last_name	= dlg1._lastname;
	sex			= dlg1._sex_id;

	nItem = 0;
	createColumns();
	nameParents();

	CString caption;
	caption.Format( L"'%s' nevû emberek és családjaik", fullname );
	SetWindowTextW( caption );

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::createColumns()
{
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_FILE,		L"file",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_WHO,			L"who",			LVCFMT_RIGHT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_COMMENT,		L"megjegyzés",	LVCFMT_LEFT,	400,-1,COL_TEXT);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::nameParents()
{
	int		cnt = 0;
	int		sex_id;
	int		pos;
	int		pos2;
	CString rowid;
	CString father_id;
	CString mother_id;
	CString spouse_id;


	CString title;
	title.Format( L"'%s' nevû emberek és családjaik", fullname );

	CProgressWnd wndProgress(NULL, title ); 
	wndProgress.GoModal();
#ifndef _DEBUG
	str.Format( L"%s nevû emberek lekérdezése...", fullname );
	wndProgress.SetText( str );
#endif

	CString fileName( L"nameFamily" );
	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );

	printHeader( title );

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
		str.Format( L"<font color='blue'>%14s %6s %6s %1s %1s %1s %-25s %-15s %-15s</font>", L"rowid", L"line#", L"table#", L"G", L"S", L"U", L"név", L"születés", L"halál" );
		fwprintf( fh1, str );

		if( first_name.IsEmpty() )
			m_command.Format( L"%s WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name", peopleColumns, last_name, '%' );
		else
			m_command.Format( L"%s WHERE last_name='%s' AND first_name='%s' ORDER BY last_name, first_name", peopleColumns, last_name, first_name );
		if( !theApp.query( m_command ) ) return;

		wndProgress.SetRange( 0, theApp.m_recordset->RecordsCount() );
		wndProgress.SetPos(0 );
		wndProgress.SetStep(1 );

		int sorsz = 1;
		for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
		{
			// emberünk
			rowid = theApp.m_recordset->GetFieldString( _ROWID );
			sex_id = _wtoi( theApp.m_recordset->GetFieldString( _SEX_ID ) );


			printPeople( L' ', sorsz );

			// apa
			father_id = theApp.m_recordset->GetFieldString( _FATHER_ID );
//			if( father_id != L"0" )
			{
				m_command.Format( L"%s WHERE rowid='%s' ORDER BY last_name, first_name", peopleColumns, father_id );
				if( !theApp.query1( m_command ) ) return;
				printPeople( 'F', sorsz );
			}

			// anya
			mother_id = theApp.m_recordset->GetFieldString( _MOTHER_ID );
//			if( mother_id != L"0" )
			{
				m_command.Format( L"%s WHERE rowid='%s' ORDER BY last_name, first_name", peopleColumns, mother_id );
				if( !theApp.query1( m_command ) ) return;
				printPeople( 'M', sorsz );
			}

			// házastársak
			if( sex_id == 1 )
				m_command.Format( L"SELECT spouse2_id, orderWife FROM marriages WHERE spouse1_id ='%s' ORDER BY spouse2_id", rowid );
			else
				m_command.Format( L"SELECT spouse1_id, orderHusband FROM marriages WHERE spouse2_id ='%s' ORDER BY spouse2_id", rowid );
			if( !theApp.query2( m_command ) ) return;

			for( UINT j = 0; j < theApp.m_recordset2->RecordsCount(); ++j, theApp.m_recordset2->MoveNext() )
			{
				spouse_id = theApp.m_recordset2->GetFieldString( 0 );
				m_command.Format( L"%s WHERE rowid = '%s' ORDER BY last_name, first_name", peopleColumns, spouse_id  );
				if( !theApp.query1( m_command ) ) return;
				printPeople( 'S', sorsz );
			}


			// gyerekek
			if( sex_id == MAN )
				m_command.Format( L"%s WHERE father_id='%s' ORDER BY last_name, first_name", peopleColumns, rowid );
			else
				m_command.Format( L"%s WHERE mother_id='%s' ORDER BY last_name, first_name", peopleColumns, rowid );
			if( !theApp.query1( m_command ) ) return;
			for( UINT j = 0; j < theApp.m_recordset1->RecordsCount(); ++j, theApp.m_recordset1->MoveNext() )
			{
				printPeople( 'C', sorsz );
			}

			++sorsz;
			++cnt;
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
		title.Format( L"Nincsenek '%s' nevû emberek", fullname );
		AfxMessageBox( title );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::printPeople( TCHAR who, int cnt  )
{
	CString whoS;
	CString cntS;

	CString name;

	CString rowid;
	CString lineNumber;
	CString tableNumber;
	CString generation;
	CString source;
	CString united;
	CString birth_date;
	CString death_date;
	CString comment;

	CString father_id;
	CString mother_id;
	CString sourceF;
	CString	sourceM;
	CString lineNumberF;

	int col;

	if( who == ' ' )
	{
		rowid		= theApp.m_recordset->GetFieldString( _ROWID );
		lineNumber	= theApp.m_recordset->GetFieldString( _LINENUMBER );
		tableNumber	= theApp.m_recordset->GetFieldString( _TABLENUMBER );
		generation	= theApp.m_recordset->GetFieldString( _GENERATION );
		source		= theApp.m_recordset->GetFieldString( _SOURCE );
		united		= theApp.m_recordset->GetFieldString( _UNITED );
		name.Format( L"%s %s", theApp.m_recordset->GetFieldString( _LAST_NAME ), theApp.m_recordset->GetFieldString( _FIRST_NAME ) );
		birth_date	= theApp.m_recordset->GetFieldString( _BIRTH_DATE );
		death_date	= theApp.m_recordset->GetFieldString( _DEATH_DATE );
		comment		= theApp.m_recordset->GetFieldString( _COMMENT );


	}
	else
	{
		rowid		= theApp.m_recordset1->GetFieldString( _ROWID );
		lineNumber	= theApp.m_recordset1->GetFieldString( _LINENUMBER );
		tableNumber	= theApp.m_recordset1->GetFieldString( _TABLENUMBER );
		generation	= theApp.m_recordset1->GetFieldString( _GENERATION );
		source		= theApp.m_recordset1->GetFieldString( _SOURCE );
		united		= theApp.m_recordset1->GetFieldString( _UNITED );
		name.Format( L"%s %s", theApp.m_recordset1->GetFieldString( _LAST_NAME ), theApp.m_recordset1->GetFieldString( _FIRST_NAME ) );
		birth_date	= theApp.m_recordset1->GetFieldString( _BIRTH_DATE );
		death_date	= theApp.m_recordset1->GetFieldString( _DEATH_DATE );
		comment		= theApp.m_recordset1->GetFieldString( _COMMENT );
	}

	str.Format( L"%c %6s %6s %6s %1s %1s %1s %-25s %-15s %-15s %s", who, rowid, lineNumber, tableNumber, generation, source, united, name, birth_date, death_date, comment );

	if( who == ' ' )
		fwprintf( fh1, L"<br><font color='red'>%4d. %s</font><br>", cnt, str );
	else if( who =='F' || who == 'M')
		fwprintf( fh1, L"<font color='blue'>%5s %s</font><br>", L" ", str );
	else if( who == 'S' )
		fwprintf( fh1, L"<font color='red'>%5s %s</font><br>", L" ", str );
	else if( who == 'C' )
		fwprintf( fh1, L"<font color='green'>%5s %s</font><br>",  L" ",str );


	whoS.Format( L"%c", who );
	if( who == ' ' )
		cntS.Format( L"%d",cnt );
	else
		cntS.Empty();


	if( who == ' ' )
	{
		nItem = m_ListCtrl.InsertItem( nItem, L" " );
		++nItem;
	}

	nItem = m_ListCtrl.InsertItem( nItem, m_fileId );
	m_ListCtrl.SetItemText( nItem, L_CNT,  cntS );
	m_ListCtrl.SetItemText( nItem, L_WHO, whoS );
	m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );
	m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, tableNumber );
	m_ListCtrl.SetItemText( nItem, L_UNITED, united );
	m_ListCtrl.SetItemText( nItem, L_GENERATION, generation );
	m_ListCtrl.SetItemText( nItem, L_SOURCE, source );
	m_ListCtrl.SetItemText( nItem, L_NAME, name );
	m_ListCtrl.SetItemText( nItem, L_BIRTH, birth_date );
	m_ListCtrl.SetItemText( nItem, L_DEATH, death_date );
	m_ListCtrl.SetItemText( nItem, L_COMMENT, comment );

	col = 0;
	if( who == ' ' )
		m_ListCtrl.SetItemData( nItem, 1 );
	else if( who == 'F' )
		m_ListCtrl.SetItemData( nItem, 2 );
	else if( who == 'M' )
		m_ListCtrl.SetItemData( nItem, 2 );
	else if( who == 'S' )
		m_ListCtrl.SetItemData( nItem, 1 );
	else if( who == 'C' )
		m_ListCtrl.SetItemData( nItem, 3 );

	++nItem;

//	str.Format( L"%d\t%c\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t", cnt, who, rowid, lineNumber, tableNumber, generation, source, united, name, birth_date, death_date, comment );
//	fwprintf( theApp.fl, L"%s\n", str );

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::printHeader( CString title )
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CListPeopleAndFamily::fillTable()
{
	int	nItem = 0;
	int fileLength;

	CStdioFile file( fileSpec, CFile::modeRead); 
	fileLength = (int)file.GetLength();

	CString			cLine;
	CStringArray	A;
	int				n;

	CString cntS;
	CString who;
	CString	rowid;
	CString	lineNumber;
	CString tableNumber;
	CString generation;
	CString source;
	CString united;
	CString name;
	CString birth;
	CString death;
	CString comment;

	int col;


	int		cnt = 1;
	int		cntNew;
	int		cntOld = 0;
	int		pos;
	int		pos2;

	CString fileId;

	while(file.ReadString(cLine)) 
	{
		col = 0;
		A.RemoveAll();
		
		if( cLine.IsEmpty() )
		{	
//			if( cnt != 1 )
			{
				nItem = m_ListCtrl.InsertItem( nItem, L"" );
				++nItem;
			}
			continue;
		}
		
		n = wordList( &A, cLine, '\t', TRUE );
//		if( n == 0 )
//		{
//			nItem = m_ListCtrl.InsertItem( nItem, L" " );
//			++nItem;
//			continue;
//		}

		if( n == 1 )  // adatbázis név szétszedés
		{
			if( cnt != 1 )
			{
				nItem = m_ListCtrl.InsertItem( nItem, L" " );
			}
			++nItem;
			str = A[0];
			if( ( pos = str.Find( '+' ) ) != -1 )
			{
				if( ( pos2 = str.ReverseFind( '.' ) ) != -1 )
				{
					fileId = str.Mid( pos, pos2-pos );
				}
			}
			else
				fileId.Empty();
			continue;
		}

		if( n != _COLUMNS )
		{
			str.Format( L"Oszlopok száma %d !=%d\n%s\n'%s'", n, _COLUMNS, cLine, A[n-1] );
			AfxMessageBox( str );
			return;
		}

		cntS = A[_CNT];

//		if( A[_CNT] != L" " && nItem != 0 )
//		{
//			nItem = m_ListCtrl.InsertItem( nItem, L" " );
//			++nItem;
//		}

		nItem = m_ListCtrl.InsertItem( nItem, fileId );
		cntNew = _wtoi( A[_CNT] );
		m_ListCtrl.SetItemText( nItem, L_CNT,  A[_CNT] );
		m_ListCtrl.SetItemText( nItem, L_WHO, A[_WHO] );
		m_ListCtrl.SetItemText( nItem, L_ROWID, A[_ROWID] );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBER, A[_LINENUMBER] );
		m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, A[_TABLENUMBER]);
		m_ListCtrl.SetItemText( nItem, L_UNITED, A[_UNITED] );
		m_ListCtrl.SetItemText( nItem, L_GENERATION, A[_GENERATION] );
		m_ListCtrl.SetItemText( nItem, L_SOURCE, A[_SOURCE] );
		m_ListCtrl.SetItemText( nItem, L_NAME, A[_NAME] );
		m_ListCtrl.SetItemText( nItem, L_BIRTH, A[_BIRTH_DATE] );
		m_ListCtrl.SetItemText( nItem, L_DEATH, A[_DEATH_DATE] );
		m_ListCtrl.SetItemText( nItem, L_COMMENT, A[_COMMENT] );

		col = 0;
		if( A[_WHO] == L" " )
			m_ListCtrl.SetItemData( nItem, 1 );
		else if( A[_WHO] == 'F' )
			m_ListCtrl.SetItemData( nItem, 2 );
		else if( A[_WHO] == 'M' )
			m_ListCtrl.SetItemData( nItem, 2 );
		else if( A[_WHO] == 'S' )
			m_ListCtrl.SetItemData( nItem, 1 );
		else if( A[_WHO] == 'C' )
			m_ListCtrl.SetItemData( nItem, 3 );

		++nItem;
		cntOld = cntNew;
	}
	file.Close();
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
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
		if( iData == 2 )
		{
			pLVCD->clrText	 = RGB( 0,0,255 );
		}
		if( iData == 3 )
		{
			pLVCD->clrText	 = RGB( 0,128,0 );
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::OnEditnotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::OnListPeople()
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
void CListPeopleAndFamily::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
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
LRESULT CListPeopleAndFamily:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
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
void CListPeopleAndFamily::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::OnHtmlShows()
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListPeopleAndFamily::OnHtml()
{
	theApp.showHtmlFile( fileSpec );
}
