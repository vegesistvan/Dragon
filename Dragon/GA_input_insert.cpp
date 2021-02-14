#include "stdafx.h"
#include "Dragon.h"
#include "GA_input.h"
#include "afxdialogex.h"
#include "utilities.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
bool CGaInput::insertIntoFiles( CString fileSpec )
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

	m_command.Format( L"SELECT rowid FROM filespec WHERE type='%d'", GA_HTML );
	if( !query( m_command ) )return false;

	if( !m_recordset.RecordsCount() )
	{
		m_command.Format( L"INSERT INTO filespec ( type, filespec, created, modified, loaded ) VALUES ( '%d', '%s', '%s', '%s', '%s' ) ", GA_HTML, fileSpec, mtime, ctime, presentDate );
		if( !theApp.execute( m_command ) ) return false;
		m_command = L"SELECT last_insert_rowid() FROM filespec";	
		if( !query( m_command ) ) return false;
	}


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
	return true;
}
*/
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
	
	int z;
	if( m_lineNumber == 190365 )
		z = 1;

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
	d.titolo		= m_tableHeader.titolo;
	d.rowid			= insertAny( &d );
	m_rowidLastDescendant = d.rowid;
	return( d.rowid );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::insertDescendantSpouse( UINT i )
{
	CString rowid(L"0");

	if( !v_marriages.at(i).lastNameM.IsEmpty() || !v_marriages.at(i).firstNameM )				// elõfordulhat ilyem: = 1848.03.15  semmi más

	s.source		= 2;
	s.generation	= ' ';
	s.sex_id		= v_marriages.at(i).sex_id;
	s.title			= v_marriages.at(i).title;
	s.titolo		= v_marriages.at(i).titolo;
	s.first_name	= v_marriages.at(i).first_name;
	s.last_name		= v_marriages.at(i).last_name;
	s.posterior		= v_marriages.at(i).posterior;
	s.birth_place	= v_marriages.at(i).birth_place;
	s.birth_date	= v_marriages.at(i).birth_date;
	s.death_place	= v_marriages.at(i).death_place;
	s.death_date	= v_marriages.at(i).death_date;
	s.comment		= v_marriages.at(i).comment;
	s.father_id		= v_marriages.at(i).rowidF;
	s.mother_id		= v_marriages.at(i).rowidM;
	s.parentIndex	= 1;
	s.parentIndexCalc = 1;
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

	if( !v_marriages.at(i).lastNameF.IsEmpty() || !v_marriages.at(i).firstNameF )				// az apa csak utónevével szerepel
	{
		sf.source			= 3;
		sf.generation		= ' ';
		sf.sex_id			= v_marriages.at(i).sex_idF;
		sf.sex_id			= 1;
		sf.title			= v_marriages.at(i).titleF;
		sf.titolo			= v_marriages.at(i).titoloF;
		sf.last_name		= v_marriages.at(i).lastNameF;
		sf.first_name		= v_marriages.at(i).firstNameF;
		sf.posterior		= v_marriages.at(i).posteriorF;
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
		sf.parentIndex		= 1;
		sf.parentIndexCalc = 1;
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
	if( !v_marriages.at(i).lastNameM.IsEmpty() || !v_marriages.at(i).firstNameM )				// az apa csak utónevével szerepel
//	if( !v_marriages.at(i).mother.IsEmpty() )				// az apa csak utónevével szerepel
	{
		sm.source			= 4;
		sm.generation		= ' ';
		sm.sex_id			= v_marriages.at(i).sex_idM;
		sm.sex_id			= 2;
		sm.title			= v_marriages.at(i).titleM;
		sm.titolo			= v_marriages.at(i).titoloM;
		sm.last_name		= v_marriages.at(i).lastNameM;
		sm.first_name		= v_marriages.at(i).firstNameM;
		sm.posterior		= v_marriages.at(i).posteriorM;
		sm.birth_place		= v_marriages.at(i).birthPlaceM;
		sm.birth_date		= v_marriages.at(i).birthDateM;
		sm.death_place		= v_marriages.at(i).deathPlaceM;
		sm.death_date		= v_marriages.at(i).deathDateM;
		sm.comment			= v_marriages.at(i).commentM;
		sm.father_id		= L"0";
		sm.mother_id		= L"0";
		sm.parentIndex		= 1;
		sm.parentIndexCalc = 1;

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
	ss.posterior		= v_spouseSpouses.at(i).posterior;
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
	ss.parentIndex		= 1;
	ss.parentIndexCalc = 1;

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
	int numOfMarriages = v_marriages.size();

	last_name = p->last_name;
	if( (pos = p->last_name.Find( L"_(" ) ) != -1 )
	{
		last_name	= p->last_name.Left( pos );
		other_names	= p->last_name.Mid( pos+2);
		other_names.Replace( ')', ' ' );
		other_names.TrimRight();
	}

	CString values;
	values.Format( L"\
'%d','%d','%d','%d','%d','%d','%d','%d','%d',\
'%c','%d','%s','%s','%s','%s','%s','%s','%s', '%s',\
'%s','%s','%s','%s','%s','%d','%d','%s','%d',\
'%s','%s','%d','%d', '%s', '%d', '%d'",
m_fileNumber, m_tableHeader.familyNumber,v_tableHeader.size(),m_lineNumber,p->source,1,0,0,0,\
p->generation,p->sex_id,p->title,p->titolo,p->first_name.Trim(),last_name, p->posterior, other_names,p->birth_place,p->birth_date,\
p->death_place,p->death_date,p->comment,p->father_id,p->mother_id,p->parentIndex,p->parentIndexCalc,p->folyt,p->tableAncestry,
p->tableRoman,p->arm,p->orderFather,p->orderMother,p->csalad,p->gap, numOfMarriages );


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
// Leszármazott házassága ( 1 típusú )
int CGaInput::insertDescMarriage( UINT i )
{
	CString orderHusband(L"");
	CString orderWife(L"");
	CString spouse1_id;
	CString spouse2_id;
	
	int sex_id1 = d.sex_id;
	int sex_id2 = v_marriages.at(i).sex_id;

	if( d.rowid==L"0" || v_marriages.at(i).rowid == L"0" ) return 0;

//	if( v_spouseSpouses.size() == 0 ) 


cont: if( d.sex_id == MAN )
	{
		spouse1_id = d.rowid;
		spouse1_id = v_marriages.at(i).spouse_id;   // uj: minden házstárshoz a leszármazott más bejegyzése tartozik
		spouse2_id = v_marriages.at(i).rowid;
		orderWife.Format( L"%d", i+1 );				// vMarriages vektorban vannak a házastársak, sorrendben
		orderHusband.Format( L"%d", v_marriages.at(i).orderSpouse );
	}
	else
	{
		spouse1_id = v_marriages.at(i).rowid;
		spouse2_id = d.rowid;
		spouse2_id = v_marriages.at(i).spouse_id;	// uj: minden házstárshoz a leszármazott más bejegyzése tartozik
		orderHusband.Format( L"%d", i+1 );
		orderWife.Format(L"%d",v_marriages.at(i).orderSpouse );
	}
	insertMarriage( spouse1_id, spouse2_id, sex_id1, sex_id2, orderHusband, orderWife, v_marriages.at(i).place, v_marriages.at(i).date, 1 );
	return 1;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Házastárs további házassága ( 2 típusú )
// V László .... =Bp 1921.06.06 Nagyabafalvi és Felsõlehotai Abaffy Erzsébet *Arad 1900.02.16 +L 1972.09.14 (Emil-Urbán Mária, 2f. Újházy László???????)
int CGaInput::insertSpouseSpousesMarriage( UINT i )
{
	CString rowidS = v_marriages.at( v_spouseSpouses.at(i).spouseIndex ).rowid;
	CString rowidSS = v_spouseSpouses.at(i).rowid;
	CString spouse1_id;
	CString spouse2_id;
	int sex_id1 = s.sex_id;
	int sex_id2 = v_spouseSpouses.at(i).sex_id;


	if( rowidS==L"0" || rowidSS == L"0" ) return 0;

	CString orderWife(L"");
	CString orderHusband(L"");

	if( sex_id2 == MAN )    
	{
		orderHusband.Format( L"%d", v_spouseSpouses.at(i).order );
//		orderWife.Format( L"%d", v_marriages.at(v_spouseSpouses.at(i).spouseIndex ).orderSpouse ); // új
		spouse1_id = rowidSS;
		spouse2_id = rowidS;
	}
	else
	{
		orderWife.Format( L"%d", v_spouseSpouses.at(i).order );
//		orderHusband.Format( L"%d", v_marriages.at(v_spouseSpouses.at(i).spouseIndex ).orderSpouse ); //új
		spouse1_id = rowidS;
		spouse2_id = rowidSS;
	}

	
	insertMarriage( spouse1_id, spouse2_id, sex_id1, sex_id2, orderHusband, orderWife, L"", L"", 2 );
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// házastárs szüleinak házassága ( 3 típusú )
int CGaInput::insertSpouseParentsMarriage( UINT i )
{
	CString rowidF = v_marriages.at(i).rowidF;
	CString rowidM = v_marriages.at(i).rowidM;

	int sex_id1 = v_marriages.at(i).sex_idF;
	int sex_id2 = v_marriages.at(i).sex_idM;


	if( rowidF==L"0" || rowidM == L"0" ) return 0;
	insertMarriage( rowidF, rowidM, sex_id1, sex_id2, L"", L"", L"", L"", 3 );
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


