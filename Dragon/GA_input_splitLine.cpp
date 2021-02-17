#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
#include "GAtoDB.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitLine( CString cLine)
{
	clearAll();

	m_cLine = cLine;									// megõrzi a teljes sort esetleges késõbbi felhasználásra ( hibajelzés )
	cLine	= getBranch( cLine );
	
	m_descendant = getDescendant( cLine );				// leszedi róla a marrige-eket
	getMarriageSubstrings( cLine );						// marriage substringeket a v_marriages vektorba teszi

	processDescendantSubstring( m_descendant );			// d struktúrába teszi az elemeket
	processMarriageSubstrings();						// a v_marriages vektor substringjeit felbontja, az értékekekt magába a vektorba teszi
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Leszedi a sor végérõl az elágazás jelzését, amit megtisztítva az 'm_folyt' változóba tesz;
// A megtisztított sort visszadja.
//
// lehetséges folyt végzõdések?
// )_%%%_folyt_I
// [%%% folyt II]
// folyt III
CString CGaInput::getBranch( CString cLine )
{
	int pos;
	CString tableRoman;
	CString ch;
	CString tmp = cLine.Right( 20 );
	CString folyt;
	CString root;

	m_folyt.Empty();
	if( ( pos = cLine.Find( L"folyt" ) ) != -1 )
	{
		tableRoman = cLine.Mid( pos + 6 );
		tableRoman.Replace( '.', ' ' );
		tableRoman.Replace( ',', ' ' );
		tableRoman.Replace( '?', ' ' );
		tableRoman.Trim();
		m_folyt = tableRoman;
		folyt += m_folyt+L",";
		if( v_tableHeader.size() > 0 )
			v_tableHeader.at( v_tableHeader.size() -1 ).folyt += folyt;
		cLine = cLine.Left( pos - 4 );				// lehagyja a %%% jeleket
		cLine.Trim();
	}
	
	int pos1;
	int pos2;

	
	while( (pos2 = cLine.ReverseFind( ']' ) ) != -1 )
	{
		if( cLine.GetAt( cLine.GetLength() - 1 ) == ']' )		// a cLine végén van
		{
			if( ( pos1 = cLine.ReverseFind( '[' ) ) != -1 )
			{
				if( pos1 < pos2 )
				{
					//root = cLine.Mid( pos1+1, pos2-pos1 - 1 );
					if( ( pos = cLine.Find( L"család õse]" ) ) != -1  )
						d.gap	= 1;
					if( ( pos = cLine.Find( L"család]" ) ) != -1 ||  ( pos = cLine.Find( L"család õse]" ) ) != -1 )
					{
						root = cLine.Mid( pos1+1, pos2-pos1 - 1 );
						if( iswupper( root.GetAt( 0 ) ) )
							d.csalad = getCsalad( root );
					}
					else
					{
						root = cLine.Mid( pos1+1, pos2-pos1 - 1 );
						d.arm = root;
					}
				}
			}
			cLine = cLine.Left( pos1 - 1 );
		}
		else
			break;
	}
	
	return cLine;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::getCsalad( CString root )
{
	CString csalad;
	CString first;
	CString roman;
	CStringArray A;
	int n;

	n = wordList( &A, root, ' ', FALSE );
	csalad = A[0];
	n = csalad.GetLength();
	if( csalad.GetAt( n-1 ) == ',' )
		csalad = csalad.Left( n-1 );

	if( isRoman( A[1] ) )
	{
		csalad += L" ";
		csalad += A[1];
	}
	return csalad;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::getDescendant( CString cLine )
{
	int pos;

	m_descendant = cLine;
	if( (pos= cLine.Find( '=' ) ) != -1 )
	{
		m_descendant	= cLine.Left( pos - 1 );	// az esetleges n= házasság sorszámot levágja
		m_descendant.TrimRight();					// ha volt házasság sorszám, akkor a spsce-t is levágja
	}
	return m_descendant;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// marriageSubstr -ek betöltése v_marriages vektorba
void CGaInput::getMarriageSubstrings( CString cLine )
{
	int order;
	int	pos;
	int posM = 0;
	int posNext;
	CString marriageSubstring;
	MARRIAGES	marriagesS;		// házasság

	v_marriages.clear();
	while( ( pos = cLine.Find( '=', posM ) ) != -1 )
	{
		posM = pos + 1;
		order = _wtoi( cLine.Mid( pos-1, 1 ) );
		if( order == 0 ) order = 1;							// a házasság sorszáma (ha nincs kiírva, akkor 1 )
		marriagesS.order	= order;
		marriagesS.orderSpouse = order;  // itt 1 volt!!

		if( ( posNext = cLine.Find( '=', posM ) ) == -1 )	// nincs több házasság
			posNext = cLine.GetLength()+1;
	
		marriageSubstring = cLine.Mid( pos+1, posNext-pos-2 );
		marriageSubstring.Trim();
		if( marriageSubstring.IsEmpty() ) break;  // ez hibás GA.html-nél fordulhat elõ

		marriagesS.marriageSubstr = marriageSubstring;
		v_marriages.push_back( marriagesS );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::clearAll()
{
	clearPeople( &d );		// az insertáláshoz használt PEOPLE struktúra
	clearPeople( &s );		// az insertáláshoz használt PEOPLE struktúra
	clearPeople( &sf );		// az insertáláshoz használt PEOPLE struktúra
	clearPeople( &sm );		// az insertáláshoz használt PEOPLE struktúra
	clearPeople( &ss );		// az insertáláshoz használt PEOPLE struktúra

	_descNameSS.Empty();
	_descBirthSS.Empty();
	_descDeathSS.Empty();
	_descCommentSS.Empty();

	v_marriages.clear(); 
	v_spouseSpouses.clear();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::clearPeople( PEOPLE *p)
{
	p->generation  = L'';
	p->birth_date.Empty();
	p->birth_place.Empty();
	p->comment.Empty();
	p->death_date.Empty();
	p->death_place.Empty();
	p->father_id = L"0";
	p->first_name.Empty();
	p->last_name.Empty();
	p->known_as.Empty();
	p->mother_id = L"0";
	p->rowid.Empty();
	p->sex_id = 0;
	p->title.Empty();
	p->titolo.Empty();
	p->posterior.Empty();
	p->parentIndex = 0;
	p->parentIndexCalc = 0;
	p->tableAncestry = 0;
	p->tableNumber.Empty();
	p->lineNumber = 0;
	p->tableRoman.Empty();
	p->orderFather = 1;
	p->orderMother = 1;
	p->folyt.Empty();
	p->arm.Empty();
	p->csalad.Empty();
	p->gap = 0;
}