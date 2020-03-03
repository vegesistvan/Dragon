// TableSameSpouse.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "checkSameSpouses.h"
#include "afxdialogex.h"
#include "html_Lines.h"
#include "Relations.h"
#include "CheckParam0.h"
#include <algorithm>
#include "GetLastFirst.h"

// txt fájl oszlopok
enum
{
	_CNT = 0,
	_ROWID,
	_LINENUMBER,
	_TABLENUMBER,
	_GENERATION,
	_SOURCE,
	_UNITED,
	_NAME,
	_BIRTH_DATE,
	_DEATH_DATE,
	_SOURCEF,
	_FATHER,
	_SOURCEM,
	_MOTHER,
	_LINENUMBERF,
	_LINENUMBERM,
	_COLUMNS
};

bool sortBySpouse(const MORESPOUSES &v1, const MORESPOUSES &v2);
bool sortBySource(const MORESPOUSES &v1, const MORESPOUSES &v2);

enum
{
	P_ROWID = 0,
	P_FIRST_NAME,
	P_LAST_NAME,
	P_SEX_ID,
	P_LINENUMBER,
	P_TABLENUMBER,
	P_GENERATION,
	P_SOURCE,
	P_BIRTH_DATE,
	P_DEATH_DATE,
	P_FATHER_ID,
	P_MOTHER_ID,
	P_UNITED,
};


// ListCtrl oszlopok
enum
{
	L_CNT = 0,
	L_ROWID, 
	L_LINENUMBER,
	L_TABLENUMBER,
	L_GENERATION,
	L_SOURCE,
	L_UNITED,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_SOURCEF,
	L_FATHER,
	L_BIRTH_F,
	L_DEATH_F,
	L_SOURCEM,
	L_MOTHER,
	L_BIRTH_M,
	L_DEATH_M,
	L_LINENUMBERF,
};

