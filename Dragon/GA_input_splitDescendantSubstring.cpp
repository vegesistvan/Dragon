#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
#include "GAtoDB.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A d struktúra elõállítása insertálásra
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitDescendantSubstring2( CString cLine )
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

	d.mother_index2	= getMotherIndex( generation, d.mother_index );		// a felülírt index
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

	str.Format( L"%s %s %s", m_tableHeader.titolo, m_tableHeader.familyName, cLine );
	cLine = str.Trim();
	processPeopleStr( cLine, &d );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processPeopleStr( CString cLine,  PEOPLE* any )
{
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

	any->sex_id = sex_id;
	if( i <= n )   // megvan a  keresztnév. Ha van utána valami, akkor az posterior vagy comment
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
		if( !comment.IsEmpty() ) any->comment = comment;

		fullName = packWords(&A, 0, i );
	}
	else
		fullName = nameSubstr;

	n = wordList(&A, fullName, ' ', FALSE );
		
	if( n < 2 ) return;

	// a sor elejérõl leszedi a title-t, ami mindig kisbetûs
	for( i = 0; i < n; ++i )
	{
		if( iswlower( A[i].GetAt( 0 ) ) )
		{
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
			any->last_name	= A[i];
			any->first_name	= packWords( &A, i+1, n-i-1 );
			return;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A d struktúra elõállítása insertálásra
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitDescendantSubstring( CString cLine )
{
	CString comment(L"");
	NAMEBLOCK	name;
	PLACE_DATE_BLOCK bb;
	PLACE_DATE_BLOCK db;
	CString arm;

	TCHAR	generation;
	int	pos;

	_descNameSS.Empty();
	_descBirthSS.Empty();
	_descDeathSS.Empty();
	_descCommentSS.Empty();

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

	if( generation == '%' )
	{
		d.generation = generation;
		d.last_name = cLine.Trim();
		return;
	}
	d.last_name = m_familyName;
	d.last_name = m_tableHeader.familyName;
	d.titolo	= m_tableHeader.titolo;

////////////

	if( (pos = cLine.Find('+') ) != - 1 )
	{
		_descDeathSS	= cLine.Mid( pos + 1 );				// halálozási blokk
		cLine = cLine.Left( pos - 1 );
	}
	if( (pos = cLine.Find('*') ) != - 1 )
	{
		_descBirthSS = cLine.Mid( pos + 1);					// születési blokk
		cLine = cLine.Left( pos - 1 );
	}

	_descNameSS = cLine;									// név blokk


	splitDescNameString( _descNameSS );
	comment = d.comment;
	comment += L" ";

	splitPlaceDateComment( _descBirthSS, &bb );
	comment += bb.comment;
	comment += L" ";

	splitPlaceDateComment( _descDeathSS, &db );
	comment += db.comment;
	comment.Trim();

	_descCommentSS = comment;

	d.mother_index2 = getMotherIndex( generation, d.mother_index );		// a felülírt index
	d.generation	= generation;

	d.birth_place	= bb.place;
	d.birth_date	= bb.date;

	d.death_place	= db.place;
	d.death_date	= db.date;
	d.comment		= comment;
	d.folyt			= m_folyt;
	d.tableAncestry	= m_tableAncestry;
	d.source		= 1;

// Megjelölöli minden tábla ösével azonos generációt, hogy a connectBranches() mindegyiket beállíthassa az õt hívó másik táblában lévõ 
// szülökre.

	if( m_tableAncestry )
		m_generationFirst = generation;
	if( generation == m_generationFirst )
		d.tableRoman = m_tableHeader.tableRoman;  // csak az õst jelöli meg, hogy az összekötésnél megtalálják
	else
		d.tableRoman.Empty();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A generation-mother_index-bõl meghatározza az mother_index2-t, ami az anya indexe a v_generation.spouse_id[mother_index2]-ben 
//
//
// Visszamegy az v_mother_index vektorban a megadott generációhoz, ha volt ilyen. és annak mother-indexét visszadja
// és a ezzel a mother_index-el és az aktuális generációval beteszi a v_mother_index vektorba.
// Ha nem talál azono sgenerációt, akkor mother_indexbe 1-et tesz.
// A v_mother_index vektorban gyûjti a táblában lévõ generációk utolsó mother_index-ét, ami a leszármazott keresztneve után van megadva. (/n)
// Ha nincs megadva a leszármazott neve után index, akkor 1-et tesz bele
int CGaInput::getMotherIndex( TCHAR generation, int n_mother_index )
{
	MOTHERINDEX mx;
	mx.generation = generation;
	int mother_index = n_mother_index;


	if( n_mother_index == 0 )   // ha a /n nincs megadva, akkor megnézi hogy volt-e már korábban ez a generáció?
	{
		int i;
		for( i= theApp.v_mother_index.size() - 1; i >= 0; i--)		// visszakeresi a legutóbbi azonos generációt
		{
			if( theApp.v_mother_index.at(i).generation == generation )
			{
				mother_index = theApp.v_mother_index.at(i).mother_index;  // ha talált, akkor azt használja
				break;
			}
		}
		if( i == -1 )														// ha nem talált, akkor 1
		{
			mother_index = 1;
		}
	}

	mx.mother_index = mother_index;									// elteszi az aktuális beállítást
	theApp.v_mother_index.push_back( mx );
	return mother_index;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// namestring: [title] first_name [first_name2][/n][,] [posterior]|[comment]
// Hibás felbontás csak akkor képzelhetõ el, ha a 'firstnames' ill 'titles' nyilvántartás hiányos.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitDescNameString( CString nameSubstring )
{
	CStringArray A;
	CString first_name(L"");
	int	sex_id = 0;

	CString word;
	int i = 0;
	int n;
	int m;
	

	if( nameSubstring.IsEmpty() ) return;	// ez bizony elõfordulhat!!

	n = wordList( &A, nameSubstring, ' ', FALSE );

	if( isTitle( A[0] ) )
	{
		d.title	= A[0];
		++i;
	}
	for( i; i < n; ++i )							// utónevet keres balról jobbra
	{
		word = A[i];
		m = word.GetLength();

		if( iswupper( word.GetAt(0) ) && word[m-1] == ',' )	// ha egy nagybetûs szó utolsó karaktere vesszõ, akkor utána már comment következik
		{
			str = word.Left( m-1 );
			if( ( sex_id = isFirstName( str ) ) != - 1 )
			{
				first_name		+= str;
				d.first_name	= first_name;
				d.sex_id		= sex_id;
				if( i + 1 < n )
					d.comment		= packWords(&A, i+1, n - i - 1 );

				return;
			}
			break;
		}

		if( ( sex_id = isFirstName( word ) ) == -1  )
		{											// nem utónév, akkor megnézi, hogy van e a végén /n Ez a mother index lenne
			m = word.GetLength();
			if( m < 3 ) break;
			if( word[ m-2 ] == '/' )
			{
				str = word.Left( m - 2 ); 
				if( ( sex_id = isFirstName( str ) ) != -1 )
				{
					first_name			+= str;
					d.sex_id			= sex_id;
					d.mother_index		= _wtoi(  word.Right(1) );
					++i;  // új
				}
			}
//			++i;
			break;	// tovább nem vizsgálja a szavak, met vagy comment vagy /n, ami utolsó keresztnév lehet
		}
		else
		{
			first_name += A[i];
			first_name += L" ";
			d.sex_id = sex_id;
		}
	}
	first_name.Trim();
	d.first_name = first_name;

	if( first_name == L"Gergõ" )
		int z = 1;

	if( i < n )		// a név után 
	{
		/*
		if( word != L"de" )
		{
			if( isTitle( word ) )
			{
				d.title	= word;
				++i;
			}
		}
		*/
		if( n != 1 )
		{
			d.comment = packWords( &A, i, n-i );
		}
	}
	
	if( !d.comment.IsEmpty() && !iswdigit( d.comment.GetAt(0) ) )
	{
		d.posterior = d.comment;
		d.comment.Empty();
	}
	if( first_name == L"N" && !d.posterior.IsEmpty() )
	{
		if( d.posterior == L"lány" || d.posterior == L"leány" )
			d.sex_id = 2;
		if( d.posterior == L"fiú" )
			d.sex_id = 1;
	}
}
