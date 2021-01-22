#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
#include "GAtoDB.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A d struktúra elõállítása insertálásra
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processDescendantSubstring( CString cLine )
{
	TCHAR	generation;
	int	pos;


	if( (pos = cLine.Find( ',' ) ) != -1 )
		pos = 0;


	generation = cLine.GetAt(0);				// generáció karakterének leszedése
	
	
	if( generation < m_generationPrev ) m_known_as.Empty();
	
	if( theApp.v_mother_index.size() )			// ha visszalép a generation, akkor törli az utolsó generációs bejegyzéseket
	{
		if( generation < m_generationPrev )
		{
			int i = theApp.v_mother_index.size() - 1;
			while( i && generation < theApp.v_mother_index.at(i).generation )
			{
				theApp.v_mother_index.pop_back();
				--i;
			}
		}
	}
	m_generationPrev = generation;

	cLine = cLine.Mid(2);						// G és szóköz leszedése
	d.generation		= generation;
	if( generation == '%' )
	{
		d.generation = generation;
		d.last_name = cLine.Trim();
		return;
	}
	if( m_tableAncestry )
		m_generationFirst = generation;
	if( generation == m_generationFirst )
		d.tableRoman = m_tableHeader.tableRoman;  // csak az õst jelöli meg, hogy az összekötésnél megtalálják
	else
		d.tableRoman.Empty();

// Megjelölöli minden tábla ösével azonos generációt, hogy a connectBranches() mindegyiket beállíthassa az õt hívó másik táblában lévõ 
// szülökre.

	d.folyt			= m_folyt;
	d.tableAncestry	= m_tableAncestry;
	d.source		= 1;

	// egy dummy családnevet tesz elé, és utána írja be a helyes elõ- és családnevet
	// Ez akkor jó pl. ha Farkas Jánost kellene szétbontani, ahol Farkas kerszetnév is lehet
	str.Format( L"XXXX %s", cLine );
	cLine = str.Trim();
	processPeopleStr( cLine, &d );
	d.titolo = m_tableHeader.titolo;
	d.last_name = m_tableHeader.familyName;
	
	// a mother_index csak ott van megadva, ahol változás történt, az ezt követõ gyerekek értelemszerûen ugyanannak az anyának a gyerekei.
	// mother_index-t kell kiírni gyerekhez a leszármazotti listánál ( 0-t nem kell )
	// mother_index2-t pedig minden gyereknél meg van adva, az  anya meghatározásához használjuk
	d.mother_index2	= getMotherIndex( generation, d.mother_index );		// a felülírt index



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
		cLine = cLine.Left( pos );
	}
	if( (pos = cLine.Find('*') ) != - 1 )
	{
		birthSubstr = cLine.Mid( pos + 1);				// születési blokk
		cLine = cLine.Left( pos );
	}
	nameSubstr = cLine.Trim();									// név blokk

	processNameSubstr( nameSubstr, any );
	processPlaceDateComment( birthSubstr, &bb );
	processPlaceDateComment( deathSubstr, &db );

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
void CGaInput::processNameSubstr( CString nameSubstr, PEOPLE* any )
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
	CString posterior;
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
	// i az elválasztó index a név és comment( posterior) között
	// megvan a  keresztnév. Ha van utána valami, akkor az posterior
	any->sex_id = sex_id;
	posterior = packWords( &A, i, n-i );
	any->posterior = posterior;

	fullName = packWords(&A, 0, i );
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
