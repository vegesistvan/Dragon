#include "stdafx.h"
#include "Dragon.h"
#include "inputGA.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "utilities_dragon.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// I N S E R T   T A B L E H E A D E R   A N D   B R A N C H E S ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CInputGA::insertTableHeader()
{
	CString tableHeader;
	CString values;
	CString folyt;
	CString csalad;
	if( !theApp.execute( L"BEGIN" ) ) return;
	for( UINT i = 0; i < v_tableHeader.size(); ++i )
	{
		folyt = v_tableHeader.at(i).folyt;
		folyt = folyt.Left( folyt.GetLength() -1 );

		csalad = v_tableHeader.at(i).csalad;
		csalad = csalad.Left( csalad.GetLength() -1 );

		tableHeader = v_tableHeader.at(i).tableHeader;

		values.Format( L"'%d','%d','%d','%s','%s','%s','%s','%s','%s','%s','%s','%s','%d','%s','%s','%s','%s'",
		m_fileNumber,
		v_tableHeader.at(i).familyNumber,
		v_tableHeader.at(i).lineNumber,
		tableHeader,	
		v_tableHeader.at(i).percent,
		v_tableHeader.at(i).familyName,
		v_tableHeader.at(i).alias,
		v_tableHeader.at(i).titolo,
		v_tableHeader.at(i).tableRoman,
		v_tableHeader.at(i).comment,
		v_tableHeader.at(i).torzs,
		v_tableHeader.at(i).arm,
		v_tableHeader.at(i).joint,
		folyt,
		csalad,
		v_tableHeader.at(i).peer,
		v_tableHeader.at(i).ancestor
		);
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
bool CInputGA::insertEntries()
{

// emberek insertáláa
	
	int z;
	if( m_lineNumber == 114309 )
		z = 1;

	
	CString ret = insertDescendant();					// a v_brunch-hoz meg kell adni a rowid-jét!!
	if (ret == L"0") return false;


	for( UINT i = 0; i < v_marriages.size(); ++i )
	{
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
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::insertDescendant( )
{
	d.source		= 1;
	if (d.last_name.IsEmpty()) d.last_name = m_familyName;
	d.numOfSpouses = v_marriages.size();
	d.spouseIndex = 0;
	d.rowid			= insertAny( &d );
	m_rowidLastDescendant = d.rowid;

	if (m_tableAncestry)
	{
		m_tableHeader.ancestor = m_rowidLastDescendant;
		m_tableHeader.ancestor = d.rowid;
		m_tableAncestry = false;
	}

	return( d.rowid );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::insertDescendantSpouse( UINT i )
{
	CString rowid(L"0");

//	if( !v_marriages.at(i).lastNameM.IsEmpty() || !v_marriages.at(i).firstNameM ) 				// elõfordulhat ilyem: = 1848.03.15  semmi más

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
	s.father_id		= v_marriages.at(i).rowidF;  // csak ha a házastárs apja,anyja is meg van adva a sorban!!! Másképp nem tudjuk!!
	s.mother_id		= v_marriages.at(i).rowidM;
	s.parentIndex	= 1;
	s.folyt			= L"";
	s.tableAncestry	= 0;
	s.lineNumber	= m_lineNumber;
	s.arm			= v_marriages.at(i).arm;
	s.peer			= v_marriages.at(i).peer;
	s.spouseIndex	= i + 1;
	
	int cnt = 1;
	for (UINT j = 0; j < v_spouseSpouses.size(); ++j)
	{
		if (v_spouseSpouses.at(j).spouseIndex == i)
			++cnt;
	}
	s.numOfSpouses = cnt;

	s.csalad.Empty();
	s.gap			= 0;
	rowid = insertAny( &s );

	v_marriages.at(i).spouse_id = rowid;
	if (d.sex_id == MAN)
	{
		v_marriages.at(i).rowidHusband = d.rowid;
		v_marriages.at(i).rowidWife = rowid;
	}
	else
	{
		v_marriages.at(i).rowidWife = d.rowid;
		v_marriages.at(i).rowidHusband = rowid;
	}
	return ( rowid );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::insertSpouseF( UINT i )
{
	CString rowidF(L"0");

	if( !v_marriages.at(i).lastNameF.IsEmpty() || !v_marriages.at(i).firstNameF )				// az apa csak utónevével szerepel
	{
		sf.source			= 3;
		sf.generation		= ' ';
		sf.sex_id			= v_marriages.at(i).sex_idF;
		sf.sex_id			= MAN;
		sf.peer				= v_marriages.at(i).peerF;
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
		sf.numOfSpouses		= 1;
		s.gap				= 0;
		s.spouseIndex		= 0;

		rowidF =  insertAny( &sf );
	}
	
	
	v_marriages.at(i).rowidF = rowidF;
	return ( rowidF );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::insertSpouseM( UINT i )
{
	CString rowidM(L"0");
	if( !v_marriages.at(i).lastNameM.IsEmpty() || !v_marriages.at(i).firstNameM )				// az apa csak utónevével szerepel
//	if( !v_marriages.at(i).mother.IsEmpty() )				// az apa csak utónevével szerepel
	{
		sm.source			= 4;
		sm.source			= 3;
		sm.generation		= ' ';
		sm.sex_id			= v_marriages.at(i).sex_idM;
		sm.sex_id			= WOMAN;
		sm.peer				= v_marriages.at(i).peerM;
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
		sm.numOfSpouses = 1;

		sm.tableAncestry	= 0;
		sm.lineNumber		= m_lineNumber;
		sm.arm.Empty();	
		sm.csalad.Empty();
		sm.gap				= 0;
		sm.spouseIndex		= 0;
		rowidM = insertAny( &sm );
	}
	v_marriages.at(i).rowidM = rowidM;
	return( rowidM );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a házastárs további házastársai
CString CInputGA::insertSpouseS(UINT i)
{
	CString  rowid(L"0");

	ss.source = 5;
	ss.source = 4;
	ss.generation = ' ';
	ss.sex_id = v_spouseSpouses.at(i).sex_id;
	ss.peer = v_spouseSpouses.at(i).peer;
	ss.title = v_spouseSpouses.at(i).title;
	ss.titolo = v_spouseSpouses.at(i).titolo;
	ss.last_name = v_spouseSpouses.at(i).last_name;
	ss.first_name = v_spouseSpouses.at(i).first_name;
	ss.posterior = v_spouseSpouses.at(i).posterior;
	ss.birth_place = v_spouseSpouses.at(i).birth_place;
	ss.birth_date = v_spouseSpouses.at(i).birth_date;
	ss.death_place = v_spouseSpouses.at(i).death_place;
	ss.death_date = v_spouseSpouses.at(i).death_date;
	ss.comment = v_spouseSpouses.at(i).comment;
	ss.tableAncestry = 0;
	ss.lineNumber = m_lineNumber;
	ss.father_id = L"0";
	ss.mother_id = L"0";
	ss.arm.Empty();
	ss.csalad.Empty();
	ss.gap = 0;
	ss.parentIndex = 1;
	ss.numOfSpouses = 1;
	ss.spouseIndex = 0;
	rowid = insertAny(&ss);
	if (ss.sex_id == MAN)
	{
		v_spouseSpouses.at(i).husband_id = rowid;
		v_spouseSpouses.at(i).wife_id = v_marriages.at(v_spouseSpouses.at(i).spouseIndex).spouse_id;
	}
	else
	{
		v_spouseSpouses.at(i).husband_id = v_marriages.at(v_spouseSpouses.at(i).spouseIndex).spouse_id;
		v_spouseSpouses.at(i).wife_id = rowid;
	}

	v_spouseSpouses.at(i).rowid	= rowid;
	return( rowid );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::insertAny( PEOPLE* p )
{
	p->comment.TrimLeft();
	CString last_name(L"");
	CString other_names(L"");
	int pos;
	last_name = p->last_name;

	if (p->father_id == L"0") p->father_id.Empty();
	if (p->mother_id == L"0") p->mother_id.Empty();
	if (isdigit( p->posterior[0])) // azért van itt, mert a birthSubsr vagy a death Substr elronthatná	
	{
		p->comment = p->posterior;
		p->posterior.Empty();
	}

	if( (pos = p->last_name.Find( L"_(" ) ) != -1 )
	{
		last_name	= p->last_name.Left( pos );
		other_names	= p->last_name.Mid( pos+2);
		other_names.Replace( ')', ' ' );
		other_names.Trim();
	}
	last_name = convertNameToLower(last_name);  // ha netán (GEDCOM) nagybetûs lenne

	CString values;

	int tableNumber = v_tableHeader.size();

	p->whichWife = p->parentIndex;
	m_fieldsP = L"\
fileNumber,\
familyNumber,\
tableNumber,\
lineNumber,\
source,\
united,\
generation,\
sex_id,\
peer,\
title,\
titolo,\
first_name,\
last_name,\
posterior,\
other_names,\
birth_place,\
birth_date,\
death_place,\
death_date,\
comment,\
tableAncestry,\
tableRoman,\
arm,\
sources,\
occupation,\
csalad,\
gap,\
folyt,\
father_id,\
mother_id,\
parentIndex,\
whichHusband,\
whichWife,\
whichChildFather, \
whichChildMother, \
numOfSpouses,\
numOfChildren,\
spouseIndex\
";

	int z;

	p->numOfChildren = 0;

	values.Format( L"\
'%d','%d','%d','%d','%d','%d',\
'%c','%s','%s','%s','%s','%s','%s','%s','%s',\
'%s','%s','%s','%s',\
'%s','%d','%s','%s','%d','%s','%s','%d','%s',\
'%s','%s','%d','%d','%d','%d','%d',\
'%d','%d','%d'",
m_fileNumber, m_tableHeader.familyNumber,m_tableNumber1,m_lineNumber,p->source,1,\
p->generation,p->sex_id,p->peer,p->title,p->titolo,p->first_name.Trim(),last_name, p->posterior, other_names,\
p->birth_place,p->birth_date,p->death_place,p->death_date,\
p->comment, p->tableAncestry,p->tableRoman, p->arm, p->source,p->occupation, p->csalad, p->gap, p->folyt,
p->father_id,p->mother_id,p->parentIndex,p->whichHusband, p->whichWife,p->whichChildFather,p->whichChildMother,
p->numOfSpouses,p->numOfChildren, p->spouseIndex );


    m_command.Format( L"INSERT INTO people (%s) VALUES (%s)", m_fieldsP, values );
	if( !theApp.execute( m_command ) ) exit(0);

	++theApp.m_cntPeople;
	++m_rowid;	// az utoljára insertált ember azonosítója. Azért számoljuk és nem visszakérdezzük a SELECT 'last_insert_rowid'-val
				// mert ez magszakítaná a BEGIN_COMMIT tranzakciót és nagyon lelassulna a beolvasás!!!
	p->rowid.Format( L"%d", m_rowid );
	return( p->rowid );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// I N S E R T  M A R R I A G E S  ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Leszármazott házassága ( 1 típusú )
int CInputGA::insertDescMarriage( UINT i )
{
	CString whichHusband	= v_marriages.at(i).whichHusband; 
	CString whichWife		= v_marriages.at(i).whichWife; 
	CString rowid_husband	= v_marriages.at(i).rowidHusband;    
	CString rowid_wife		= v_marriages.at(i).rowidWife;		
	if (rowid_husband == L"0" || rowid_wife == L"0") return 0;

	insertMarriage( rowid_husband, rowid_wife, whichHusband, whichWife, v_marriages.at(i).place, v_marriages.at(i).date, 1 );
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Házastárs további házassága ( 2 típusú )
// V László .... =Bp 1921.06.06 Nagyabafalvi és Felsõlehotai Abaffy Erzsébet *Arad 1900.02.16 +L 1972.09.14 (Emil-Urbán Mária, 2f. Újházy László???????)
int CInputGA::insertSpouseSpousesMarriage( UINT i )
{
	CString whichWife(L"");
	CString whichHusband(L"");
	CString husband_id = v_spouseSpouses.at(i).husband_id;
	CString wife_id = v_spouseSpouses.at(i).wife_id;
	if (husband_id == L"0" || wife_id == L"0") return 0;


	CString sex_id = v_spouseSpouses.at(i).sex_id;


	if (sex_id == MAN)
		whichHusband.Format(L"%d", v_spouseSpouses.at(i).order);
	else
		whichWife.Format(L"%d", v_spouseSpouses.at(i).order);

	insertMarriage( husband_id, wife_id, whichHusband, whichWife, L"", L"", 2 );
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// házastárs szüleinak házassága ( 3 típusú )
int CInputGA::insertSpouseParentsMarriage( UINT i )
{
	CString rowidF = v_marriages.at(i).rowidF;
	CString rowidM = v_marriages.at(i).rowidM;
	
	if( rowidF==L"0" || rowidM == L"0" ) return 0;
	
	insertMarriage( rowidF, rowidM, L"", L"", L"", L"", 3 );
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// eg házasság bejegyzése
int CInputGA::insertMarriage( CString husband_id, CString wife_id, CString whichHusband, CString whichWife, CString place, CString date, int source )
{
	CString values;
	values.Format( L"'%d','%d','%s','%s','%s','%s','%s','%s','%d'",
	m_tableNumber,\
	m_lineNumber,\
	husband_id,\
	wife_id,\
	whichHusband,\
	whichWife,\
	place,\
	date,\
	source\
	);
	
	m_command.Format( L"INSERT INTO marriages (%s) VALUES (%s)", m_fieldsM, values );
	if( !theApp.execute( m_command ) ) return 0;
	return( 10 );
}