// CCheckSameSpouses dialog
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CCheckSameSpouses, CDialogEx)
CCheckSameSpouses::CCheckSameSpouses(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckSameSpouses::IDD, pParent)
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;

	m_explanation = L"Azokat az embereket listázzuk, akiknek több azonos nevû házastársa van. Egy emberenek természetesen lehet két \
vagy akár több azonos nevû házastársa, sokszor azonban hibás adatokra hívja fel a figyelmet.\
<br>\
Emberünket az azonos nevû házastársai követik születési, halálozási dátumukkal valamint apjuk, anyjuk nevével, \
sárgával kiemelve az eltérõ azonosítási kritériumot.\
<br>\
Ha az azonos emberek összevonása elõtt készítjük el a listát, akkor csak olyankor fordulhat elõ több azonos nevû házaspár, \
ha azok egyetlen leszármazotti sorban vannak megadva házastársként.\
<br>\
Az azonos emberek összevonása megsokszorozhatja az ilyen eseteket, ugyanis ha emberünk azonossá lett egy vagy több más azonos \
nevû emberrel, akkor ezek összevonásra kerültek, de azonos nevû házastársaik nem feltétlenül teljesítik az összevonási kritériumot, \
ezért õk bár azonos nevûek, mégis különbözõ emberként maradnak az adatbázisban.\
<br><br>\
Az 'S' oszlopban az ember szerepkódja van, az 'U' oszlopban lévõ szám pedig megmutatja, hogy hány embert vont össze az \
egyesítési eljárás.\
<br><br>\
A besárgított házaspárt nem egyesíti, mert annak adatai eltérnek a másik azonos nevûházaspárétól.<br><br>\
Az azonos nevû házaspárok összevonását is többször elvégezhetjük, míg azt nem jelzi, hogy nem talált azonos nevû házaspárokat.\
<br><br>\
Egy embernek 3 okból lehet azonos nevû házastársa:\
<br><br>\
1. Valóban volt azonos nevû házastársa. Ebben az esetben várható, hogy a házastárs személyes adatai különböznek.\
<br>\
2. Valójában nem volt azonos nevû házastársa, csak a html fájlban és az adatbázisban hibásan szerepelt valamelyik házastárs.\
<br>\
3. Valójában nem volt azonos nevû házastársa, csak az azonos emerek összevonása során valami hiba történ.\
<br>\
";

	_contractions = theApp.contractions();

	p_fields = L"\
rowid,\
first_name,\
last_name,\
sex_id,\
lineNumber,\
tableNumber,\
generation,\
source,\
birth_date,\
death_date,\
father_id,\
mother_id,\
united\
";


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckSameSpouses::~CCheckSameSpouses()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckSameSpouses, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckSameSpouses::OnCustomdrawList)
	ON_COMMAND(ID_EDITNOTEPAD, &CCheckSameSpouses::OnEditnotepad)
	ON_COMMAND(ID_EDIT_NOTPAD_PARENTS, &CCheckSameSpouses::OnEditNotpadParents)
	ON_COMMAND(ID_LIST_PEOPLE, &CCheckSameSpouses::OnListPeople)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCheckSameSpouses::OnDblclkList)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)

	ON_COMMAND(ID_HTML_EDIT, &CCheckSameSpouses::OnHtmlEdit)
	ON_COMMAND(ID_HTML_SHOWS, &CCheckSameSpouses::OnHtmlShows)
	ON_COMMAND(ID_HTML_PEOPLEFATHER, &CCheckSameSpouses::OnHtmlPeoplefather)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckSameSpouses::OnHtmlNotepad)

	ON_COMMAND(ID_HTML, &CCheckSameSpouses::OnHtml)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckSameSpouses::OnInitDialog()
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
			return false;
		}
	}

	CString info;
	info = L"\
Azokat az embereket listázzuk, akiknek több azonos nevû házastársa van. Egy embernek természetesen lehet két \
vagy akár több azonos nevû házastársa, azonban legtöbbször hibás adatokra hívja fel a figyelmet.\
\r\n\
Két vagy több azonos nevû házastárs személyes adatai közötti különbségeket sárga színnel emeli ki a program.\
\r\n\r\n\
Ha az azonos emberek összevonása elõtt készítjük el a listát, akkor csak olyankor fordulhat elõ több azonos nevû házaspár, \
ha azok egyetlen leszármazotti sorban vannak megadva házastársként.\
\r\n\
Az azonos emberek összevonása megsokszorozhatja az ilyen eseteket, ugyanis ha emberünk azonossá lett egy vagy több más azonos \
nevû emberrel, akkor ezek összevonásra kerültek, de azonos nevû házastársaik nem feltétlenül teljesítik az összevonási \
kritériumot, ezért õk bár azonos nevûek, mégis különbözõ emberként maradnak az adatbázisban.\
\r\n\
";

	CCheckParam0 dlg;
	dlg._caption = L"Azonos nevû házastársakkal rendelkezõ emberek listája";
	dlg._info = info;
	if( dlg.DoModal() == IDCANCEL )
	{
		OnCancel();
		return TRUE;
	}

	_fullname.Empty();
	if( !dlg._all )
	{
		CGetLastFirst dlg1;
		if( dlg1.DoModal() == IDCANCEL )
		{
			OnCancel();
			return false;
		}

		_fullname	= dlg1._fullname;
		_first_name	= dlg1._firstname;
		_last_name	= dlg1._lastname;
	}
	
	CString caption( L"");
	if( _fullname.IsEmpty() )
	{
		caption = L"Emberek. akiknek több azonos nevû házastársa van.";
	}
	else
		caption.Format( L"%s nevû emberek, akinknek több azonos nevû házastársa van.", _fullname );

	SetWindowTextW( caption ),

	createColumns();
	nItem = 0;
	m_cnt = 0;
	sameSpouses();

	if( !m_cnt ) OnCancel();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::createColumns()
{

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCEF,		L"s",			LVCFMT_LEFT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,		L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_F,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH_F,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCEM,		L"s",			LVCFMT_LEFT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,		L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_M,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH_M,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBERF,	L"line#F",		LVCFMT_RIGHT,	 60,-1,COL_HIDDEN );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::sameSpouses()
{
	CString fileName;
	CString first_name1;
	CString title( L"Emberek, akiknek több azonos nevû házastársuk van..." ); 

	CString	sex_id;
	CString spouse2;
	CString rowidS;
	CString lastNameS;
	CString firstNameS;
	CString fatherId;
	CString motherId;


	UINT i;

	MORESPOUSES vspouse;

	CProgressWnd wndP(NULL, title ); 
	wndP.GoModal();
	fileName += L"peopleHaveSameSpouses";
	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	htmlHeader( title );

#ifndef _DEBUG
	str.Format( L"Emberek lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif
	if( _fullname.IsEmpty() )
		m_command.Format( L"SELECT %s FROM people ORDER BY last_name, first_name", p_fields );
	else if( _first_name.IsEmpty() )
		m_command.Format( L"SELECT %s FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, source", p_fields, _last_name, '%' );
	else
		m_command.Format( L"SELECT %s FROM people WHERE last_name = '%s' AND first_name='%s' ORDER BY last_name, first_name, source", p_fields, _last_name, _first_name );
	if( !query( m_command ) ) return;

	theApp.v_Replace.clear();
	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );
#ifndef _DEBUG
	str.Format( L"Azonos nevû házastársak keresése..." );
	wndP.SetText( str );
#endif
	for( i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		rowid		= m_recordset->GetFieldString( P_ROWID );
		sex_id		= m_recordset->GetFieldString( P_SEX_ID );

		fillSpouses( rowid, sex_id );			// összes házastárs begyûjtése
		if( vSpouses.size() < 2  ) goto cont;	// ga csak egy házastárs volt, akkor az nem érdekel
		fillSameSpouses();						// az azono snevû házaspárok kiszedése
		if( vSameSpouses.size() )				// ha voltak azonos nevõ házaspárok
		{
			same();
			++m_cnt;
		}
cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	fwprintf( fh1, L"</pre>" );
	fclose( fh1 );
	wndP.DestroyWindow();

	fclose( fh1 );
	if( !m_cnt )
	{
		if( _fullname.IsEmpty()  )
			str = L"Nem találtam olyan embert, akinek több azonos nevû házastársa lenne!";
		else if( _first_name.IsEmpty() )
			str.Format( L"Nem találtam olyan '%s'-el kezdõdõ családnevû embert, akinek több azonos nevû házastársa lenne!", _last_name ); 
		else
			str.Format( L"Nem találtam olyan '%s %s' nevû embert, akinek több azonos nevû házastársa lenne!", _last_name, _first_name ); 
		AfxMessageBox( str );

		return;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// az azonos nevû házastársakat mind összedaja! Ez nem jó!!! Kritériumokat kellen alkalmazni!!
void CCheckSameSpouses::same()
{
	CString first_name1;
	firstName	= m_recordset->GetFieldString( P_FIRST_NAME );
	lastName	= m_recordset->GetFieldString( P_LAST_NAME );

	name.Format( L"%s %s", lastName, firstName );
	if( name.IsEmpty() ) return;

	lineNumber	= m_recordset->GetFieldString( P_LINENUMBER );
	tableNumber	= m_recordset->GetFieldString( P_TABLENUMBER );
	generation	= m_recordset->GetFieldString( P_GENERATION );
	source		= m_recordset->GetFieldString( P_SOURCE );
	birth		= m_recordset->GetFieldString( P_BIRTH_DATE );
	death		= m_recordset->GetFieldString( P_DEATH_DATE );
	father_id	= m_recordset->GetFieldString( P_FATHER_ID );
	mother_id	= m_recordset->GetFieldString( P_MOTHER_ID );
	united		= m_recordset->GetFieldString( P_UNITED );

	m_command.Format( L"SELECT lineNumber, source, first_name, last_name, birth_date, death_date FROM people WHERE rowid='%s'", father_id );
	if( !query1( m_command ) ) return ;
	lineNumberF = m_recordset1->GetFieldString( 0 );
	sourceF		= m_recordset1->GetFieldString( 1 );
	first_name1 = getFirstWord( m_recordset1->GetFieldString( 2 ) );
	father.Format( L"%s %s", m_recordset1->GetFieldString( 3 ), first_name1 );
	birthDateF	= m_recordset1->GetFieldString( 4 );
	deathDateF	= m_recordset1->GetFieldString( 5 );


	m_command.Format( L"SELECT lineNumber, source, first_name, last_name, birth_date, death_date FROM people WHERE rowid='%s'", mother_id );
	if( !query1( m_command ) ) return ;
	lineNumberM = m_recordset1->GetFieldString( 0 );
	sourceM		= m_recordset1->GetFieldString( 1 );
	first_name1 = getFirstWord( m_recordset1->GetFieldString( 2 ) );
	mother.Format( L"%s %s", m_recordset1->GetFieldString( 3 ), first_name1 );
	birthDateM	= m_recordset1->GetFieldString( 4 );
	deathDateM	= m_recordset1->GetFieldString( 5 );

	std::sort( vSameSpouses.begin(), vSameSpouses.end(), sortBySource );
	printHtml();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::printHtml()
{
	CString birthDate1;
	CString birthDate2;
	CString deathDate1;
	CString deathDate2;
	CString father1;
	CString father2;
	CString mother1;
	CString mother2;
	CString source1;
	CString source2;

//	if( vSameSpouses.at(1).source == L"1" )	return;

	fwprintf( fh1, L"<br><font color='red'>%4d. %6s %6s %6s %s %s %s %-25s %-15s %-15s %-25s %-15s %-15s %-25s %-15s %-15s</font><br>",\
m_cnt+1,\
rowid,\
lineNumber,\
tableNumber,\
generation,\
source,\
united,\
name,\
birth,\
death,\
father,\
birthDateF,\
deathDateF,\
mother,\
birthDateM,\
deathDateM\
);

	str.Format( L"%d", m_cnt + 1 );
	nItem = m_ListCtrl.InsertItem( nItem, str );
		
	m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );
	m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, tableNumber );
	m_ListCtrl.SetItemText( nItem, L_GENERATION, generation );
	m_ListCtrl.SetItemText( nItem, L_SOURCE, source );
	m_ListCtrl.SetItemText( nItem, L_UNITED, united );
	m_ListCtrl.SetItemText( nItem, L_NAME, name );
	m_ListCtrl.SetItemText( nItem, L_BIRTH, birth );
	m_ListCtrl.SetItemText( nItem, L_DEATH, death );
	m_ListCtrl.SetItemText( nItem, L_SOURCEF, sourceF );
	m_ListCtrl.SetItemText( nItem, L_FATHER, father );
	m_ListCtrl.SetItemText( nItem, L_BIRTH_F, birthDateF );
	m_ListCtrl.SetItemText( nItem, L_DEATH_F, deathDateF );
	m_ListCtrl.SetItemText( nItem, L_SOURCEM, sourceM );
	m_ListCtrl.SetItemText( nItem, L_MOTHER, mother );
	m_ListCtrl.SetItemText( nItem, L_BIRTH_M, birthDateM );
	m_ListCtrl.SetItemText( nItem, L_DEATH_M, deathDateM );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBERF, lineNumberF );

	m_ListCtrl.SetItemData( nItem, -1 );
	++nItem;



	fwprintf( fh1, L"%12s %6s %6s %s %s %s %-25s %-15s %-15s %-25s %-15s %-15s %-25s %-15s %-15s<br>",\
vSameSpouses.at(0).rowid,\
vSameSpouses.at(0).lineNumber,\
vSameSpouses.at(0).tableNumber,\
vSameSpouses.at(0).generation,\
vSameSpouses.at(0).source,\
vSameSpouses.at(0).united,\
vSameSpouses.at(0).spouse,\
vSameSpouses.at(0).birthDate,\
vSameSpouses.at(0).deathDate,\
vSameSpouses.at(0).father,\
vSameSpouses.at(0).birthDateF,\
vSameSpouses.at(0).deathDateF,\
vSameSpouses.at(0).mother,\
vSameSpouses.at(0).birthDateM,\
vSameSpouses.at(0).deathDateM\
);

	nItem = m_ListCtrl.InsertItem( nItem, L"" );
		
	m_ListCtrl.SetItemText( nItem, L_ROWID, vSameSpouses.at(0).rowid );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBER, vSameSpouses.at(0).lineNumber );
	m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, vSameSpouses.at(0).tableNumber );
	m_ListCtrl.SetItemText( nItem, L_UNITED, vSameSpouses.at(0).united );
	m_ListCtrl.SetItemText( nItem, L_GENERATION, vSameSpouses.at(0).generation );
	m_ListCtrl.SetItemText( nItem, L_SOURCE, vSameSpouses.at(0).source );
	m_ListCtrl.SetItemText( nItem, L_NAME, vSameSpouses.at(0).spouse );
	m_ListCtrl.SetItemText( nItem, L_BIRTH, vSameSpouses.at(0).birthDate );
	m_ListCtrl.SetItemText( nItem, L_DEATH, vSameSpouses.at(0).deathDate );

	m_ListCtrl.SetItemText( nItem, L_SOURCEF, vSameSpouses.at(0).sourceF );
	m_ListCtrl.SetItemText( nItem, L_FATHER, vSameSpouses.at(0).father );
	m_ListCtrl.SetItemText( nItem, L_BIRTH_F, vSameSpouses.at(0).birthDateF );
	m_ListCtrl.SetItemText( nItem, L_DEATH_F, vSameSpouses.at(0).deathDateF );
	m_ListCtrl.SetItemText( nItem, L_SOURCEM, vSameSpouses.at(0).sourceM );
	m_ListCtrl.SetItemText( nItem, L_MOTHER, vSameSpouses.at(0).mother );
	m_ListCtrl.SetItemText( nItem, L_BIRTH_M, vSameSpouses.at(0).birthDateM );
	m_ListCtrl.SetItemText( nItem, L_DEATH_M, vSameSpouses.at(0).deathDateM );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBERF, vSameSpouses.at(0).lineNumberF );

	++nItem;



	int col = 0;
	int cnt = 1;
	for( UINT j = 1; j < vSameSpouses.size(); ++j )
	{
		fwprintf( fh1, L"%12s %6s %6s %s ", vSameSpouses.at(j).rowid, vSameSpouses.at(j).lineNumber, vSameSpouses.at(j).tableNumber, vSameSpouses.at(j).generation );
		source1 = vSameSpouses.at(j-1).source;
		source2 = vSameSpouses.at(j).source;
		if( source1 == L"1" && source2 == L"1" )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%s</span> ", source2 );

		}
		else
			fwprintf( fh1, L"%s ", source2 );

		fwprintf( fh1, L"%s %-25s ", vSameSpouses.at(j).united, vSameSpouses.at(j).spouse );

		birthDate1 = vSameSpouses.at(j-1).birthDate.Left(15);
		birthDate2 = vSameSpouses.at(j).birthDate.Left(15);
		if( !birthDate1.IsEmpty() && !birthDate2.IsEmpty() && birthDate1 !=  birthDate2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-15s</span> ", birthDate2 );
			col = col | 1 << L_BIRTH;
		}
		else
			fwprintf( fh1, L"%-15s ", birthDate2 );

		deathDate1 = vSameSpouses.at(j-1).deathDate.Left(15);
		deathDate2 = vSameSpouses.at(j).deathDate.Left(15);
		if( !deathDate1.IsEmpty() && !deathDate2.IsEmpty() && deathDate1 !=  deathDate2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-15s</span> ", deathDate2 );
			col = col | 1 << L_DEATH;
		}
		else
			fwprintf( fh1, L"%-15s ", deathDate2 );

		father1 = vSameSpouses.at(j-1).father.Left(25);
		father2 = vSameSpouses.at(j).father.Left(25);
		if( !father1.Trim().IsEmpty() && !father2.Trim().IsEmpty() && father1 !=  father2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-25s</span> ", father2 );
			col = col | 1 << L_FATHER;
		}
		else
			fwprintf( fh1, L"%-25s ", father2 );

		birthDate1 = vSameSpouses.at(j-1).birthDateF;
		birthDate2 = vSameSpouses.at(j).birthDateF;
		if( !birthDate1.Trim().IsEmpty() && !birthDate2.Trim().IsEmpty() && birthDate1 != birthDate2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-15s</span> ", birthDate2 );
//			unite = FALSE;
		}
		else
			fwprintf( fh1, L"%-15s ", birthDate2 );

		deathDate1 = vSameSpouses.at(j-1).deathDateF;
		deathDate2 = vSameSpouses.at(j).deathDateF;
		if( !deathDate1.Trim().IsEmpty() && !deathDate2.Trim().IsEmpty() && deathDate1 != deathDate2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-15s</span> ", deathDate2 );
//			unite = FALSE;
		}
		else
			fwprintf( fh1, L"%-15s ", deathDate2 );


		mother1 = vSameSpouses.at(j-1).mother.Left(25);
		mother2 = vSameSpouses.at(j).mother.Left(25);
		if( !mother1.TrimRight().IsEmpty() && !mother2.TrimRight().IsEmpty() && mother1 !=  mother2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-15s</span> ", mother2 );
			col = col | 1 << L_MOTHER;
		}
		else
			fwprintf( fh1, L"%-25s ", mother2 );

		birthDate1 = vSameSpouses.at(j-1).birthDateM;
		birthDate2 = vSameSpouses.at(j).birthDateM;
		if( !birthDate1.Trim().IsEmpty() && !birthDate2.Trim().IsEmpty() && birthDate1 != birthDate2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-15s</span> ", birthDate2 );
//			unite = FALSE;
		}
		else
			fwprintf( fh1, L"%-15s ", birthDate2 );

		deathDate1 = vSameSpouses.at(j-1).deathDateM;
		deathDate2 = vSameSpouses.at(j).deathDateM;
		if( !deathDate1.Trim().IsEmpty() && !deathDate2.Trim().IsEmpty() && deathDate1 != deathDate2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-15s</span> ", deathDate2 );
//			unite = FALSE;
		}
		else
			fwprintf( fh1, L"%-15s ", deathDate2 );


		fwprintf( fh1, L"<br>" );


		nItem = m_ListCtrl.InsertItem( nItem, L"" );
		
		m_ListCtrl.SetItemText( nItem, L_ROWID, vSameSpouses.at(j).rowid );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBER, vSameSpouses.at(j).lineNumber );
		m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, vSameSpouses.at(j).tableNumber );
		m_ListCtrl.SetItemText( nItem, L_UNITED, vSameSpouses.at(j).united );
		m_ListCtrl.SetItemText( nItem, L_GENERATION, vSameSpouses.at(j).generation );
		m_ListCtrl.SetItemText( nItem, L_SOURCE, vSameSpouses.at(j).source );
		m_ListCtrl.SetItemText( nItem, L_NAME, vSameSpouses.at(j).spouse );
		m_ListCtrl.SetItemText( nItem, L_BIRTH, vSameSpouses.at(j).birthDate );
		m_ListCtrl.SetItemText( nItem, L_DEATH, vSameSpouses.at(j).deathDate );

		m_ListCtrl.SetItemText( nItem, L_SOURCEF, vSameSpouses.at(j).sourceF );
		m_ListCtrl.SetItemText( nItem, L_FATHER, vSameSpouses.at(j).father );
		m_ListCtrl.SetItemText( nItem, L_BIRTH_F, vSameSpouses.at(j).birthDateF );
		m_ListCtrl.SetItemText( nItem, L_DEATH_F, vSameSpouses.at(j).deathDateF );
		m_ListCtrl.SetItemText( nItem, L_SOURCEM, vSameSpouses.at(j).sourceM );
		m_ListCtrl.SetItemText( nItem, L_MOTHER, vSameSpouses.at(j).mother );
		m_ListCtrl.SetItemText( nItem, L_BIRTH_M, vSameSpouses.at(j).birthDateM );
		m_ListCtrl.SetItemText( nItem, L_DEATH_M, vSameSpouses.at(j).deathDateM );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBERF, vSameSpouses.at(j).lineNumberF );

		m_ListCtrl.SetItemData( nItem, col );
		++nItem;
	}
	nItem = m_ListCtrl.InsertItem( nItem, L"" );
	++nItem;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
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
		if( iData == - 1 )
		{
			pLVCD->clrText = RGB( 255,0,0 );
		}
		else if( iData & mask )
		{
//			pLVCD->clrText	 = RGB( 0,0,255 );
			pLVCD->clrTextBk = YELLOW;
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnEditnotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnEditNotpadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	L_LINENUMBERF );
	if( !lineNumber.IsEmpty() ) theApp.editNotepad( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnListPeople()
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
void CCheckSameSpouses::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CRelations dlg;
	int nItem		= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid	= m_ListCtrl.GetItemText( nItem, L_ROWID );

	dlg.nItem		= nItem;
	dlg.m_rowid		= rowid;
	if( dlg.DoModal() == IDCANCEL ) return;

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckSameSpouses:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CCheckSameSpouses::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnHtmlShows()
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
void CCheckSameSpouses::OnHtmlPeoplefather()
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySpouse(const MORESPOUSES &v1, const MORESPOUSES &v2) 
{ 
    return( v1.spouse < v2.spouse ); 
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySource(const MORESPOUSES &v1, const MORESPOUSES &v2) 
{ 
    return( v1.source < v2.source ); 
} 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckSameSpouses::query( CString command )
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
BOOL CCheckSameSpouses::query1( CString command )
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
BOOL CCheckSameSpouses::query2( CString command )
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
BOOL CCheckSameSpouses::query3( CString command )
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
BOOL CCheckSameSpouses::query4( CString command )
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
void CCheckSameSpouses::htmlHeader( CString title )
{
	fwprintf( fh1, L"<HEAD>\n" );
	fwprintf( fh1, L"<style>\n" );
	fwprintf( fh1, L"</style>\n" );
	fwprintf( fh1, L"</HEAD>\n" );
	fwprintf( fh1, L"<BODY>\n" );
	fwprintf( fh1, L"<center>%s</center><br><br>\n\n", title );
	fwprintf( fh1, m_explanation );
	fwprintf( fh1, L"<pre>" );
	fwprintf( fh1, L"\n%-20s %s<br>", L"Adatbázis:", theApp.m_databaseSpec );
//	fwprintf( fh1, L"%-20s %d<br>", L"Összevonások száma:", theApp._iterationCount );
	fwprintf( fh1, L"%-20s %s<br><br><br>\n", L"lista készült:", theApp.getPresentDateTime() );
	
	str.Format( L"\n%12s %6s %6s %1s %1s %1s %-25s %-15s %-15s %-25s %-25s<br>\n", L"rowid", L"line#", L"table#", L"G", L"S", L"U", L"name", L"születés", L"halál", L"apja neve", L"anyja neve" );
	fwprintf( fh1, str );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::fillSpouses( CString rowid, CString sex_id )
{
	CString rowidS;
	CString firstNameS;
	CString lastNameS;
	CString spouse2;
	CString fatherId;
	CString motherId;
	MORESPOUSES vspouse;
	CString first_name;

	vSpouses.clear();

	if( sex_id == L"1" )
	{
		m_command.Format( L"SELECT spouse2_id FROM marriages WHERE spouse1_id=%s", rowid );
	}
	else
	{
		m_command.Format( L"SELECT spouse1_id FROM marriages WHERE spouse2_id=%s", rowid );
	}
	if( !query1( m_command ) ) return;

	if( m_recordset1->RecordsCount() < 2 ) return;

	
	for( UINT j = 0; j < m_recordset1->RecordsCount(); ++j, m_recordset1->MoveNext() )
	{
		rowidS = m_recordset1->GetFieldString( 0 );

		m_command.Format( L"SELECT first_name, last_name, generation, source, lineNumber, tableNumber, birth_date, death_date, father_id, mother_id, sex_id, united FROM people WHERE rowid ='%s'", rowidS );
		if( !query2( m_command ) ) return;

		firstNameS		= m_recordset2->GetFieldString( 0 );
		lastNameS		= m_recordset2->GetFieldString( 1 );
		spouse2.Format ( L"%s %s", lastNameS, firstNameS );

		vspouse.rowid		= rowidS;
		vspouse.spouse		= spouse2;
		vspouse.generation	= m_recordset2->GetFieldString( 2 );
		vspouse.source		= m_recordset2->GetFieldString( 3 );
		vspouse.lineNumber	= m_recordset2->GetFieldString( 4 );
		vspouse.tableNumber = m_recordset2->GetFieldString( 5 );
		vspouse.birthDate	= m_recordset2->GetFieldString( 6 );
		vspouse.deathDate	= m_recordset2->GetFieldString( 7 );
			
		fatherId			= m_recordset2->GetFieldString( 8 );
		motherId			= m_recordset2->GetFieldString( 9 );
		vspouse.sex_id		= m_recordset2->GetFieldString( 10 );

		vspouse.united		= m_recordset2->GetFieldString( 11 );
	
		m_command.Format( L"SELECT last_name, first_name, lineNumber, source, birth_date, death_date FROM people WHERE rowid ='%s'", fatherId );
		if( !query3( m_command ) ) return;
		first_name = getFirstWord( m_recordset3->GetFieldString( 1 ) );
		vspouse.father.Format( L"%s %s", m_recordset3->GetFieldString( 0 ), first_name );
		vspouse.lineNumberF.Format( L"%s", m_recordset3->GetFieldString( 2 ) );
		vspouse.sourceF.Format( L"%s", m_recordset3->GetFieldString( 3 ) );
		vspouse.birthDateF	= m_recordset3->GetFieldString( 4 );
		vspouse.deathDateF	= m_recordset3->GetFieldString( 5 );



		m_command.Format( L"SELECT last_name, first_name, lineNumber,source, birth_date, death_date FROM people WHERE rowid ='%s'", motherId );
		if( !query3( m_command ) ) return;
		first_name = getFirstWord( m_recordset3->GetFieldString( 1 ) );
		vspouse.mother.Format( L"%s %s", m_recordset3->GetFieldString( 0 ), first_name );
		vspouse.lineNumberM.Format( L"%s", m_recordset3->GetFieldString( 2 ) );
		vspouse.sourceM.Format( L"%s", m_recordset3->GetFieldString( 3 ) );
		vspouse.birthDateM	= m_recordset3->GetFieldString( 4 );
		vspouse.deathDateM	= m_recordset3->GetFieldString( 5 );
		vSpouses.push_back( vspouse );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::fillSameSpouses( )
{
	std::sort( vSpouses.begin(), vSpouses.end(), sortBySpouse );

	MORESPOUSES vspouse;
	BOOL FIRST = TRUE;
	vSameSpouses.clear();
	for( UINT j = 1; j < vSpouses.size(); ++j )
	{
		if( !vSpouses.at(j-1).spouse.Compare( vSpouses.at(j).spouse ) )		// ha az egymás utáni  házastársak neve
		{																	// azonos, akkor beteszi a VSameSpouses
			if( FIRST )														// vektorba
			{
				vspouse.birthDate	= vSpouses.at(j-1).birthDate;
				vspouse.deathDate	= vSpouses.at(j-1).deathDate;
				vspouse.father		= vSpouses.at(j-1).father;
				vspouse.lineNumber	= vSpouses.at(j-1).lineNumber;
				vspouse.mother 		= vSpouses.at(j-1).mother;
				vspouse.rowid 		= vSpouses.at(j-1).rowid;
				vspouse.sex_id		= vSpouses.at(j-1).sex_id;
				vspouse.generation	= vSpouses.at(j-1).generation;
				vspouse.source		= vSpouses.at(j-1).source;
				vspouse.spouse		= vSpouses.at(j-1).spouse;
				vspouse.tableNumber	= vSpouses.at(j-1).tableNumber;
				vspouse.united		= vSpouses.at(j-1).united;
				vspouse.lineNumberF	= vSpouses.at(j-1).lineNumberF;
				vspouse.lineNumberM	= vSpouses.at(j-1).lineNumberM;
				vspouse.sourceF		= vSpouses.at(j-1).sourceF;
				vspouse.sourceM		= vSpouses.at(j-1).sourceM;
				vspouse.birthDateF	= vSpouses.at(j-1).birthDateF;
				vspouse.birthDateM	= vSpouses.at(j-1).birthDateM;
				vspouse.deathDateF	= vSpouses.at(j-1).deathDateF;
				vspouse.deathDateM	= vSpouses.at(j-1).deathDateM;

//				vspouse.unite		= TRUE;
				vSameSpouses.push_back( vspouse );
				FIRST = FALSE;
			}
			vspouse.birthDate	= vSpouses.at(j).birthDate;
			vspouse.deathDate	= vSpouses.at(j).deathDate;
			vspouse.father		= vSpouses.at(j).father;
			vspouse.lineNumber	= vSpouses.at(j).lineNumber;
			vspouse.mother 		= vSpouses.at(j).mother;
			vspouse.rowid 		= vSpouses.at(j).rowid;
			vspouse.sex_id		= vSpouses.at(j).sex_id;
			vspouse.generation	= vSpouses.at(j).generation;
			vspouse.source		= vSpouses.at(j).source;
			vspouse.spouse		= vSpouses.at(j).spouse;
			vspouse.tableNumber	= vSpouses.at(j).tableNumber;
			vspouse.united		= vSpouses.at(j).united;
			vspouse.lineNumberF	= vSpouses.at(j).lineNumberF;
			vspouse.lineNumberM	= vSpouses.at(j).lineNumberM;
			vspouse.sourceF		= vSpouses.at(j).sourceF;
			vspouse.sourceM		= vSpouses.at(j).sourceM;
			vspouse.birthDateF	= vSpouses.at(j-1).birthDateF;
			vspouse.birthDateM	= vSpouses.at(j-1).birthDateM;
			vspouse.deathDateF	= vSpouses.at(j-1).deathDateF;
			vspouse.deathDateM	= vSpouses.at(j-1).deathDateM;
//			vspouse.unite		= TRUE;
			vSameSpouses.push_back( vspouse );
		}
		else							// egy embernek több pár azonos nevû házastársa is lehetséges!!
		{
			if( !FIRST )
			{
				same();
				++m_cnt;
				vSameSpouses.clear();
			}
			FIRST = TRUE;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnHtml()
{
	theApp.showHtmlFile( fileSpec );
}
