// SamePeople.cpp : implementation file
//
// Nagy kihívás, hogy az azonos nevû emeberek bejegyzései között megtaláljuk az azonos emberhez tartozókat.
// Elõfordulhat, hogy két ember külön-külön megfelel egy harmadikkal való azonosításnak, de egymással már konfliktusban vannak.
// Ezt valahogy meg kellene oldani.

#include "stdafx.h"
#include "Dragon.h"
#include "SamePeople.h"
#include "afxdialogex.h"
#include <algorithm>
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByCnt(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.cnt < v2.cnt );
}
bool sortByGroup(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.group < v2.group );
}
bool sortBySource(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.source < v2.source );
}
bool sortByMother(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.mother < v2.mother );
}
bool sortBy(const CONTRACT &v1, const CONTRACT &v2) 
{
	return( v1.rowidBy < v2.rowidBy );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortByGroupStatus(const SAMENAMES &v1, const SAMENAMES &v2) 
{ 
	if( v1.group > v2.group )
		return false;
	if( v1.group == v2.group )
	{
		if( v1.status > v2.status )
			return true;
		else
			return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSamePeople, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSamePeople::CSamePeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSamePeople::IDD, pParent)
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

	m_columns.Format( L"\
%2s %2s %2s %2s \
%9s %2s %1s %1s \
%s %s %s %s",\
L"gr",L"mt",L"gp",L"st",\
L"line", L"u", L"G", L"S",\
L"   rowid-név----------------------------- születés---- halál---- ",\
L"   rowid-apa----------------------------- születés---- halál---- ",\
L"   rowid-anya---------------------------- születés---- halál---- ",\
L"   rowid-házastársak---------------"\
);


	m_description = L"\
Az oszlopok jelentése:\n\n\
gr       - group, az azonos nevû embercsoprton belül azonosnak éréklelt alcsoportok sorszáma.\n\
mt		 - match, az azonosnak talált adatpárok száma\n\
gp       - groupP, korábban a gp csoporthoz tartozott, de elvette, mert több azonosság van ezzel a bejegyzéssel\n\
st       - status, az azonosítás eredménye: -1 azonos, azaz egyesített, majd törölt, 0: változatlanul hagyott, 1: ez az egyesített bejegyzés.\n\
line     - a bejegyzés sorszáma a GA html fájlban.\n\
u        - united, az ember u számú bejegyzés összevonása.\n\
G        - generáció, az ember generációs jele a GA fájlban.\n\
S        - az enber elõfordulása ( 1-2-3-4 )\n\
rowid    - a bejegyzés azonosítója\n\
név      - az ember neve\n\
születés - születési dátum\n\
halál    - halálozás dátuma\n\
apa      - neve, majd adatai\n\
anya     - neve, majd adatai\n\
házastársak\n\n\
";

m_info = L"\
Az alábbi adatokat vizsgáljuk az azonos nevû emberek azonosságának eldöntéséhez:\r\n\
\r\n\
1. születési dátuma\r\n\
2. halálozási dátuma\r\n\
3. apa neve\r\n\
4. apa születési dátuma\r\n\
5. apa halálozási dátuma\r\n\
6. anya neve\r\n\
7. anya születési dátuma\r\n\
8. anya halálozási dátuma\r\n\
9. házastárs neve\r\n\
\r\n\
A kettõs keresztnevekbõl csak az elsõ használjuk az összehasonlításhoz.\r\n\
\r\n\
Ha a vizsgált adatok között ellentmondás van, akkor nyilvánvalóan nem azonos személy bejegyzéseirõl van szó.\r\n\
Ha nincs ellentmondás, akkor a megadott számú adat létezése és egyezése szükséges az azonosság megállapításához.\r\n\
Leszármazottakat akkor sem egyesítünk, ha mindenben megfelelel az összevonási kritériumoknak, mert leszármazottként \n\
csak egyszer szerepelhet egy ember.\r\n\
Az adathibák nyilván egyesítési hibákhoz vezetnek, valójában különbözõ emberekhez tartozó bejegyzésket tévesen \
egyesítünk, vagy azonos emberek bejegyzéseinek összevonása elmarad. Ezért az egyesítés elõtt minden hibát \
ki kel javítani!.\r\n\
Erre szolgálnak az \"Ellenõrzése egyesítés elõtt\" funkciók, de az egyesítés során az azonosítás erdményét tartalmazó \
fájlok is felfedhetnek hibákat.\r\n\
\r\n\
";
//Elõírhatjuk, hogy az ellentmondásmentes adatok mellett hány adatpár létezését és egyenlõségét követeljük meg az \
//azonosság elfogadásához. Ajánlott értéke: 1.\



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

//	m_name = L"Abaffy Gáspár";   // ha csak egy embert akarunk vizsgálni, itt megadhatjuk a nevét
	m_contract	= false;
	m_contract	= true;			// végrejatsa-e az összevonásokat	
	m_loop		= 3;
	m_azonos	= 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSamePeople::~CSamePeople()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_info);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSamePeople, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSamePeople::OnBnClickedOk)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSamePeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();

