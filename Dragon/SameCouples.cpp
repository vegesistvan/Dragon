// CSameCouples.cpp : implementation file
//



#include "stdafx.h"
#include "Dragon.h"
#include "SameCouples.h"
#include "afxdialogex.h"
#include "checkParam0.h"
#include "GetLastFirst.h"
#include <algorithm>
#include <vector> 


// lekérdezett oszlopok a people táblából
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
bool sortByC(const CONTRACT &v1, const CONTRACT &v2) 
{
	return( v1.rowidBy < v2.rowidBy );
}
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
h_id     - a házasság azonosítója,\n\
házasság - aházasság dátuma\n\
gr       - group, az azonos nevû embercsoprton belül azonosnak éréklelt alcsoportok sorszáma.\n\n\
st       - status, az azonosítás eredménye: -1 azonos, azaz egyesített, majd törölt, 0: változatlanul hagyott, 1: ez az egyesített bejegyzés.\n\
line     - a GA html fájlban a sor száma, amelyben elõfordul.\n\
G        - generáció, az ember generációs jele a GA fájlban.\n\
S        - source, a név elõfordulásának hierarchia száma.\n\
U        - united, az ember u számú bejegyzésének összevonása.\n\
férj adatai:\n\
rowid    - a férj azonosítója\n\
férj     - a férj neve.\n\
születés - születési dátum.\n\
halál    - halálozás dátuma.\n\
apja     - a férj apjának neve\n\
anyja    - a férj anyjának neve.\n\n\
feleség hasoló adatai.\n\n\
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
A kettõs keresztnevekbõl csak az elsõ használjuk az összehasoonlításhoz.\r\n\
\r\n\
Ha a vizsgált adatok között ellentmondás van, akkor nyilvánvalóan nem azonos személy bejegyzéseirõl van szó.\r\n\
Ha nincs ellentmondás, akkor a megadott számú adat létezése és egyezése szükséges az azonosság megállapításához.\r\n\
\r\n\
Az adathibák nyilván egyesítési hibákhoz vezetnek, valójában különbözõ emberekhez tartozó bejegyzésket tévesen \
egyesítünk, vagy azonos emberek bejegyzéseinek összevonása elmarad. Ezért az egyesítés elõtt minden hibát \
ki kel javítani!.\r\n\
Erre szolgálnak az \"Ellenõrzése egyesítés elõtt\" funkciók, de az egyesítés során az azonosítás erdményét tartalmazó \
fájlok is felfedhetnek hibákat.\r\n\
\r\n\
";

//Elõírhatjuk, hogy az ellentmondásmentes adatok mellett hány adatpár létezését és egyenlõségét követeljük meg az \
//azonosság elfogadásához. Javasolt értéke: 0.\r\n\

	m_columns.Format( L"\n<b>\
%6s %12s \
%2s \
%2s %8s %1s %1s %2s \
%8s %-30s %-13s %-13s %-30s %-30s \
%2s %8s %1s %1s %2s %8s %-30s %-13s %-13s %-30s %-30s</b>\n", \
L"h_id", L"házasság",\
L"gr",\
L"st",L"line", L"G", L"S", L"U",\
L"rowid", L"férj", L"születés", L"halál", L"apja", L"anyja", \
L"st",L"line", L"G", L"S", L"U",\
L"rowid", L"feleség", L"születés", L"halál", L"apja", L"anyja" \
);

	m_colors.Add( L"bisque" );
	m_colors.Add( L"aquamarine" );
	m_colors.Add( L"yellow" );
	m_colors.Add( L"deepskyblue" );
	m_colors.Add( L"greenyellow" );
	m_colors.Add( L"thistle" ); 
	m_colors.Add( L"hotpink" );
	m_colors.Add( L"lightyellow" );
	m_colors.Add( L"aqua" );
	m_colors.Add( L"moccasin" );

//Ha csak egy házaspárt akatsz vizsgálni, add meg a nevüket

