// Contract.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Contract.h"
#include "utilities.h"
#include "ProgressWnd.h"
#include "ContractedPeople.h"
#include <algorithm>
// lekérdezett mezõk a people táblából
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
// CListCtrl táblázat oszlopai
enum
{
	S_CNT = 0,
	S_LOOP, 
	S_GROUP,
	S_MATCH,
	S_GROUP2,
	S_STATUS,
	S_RGBCOLOR,
	S_LINE,
	S_UNITED,
	S_GENERATION,
	S_SOURCE,
	S_NAME,
	S_ROWID,
	S_BIRTH,
	S_DEATH,
	S_FATHER,
	S_ROWIDF,
	S_BIRTHF,
	S_DEATHF,
	S_MOTHER,
	S_ROWIDM,
	S_BIRTHM,
	S_DEATHM,
	S_ROWIDS,
	S_SPOUSES,
	S_LINEF,
	COLUMNSCOUNT,
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySourceX(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.source < v2.source );
}
bool sortByX(const CONTRACT &v1, const CONTRACT &v2) 
{
	return( v1.rowidBy < v2.rowidBy );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortByGroupStatusX(const SAMENAMES &v1, const SAMENAMES &v2) 
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CContract, CWnd)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContract::CContract()
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



	m_description1 = L"\
Az oszlopok jelentése:\n\n\
gr       - group, az azonos nevû embercsoprton belül azonosnak éréklelt alcsoportok sorszáma.\n\
mt       - match, az azonosnak talált adatpárok száma\n\
gp       - groupP, korábban a gp csoporthoz tartozott, de elvette, mert több azonosság van ezzel a bejegyzéssel\n\
st       - status, az azonosítás eredménye: -1 azonos, azaz egyesített, majd törölt, 0: változatlanul hagyott, 1: ez az egyesített bejegyzés.\n\
";

	m_description2 = L"\
line     - a bejegyzés sorszáma a GA html fájlban.\n\
u        - united, az ember u számú bejegyzés összevonása.\n\
G        - generáció, az ember generációs jele a GA fájlban.\n\
S        - az enber elõfordulása a GA.html fájlban ( 1-2-3-4 )\n\
rowid    - a bejegyzés azonosítója\n\
név      - az ember neve\n\
születés - születési dátum\n\
halál    - halálozás dátuma\n\
apa      - neve, majd adatai\n\
anya     - neve, majd adatai\n\
házastársak\n\n\
";

	m_colors.Add( L"bisque" );
	m_colors.Add( L"aquamarine" );		//127,255,212
	m_colors.Add( L"yellow" );			//255,255,0
	m_colors.Add( L"deepskyblue" );		//0,191,255
	m_colors.Add( L"greenyellow" );		//173,255,47
	m_colors.Add( L"thistle" );			//216,191,216
	m_colors.Add( L"hotpink" );			//255,105,180
	m_colors.Add( L"lightyellow" );		//255,255,224
	m_colors.Add( L"aqua" );			//0,255,255
	m_colors.Add( L"lightGray" );		//211,211,211

	m_rgb[0] = RGB( 255, 255, 255 );
	m_rgb[1] = RGB( 127, 255, 212 );
	m_rgb[2] = RGB( 255, 255, 0 );
	m_rgb[3] = RGB( 0, 191, 255 );
	m_rgb[4] = RGB( 173, 255, 47 );
	m_rgb[5] = RGB( 216,191,216 );
	m_rgb[6] = RGB( 255, 105,180 );
	m_rgb[7] = RGB( 255, 255, 224 );
	m_rgb[8] = RGB( 0,255,255 );
	m_rgb[9] = RGB( 211,211,211 );

	sWHITE.Format( L"%u", RGB(255,255,255) );

	m_name = L"";				// ha csak egy embert akarunk vizsgálni, itt megadhatjuk a nevét
	m_azonos	= 1;			// az azonos adatpárok elõírt száma
	nItem		= 0;
	m_loopMax   = 4;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContract::~CContract()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContract, CWnd)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CContract::contractPeople()
{
	CString fileName;
	
	fileName = L"peopleUnited";
	m_fileSpecTextU = theApp.openTextFile( &textU, fileName, L"w+" );

	fileName = L"peopleDifferent";
	m_fileSpecTextD = theApp.openTextFile( &textD, fileName, L"w+" );

	CString drive;
	CString path;
	CString fname;
	CString ext;
	TCHAR* old;
	TCHAR* renamed;

	m_loop = 1;		// ciklus számláló

	wndP.Create( NULL, L"" );
	wndP.GoModal();
	
	tableLines.clear();
	while( m_loop <= m_loopMax )
	{
		str.Format( L"Azonos emberek bejegyzéseinek összevonása. (%d. ciklus)", m_loop );
		wndP.m_strTitle = str;

		// az aktuáli sadatbázis fájlok másolása *P.db fájlba
		splitFilespec( theApp.m_databaseSpec, &drive, &path,  &fname, &ext );
		str.Format( L"%s:%s\\%sP.%s", drive, path, fname, ext );
		CopyFile( theApp.m_databaseSpec, str, false );
		theApp.m_databaseSpec = str;
		str.Format( L"%s:%s\\%sP_blob.%s", drive, path, fname, ext );
		CopyFile( theApp.m_blobSpec, str, false );
		theApp.openDatabase();

		theApp.setStartTime();
		openDifferent();			// html fájl
		openUnited();				// html fájl

		vContract.clear();			// az összevont emberek 
	

		core();

		if( vContract.size() > 200 )
		{
#ifndef _DEBUG
			str.Format( L"(5/5) Adatbázis tömörítése" ); 
			wndP.SetText( str );
#endif
			theApp.execute( L"VACUUM");
		}

		if( !vContract.size() ) break;		// nincs összevont ember, vége a programnak
		fclose( fU );
		fclose( fD );

		theApp.setUserVersion( m_loop );
		++m_loop;
	}
	wndP.DestroyWindow();
	
	if( tableLines.size() )
	{
		m_command.Format( L"INSERT INTO contracted ( code1, code2, filespec) VALUES ( 1, 0, '%s')", m_fileSpecTextU );
		if( !theApp.execute( m_command ) ) return false;

		m_command.Format( L"INSERT INTO contracted ( code1, code2, filespec) VALUES ( 2, 0, '%s')", m_fileSpecTextD );
		if( !theApp.execute( m_command ) ) return false;
	}

	fclose( fU );
	fclose( fD );
	fclose( textU );
	fclose( textD );

	CContractedPeople dlg;
	dlg.m_filter = L"code1=1 AND code2=0";
	dlg.m_contracted = true;
	dlg.DoModal();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContract::core()
{
	CString namePrev;
	CString name;
	CString lastName;
	CString firstName;
	bool	first = true;;
	int		pos;

#ifndef _DEBUG

	str.Format( L"(1/5) Azonos nevû emberek bejegyzéseinek összegyûjtése..." );
	wndP.SetText( str );
#endif
			
	m_command.Format( L"SELECT %s FROM people ORDER BY last_name, first_name, source", p_fields );
	if( !query( m_command ) ) return;

	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	CString nameR;
	vPeople.clear();
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		lastName  = m_recordset->GetFieldString( P_LAST_NAME );
		if( lastName.IsEmpty() ) goto cont;

		firstName = m_recordset->GetFieldString( P_FIRST_NAME );
		name.Format( L"%s %s", lastName, sepFirstName( firstName ) );
		if( name.TrimRight().IsEmpty() ) goto cont;
			
		if( !m_name.IsEmpty() )
		{
			nameR = name.Left( m_name.GetLength() );	
			if( nameR != m_name ) goto cont;
		}

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
	contractFull();
	deleteMarriages();
	theApp.execute( L"COMMIT" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContract::putPeople( CString name, UINT i )
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

	CString father_id = m_recordset->GetFieldString( P_FATHER_ID );
	m_command.Format( L"SELECT last_name, first_name, birth_date, death_date, lineNumber FROM people WHERE rowid = '%s'", father_id );
	if( !query1( m_command ) ) return;

	name.Empty();
	if( m_recordset1->RecordsCount() )
		name.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), sepFirstName( m_recordset1->GetFieldString( 1 ) ) );
	vpeople.father	= name.TrimRight();

	if( father_id == L"0" ) father_id.Empty();
	vpeople.rowidF	= father_id;
	vpeople.birthF	= m_recordset1->GetFieldString( 2 );
	vpeople.deathF	= m_recordset1->GetFieldString( 3 );
	vpeople.lineF	= m_recordset1->GetFieldString( 4 );

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
void CContract::processPeople()
{
	UINT group = 1;
	UINT db = 0;
	int i;
	int z;
	int mi2;
	int g;
	CString gen1;
	CString gen2;
	int source1;
	int source2;
	CString line1;
	CString line2;
	CString spouse1;
	CString spouse2;
	CString rowid1;
	CString rowid2;

	if( vPeople.at(0).name == L"Lányi János" )
		z = 1;
	m_contracted = false;
	resetRef();
	std::sort( vPeople.begin(), vPeople.end(), sortBySourceX );

/*
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		str.Format( L"%2d %8s %s %2s %-30s %-30s", i, vPeople.at(i).rowid, vPeople.at(i).generation, vPeople.at(i).source, vPeople.at(i).name, vPeople.at(i).spouses );
		fwprintf( fU, L"%s\n", str );
	}
	fwprintf( fU, L"\n\n" );
*/
	for( UINT i1 = 0; i1 < vPeople.size(); ++i1 )
	{
		rowid1 = vPeople.at(i1).rowid;
		if( rowid1 == L"87409" )
			z = 1;
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
				rowid2 = vPeople.at(i2).rowid;
				if( i1 != i2 )
				{
					gen1 = vPeople.at( i1 ).generation;
					gen2 = vPeople.at( i2 ).generation;

					if( !gen1.IsEmpty() && !gen2.IsEmpty() ) continue;
 					if( identical( i1, i2 ) )
					{  
						if( group == 2 )
							z = 2;

						line1 = vPeople.at(i1).line;
						line2 = vPeople.at(i2).line;
						
						source1 = _wtoi( vPeople.at(i1).source ); 
						source2 = _wtoi( vPeople.at(i2).source ); 

						if( (line1 == line2) && (source1 == source2) || gen1.IsEmpty() || gen2.IsEmpty() )
						{

							
							// Ha a bejegyzés már tartozott valakihez, de az egyezés most jobb, akkor elveszi tõle!!

							mi2 = vPeople.at(i2).match;
							g = vPeople.at(i2).group;

							if( vPeople.at(i2).group )
							{
								if( vPeople.at(i2).match < m_match )
								{
									vPeople.at(i2).group2	= vPeople.at(i2).group;		// átveszi a group-ból
								}
								else
								{
									continue;
								}
							}
							vPeople.at( i1 ).group = group;
							vPeople.at( i2 ).group = group;
/*
// i1 marad, i2 törlve lesz
// itt vizsglódni kellene, hogy melyiket cseréljük le!! Ha nõt vonunk össze, akkor nem jó a 1 hierarchiát megtartani, mert 
// annak nincsenek gyerekei!!! Alapesetben H(i1) <= H(i2)
// Ha i1 nõ, akko meg kell nézni H(i1)-et és H(i2)-t és ha h(I1)==1 és H(i2) == 2 akkor i1 és i2 felcserélendõ, azaz H(i2)-t tarsuk meg!!!
							if( vPeople.at(i1).sex_id == WOMAN )
							{
								if( vPeople.at(i1).source == 1 && vPeople.at(i2).source == 2 )
								{
									i  = i1;
									i1 = i2;
									i2 = i;
								}
							}
*/
if( (i1 == 95921 && i2 == 141743 ) || (i1 == 141743 && i2 == 95921 ) )
   z = 1;

							vPeople.at( i1 ).status = 1;	// megtartani	
							vPeople.at( i2 ).status = -1;   // törölni
							vPeople.at( i2 ).match	= m_match;
							contract( i1, i2 );

							setRef( i1 );	// ha i1 és i2 azonos, akkor a ref-be másoljuk át a megadott adatokat
							setRef( i2 );


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
int CContract::identical( UINT i1, UINT i2 )
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
int CContract::sameSpouses( CString spouse1, CString spouse2 )
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
void CContract::contract( UINT iBy, UINT iDel )
{
	CString rowid	= vPeople.at( iDel ).rowid;
	CString rowidBy	= vPeople.at(iBy).rowid;
	CString sex_id	= vPeople.at(iDel).sex_id;
	int		source	= _wtoi( vPeople.at(iBy).source );
	CONTRACT vcontract;

// if( m_contract ) 
	{
		vcontract.rowid		= rowid;
		vcontract.rowidBy	= rowidBy;
		vcontract.sex_id	= sex_id;
		vcontract.source	= source;
		vContract.push_back( vcontract );
	}
	m_contracted = true;		// csak a csoportra jelzi, hogy volt összevonás, az annak megfelleõ fájlba kell listázni
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContract::contractFull()
{
	CString rowid;
	CString rowidBy;
	CString sex_id;
	int		source;


	int united;	
	int spouse;
	int spouseparent;
	int spousespouse;


#ifndef _DEBUG
	str.Format( L"(2/5) Törlendõ-megtartandó azonosítók korrigálása..." );
	wndP.SetText( str );
#endif
	wndP.SetRange( 0, vContract.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );


	// A törlendõ rowid-ik létezését vizsgálja a megtartandó rowid-k között-
	// Ha talál ilyet, akkor azt a megtartandó rowid-t lecseréli az õt helyettesító rowid-val.
	std::sort( vContract.begin(), vContract.end(), sortByX );
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
	str.Format( L"(3/5) Emberek fölösleges bejegyzéseinek törlése..." ); 
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
		source	= vContract.at(i).source;

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



		
// az egyesítések számát 1-el növeli
	m_command.Format( L"SELECT united, spouse, spouseparent, spousespouse FROM people WHERE rowid='%s'", rowidBy );
	if( !query1( m_command ) ) return;
	united			= _wtoi( m_recordset1->GetFieldString( 0 ) ) + 1;
	spouse			= _wtoi( m_recordset1->GetFieldString( 1 ) );
	spouseparent	= _wtoi( m_recordset1->GetFieldString( 2 ) );
	spousespouse	= _wtoi( m_recordset1->GetFieldString( 3 ) );

	

	switch( source )
	{
	case 2:
		++spouse;
		break;
	case 3:
		++spouseparent;
		break;
	case 4:
		++spouseparent;
		break;
	case 5:
		++spousespouse;
		break;
	}

	m_command.Format( L"UPDATE people SET united='%d', spouse='%d', spouseparent='%d', spousespouse='%d' WHERE rowid ='%s'",united, spouse, spouseparent, spousespouse, rowidBy );
	if( !theApp.execute( m_command ) ) return;

	
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az azonos (==rowid) házaspárokat tartalmazó házasságokat törli.
// Nem a nevek, hanem a rowid-k azonosságát vizsgálja!!
void CContract::deleteMarriages()
{

#ifndef _DEBUG
	str.Format( L"(4/5 Házasságok ellenõrzése és a fölöslegesek törlése..." );
	wndP.SetText( str );
#endif
	// A legkisebb szerepkódú házasságot tartja meg ( ORDER BY ... source )



	m_command = L"SELECT rowid, spouse1_id, spouse2_id FROM marriages ORDER BY spouse1_id, spouse2_id, source ";
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
void CContract::listPeople()
{

	int status;
	int	group;
	int colorIndex;
	int	rgbColor;


	int index;
	int z;
	SAMENAMES x;
	CString ident;
	std::sort( vPeople.begin(), vPeople.end(), sortByGroupStatusX );


// list into HTML file
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		x = vPeople.at(i);

		if( x.group2 == 0 )
			ident.Format( L"%2d %2d %2d    %2d ", m_loop, x.group, x.match, x.status );
		else
			ident.Format( L"%2d %2d %2d %2d %2d ", m_loop, x.group, x.match, x.group2, x.status );

		if( x.status == 1 )
			ident.Format( L"%2d %2d       %2d ", m_loop, x.group, x.status );

		if( !x.group )
			ident = L"               ";


		str.Format( L"\
%9s %2s %1s %1s \
%-30s %8s %12s %12s \
%-30s %8s %12s %12s \
%-30s %8s, %12s %12s \
%8s %s",\
x.line, x.united, x.generation, x.source,\
x.name,   x.rowid, x.birth,  x.death,\
x.father, x.rowidF, x.birthF, x.deathF,\
x.mother, x.rowidM, x.birthM, x.deathM,\
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
			fwprintf( fD, L"%s\n", str );
	}

	if( m_contracted )
	{
		fwprintf( fU, L"\n" );
		list();
	}
	else
		fwprintf( fD, L"\n" );



// list into text file
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		x = vPeople.at(i);
		
		switch( x.status )
		{
		case 0:
			colorIndex = 0;					// fehér
			break;
		case 1:
			colorIndex = x.group % 10;		// váltakozó színes
			break;
		case -1:
			colorIndex = 9;					// szürke
			break;
		}

		rgbColor = m_rgb[colorIndex];

		str.Format( L"\
%d\t%d\t%d\t%d\t%d\t%d\t%d\t\
%s\t%s\t%s\t%s\t\
%s\t%s\t%s\t%s\t\
%s\t%s\t%s\t%s\t\
%s\t%s\t%s\t%s\t\
%s\t%s\t%s\t\n",\
colorIndex, m_loop, x.group, x.match, x.group2, x.status, rgbColor,\
x.line, x.united, x.generation, x.source,\
x.name, x.rowid,  x.birth,  x.death,\
x.father, x.rowidF, x.birthF, x.deathF,\
x.mother, x.rowidM, x.birthM, x.deathM,\
x.rowidS, x.spouses, x.lineF\
);

		if( m_contracted )
			fwprintf( textU, str );
		else
			fwprintf( textD, str );
	}
	if( m_contracted )
		emptyLine( textU, m_loop );
	else
		emptyLine( textD, m_loop );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContract::emptyLine( FILE* fl, int loop )
{
	UINT i;
	fwprintf( fl, L"\t%d\t", loop );
	for( i = 2; i < S_RGBCOLOR; i++ )	// egy üres sor az azonos nevû emberek után
		fwprintf( fl, L"\t" );
	fwprintf( fl, sWHITE );						// RGB color white
	for( ; i < COLUMNSCOUNT; i++ )  // eg yüres sor az azonos nevû emberek után
		fwprintf( fl, L"\t" );
	fwprintf( fl, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CContract::printRef( int group )
{
		str.Format( L"\
%2d %2d %2s %9s %2s %1s %1s \
%-39s %12s %12s \
%8s %-30s %12s %12s \
%8s %-30s %12s %12s \
%8s %s",\
m_loop, group, L" ", L" ", L" ", r.generation, L" ",
L" ", r.birth, r.death,
L" ", r.father, r.birthF, r.deathF,
L" ", r.mother, r.birthM, r.deathM,
L" ", r.spouses
);
	fwprintf( fU, L"<span style=\"background:DeepPink\">%s</span>\n", str );

	fwprintf( textU, str );
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContract::resetRef()
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
void CContract::setRef( int i )
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
void CContract::openUnited()
{
	CString fileName;
	fileName.Format( L"peopleUnited_%d", m_loop );
	unitedSpec = theApp.openHtmlFile( &fU, fileName, L"w+" );

	createHead( L"AZONOS NEVÛ EMBEREK, AKIK AZONOSAK IS, EZÉRT BEJEGYZÉSEIK ÖSSZEVONHATÓAK" );
	fwprintf( fU, m_head );
	fwprintf( fU, m_description1 );
	fwprintf( fU, m_description2 );
	str = L"A szürke hátterû bejegyzéseket egyesítettük a zöld hátterû bejegyzéssel.\n\
Ha egy azonos nevû csoportban több különbözõ egyesítés lehetséges, akkor azok a 'g'-group oszlopba található számmal vannak meglülönböztetve.\n\n";
	fwprintf( fU, str );

	CString columns;
	columns.Format( L"\
%2s %2s %2s %2s \
%9s %2s %1s %1s    \
%s %s %s %s",\
L"gr",L"mt",L"gp",L"st",\
L"line", L"u", L"G", L"S",\
L"név----------------------------- rowid születés---- halál------- ",\
L"apa----------------------------- rowid születés---- halál------- ",\
L"anya---------------------------- rowid születés---- halál------- ",\
L"rowid-házastársak---------------"\
);


	fwprintf( fU, L"<font color='red'>%s</font><br>", columns );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContract::openDifferent()
{
	CString fileName;
	fileName.Format( L"peopleDifferent_%d", m_loop );
	differentSpec = theApp.openHtmlFile( &fD, fileName, L"w+" );

	createHead( L"AZONOS NEVÛ EMBEREK, AKIK KÜLÖNBÖZNEK EGYMÁSTÓ" );
	fwprintf( fD, m_head );
	fwprintf( fD, m_description2 );


	CString columns;
	columns.Format( L"\
%9s %2s %1s %1s    \
%s %s %s %s",\
L"line", L"u", L"G", L"S",\
L"név----------------------------- rowid születés---- halál------- ",\
L"apa----------------------------- rowid születés---- halál------- ",\
L"anya---------------------------- rowid születés---- halál------- ",\
L"rowid-házastársak---------------"\
);

	fwprintf( fD, L"<font color='red'>%s</font><br>", columns );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContract::createHead( CString title  )
{
	m_head.Format( L"\
<HEAD>\n\
<style>\n\
</style>\n\
</HEAD>\n\
<BODY>\n\
<center>%s</center><br><br>\n\n\
<pre>\n\
%-21s %s<br>\
%-21s %s<br>\
%-21s %d<br><br>\
",
title,\
L"Adatbázis:",\
theApp.m_databaseSpec,\
L"Lista készült:",\
theApp.getPresentDateTime(),\
L"Egyezések min. száma:",\
m_azonos\
);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContract::query( CString command )
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
BOOL CContract::query1( CString command )
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
BOOL CContract::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContract::list()
{
	int status;
	int group;
	int colorIndex;
	int	i;

	
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		str.Format( L"%d", i+1 );
		push( str );

		str.Format( L"%d", m_loop );
		push( str );

		group = vPeople.at(i).group;
		str.Format( L"%d", group );
		push( str );

		str.Format( L"%d", vPeople.at(i).match );
		push( str );
		
		str.Format( L"%d", vPeople.at(i).group2 );
		push( str );

		status = vPeople.at(i).status;
		switch( status )
		{
		case 0:
			colorIndex = 0;
			break;
		case 1:
			colorIndex = group % 10;
			break;
		case -1:
			colorIndex = 9;
			break;
		}
		str.Format( L"%d", status );
		push( str );

		str.Format( L"%u", m_rgb[ colorIndex ] );
		push( str );

		push( vPeople.at(i).line );
		push( vPeople.at(i).united );
		push( vPeople.at(i).generation );
		push( vPeople.at(i).source );

		push( vPeople.at(i).rowid );
		push( vPeople.at(i).name );
		push( vPeople.at(i).birth );
		push( vPeople.at(i).death );
		
		push( vPeople.at(i).rowidF );
		push( vPeople.at(i).father );
		push( vPeople.at(i).birthF );
		push( vPeople.at(i).deathF );
	
		push( vPeople.at(i).rowidM );
		push( vPeople.at(i).mother );
		push( vPeople.at(i).birthM );
		push( vPeople.at(i).deathM );
		
		push( vPeople.at(i).rowidS );
		push( vPeople.at(i).spouses );
		push( vPeople.at(i).lineF );

		++nItem;
	}

	for( i = 0; i < S_RGBCOLOR; i++ )	// egy üres sor az azonos nevû emberek után
		push( L" " );
	push( sWHITE );						// RGB color white
	for( ; i < COLUMNSCOUNT-1; i++ )  // eg yüres sor az azonos nevû emberek után
		push( L" " );

	++nItem;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContract::push( CString item )
{

	int tLen;
	TCHAR* sT;
	tLen	= item.GetLength()+1;
	sT		= new TCHAR[tLen];
	_tcscpy_s( sT, tLen, item.GetBuffer() );
	tableLines.push_back( sT );

}