/*
	for( UINT i = 0; i < 10; ++i )
	{
		str.Format( L"%d", i );
		ComboCtrl.AddString( str );
	}
	ComboCtrl.SetCurSel( 1 );

	UpdateData( TOSCREEN );
*/
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::OnBnClickedOk()
{
//	_azonos = ComboCtrl.GetCurSel();

	int loop = 1;

	str.Format( L"Azonos nevû emberek beolvasása..." );
	wndP.Create( NULL, str );
	wndP.GoModal();

	while( true )
	{
		theApp.setStartTime();
		openDifferent( loop );
		openUnited( loop );
		m_deleted = 0;
	
		core( loop );
	
		if( m_deleted )
		{
			theApp.m_user_version = theApp.m_user_version * 10 + 2;
			m_command.Format( L"PRAGMA user_version='%d'", theApp.m_user_version );
			theApp.execute( m_command );

			str.Format( L"\n\n%d bejegyzés került összevonásra.\n\n", m_deleted );
			fwprintf( fU, str );
	
			fwprintf( fU, L"Eltelt idõ: %s<br><br>", theApp.get_time_elapsed() );
			fclose( fU );
			theApp.showHtmlFile( unitedSpec );

			fwprintf( fD, L"Eltelt idõ: %s<br><br>", theApp.get_time_elapsed() );
			fclose( fD );
			theApp.showHtmlFile( differentSpec );
		}
		else
		{
			fclose( fU );
			
			str = L"Nincs összevonható bejegyzés!";
			fwprintf( fD, L"%s\n\n", str );
			fwprintf( fD, L"Eltelt idõ: %s<br><br>", theApp.get_time_elapsed() );
			fclose( fD );
			theApp.showHtmlFile( differentSpec );
			break;
		}
		if( loop == m_loop ) break;
		++loop;
	}
	wndP.DestroyWindow();
	#ifndef _DEBUG
	str.Format( L"Adatbázis tömörítése..." );
	wndP.SetText( str );
#endif
	theApp.execute( L"VACUUM");
	AfxMessageBox( L"Nincs több összevonható bejegyzés!" );
	CDialogEx::OnOK();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::core( int loop )
{
	CString namePrev;
	CString name;
	CString lastName;
	CString firstName;
	bool	first = true;;
	int		pos;

	str.Format( L"Azonos nevû emberek bejegyzéseinek vizsgálata és összevonása - %d. iteráció", loop );
	wndP.m_strTitle = str;


#ifndef _DEBUG
	wndP.SetText( L"Azonos nevû emberek beolvasása..." );
#endif

	m_command.Format( L"SELECT %s FROM people ORDER BY last_name, first_name, source", p_fields );
	if( !query( m_command ) ) return;

	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );


	vPeople.clear();
	vContract.clear();
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		lastName  = m_recordset->GetFieldString( P_LAST_NAME );
		if( lastName.IsEmpty() ) goto cont;

		firstName = m_recordset->GetFieldString( P_FIRST_NAME );

		name.Format( L"%s %s", lastName, sepFirstName( firstName ) );
		if( name.TrimRight().IsEmpty() ) goto cont;
		if( !m_name.IsEmpty() && name != m_name ) goto cont;

		
		if( name == namePrev )
		{
			if( first )
			{
				putPeople( name, i-1 );
				first = false;
			}
			putPeople( name, i );
		}
		else if( vPeople.size() )
		{
			processPeople();
			vPeople.clear();
			first = true;
		}
		namePrev = name;
cont:	m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if( vPeople.size() )
		processPeople();

	theApp.execute( L"BEGIN" );
	contractFull( loop );
	clearMarriages( loop );
	theApp.execute( L"COMMIT" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::putPeople( CString name, UINT i )
{
	m_recordset->MoveTo( i );

	SAMENAMES vpeople;
	CString rowid = m_recordset->GetFieldString( P_ROWID );
	CString spouses;
	CString fullname;
	CString spouse_id;


	int z;
	if( rowid == L"136437" )
		z = 1;

	vpeople.name		= name;
	vpeople.group		= 0;
	vpeople.group2		= 0;
	vpeople.status		= 0;
	vpeople.match		= 0;
	vpeople.rowid		= rowid;
	vpeople.line		= m_recordset->GetFieldString( P_LINE );
	vpeople.generation	= m_recordset->GetFieldString( P_GENERATION );
	vpeople.generation.Trim();
	vpeople.source		= m_recordset->GetFieldString( P_SOURCE );
	vpeople.united		= m_recordset->GetFieldString( P_UNITED );
	vpeople.sex_id		= m_recordset->GetFieldString( P_SEX_ID );
	vpeople.birth		= m_recordset->GetFieldString( P_BIRTH_DATE );
	vpeople.death		= m_recordset->GetFieldString( P_DEATH_DATE );

	if( rowid == L"31140" )
		z = 1;
	CString father_id = m_recordset->GetFieldString( P_FATHER_ID );
	m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid = '%s'", father_id );
	if( !query1( m_command ) ) return;

	name.Empty();
	if( m_recordset1->RecordsCount() )
		name.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), sepFirstName( m_recordset1->GetFieldString( 1 ) ) );
	vpeople.father	= name.TrimRight();

	if( rowid == L"31140" &&  father_id == L"31138" )
		z = 1;


	if( father_id == L"0" ) father_id.Empty();
	vpeople.rowidF	= father_id;
	vpeople.birthF	= m_recordset1->GetFieldString( 2 );
	vpeople.deathF	= m_recordset1->GetFieldString( 3 );

	CString mother_id = m_recordset->GetFieldString( P_MOTHER_ID );
	m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid = '%s'", mother_id );
	if( !query1( m_command ) ) return;

	name.Empty();
	if( m_recordset1->RecordsCount() )
		name.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), sepFirstName( m_recordset1->GetFieldString( 1 ) ) );
	vpeople.mother	= name.TrimRight();
	if( mother_id == L"0" ) mother_id.Empty();
	vpeople.rowidM	= mother_id;
	vpeople.birthM	= m_recordset1->GetFieldString( 2 );
	vpeople.deathM	= m_recordset1->GetFieldString( 3 );

	if( vpeople.sex_id == L"1" )
		m_command.Format( L"SELECT spouse2_id FROM marriages WHERE spouse1_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT spouse1_id FROM marriages WHERE spouse2_id = '%s'", rowid );
	if( !query1( m_command ) ) return;

	

	for( UINT i = 0; i < m_recordset1->RecordsCount(); ++i )
	{
		spouse_id = m_recordset1->GetFieldString( 0 );
		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid = '%s'", spouse_id );
		if( !query2( m_command ) ) return;
			fullname.Format( L"%s %s", m_recordset2->GetFieldString(0), sepFirstName( m_recordset2->GetFieldString(1) ) );
		spouses += fullname.Trim();
		spouses += L",";
		m_recordset1->MoveNext();
	}
	if( m_recordset1->RecordsCount() )
		spouses = spouses.Left( spouses.GetLength() - 1); 
	vpeople.spouses = spouses;
	if( spouse_id == L"0" ) spouse_id.Empty();
	vpeople.rowidS	= spouse_id;
	vPeople.push_back( vpeople );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az alábbi algoritmusban az a probléma, hogy a legmagasabb prioritású bejegyzéshez (LPB) passzoló bejegyzések (BP)
