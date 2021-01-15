#include "stdafx.h"
#include "Dragon.h"
#include "Ga_descendants.h"
#include "GA_structures.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ix a kiirandó leszármazott indexe a vDesc vektorban
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::printGAline( UINT ix )
{
//	fwprintf( fl, L"%d %d %d %d\n", vDesc.at(ix).childrenPrinted, vDesc.at(ix).gen, vDesc.at(ix).numOfChildren, vDesc.size() );

	CString rowid = vDesc.at( vDesc.size()-1).rowid;
	queryPeople( rowid, &p );

	printBegining( ix );	// html kódok és generáció elkészítése; 
	printDescendant( ix );
	printMarriages(); 


	if( !p.arm.IsEmpty() )
	{
		str.Format( L" <font color='blue'>[%s család]</font>", p.arm );
		str.Format( L" <font color='blue'>[%s]</font>", p.arm );
		print( str );
	}

	CString folyt = recP.GetFieldString( PEOPLE_FOLYT );
	if( !p.folyt.IsEmpty() )
	{
		str.Format( L"<font color='blue'> %c%c%c folyt %s</font>", '%','%','%', p.folyt );
		print( str );
	}

//	if( !p.known_as.IsEmpty() ) fwprintf( fl, L"\n%c %s", '%', p.known_as );
	fwprintf( fl, L"\n" );
	fflush( fl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A genráció változástól függû behuzás és genrációs kód nyomtatása az m_sytax értékétõl függõen
void CGaDescendants::printBegining( int ix )
{
	CString tags;

	UINT	generation	= vDesc.at(ix).gen;
	TCHAR	gen			= TCHAR('A') + TCHAR(generation);	// a generációs karakter-jel ( A,B,C,D.....);

	// a generációnak megfelelõ sor-kihúzás, visszatolás
	if( generation > m_genPrev || m_genPrev == 0 )
	{
		tags.Format( L"%s<li>", m_tag1 );
		++cnt_ol;
	}
	else if( generation == m_genPrev )
		tags = L"<li>";
	else if( generation < m_genPrev )						// régi generáció, kijebb hozza a generáció-különbség-szeresen
	{
		for( UINT i = 0; i < (m_genPrev - generation); ++i )
		{
			fwprintf( fl, L"%s\n", m_tag2 );
			--cnt_ol;
		}
		tags = L"<li>";
	}
	m_genPrev = generation;
	


	if( m_numbering == SZLUHA )
		str.Format( L"%s%c&diams;", tags, gen );			// gedcom és kézi bevitelnél nincs generáció, ezt úgy kell beletenni!!
	else if( m_numbering == VIL )
		str.Format( L"%s%c%d&diams;", tags, gen, vDesc.at(ix).children );
	else if( m_numbering == TUP )
		str.Format( L"%s%c-%d&diams;", tags, gen, vSerial.at( generation ) );
	print( str );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::printDescendant( int ix )
{
	CString rowid = vDesc.at( vDesc.size()-1).rowid;

// leszármazott neve
	if( m_CheckLastName )
		cLine.Format( L"%s %s",  getLastname( &p ), getFirstname( &p ) ); //attrib[m_ixName].code1, p.first_name, attrib[m_ixName].code2 ); 
	else
		cLine.Format( L"%s", getFirstname( &p ) ); //attrib[m_ixName].code1, p.first_name, attrib[m_ixName].code2 ); 
	
// ha apjának több felkesége volt,a felség sorszámának kiírása
	if( vDesc.at(ix).numOfMothers > 1 )
	{
		if( m_numbering == SZLUHA || m_numbering == TUP )
		{
			if( !p.mother_index.IsEmpty() && p.mother_index.Compare( L"0" ) )
			{
				str.Format( L"/%s", p.mother_index ); 
				cLine += str;
			}
		}
		else if( m_numbering == VIL )  // felség sorszámának kiírása a név elõtt
		{
			if( !p.mother_index2.IsEmpty() && p.mother_index2.Compare( L"0" ) )
				fwprintf( fl, L"%s. ", p.mother_index2 );
		}
	}
	// nem értem, hogy hogy az alábbi if-nek mi az értelme. Ezuért most kikapcsoltam 2020.10.04
/*	
	if( m_numbering == SZLUHA || m_numbering == TUP )
	{
		str = getPlaceDateBlock( p.birth_place, p.birth_date, '*' );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );

		str = getPlaceDateBlock( p.death_place, p.death_date, '+' );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );

		str = getCommentBlock( p.comment, p.occupation );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );
	}
	else if( m_numbering == VIL )
*/
	{
		cLine += getPlaceDateBlock( p.birth_place, p.birth_date, '*' );
		cLine += getPlaceDateBlock( p.death_place, p.death_date, '+' );
		cLine += getCommentBlock( p.comment, p.occupation );
	}
	cLine.Trim();

	if( !p.known_as.IsEmpty() )
	{
		cLine += L" % ";
		cLine += p.known_as;
	}


	CString csalad;
	if( !p.csalad.IsEmpty() )
	{
		cLine += L" [";
		cLine += p.csalad;
		cLine += L" család]";
	}

//	if( m_code == UTF8 ) cLine =  UnicodeToUtf8( cLine );
//	fwprintf( fl, L"%s", cLine );
	print( cLine );
	fflush( fl );
	m_genPrev = vDesc.at(ix).gen;
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::printMarriages()
{
	CString spouse_id;
	CString place;
	CString date;
	CString marriage;
	CString spouse;
	int		numberOfSpouses;

	if( p.sex_id ==  MANS )
		m_command.Format( L"SELECT place, date, spouse2_id FROM marriages WHERE spouse1_id='%s' ORDER BY orderWife", p.rowid );
	else
		m_command.Format( L"SELECT place, date, spouse1_id FROM marriages WHERE spouse2_id='%s' ORDER BY orderHusband", p.rowid );
	if( !query2( m_command ) ) return;

	numberOfSpouses = m_recordset2.RecordsCount();

	if( !numberOfSpouses ) return;					// nincs házastárs


	for( int i = 0; i < numberOfSpouses; ++i, m_recordset2.MoveNext() )
	{
		place	= m_recordset2.GetFieldString( 0 );
		date	= m_recordset2.GetFieldString( 1 );
		spouse_id = m_recordset2.GetFieldString( 2 );
		if( !spouse_id.IsEmpty() && spouse_id.Compare(L"0")  )
		{
			queryPeople( spouse_id, &s );   // házastárs adatainak beolvasása
			printMarriage( place, date, i, numberOfSpouses );
			printSpouse( );
			printSpRelatives();
		}
	}
	fflush( fl );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::printMarriage( CString place, CString date, int i, int numberOfSpouses )
{
	CString marriage;

//	if( m_unordered == UNORDERED )
//		fwprintf( fl, L";" );

	marriage = attrib[m_ixSpec].code1;
	if( numberOfSpouses > 1  )
	{
		str.Format( L" %d=", i+1 );
		marriage += str;
	}
	else
		marriage += L" =";
	marriage += attrib[m_ixSpec].code2;


	if( !place.IsEmpty() )
	{
		marriage += place;
		marriage += L" ";
	}
	if( !date.IsEmpty() )
	{
		marriage += date;
		marriage += L" ";
	}
//	if( m_code == UTF8 ) marriage =  UnicodeToUtf8( marriage );
//	fwprintf( fl, L"%s", marriage );
	print( marriage );
	fflush( fl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a házastárs személyleíró blokkját elkészíti, a neveket bold-dal kiemnelve
void CGaDescendants::printSpouse()
{
	CString spouse;

	spouse = getFullname( &s );
	spouse += getPlaceDateBlock( s.birth_place, s.birth_date, '*' );
	spouse += getPlaceDateBlock( s.death_place, s.death_date, '+' );
	spouse += getCommentBlock( s.comment, s.occupation );

//	if( m_code == UTF8 ) spouse =  UnicodeToUtf8( spouse );
//	fwprintf( fl, L"%s", spouse );
	print( spouse );
	fflush( fl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a házastárs személyleíró blokkját elkészíti, a neveket bold-dal kiemnelve
void CGaDescendants::printSpRelatives()
{
	CString father(L"");
	CString mother(L"");
	CString parents;
	int		numOfSpouses;
	CString spouse_id;
	CString order;
	CString spouseSpouse;


	if( !s.father_id.IsEmpty() && s.father_id != L"0" )
	{
		queryPeople( s.father_id, &sf );
		father = getFirstname( &sf );
	}
	if( !s.mother_id.IsEmpty() && s.mother_id != L"0" )
	{
		queryPeople( s.mother_id, &sm );
		mother = getFullname( &sm );
		mother = getLastFirst( &sm );
	}
//	if( father.IsEmpty() && mother.IsEmpty() ) return;   // nincs se apa, se anya
	
	if( !father.IsEmpty() && !mother.IsEmpty() )		// ( apa és anya is meg van adva )
	{
		parents.Format( L" (%s-%s", father, mother );
	}

	if( !father.IsEmpty() && mother.IsEmpty() )
	{
		if( s.sex_id == MANS )
			parents.Format( L" (%s fia", father );
		else
			parents.Format( L" (%s lánya", father );
	}

	if( father.IsEmpty() && !mother.IsEmpty() )
	{
		if( s.sex_id == MANS )
			parents.Format( L" (%s fia", mother );
		else
			parents.Format( L" (%s lánya", mother );
	}


	if( s.sex_id == MANS )
		m_command.Format( L"SELECT spouse2_id, orderWife  FROM marriages WHERE spouse1_id='%s'", s.rowid ); // a házastárs házastársai
	else
		m_command.Format( L"SELECT spouse1_id, orderHusband FROM marriages WHERE spouse2_id='%s'", s.rowid ); // a házastárs házastársai
	if( !query4( m_command ) ) return;
	numOfSpouses = m_recordset4.RecordsCount();
	if( numOfSpouses > 1 )
	{
		for( int i = 0; i < numOfSpouses; ++i, m_recordset4.MoveNext() )
		{
			spouse_id	= m_recordset4.GetFieldString( 0 );
			order		= m_recordset4.GetFieldString( 1 );
			queryPeople( spouse_id, &ss );
			if( ss.rowid != p.rowid )		// a GA sorban szereplõ házastársat kihagyja
			{
				spouseSpouse = getLastFirst( &ss );
				if( parents.GetLength() )
					parents += L", ";
				else
					parents = L"(";

				parents += L" ";
				parents += order,
				parents += L"f. ";
				parents += spouseSpouse; 
			}
		}
	}
	if( !parents.IsEmpty() ) parents += L")";
//	if( m_code == UTF8 ) parents =  UnicodeToUtf8( parents );
//	fwprintf( fl, L"%s", parents );
	print( parents );
	fflush( fl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getLastname( PPEOPLE* p )
{
	CString lastname = attrib[m_ixName].code1;
	lastname += p->last_name;
	lastname += attrib[m_ixName].code2;
	return lastname;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getFirstname( PPEOPLE* p )
{
	CString firstname(L"");

	if( !p->first_name.IsEmpty() )
	{
		firstname = attrib[m_ixName].code1;
		firstname += p->first_name;
		firstname += attrib[m_ixName].code2;
	}
	return firstname;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getLastFirst( PPEOPLE* p )
{
	CString name;
	if( !p->first_name.IsEmpty() && !p->last_name.IsEmpty() )
	{
		name = attrib[m_ixName].code1;
		name += p->last_name;
		name += L" ";
		name += p->first_name;
		name += attrib[m_ixName].code2;
	}
	return name;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getFullname( PPEOPLE* p )
{
	CString fullname  = attrib[m_ixName].code1;

	if( !p->title.IsEmpty() )
	{
		fullname += p->title;
		fullname += L" ";
	}
	if( !p->titolo.IsEmpty() )
	{
		fullname += p->titolo;
		fullname += L" ";
	}
	if( !p->last_name.IsEmpty() )
	{
		fullname += p->last_name;
		fullname += L" ";
	}
	if( !p->other_names.IsEmpty() )
	{
		fullname += L"_(";
		fullname += p->other_names;
		fullname += L") ";
	}
	if( !p->first_name.IsEmpty() )
	{
		fullname += p->first_name;
	}		
	fullname.Trim();
	fullname += attrib[m_ixName].code2;
	return( fullname.Trim() );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getPlaceDateBlock( CString place, CString date, TCHAR jel )
{
	
	CString block(L"");

	if( !place.IsEmpty() || !date.IsEmpty() )
	{
		block.Format( L"%s %c%s", attrib[ m_ixSpec ].code1, jel, attrib[m_ixSpec].code2 );
		if( !place.IsEmpty() )
		{
			block += place;
			block += L" ";
		}
		if( !date.IsEmpty() )
		{
			block += date;
		}
		block.TrimRight();
		
	}
	return block;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getCommentBlock( CString comment, CString occupation  )
{
	CString block = attrib[m_ixComment].code1;
	CString str;

	str = occupation;
	if( !occupation.IsEmpty() && !comment.IsEmpty() )
	{
		str += L", ";
	}
	str += comment;
	block += L" ";
	block += str;
	block.Trim();


/*	
	if( !occupation.IsEmpty() )
	{
		if( !comment.IsEmpty() )
		{
			comment += L", ";
			comment += occupation;
		}
		else
		{
			comment = occupation;
		}
	}

	if( !comment.IsEmpty() )
	{
		block += L" ";
		block += comment;
	}
*/
	block += attrib[m_ixComment].code2;
	return block;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::queryPeople( CString rowid, PPEOPLE* p )
{
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'", rowid );
	if( !query( m_command ) ) return;

	p->arm				= m_recordset.GetFieldString( PEOPLE_ARM );
	p->birth_date		= m_recordset.GetFieldString( PEOPLE_BIRTH_DATE );
	p->birth_place		= m_recordset.GetFieldString( PEOPLE_BIRTH_PLACE );
	p->comment			= m_recordset.GetFieldString( PEOPLE_COMMENT );
	p->csalad			= m_recordset.GetFieldString( PEOPLE_CSALAD );
	p->death_date		= m_recordset.GetFieldString( PEOPLE_DEATH_DATE );
	p->death_place		= m_recordset.GetFieldString( PEOPLE_DEATH_PLACE );
	p->father_id		= m_recordset.GetFieldString( PEOPLE_FATHER_ID );
	p->first_name		= m_recordset.GetFieldString( PEOPLE_FIRST_NAME );
	p->folyt			= m_recordset.GetFieldString( PEOPLE_FOLYT );
	p->gap				= m_recordset.GetFieldString( PEOPLE_GAP );
	p->generation		= m_recordset.GetFieldString( PEOPLE_GENERATION );
	p->known_as			= m_recordset.GetFieldString( PEOPLE_KNOWN_AS );
	p->last_name		= m_recordset.GetFieldString( PEOPLE_LAST_NAME );
	p->lineNumber		= m_recordset.GetFieldString( PEOPLE_LINENUMBER );
	p->mother_id		= m_recordset.GetFieldString( PEOPLE_MOTHER_ID );
	p->mother_index		= m_recordset.GetFieldString( PEOPLE_MOTHER_INDEX );
	p->mother_index2	= m_recordset.GetFieldString( PEOPLE_MOTHER_INDEX2 );
	p->tableAncestry	= m_recordset.GetFieldString( PEOPLE_TABLEANCESTRY );
	p->occupation		= m_recordset.GetFieldString( PEOPLE_OCCUPATION );
	p->orderFather		= m_recordset.GetFieldString( PEOPLE_ORDERFATHER);
	p->orderMother		= m_recordset.GetFieldString( PEOPLE_ORDERMOTHER );
	p->other_names		= m_recordset.GetFieldString( PEOPLE_OTHER_NAMES );
	p->rowid			= m_recordset.GetFieldString( PEOPLE_ROWID );
	p->sex_id			= m_recordset.GetFieldString( PEOPLE_SEX_ID );
	p->source			= m_recordset.GetFieldString( PEOPLE_SOURCE );
	p->tableNumber		= m_recordset.GetFieldString( PEOPLE_TABLENUMBER );
	p->tableRoman	= m_recordset.GetFieldString( PEOPLE_TABLEROMAN );
	p->title			= m_recordset.GetFieldString( PEOPLE_TITLE );
	p->titolo			= m_recordset.GetFieldString( PEOPLE_TITOLO );

	int pos;
	if( ( p->comment.Find( L"http" ) ) != -1 ) p->comment.Empty();

}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaDescendants::getTupigny( UINT gen )
{
	for( UINT i = 0; i < v_tupigny.size(); ++i )
	{
		if( v_tupigny.at(i).gen == gen )
			return v_tupigny.at(i).tupigny;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HA a gen mér létezik, akkor 1-el növeli, ha nem, akkor 1-et tesz bele;
int CGaDescendants::putTupigny( UINT gen )
{
	TUPIGNY tupigny;
	for( UINT i = 0; i < v_tupigny.size(); ++i )
	{
		if( v_tupigny.at(i).gen == gen )
		{
			++v_tupigny.at(i).tupigny;
			return v_tupigny.at(i).tupigny;
		}
	}
	tupigny.gen			= gen;
	tupigny.tupigny		= 1;
	v_tupigny.push_back( tupigny );

	return v_tupigny.at( v_tupigny.size() - 1 ).tupigny;
}
*/
