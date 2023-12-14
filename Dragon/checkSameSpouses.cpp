// TableSameSpouse.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkSameSpouses.h"
#include "afxdialogex.h"
#include "editHtmlLines.h"
#include "Relatives.h"
#include "CheckParam0.h"
#include <algorithm>
#include "GetLastFirst.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "EditPeople.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckSameSpouseName()
//{
//	CCheckSameSpouses dlg;
//	dlg.m_wedding = false;
//	dlg.DoModal();
//}
//void CDragonDlg::OnCheckWeddingDate()
//{
//	CCheckSameSpouses dlg;
//	dlg.m_wedding = true;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySpouses(const MORESPOUSES &v1, const MORESPOUSES &v2);
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
	L_LINENUMBER,
	L_TABLENUMBER,
	L_UNITED,
	L_GENERATION,
	L_SOURCE,
	L_ROWID, 
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_WEDDING,
	L_SOURCEF,
	L_ROWIDF,
	L_FATHER,
	L_BIRTH_F,
	L_DEATH_F,
	L_SOURCEM,
	L_ROWIDM,
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

	m_explanation = L"Egy emberenek term�szetesen lehet k�t vagy ak�r t�bb azonos nev� h�zast�rsa, \
sokszor azonban hib�s adatokra h�vja fel a figyelmet.\
\r\n\r\n\
Egy embernek 2 okb�l lehet azonos nev� h�zast�rsa:\
\r\n\r\n\
1. Val�ban volt azonos nev� h�zast�rsa. Ebben az esetben v�rhat�, hogy a h�zast�rs szem�lyes adatai k�l�nb�znek.\
\r\n\r\n\
2. Val�j�ban nem volt azonos nev� h�zast�rsa, csak annak t�bb bejegyz�se volt, �s nem tudtuk �sszevonni azokat \
valemelyik szem�lyes adat�nak k�l�nb�z�s�ge miatt.\
\r\n\r\n\
Mindk�t esetben a h�zast�rsak valamely szem�lyes adata k�l�nb�zik, teh�t nek�nk kell eld�nteni, hogy a fenti 2 eset k�z�l \
melyikkel �llunk szembe.\
\r\n\r\n\
Ember�nket az azonos nev� h�zast�rsai k�vetik sz�let�si, hal�loz�si, h�zass�gk�t�si d�tumukkal valamint apjuk, anyjuk nev�vel, \
s�rg�val kiemelve az elt�r� szem�lyes adatot.\
\r\n\r\n\
Ha az azonos emberek �sszevon�sa el�tt k�sz�tj�k el a list�t, akkor csak olyankor fordulhat el� t�bb azonos nev� h�zasp�r, \
ha azok egyetlen lesz�rmazotti sorban vannak megadva h�zast�rsk�nt, teh�t val�ban t�bb azonos nev� h�zast�rsa volt ember�nknek.\
\r\n\r\n\
Az azonos emberek bejegyz�seinek �sszevon�sa megsokszorozhatja a t�bb azonos nev� h�zasp�rok sz�m�t, \
ugyanis ha ember�nk azonoss� lett egy vagy t�bb m�s azonos nev� emberrel, akkor ezek �sszevon�sra ker�ltek, \
de azonos nev� h�zast�rsaik nem felt�tlen�l teljes�tik az �sszevon�si krit�riumot, ez�rt �k b�r azonos nev�ek, \
m�gis k�l�nb�z� emberk�nt maradnak az adatb�zisban.\
\r\n\r\n\
Az 'S' oszlopban az ember szerepk�dja van, az 'U' oszlopban l�v� sz�m pedig megmutatja, hogy h�ny embert vont �ssze az \
egyes�t�si elj�r�s.\
";

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
	_lastname	= L"";
	_firstname	= L"";

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
	DDX_Text(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeress);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckSameSpouses, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckSameSpouses::OnCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCheckSameSpouses::OnDblclkList)
	ON_COMMAND(ID_INFO, &CCheckSameSpouses::OnInfo)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CCheckSameSpouses::OnClickedKeress)
	ON_STN_CLICKED(IDC_NEXT, &CCheckSameSpouses::OnClickedNext)
	

	// IDR_DROPDOWN_HTML funkci�k	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckSameSpouses::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckSameSpouses::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckSameSpouses::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckSameSpouses::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_HTML_CHILDREN, &CCheckSameSpouses::OnHtmlChildren)
	ON_COMMAND(ID_DB_EDIT, &CCheckSameSpouses::OnDbEdit)

