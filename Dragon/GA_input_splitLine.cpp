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

	m_cLine = cLine;									// meg�rzi a teljes sort esetleges k�s�bbi felhaszn�l�sra ( hibajelz�s )
	cLine	= getBranch( cLine );
	
	m_descendant = getDescendant( cLine );				// leszedi r�la a marrige-eket
	getMarriageSubstrings( cLine );						// marriage substringeket a v_marriages vektorba teszi

	processDescendantSubstring( m_descendant );			// d strukt�r�ba teszi az elemeket
	processMarriageSubstrings();						// a v_marriages vektor substringjeit felbontja, az �rt�kekekt mag�ba a vektorba teszi
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Leszedi a sor v�g�r�l az el�gaz�s jelz�s�t, amit megtiszt�tva a d.folyt v�ltoz�ba tesz;
// valmaint az m_tableHeader.folyt.ban is akkumul�lja
// A megtiszt�tott sort visszadja.
CString CGaInput::getBranch( CString cLine )
{
	int pos;
	CString folyt;

	if( ( pos = cLine.Find( L"%%% folyt" ) ) != -1 )
	{
		str		= cLine.Mid( pos + 10 );
		folyt	= getFirstWord( str );   // lehet a romanNumber ut�n m�g valami feljegyz�s!!
		d.folyt	= folyt;
		folyt += L",";
		m_tableHeader.folyt += folyt;
		cLine = cLine.Left( pos-1 );				// lehagyja a %%% jeleket
		
	}


	int pos1;
	int pos2;
	CString root;
	CString csalad;

	// ha a sor v�g�n [root] -et tal�l
	while( (pos2 = cLine.ReverseFind( ']' ) ) != -1 )
	{
		if( cLine.GetAt( cLine.GetLength() - 1 ) == ']' )		// a cLine v�g�n van
		{
			if( ( pos1 = cLine.ReverseFind( '[' ) ) != -1 )
			{
				if( pos1 < pos2 )
				{
					if( ( pos = cLine.Find( L"csal�d �se]" ) ) != -1  )
						d.gap	= 1;
					if( ( pos = cLine.Find( L"csal�d]" ) ) != -1 ||  ( pos = cLine.Find( L"csal�d �se]" ) ) != -1 )
					{
						root = cLine.Mid( pos1+1, pos2-pos1 - 1 );
						if( iswupper( root.GetAt( 0 ) ) )
						{
							csalad = getCsalad( root );
							d.csalad = csalad;
							csalad += L",";
							m_tableHeader.csalad += csalad;
						}
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
		m_descendant	= cLine.Left( pos - 1 );	// az esetleges n= h�zass�g sorsz�mot lev�gja
		m_descendant.TrimRight();					// ha volt h�zass�g sorsz�m, akkor a spsce-t is lev�gja
	}
	return m_descendant;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// marriageSubstr -ek bet�lt�se v_marriages vektorba
void CGaInput::getMarriageSubstrings( CString cLine )
{
	int order;
	int	pos;
	int posM = 0;
	int posNext;
	CString marriageSubstring;
	MARRIAGES	marriagesS;		// h�zass�g

	v_marriages.clear();
	while( ( pos = cLine.Find( '=', posM ) ) != -1 )
	{
		posM = pos + 1;
		order = _wtoi( cLine.Mid( pos-1, 1 ) );
		if( order == 0 ) order = 1;							// a h�zass�g sorsz�ma (ha nincs ki�rva, akkor 1 )
		marriagesS.order	= order;
		marriagesS.orderSpouse = order;  // itt 1 volt!!

		if( ( posNext = cLine.Find( '=', posM ) ) == -1 )	// nincs t�bb h�zass�g
			posNext = cLine.GetLength()+1;
	
		marriageSubstring = cLine.Mid( pos+1, posNext-pos-2 );
		marriageSubstring.Trim();
		if( marriageSubstring.IsEmpty() ) break;  // ez hib�s GA.html-n�l fordulhat el�

		marriagesS.marriageSubstr = marriageSubstring;
		v_marriages.push_back( marriagesS );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::clearAll()
{
	clearPeople( &d );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra
	clearPeople( &s );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra
	clearPeople( &sf );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra
	clearPeople( &sm );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra
	clearPeople( &ss );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra


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