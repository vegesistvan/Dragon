#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ez a függvény bontjha fel az 1-4. rangú bejegyzések személyes adatait
// bejegyzés
// 1. rangú: az egész sor a generációs kód nélkül, elé téve az XXXX dummy családnevet
// 2. rangú: az esküvõ adataitól megtisztított substring
// 3. rangú: a teljes sor, apa estén elé téve az XXX dummy családnevet
// 4. rangú: a sor elején lévõ N. utáni substring
//
// Ílymódon kaptt string egységesen kezelhetõ.
// name[*birth] [+death] [comment]
//
// A szétszedett adatrokat az "any" struktõúrába teszi el. 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processPeopleStr( CString cLine,  PEOPLE* any )
{
	if( cLine.IsEmpty() ) return;

	PLACE_DATE_BLOCK bb;
	PLACE_DATE_BLOCK db;
	int		pos;

	m_nameSubstr.Empty();
	m_birthSubstr.Empty();
	m_deathSubstr.Empty();

	cLine.Trim();


	
 	if( (pos = cLine.Find('+') ) != - 1 )
	{
		m_deathSubstr	= cLine.Mid( pos + 1 );		// halálozási blokk
		cLine = cLine.Left( pos );
	}
	if( (pos = cLine.Find('*') ) != - 1 )
	{
		m_birthSubstr = cLine.Mid( pos + 1);		// születési blokk
		cLine = cLine.Left( pos );
	}
/*
	CString twoWords;
	twoWords = getTwoWords( cLine );
	if( (pos = twoWords.Find( ',' ) ) != -1 )
	{
		m_nameSubstr = cLine.Left( cLine.Find( ',') );
		any->comment = cLine.Mid( cLine.Find( ',' ) + 1 );
		any->comment.Trim();
	}
	else
*/
	m_nameSubstr = cLine;
	processNameSubstr( any );
	processPlaceDateComment( m_birthSubstr, &bb );
	processPlaceDateComment( m_deathSubstr, &db );

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
void CGaInput::processNameSubstr( PEOPLE* any )
{
	if( m_nameSubstr.IsEmpty() ) return;

	CStringArray A;
	int i;
	int j;
	int z;
	int pos;
	CString word;
	CString fullName;
	CString comment;
	CString title;
	CString posterior;
	int parentIndex;
	int ret;
	int sex_id;
	bool volt = false;
	int numOfFirstNames = 0;


	int n = wordList(&A, m_nameSubstr, ' ', FALSE );
	for( i = 0; i < n; ++i )
	{
		word = A[i];
//		word.Replace( '?', ' ' );  // az egy szón belül a ? helyén maradjon space, hogy a Mária???Emese-féle neveket kezeni tudja!!
		word.Trim();
		A[i] = word;
	}

	// parentIndex leszedése, ha van
	for( i = 0; i < n; ++i )
	{
		word = A[i];
//		word.Remove( '?' );
		if( ( pos = word.Find( '/' ) ) != -1 )
		{
			str = word.Left( pos );
			if( ( ret = theApp.isFirstName( str ) ) != -1 )
			{
				parentIndex = _wtoi(word.Mid(pos+1));
				any->parentIndex = parentIndex;
				A[i] = str;
				break;
			}
		}
	}


	// [titolo][title][családnév][keresztnév][posterior][leírás]

	// [leírás] azonosítása
	if( m_nameSubstr == L"Palásthy Judit 1689" )
		z = 1;

	// megkeresi a név és az esetleges comment elválasztó indexét, ami az utolsó keresztnevet követõ nemkeresztnévnél van
 	for( i = 1 ; i < n; ++i )	// az elsõ keresztnevet keresi, de az lehet helyiség (pl. Gyula is, és lehet családnév is (pl. péter Zoltán
	{
		word = A[i];
		word.Replace( ',', ' ' );						// a név után lehet vesszõ !!
		word.Replace( '?', ' ' );
		word.Remove( '(' );
		word.Remove( ')' );
		
		word.TrimRight();
		
		if( ( ret = theApp.isFirstName( word ) ) != -1 )			//  =Budaörs 1944.12.17 Richard Rostoczil Mária
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
		m_nameSubstr.Remove( '?' );
		m_nameSubstr.Trim();
		any->last_name = getFirstWord( m_nameSubstr );
		any->first_name = getLastWord( m_nameSubstr );
		return;
	}
	// i az elválasztó index a név és comment( posterior) között
	// megvan a  keresztnév. Ha van utána valami, akkor az posterior, ha volt birth vagy death. He nem volt, akkor comment
	any->sex_id = sex_id;
	posterior = packWords( &A, i, n-i );
	posterior.Trim();
	if( !m_birthSubstr.IsEmpty() || !m_deathSubstr.IsEmpty() && !iswdigit( posterior.GetAt(0) ) )
		any->posterior = posterior;
	else
	{
		if( !posterior.IsEmpty() )
			any->comment = posterior;
	}


//// FULLNAME FELBONTÁSA ///////////////////////////////////////////////////

	fullName = packWords(&A, 0, i );
	n = wordList(&A, fullName, ' ', FALSE );
	if( n < 2 ) return;
	i = 0;
// [title][elõnév][title][családnév][keresztnév][utótag]
// a title két helyen is elõfordulhat!!
// ELÕNEVEK ELÕTTI CÍMEK AZONOSÍTÁSA
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

// x és y ELÕNEVEK AZONOSÍTÁSA

	if( i+1 < n && A[i+1] == L"és" )					// X és Y, ahol X nem i-re végzõdik
	{
		any->titolo = packWords( &A, i,  3 );
		i += 3;
	}
														// Giczi, Assa és Ablánckürti Ghyczy Anna Mária
	if( ((i + 2)  < n) && A[i+2] == L"és" )
	{
		any->titolo = packWords( &A, i, 4 );
		i += 4;
	}

	
// Xi elõnevek azonosítása	
	if( isLastCharacter( A[i], 'i' ) )					// i-re végzõdõ egytagú elõnebek azonosítása 
	{													// elõnév következhet, de lehet családnév is!!
		if( ( ret = theApp.isFirstName( A[i+1] )) != -1 )		// az ezt követõ szó keresznév, ami lehet családnév is
		{
			if( i+2 < n )								// azzaz már csak 3 tag van hátra, akkor ezek: titolo, lastname, firstname
			{
				any->titolo		= A[i];
				any->last_name	= A[i+1];
				any->first_name	= A[i+2];			
				return;
			}
			else if( i+1 < n )							// már csak 2 szó van hátra, akkor azok: lasstname, firstname
			{
				any->last_name = A[i];
				any->first_name = A[i+1];
				return;
			}
		}
		any->titolo = A[i];
		++i;
	}

// most már csak ilyen lehet: [title][családnév][keresztnév][posterior]
// title azonsítása
// kisbetûvel nem lehet megkülönböztetni a rangot, mert számos idegen családnév kezdõdik kisbetûvel
// pl. di_, la_, von_, des_, del_

// ELÕNEVEK ELÕTTI CÍMEK AZONOSÍTÁSA
	title.Empty();
	for( ; i < n; ++i )
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
	if( any->title.IsEmpty() )
		any->title = title.TrimRight();

	
	// a végérõl leszedi az utótagot
	if( iswlower( A[n-1][0] ) )
	{
		any->posterior = A[n-1];
		--n;
	}


	// kettõs vezetéknevek kezelése
	// i-n a család- és keresztenevek sorozata
	// a családnév is lehet kettõs és a kerezstnév is lehet kettõs, vagy többszörös

	for( int j = i+1; j < n; ++j )
	{
		word = A[j];
		if( ( ret = theApp.isFirstName( word ) ) != -1 )		// az ezt követõ szó keresznév, ami lehet családnév is
			++numOfFirstNames;
	}
	int k = n - i;			// a szavak száma
	any->first_name = packWords( &A, n-numOfFirstNames, numOfFirstNames );
	any->last_name	= packWords( &A, i, k - numOfFirstNames );
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
void CGaInput::processPlaceDateComment( CString placeDateComment, PLACE_DATE_BLOCK* ns )
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