END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckSameSpouses::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if( m_wedding )
	{
			theApp.m_pszAppName = _tcsdup( L"Azonos nev� h�zast�rsak, k�l�nb�z� esk�v�i d�tumok" );
	}
	else
	{
		theApp.m_pszAppName = _tcsdup( L"Azonos nev� h�zast�rsakkal rendelkez� emberek list�ja" );
	}
/*
	int iter = theApp.getUserVersion();
	CString attention = L"Ezt a m�veletet az azonos emberek �sszevon�sa ut�n �rdemes alkalmazni, hogy csak a gyan�s eseteket tartalmazza.\nAkarod, hogy m�gis elk�sz�ts�k a list�t?";
	if( !iter )
	{
		if( AfxMessageBox( attention, MB_YESNO ) == IDNO ) 
		{
			OnCancel();
			return false;
		}
	}
*/
/*	
	_info = L"\
Azokat az embereket list�zzuk, akiknek t�bb azonos nev� h�zast�rsa van. Egy embernek term�szetesen lehet k�t \
vagy ak�r t�bb azonos nev� h�zast�rsa, azonban legt�bbsz�r hib�s adatokra h�vja fel a figyelmet.\
\r\n\
K�t vagy t�bb azonos nev� h�zast�rs szem�lyes adatai k�z�tti k�l�nbs�geket s�rga sz�nnel emeli ki a program.\
\r\n\r\n\
Ha az azonos emberek �sszevon�sa el�tt k�sz�tj�k el a list�t, akkor csak olyankor fordulhat el� t�bb azonos nev� h�zasp�r, \
ha azok egyetlen lesz�rmazotti sorban vannak megadva h�zast�rsk�nt.\
\r\n\
Az azonos emberek �sszevon�sa megsokszorozhatja az ilyen eseteket, ugyanis ha ember�nk azonoss� lett egy vagy t�bb m�s azonos \
nev� emberrel, akkor ezek �sszevon�sra ker�ltek, de azonos nev� h�zast�rsaik nem felt�tlen�l teljes�tik az �sszevon�si \
krit�riumot, ez�rt �k b�r azonos nev�ek, m�gis k�l�nb�z� emberk�nt maradnak az adatb�zisban.\
\r\n\r\n\
A mag�t�l �rthet�d� oszlopneveken t�l az al�bbiak vannak:\n\
U - a n�v alatt egyes�tett bejegyz�sek sz�ma\n\
G - gener�ci�s k�d\n\
s - az ember hierarchia sz�ma (1-4)\
";

	theApp.m_pszAppName = _tcsdup( L"Azonos nev� h�zast�rsakkal rendelkez� emberek list�ja" );
	if( (AfxMessageBox( _info, MB_ICONINFORMATION | MB_OKCANCEL )) == IDCANCEL ) 
	{
		OnCancel();
		return false;
	}

	CCheckParam0 dlg;
	dlg._caption = L"Azonos nev� h�zast�rsakkal rendelkez� emberek list�ja";
	dlg._info = _info;
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
*/
	colorKeress.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );

	_fullname.Format( L"%s %s", _lastname, _firstname );
	_fullname.Trim();

	if( _fullname.IsEmpty() )
	{
		if( m_wedding )
			m_caption = L"Azonos nev� h�zasp�rok, k�l�nb�z� esk�v�i d�tumok";
		else
			m_caption = L"Emberek, akiknek t�bb azonos nev� h�zast�rsa van.";
	}
	else
		m_caption.Format( L"%s nev� emberek, akinknek t�bb azonos nev� h�zast�rsa van.", _fullname );

	SetWindowTextW( m_caption ),

	createColumns();
	nItem = 0;
	m_cnt = 0;
	sameSpouses();

	if( !m_cnt ) OnCancel();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::createColumns()
{

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"s",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,		L"n�v",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"sz�let�s",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"hal�l",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_WEDDING,		L"esk�v�",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCEF,		L"s",			LVCFMT_LEFT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDF,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,		L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_F,		L"sz�let�s",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH_F,		L"hal�l",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCEM,		L"s",			LVCFMT_LEFT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWIDM,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,		L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH_M,		L"sz�let�s",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH_M,		L"hal�l",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBERF,	L"line#F",		LVCFMT_RIGHT,	 60,-1,COL_HIDDEN );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::sameSpouses()
{
	CString fileName;
	CString first_name1;
	CString	sex_id;
	CString spouse2;
	CString rowidS;
	CString lastNameS;
	CString firstNameS;
	CString fatherId;
	CString motherId;
	UINT i;
	MORESPOUSES vspouse;

	CProgressWnd wndP(NULL, m_caption ); 
	wndP.GoModal();

#ifndef _DEBUG
	str.Format( L"Emberek lek�rdez�se az adatb�zisb�l..." );
	wndP.SetText( str );
#endif
	if( _fullname.IsEmpty() )
		m_command.Format( L"SELECT %s FROM people ORDER BY last_name, first_name", p_fields );
	else if( _firstname.IsEmpty() )
		m_command.Format( L"SELECT %s FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, source", p_fields, _lastname, '%' );
	else
		m_command.Format( L"SELECT %s FROM people WHERE last_name = '%s' AND first_name='%s' ORDER BY last_name, first_name, source", p_fields, _lastname, _firstname );
	if( !query( m_command ) ) return;

	theApp.v_Replace.clear();
	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );
