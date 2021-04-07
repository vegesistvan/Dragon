#include "stdafx.h"
#include "Dragon.h"
#include "Ga_descendants.h"
#include "GA_structures.h"
#include "utilities.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ix a kiirandó leszármazott indexe a vDesc vektorban
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::printGAline()
{
//	fwprintf( fl, L"%d %d %d %d\n", vDesc.at(ix).childrenPrinted, vDesc.at(ix).gen, vDesc.at(ix).numOfChildren, vDesc.size() );

	CString rowid;
	rowid = vDesc.at( vDesc.size()-1).rowid;
//	rowid = vDesc.at(ix).rowid;
	queryPeople( rowid, &p );

	if( m_spaces )
		printBegining2();	// html kódok és generáció elkészítése; 
	else
		printBegining();	// html kódok és generáció elkészítése; 
	printDescendant();
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
	fflush( fl );

	if( !p.known_as.IsEmpty() )
	{
		if( !m_spaces )
			str.Format( L"<ul><font color='blue'> %c %s</font>", '%', p.known_as );
		else
		{
			tab();
			str.Format( L"<font color='blue'> %c %s</font>", '%', p.known_as );
		}
		print( str );
		fflush( fl );
	}


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A genráció változástól függû behuzás és genrációs kód nyomtatása az m_sytax értékétõl függõen
void CGaDescendants::printBegining()
{
	CString tags;
	bool ul = false;
	int ix = vDesc.size() -1;

	UINT	generation	= vDesc.at(ix).gen;
	m_gen = generation;
	TCHAR	gen			= TCHAR('A') + TCHAR(generation);	// a generációs karakter-jel ( A,B,C,D.....);

	// a generációnak megfelelõ sor-kihúzás, visszatolás
	if( generation > m_genPrev || m_genPrev == 0 )
	{
		tags.Format( L"%s<li>", m_tag1 );
		++cnt_ol;
		ul = true;
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
	

	CString family;
//	CString familyName = getColoredString( p.last_name, m_ixName );


	if( m_checkFamily )
	{
		if( m_familyName != p.last_name )
		{
			family = getFamilyName();			// a tables táblából visszadja a tableHeader értékét 
/*			if( ul )
				str.Format( L"\n\n<p><b>%s %s</b><p>\n\n", L"%%%", family );
			else
				str.Format( L"\n\n<p><b>%s %s</b><p>\n\n", L"%%", family );
*/

			if( ul )
				str.Format( L"\n\n<p>%s %s</p>\n\n", L"%%%", family );
			else
				str.Format( L"\n\n<p>%s %s</p>\n\n", L"%%", family );
			str = getColoredString( str, m_ixFamily );

			print( str );
		}
		m_familyName = p.last_name;
	}

	
	if( m_numbering == SZLUHA )
		str.Format( L"%s%c&diams;", tags, gen );			// gedcom és kézi bevitelnél nincs generáció, ezt úgy kell beletenni!!
	else if( m_numbering == VIL )
		str.Format( L"%s%c%d&diams;", tags, gen, vDesc.at(ix).children );
	else if( m_numbering == TUP )
		str.Format( L"%s%c-%d&diams;", tags, gen, vSerial.at( generation ) );
	print( str );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::tab()
{
	fwprintf( fl, L"<br>%*s", m_gen*4, L" " );
//	fwprintf( fl, L"<br>%*s", m_gen, L"\t" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A genráció változástól függû behuzás és genrációs kód nyomtatása az m_sytax értékétõl függõen
void CGaDescendants::printBegining2()
{
	CString tags;
	bool ul = false;
	int ix = vDesc.size() -1;

	UINT	generation	= vDesc.at(ix).gen;
	m_gen = vDesc.at(ix).gen;

	TCHAR	gen			= TCHAR('A') + TCHAR(generation);	// a generációs karakter-jel ( A,B,C,D.....);

	CString family;

	if( m_checkFamily )
	{
		if( m_familyName != p.last_name )
		{
			family = getFamilyName();			// a tables táblából visszadja a tableHeader értékét 
			fwprintf( fl, L"<br>" );
			tab();
//			str.Format( L"<b>%s %s</b><br>", L"%%%", family );
			str.Format( L"%s %s\n", L"%%%", family );
			str = getColoredString( str, m_ixFamily );
			print( str );
		}
		m_familyName = p.last_name;
	}
	tab();


	if( m_numbering == SZLUHA )
		str.Format( L"%c&diams;",  gen );			// gedcom és kézi bevitelnél nincs generáció, ezt úgy kell beletenni!!
	else if( m_numbering == VIL )
		str.Format( L"%c%d&diams;", gen, vDesc.at(ix).children );
	else if( m_numbering == TUP )
		str.Format( L"%c-%d&diams;", gen, vSerial.at( generation ) );
	print( str );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getFamilyName()
{
	CString titolo;
	CString familyName;

	familyName = p.last_name;
	familyName.MakeUpper();
	titolo = p.titolo;

	if( titolo.IsEmpty() )
		str = familyName;
	else if( !familyName.IsEmpty() )
		str.Format( L"%s, %s", familyName, titolo );
	else
		str.Empty();
	return( str );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::printDescendant()
{

	int ix = vDesc.size() -1;
// leszármazott neve
	if( m_CheckLastName )
		cLine.Format( L"%s %s",  getColoredString( p.last_name, m_ixName ), getColoredString( p.first_name, m_ixName )); 
	else
		cLine.Format( L"%s", getColoredString( p.first_name, m_ixName ));
	cLine.TrimRight();
// ha apjának több felkesége volt,a felség sorszámának kiírása

	if( vDesc.at(ix).numOfMothers > 1 )
	{
		if( p.parentIndex )									// parentindex != 0 csak ha változás van
			m_parentindexLast = p.parentIndex;
		if( m_numbering == SZLUHA || m_numbering == TUP )
		{
			if( p.parentIndex != 0 )
			{
				str.Format( L"/%d", p.parentIndex ); 
				cLine += str;
				m_parentindexLast = p.parentIndex;
			}
		}
		else if( m_numbering == VIL )  // felség sorszámának kiírása a név elõtt
		{
			if( m_parentindexLast )
				fwprintf( fl, L"%d. ", m_parentindexLast );
		}
	}
	if( !p.posterior.IsEmpty() )
	{
		cLine += L" ";
		cLine += p.posterior;
	}


	// nem értem, hogy hogy az alábbi if-nek mi az értelme. Ezuért most kikapcsoltam 2020.10.04
	
	if( m_numbering == SZLUHA || m_numbering == TUP )
	{
		str = getPlaceDateBlock( p.birth_place, p.birth_date, L"*" );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );

		str = getPlaceDateBlock( p.death_place, p.death_date, L"+" );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );

//		str = getCommentBlock( p.comment );
		str = getColoredString( p.comment, m_ixComment );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );
	}
	else if( m_numbering == VIL )
	{
		str = getPlaceDateBlock( p.birth_place, p.birth_date, L"*" );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );

		str = getPlaceDateBlock( p.death_place, p.death_date, L"+" );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );
	}
	cLine.Trim();

	CString csalad;
	if( !p.csalad.IsEmpty() )
	{
		str.Format( L"<font color='blue'>[%s család]</font>", p.csalad );
		cLine += str;
	}
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

	if( numberOfSpouses > 1  )
		str.Format( L" %d=", i+1 );
	else
		str = L" =";

	marriage = getColoredString( str, m_ixSpec );


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
	str = getPlaceDateBlock( s.birth_place, s.birth_date, L"*" );
	if( !str.IsEmpty() )
		spouse.Format( L"%s %s", (CString)spouse, str );

	str = getPlaceDateBlock( s.death_place, s.death_date, L"+" );
	if( !str.IsEmpty() )
		spouse.Format( L"%s %s", (CString)spouse, str );

//	str = getCommentBlock( s.comment );
	str = getColoredString( s.comment, m_ixComment );
	if( !str.IsEmpty() )
		spouse.Format( L"%s %s", (CString)spouse, str );


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
		father = getColoredString( sf.first_name, m_ixName );
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
					parents += L",";
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
CString CGaDescendants::getColoredString( CString str, int index )
{
	CString colored(L"");
	if( !str.IsEmpty() )
		colored.Format( L"%s%s%s", attrib[index].code1, str, attrib[index].code2 );
	return colored;
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
		fullname += L" ";
	}
	if( !p->posterior.IsEmpty() )
	{
		fullname += p->posterior;
	}
	fullname.Trim();
	fullname += attrib[m_ixName].code2;
	return( fullname.Trim() );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getPlaceDateBlock( CString place, CString date, CString jel )
{
	
	CString block(L"");

	if( !place.IsEmpty() || !date.IsEmpty() )
	{
		block = getColoredString( jel, m_ixSpec );
		if( !place.IsEmpty() )
		{
			block += place;
			block += L" ";
		}
		if( !date.IsEmpty() )
		{
			block += date;
		}
		block.Trim();
		
	}
	return block;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getCommentBlock( CString comment )
{
	if( comment.IsEmpty() ) return L"";

	CString block = attrib[m_ixComment].code1;
	block += comment.Trim();
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
	p->posterior		= m_recordset.GetFieldString( PEOPLE_POSTERIOR );
	p->folyt			= m_recordset.GetFieldString( PEOPLE_FOLYT );
	p->gap				= m_recordset.GetFieldString( PEOPLE_GAP );
	p->generation		= m_recordset.GetFieldString( PEOPLE_GENERATION );
	p->known_as			= m_recordset.GetFieldString( PEOPLE_KNOWN_AS );
	p->last_name		= m_recordset.GetFieldString( PEOPLE_LAST_NAME );
	p->lineNumber		= m_recordset.GetFieldString( PEOPLE_LINENUMBER );
	p->mother_id		= m_recordset.GetFieldString( PEOPLE_MOTHER_ID );
	p->parentIndex		= _wtoi( m_recordset.GetFieldString( PEOPLE_MOTHER_INDEX ));
	p->parentIndexCalc	= _wtoi( m_recordset.GetFieldString( PEOPLE_MOTHER_INDEX2 ));
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

	if( ( p->comment.Find( L"http" ) ) != -1 ) p->comment.Empty();

}