// egymással ellentmondásban lehetnek!
// Pl: LPB: birth, BP1= birth death1, BP2:birth death2
// A kijelölt bejegyzések közötti ellentmondásokat ki kellen szûrni. De akkor melyik a jó bejegyzés?
void CSamePeople::processPeople()
{
	UINT group = 1;
	UINT db = 0;
	int z;
	CString gen1;
	CString gen2;
	int source1;
	int source2;
	CString line1;
	CString line2;
	CString spouse1;
	CString spouse2;

	if( vPeople.at(0).name == L"Battyhány Zsigmond" )
		z = 1;
	m_contracted = 0;
	resetRef();
	std::sort( vPeople.begin(), vPeople.end(), sortBySource );

/*
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		str.Format( L"%8s %s %2s %-30s %-30s", vPeople.at(i).rowid, vPeople.at(i).generation, vPeople.at(i).source, vPeople.at(i).name, vPeople.at(i).spouses );
		fwprintf( fU, L"%s\n", str );
	}
	fwprintf( fU, L"\n\n" );
*/
	for( UINT i1 = 0; i1 < vPeople.size(); ++i1 )
	{
		if( db )
		{
//			printRef( group ); 
			resetRef();  // ha új csoport kezdõdik, akkor üres rf-ek kellenek 
			++group;
		}
		db = 0;

		if( vPeople.at(i1).group == 0 )		// az i1. házaspárt még nem rendelték hozzá egyik csoporthoz sem
		{
			for( UINT i2 = 0; i2 < vPeople.size(); ++i2 )
			{
				if( i1 != i2 )
				{
					gen1 = vPeople.at( i1 ).generation;
					gen2 = vPeople.at( i2 ).generation;

					if( !gen1.IsEmpty() && !gen2.IsEmpty() ) continue;
					if( identical( i1, i2 ) )
					{  
						setRef( i1 );	// ha i1 és i2 azonos, akkor a ref-be másoljuk át a megadott adatokat
						setRef( i2 );

						gen1 = vPeople.at( i1 ).generation;
						gen2 = vPeople.at( i2 ).generation;

						line1 = vPeople.at(i1).line;
						line2 = vPeople.at(i2).line;
						
						source1 = _wtoi( vPeople.at(i1).source ); 
						source2 = _wtoi( vPeople.at(i2).source ); 

						if( (line1 == line2) && (source1 == source2) || gen1.IsEmpty() || gen2.IsEmpty() )
						{
							// Ha a bejegyzés már tartozott valakihez, de az egyezés most jobb, akkor elveszi tõle!!
							if( vPeople.at(i2).group && vPeople.at(i2).match < m_match )
								vPeople.at(i2).group2	= vPeople.at(i2).group;				// ezt jelzi a group2-ben
							vPeople.at( i1 ).group = group;
							vPeople.at( i2 ).group = group;
							vPeople.at( i1 ).status = 1;
							vPeople.at( i2 ).status = -1;
							vPeople.at( i2 ).match	= m_match;

							contract( i1, i2 );
							++db;
						}
					}
				}
			}
		}
	}
	listPeople();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az i1 és i2 indexû házasspárok azonosságát állapíthja meg.
// return true: azonosak (nincs ellentmondás az adatai között!!!) 
// return false: különbözõek
int CSamePeople::identical( UINT i1, UINT i2 )
{
	SAMENAMES	a = vPeople.at(i1);
	SAMENAMES	b = vPeople.at(i2);
	int			g;
	int z;


	if( a.rowid == L"261349" && b.rowid == L"26699" )
		z = 1;



	if( a.source == L"1" && b.source == L"1" )  return false;

	m_match = 0;
	if( ( g = same( r.birth, a.birth, b.birth ) ) == -1 ) return false;
	if( g == 1 ) ++m_match;

	if( ( g = same( r.death, a.death, b.death ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 

	if( ( g = same( r.father, a.father, b.father ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 

	if( ( g = same( r.mother, a.mother, b.mother ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 

	if( ( g = same( r.birthM, a.birthM, b.birthM ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 

	if( ( g = same( r.deathM, a.deathM, b.deathM ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 

	if( ( g = same( r.birthF, a.birthF, b.birthF ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 

	if( ( g = same( r.deathF, a.deathF, b.deathF ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 

/*
// a megadott házastársak azonos nevét nem lehet mindig kizáró oknak tekinteni, mert egy embernek lehetnek egyenkénti házasságai is
	if( ( g = sameSpouses( a.spouses, b.spouses ) ) == -1 )
	{
		if( !m_match )	return false;	// ha nincs más egyezés, és a házastárs is különbözik, akkor kizáró
										// de ha van más egyezés, akkor különbözõ nevû házastársakat is elfogad!!
	}
	else if( g == 1 ) ++m_match; 
*/

	if( ( g = sameSpouses( a.spouses, b.spouses ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 
	
	if( m_azonos <= m_match ) return true;			// legalább _azonos számú egyezés ellentmondás nélkül 
	return false;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ha referencia és spouse2 is meg van adva, akkor eueket hasonlítja össze
// Ha nincs, de spouse1 és spouse2 is adva van, akkor ezeket hasonlítja.
// spouse1 vagy spouse2 üres, akkor return 0;
//  1 : mindkettõ meg van adva és egyezik;
//  0 : csak az egyik vagy egy sincs van megadva, így nincs ellentmondás
//  -1 : mindkettõ meg van adva és nem egyezik
int CSamePeople::sameSpouses( CString spouse1, CString spouse2 )
{
	CStringArray a;
	CStringArray b;
	CStringArray x;
	int n1;
	int n2;
	int nx;
	CString sx;
	CString s1;
	CString s2;
	int z;

//	if( spouse1 == L"Krucsay Klára" )
//		z = 1;
//	if( spouse2 == L"Krucsay Klára" && spouse1 == L"Krucsay Klára, Dobay Kata" )
//		z = 1;

//	if( spouse1.IsEmpty() || spouse2.IsEmpty() ) return 0;

	n1 = wordList( &a, spouse1, ',', false );
	n2 = wordList( &b, spouse2, ',', false );
	nx = wordList( &x, r.spouses, ',', false );
	if( !spouse1.IsEmpty() && !spouse2.IsEmpty() )
	{
		for( int i = 0; i < n1; ++i )
		{
			s1 = a[i];
			if( !s1.IsEmpty() )
			{
				for( int j = 0; j < n2; ++j )
				{
					s2 = b[j];
					if( !s2.IsEmpty() )
					{
						if( s1 == s2 )
						{
							return 1;
						}
					}
				}
			}
		}
		return -1;
	}



	if( !r.spouses.IsEmpty() && !spouse2.IsEmpty() )
	{
		if( nx ==1 && n2 == 1 )
			return( same( r.spouses, spouse1, spouse2 ) );
	
		for( int i = 0; i < nx; ++i )
		{
			sx = x[i];
			for( int j = 0; j < n2; ++j )
			{
				s2 = b[j];
				if( sx == s2 )
				{
					if( nx < n2 ) r.spouses = spouse2;
					return 1;
				}
			}
		}

	}
	if( !spouse1.IsEmpty() && !spouse2.IsEmpty() )
	{
		if( n1 ==1 && n2 == 1 )
			return( same( r.spouses, spouse1, spouse2 ) );
		
		for( int i = 0; i < n1; ++i )
		{
			s1 = a[i];
			for( int j = 0; j < n2; ++j )
			{
				s2 = b[j];
				if( s1 == s2 ) 
				{
					if( n1 < n2 ) r.spouses = spouse2;
					else r.spouses = spouse1;
					return 1;
				}
			}
		}
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az iBy indexû emberrel helyettesíti az iDel indexû embert
void CSamePeople::contract( UINT iBy, UINT iDel )
{
	CString rowid	= vPeople.at( iDel ).rowid;
	CString rowidBy	= vPeople.at(iBy).rowid;
	CString sex_id	= vPeople.at(iDel).sex_id;
	int		source	= _wtoi( vPeople.at(iBy).source );
	CONTRACT vcontract;

	int z;
	if( rowid == L"25586" )
		z = 1;

	if( m_contract ) 
	{
		vcontract.rowid		= rowid;
		vcontract.rowidBy	= rowidBy;
		vcontract.sex_id	= sex_id;
		vContract.push_back( vcontract );
	}
	m_contracted = 1;		// csak a csoportra jelzi, hogy volt összevonás, az annak megfelleõ fájlba kell listázni
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::contractFull( int loop )
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

	std::sort( vContract.begin(), vContract.end(), sortBy );
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
void CSamePeople::clearMarriages( int loop )
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
void CSamePeople::listPeople()
{
	int index;
	int z;
	SAMENAMES x;
	CString ident;
	std::sort( vPeople.begin(), vPeople.end(), sortByGroupStatus );
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		x = vPeople.at(i);

		if( x.group2 == 0 )
			ident.Format( L"%2d %2d    %2d ", x.group, x.match, x.status );
		else
			ident.Format( L"%2d %2d %2d %2d ", x.group, x.match, x.group2, x.status );

		if( x.status == 1 )
			ident.Format( L"%2d       %2d ", x.group, x.status );

		if( !x.group )
			ident = L"            ";


		str.Format( L"\
%9s %2s %1s %1s \
%8s %-30s %12s %12s \
%8s %-30s %12s %12s \
%8s %-30s %12s %12s \
%8s %s",\
x.line, x.united, x.generation, x.source,\
x.rowid,  x.name,   x.birth,  x.death,\
x.rowidF, x.father, x.birthF, x.deathF,\
x.rowidM, x.mother, x.birthM, x.deathM,\
x.rowidS, x.spouses\
);
		ident += str;
		if( m_contracted )
		{
			if( x.status == 1 )
			{
				index = x.group;
				if( index > 5 ) index = 0;
				fwprintf( fU, L"<span style=\"background:%s\">%s</span>\n", m_colors.GetAt( index ), ident );
			}
			else if( x.status == -1 )
				fwprintf( fU, L"<span style=\"background:lightgray\">%s</span>\n", ident );
			else
				fwprintf( fU, L"%s\n", ident );
		}
		else
			fwprintf( fD, L"%s\n", ident );
	}

	if( m_contracted )
	{
		fwprintf( fU, L"\n" );
	}
	else
		fwprintf( fD, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::printRef( int group )
{
		str.Format( L"\
%2d %2s %9s %2s %1s %1s \
%-39s %12s %12s \
%8s %-30s %12s %12s \
%8s %-30s %12s %12s \
%8s %s",\
group, L" ", L" ", L" ", r.generation, L" ",
L" ", r.birth, r.death,
L" ", r.father, r.birthF, r.deathF,
L" ", r.mother, r.birthM, r.deathM,
L" ", r.spouses
);
	fwprintf( fU, L"<span style=\"background:DeepPink\">%s</span>\n", str );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::resetRef()
{
	r.group = 0;
	r.birth.Empty();
	r.death.Empty();
	r.father.Empty();
	r.mother.Empty();
	r.birthF.Empty();
	r.birthM.Empty();
	r.deathF.Empty();
	r.deathM.Empty();
	r.spouses.Empty();
	r.generation.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ha vPeople.at(i) azonos egy másik emberrel, akkor a kettõbõl összeszedjük a megadott értékeket
void CSamePeople::setRef( int i )
{
	SAMENAMES a;
	a = vPeople.at(i);

	if( r.birth.IsEmpty() && !a.birth.IsEmpty() )				
		r.birth = a.birth;
	if( r.birthF.IsEmpty() && !a.birthF.IsEmpty() )				
		r.birthF = a.birthF;
	if( r.birthM.IsEmpty() && !a.birthM.IsEmpty() )				
		r.birthM = a.birthM;
	if( r.death.IsEmpty() && !a.death.IsEmpty() )				
		r.death = a.death;
	if( r.deathF.IsEmpty() && !a.deathF.IsEmpty() )				
		r.deathF = a.deathF;
	if( r.deathM.IsEmpty() && !a.deathM.IsEmpty() )				
		r.deathM = a.deathM;
	if( r.father.IsEmpty() && !a.father.IsEmpty() )				
		r.father = a.father;
	if( r.mother.IsEmpty() && !a.mother.IsEmpty() )				
		r.mother = a.mother;
	if( r.spouses.IsEmpty() && !a.spouses.IsEmpty() )			
		r.spouses = a.spouses;

	if( !r.spouses.IsEmpty() && r.spouses.GetLength() < a.spouses.GetLength() ) 
		r.spouses = a.spouses;  // ha több házastárs van megadava



	if( r.generation.IsEmpty() && !a.generation.IsEmpty() )		
		r.generation = a.generation;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::openUnited( int loop )
{
	CString fileName;
	fileName.Format( L"peopleUnited_%d", theApp.m_user_version );
	unitedSpec = theApp.openHtmlFile( &fU, fileName, L"w+" );

	createHead( L"AZONOS NEVÛ EMBEREK, AKIK AZONOSAK IS, EZÉRT BEJEGYZÉSEIK ÖSSZEVONHATÓAK", loop  );
	fwprintf( fU, m_head );
	fwprintf( fU, m_description );

	str = L"A szürke hátterû bejegyzéseket egyesítettük a zöld hátterû bejegyzéssel.\n\
Ha egy azonos nevû csoportban több különbözõ egyesítés lehetséges, akkor azok a 'g'-group oszlopba található számmal vannak meglülönböztetve.\n\n";
	fwprintf( fU, str );

//	fwprintf( fU, m_columns );
	fwprintf( fU, L"<font color='red'>%s</font><br>", m_columns );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::openDifferent( int loop )
{
	CString fileName;
	fileName.Format( L"peopleDifferent_%d", theApp.m_user_version );
	differentSpec = theApp.openHtmlFile( &fD, fileName, L"w+" );

	createHead( L"AZONOS NEVÛ EMBEREK, AKIK KÜLÖNBÖZNEK EGYMÁSTÓ", loop  );
	fwprintf( fD, m_head );
	fwprintf( fD, m_description );
//	fwprintf( fD, m_columns );

	fwprintf( fD, L"<font color='red'>%s</font><br>", m_columns );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::createHead( CString title, int loop  )
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
%-21s %s<br>\
%-21s %d<br><br>\
%-21s %d<br><br>\
",
title,\
L"Adatbázis:",\
theApp.m_databaseSpec,\
theApp.m_user_version,\
L"Lista készült:",\
theApp.getPresentDateTime(),\
L"Interáció:",\
loop,\
L"Egyezések min. száma:",\
m_azonos\
);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSamePeople::query( CString command )
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
BOOL CSamePeople::query1( CString command )
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
BOOL CSamePeople::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