//	_husband	= L"Ajkay István";
//	_wife		= L"N N";
	m_contract	= false;
	m_contract	= true;			// végrejatsa-e az összevonásokat	
	m_loop		= 3;
	m_azonos	= 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameCouples::~CSameCouples()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_COMBO, ComboCtrl);
	DDX_Text(pDX, IDC_EDIT, m_info);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSameCouples, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSameCouples::OnBnClickedOk)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameCouples::OnInitDialog()
{
	CDialogEx::OnInitDialog();

/*
	for( UINT i = 0; i < 10; ++i )
	{
		str.Format( L"%d", i );
		ComboCtrl.AddString( str );
	}
	ComboCtrl.SetCurSel( 0 );

	UpdateData( TOSCREEN );
*/
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnBnClickedOk()
{
//	_azonos	= ComboCtrl.GetCurSel();
	int loop = 1;
	
	
	wndP.Create( NULL, L"Azonos nevû házaspárok..." );
	wndP.GoModal();
	
	while( true )
	{
		theApp.setStartTime();
		openUnited();
		openDifferent();

		m_deleted = 0;
		core( loop );

		if( m_deleted )
			str.Format( L"%d ember összevonva.", m_deleted );
		else
			str = L"Nincs összevonható ember!";

		fwprintf( fU, L"\n\n%s\n", str );
		fwprintf( fD, L"\n\n%s\n", str );
		fwprintf( fU, L"</pre>" );
		fwprintf( fD, L"</pre>" );
		fwprintf( fU, L"Eltelt idõ: %s<br><br>", theApp.get_time_elapsed() );
		fwprintf( fD, L"Eltelt idõ: %s<br><br>", theApp.get_time_elapsed() );

		fclose( fU );
		fclose( fD );

			if( m_deleted )
		{
			theApp.m_user_version = theApp.m_user_version*10+1;
			m_command.Format( L"PRAGMA user_version='%d'", theApp.m_user_version );
			theApp.execute( m_command );

			theApp.showHtmlFile( unitedSpec );
			theApp.showHtmlFile( differentSpec );
		}
		else
		{
			theApp.showHtmlFile( differentSpec );
			break;
		}
		if( loop == m_loop ) break;
		++loop;
	}
	wndP.DestroyWindow();		

#ifndef _DEBUG
	wndP.SetText( L"Az adatbázis tömörítése..." );
#endif
	theApp.execute( L"VACUUM");

	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::core( int loop )
{
	CString first_name1;
	CString	lastName;
	CString firstName;
	COUPLES vcouples;
	CString spouse;
	CString mother_id;
	CString father_id;

	int		z;

	str.Format( L"Azonos nevû emberek házasságainak vizsgálata és összevonása - %d. iteráció", loop );
	wndP.m_strTitle = str;

#ifndef _DEBUG
	str.Format( L"Házasságok lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif


	vContract.clear();

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
		
		vcouples.generationS2.Trim();
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

//	getSameCouples( loop );
	wndP.SetPos(0);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CSameCouples::getSameCouples( int loop )
//{

	CString spouse1;
	CString spouse2;
	CString name;
	CString namePrev(L"");
	bool	first = true;
	
	
	wndP.SetRange( 0, vCouples.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	vSame.clear();
	
	for( ic = 0; ic < vCouples.size(); ++ic )
	{
		spouse1 =  vCouples.at(ic).spouse1;
		spouse2 =  vCouples.at(ic).spouse2;

		if( !_husband.IsEmpty()  && _husband != spouse1 ) goto cont2;
		if( !_wife.IsEmpty() && _wife != spouse2 ) goto cont2;
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

cont2:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if( vSame.size() )
	{
		processSame();
		vSame.clear();
	}

	theApp.execute( L"BEGIN" );
	contractFull( loop );
	clearMarriages( loop );
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
	CString rowid1;
	CString rowid2;
	
	resetRef();		// ez kell, hogy a korábbi ref-et törölje!!
	for( UINT i1 = 0; i1 < vSame.size(); ++i1 )
	{
		rowid1 = vSame.at(i1).rowidS1;
		
		if( db ) 
		{
			resetRef();
			++group;
		}
		db = 0;
		if( vSame.at(i1).group == 0 )		// az i1. házaspárt még nem rendelték hozzá egyik csoporthoz sem
		{
			for( UINT i2 = 0; i2 < vSame.size(); ++i2 )
			{
				rowid2 = vSame.at(i2).rowidS1;
				if( i1 != i2 && vSame.at(i2).group == 0 )	// természetesen csak különbözõ házaspárokat vizsgál, amelyeket még nem redneltek hozzá egyik csooprthoz sem
				{
					if( identical( i1, i2 ) )
					{
						setRef( i1 );
						setRef( i2 );

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
void CSameCouples::resetRef()
{

	r.birthS1.Empty();
	r.birthS2.Empty();
	r.cnt = 0;
	r.contracted = 0;
	r.dateM.Empty();
	r.deathS1.Empty();
	r.deathS2.Empty();
	r.fatherS1.Empty();
	r.fatherS2.Empty();
	r.father_idS1.Empty();
	r.father_idS2.Empty();
	r.generationS1.Empty();
	r.generationS2.Empty();
	r.group = 0;
	r.lineS1.Empty();
	r.lineS2.Empty();
	r.motherS1.Empty();
	r.motherS2.Empty();
	r.mother_idS1.Empty();
	r.mother_idS2.Empty();
	r.rowidM.Empty();
	r.rowidS1.Empty();
	r.rowidS2.Empty();
	r.sex_idS1.Empty();
	r.sex_idS2.Empty();
	r.spouse1.Empty();
	r.spouse2.Empty();
	r.status1 = 0;
	r.status2 = 0;
	r.unitedS1.Empty();
	r.unitedS2.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::setRef( int i )
{
	COUPLES a = vSame.at(i );

	r.rowidS1 = a.rowidS1;
	r.rowidS2 = a.rowidS2;

	if( r.birthS1.IsEmpty() && !a.birthS1.IsEmpty() )				r.birthS1 = a.birthS1;
	if( r.deathS1.IsEmpty() && !a.deathS1.IsEmpty() )				r.deathS1 = a.deathS1;
	if( r.fatherS1.IsEmpty() && !a.fatherS1.IsEmpty() )				r.fatherS1 = a.fatherS1;
	if( r.father_idS1.IsEmpty() && !a.father_idS1.IsEmpty() )				r.father_idS1 = a.father_idS1;
	if( r.motherS1.IsEmpty() && !a.motherS1.IsEmpty() )				r.motherS1 = a.motherS1;
	if( r.mother_idS1.IsEmpty() && !a.mother_idS1.IsEmpty() )		r.mother_idS1 = a.mother_idS1;

	if( r.birthS2.IsEmpty() && !a.birthS2.IsEmpty() )				r.birthS2 = a.birthS2;
	if( r.deathS2.IsEmpty() && !a.deathS2.IsEmpty() )				r.deathS2 = a.deathS2;
	if( r.fatherS2.IsEmpty() && !a.fatherS2.IsEmpty() )				
		r.fatherS2 = a.fatherS2;
	if( r.motherS2.IsEmpty() && !a.motherS2.IsEmpty() )				r.motherS2 = a.motherS2;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az i1 és i2 indexû házasspárok azonosságát állapíthja meg.
// return true: azonosak (nincs ellentmondás az adatai között!!!) 
// return false: különbözõek
bool CSameCouples::identical( UINT i1, UINT i2 )
{
	COUPLES a = vSame.at(i1);
	COUPLES b = vSame.at(i2);

	if( !a.generationS1.IsEmpty() && !b.generationS1.IsEmpty() ) return false;

	int	good = 0;
	int g;



	if( ( g = same( r.birthS1, a.birthS1, b.birthS1 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.deathS1, a.deathS1, b.deathS1 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.fatherS1, a.fatherS1, b.fatherS1 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.motherS1, a.motherS1, b.motherS1 ) ) == -1 ) return false;
	if( g == 1 ) ++good;


	if( ( g = same( r.birthS2, a.birthS2, b.birthS2 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.deathS2, a.deathS2, b.deathS2 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.fatherS2, a.fatherS2, b.fatherS2 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.motherS2, a.motherS2, b.motherS2 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( m_azonos <= good ) return true;				// legalább _azonos egyezés ellentmondás nélkül 
	return false;
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
	CONTRACT vcontract;

	if( vSame.at(0).spouse1 == L"Barkóczy László"  && vSame.at(0).spouse2 == L"Krucsay Klára" )
		z = 1;

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
				break;
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

					if( rowid != rowidBy )  // akkor fordulhat elõ, ha egy korábban házasságban rowid-t már felülírták rowidBy-al
					{ 
						vcontract.rowid		= rowid;
						vcontract.rowidBy	= rowidBy;
						vcontract.sex_id	= sex_id;
						vContract.push_back( vcontract );
					}
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
				break;
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

					if( rowid != rowidBy )
					{
						vcontract.rowid		= rowid;
						vcontract.rowidBy	= rowidBy;
						vcontract.sex_id	= sex_id;
						vContract.push_back( vcontract );
					}
					++vSame.at(0).contracted;
					++m_contracted;
				}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::contractFull( int loop )
{
	if( !m_contract ) return;

	CString rowid;
	CString rowidBy;
	CString sex_id;

#ifndef _DEBUG
	str.Format( L"Törlendõ bejegyzések vizsgálata..." );
	wndP.SetText( str );
#endif
	wndP.SetRange( 0, vContract.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	std::sort( vContract.begin(), vContract.end(), sortByC );
	for( UINT i = 0; i < vContract.size(); ++i )
	{
		rowid	= vContract.at(i).rowid;
		rowidBy = vContract.at(i).rowidBy;
		for( UINT j = 0; j < vContract.size(); ++j )
		{
			if( vContract.at(j).rowidBy == rowid ) 
				vContract.at(j).rowidBy = rowidBy;
			else if( rowid  < vContract.at(j).rowidBy ) break;
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}


#ifndef _DEBUG
	str.Format( L"Törlés...", loop );
	wndP.SetText( str );
#endif
	wndP.SetRange( 0, vContract.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	for( UINT i = 0; i < vContract.size(); ++i )
	{
		rowid	= vContract.at(i).rowid;
		rowidBy = vContract.at(i).rowidBy;
		sex_id	= vContract.at(i).sex_id;

		m_command.Format( L"DELETE FROM people WHERE rowid ='%s'", rowid );
		if( !theApp.execute( m_command ) ) return;

		if( sex_id == L"1" )
			m_command.Format( L"UPDATE people SET father_id = '%s' WHERE father_id='%s'", rowidBy, rowid );
		else
			m_command.Format( L"UPDATE people SET mother_id = '%s' WHERE mother_id='%s'", rowidBy, rowid );
		if( !theApp.execute( m_command ) ) return;

		if( sex_id == L"1" )
			m_command.Format( L"UPDATE marriages SET spouse1_id='%s' WHERE spouse1_id ='%s'", rowidBy, rowid );
		else
			m_command.Format( L"UPDATE marriages SET spouse2_id='%s' WHERE spouse2_id ='%s'", rowidBy, rowid );
		if( !theApp.execute( m_command ) ) return;

		++m_deleted;			// az összes összevonást számolja
		
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::clearMarriages( int loop )
{

#ifndef _DEBUG
	str.Format( L"Házasságok ellenõrzése és törlése..." );
	wndP.SetText( str );
#endif

	m_command = L"SELECT rowid, spouse1_id, spouse2_id FROM marriages ORDER BY spouse1_id, spouse2_id";
	if( !query( m_command ) ) return;

	CString rowid11;
	CString rowid21;
	CString rowid12;
	CString rowid22;

	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		rowid11 = m_recordset->GetFieldString( 1 );
		rowid21 = m_recordset->GetFieldString( 2 );
		m_recordset->MoveNext();
		rowid12 = m_recordset->GetFieldString( 1 );
		rowid22 = m_recordset->GetFieldString( 2 );
		if( rowid11 == rowid12 && rowid21 == rowid22 )
		{
			m_command.Format( L"DELETE FROM marriages WHERE rowid = '%s'", m_recordset->GetFieldString( 0 ) );
			if( !theApp.execute( m_command ) ) return;
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.SetPos(0 );
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
	bool first;
	for( UINT j = 0; j <= m_numOfGroups; ++j )
	{
		first = true;
		for( UINT i = 0; i < vSame.size();++i )
		{
			if( vSame.at(i).group == j )
			{
				if( first )
				{
					r = vSame.at(j);
					first = false;
				}
				printYellow( i );
			}
		}
	}
	fwprintf( fD, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kiírja az i2 rekordot, az i1 rekordhoz képesti eltéréseket sárga háttárrel
void CSameCouples::printYellow( UINT i )
{
	COUPLES b = vSame.at( i );

	str.Format( L"%6s %12s %2d %2d %8s ", b.rowidM, b.dateM, b.group, b.status1, b.lineS1 );
	fwprintf( fD, L"%s", str );

	if( !r.generationS1.IsEmpty() && !b.generationS1.IsEmpty() && r.generationS1 != b.generationS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%1s</span> ", b.generationS1 );
	}
	else
		fwprintf( fD, L"%1s ", b.generationS1 );

	str.Format( L"%1s %2s %8s %-30s ", b.sourceS1, b.unitedS1, b.rowidS1, b.spouse1 );
	fwprintf( fD, L"%s", str );

// birth
	if( !r.birthS1.IsEmpty() && !b.birthS1.IsEmpty() && r.birthS1 != b.birthS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", b.birthS1 );
	}
	else
		fwprintf( fD, L"%-13s ", b.birthS1 );// deathDate1


// death
	if( !r.deathS1.IsEmpty() && !b.deathS1.IsEmpty() && r.deathS1 != b.deathS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", b.deathS1 );
	}
	else
		fwprintf( fD, L"%-13s ", b.deathS1 );

// father1
	if( !r.fatherS1.IsEmpty() && !b.fatherS1.IsEmpty() && r.fatherS1 != b.fatherS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", b.fatherS1 );
	}
	else
		fwprintf( fD, L"%-30s ", b.fatherS1 );

// mother1
	if( !r.motherS1.IsEmpty() && !b.motherS1.IsEmpty() && r.motherS1 != b.motherS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", b.motherS1 );
	}
	else
		fwprintf( fD, L"%-30s ", b.motherS1 );

	str.Format( L"%2d %8s ", b.status2, b.lineS2 );
	fwprintf( fD, L"%s", str );

// generation
	if( !r.generationS2.IsEmpty() && !b.generationS2.IsEmpty() && r.generationS2 != b.generationS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%1s</span> ", b.generationS2 );
	}
	else
		fwprintf( fD, L"%-1s ", b.generationS2 );

	str.Format( L"%1s %2s %8s %-30s ", b.sourceS2, b.unitedS2, b.rowidS2, b.spouse2 );
	fwprintf( fD, L"%s", str );

// birthS2
	if( !r.birthS2.IsEmpty() && !b.birthS2.IsEmpty() && r.birthS2 != b.birthS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", b.birthS2 );
	}
	else
		fwprintf( fD, L"%-13s ", b.birthS2 );


// deathDate2
	if( !r.deathS2.IsEmpty() && !b.deathS2.IsEmpty() && r.deathS2 != b.deathS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", b.deathS2 );
	}
	else
		fwprintf( fD, L"%-13s ", b.deathS2 );

// father2
	if( !r.fatherS2.IsEmpty() && !b.fatherS2.IsEmpty() && r.fatherS2 != b.fatherS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", b.fatherS2 );
	}
	else
		fwprintf( fD, L"%-30s ", b.fatherS2 );

// mother2
	if( !r.motherS2.IsEmpty() && !b.motherS2.IsEmpty() && r.motherS2 != b.motherS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", b.motherS2 );
	}
	else
		fwprintf( fD, L"%-30s ", b.motherS2 );

	fwprintf( fD, L"\n");

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::listUnited()
{
	int z;
	int index;
	CString color;
	COUPLES a;
	std::sort( vSame.begin(), vSame.end(), sortByGroupStatus );

	for( UINT i = 0; i < vSame.size();++i )
	{
		a = vSame.at(i);
//férj
		str.Format( L"\
%6s %12s %2d \
%2d %8s %1s %1s %2s \
%8s %-30s %-13s %-13s %-30s %-30s",\
a.rowidM, a.dateM, a.group, \
a.status1, a.lineS1, a.generationS1, a.sourceS1, a.unitedS1,\
a.rowidS1, a.spouse1, a.birthS1, a.deathS1, a.fatherS1, a.motherS1 );
		if( a.status1 != 0 && a.status1 != -1 )
		{
			index = a.group;
			if( index > 5 ) index = 0;
			if( index == 2 )
				z = 1;
			color = m_colors.GetAt( index );
			fwprintf( fU, L"<span style=\"background:%s\">%s</span> ", color, str );
		}
		else if( a.status1 == -1 )
			fwprintf( fU, L"<span style=\"background:LightGray\">%s</span> ", str );
		else
			fwprintf( fU, L"%s ", str );
//feleség
		str.Format( L"\
%2d %8s %1s %1s %2s \
%8s %-30s %-13s %-13s %-30s %-30s",\
a.status2, a.lineS2, a.generationS2, a.sourceS2, a.unitedS2, \
a.rowidS2, a.spouse2, a.birthS2, a.deathS2, a.fatherS2, a.motherS2 );
		if( a.status2 != 0 && a.status2 != -1 )
		{
			index = a.group;
			if( index > 5 ) index = 0;
			color = m_colors.GetAt( index );
			fwprintf( fU, L"<span style=\"background:%s\">%s</span>", color, str );
		}
		else if( a.status2 == -1 )
			fwprintf( fU, L"<span style=\"background:LightGray\">%s</span>", str );
		else
			fwprintf( fU, L"%s ", str );

		fwprintf( fU, L"\n");
	}
	fwprintf( fU, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::openUnited()
{
	CString fileName;
	fileName.Format( L"couplesUnited_%d", theApp.m_user_version );
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
	CString fileName;
	fileName.Format( L"couplesDifferent_%d", theApp.m_user_version );
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
%-21s %s (%d)<br>\
%-21s %s<br><br>\
%-21s %d<br><br>\
",
title,\
L"Adatbázis:",\
theApp.m_databaseSpec,\
theApp.m_user_version,\
L"Lista készült:",\
theApp.getPresentDateTime(),\
L"Egyezések min. száma:",\
m_azonos\
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