#ifndef _DEBUG
	str.Format( L"Azonos nev� h�zast�rsak keres�se..." );
	wndP.SetText( str );
#endif


	for( i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		rowid		= m_recordset->GetFieldString( P_ROWID );
		sex_id		= m_recordset->GetFieldString( P_SEX_ID );
		vSpouses.clear();
		fillSpouses( rowid, sex_id );			// �sszes h�zast�rs begy�jt�se
		if( vSpouses.size() < 2  ) goto cont;	// ga csak egy h�zast�rs volt, akkor az nem �rdekel
		fillSameSpouses();						// az azono snev� h�zasp�rok kiszed�se
		if( vSameSpouses.size() )				// ha voltak azonos nev� h�zasp�rok
		{
			same();
			++m_cnt;
		}
cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

	wndP.DestroyWindow();

	if( !m_cnt )
	{
		if( _fullname.IsEmpty()  )
			str = L"Nem tal�ltam olyan embert, akinek t�bb azonos nev� h�zast�rsa lenne!";
		else if( _firstname.IsEmpty() )
			str.Format( L"Nem tal�ltam olyan '%s'-el kezd�d� csal�dnev� embert, akinek t�bb azonos nev� h�zast�rsa lenne!", _lastname ); 
		else
			str.Format( L"Nem tal�ltam olyan '%s %s' nev� embert, akinek t�bb azonos nev� h�zast�rsa lenne!", _lastname, _firstname ); 
		AfxMessageBox( str );

		return;
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// rowid emebr �sszes h�zast�rs�nak �sszegy�jt�se 
void CCheckSameSpouses::fillSpouses( CString rowid, CString sex_id )
{
	CString rowidM;			// rowid marriage
	CString rowidS;			// rowid sopouse;
	CString firstNameS;
	CString lastNameS;
	CString spouse2;
	CString fatherId;
	CString motherId;
	CString first_name;
	MORESPOUSES vspouse;
	CString wedding;
	

	if( sex_id == MAN )
	{
		m_command.Format( L"SELECT rowid, wife_id, date FROM marriages WHERE husband_id=%s", rowid );
	}
	else
	{
		m_command.Format( L"SELECT rowid, husband_id, date FROM marriages WHERE wife_id=%s", rowid );
	}
	if( !query1( m_command ) ) return;

	if( m_recordset1->RecordsCount() < 2 ) return;

	// h�zast�rsak lek�rdez�se �s vSpouses vektorba gy�jt�se
	
	for( UINT j = 0; j < m_recordset1->RecordsCount(); ++j, m_recordset1->MoveNext() )
	{
		
		rowidM = m_recordset1->GetFieldString( 0 );
		rowidS = m_recordset1->GetFieldString( 1 );
		wedding = m_recordset1->GetFieldString( 2);

		m_command.Format( L"SELECT first_name, last_name, generation, source, lineNumber, tableNumber, birth_date, death_date, father_id, mother_id, sex_id, united FROM people WHERE rowid ='%s'", rowidS );
		if( !query2( m_command ) ) return;

		firstNameS		= m_recordset2->GetFieldString( 0 );
		lastNameS		= m_recordset2->GetFieldString( 1 );
		spouse2.Format ( L"%s %s", lastNameS, firstNameS );

		vspouse.rowidM		= rowidM;
		vspouse.rowid		= rowidS;
		vspouse.spouse		= spouse2;
		vspouse.generation	= m_recordset2->GetFieldString( 2 );
		vspouse.source		= m_recordset2->GetFieldString( 3 );
		vspouse.lineNumber	= m_recordset2->GetFieldString( 4 );
		vspouse.tableNumber = m_recordset2->GetFieldString( 5 );
		vspouse.birthDate	= m_recordset2->GetFieldString( 6 );
		vspouse.deathDate	= m_recordset2->GetFieldString( 7 );
		vspouse.wedding		= wedding;
		
		vspouse.father_id	= m_recordset2->GetFieldString( 8 );
		vspouse.mother_id	= m_recordset2->GetFieldString( 9 );
			
		fatherId			= m_recordset2->GetFieldString( 8 );
		motherId			= m_recordset2->GetFieldString( 9 );
		vspouse.sex_id		= m_recordset2->GetFieldString( 10 );

		vspouse.united		= m_recordset2->GetFieldString( 11 );
/*	
		vspouse.father.Empty();
		vspouse.lineNumberF.Empty();
		vspouse.sourceF.Empty();
		vspouse.birthDateF.Empty();
		vspouse.deathDateF.Empty();
*/
		m_command.Format( L"SELECT last_name, first_name, lineNumber, source, birth_date, death_date FROM people WHERE rowid ='%s'", fatherId );
		if( !query3( m_command ) ) return;
		first_name = getFirstWord( m_recordset3->GetFieldString( 1 ) );
		vspouse.father.Format( L"%s %s", m_recordset3->GetFieldString( 0 ), first_name );
		vspouse.lineNumberF.Format( L"%s", m_recordset3->GetFieldString( 2 ) );
		vspouse.sourceF.Format( L"%s", m_recordset3->GetFieldString( 3 ) );
		vspouse.birthDateF	= m_recordset3->GetFieldString( 4 );
		vspouse.deathDateF	= m_recordset3->GetFieldString( 5 );
	
/*
		vspouse.mother.Empty();
		vspouse.lineNumberM.Empty();
		vspouse.sourceM.Empty();
		vspouse.birthDateM.Empty();
		vspouse.deathDateM.Empty();
*/
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
// Az azonos nev� h�zast�rsak �trak�sa a vSmaeSpouses vektorba
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::fillSameSpouses( )
{
	std::sort( vSpouses.begin(), vSpouses.end(), sortBySpouses );
	MORESPOUSES vspouse;
	BOOL FIRST	= TRUE;

	CString spouse1;
	CString spouse2;
	CString wedding1;
	CString wedding2;

	vSameSpouses.clear();
	for( UINT j = 1; j < vSpouses.size(); ++j )
	{
		firstName	= m_recordset->GetFieldString( P_FIRST_NAME );
		lastName	= m_recordset->GetFieldString( P_LAST_NAME );

		name.Format( L"%s %s", lastName, firstName );
		spouse1 = vSpouses.at(j-1).spouse;
		spouse2 = vSpouses.at(j).spouse;
		if( !vSpouses.at(j-1).spouse.Compare( vSpouses.at(j).spouse ) )		// ha az egym�s ut�ni  h�zast�rsak neve
		{																	// azonos, akkor beteszi a VSameSpouses

			if( m_wedding )
			{
				// ha az esk�v�i d�tum nem k�l�nb�zik, akkor kihagyja
				wedding1 = vSpouses.at(j-1).wedding;
				if( wedding1.IsEmpty() ) continue;
				wedding2 = vSpouses.at(j).wedding;
				if( wedding2.IsEmpty() ) continue;
				if( wedding1 == wedding2 ) continue;
			}

			if( FIRST )   // az els� h�zasp�r														// vektorba
			{  
				vspouse.rowidM		= vSpouses.at(j-1).rowidM; 
				vspouse.birthDate	= vSpouses.at(j-1).birthDate;
				vspouse.deathDate	= vSpouses.at(j-1).deathDate;
				vspouse.wedding		= vSpouses.at(j-1).wedding;
				vspouse.father_id	= vSpouses.at(j-1).father_id;
				vspouse.father		= vSpouses.at(j-1).father;
				vspouse.lineNumber	= vSpouses.at(j-1).lineNumber;
				vspouse.mother_id	= vSpouses.at(j-1).mother_id;
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

				vSameSpouses.push_back( vspouse );
				FIRST = FALSE;
			}
			vspouse.rowidM		= vSpouses.at(j).rowidM;
			vspouse.birthDate	= vSpouses.at(j).birthDate;
			vspouse.deathDate	= vSpouses.at(j).deathDate;
			vspouse.wedding		= vSpouses.at(j).wedding;
			vspouse.father_id	= vSpouses.at(j).father_id;
			vspouse.father		= vSpouses.at(j).father;
			vspouse.lineNumber	= vSpouses.at(j).lineNumber;
			vspouse.mother_id	= vSpouses.at(j).mother_id;
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
			vspouse.birthDateF	= vSpouses.at(j).birthDateF;
			vspouse.birthDateM	= vSpouses.at(j).birthDateM;
			vspouse.deathDateF	= vSpouses.at(j).deathDateF;
			vspouse.deathDateM	= vSpouses.at(j).deathDateM;

			vSameSpouses.push_back( vspouse );
		}
		else							// egy embernek t�bb p�r azonos nev� h�zast�rsa is lehets�ges!!
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az ember adatai, majd ki�r�sa ah�zast�rsakkal egy�tt
void CCheckSameSpouses::same()
{
	// az ember adatai

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

	// az ember apj�nak adatai
	m_command.Format( L"SELECT lineNumber, source, first_name, last_name, birth_date, death_date FROM people WHERE rowid='%s'", father_id );
	if( !query1( m_command ) ) return ;
	lineNumberF = m_recordset1->GetFieldString( 0 );
	sourceF		= m_recordset1->GetFieldString( 1 );
	first_name1 = getFirstWord( m_recordset1->GetFieldString( 2 ) );
	father.Format( L"%s %s", m_recordset1->GetFieldString( 3 ), first_name1 );
	birthDateF	= m_recordset1->GetFieldString( 4 );
	deathDateF	= m_recordset1->GetFieldString( 5 );

	// az emebr anyj�nak adatai
	m_command.Format( L"SELECT lineNumber, source, first_name, last_name, birth_date, death_date FROM people WHERE rowid='%s'", mother_id );
	if( !query1( m_command ) ) return ;
	lineNumberM = m_recordset1->GetFieldString( 0 );
	sourceM		= m_recordset1->GetFieldString( 1 );
	first_name1 = getFirstWord( m_recordset1->GetFieldString( 2 ) );
	mother.Format( L"%s %s", m_recordset1->GetFieldString( 3 ), first_name1 );
	birthDateM	= m_recordset1->GetFieldString( 4 );
	deathDateM	= m_recordset1->GetFieldString( 5 );

	std::sort( vSameSpouses.begin(), vSameSpouses.end(), sortBySource );
	listBlock();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCheckSameSpouses::listBlock()
{
	CString birthDate0;
	CString deathDate0;
	CString father0;
	CString birthDateF0;
	CString deathDateF0;
	CString mother0;
	CString birthDateM0;
	CString deathDateM0;
	CString source0;
	CString wedding0;
	CString wedding;

	CString birthDateJ;
	CString deathDateJ;
	CString fatherJ;
	CString birthDateFJ;
	CString deathDateFJ;
	CString motherJ;
	CString birthDateMJ;
	CString deathDateMJ;
	CString sourceJ;
	CString sourceFJ;
	CString sourceMJ;

	int nItemH;

	CString rowidBy;
	
	CString sex_id;

/*

	fwprintf( fh1, L"<br><font color='red'>%4d. %6s %6s %6s %s %s %s %-25s %-15s %-15s %1s %-25s %-15s %-15s %1s %-25s %-15s %-15s</font><br>",\
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
sourceF,\
father,\
birthDateF,\
deathDateF,\
sourceM,\
mother,\
birthDateM,\
deathDateM\
);
*/
	// az ember sora

	str.Format( L"%d", m_cnt + 1 );
	nItem = m_ListCtrl.InsertItem( nItem, str );
		
	m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );
	m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, tableNumber );
	m_ListCtrl.SetItemText( nItem, L_GENERATION, generation );

	m_ListCtrl.SetItemText( nItem, L_SOURCE, source );
	m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
	m_ListCtrl.SetItemText( nItem, L_UNITED, united );
	m_ListCtrl.SetItemText( nItem, L_NAME, name );
	m_ListCtrl.SetItemText( nItem, L_BIRTH, birth );
	m_ListCtrl.SetItemText( nItem, L_DEATH, death );

	m_ListCtrl.SetItemText( nItem, L_SOURCEF, sourceF );
	m_ListCtrl.SetItemText( nItem, L_ROWIDF, father_id );
	m_ListCtrl.SetItemText( nItem, L_FATHER, father );
	m_ListCtrl.SetItemText( nItem, L_BIRTH_F, birthDateF );
	m_ListCtrl.SetItemText( nItem, L_DEATH_F, deathDateF );

	m_ListCtrl.SetItemText( nItem, L_SOURCEM, sourceM );
	m_ListCtrl.SetItemText( nItem, L_ROWIDM, mother_id );
	m_ListCtrl.SetItemText( nItem, L_MOTHER, mother );
	m_ListCtrl.SetItemText( nItem, L_BIRTH_M, birthDateM );
	m_ListCtrl.SetItemText( nItem, L_DEATH_M, deathDateM );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBERF, lineNumberF );

	nItemH = nItem;
	m_ListCtrl.SetItemData( nItem, -1 );
	++nItem;


/*
	fwprintf( fh1, L"%12s %6s %6s %s %s %s %-25s %-15s %-15s %1s %-25s %-15s %-15s %1s %-25s %-15s %-15s<br>",\
vSameSpouses.at(0).rowid,\
vSameSpouses.at(0).lineNumber,\
vSameSpouses.at(0).tableNumber,\
vSameSpouses.at(0).generation,\
vSameSpouses.at(0).source,\
vSameSpouses.at(0).united,\
vSameSpouses.at(0).spouse,\
vSameSpouses.at(0).birthDate,\
vSameSpouses.at(0).deathDate,\
vSameSpouses.at(0).sourceF,\
vSameSpouses.at(0).father,\
vSameSpouses.at(0).birthDateF,\
vSameSpouses.at(0).deathDateF,\
vSameSpouses.at(0).sourceM,\
vSameSpouses.at(0).mother,\
vSameSpouses.at(0).birthDateM,\
vSameSpouses.at(0).deathDateM\
);
*/
	nItem = m_ListCtrl.InsertItem( nItem, L"" );
	
	// els� h�zast�rs
	rowidBy = vSameSpouses.at(0).rowid;
	m_ListCtrl.SetItemText( nItem, L_LINENUMBER, vSameSpouses.at(0).lineNumber );
	m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, vSameSpouses.at(0).tableNumber );
	m_ListCtrl.SetItemText( nItem, L_UNITED, vSameSpouses.at(0).united );
	m_ListCtrl.SetItemText( nItem, L_GENERATION, vSameSpouses.at(0).generation );
	m_ListCtrl.SetItemText( nItem, L_SOURCE, vSameSpouses.at(0).source );
	m_ListCtrl.SetItemText( nItem, L_ROWID, rowidBy );
	m_ListCtrl.SetItemText( nItem, L_NAME, vSameSpouses.at(0).spouse );
	m_ListCtrl.SetItemText( nItem, L_BIRTH, vSameSpouses.at(0).birthDate );
	m_ListCtrl.SetItemText( nItem, L_DEATH, vSameSpouses.at(0).deathDate );
	m_ListCtrl.SetItemText( nItem, L_WEDDING, vSameSpouses.at(0).wedding );

	m_ListCtrl.SetItemText( nItem, L_SOURCEF, vSameSpouses.at(0).sourceF );
	m_ListCtrl.SetItemText( nItem, L_ROWIDF, vSameSpouses.at(0).father_id );
	m_ListCtrl.SetItemText( nItem, L_FATHER, vSameSpouses.at(0).father );
	m_ListCtrl.SetItemText( nItem, L_BIRTH_F, vSameSpouses.at(0).birthDateF );
	m_ListCtrl.SetItemText( nItem, L_DEATH_F, vSameSpouses.at(0).deathDateF );

	m_ListCtrl.SetItemText( nItem, L_SOURCEM, vSameSpouses.at(0).sourceM );
	m_ListCtrl.SetItemText( nItem, L_ROWIDM, vSameSpouses.at(0).mother_id );
	m_ListCtrl.SetItemText( nItem, L_MOTHER, vSameSpouses.at(0).mother );
	m_ListCtrl.SetItemText( nItem, L_BIRTH_M, vSameSpouses.at(0).birthDateM );
	m_ListCtrl.SetItemText( nItem, L_DEATH_M, vSameSpouses.at(0).deathDateM );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBERF, vSameSpouses.at(0).lineNumberF );

	// �sszehasonl�tand� adatok
	source0		= vSameSpouses.at(0).source;
	birthDate0	= vSameSpouses.at(0).birthDate.Left(15);
	deathDate0	= vSameSpouses.at(0).deathDate.Left(15);
	wedding0	= vSameSpouses.at(0).wedding;


	father0		= vSameSpouses.at(0).father.Left(25);
	birthDateF0 = vSameSpouses.at(0).birthDateF;
	deathDateF0 = vSameSpouses.at(0).deathDateF;
	mother0		= vSameSpouses.at(0).mother.Left(25);
	birthDateM0 = vSameSpouses.at(0).birthDateM;
	deathDateM0 = vSameSpouses.at(0).deathDateM;


	source0.TrimRight();
	birthDate0.TrimRight();
	deathDate0.TrimRight();
	wedding0.Trim();
	father0.TrimRight();
	birthDateF0.TrimRight();
	deathDateF0.TrimRight();
	mother0.TrimRight();
	birthDateM0.TrimRight();
	deathDateM0.TrimRight();

	++nItem;



	int col = 0;
	int cnt = 1;
	int empty = 0;
	// tov�bbi h�zast�rsak
	for( UINT j = 1; j < vSameSpouses.size(); ++j )
	{
		col = 0;
		empty = 0;

		sourceJ		= vSameSpouses.at(j).source.Trim();
		sourceFJ	= vSameSpouses.at(j).sourceF;
		sourceMJ	= vSameSpouses.at(j).sourceM;
		birthDateJ	= vSameSpouses.at(j).birthDate.Left(15).Trim();
		deathDateJ	= vSameSpouses.at(j).deathDate.Left(15).Trim();
		wedding		= vSameSpouses.at(j).wedding;

		fatherJ		= vSameSpouses.at(j).father.Left(25).Trim();
		birthDateFJ = vSameSpouses.at(j).birthDateF.Trim();
		deathDateFJ = vSameSpouses.at(j).deathDateF.Trim();
		motherJ		= vSameSpouses.at(j).mother.Left(25).Trim();
		birthDateMJ = vSameSpouses.at(j).birthDateM.Trim();
		deathDateMJ = vSameSpouses.at(j).deathDateM.Trim();


// birthDate
		if( birthDate0.IsEmpty() || birthDateJ.IsEmpty() )
			++empty;
		if( !birthDate0.IsEmpty() && !birthDateJ.IsEmpty() && birthDate0 != birthDateJ )
			col = col | 1 << L_BIRTH;

// deathDate
		if( deathDate0.IsEmpty() || birthDateJ.IsEmpty() )
			++empty;
		if( !deathDate0.IsEmpty() && !deathDateJ.IsEmpty() && deathDate0 != deathDateJ )
			col = col | 1 << L_DEATH;

		// wedding
		if( !wedding0.IsEmpty() && !wedding.IsEmpty() && wedding0 != wedding )
		{
			col = col | 1 << L_WEDDING;
		}


// father
		if( father0.IsEmpty() || fatherJ.IsEmpty() )
			++empty;
		if( !father0.IsEmpty()&& !fatherJ.IsEmpty() && father0 !=  fatherJ )
			col = col | 1 << L_FATHER;

// father birthdate
		if( birthDateF0.IsEmpty() || birthDateFJ.IsEmpty() )
			++empty;
		if( !birthDateF0.IsEmpty() && !birthDateFJ.IsEmpty() && birthDateF0 != birthDateFJ )
			col = col | 1 << L_BIRTH_F;


// father deathdate
		if( deathDateF0.IsEmpty() || deathDateFJ.IsEmpty() )
			++empty;
		if( !deathDateF0.IsEmpty() && !deathDateFJ.IsEmpty() && deathDateF0 != deathDateFJ )
			col = col | 1 << L_DEATH_F;

// mother
		if( mother0.IsEmpty() || motherJ.IsEmpty() )
			++empty;
		if( !mother0.IsEmpty() && !motherJ.IsEmpty() && mother0 !=  motherJ )
			col = col | 1 << L_MOTHER;

// mother birthdate
		if( birthDateM0.IsEmpty() || birthDateMJ.IsEmpty() )
			++empty;
		if( !birthDateM0.IsEmpty() && !birthDateMJ.IsEmpty() && birthDateM0 != birthDateMJ )
			col = col | 1 << L_BIRTH_M;

// mother deathdate
		if( deathDateM0.IsEmpty() || deathDateMJ.IsEmpty() )
			++empty;
		if( !deathDateM0.IsEmpty() && !deathDateMJ.IsEmpty() &&  deathDateM0 != deathDateMJ )
			col = col | 1 << L_DEATH_M;
////////////////////////////////////////////////////////////////////////////////////////////

		nItem = m_ListCtrl.InsertItem( nItem, L"" );
		
		m_ListCtrl.SetItemText( nItem, L_LINENUMBER, vSameSpouses.at(j).lineNumber );
		m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, vSameSpouses.at(j).tableNumber );
		m_ListCtrl.SetItemText( nItem, L_UNITED, vSameSpouses.at(j).united );
		m_ListCtrl.SetItemText( nItem, L_GENERATION, vSameSpouses.at(j).generation );

		m_ListCtrl.SetItemText( nItem, L_SOURCE, vSameSpouses.at(j).source );
		m_ListCtrl.SetItemText( nItem, L_ROWID, vSameSpouses.at(j).rowid );
		m_ListCtrl.SetItemText( nItem, L_NAME, vSameSpouses.at(j).spouse );
		m_ListCtrl.SetItemText( nItem, L_BIRTH, vSameSpouses.at(j).birthDate );
		m_ListCtrl.SetItemText( nItem, L_DEATH, vSameSpouses.at(j).deathDate );
		m_ListCtrl.SetItemText( nItem, L_WEDDING, vSameSpouses.at(j).wedding );

		m_ListCtrl.SetItemText( nItem, L_SOURCEF, vSameSpouses.at(j).sourceF );
		m_ListCtrl.SetItemText( nItem, L_ROWIDF, vSameSpouses.at(j).father_id );
		m_ListCtrl.SetItemText( nItem, L_FATHER, fatherJ );
		m_ListCtrl.SetItemText( nItem, L_BIRTH_F, vSameSpouses.at(j).birthDateF );
		m_ListCtrl.SetItemText( nItem, L_DEATH_F, vSameSpouses.at(j).deathDateF );

		m_ListCtrl.SetItemText( nItem, L_SOURCEM, vSameSpouses.at(j).sourceM );
		m_ListCtrl.SetItemText( nItem, L_ROWIDM, vSameSpouses.at(j).mother_id );
		m_ListCtrl.SetItemText( nItem, L_MOTHER, motherJ );
		m_ListCtrl.SetItemText( nItem, L_BIRTH_M, birthDateMJ );
		m_ListCtrl.SetItemText( nItem, L_DEATH_M, deathDateMJ );

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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySpouses(const MORESPOUSES &v1, const MORESPOUSES &v2) 
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnInfo()
{
	AfxMessageBox( m_explanation, MB_ICONINFORMATION );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S � S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckSameSpouses::PreTranslateMessage(MSG* pMsg)
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnClickedKeress()
{
	keress( 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress( nItem + 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, L_NAME, start, true);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CRelatives dlg;
	int nItem = pNMItemActivate->iItem;
//	int nItem		= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rowid	= m_ListCtrl.GetItemText( nItem, L_ROWID );

//	dlg.nItem		= nItem;
	dlg.m_rowid		= rowid;
	if( dlg.DoModal() == IDCANCEL ) return;

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckSameSpouses:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CCheckSameSpouses::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html f�jlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijel�lt sor a ga.html f�jlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CEditHtmlLines dlg;
	dlg.m_title.Format( L"%s sz�lei �s testv�rei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnHtmlChildren()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CEditHtmlLines dlg;
	dlg.m_title.Format( L"%s �s gyermekei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_CHILDREN";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );

	CEditPeople dlg;
	dlg.m_caption.Format( L"%s adatainak szerkeszt�se", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_rowid = rowid;

	ShowWindow( SW_HIDE );
	dlg.DoModal();
	ShowWindow( SW_RESTORE );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSameSpouses::On3Generations()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
