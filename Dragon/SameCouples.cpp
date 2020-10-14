// CSameCouples.cpp : implementation file
//



#include "stdafx.h"
#include "Dragon.h"
#include "SameCouples.h"
#include "afxdialogex.h"
#include "checkParam0.h"
#include "GetLastFirst.h"
#include "Param1.h"
#include <algorithm>
#include <vector> 

enum
{
	P_ROWID = 0,
	P_LINE,
	P_GENERATION,
	P_SOURCE,
	P_UNITED,
	P_SEX_ID,
	P_LAST_NAME,
	P_FIRST_NAME,
	P_BIRTH_DATE,
	P_DEATH_DATE,
	P_FATHER_ID,
	P_MOTHER_ID,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortBySpouses(const COUPLES &v1, const COUPLES &v2) 
{ 
	if( v1.spouse1 < v2.spouse1 ) return true;
	if( v1.spouse1 == v2.spouse1 )
	{
		if( v1.spouse2 < v2.spouse2 ) return true;
		if( v1.spouse2 == v2.spouse2 ) 
		{
			return( v1.sourceS1 < v2.sourceS1 );
		}
		return false;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortByGroupStatus(const COUPLES &v1, const COUPLES &v2) 
{ 
	if( v1.group > v2.group )
		return false;
	if( v1.group == v2.group )
	{
		if( v1.status1 < v2.status1 )
			return true;
		else
			return false;
	}
	return true;
}
bool sortByGroup(const COUPLES &v1, const COUPLES &v2) 
{
	return( v1.group < v2.group );
}
bool sortByMarriage(const COUPLES &v1, const COUPLES &v2) 
{
	return( _wtoi( v1.rowidM ) < _wtoi( v2.rowidM ) );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSameCouples, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameCouples::CSameCouples(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSameCouples::IDD, pParent)
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;

		p_fields = L"\
rowid,\
lineNumber,\
generation,\
source,\
united,\
sex_id,\
last_name,\
first_name,\
birth_date,\
death_date,\
father_id,\
mother_id\
";

	m_description = L"\
Az oszlopok jelentése:\n\n\
marri  - a házasság azonosítója,\n\
gr     - group, az azonos nevû embercsoprton belül azonosnak éréklelt alcsoportok sorszáma.\n\n\
férj adatai:\n\
st     - status, az azonosítás eredménye: -1 azonos, azaz egyesített, majd törölt, 0: változatlanul hagyott, 1: ez az egyesített bejegyzés.\n\
line   - a GA html fájlban a sor száma, amelyben elõfordul.\n\
G      - generáció, az ember generációs jele a GA fájlban.\n\
S      - source, a név elõfordulásának hierarchia száma.\n\
U      - united, az ember u számú bejegyzésének összevonása.\n\
rowid  - a bejegyzés azonosítója.\n\
husband- a férj neve.\n\
birth  - születési dátum.\n\
death  - halálozás dátuma.\n\
father - a férj apjának neve\n\
mother - a férj anyjának neve.\n\n\
felség hasoló adatai.\n\n\
";

	m_info = L"\
Az alábbi adatokat vizsgáljuk az azonos nevû házaspárok azonosságának eldöntéséhez:\r\n\
\r\n\
1. Férj születési dátuma\r\n\
2. Férj halálozási dátuma\r\n\
3. Férj apja neve\r\n\
4. Férj anyja neve\r\n\
5. Feleség születési dátuma\r\n\
6. Feleség halálozási dátuma\r\n\
7. Feleség apja neve\r\n\
8. Feleség anyja neve\r\n\
\r\n\
A kettõs keresztnevekbõl csak az elsõ használjuk az összehasoonlításhoz.\r\n\r\n\
Ha a vizsgált adatok között ellentmondás van, akkor nyilvánvalóan nem azonos személy bejegyzéseirõl van szó.\r\n\
Ha nincs ellentmondás, akkor még megadhatjuk, hogy hány egyezést kívánunk meg az azonossághoz.\
\r\n\r\n\
";

	m_columns.Format( L"\n<b>\
%6s \
%2s \
%2s %8s %1s %1s %2s %8s %-30s %-13s %-13s %-30s %-30s \
%2s %8s %1s %1s %2s %8s %-30s %-13s %-13s %-30s %-30s</b>\n", \
L"marri",\
L"gr",\
L"st",L"line", L"G", L"S", L"U", L"rowid", L"husband", L"birth", L"death", L"father", L"mother", \
L"st",L"line", L"G", L"S", L"U", L"rowid", L"wife", L"birth", L"death", L"father", L"mother" );

//Ha csak egy házaspárt akatsz vizsgálni, add meg a nevüket

//	_husband	= L"Bartakovich Ferenc";
//	_wife		= L"Almássy Ágnes";


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameCouples::~CSameCouples()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSameCouples, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameCouples::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();


	CParam1 dlg;
	dlg.m_caption	= L"Azonos nevû házaspárok összevonása";
	dlg.m_info		= m_info;
	dlg.m_text		= L"Az azonossághoz szükséges egyezõ adatok száma:";
	dlg._default	= 1;
	if( dlg.DoModal() == IDCANCEL )
	{
		OnCancel();
		return false;
	}
	_azonos	= dlg._azonos;
	
//	createColumns();

	openUnited();
	openDifferent();

	m_deleted = 0;
	sameSpouses();

	if( m_deleted )
	{
		str.Format( L"%d ember összevonva.", m_deleted );
#ifndef _DEBUG
		wndP.SetText( L"Az adatbázis tömörítése..." );
#endif
		theApp.execute( L"VACUUM");
	}
	else
	{
		str = L"Nincs összevonható ember!";
	}
	fwprintf( fU, L"\n\n%s\n", str );
	fwprintf( fD, L"\n\n%s\n", str );
	fwprintf( fU, L"</pre>" );
	fwprintf( fD, L"</pre>" );
	fclose( fU );
	fclose( fD );

	theApp.execute( L"PRAGMA user_version='1'" );
	wndP.DestroyWindow();		

	theApp.showHtmlFile( differentSpec );
	theApp.showHtmlFile( unitedSpec );

	OnCancel();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CSameCouples::createColumns()
{


	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,				L"cnt",		LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,			L"marriage",	LVCFMT_RIGHT,	 60,-1,COL_NUM);

	m_ListCtrl.InsertColumn( L_GENERATIONS1,	L"G",		LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCES1,		L"S",		LVCFMT_LEFT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITEDS1,		L"U",		LVCFMT_RIGHT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINES1,			L"lineS1",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ROWIDS1,			L"rowidS1",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_SPOUSE1,			L"spouse1",	LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHS1,			L"birthS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_DEATHS1,			L"deathS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHERS1,		L"motherS1",LVCFMT_LEFT,	150,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( L_GENERATIONS2,	L"G",		LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCES2,		L"S",		LVCFMT_LEFT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITEDS2,		L"U",		LVCFMT_RIGHT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINES2,			L"lineS2",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ROWIDS2,			L"rowidS1",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_SPOUSE2,			L"spouse1",	LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHS2,			L"birthS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_DEATHS2,			L"deathS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHERS2,		L"motherS1",LVCFMT_LEFT,	150,-1,COL_TEXT);
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::sameSpouses()
{
	CString first_name1;
	CString	lastName;
	CString firstName;
	COUPLES vcouples;
	CString spouse;
	CString mother_id;
	CString father_id;

	int		z;

	wndP.Create( NULL, L"Azonos nevû házaspárok..." );
	wndP.GoModal();

	

#ifndef _DEBUG
	str.Format( L"Házasságok lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif

	m_command.Format( L"SELECT rowid,* FROM marriages" );
	if( !query( m_command ) ) return;

	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	vCouples.clear();
	for( int i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		vcouples.group		= 0;
		vcouples.status1	= 0;
		vcouples.status2	= 0;
		
		vcouples.rowidM		= m_recordset->GetFieldString( MARRIAGES_ROWID );
		vcouples.dateM		= m_recordset->GetFieldString( MARRIAGES_DATE );
		vcouples.rowidS1	= m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		vcouples.rowidS2	= m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
	
//férj
		m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", p_fields, vcouples.rowidS1 );
		if( !query1( m_command ) ) return;

		vcouples.sex_idS1		= m_recordset1->GetFieldString( P_SEX_ID );
		vcouples.lineS1			= m_recordset1->GetFieldString( P_LINE );
		vcouples.generationS1	= m_recordset1->GetFieldString( P_GENERATION );
		vcouples.sourceS1		= m_recordset1->GetFieldString( P_SOURCE );
		vcouples.unitedS1		= m_recordset1->GetFieldString( P_UNITED );

		lastName				= m_recordset1->GetFieldString( P_LAST_NAME);
		firstName				= m_recordset1->GetFieldString( P_FIRST_NAME );
		str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
		str.TrimRight();
		if( str.IsEmpty() ) goto cont;
		vcouples.spouse1		= str;

		if( str == L"Harmos Tamás" )
			z = 1;


		vcouples.birthS1		= m_recordset1->GetFieldString( P_BIRTH_DATE );
		vcouples.deathS1		= m_recordset1->GetFieldString( P_DEATH_DATE );
		mother_id				= m_recordset1->GetFieldString( P_MOTHER_ID );
		vcouples.mother_idS1	= mother_id;
		father_id				= m_recordset1->GetFieldString( P_FATHER_ID );
		vcouples.father_idS1	= father_id;

// ez nem jó		if( !_husband.IsEmpty() && spouse != _husband ) goto cont;  // ha csak egy nevet akarunk vizsgálni


		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", mother_id );
		if( !query1( m_command ) ) return;
		str.Empty();
		if( m_recordset1->RecordsCount() )
		{
			lastName	= m_recordset1->GetFieldString( 0 );
			firstName	= m_recordset1->GetFieldString( 1 );
			str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
			str.TrimRight();
		}
		vcouples.motherS1 = str;

		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", father_id );
		if( !query1( m_command ) ) return;
		str.Empty();
		if( m_recordset1->RecordsCount() )
		{
			lastName	= m_recordset1->GetFieldString( 0 );
			firstName	= m_recordset1->GetFieldString( 1 );
			str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
			str.TrimRight();
		}
		vcouples.fatherS1 = str;

		vcouples.generationS1.Trim();

// feleség
		m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", p_fields, vcouples.rowidS2 );
		if( !query1( m_command ) ) return;

		vcouples.sex_idS2		= m_recordset1->GetFieldString( P_SEX_ID );
		vcouples.lineS2			= m_recordset1->GetFieldString( P_LINE );
		vcouples.generationS2	= m_recordset1->GetFieldString( P_GENERATION );
		vcouples.sourceS2		= m_recordset1->GetFieldString( P_SOURCE );
		vcouples.unitedS2		= m_recordset1->GetFieldString( P_UNITED );

		lastName				= m_recordset1->GetFieldString( P_LAST_NAME);
		firstName				= m_recordset1->GetFieldString( P_FIRST_NAME );
		str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
		str.TrimRight();
		if( str.IsEmpty() ) goto cont;
		vcouples.spouse2		= str;

		father_id				= m_recordset1->GetFieldString( P_FATHER_ID );
		vcouples.father_idS2	= father_id;
		vcouples.birthS2		= m_recordset1->GetFieldString( P_BIRTH_DATE );
		vcouples.deathS2		= m_recordset1->GetFieldString( P_DEATH_DATE );
		mother_id				= m_recordset1->GetFieldString( P_MOTHER_ID );
		vcouples.mother_idS2	= mother_id;

// ez nem jó		if( !_wife.IsEmpty() && spouse != _wife ) goto cont;

		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", mother_id );
		if( !query1( m_command ) ) return;
		str.Empty();
		if( m_recordset1->RecordsCount() )
		{
			lastName = m_recordset1->GetFieldString( 0 );
			firstName = m_recordset1->GetFieldString( 1 );
			str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
			str.TrimRight();
		}
		vcouples.motherS2 = str;

		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", father_id );
		if( !query1( m_command ) ) return;
		str.Empty();
		if( m_recordset1->RecordsCount() )
		{
			lastName = m_recordset1->GetFieldString( 0 );
			firstName = m_recordset1->GetFieldString( 1 );
			str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
			str.TrimRight();
			vcouples.fatherS2 = str;
		}
		vcouples.fatherS2 = str;
		
		vcouples.generationS2.Trim();;
		vcouples.cnt = i;
		vCouples.push_back( vcouples );
cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

	

#ifndef _DEBUG
	str.Format( L"Lekérdezett adatok rendezése..." );
	wndP.SetText( str );
#endif

	wndP.SetPos(0);
	std::sort( vCouples.begin(), vCouples.end(), sortBySpouses );   // házaspárok név szerinti rendezése

#ifndef _DEBUG
	str.Format( L"Azonos házastársak és házasságok összevonása..." );
	wndP.SetText( str );
#endif

	getSameCouples();
	wndP.SetPos(0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::getSameCouples()
{

	CString spouse1;
	CString spouse2;
	CString name;
	CString namePrev(L"");
	bool	first = true;
	
	theApp.execute( L"BEGIN" );

	wndP.SetRange( 0, vCouples.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	vSame.clear();
	vDeleted.clear();
	for( ic = 0; ic < vCouples.size(); ++ic )
	{
		spouse1 =  vCouples.at(ic).spouse1;
		spouse2 =  vCouples.at(ic).spouse2;

		if( !_husband.IsEmpty()  && _husband != spouse1 ) goto cont;
		if( !_wife.IsEmpty() && _wife != spouse2 ) goto cont;
		name.Format( L"%-40s %-40s", spouse1, spouse2 );
		if( name == namePrev )
		{
			if( first )			// a csoport elsõ házaspárjának mentése
			{
				vSame.push_back( vCouples.at(ic-1) );
				first = false;
			}
			vSame.push_back( vCouples.at(ic) );
		}
		else
		{
			if( vSame.size() )
			{
				processSame();
				vSame.clear();
			}
			first = true;
		}
		namePrev = name;

cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if( vSame.size() )
	{
		processSame();
		vSame.clear();
	}
	theApp.execute( L"COMMIT" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Egy vSame-ben adott azonos nevû házastársak rekordjaiban beállítja a group, statusS1 és statusS2 értékeit.
// A group 0-n értéket vehet fel, és a vSame-en belüli lehetséges csoportokat jelöli. Egy-egy group értékû házaspárok azonosak.  
// 
void CSameCouples::processSame()
{

	UINT group = 1;
	UINT db = 0;
	int z;


	if( vSame.at(0).spouse1 == L"Aisdorfer Sámuel"  && vSame.at(0).spouse2 == L"Pottornyay Zsuzsa" )
		z = 1;

	for( UINT i1 = 0; i1 < vSame.size(); ++i1 )
	{
		if( db ) ++group;
		db = 0;
		if( vSame.at(i1).group == 0 )		// az i1. házaspárt még nem rendelték hozzá egyik csoporthoz sem
		{
			for( UINT i2 = 0; i2 < vSame.size(); ++i2 )
			{
				if( i1 != i2 && vSame.at(i2).group == 0 )	// természetesen csak különbözõ házaspárokat vizsgál, amelyeket még nem redneltek hozzá egyik csooprthoz sem
				{
					if( identical( i1, i2 ) )
					{
						vSame.at( i1 ).group = group;
						vSame.at( i2 ).group = group;
						++db;
					}
				}
 			}
		}
	}



	// a legalacsonyabb szerep kiválasztása csoportonként
	// status1 = 1, a legalacsonyabb szerepre, ami megtartásra kerül
	// status1 = -1, az össze stöbbi, ami törlésre kerül.

	m_numOfGroups = getNumOfGroups();
	UINT ix1;
	UINT ix2;
	int sourceMin1;
	int sourceMin2;
	int sourceS1;
	int sourceS2;


	std::sort( vSame.begin(), vSame.end(), sortByMarriage );  //cnt a vCouples rekord sorszáma!! Erre valamiért szükség van!!!
	std::sort( vSame.begin(), vSame.end(), sortByGroup );		// ez nem kell talán, bár?
	// csoport legalacsonyabb szerepû indexének meghatározása
	for( UINT i = 1; i < m_numOfGroups + 1; ++i )
	{
		sourceMin1 = INT_MAX;
		sourceMin2 = INT_MAX;
		ix1 = 0;
		ix2 = 0;
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			str = vSame.at(j).spouse1;
			if( str == L"Amadé István" )
				z = 1;
			if( vSame.at(j).group == i )
			{
				sourceS1 = _wtoi( vSame.at(j).sourceS1 );
				if( sourceS1 < sourceMin1 )	ix1 = j;
				sourceMin1 = __min( sourceMin1, sourceS1 );
			
				sourceS2 = _wtoi( vSame.at(j).sourceS2 );
				if( sourceS2 < sourceMin2 )	ix2 = j;
				sourceMin2 = __min( sourceMin2, sourceS2 );
			}
		}
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == i )
			{
				if( j == ix1 ) vSame.at(j).status1 = 1;
				else vSame.at(j).status1 = -1;
				if( j == ix2 ) vSame.at(j).status2 = 1;
				else vSame.at(j).status2 = -1;
			}
		}
	}
	contract();

	if( m_contracted )
		listUnited();
	else
		listDiff();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Azonos nevû és születési, halálozási dátumokban nem ellentmondásos házaspárok házassáágainak egyesítése
// A spouse1-bõl és a spouse2-bõl a legkisebb source-ú embert tartja meg
void CSameCouples::contract()
{
	CString rowidBy;
	CString rowid;
	CString rowidM;
	CString sex_id;
	CString name;
	int		source;
	CString rowidMD;
	int		group;
	int		z;

	m_contracted = 0;
	// csoporton belül keresi a törlendõ és megtartandó sorokat

	for( UINT i = 0; i < m_numOfGroups; ++i )
	{
		group = i + 1;
		// megtartandó férj bejegyzés
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == group && vSame.at(j).status1 == 1 )
			{
				rowidBy = vSame.at(j).rowidS1;
				rowidM  = vSame.at(j).rowidM;
				source	= _wtoi( vSame.at(j).sourceS1 );
			}
		}

		if( rowidBy.IsEmpty() ) 
			return;

		// törlendõ férj bejegyzés
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == group )
			{
				if( vSame.at(j).status1 == -1 )
				{
					rowidMD = vSame.at(j).rowidM;
					rowid	= vSame.at(j).rowidS1;
					sex_id	= vSame.at(j).sex_idS1;

// Lajos +Késmárk 1924 vasúti fõellenõr 1=N N 2=N N
// Két házasság, amelyben Lajos rowid-ja ugyanaz, a feleségeké különbözõ, de a két házasság egy csoportban van!!
// Ugynazt a rowid-t meg akrjuk tartani és törölni is akarjuk! Megáll az ész!!
// Ezeket nem lehet összevonni!!!
//
					if( rowid == rowidBy ) break;   // férj ugyanaz, õt nem kell összevonni, de a feleségét még lehet!!

					ReplaceSpouse1( rowid, rowidBy, vSame.at(j).spouse1, source );
					theApp.replaceBy( rowid, rowidBy, sex_id, source );

					vDeleted.push_back( rowid );	// a törölt rowid-k gyûjtése

					m_command.Format( L"DELETE FROM marriages WHERE rowid ='%s'", rowidMD );
					if( !theApp.execute( m_command ) ) return;

					++m_deleted;
					++m_contracted;
				}
			}
		}

		// negtartandó feleség bejegyzés
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == group && vSame.at(j).status2 == 1 )  // miért nem lép ki, ha megvan?
			{
				rowidBy = vSame.at(j).rowidS2;
				rowidM  = vSame.at(j).rowidM;
			}
		}

		if( rowidBy.IsEmpty() ) 
			return;

		// törlendõ feleség bejegyzés
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == group )
			{
				if( vSame.at(j).status2 == -1 )
				{
					rowidMD = vSame.at(j).rowidM;
					rowid	= vSame.at(j).rowidS2;
					sex_id	= vSame.at(j).sex_idS2;
					source	= _wtoi( vSame.at(j).sourceS2 );

					if( rowid == rowidBy ) break;  // feleség ugyanaz
////					ReplaceSpouse2( rowid, rowidBy, vSame.at(j).spouse2, source );
					theApp.replaceBy( rowid, rowidBy, sex_id, source );

					// ha a férj rowid-ja ugynanaz volt, de a feleségé különbözõ, akkor a házasságot is most kell törölni!
					m_command.Format( L"DELETE FROM marriages WHERE rowid ='%s'", rowidMD );
					if( !theApp.execute( m_command ) ) return;

					++m_deleted;
					++vSame.at(0).contracted;
					++m_contracted;
				}
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ha a házaspárok között talál olyan bejegyzést, ahol a férj azonosítója megegyezik a törlendõ bejegyzés azonosítójával, 
// akkor annak rowid-ját a megmaradó bejegyzés azonosítójára módosítja 
// A vCouples spouse1 szerint van rendezve, ezért elegendõ a pillanatnyi bejegyzástõl kezdeni és addig pörögni,
// amíg a férj neve azonos.
void CSameCouples::ReplaceSpouse1( CString rowid, CString rowidBy, CString name, int source )
{
 	for( UINT i = ic+1; i < vCouples.size(); ++i )
	{
		if( vCouples.at(i).spouse1 != name )
			break;
		if( vCouples.at(i).rowidS1 == rowid )
		{
			vCouples.at(i).rowidS1	= rowidBy;
			str.Format( L"%d", source );
			vCouples.at(i).sourceS1	= str;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::ReplaceSpouse2( CString rowid, CString rowidBy, CString name, int source )
{
	for( UINT i = 0; i < vCouples.size(); ++i )
	{
		if( vCouples.at(i).rowidS2 == rowid )
		{
			vCouples.at(i).rowidS2 = rowidBy;
			str.Format( L"%d", source );
			vCouples.at(i).sourceS2 = str;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CSameCouples::referenceValues()
{
	birth1Ref.Empty();
	death1Ref.Empty();
	mother1Ref.Empty();
	birth2Ref.Empty();
	death2Ref.Empty();
	mother2Ref.Empty();
	for( UINT i = 0; i < vSame.size(); ++i )
	{
		if( !vSame.at(i).birthS1.IsEmpty() ) birth1Ref = vSame.at(i).birthS1;
		if( !vSame.at(i).deathS1.IsEmpty() ) death1Ref = vSame.at(i).deathS1;
		if( !vSame.at(i).motherS1.IsEmpty() ) mother1Ref = vSame.at(i).motherS1;
		
		if( !vSame.at(i).birthS2.IsEmpty() ) birth2Ref = vSame.at(i).birthS2;
		if( !vSame.at(i).deathS2.IsEmpty() ) death2Ref = vSame.at(i).deathS2;
		if( !vSame.at(i).motherS2.IsEmpty() ) mother2Ref = vSame.at(i).motherS2;
	}
}
*/
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az i1 és i2 indexû házasspárok azonosságát állapíthja meg.
// return true: azonosak (nincs ellentmondás az adatai között!!!) 
// return false: különbözõek
bool CSameCouples::identical( UINT i1, UINT i2 )
{
	setData( i1, i2 );

	if( !generationS1_1.IsEmpty() && !generationS1_2.IsEmpty() &&  generationS1_1 != generationS1_2 ) return false;	// különbözû generációk,különbözõ emberek.

	if( ( birthS1_1.IsEmpty() || birthS1_2.IsEmpty() ) || birthS1_1 == birthS1_2 )
	{
		if( ( deathS1_1.IsEmpty() || deathS1_2.IsEmpty() ) ||  deathS1_1 == deathS1_2 )
		{
			if( ( motherS1_1.IsEmpty() || motherS1_2.IsEmpty() ) ||  motherS1_1 == motherS1_2 )
			{
				if( ( birthS2_1.IsEmpty() || birthS2_2.IsEmpty() ) || birthS2_1 == birthS2_2 )
				{
					if( ( deathS2_1.IsEmpty() || deathS2_2.IsEmpty() ) ||  deathS2_1 == deathS2_2 )
					{
						if( ( motherS2_1.IsEmpty() || motherS2_2.IsEmpty() ) ||  motherS2_1 == motherS2_2 )
						{
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az i1 és i2 indexû házasspárok azonosságát állapíthja meg.
// return true: azonosak (nincs ellentmondás az adatai között!!!) 
// return false: különbözõek
bool CSameCouples::identical( UINT i1, UINT i2 )
{
	setData( i1, i2 );

	int z;
	
	int	good = 0;
	int b1;
	int d1;
	int f1;
	int m1;
	int b2;
	int d2;
	int f2;
	int m2;

	if( ( b1 = birth1() ) == -1 ) return false;  // ellentmondás
	else
	{
		if( b1 == 1 ) ++good;
	}
	if( ( d1 = death1() ) == -1 ) return false;	// ellentmondás
	else
	{
		if( d1 == 1 ) ++good;
	}
	if( ( m1 = father1() ) == -1 ) return false;	// ellentmondás
	else
	{
		if( m1 == 1 ) ++good;
	}
	if( ( m1 = mother1() ) == -1 ) return false;	// ellentmondás
	else
	{
		if( m1 == 1 ) ++good;
	}

	if( ( b1 = birth2() ) == -1 ) return false;  // ellentmondás
	else
	{
		if( b1 == 1 ) ++good;
	}
	if( ( d1 = death2() ) == -1 ) return false;	// ellentmondás
	else
	{
		if( d1 == 1 ) ++good;
	}
	if( ( m1 = father2() ) == -1 ) return false;	// ellentmondás
	else
	{
		if( m1 == 1 ) ++good;
	}
	if( ( m1 = mother2() ) == -1 ) return false;	// ellentmondás
	else
	{
		if( m1 == 1 ) ++good;
	}

	if( _azonos <= good ) return true;				// legalább egy egyezés ellentmondás nélkül 
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  1 : mindkettõ meg van adva és egyezik;
//  0 : csak az egyik vagy egyse van megadva, így nincs ellentmondás
//  -1 : mindkettõ meg van adva és nem egyezik
int CSameCouples::birth1( )
{
	if( !birthS1_1.IsEmpty() && !birthS1_2.IsEmpty() )
	{
		if( birthS1_1 == birthS1_2 )
			return 1;
		else
			return -1;
	}
	return 0;
}
int CSameCouples::death1( )
{
	if( !deathS1_1.IsEmpty() && !deathS1_2.IsEmpty() )
	{
		if( deathS1_1 == deathS1_2 )
			return 1;
		else
			return -1;
	}
	return 0;
}
int CSameCouples::father1( )
{
	if(  !fatherS1_1.IsEmpty() && !fatherS1_2.IsEmpty() )
	{
		if( fatherS1_1 == fatherS1_2 )
			return 1;
		else
			return -1;
	}
	return 0;
}
int CSameCouples::mother1( )
{
	if(  !motherS1_1.IsEmpty() && !motherS1_2.IsEmpty() )
	{
		if( motherS1_1 == motherS1_2 )
			return 1;
		else
			return -1;
	}
	return 0;
}
///
int CSameCouples::birth2( )
{
	if( !birthS2_1.IsEmpty() && !birthS2_2.IsEmpty() )
	{
		if( birthS2_1 == birthS2_2 )
			return 1;
		else
			return -1;
	}
	return 0;
}
int CSameCouples::death2( )
{
	if( !deathS2_1.IsEmpty() && !deathS2_2.IsEmpty() )
	{
		if( deathS2_1 == deathS2_2 )
			return 1;
		else
			return -1;
	}
	return 0;
}
int CSameCouples::father2( )
{
	if(  !fatherS2_1.IsEmpty() && !fatherS2_2.IsEmpty() )
	{
		if( fatherS2_1 == fatherS2_2 )
			return 1;
		else
			return -1;
	}
	return 0;
}
int CSameCouples::mother2( )
{
	if(  !motherS2_1.IsEmpty() && !motherS2_2.IsEmpty() )
	{
		if( motherS2_1 == motherS2_2 )
			return 1;
		else
			return -1;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::setData( UINT i1, UINT i2 )
{
	group_1			= vSame.at(i1).group;
	rowidM_1		= vSame.at(i1).rowidM;
	dateM_1			= vSame.at(i1).dateM;

	statusS1_1		= vSame.at(i1).status1;
	rowidS1_1		= vSame.at(i1).rowidS1;
	sex_idS1_1		= vSame.at(i1).sex_idS1;
	generationS1_1	= vSame.at(i1).generationS1;
	sourceS1_1		= vSame.at(i1).sourceS1;
	unitedS1_1		= vSame.at(i1).unitedS1;
	lineS1_1		= vSame.at(i1).lineS1;
	spouse1_1		= vSame.at(i1).spouse1;
	birthS1_1		= vSame.at(i1).birthS1;
	deathS1_1		= vSame.at(i1).deathS1;
	motherS1_1		= vSame.at(i1).motherS1;
	fatherS1_1		= vSame.at(i1).fatherS1;

	statusS2_1		= vSame.at(i1).status2;
	rowidS2_1		= vSame.at(i1).rowidS2;
	sex_idS2_1		= vSame.at(i1).sex_idS2;
	generationS2_1	= vSame.at(i1).generationS2;
	sourceS2_1		= vSame.at(i1).sourceS2;
	unitedS2_1		= vSame.at(i1).unitedS2;
	lineS2_1		= vSame.at(i1).lineS2;
	spouse2_1		= vSame.at(i1).spouse2;
	birthS2_1		= vSame.at(i1).birthS2;
	deathS2_1		= vSame.at(i1).deathS2;
	motherS2_1		= vSame.at(i1).motherS2;
	fatherS2_1		= vSame.at(i1).fatherS2;

	group_2			= vSame.at(i2).group;
	rowidM_2		= vSame.at(i2).rowidM;
	dateM_2			= vSame.at(i2).dateM;

	statusS1_2		= vSame.at(i2).status1;
	rowidS1_2		= vSame.at(i2).rowidS1;
	sex_idS1_2		= vSame.at(i2).sex_idS1;
	generationS1_2	= vSame.at(i2).generationS1;
	sourceS1_2		= vSame.at(i2).sourceS1;
	unitedS1_2		= vSame.at(i2).unitedS1;
	lineS1_2		= vSame.at(i2).lineS1;
	spouse1_2		= vSame.at(i2).spouse1;
	birthS1_2		= vSame.at(i2).birthS1;
	deathS1_2		= vSame.at(i2).deathS1;
	motherS1_2		= vSame.at(i2).motherS1;
	fatherS1_2		= vSame.at(i2).fatherS1;
	
	statusS2_2		= vSame.at(i2).status2;
	rowidS2_2		= vSame.at(i2).rowidS2;
	sex_idS2_2		= vSame.at(i2).sex_idS2;
	generationS2_2	= vSame.at(i2).generationS2;
	sourceS2_2		= vSame.at(i2).sourceS2;
	unitedS2_2		= vSame.at(i2).unitedS2;
	lineS2_2		= vSame.at(i2).lineS2;
	spouse2_2		= vSame.at(i2).spouse2;
	birthS2_2		= vSame.at(i2).birthS2;
	deathS2_2		= vSame.at(i2).deathS2;
	motherS2_2		= vSame.at(i2).motherS2;
	fatherS2_2		= vSame.at(i2).fatherS2;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT CSameCouples::getNumOfGroups()
{
	UINT max = 0;
	for( UINT i = 0; i < vSame.size(); ++i )
	{ 
		max = __max( max, vSame.at(i).group );
	}
	return max;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::listDiff()
{
//	int nItem = 0;

	bool first;
	UINT  ix;
	for( UINT j = 0; j <= m_numOfGroups; ++j )
	{
		first = true;
		for( UINT i = 0; i < vSame.size();++i )
		{
			if( vSame.at(i).group == j )
			{
				if( first )
				{
					ix = i;
					first = false;
				}
				setData( ix, i ); 
				printYellow( i, nItem );
				++nItem;
			}
		}
	}
//	nItem = m_ListCtrl.InsertItem( nItem, L"" );
//	++nItem;
	fwprintf( fD, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::printYellow( UINT i, int nItem )
{
//	int		col;
//	int		empty;
//	CString cnt;

//	col = 0;
//	empty = 0;
//	cnt.Format( L"%d",	m_cnt );

	str.Format( L"%6s %2d %2d %8s %12s ", rowidM_2, group_2, statusS1_2, lineS1_2, dateM_2 );
	fwprintf( fD, L"%s", str );

//	if( generationS1_1.IsEmpty() || generationS1_2.IsEmpty() )
//	{
//		++empty;
//	}
	if( i && !generationS1_1.IsEmpty() && !generationS1_2.IsEmpty() && generationS1_1 != generationS1_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%1s</span> ", generationS1_2 );
//		col = col | 1 << L_GENERATIONS1;
	}
	else
		fwprintf( fD, L"%1s ", generationS1_2 );

	str.Format( L"%1s %2s %8s %-30s ", sourceS1_2, unitedS1_2, rowidS1_2, spouse1_2 );
	fwprintf( fD, L"%s", str );


// birthDate1
	if( !birthS1_1.IsEmpty() && !birthS1_2.IsEmpty() && birthS1_1 != birthS1_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", birthS1_2 );
//		col = col | 1 << L_BIRTHS1;
	}
	else
		fwprintf( fD, L"%-13s ", birthS1_2 );

// deathDate1
	if( !deathS1_1.IsEmpty() && !deathS1_2.IsEmpty() && deathS1_1 != deathS1_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", deathS1_2 );
//		col = col | 1 << L_DEATHS1;
	}
	else
		fwprintf( fD, L"%-13s ", deathS1_2 );

// father1
	if( !fatherS1_1.IsEmpty() && !fatherS1_2.IsEmpty() && fatherS1_1 != fatherS1_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", fatherS1_2 );
//		col = col | 1 << L_FATHERS1;
	}
	else
		fwprintf( fD, L"%-30s ", fatherS1_2 );

// mother1
	if( !motherS1_1.IsEmpty() && !motherS1_2.IsEmpty() && motherS1_1 != motherS1_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", motherS1_2 );
//		col = col | 1 << L_MOTHERS1;
	}
	else
		fwprintf( fD, L"%-30s ", motherS1_2 );


	str.Format( L"%2d %8s ", statusS2_2, lineS2_2 );
	fwprintf( fD, L"%s", str );

// generation
	if( i & !generationS2_1.IsEmpty() && !generationS2_2.IsEmpty() && generationS2_1 != generationS2_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%1s</span> ", generationS2_2 );
//		col = col | 1 << L_GENERATIONS2;
	}
	else
		fwprintf( fD, L"%-1s ", generationS2_2 );

	str.Format( L"%1s %2s %8s %-30s ", sourceS2_2, unitedS2_2, rowidS2_2, spouse2_2 );
	fwprintf( fD, L"%s", str );

// birthDate2
	if( !birthS2_1.IsEmpty() && !birthS2_2.IsEmpty() && birthS2_1 != birthS2_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", birthS2_2 );
//		col = col | 1 << L_BIRTHS2;
	}
	else
		fwprintf( fD, L"%-13s ", birthS2_2 );


// deathDate2
	if( !deathS2_1.IsEmpty() && !deathS2_2.IsEmpty() && deathS2_1 != deathS2_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", deathS2_2 );
//		col = col | 1 << L_DEATHS2;
	}
	else
		fwprintf( fD, L"%-13s ", deathS2_2 );

// father2
	if( !fatherS2_1.IsEmpty() && !fatherS2_2.IsEmpty() && fatherS2_1 != fatherS2_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", fatherS2_2 );
//		col = col | 1 << L_FATHERS2;
	}
	else
		fwprintf( fD, L"%-30s ", fatherS2_2 );

// mother2
	if( !motherS2_1.IsEmpty() && !motherS2_2.IsEmpty() && motherS2_1 != motherS2_2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", motherS2_2 );
//		col = col | 1 << L_MOTHERS2;
	}
	else
		fwprintf( fD, L"%-30s ", motherS2_2 );

	fwprintf( fD, L"\n");
/*
	int n = m_ListCtrl.InsertItem( nItem, cnt );

	m_ListCtrl.SetItemText( n, L_ROWID, rowidM_2 );

	m_ListCtrl.SetItemText( n, L_ROWIDS1, rowidS1_2 );
	m_ListCtrl.SetItemText( n, L_GENERATIONS1, generationS1_2 );
	m_ListCtrl.SetItemText( n, L_SOURCES1, sourceS1_2 );
	m_ListCtrl.SetItemText( n, L_UNITEDS1, unitedS1_2 );
	m_ListCtrl.SetItemText( n, L_LINES1, lineS1_2 );
	m_ListCtrl.SetItemText( n, L_SPOUSE1, spouse1_2 );
	m_ListCtrl.SetItemText( n, L_BIRTHS1, birthS1_2 );
	m_ListCtrl.SetItemText( n, L_DEATHS1, deathS1_2 );
	m_ListCtrl.SetItemText( n, L_MOTHERS1, motherS1_2 );

	m_ListCtrl.SetItemText( n, L_ROWIDS2, rowidS2_2 );
	m_ListCtrl.SetItemText( n, L_GENERATIONS2, generationS2_2 );
	m_ListCtrl.SetItemText( n, L_SOURCES2, sourceS2_2 );
	m_ListCtrl.SetItemText( n, L_UNITEDS2, unitedS2_2 );
	m_ListCtrl.SetItemText( n, L_LINES2, lineS2_2 );
	m_ListCtrl.SetItemText( n, L_SPOUSE2, spouse2_2 );
	m_ListCtrl.SetItemText( n, L_BIRTHS2, birthS2_2 );
	m_ListCtrl.SetItemText( n, L_DEATHS2, deathS2_2 );
	m_ListCtrl.SetItemText( n, L_MOTHERS2, motherS2_2 );

	m_ListCtrl.SetItemData( n, col );
*/
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::listUnited()
{
	CString rowidM;
	CString	dateM;
	UINT	group;
	
	int		status1;
	CString rowidS1;
	CString generationS1;
	CString sourceS1;
	CString unitedS1;
	CString lineS1;
	CString spouse1;
	CString birthS1;
	CString deathS1;
	CString fatherS1;
	CString motherS1;

	int		status2;
	CString rowidS2;
	CString generationS2;
	CString sourceS2;
	CString unitedS2;
	CString lineS2;
	CString spouse2;
	CString birthS2;
	CString deathS2;
	CString fatherS2;
	CString motherS2;
	UINT numOfGroups = getNumOfGroups();

	std::sort( vSame.begin(), vSame.end(), sortByGroupStatus );

	for( UINT i = 0; i < vSame.size();++i )
	{
		rowidM			= vSame.at(i).rowidM;
		dateM			= vSame.at(i).dateM;
		group			= vSame.at(i).group;
//férj
		status1			= vSame.at(i).status1;
		rowidS1			= vSame.at(i).rowidS1;
		generationS1	= vSame.at(i).generationS1;
		sourceS1		= vSame.at(i).sourceS1;
		unitedS1		= vSame.at(i).unitedS1;
		lineS1			= vSame.at(i).lineS1;
		spouse1			= vSame.at(i).spouse1;
		birthS1			= vSame.at(i).birthS1;
		deathS1			= vSame.at(i).deathS1;
		fatherS1		= vSame.at(i).fatherS1;
		motherS1		= vSame.at(i).motherS1;
//feleség
		status2			= vSame.at(i).status2;
		rowidS2			= vSame.at(i).rowidS2;
		generationS2	= vSame.at(i).generationS2;
		sourceS2		= vSame.at(i).sourceS2;
		unitedS2		= vSame.at(i).unitedS2;
		lineS2			= vSame.at(i).lineS2;
		spouse2			= vSame.at(i).spouse2;
		birthS2			= vSame.at(i).birthS2;
		deathS2			= vSame.at(i).deathS2;
		fatherS2		= vSame.at(i).fatherS2;
		motherS2		= vSame.at(i).motherS2;

//férj
		str.Format( L"\
%6s %2d \
%2d %8s %1s %1s %2s %12s \
%8s %-30s %-13s %-13s %-30s %-30s",\
rowidM, group, \
status1, lineS1, generationS1, sourceS1, unitedS1, dateM,\
rowidS1, spouse1, birthS1, deathS1, fatherS1, motherS1 );
		if( status1 == 1 )
			fwprintf( fU, L"<span style=\"background:aquamarine\">%s</span> ", str );
		else if( status1 == -1 )
			fwprintf( fU, L"<span style=\"background:LightGray\">%s</span> ", str );
		else
			fwprintf( fU, L"%s ", str );
//feleség
		str.Format( L"\
%2d %8s %1s %1s %2s \
%8s %-30s %-13s %-13s %-30s %-30s",\
status2, lineS2, generationS2, sourceS2, unitedS2, \
rowidS2, spouse2, birthS2, deathS2, fatherS2, motherS2 );
		if( status2 == 1 )
			fwprintf( fU, L"<span style=\"background:aquamarine\">%s</span> ", str );
		else if( status2 == -1 )
			fwprintf( fU, L"<span style=\"background:LightGray\">%s</span> ", str );
		else
			fwprintf( fU, L"%s ", str );

		fwprintf( fU, L"\n");
	}
	fwprintf( fU, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::openUnited()
{
	CString fileName = L"couplesUnited";
	unitedSpec = theApp.openHtmlFile( &fU, fileName, L"w+" );

	createHead( L"AZONOS NEVÛ HÁZASPÁROK, AKIK AZONOSAK, EZÉRT ÖSSZEVONHATÓAK" ); 

	fwprintf( fU, m_head );
	fwprintf( fU, m_description );

	str = L"A szürke hátterû bejegyzéseket egyesítettük a zöld hátterû bejegyzéssel. A fehér hátterû bejegyzések változatlanok maradtak.\n\
Ha egy azonos nevû csoportban több különbözõ egyesítés lehetséges, akkor azok a 'gr'-group oszlopba található számmal vannak meglülönböztetve.\n\n";
	fwprintf( fU, str );

	fwprintf( fU, m_columns );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::openDifferent()
{
	CString fileName = L"couplesDifferent";
	differentSpec = theApp.openHtmlFile( &fD, fileName, L"w+" );

	createHead( L"AZONOS NEVÛ HÁZASPÁROK, AKIK KÜLÖNBÖZNEK EGYMÁSTÓL" ); 
	fwprintf( fD, m_head );
	fwprintf( fD, m_description );

	str.Format( L"A sárga hátterû adat különbözik a csoport elsõ sorában lévõ megfelelõ adattól.\n\n" );
	fwprintf( fD, str );

	fwprintf( fD, m_columns );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::createHead( CString title )
{
	m_head.Format( L"\
<HEAD>\n\
<style>\n\
</style>\n\
</HEAD>\n\
<BODY>\n\
<center>%s</center><br><br>\n\n\
<pre>\n\
%-20s %s (%s)<br>\
%-20s %s<br><br>\
%s %d %s<br><br>\
",
title,\
L"Adatbázis:",\
theApp.m_databaseSpec,\
theApp.m_user_version,\
L"Lista készült:",\
theApp.getPresentDateTime(),\
L"Az azonosság megállapításához legalább ",\
_azonos,\
L"számú egyezés volt elõírva."\
);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameCouples::query( CString command )
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
BOOL CSameCouples::query1( CString command )
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
BOOL CSameCouples::query2( CString command )
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
BOOL CSameCouples::query3( CString command )
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
BOOL CSameCouples::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSameCouples:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_SAMECOUPLES ))
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnLineHusband()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINES1 );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnLineWife()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINES2 );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CSameCouples::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
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
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
