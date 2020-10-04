#include "stdafx.h"
#include "Dragon.h"
#include "GA_input.h"
#include "afxdialogex.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::insertIntoFiles( CString fileSpec )
{
	CFileStatus status;
	CString mtime;
	CString ctime;
	CString atime;
	CString size;
	CString presentDate;

	if( CFile::GetStatus( fileSpec, status) )
	{
		mtime	= status.m_mtime.Format( L"%Y.%m.%d %H:%M:%S" );
		ctime	= status.m_ctime.Format( L"%Y.%m.%d %H:%M:%S" );
		atime	= status.m_atime.Format( L"%Y.%m.%d %H:%M:%S" );
		size	= thousend( status.m_size );
	}
	// a fájl azonosítóinak megõrzése
	presentDate = theApp.getPresentDateTime();

	m_command.Format( L"SELECT rowid FROM inputFiles WHERE filename ='%s'", fileSpec );
	if( !query( m_command ) )return;


	if( !m_recordset.RecordsCount() )
	{
		m_command.Format( L"INSERT INTO inputFiles ( filename, size, created, modified, loaded ) VALUES ( '%s', '%s', '%s', '%s', '%s' ) ", fileSpec, size, mtime, ctime, presentDate );
		if( !theApp.execute( m_command ) ) return;
		m_command = L"SELECT last_insert_rowid() FROM inputFiles";	
		if( !query( m_command ) ) return;
	}
	m_fileNumber = _wtoi( m_recordset.GetFieldString(0) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// I N S E R T   T A B L E H E A D E R   A N D   B R A N C H E S ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::insertTableHeader()
{
	CString tableHeader;
	CString values;
	if( !theApp.execute( L"BEGIN" ) ) return;
	for( UINT i = 0; i < v_tableHeader.size(); ++i )
	{
		tableHeader = v_tableHeader.at(i).tableHeader;
		if( !v_tableHeader.at(i).known_as.IsEmpty() )
		{
			tableHeader += L" \n";
			tableHeader += v_tableHeader.at(i).known_as;
		}

		values.Format( L"'%d','%d', '%d','%s','%s','%s', '%s','%s', '%s','%s', '%s', '%s', '%s', '%d','%s'",
		m_fileNumber,
		v_tableHeader.at(i).familyNumber,
		v_tableHeader.at(i).lineNumber,
		tableHeader,									//v_tableHeader.at(i).tableHeader,
		v_tableHeader.at(i).percent,
		v_tableHeader.at(i).familyName,
		v_tableHeader.at(i).alias,
		v_tableHeader.at(i).titolo,
		v_tableHeader.at(i).tableRoman,
		v_tableHeader.at(i).comment,
		v_tableHeader.at(i).torzs,
		v_tableHeader.at(i).known_as,
		v_tableHeader.at(i).arm,
		v_tableHeader.at(i).joint,
		v_tableHeader.at(i).folyt);
		m_command.Format( L"INSERT INTO tables ( %s ) VALUES ( %s )", m_fieldsT, values );
		if( !theApp.execute( m_command ) ) return;


	}
	if( !theApp.execute( L"COMMIT" ) ) return;
	v_tableHeader.clear();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// I N S E R T   A L L   P E O P L E /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::insertEntries()
{

// emberek insertáláa
	

	insertDescendant();					// a v_brunch-hoz meg kell adni a rowid-jét!!

	for( UINT i = 0; i < v_marriages.size(); ++i )
	{
//		insertDescendant();			// minden házastárhoz a leszármazottatt új bejehgyzését hozza létre, hogy más rowid-ja legyen
		insertSpouseF( i );			// elõször a házastárs szüleit insertáljuk, hogy a házastársnak
		insertSpouseM( i );			// megadhassuk az apa-anya rowid-jét;
		insertDescendantSpouse( i );    
	}
	for( UINT i = 0; i < v_spouseSpouses.size(); ++i )
	{
		insertSpouseS( i );
	}

// házasságok insertálása
	for( UINT i = 0; i < v_marriages.size(); ++i )
	{
		insertDescMarriage( i );
		insertSpouseParentsMarriage( i);
	}

	for( UINT i = 0; i < v_spouseSpouses.size(); ++i )
	{
		insertSpouseSpousesMarriage( i );
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::insertDescendant( )
{
	d.last_name		= m_familyName;
	d.rowid			= insertAny( &d );
	m_rowidLastDescendant = d.rowid;
	return( d.rowid );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::insertDescendantSpouse( UINT i )
{
	CString rowid(L"0");
	s.source		= 2;
	s.generation	= ' ';
	s.sex_id		= v_marriages.at(i).sex_id;
	s.title			= v_marriages.at(i).title;
	s.titolo		= v_marriages.at(i).titolo;
	s.first_name	= v_marriages.at(i).first_name;
	s.last_name		= v_marriages.at(i).last_name;
	s.birth_place	= v_marriages.at(i).birth_place;
	s.birth_date	= v_marriages.at(i).birth_date;
	s.death_place	= v_marriages.at(i).death_place;
	s.death_date	= v_marriages.at(i).death_date;
	s.comment		= v_marriages.at(i).comment;
	s.father_id		= v_marriages.at(i).rowidF;
	s.mother_id		= v_marriages.at(i).rowidM;
	s.mother_index	= 0;
	s.mother_index2	= 0;
	s.folyt			= L"";
	s.tableAncestry	= 0;
	s.lineNumber	= m_lineNumber;
	s.arm			= v_marriages.at(i).arm;
	s.csalad.Empty();
	s.gap			= 0;

	rowid = insertAny( &s );
	v_marriages.at(i).rowid = rowid;
	v_marriages.at(i).spouse_id = d.rowid;  // ez új, minden házastárshoz a leszármazott más bejegyzése tartozik. de mégse

	return ( rowid );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::insertSpouseF( UINT i )
{
	CString rowidF(L"0");

	if( !v_marriages.at(i).father.IsEmpty() )				// az apa csak utónevével szerepel
	{
		sf.source			= 3;
		sf.generation		= ' ';
		sf.sex_id			= v_marriages.at(i).sex_idF;
		sf.title			= v_marriages.at(i).titleF;
		sf.titolo			= v_marriages.at(i).titoloF;
		sf.last_name		= v_marriages.at(i).lastNameF;
		sf.first_name		= v_marriages.at(i).firstNameF;
		sf.birth_place		= v_marriages.at(i).birthPlaceF;
		sf.birth_date		= v_marriages.at(i).birthDateF;
		sf.death_place		= v_marriages.at(i).deathPlaceF;
		sf.death_date		= v_marriages.at(i).deathDateF;
		sf.comment			= v_marriages.at(i).commentF;
		sf.tableAncestry	= 0;
		sf.lineNumber		= m_lineNumber;
		sf.father_id		= L"0";
		sf.mother_id		= L"0";
		sf.arm.Empty();
		sf.csalad.Empty();
		s.gap				= 0;

		rowidF =  insertAny( &sf );
	}
	v_marriages.at(i).rowidF = rowidF;
	return ( rowidF );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::insertSpouseM( UINT i )
{
	CString rowidM(L"0");
	if( !v_marriages.at(i).mother.IsEmpty() )				// az apa csak utónevével szerepel
	{
		sm.source			= 4;
		sm.generation		= ' ';
		sm.sex_id			= v_marriages.at(i).sex_idM;
		sm.title			= v_marriages.at(i).titleM;
		sm.titolo			= v_marriages.at(i).titoloM;
		sm.last_name		= v_marriages.at(i).lastNameM;
		sm.first_name		= v_marriages.at(i).firstNameM;
		sm.birth_place		= v_marriages.at(i).birthPlaceM;
		sm.birth_date		= v_marriages.at(i).birthDateM;
		sm.death_place		= v_marriages.at(i).deathPlaceM;
		sm.death_date		= v_marriages.at(i).deathDateM;
		sm.comment			= v_marriages.at(i).commentM;
		sm.father_id		= L"0";
		sm.mother_id		= L"0";

		sm.tableAncestry	= 0;
		sm.lineNumber		= m_lineNumber;
		sm.arm.Empty();	
		sm.csalad.Empty();
		sm.gap				= 0;
		
		rowidM = insertAny( &sm );
	}
	v_marriages.at(i).rowidM = rowidM;
	return( rowidM );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a házastárs további házastársai
CString CGaInput::insertSpouseS( UINT i )
{
	CString  rowid(L"0");

	ss.source			= 5;
	ss.generation		= ' ';
	ss.sex_id			= v_spouseSpouses.at(i).sex_id;
	ss.title			= v_spouseSpouses.at(i).title;
	ss.titolo			= v_spouseSpouses.at(i).titolo;
	ss.last_name		= v_spouseSpouses.at(i).last_name;
	ss.first_name		= v_spouseSpouses.at(i).first_name;
	ss.birth_place		= v_spouseSpouses.at(i).birth_place;
	ss.birth_date		= v_spouseSpouses.at(i).birth_date;
	ss.death_place		= v_spouseSpouses.at(i).death_place;
	ss.death_date		= v_spouseSpouses.at(i).death_date;
	ss.comment			= v_spouseSpouses.at(i).comment;
	ss.tableAncestry	= 0;
	ss.lineNumber		= m_lineNumber;
	ss.father_id		= L"0";
	ss.mother_id		= L"0";
	ss.arm.Empty();	
	ss.csalad.Empty();
	ss.gap				= 0;

	rowid = insertAny( &ss );
	v_spouseSpouses.at(i).rowid	= rowid;
	return( rowid );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::insertAny( PEOPLE* p )
{
	p->comment.TrimLeft();
	CString last_name(L"");
	CString other_names(L"");
	int pos;

	last_name = p->last_name;
	if( (pos = p->last_name.Find( L"_(" ) ) != -1 )
	{
		last_name	= p->last_name.Left( pos );
		other_names	= p->last_name.Mid( pos+2);
		other_names.Replace( ')', ' ' );
		other_names.TrimRight();
	}
/*
	m_fieldsP = L"\
fileNumber,\
familyNumber,\
tableNumber,\
lineNumber,\
source,\
united,\
spouse,\
spouseparent,\
spousespouse,\
generation,\
sex_id,\
title,\
titolo,\
first_name,\
last_name,\
other_names,\
birth_place,\
birth_date,\
death_place,\
death_date,\
comment,\
father_id,\
mother_id,\
mother_index,\
mother_index2,\
folyt,\
tableAncestry,\
tableRoman,\
arm,\
orderFather,\
orderMother,\
csalad,\
gap\
";
*/
	CString values;
	values.Format( L"\
'%d','%d','%d','%d','%d','%d','%d','%d','%d',\
'%c','%d','%s','%s','%s','%s','%s','%s','%s',\
'%s','%s','%s','%s','%s','%d','%d','%s','%d',\
'%s','%s','%d','%d', '%s', '%d'",
m_fileNumber, m_tableHeader.familyNumber,v_tableHeader.size(),m_lineNumber,p->source,1,0,0,0,\
p->generation,p->sex_id,p->title,p->titolo,p->first_name.Trim(),last_name,other_names,p->birth_place,p->birth_date,\
p->death_place,p->death_date,p->comment,p->father_id,p->mother_id,p->mother_index,p->mother_index2,p->folyt,p->tableAncestry,
p->tableRoman,p->arm,p->orderFather,p->orderMother,p->csalad,p->gap );


    m_command.Format( L"INSERT INTO people (%s) VALUES (%s)", m_fieldsP, values );
	if( !theApp.execute( m_command ) ) return 0;

	++theApp.m_cntPeople;
	++m_rowid;	// az utoljára insertált ember azonosítója. Azért számoljuk és nem visszakérdezzük a SELECT 'last_insert_rowid'-val
				// mert ez magszakítaná a BEGIN_COMMIT tranzakciót és nagyon lelassulna a beolvasás!!!

	// hibajalzés
	if( p->first_name.IsEmpty() || p->last_name.IsEmpty() )
	{
		str.Format( L"%s %s ", p->last_name.TrimRight(), p->first_name.TrimRight() );
		if( !p->birth_place.IsEmpty() || !p->birth_date.IsEmpty() )
		{
			str += L" *";
			if( !p->birth_place.IsEmpty() )
			{
				str += p->birth_place;
				str += L" ";
			}
			str += p->birth_date;
			str.TrimRight();
		}
		if( !p->death_place.IsEmpty() || !p->death_date.IsEmpty() )
		{
			str += L" +";
			if( !p->death_place.IsEmpty() )
			{
				str += p->death_place;
				str += L" ";
			}
			str += p->death_date;
			str.TrimRight();
		}
		str += L" ";
		str += p->comment;
		fwprintf( fh3, L"%6dL %s<br>\n", m_lineNumber,m_cLine );
		fwprintf( fh3, L"%6dR %s<br><br>\n\n", m_rowid, str );
		++m_error_cnt3;
	}

	p->rowid.Format( L"%d", m_rowid );
	return( p->rowid );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// I N S E R T  M A R R I A G E S  ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CGaInput::insertDescMarriage( UINT i )
{
	CString order1(L"1");
	CString order2(L"1");
	CString spouse1_id;
	CString spouse2_id;
	
	int sex_id1 = d.sex_id;
	int sex_id2 = v_marriages.at(i).sex_id;

	int source;			// mindig a férjet jelenti!!

	if( d.rowid==L"0" || v_marriages.at(i).rowid == L"0" ) return 0;

	if( d.sex_id == MAN )
	{
		spouse1_id = d.rowid;
		spouse1_id = v_marriages.at(i).spouse_id;   // uj: minden házstárshoz a leszármazott más bejegyzése tartozik
		spouse2_id = v_marriages.at(i).rowid;
		order2.Format( L"%d", i+1 );
		source = 1;
	}
	else
	{
		spouse1_id = v_marriages.at(i).rowid;
		spouse2_id = d.rowid;
		spouse2_id = v_marriages.at(i).spouse_id;	// uj: minden házstárshoz a leszármazott más bejegyzése tartozik
		order1.Format( L"%d", i+1 );
		source = 2;
	}
	insertMarriage( spouse1_id, spouse2_id, sex_id1, sex_id2, order1, order2, v_marriages.at(i).place, v_marriages.at(i).date, source );
	return 1;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaInput::insertSpouseParentsMarriage( UINT i )
{
	CString rowidF = v_marriages.at(i).rowidF;
	CString rowidM = v_marriages.at(i).rowidM;

	int sex_id1 = v_marriages.at(i).sex_idF;
	int sex_id2 = v_marriages.at(i).sex_idM;


	if( rowidF==L"0" || rowidM == L"0" ) return 0;
	insertMarriage( rowidF, rowidM, sex_id1, sex_id2, L"1", L"1", L"", L"", 3 );
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaInput::insertSpouseSpousesMarriage( UINT i )
{
	CString rowidS = v_marriages.at( v_spouseSpouses.at(i).spouseIndex ).rowid;
	CString rowidSS = v_spouseSpouses.at(i).rowid;
	CString spouse1_id;
	CString spouse2_id;
	int sex_id1 = s.sex_id;
	int sex_id2 = v_spouseSpouses.at(i).sex_id;

	int source;

	if( rowidS==L"0" || rowidSS == L"0" ) return 0;

	CString order1(L"1");
	CString order2(L"1");

	if( d.sex_id == MAN )
	{
		order1.Format( L"%d", v_spouseSpouses.at(i).order );
		spouse1_id = rowidSS;
		spouse2_id = rowidS;
		source = 2;
	}
	else
	{
		order2.Format( L"%d", v_spouseSpouses.at(i).order );
		spouse1_id = rowidS;
		spouse2_id = rowidSS;
		source = 5;
	}

	insertMarriage( spouse1_id, spouse2_id, sex_id1, sex_id2, order1, order2, L"", L"", source );
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// eg házasság bejegyzése
int CGaInput::insertMarriage( CString spouse1_id, CString spouse2_id, int sex_id1, int sex_id2, CString orderHusband, CString orderWife, CString place, CString date, int source )
{

	if( spouse1_id == L"0" || spouse2_id == L"0" )
		return 0;

	CString values;
	values.Format( L"'%d','%d','%d','%d','%s','%s','%s','%s','%s','%s','%d'",
	m_fileNumber,\
	m_familyNumber,
	m_tableNumber,\
	m_lineNumber,\
	spouse1_id,\
	spouse2_id,\
	orderHusband,\
	orderWife,\
	place,\
	date,\
	source\
	);

	
	m_command.Format( L"INSERT INTO marriages (%s) VALUES (%s)", m_fieldsM, values );
	if( !theApp.execute( m_command ) ) return 0;



	if( sex_id1 == sex_id2 )
	{
		fwprintf( fh1, L"%8dT %8dL %s<br>", m_tableNumber, m_lineNumber, m_cLine );
		++m_error_cnt1;
	}
	++theApp.m_cntMarriages;
	return( 10 );
}


