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
	int i;


	if( (pos = cLine.Find( ',' ) ) != -1 )
		pos = 0;


	generation = cLine.GetAt(0);				// generáció karakterének leszedése
	
	
	if( generation < m_generationPrev ) m_known_as.Empty();
	
	if( vParent2Index.size() )			// ha visszalép a generation, akkor törli az utolsó generációs bejegyzéseket
	{
		if( generation < m_generationPrev )
		{
			i = vParent2Index.size() - 1;
			while( i && generation < vParent2Index.at(i).generation )
			{
				vParent2Index.pop_back();
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
	
	// a parent2Index csak ott van megadva, ahol változás történt, az ezt követõ gyerekek értelemszerûen ugyanannak az anyának a gyerekei.
	// parent2Index-t kell kiírni gyerekhez a leszármazotti listánál ( 0-t nem kell )
	// parent2IndexCalc-t pedig minden gyereknél meg van adva, az  anya meghatározásához használjuk
	d.parent2IndexCalc	= getParent2Index( generation, d.parent2Index );		// a felülírt index



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

	processNameSubstr( nameSubstr, birthSubstr, deathSubstr,  any );
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
void CGaInput::processNameSubstr( CString nameSubstr, CString birthSubstr, CString deathSubstr,  PEOPLE* any )
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
	int parent2Index;
	int ret;
	int sex_id;
	bool volt = false;

	nameSubstr.Trim();
	// parent2Index leszedése, ha van
	int n = wordList(&A, nameSubstr, ' ', FALSE );
	for( i = 0; i < n; ++i )
	{
		word = A[i];
		if( ( pos = word.Find( '/' ) ) != -1 )
		{
			str = word.Left( pos );
			if( ( ret = isFirstName( str ) ) != -1 )
			{
				parent2Index = _wtoi(word.Mid(pos+1));
				any->parent2Index = parent2Index;
				A[i] = str;
				break;
			}
		}
	}


	// [titolo][title][családnév][keresztnév][leírás]

	// [leírás] azonosítása
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
		if( fh4 != NULL && !nameSubstr.IsEmpty() )									// Privát->Házastársak nem nyitja meg az fh4-et!!!
		{
			fwprintf( fh4, L"%6d %s<br>\n", m_lineNumber, nameSubstr );
		}
		++m_error_cnt4;
		return;
	}
	// i az elválasztó index a név és comment( posterior) között
	// megvan a  keresztnév. Ha van utána valami, akkor az posterior, ha volt birth vagy death. He nem volt, akkor comment
	any->sex_id = sex_id;
	posterior = packWords( &A, i, n-i );
	posterior.Trim();
	if( !birthSubstr.IsEmpty() || !deathSubstr.IsEmpty() && !isdigit( posterior.GetAt(0) ) )
		any->posterior = posterior;
	else
		any->comment = posterior;


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
		if( ( ret = isFirstName( A[i+1] )) != -1 )		// az ezt követõ szó keresznév, ami lehet családnév is
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

// most már csak ilyen lehet: [tile] [családnév] {keresztnév]
// title azonsítása
// kisbetûvel nem lehet megkülönböztetni a rangot, mert számos idegen családnév kezdõdik kisbetûvel
// pl. di_, la_, von_, des_, del_

// ELÕNEVEK ELÕTTI CÍMEK AZONOSÍTÁSA
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
	any->title = title.TrimRight();


	any->last_name = A[i];		// családnév
	// a végérõl leszedi az utótagot
	if( iswlower( A[n-1][0] ) )
	{
		any->posterior = A[n-1];
		--n;
	}
	// ami marad, aza keresztnév
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A generation-parent2Index-bõl meghatározza az parent2IndexCalc-t, ami az anya indexe a v_generation.spouse_id[parent2IndexCalc]-ben 
// Mert a leszármazoitti listán csak az elsõ gyereknél van a parent2Index, az utána következõknél nem!
// Ezért határozza meg a generáció utolsó parent2Index-ét, amit visszaad 
//
// Visszamegy az vParent2Index vektorban a megadott generációhoz, ha volt ilyen. és annak mother-indexét visszadja
// és a ezzel a parent2Index-el és az aktuális generációval beteszi a vParent2Index vektorba.
// Ha nem talál azono sgenerációt, akkor mother_indexbe 1-et tesz.
// A vParent2Index vektorban gyûjti a táblában lévõ generációk utolsó parent2Index-ét, ami a leszármazott keresztneve után van megadva. (/n)
// Ha nincs megadva a leszármazott neve után index, akkor 1-et tesz bele
int CGaInput::getParent2Index( TCHAR generation, int n_mother_index )
{
	PARENT2INDEX mx;
	
	int parent2Index = n_mother_index;

	if( n_mother_index == 0 )   // ha a /n nincs megadva, akkor megnézi hogy volt-e már korábban ez a generáció?
	{
		int i;
		for( i= vParent2Index.size() - 1; i >= 0; i--)		// visszafele keresi a legutóbbi azonos generációt
		{
			if( vParent2Index.at(i).generation == generation )
			{
				parent2Index = vParent2Index.at(i).parent2Index;  // ha talált, akkor azt használja
				break;
			}
		}
		if( i == -1 )														// ha nem talált, akkor 1
		{
			parent2Index = 1;
		}
	}

	mx.generation	= generation;									// az adott generáció utolsó indexe
	mx.parent2Index = parent2Index;									// elteszi az aktuális beállítást
	vParent2Index.push_back( mx );
	return parent2Index;
}