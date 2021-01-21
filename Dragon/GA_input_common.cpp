#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
#include "GAtoDB.h"
#include "SaveFirstName.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// S E G É D   F Ü G G V É N Y E K ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////û
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ellenõrzi, hogy a megadott first_name létezik-e a nyilvántartásban.
// Ha létezik, akkor megadja a nemének kódját ( férfi=0, nõ=2 )
// Ha nem létezik, akkor a hiba listába írja és 0 sex_id-t ad vissza!!
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaInput::getSexId( CString first_name )
{
	int sex_id1 = 0;
	int sex_id2 = 0;
	CString name;
	CString utonev;
	
	int pos;

	first_name.Trim();

	int n = countWords( first_name );

	first_name.Replace( '?', ' ' );
	first_name.Replace( '_', ' ' );
	first_name.Replace( '-', ' ' );
	first_name.Trim();

	utonev = getWord( first_name, 1, &pos );
	m_command.Format( L"SELECT sex_id FROM firstnames WHERE first_name = '%s'", utonev );
	if( !theApp.querySystem( m_command ) ) return 0;
	if( theApp.m_recordsetSystem->RecordsCount() )
	sex_id1 = _wtoi( theApp.m_recordsetSystem->GetFieldString( 0 ) );
	if( n > 1 )
	{
		utonev = getWord( first_name, 2, &pos );
		m_command.Format( L"SELECT sex_id FROM firstnames WHERE first_name = '%s'", utonev );
		if( !theApp.querySystem( m_command ) ) return 0;
		if( theApp.m_recordsetSystem->RecordsCount() )
			sex_id2 = _wtoi( theApp.m_recordsetSystem->GetFieldString( 0 ) );
		if( sex_id1 != sex_id2 )
			sex_id1 = -1;
	}
	return sex_id1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaInput::isTitle( CString cLine ) 
{
	m_command = L"SELECT title FROM titles";
	if( !theApp.querySystem( m_command ) ) return FALSE;

	for( UINT i = 0; i < theApp.m_recordsetSystem->RecordsCount(); ++i, theApp.m_recordsetSystem->MoveNext() )
	{
		if( cLine == theApp.m_recordsetSystem->GetFieldString(0 ) ) return TRUE;
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A _,-,? és ) karaktereket leszedi, a maradékot szétszedi. Csak az elsõ szó létezését vizsgálja 
// return:
// -1 nem keresztnév
// 0 biszex
// 1 férfi keresztnév
// 2 nõi keresztnév
int CGaInput::isFirstName( CString name )
{
	name.Replace( ')', ' ' );
	name.Replace( '?', ' ' );
	name.Trim();
	if( name.IsEmpty() ) return -1;

	int z;
	int pos;
	int i;
	int n;
	CStringArray A;

	if( (pos = name.Find( '_' ) ) != -1 )
		n = wordList( &A, name, '_', FALSE );
	else if( (pos = name.Find( '-' ) ) != -1 )
		n = wordList( &A, name, '-', FALSE );
	else
		n = wordList( &A, name, ' ', FALSE );



	if( n > 1 && A[1] == L"Nep" )
		z = 1;

	m_command.Format( L"SELECT rowid, sex_id FROM firstnames WHERE first_name='%s'", A[0] );
	if( !theApp.querySystem( m_command ) )	return -1;
	if( theApp.m_recordsetSystem->RecordsCount() )
		return _wtoi( theApp.m_recordsetSystem->GetFieldString( 1 ) );
	else
		return -1;

/*


	for( i = 0; i < n; ++i )
	{
		m_command.Format( L"SELECT rowid, sex_id FROM firstnames WHERE first_name='%s'", A[i] );
		if( !theApp.querySystem( m_command ) )	return -1;
		if( !theApp.m_recordsetSystem->RecordsCount() ) break;
	}
	if( i == n )
		return _wtoi( theApp.m_recordsetSystem->GetFieldString( 1 ) );
	return -1;
*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//typedef struct
//{
//	CString place;
//	CString date;
//	CString comment;
//}PLACE_DATE_BLOCK;

// Called from:	splitDesendantsubstring
//				splitMarriagesVector
//				splitPeopesString

// [place][date][comment] blokkot felbontja elemeire.
// Ha a place után ',' van, akkor az utána álló szám már comment
// Ha talál numerikus szót, akkor az elõtte lévõ string a place, az utána lévõ a comment
// Ha nem talál numerikus szót, akkor a place-ben adja vissza az egész stringet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void splitPlaceDateComment( CString placeDateComment, PLACE_DATE_BLOCK* ns )
{
	ns->place.Empty();
	ns->date.Empty();
	ns->comment.Empty();

	if( placeDateComment.IsEmpty() ) return;

	CStringArray A;
	CString word;
	CString date;
	CString datum;
	CString place;
	CString comment;
	int i;
	int pos;
	int	ret;

	int n = wordList( &A, placeDateComment, ' ', TRUE );
	

	// megkeresi az elsõ numerikus szót
	for( i = 0; i < n; ++i )
	{
		ret = isDate( &A, i, &datum );
        if( ret )  // van datum!!
		{
			place	= packWords( &A, 0, i );			// dátum elõtt 'place'  (ha van)
			if( place.Right(1) == ',' )
			{
				place = place.Left( place.GetLength() - 1 );
				ns->comment = datum;
				ns->comment += L" ";
			}
			else
			{
				ns->date = datum;
			}
			ns->place = place;

//			ns->place	= packWords( &A, 0, i );			// dátum elõtt 'place'  (ha van)
			

//			ns->date	= datum;
			if( (i+ret) < n )
			{
				if( A[i+ret] == L"éves" || A[i+ret] == L"napos" )	// x éves || x napos
				{
					ns->date.Empty();								// date nem lesz
					--i;											// comment: x éves || x napos
				}
				ns->comment	+= packWords( &A, i + ret , n - (i + ret) );
			}
			return;
		}
	}


	// Nem talált numerikus szót a placeDateComment stringben. Csak place[, comment] van!!!
	// Ha van comment, akkor vesszõnek kell lenni a place és comment között!!
	if( ( pos = placeDateComment.Find( ',' ) ) != -1 )
	{
		ns->place = placeDateComment.Left( pos );
		ns->comment = placeDateComment.Mid( pos + 2 );   // 2: mert , és szóköz is van!
	}
	else
		ns->place = placeDateComment;   // nem talált vesszõt, az egész sor place
}

// who: 1 házastárs apja
// who: 2 házastárs anyja
// who: 2 házastárs további házastársa
//
// cLine:		nameString [*birthString] [+deathstring]
// nameString:	[title][titoloString][lastName][firstNameString][comment]
// birthString: [place][date][comment]
// deathString: [place][date][comment]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitPeopleString( int who,  CString cLine, PEOPLE * p )
{
	clearPeople( p );
	if( cLine.IsEmpty() ) return;

	CString nameString;
	CString birthString;
	CString deathString;

	CString comment(L"");

	NAME		name;
	PLACE_DATE_BLOCK b;
	PLACE_DATE_BLOCK d;

	int pos;

	
	nameString.Empty();

	birthString.Empty();
	deathString.Empty();

	if( (pos = cLine.Find('+') ) != - 1 )
	{
		deathString = cLine.Mid( pos + 1 );
		cLine = cLine.Left( pos );
	}
	if( (pos = cLine.Find('*') ) != - 1 )
	{
		birthString = cLine.Mid( pos + 1);
		cLine = cLine.Left( pos );
	}
	nameString = cLine;

	if( who == 1 )
		splitSpFatherName( nameString, &name );
	else
		splitSpouseNameString( nameString, &name );
//		splitSName( nameString, &name, who  );

	comment += name.comment;
	comment += L" ";

	splitPlaceDateComment( birthString, &b );
	comment += b.comment;
	comment += L" ";

	splitPlaceDateComment( deathString, &d );
	comment += d.comment;
	comment.Trim();

	p->sex_id		= name.sex_id;
	p->title		= name.title;
	p->titolo		= name.titolo;
	p->first_name	= name.first_name;
	p->last_name	= name.last_name;

	p->birth_place	= b.place;
	p->birth_date	= b.date;

	p->death_place	= d.place;
	p->death_date	= d.date;

	p->comment		= comment;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Csak keresztnév van, nincs title,titolo,last_name!!!
// nameStr: [keresztnév] [comment]
void CGaInput::splitSpFatherName( CString nameStr, NAME* name ) 
{
	CString word;
	CStringArray A;
	int n;
	int i = 0;
	CString last( L"" );
	CString first( L"" );
	CString comment( L"" );

	clearName( name );
	nameStr.Trim();
	if( nameStr.GetLength() == 0 ) return;		// ha üres a sor, nincs semmi

	n = wordList( &A, nameStr, ' ', FALSE );

	// hátulról az elsõ keresztnév keresése
	for( i = n - 1; i >= 0; --i )
	{
		word = A[i];
		word.Trim();
		if( isFirstName( word ) != - 1 ) break;
	}
	first = packWords( &A, 0, i+1 );  
	if( i < n -1 ) 	comment = packWords( &A, i+1, n - i - 1 );

	name->first_name	= first;
	name->comment		= comment;
	name->sex_id		= MAN;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processPeopleStr( CString cLine,  PEOPLE* any )
{
	if( cLine.IsEmpty() ) return;

	PLACE_DATE_BLOCK bb;
	PLACE_DATE_BLOCK db;
	int		pos;

	CString nameSubstr;
	CString birthSubstr;
	CString deathSubstr;

	if( (pos = cLine.Find('+') ) != - 1 )
	{
		deathSubstr	= cLine.Mid( pos + 1 );				// halálozási blokk
		cLine = cLine.Left( pos - 1 );
	}
	if( (pos = cLine.Find('*') ) != - 1 )
	{
		birthSubstr = cLine.Mid( pos + 1);				// születési blokk
		cLine = cLine.Left( pos - 1 );
	}
	nameSubstr = cLine;									// név blokk

	splitNameSubstr( nameSubstr, any );
	splitPlaceDateComment( birthSubstr, &bb );
	splitPlaceDateComment( deathSubstr, &db );

	any->birth_place	= bb.place;
	any->birth_date		= bb.date;
	if( !bb.comment.IsEmpty() )
		any->comment = bb.comment;

	any->death_place	= db.place;
	any->death_date		= db.date;
	if( !db.comment.IsEmpty() )
		any->comment = db.comment;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// name [[,] [posterior][comment]] szétszedése elemeire
// A name[[,] [comment] elválasztó index keresése.
// Az elválasztó index biztos nagyobb mint 1, mert vezetéknévnek mindenképpen kell lenni
// A 'name' vége: 
// 'name,' vagy 
//	a balról-jobbra elsõ keresztnév, amit nem keresztnév követ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitNameSubstr( CString nameSubstr, PEOPLE* any )
{
	if( nameSubstr.IsEmpty() ) return;

	CStringArray A;
	int i;
	int j;
	int z;
	int pos;
	CString word;
	CString fullName;
	CString comment;
	CString title;
	CString mother_index;
	int ret;
	int sex_id;
	bool volt = false;


	// mother_index leszedése, ha van
	int n = wordList(&A, nameSubstr, ' ', FALSE );
	for( i = 0; i < n; ++i )
	{
		word = A[i];
		if( ( pos = word.Find( '/' ) ) != -1 )
		{
			str = word.Left( pos );
			if( ( ret = isFirstName( str ) ) != -1 )
			{
				mother_index = word.Mid(pos+1);
				any->mother_index = _wtoi( mother_index );
				A[i] = str;
				break;
			}
		}
	}


	if( nameSubstr == L"Palásthy Judit 1689" )
		z = 1;

	// megkeresi a név és az esetleges comment elválasztó indexét, ami az utolsó keresztnevet követõ nemkeresztnévnél van
	for( i = 1 ; i < n; ++i )	// az elsõ keresztnevet keresi, de az lehet helyiség (pl. Gyula is, és lehet családnév is (pl. péter Zoltán
	{
		word = A[i];
		word.Replace( ',', ' ' );						// a név után lehet vesszõ !!
		word.Replace( '?', ' ' );
		word.TrimRight();
		
		if( ( ret = isFirstName( word ) ) != -1 )			//  =Budaörs 1944.12.17 Richard Rostoczil Mária
		{
			sex_id = ret;									// az utolsó keresztnév számít. pl  Behárfalvi Urbán Anna
															// hamis firsName kiszûrése
			if( A[i-1].Right(1) == ',' ) continue;			// ha az elõzõ szó végén , van  Mikor???
			if( i+1 < n && A[i+1] == L"és" ) continue;		// Márkus és Batiizfalvi....ez nem keresztnév!
			if( iswdigit( A[i-1].GetAt(0) ) ) continue;		// ha elõtte szám van, akkor nem fogadja el miért????

			volt = true;									// ez már elfogadott keresztnév, de még keres továbbiakat
			if( isLastCharacter( A[i], ',' ) )				// ha vesszõ van, akkor az éppen a nevet és a commentet választja el
			{
				A[i].Replace( ',', ' ' );
				A[i].TrimRight();
				++i;
				break;										// i: elválasztó index!
			}												// ha nincs elválasztó index, akkor további keresztnevet keres!!
		}
		else												
		{
			if( volt ) break;								// keresztnevet követõ elsõ nem kersztnév. Elválasztó index!
		}
	}
	if( !volt )												// nem talált kersztnevet, baj van!!
	{
		if( fh4 != NULL )									// Privát->Házastársak nem nyitja meg az fh4-et!!!
		{
			fwprintf( fh4, L"%6d<br>\n", m_lineNumber, nameSubstr );
		}
		++m_error_cnt4;
		return;
	}
	// i az elválasztó index a név és comment között
	any->sex_id = sex_id;
	if( i <= n )   // megvan a  keresztnév. Ha van utána valami, akkor az posterior vagy comment.
	{
		for( ; i < n; ++i )
		{
			if( isTitle( A[i] ) )
			{
				any->title += A[i];
			}
			else
				break;
		}

		comment = packWords( &A, i, n-i );
//		if( !comment.IsEmpty() ) any->comment = comment;
		if( !comment.IsEmpty() ) any->posterior = comment;


		fullName = packWords(&A, 0, i );
	}
	else
		fullName = nameSubstr;

	n = wordList(&A, fullName, ' ', FALSE );
		
	if( n < 2 ) return;

// kisbetûvel nem lehet megkülönböztetni a rangot, mert számos idegen családnév kezdõdik kisbetûvel
// pl. di_, la_, von_, des_, del_
	for( i = 0; i < n; ++i )
	{
		if( iswlower( A[i].GetAt( 0 ) ) )
		{
			if( A[i].Find( '_' ) != -1 ) break;		// ezek elfordulhatnak idegen családnevekben
			if( A[i].Find( '|' ) != -1 ) break;		// a családnevekben elõforduló '-t helyettesíti a | karkter

			title += A[i];
			title += L" ";
		}
		else
			break;
	}
	any->title = title.TrimRight();

	if( isLastCharacter( A[i], 'i' ) )						// elõnév következik
	{
		if( ( ret = isFirstName( A[i+1] )) != -1 )		// nem hamis elõnév
		{
			if( i+2 < n )  // azzaz már csak 3 tag vamn hátra
			{
				any->titolo		= A[i];
				any->last_name	= A[i+1];
				any->first_name	= A[i+2];			
				return;
			}
			else if( i+1 < n )
			{
				any->last_name = A[i];
				any->first_name = A[i+1];
				return;
			}
		}
		if( A[i+1] == L"és" )					// kettõs elõnév
		{
			any->titolo = packWords( &A, i, 3 );
			i += 3;
		}
		else
		{
			any->titolo = A[i];
			++i;
		}
	}
	any->last_name = A[i];		// családnév
	if( iswlower( A[n-1][0] ) )
	{
		any->posterior = A[n-1];
		--n;
	}
	any->first_name = packWords( &A, i+1, n-i-1 );

}
