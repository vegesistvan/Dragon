#include "stdafx.h"
#include "Fa.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
#include "GA_input_parameters.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::getDescendant( CString cLine )
{
	int pos;
	if( (pos = cLine.Find( '=' )) == -1 ) return cLine;

	CString descendant;
	if( pos < 1 )
	{
		str.Format( L"Hibás sor: %s", cLine );
		AfxMessageBox( str );
		exit(0);
	}

	m_descendant	= cLine.Left( pos - 1 );	// az esetleges n= házasság sorszámot levágja
	m_descendant.TrimRight();					// ha volt házasság sorszám, akkor a spsce-t is levágja
//	if( m_descendant == "E Dorottya" )
//		int z = 1;
	return m_descendant;
}
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

////////////

	if( (pos = cLine.Find('+') ) != - 1 )
	{
		_descDeathSS	= cLine.Mid( pos + 1 );
		cLine = cLine.Left( pos - 1 );
	}
	if( (pos = cLine.Find('*') ) != - 1 )
	{
		_descBirthSS = cLine.Mid( pos + 1);
		cLine = cLine.Left( pos - 1 );
	}

	_descNameSS = cLine;


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


/*
	int last_index = theApp.v_mother_index.size() - 1;

	if( last_index >= 0 )
	{
		if( theApp.v_mother_index.at(last_index ).generation < generation )
			theApp.v_mother_index.pop_back();
	}
*/

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
// namestring: [title] first_name [first_name2][/n][,] [comment]
// Hibás felbontás csak akkor képzelhetõ el, ha a 'firstnames' ill 'titles' nyilvántartás hiányos.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitDescNameString( CString nameSubstring )
{
	CStringArray A;
	CString first_name(L"");
	int	sex_id = 0;

	CString word;
	int i = 0;
//	int j = 0;
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
			if( ( d.sex_id = isFirstName( str ) ) != - 1 )
			{
				first_name		+= str;
				d.first_name	= first_name;
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
	if( d.sex_id == 0 )
		d.sex_id = 1;
	first_name.Trim();
	d.first_name = first_name;

	if( first_name == L"Gergõ" )
		int z = 1;

	if( i < n )		// a név után 
	{
		if( word != L"de" )
		{
			if( isTitle( word ) )
			{
				d.title	= word;
				++i;
			}
		}
		if( n != 1 )
		{
			d.comment = packWords( &A, i, n-i );
		}
	}
}
