// InputGedcom.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "gedcomin.h"
#include "GAtoDB.h"
#include <algorithm>
#include "utilities.h"
#include "ProgressWnd.h"
//_Bool is_utf8( UCHAR * string, _int64 length );
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INDI rekordokat megkeresi a ged-fájlban és mindegyikre meghívja a recordINDI függvényt
void CGedcomIn::fillVectors()
{
	theApp.m_inputCode = gedCHAR( theApp.m_gedFileSpec );
	if( theApp.m_inputCode == -1 )
		theApp.m_inputCode = GetInputCode( theApp.m_gedFileSpec );

	file_ged.Open( theApp.m_gedFileSpec, CFile::modeRead );
	if( theApp.m_inputCode == UTF8BOM )
	{
		file_ged.Seek( 3, CFile::begin );
	}

	fill_v_indi();
	fill_v_fam();

	if( v_fam.size() ) sync_fam_indi();   // ha több felesége van, akkor
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGedcomIn::gedCHAR( CString fileSpec )
{
	CString cLine;
	CString code;
	int		pos;
	file_ged.Open( theApp.m_gedFileSpec, CFile::modeRead );
	while( file_ged.ReadString( cLine ) )
	{
		if( cLine.Find( L"CHAR" ) != -1 )
			break;
	}
	file_ged.Close();
	if( ( pos= cLine.Find( L"CHAR" ) ) == -1 ) return -1; // nincs CHAR
	code = cLine.Mid( pos + 5 );
	if( code == L"ANSI" ) return ANSI;
	else return UTF8;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INDI rekordokat megkeresi a ged-fájlban és mindegyikre meghívja a recordINDI függvényt
void CGedcomIn::fill_v_indi()
{
	str.Format( L"%s fájl beolvasása folyik...", theApp.m_gedFileName );
	CProgressWnd wndP( NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, (int)file_ged.GetLength() );

	CString cLine;	
	GEDLINE lxtv;

	v_famc.clear();
	v_indi.clear();
	vPhotos.clear();

	theApp.execute( L"BEGIN" );
	while( file_ged.ReadString( cLine ) )
	{
		fpos = file_ged.GetPosition();
		extract( cLine, &lxtv );
		if( lxtv.level < 0 ) continue;


		if( lxtv.tag.GetAt(0) == '_' ) continue;
		v_lxtv.clear();
		if( lxtv.tag == L"INDI" )
		{
			v_lxtv.push_back( lxtv );
			recordINDI( &lxtv );
		}
	}
	theApp.execute( L"COMMIT" );
	file_ged.Close();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FAM rekordokat megkeresi a ged-fájlban és mindegyikra meghívja a recordFAM függvényt
void CGedcomIn::fill_v_fam()
{
	file_ged.Open( theApp.m_gedFileSpec, CFile::modeRead );

	str.Format( L"%s fájl beolvasása folyik...", theApp.m_gedFileName );
	CProgressWnd wndP( NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, (int)file_ged.GetLength() );

	int		cnt = 0;
	CString cLine;
	GEDLINE lxtv;
	v_famc.clear();
	v_fam.clear();
	v_chil.clear();
	while( file_ged.ReadString( cLine ) )
	{
		fpos = file_ged.GetPosition();
		extract( cLine, &lxtv );
		if( lxtv.level < 0 ) continue;

		if( lxtv.tag.GetAt(0) == '_' ) continue;
		v_lxtv.clear();
		if( lxtv.tag == L"FAM" )
		{
			v_lxtv.push_back( lxtv );
			++cnt;							// a házasság sorszáma. MIre való ez?
			recordFAM( &lxtv, cnt );  
		}
	}
	for( UINT i = 0; i < v_chil.size(); ++i )
		v_chil.at(i).cnt = i; // hoyg az eeredit sorrend visszaállítható legyen???

	file_ged.Close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INDI rekordot beteszi a v_indi vektorba
// A INDIFAMC azonosítójaó családot beteszi a v_infifam vektorba, megadva a refI és refM értékeket
void CGedcomIn::recordINDI( GEDLINE* gl )
{
	CString cLine;
	GEDLINE lxtv;
	CString date;

	INDI	indi;
	PHOTOS	photos;
	UINT	last;

	INDIFAMC famc;
	INDIFAMS fams;

	int pos;

	indi.refI = gl->xref;
	indi.numOfSpouses = 0;

	fams.order = 0;				// házasság sorszáma
	while( file_ged.ReadString( cLine ) )
	{
		if( cLine.IsEmpty() ) break;
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM )
			cLine = Utf8ToAnsi( cLine );
		cLine.Replace( '\'', '|' );

		extract( cLine, &lxtv );
		if( lxtv.level < 0 ) continue;
		if( lxtv.tag.GetAt(0) == '_' ) continue;
		if( lxtv.level == 0 ) break;
		
		last = last1Bellow( lxtv.level );

		if( lxtv.tag == L"NAME" )
		{
//			indi.titolo = lxtv.value;
			getNameLloyd( lxtv.value, &indi );
		}
		else if( lxtv.tag == L"SEX" )
			if( lxtv.value == L"M" )
				indi.sex = L"1";
			else
				indi.sex = L"2";
		else if( lxtv.tag == L"OCCU" )
			indi.occupation = lxtv.value;
		else if( lxtv.tag == L"NOTE" )
			indi.comment = lxtv.value;
		else if( lxtv.tag == L"DATE" )
		{
			if( v_lxtv.at(last).tag == L"BIRT" )
				indi.birth_date = date2date( lxtv.value );
			else if( v_lxtv.at(last).tag == L"DEAT" )
				indi.death_date = date2date( lxtv.value );
		}
		else if( lxtv.tag == L"PLAC" )
		{
			if( v_lxtv.at(last).tag == L"BIRT" )
				indi.birth_place = lxtv.value;
			else if( v_lxtv.at(last).tag == L"DEAT" )
				indi.death_place = lxtv.value;
		}
		else if( lxtv.tag == L"EDUC" )
		{
			indi.occupation += L", ";
			indi.occupation += lxtv.value;
		}
		else if( lxtv.tag == L"NPFX" )			// titulus
			indi.title	= lxtv.value;
		else if( lxtv.tag == L"SURN" )
			indi.last_name = lxtv.value;
		else if( lxtv.tag == L"GIVN" )
			indi.first_name = lxtv.value;
		else if( lxtv.tag == L"SOUR" )
			indi.source = lxtv.value;
		else if( lxtv.tag == L"CONT" )
		{
			if( v_lxtv.at(last).tag == L"SOUR" )
			{
				indi.source += L" ";
				indi.source += lxtv.value;
			}
			else if( v_lxtv.at(last).tag == L"NOTE" )
			{
				indi.comment += L" ";
				indi.comment += lxtv.value;
			}
		}
		else if( lxtv.tag == L"FAMC" )
		{
			famc.refI = v_lxtv.at(last).xref;		// család azonosítója
			famc.refF = lxtv.value;					// gyerek azonosítója
			v_famc.push_back( famc );				// az anya-index meghatározására használja a sync_fam_indi-ben
		}
		else if( lxtv.tag == L"FAMS" )				// a FAMS felsorolások sorrendje a házasságok sorrendje (feltételezés)
		{
			fams.refI = v_lxtv.at(last).xref;		// család azonosítója
			fams.refF = lxtv.value;					// házastárs azonosítója
			fams.order++;
			fams.sex  = indi.sex;
			indi.numOfSpouses++;
			v_fams.push_back( fams );				// a házastársak sorrendjénke meghatározására hsználja a sync_fam_indi-ben

		}
		else if ( lxtv.tag == L"FILE" )
		{
			photos.refI = indi.refI;
			photos.link = lxtv.value;
			vPhotos.push_back( photos );
		}
		else if( lxtv.tag == L"NPFX" )				// surname prefix
			indi.title = lxtv.value;

		v_lxtv.push_back( lxtv );
	}

	indi.tablenumber		= getTablenumber( indi.last_name );
	indi.parentIndex		= 0;
	indi.parentIndexCalc	= 0;
	indi.orderFather		= 0;
	indi.orderMother		= 0;
	v_indi.push_back( indi );
		
	file_ged.Seek( fpos, 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A FAM rekordot beteszi a v_fam és a v_chil vektorokba,  cnt a házasság sorsszáma (az eredeti sorrend visszaállítására??? )
void CGedcomIn::recordFAM( GEDLINE* gl, int cnt )
{
	CString cLine;
	GEDLINE lxtv;
	CString date;
	
	FAM		fam;
	CHIL	chil;
	UINT	last;
	int		numOfChildren = 0;

	fam.cnt	= 0;
	fam.date.Empty();
	fam.refH.Empty();
	fam.marriageH = 1;
	fam.marriageHAll = 1;
	fam.marriageW = 1;
	fam.marriageWAll = 1;
	fam.numOfChildren = 0;
	fam.place.Empty();
	fam.refW.Empty();
	fam.refF = gl->xref;
	while( file_ged.ReadString( cLine ) )
	{
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM  ) cLine = Utf8ToAnsi( cLine );

		extract( cLine, &lxtv );
		if( lxtv.level < 0 ) continue;
		if( lxtv.tag.GetAt(0) == '_' ) continue;
		if( lxtv.level == 0 ) break;
		
		last = last1Bellow( lxtv.level );  

		if( lxtv.tag == L"HUSB" )
			fam.refH = lxtv.value;
		else if( lxtv.tag == L"WIFE" )
			fam.refW = lxtv.value;
		else if( lxtv.tag == L"DATE" )
		{
			if( v_lxtv.at(last).tag == L"MARR" )
				fam.date = date2date( lxtv.value );
		}
		else if( lxtv.tag == L"PLAC" )
		{
			if( v_lxtv.at(last).tag == L"MARR" )
				fam.place = lxtv.value;
		}
		else if( lxtv.tag == L"CHIL" )
		{
			str = lxtv.value;
			str.Replace( '@', ' ' );
			str.Replace( 'I', ' ' );
			str.Trim();
			chil.numRefI		= _wtoi( str );
			chil.refF			= fam.refF;
			chil.refH			= fam.refH;
			chil.refW			= fam.refW;
			chil.refC			= lxtv.value;
			chil.parentIndex	= 0;
			chil.parentIndexCalc	= 0;
			chil.orderFather	= numOfChildren + 1;
			chil.orderMother	= 0;
			v_chil.push_back( chil );
			++numOfChildren;
		}
		v_lxtv.push_back( lxtv );
	}
	fam.cnt		= cnt;
	fam.marriageH = 1;
	fam.marriageHAll = 1;
	fam.marriageW = 1;
	fam.marriageWAll = 1;
	fam.numOfChildren	= numOfChildren;
	v_fam.push_back( fam );
	v_lxtv.clear();
	file_ged.Seek( fpos, 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A level paraméternél eggyel alacsonaybb level-û sor indexet keresi meg és adja vissza 
UINT CGedcomIn::last1Bellow( int level )
{
	UINT i;
	for( i = v_lxtv.size() - 1; i >= 0; --i )
	{
		if( v_lxtv.at(i).level == level - 1  )
			break;
	}
	return i;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A NAME tagû sor value felbontása elõnév-családnév-keresztnév- comment substringekre 
// Ez specifikusan egy ged fájlra készült. nem valószínû, hogy a standard NAME-ben íg lennének az elõnevek és a comment
void CGedcomIn::getName( CString value, INDI* indi )
{
	return;
	CString first_name(L"");
	CString titolo(L"");
	CString title(L"");
	CString comment(L"");
	CStringArray A;
	CString rest(L"");
	int n;
	int ret;

	if( (n= value.Find( L"nemes és nemzetes" ) ) != -1 )
		n = 2;
	n = wordList( &A, value, ' ', FALSE );
	
	indi->last_name = A[0];
	for( int i = 1; i < n; ++i )
	{
		if( (ret = theApp.isFirstName( A[i] ) ) != - 1 )
		{
			first_name += L" ";
			first_name += A[i];
		}
		else
		{
			rest= packWords( &A, i, n - i - 1 );
			break;
		}
	}
	first_name.Trim();
	indi->first_name = first_name;
	if( !rest.IsEmpty() )
	{
		n = wordList( &A, rest, ' ', FALSE );
		if( n == 1 )
		{
			if( rest.Right(1) =='i' )
				titolo = rest;
			else
				indi->comment = rest;
		}
		else
		{
			for( int i = 0; i < n; ++i )
			{
				if( A[i] == L"és" )
				{
					if( (i - 1) > 0 && (i + 1) < n )  
						titolo.Format( L"%s és %s", A[i-1], A[i+1] );
					break;
				}
			}
		}
		if( !titolo.IsEmpty() )
			titolo = firstUpper( titolo );
		else if( n > 1 )
			titolo = rest;
		indi->title		= title;
		indi->titolo	= titolo;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A NAME tagû sor value felbontása elõnév-családnév-keresztnév- comment substringekre 
// Ez specifikusan egy ged fájlra készült. nem valószínû, hogy a standard NAME-ben íg lennének az elõnevek és a comment
void CGedcomIn::getNameLloyd( CString value, INDI* indi )
{
	CString first_name(L"");
	CString titolo(L"");
	CString title(L"");
	CString comment(L"");
	CStringArray A;
	CString rest(L"");
	int n;
	int ret;
	int pos1;
	int pos2;
	CString firstnames;
	CString lastname;

	if( ( pos1 = value.Find( '/' ) ) != -1 )
	{
		if( pos2 = value.ReverseFind( '/' ) != -1 )
		{
			firstnames	= value.Left( pos1-1 );
			lastname	= value.Mid( pos1+1, value.GetLength()-pos1-2 );
			indi->first_name	= firstnames;
			indi->last_name		= lastname;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::sync_fam_indi()
{
// v_fams vektorban lévõ család-azonosítók megkereséses a v_fam vektoran 
// és a házasság sorszámának valamint az összes házasságok számának áthelyezése a v_fam vektorba
	CString refF;  // család azonosító
	for( UINT i = 0; i < v_fams.size()-1; ++i )
	{
		refF = v_fams.at(i).refF;
		for( UINT j = 0; j < v_fam.size(); ++j )
		{
			if( v_fam.at(j).refF == refF )
			{
				if( v_fams.at(i).sex == L"1" )	// ha a férj volt megadva
				{
					v_fam.at(j).marriageH = v_fams.at(i).order;
					v_fam.at(j).marriageHAll = getMarriageAll( v_fams.at(i).refI );
				}
				else							// ha a feleség volt megadva
				{
					v_fam.at(j).marriageW = v_fams.at(i).order;
					v_fam.at(j).marriageWAll = getMarriageAll( v_fams.at(i).refI );
				}
			}
		}
	}


// a családba tartozó gyerekek indi.parentIndex-ének beállítása az anya sorszámára

// a v_chil eleve a refF szerint rendezett

	int	cnt = 0;
	int	z;
	int	parentIndex;
	int	indexC;
	int	ixF;
	CString child;
	CString refC;

	for( UINT j = 0; j < v_chil.size()-1; ++j )
	{
		refF = v_chil.at(j).refF;
		if( v_chil.at(j).refF == v_chil.at(j+1).refF )
		{
			ixF = getIndexFam( v_chil.at(j).refF );
			child = getIndi( v_chil.at(j).refC );

			z = v_fam.at(ixF).marriageHAll;
			v_chil.at(j).parentIndex		= 0;
			v_chil.at(j+1).parentIndex		= 0;

			if( v_fam.at(ixF).marriageHAll > 1 )   // csak 1-nél több feleség esetén ad értékel neki
			{
				parentIndex = v_fam.at(ixF).marriageH;
		
				v_chil.at(j).parentIndexCalc		= parentIndex;
				v_chil.at(j+1).parentIndexCalc	= parentIndex;

				// csak az elsõ gyerek parentIndex-ét tölti ki
				if( !cnt )
				{
					v_chil.at(j).parentIndex		= parentIndex;
				}

			// ha az apának csak 1 felesége volt, akkor annak sorszámát nem tartja nyilván
				refC = v_chil.at(j).refC;
				if( ( indexC = getIndexIndi( refC ) ) != -1 )
				{
					v_indi.at( indexC ).parentIndexCalc	= parentIndex; //v_chil.at(j).parentIndexCalc;
					if( !cnt )
						v_indi.at( indexC ).parentIndex	= v_chil.at(j).parentIndex;  // ez lehet 0 is!!!!
					else
						v_indi.at( indexC ).parentIndex	= 0;
					v_indi.at( indexC ).orderFather		= v_chil.at(j).orderFather;
					v_indi.at( indexC ).orderMother		= v_chil.at(j).orderMother;
				}

				refC = v_chil.at(j+1).refC;
				if( ( indexC = getIndexIndi( refC ) ) != -1 )
				{
					v_indi.at( indexC ).parentIndexCalc	= parentIndex;
//					if( !cnt )
//						v_indi.at( indexC ).parentIndex	= v_chil.at(j).parentIndex;
//					else
						v_indi.at( indexC ).parentIndex	= 0;
					v_indi.at( indexC ).orderFather		= v_chil.at(j).orderFather;
					v_indi.at( indexC ).orderMother		= v_chil.at(j).orderMother;
				}
				++cnt;
			}
		}
		else
		{
//			if( cnt == 1 )			// ha csak 2 gyerek volt, akkor 2.-at törli
//				v_chil.at(j).parentIndex		= 0;
			cnt = 0;
		}
	}

// orderFather beállítása

	int	fix;
	int		orderFather;
	int		orderMother;

//	std::sort( v_chil.begin(), v_chil.end(), multiSort_refH );  // apa-anya szerint rendez
	std::sort( v_chil.begin(), v_chil.end(), sortChil_cnt );  // apa-anya szerint rendez
	for( UINT i = 0; i < v_chil.size()-2; ++i )
	{
		cnt = 1;
		fix = i;
		while(  i < v_chil.size() - 1 &&  v_chil.at(i).refH == v_chil.at(i+1).refH )	// megszámolja az apa gyerekeinek számát
		{
			++i;		// index a folytatáshoz
			++cnt;		// egy apának hány gyereke van
		}
		orderFather = 1;
		for( int j = 0; j < cnt; ++j )
		{
			v_chil.at(fix+j).orderFather = orderFather;

			str = v_chil.at(fix+j).refC;
			if( (_ix = getIndexIndi( v_chil.at(fix+j).refC )  ) != -1 )
				v_indi.at(_ix).orderFather	= orderFather;

			++orderFather;
		}
	}

// orderMother beállítása
	std::sort( v_chil.begin(), v_chil.end(), sortChil_cnt );  // eredeti sorrend
	std::sort( v_chil.begin(), v_chil.end(), multiSort_refW );  // apa-anya szerint rendez
	for( UINT i = 0; i < v_chil.size()-1; ++i )
	{
		cnt = 1;
		fix = i;
		while(  i < v_chil.size() - 1 &&  v_chil.at(i).refW == v_chil.at(i+1).refW )	// megszámolja az anya gyerekeinek számát
		{
			++i;		// index a folytatáshoz
			++cnt;		// egy anyának hány gyereke van
		}
		orderMother = 1;
		for( int j = 0; j < cnt; ++j )
		{
			v_chil.at(fix+j).orderMother = orderMother;

			str = v_chil.at(fix+j).refC;
			if( (_ix = getIndexIndi( v_chil.at(fix+j).refC )  ) != -1 )
				v_indi.at(_ix).orderMother	= orderMother;

			++orderMother;
		}
	}
	std::sort( v_chil.begin(), v_chil.end(), sortChil_cnt );  // eredeti sorrend
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// egy GEDCOM sor felbontása 'level-xref-tag-value' struktúrába 
//
// return: lxtv structure
bool extract( CString cLine, GEDLINE* lxtv )
{
	CStringArray A;
	int n;
	int i;

	if( cLine.Find( L"INDI" ) != -1 )
		int z = 0;

	cLine.Trim();
	if( cLine.IsEmpty() ) return false;

	lxtv->level = -1;
	lxtv->xref.Empty();
	lxtv->value.Empty();

	n = wordList( &A, cLine, ' ', FALSE );
	if( n == 1 ) return false;

	int level = _wtoi( A[0] );
	if( level < 0 || level > 99 ) return false;
	lxtv->level	=  level;
	
	i = 1;
	if( A[1].GetAt(0) == '@' )
	{
		lxtv->xref = A[1];
		++i;
	}
	
	if( n <= i || i < 0 ) 
		int z = 1;

	lxtv->tag = A[i];
	if( n > i + 1 )
		lxtv->value = packWords( &A, i+1, n-i-1 );
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGedcomIn::getMarriageAll( CString refI )
{
	for( UINT i = 0; i < v_indi.size(); ++i )
	{
		if( v_indi.at(i).refI == refI )
		{
			return v_indi.at(i).numOfSpouses;
			break;
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGedcomIn::getTablenumber( CString lastname )
{
	CString rowid;
	CString tableHeader;
	tableHeader.Format( L"%s %s", L"%%%", lastname );
	m_command.Format( L"SELECT rowid FROM tables WHERE familyName ='%s'", lastname );
	if( !theApp.query( m_command ) ) return L"0";

	rowid = theApp.m_recordset->GetFieldString( 0 );
	if( !theApp.m_recordset->RecordsCount() )
	{
		m_command.Format( L"INSERT INTO tables ( familyNumber, tableHeader, percent, familyName) VALUES ( 1, '%s', '%s','%s' )", tableHeader, L"%%%", lastname );
		if( !theApp.execute( m_command ) ) return 0;
		m_command = L"SELECT last_insert_rowid() FROM tables";
		if( !theApp.query( m_command ) ) return L"";
		rowid = theApp.m_recordset->GetFieldString( 0 );
	}
	return rowid;
}
