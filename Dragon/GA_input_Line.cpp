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
	cLine			= getBranch( cLine );				// a sor végén lévõ esetleges leágazást ás [...család]-ot leszedi és m_folyt-ba teszi;
	m_descendant	= splitLineToSubstrings( cLine );	// sort szétszedi descendantra és házasság-vektorokra
	splitDescendantSubstring( m_descendant );			// d struktúrába teszi az elemeket

	splitMarriageSubstrings();								// a v_marriages vektor substringjeit felbontja és visszateszi a vektorba

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

	// ezt már leszedte a splitDescendantSubstring
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
						if( isupper( root.GetAt( 0 ) ) )
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// marriageSubstr -ek betöltése v_marriages vektorba
CString CGaInput::splitLineToSubstrings( CString cLine )
{
	v_marriages.clear();
	CString descendant = getDescendant( cLine );

	int		pos;
	if( (pos = cLine.Find( '=' )) == -1 ) return descendant;
	cLine = cLine.Mid( pos - 1 );		// a házasságok, az elsõ n= jellel együtt ( ha nincs, akkor szóköz lesz )

	
	int			pos2;
	int			order;
	MARRIAGES	marriagesS;		// házasság

	do
	{
		str					= cLine.Left( 1 );
		order				= _wtoi( str );
		marriagesS.order	= order;
		cLine				= cLine.Mid( 2 );					// leszedi az n= karaktereket

		if( ( pos2 = cLine.Find( '=' ) ) != -1 )
		{
			marriagesS.marriageSubstr	= cLine.Left( pos2-1 );			// a vizsgálandó házasság
			cLine		= cLine.Mid( pos2-1 );					// a maradék házasságok
		}
		else
		{
			marriagesS.marriageSubstr = cLine;						// nincs több házasság, csak ez
			cLine.Empty();
		}
		marriagesS.marriageSubstr.Trim();
		v_marriages.push_back( marriagesS );
	}while( !cLine.IsEmpty() );

	return descendant;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// A %N jelet az elõzõ ember comment mezejéhez hozzáteszi
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::updatePreviousDescendant( CString cLine )
{
	CString known_as;
	int		pos;
	known_as = getWord( cLine, 2, &pos );

	m_command.Format( L"UPDATE people SET known_as='%s' WHERE rowid ='%s'", known_as, m_rowidLastDescendant );
	if( !theApp.execute( m_command ) ) return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// A %N jelet az elõzõ ember comment mezejéhez hozzáteszi
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::updatePreviousTable( CString cLine )
{
	m_command.Format( L"UPDATE tables SET known_as='%s' WHERE rowid ='%d'", cLine, m_rowid_table );
	if( !theApp.execute( m_command ) ) return;
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