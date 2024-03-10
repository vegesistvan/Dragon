#include "stdafx.h"
#include "Dragon.h"
#include "descendants.h"
#include "utilities.h"
#include "chstring.h"
#include "utilities_dragon.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ix a kiirandó leszármazott indexe a vDesc vektorban
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printGAline()
{
//	fwprintf( fl, L"%d %d %d %d\n", vDesc.at(ix).childrenPrinted, vDesc.at(ix).gen, vDesc.at(ix).numOfChildren, vDesc.size() );

	CString rowid;
	rowid = vDesc.at( m_gen ).rowid;
	queryPeople( rowid, &p );

	m_printed = rowidExist();					// ismétlõdõ leszármazottak kiszûrése
	if (m_printed && m_checkDeleteRepeated )
		return;

	// a leszármazatti sor elõtt kiírja a megváltozott családnevet: % name
	if( m_last_name_prev != p.last_name && !m_radioDNameX )
	{
		str.Format(L"%c %s\n", '%', p.last_name );
		print(str);
	}
	m_last_name_prev = p.last_name;
	m_sex_id_prev = p.sex_id;

	printBegining();	// html kódok és generáció elkészítése; 
	printDescendant();
	printMarriages();

	if (!p.arm.IsEmpty())
	{
		str.Format(L" <font color='blue'>[%s család]</font>", p.arm);
		str.Format(L" <font color='blue'>[%s]</font>", p.arm);
		print(str);
	}
	if (!p.csalad.IsEmpty())
	{
		str.Format(L" <font color='blue'>[%s család]</font>", p.csalad);
		str.Format(L" <font color='blue'>[%s]</font>", p.csalad);
		print(str);
	}

	if (!p.folyt.IsEmpty() && m_checkFolyt)
//	if (!p.folyt.IsEmpty() )
	{
		str.Format(L"<font color='blue'> %c%c%c folyt %s</font>", '%', '%', '%', p.folyt);
		print(str);
	}
	fwprintf(fl, L"\n");
	fflush(fl);
	m_genPrev = m_gen;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A genráció változástól függû behuzás és genrációs kód nyomtatása az m_sytax értékétõl függõen
void CDescendants::printBegining()
{
	CString shift;
	CString tags;
	int		child;

	tags.Empty();
	if (m_gen > m_genPrev || m_genPrev == 0)			// jobbra tolás, új sor generációja nagyobb, (csak 1-esével nõhet a generáció)
	{
		tags = m_tag1;
		++m_indent;
	}
	else if (m_gen < m_genPrev)						// balra tolás vissza, régi generáció, kijebb hozza a generáció-különbség-szeresen
	{
		for (UINT i = 0; i < (m_genPrev - m_gen); ++i)
		{
			tags += m_tag2;
			--m_indent;
		}
	}
	shift = tags;
	tags += L"<li>";

	CString family;
	if (m_radioDNameX == 2)   // kiemelt családnév
	{
		if (m_familyName != p.last_name)  
		{
			family = getTableHeader();
//			family.Format(L"\n\n%s<p>%s</p>\n\n", shift, p.last_name );		// a tolást  elvégzi	
			family.Format(L"\n\n%s<p>%s</p>\n\n", shift, (CString)family);		// a tolást  elvégzi	
			tags = L"<li>";				// az ezt követõ leszármazottat már nem kell tolni
			print(getColoredString(family, m_comboKiemeltAttrib));
			m_familyName = p.last_name;
		}
	
	}

	TCHAR gen;
	int abc = 26;

	int cycle = (m_indent - 1) / abc;
	int bias = (m_indent - 1) % abc;


	if (isEven(cycle))
		gen = TCHAR('A') + bias;
	else
		gen = TCHAR('a') + bias;


	if (m_radioNumbering == SZLUHA)
	{
		str.Format(L"%s%c&diams;", tags, gen);
	}
	else if (m_radioNumbering == VIL)
	{
		if (m_gen > 0)
			child = vDesc.at(m_gen - 1).childrenPrinted;
		else
			child = 1;
		str.Format(L"%s%c%d&diams;", tags, gen, child);

	}
	else if (m_radioNumbering == TUP)
		str.Format(L"%s%c-%d&diams;", tags, gen, vSerial.at(m_gen));

	if (m_printed)
		str = getColoredString(str, PIROS);
	print(str);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getTableHeader()
{
	CString last_name;
	CString percent;
	CString family;
	CString roman;
	CString arm;

	
	m_command.Format(L"SELECT percent, familyName, tableRoman, arm FROM tables WHERE rowid='%s'", p.tableNumber);
	if (!query4(m_command)) return str;

	percent = m_recordset4.GetFieldString(0);
	family = m_recordset4.GetFieldString(1);
	roman = m_recordset4.GetFieldString(2);
	arm = m_recordset4.GetFieldString(3);

	last_name = p.last_name;
	last_name.MakeUpper();
	str = last_name;
	if (!p.titolo.IsEmpty())
		str.Format(L"%s, %s", (CString)str, p.titolo);

//	if (!arm.IsEmpty())
//		str.Format(L"%s (%s %s %s [%s])", (CString)str, percent, family, roman, arm );
//	else
	if( theApp.m_inputMode == GAHTML )
		str.Format(L"%s (%s %s %s)", (CString)str, percent, family, roman);

	return(str);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printDescendant()
{
	// leszármazott neve
	CString titolo;
	CString rang;
	CString lastname;
	int z;

	if (p.rowid == L"13")
		z = 1;

	
	if( m_radioDNameX == 1 )		// családnév a leszármazotti sorban
	{
		lastname = p.last_name;
		if (m_checkCapital)
			lastname = convertNameToUpper( lastname );
		
		if (lastname == L"N") lastname.Empty();

		//if( m_check_titolo && p.peer.IsEmpty() )
		/*
		if ( p.peer.IsEmpty())
			cLine.Format(L"%s %s %s", p.titolo, lastname, p.first_name);
		else
			cLine.Format(L"%s %s %s", p.peer, lastname, p.first_name);
		cLine.Trim();
		if (!p.other_names.IsEmpty())
			cLine.Format(L"%s_(%s)", (CString)cLine, p.other_names);
		*/


//		if (p.peer.IsEmpty())
//			cLine.Format(L"%s %s %s", p.titolo, lastname, p.first_name );
//		else


		if( lastname == L"N;")
			cLine.Format(L"%s %s %s", lastname, p.first_name, p.peer);
		else
			cLine.Format(L"%s %s %s %s", p.titolo, lastname, p.first_name, p.peer);
		cLine.Trim();
		if (!p.other_names.IsEmpty())
			cLine.Format(L"%s_(%s)", (CString)cLine, p.other_names);
		cLine = getColoredString( cLine, m_comboDAttrib );
		


	}
	else
	{
		cLine = getColoredString( p.first_name, m_comboDAttrib);
	}
	cLine.TrimRight();


// ha apjának több felesége volt (aki anya), akkor az anya sorszámának kiírása
/*
	if( m_gen > 0 && vDesc.at( m_gen - 1 ).numOfSpouses > 1 )
	{
		if( p.parentIndex && p.parentIndex != vDesc.at(m_gen-1).parentIndexLast ) // csak akkor írja ki a parentIndex-et, ha változik
		{
			vDesc.at(m_gen-1).parentIndexLast = p.parentIndex;
	
			if( m_radioNumbering == SZLUHA || m_radioNumbering == TUP )
			{
				str.Format( L"/%d", p.parentIndex ); 
				cLine += str;
			}
			else if ( m_radioNumbering == VIL )
				fwprintf( fl, L"%d. ", p.parentIndex );
		}
	}
*/
	if (m_gen > 0 && vDesc.at(m_gen - 1).numOfSpouses > 1)
	{
		if (vDesc.at(m_gen - 1).sex_id == WOMAN)
			p.parentIndex = _wtoi(p.whichHusband);
		else
			p.parentIndex = _wtoi(p.whichWife);

		if (p.parentIndex && p.parentIndex != vDesc.at(m_gen - 1).parentIndexLast) // csak akkor írja ki a parentIndex-et, ha változik
		{
			vDesc.at(m_gen - 1).parentIndexLast = p.parentIndex;

			if (m_radioNumbering == SZLUHA || m_radioNumbering == TUP)
			{
				str.Format(L"/%d", p.parentIndex);
				cLine += str;
			}
			else if (m_radioNumbering == VIL)
				fwprintf(fl, L"%d. ", p.parentIndex);
		}
	}


	cLine += L" ";

//	if (m_radioDNameX == 1)		// családnév a leszármazotti sorban
	{
	//	if (p.posterior != L"báró" && p.posterior != L"gróf" && p.posterior != L"herceg" && p.posterior != L"br" && p.posterior != L"gr" && p.posterior != L"hg")
		{
			cLine += L" ";
			cLine += p.posterior;
			cLine.TrimRight();
		}
	}

	// nem értem, hogy hogy az alábbi if-nek mi az értelme. Ezuért most kikapcsoltam 2020.10.04
	
//	if( m_radioNumbering == SZLUHA || m_radioNumbering == TUP )
	{
		m_birth = p.birth_date;
		str = getPlaceDateBlock( p.birth_place, p.birth_date, L"*" );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );

		m_birth = p.birth_date;
		str = getPlaceDateBlock( p.death_place, p.death_date, L"+" );
		if( !str.IsEmpty() )
			cLine.Format( L"%s %s", (CString)cLine, str );

		str.Empty();
		if( !p.comment.IsEmpty() )
		{ 
			if( !p.occupation.IsEmpty() )
				str.Format( L"%s, %s", p.comment, p.occupation );
			else
				str = p.comment;
		}
		else 
		{
			if( !p.occupation.IsEmpty() )
				str = p.occupation;
		}
		if( !str.IsEmpty() )
		{
			str = getColoredString( str, m_comboComment );
			cLine.Format( L"%s %s", (CString)cLine, str );
		}
	}
	cLine.Trim();

//	CString csalad;
	if( !p.csalad.IsEmpty() )
	{
		str.Format( L"<font color='blue'>[%s család]</font>", p.csalad );
		cLine += str;
	}
	if (m_printed)   // Ha a leszármazott ismétlõdõ, de nem írtuk elõ, hogy hagyja ki, akkor színesen listázza 
	{
		cLine += L"(!)";
		cLine = getColoredString(cLine, PIROS);
	}
	print( cLine );
	++m_db;
	fflush( fl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vRowid vektorba gyûjti a listázott leszármazottak rowid-jeit.
// Elõtte megnézi, hogy szerepel-e már benne. Ha igen true, ha nem false a visszatérés
bool CDescendants::rowidExist()
{
	int i;
	for (i = 0; i < vRowid.size(); ++i)
	{
		if (vRowid.at(i) == p.rowid)
			return true;
	}
	vRowid.push_back(p.rowid);
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printMarriages()
{
	CString spouse_id;
	CString place;
	CString date;
	CString marriage;
	CString spouse;
	int		numberOfSpouses;

	if( p.sex_id ==  MAN )
		m_command.Format( L"SELECT place, date, wife_id FROM marriages WHERE husband_id='%s' ORDER BY whichWife", p.rowid );
	else
		m_command.Format( L"SELECT place, date, husband_id FROM marriages WHERE wife_id='%s' ORDER BY whichHusband", p.rowid );
	if( !query2( m_command ) ) return;

	numberOfSpouses = m_recordset2.RecordsCount();
	if( !numberOfSpouses ) return;					// nincs házastárs

	vFullname.clear();
/*
	for (int i = 0; i < numberOfSpouses; ++i, m_recordset2.MoveNext())
	{
		spouse_id = m_recordset2.GetFieldString(2);
		queryPeople(spouse_id, &s);   // házastárs adatainak beolvasása
		vFullname.push_back(s.fullname);
	}

	m_recordset2.MoveFirst();
*/
	for( int i = 0; i < numberOfSpouses; ++i, m_recordset2.MoveNext() )
	{
		place	= m_recordset2.GetFieldString( 0 );
		date	= m_recordset2.GetFieldString( 1 );
		spouse_id = m_recordset2.GetFieldString( 2 );
		if( !spouse_id.IsEmpty() && spouse_id.Compare(L"0")  )
		{
			queryPeople( spouse_id, &s );   // házastárs adatainak beolvasása
			printMarriage( place, date, i, numberOfSpouses );
			printSpouse( );						// kiírás elõtt ellenõrzi, hogy szerepel-e ilyen névem már kiírt házastárs. Ha igen, akkor színezi
			vFullname.push_back(s.fullname);	// vFullname-en gyûjti a már kilistázott házastársak nevét
			printSpRelatives();
		}
	}
	fflush( fl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printMarriage( CString place, CString date, int i, int numberOfSpouses )
{
	CString marriage;

	if( numberOfSpouses > 1  )
		str.Format( L" %d=", i+1 );
	else
		str = L" =";

	marriage = getColoredString( str, m_comboSpec );

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
	print( marriage );
	fflush( fl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a házastárs listázados blokkját elkészíti
void CDescendants::printSpouse()
{
	CString spouse;
	CString fullname;
	CString lastname = s.last_name;

	if ( lastname != L"N;" && !s.titolo.IsEmpty() && s.peer.IsEmpty())
	{
		fullname += s.titolo;
		fullname += L" ";
	}
	if (!s.last_name.IsEmpty())
	{
		if (m_checkCapital)
			lastname = convertNameToUpper(lastname);

		fullname += lastname;
		fullname += L" ";
	}
	if (!s.other_names.IsEmpty())
	{
		fullname += L"_(";
		fullname += s.other_names;
		fullname += L") ";
	}
	if (!s.first_name.IsEmpty())
	{
		fullname += s.first_name;
	}
	fullname.Trim();
	fullname = getColoredString(fullname, m_comboOtherName);

	// Ellenõrti, hogy ilyen néven már listázott-e házastársat. Ha igen, akkoe színezi
	for (int i = 0; i < vFullname.size(); ++i)
	{
		if (s.fullname == vFullname.at(i))
		{
			fullname = getColoredString(fullname, PIROS);   //!!!!!!!!!!!!!!!!!!
			break;
		}
	}
	

	if (!s.title.IsEmpty())
	{
		fullname.Format(L"%s %s", s.title, (CString)fullname);
	}
	if (!s.peer.IsEmpty())
	{
		fullname.Format(L"%s %s", s.peer, (CString)fullname);
	}

	if (!s.posterior.IsEmpty())
	{
		fullname += L" ";
		fullname += s.posterior;
	}
	fullname.Trim();
	spouse = fullname;

//	m_birth = s.birth_date;
	str = getPlaceDateBlock(s.birth_place, s.birth_date, L"*");
	if (!str.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, str);

	str = getPlaceDateBlock(s.death_place, s.death_date, L"+");
	if (!str.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, str);

	str = getColoredString(s.comment, m_comboComment);
	if (!str.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, str);

	print(spouse);
	fflush(fl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a házastárs személyleíró blokkját elkészíti, a neveket bold-dal kiemnelve
void CDescendants::printSpRelatives()
{
	CString father(L"");
	CString mother(L"");
	CString parents;
	int		numOfSpouses;
	CString spouse_id;
	CString order;
	CString spouseSpouse;
	int pos;

	if( !s.father_id.IsEmpty() && s.father_id != L"0" )
	{
		queryPeople( s.father_id, &sf );
		sf.first_name.Replace('-', ' ');
		str = getFirstWord(sf.first_name);
	
		father = getColoredString( str, m_comboOtherName );
//		if (!sf.peer.IsEmpty())
//			father.Format(L"%s %s", sf.peer, (CString)father);


	}
	if( !s.mother_id.IsEmpty() && s.mother_id != L"0" )
	{
		queryPeople( s.mother_id, &sm );
		mother = getLastFirst( &sm );
		mother.Trim();
	}

	if( !father.IsEmpty() && !mother.IsEmpty() )		// ( apa és anya is meg van adva )
	{
		parents.Format( L" (%s-%s", father, mother );
	}

	if( !father.IsEmpty() && mother.IsEmpty() )
	{
		if( s.sex_id == MAN )
			parents.Format( L" (%s fia", father );
		else
			parents.Format( L" (%s lánya", father );
	}

	if( father.IsEmpty() && !mother.IsEmpty() )
	{
		if( s.sex_id == MAN )
			parents.Format( L" (%s fia", mother );
		else
			parents.Format( L" (%s lánya", mother );
	}


	if( s.sex_id == MAN )
		m_command.Format( L"SELECT wife_id, whichWife  FROM marriages WHERE husband_id='%s' ORDER BY whichWife", s.rowid ); // a házastárs házastársai
	else
		m_command.Format( L"SELECT husband_id, whichHusband FROM marriages WHERE wife_id='%s' ORDER BY whichHusband", s.rowid ); // a házastárs házastársai
	if( !query4( m_command ) ) return;
	numOfSpouses = m_recordset4.RecordsCount();
	if( numOfSpouses > 1 )
	{
		vSpouseIndex.clear();
		for( int i = 0; i < numOfSpouses; ++i, m_recordset4.MoveNext() )
		{
			spouse_id	= m_recordset4.GetFieldString( 0 );
			order		= m_recordset4.GetFieldString( 1 );
			queryPeople( spouse_id, &ss );
			if( ss.rowid != p.rowid )		// a GA sorban szereplõ házastársat kihagyja
			{
				spouseSpouse = getLastFirst( &ss );
				
				//spouseSpouse.Format(L"%s %s", ss.peer, (CString)spouseSpouse);
				// spouseSpouse.Trim();
				if( parents.GetLength() )
					parents += L", ";
				else
					parents = L" (";

				str.Format(L"%sf. %s", order, spouseSpouse);
				if (voltmar(order))
				{
					str = getColoredString(str, PIROS);
				}
				parents += str;
				vSpouseIndex.push_back(order);
			}
		}
	}
	if (!parents.IsEmpty())
	{
		parents.TrimRight();
		parents += L")";
	}
	print( parents );
	fflush( fl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendants::voltmar(CString index )
{
	for (int i = 0; i < vSpouseIndex.size(); ++i)
	{
		if (vSpouseIndex.at(i) == index)
			return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getColoredString( CString str, int index )
{
	CString colored( str ) ;
	if( !str.IsEmpty() && index )
		colored.Format( L"%s%s%s", attrib[index].code1, str, attrib[index].code2 );
	return colored;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getLastFirst( PPEOPLE* p ) // házatárs anyjának é stovábbi házastársainak neve képzõdik így
{

	CString name = p->last_name;
	if (!p->other_names.IsEmpty())
		name.Format(L"%s_(%s)", p->last_name, p->other_names);

	if( !p->first_name.IsEmpty() && !p->last_name.IsEmpty() )
	{
		if (m_checkCapital)
			name = convertNameToUpper( name );
		name += L" ";
		name += p->first_name;
		name = getColoredString( name, m_comboOtherName );
	}
	if( !p->peer.IsEmpty() )
		name.Format( L"%s %s", p->peer, (CString)name );

	return name;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getPlaceDateBlock( CString place, CString date, CString jel )
{
	
	CString block(L"");
	
	if (jel == L"+" && date == L"meghalt")
	{
		if (m_birth.Left(4) < L"1922")
			date.Empty();
	}
	
	if( !place.IsEmpty() || !date.IsEmpty() )
	{
		block = getColoredString(jel, m_comboSpec);
		if (!place.IsEmpty())
		{
			block += place;
			block += L" ";
		}
		if (!date.IsEmpty())
		{
			block += date;
		}
	}
	m_birth.Empty();
	block.Trim();
	return block;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::queryPeople( CString rowid, PPEOPLE* p )
{
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'", rowid );
	if( !query( m_command ) ) return;

	p->arm				= m_recordset.GetFieldString( DBP_ARM );
	p->birth_date		= m_recordset.GetFieldString( DBP_BIRTH_DATE );
	p->birth_place		= m_recordset.GetFieldString( DBP_BIRTH_PLACE );
	p->comment			= m_recordset.GetFieldString( DBP_COMMENT );
	p->csalad			= m_recordset.GetFieldString( DBP_CSALAD );
	p->death_date		= m_recordset.GetFieldString( DBP_DEATH_DATE );
	p->death_place		= m_recordset.GetFieldString( DBP_DEATH_PLACE );
	p->father_id		= m_recordset.GetFieldString( DBP_FATHER_ID );
	p->first_name		= m_recordset.GetFieldString( DBP_FIRST_NAME );
	p->peer				= m_recordset.GetFieldString( DBP_PEER);
	p->posterior		= m_recordset.GetFieldString( DBP_POSTERIOR );
	p->folyt			= m_recordset.GetFieldString( DBP_FOLYT );
	p->gap				= m_recordset.GetFieldString( DBP_GAP );
	p->last_name		= m_recordset.GetFieldString( DBP_LAST_NAME );
	p->lineNumber		= m_recordset.GetFieldString( DBP_LINENUMBER );
	p->mother_id		= m_recordset.GetFieldString( DBP_MOTHER_ID );
	p->parentIndex		= _wtoi( m_recordset.GetFieldString( DBP_PARENTINDEX ));
	p->tableAncestry	= m_recordset.GetFieldString( DBP_TABLEANCESTRY );
	p->occupation		= m_recordset.GetFieldString( DBP_OCCUPATION );
	p->whichChildFather	= m_recordset.GetFieldString( DBP_ORDERFATHER);
	p->whichChildMother	= m_recordset.GetFieldString( DBP_ORDERMOTHER );
	p->other_names		= m_recordset.GetFieldString( DBP_OTHER_NAMES );
	p->rowid			= m_recordset.GetFieldString( DBP_ROWID );
	p->sex_id			= m_recordset.GetFieldString( DBP_SEX_ID );
	p->source			= m_recordset.GetFieldString( DBP_SOURCE );
	p->tableNumber		= m_recordset.GetFieldString( DBP_TABLENUMBER );
	p->tableRoman		= m_recordset.GetFieldString( DBP_TABLEROMAN );
	p->title			= m_recordset.GetFieldString( DBP_TITLE );
	p->titolo			= m_recordset.GetFieldString( DBP_TITOLO );
	p->whichHusband		= m_recordset.GetFieldString(DBP_WHICHHUSBAND);
	p->whichWife		= m_recordset.GetFieldString(DBP_WHICHWIFE);

	p->fullname.Format(L"%s %s", p->last_name, p->first_name);

	if( ( p->comment.Find( L"http" ) ) != -1 ) p->comment.Empty();
}
