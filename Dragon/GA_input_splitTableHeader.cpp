#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLine: %%[%]nnnnnnn(, titolo) (tableNumber) [ág][törzs: mmmmmmmm ] ///comment
//
// nnnnnnnnnnn: familyName
// 
// tableNumber: római szám [[] azt jelzi, hogy opcionális
// mmmmmmmmmmm: szöveg   ( tényleg [] jelek között van
// titolo:		ha a 
// meghatározandó: tableNumber és mmmmmmmm
// 
// TABLE.header			// == cLine );
// TABLE.percent		// %% vagy %%% 
// TABLE.familyName     // nnnn elsõ szava
// TABLE.titolo			// ha a familyName után veszzõ van, az comment a római számig vagy végig 	
// TABLE.number			// tableNumber
// TABLE.comment		// ha van tableNumber és van nmég utána valami
// TABLE.torzs			// mmmmmmmmmm			// rögtön az elején le kell venni
// TABLE.arm
// TABLE.knownAs		// a tableHeader-t követõ % knownAs sor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitTableHeader( CString cLine )  
{
	CString percent;
	CString familyName;
	CString alias;
	CString titolo(L"");
	CString tableRoman(L"");
	CString comment(L"");
	int		joint;
	CString str;
	int pos;

	int pos1;
	int	pos2;

	clearTableHeader();
	++m_tableNumber;

	m_tableHeader.tableHeader	= cLine;
	m_tableHeader.tableNumber	= m_tableNumber;
	m_tableHeader.lineNumber	= m_lineNumber;
	
	if( cLine.Left(3) == L"%%%" )
	{
		++m_familyNumber;
		m_tableHeader.percent = cLine.Left(3 );
	}
	else
		m_tableHeader.percent = cLine.Left(2);
	
	m_tableHeader.familyNumber	= m_familyNumber;


	cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
	cLine.Replace( '%', ' ' );
	cLine.Trim();
	
	if( ( pos = cLine.Find( L"nem kapcs" ) ) != -1  )
	{
		joint = 0;
		str = cLine.Mid( pos );
		str = dropFirstWord( str );
		str = dropFirstWord( str );
		str.Trim();
		if( !str.IsEmpty() )
			comment = str;
		cLine = cLine.Left( pos-1 );								// nem kapcs leszedése
	}
	else
		joint = 1;

	if( ( pos1 = cLine.Find( '<' ) ) != -1 )   // az esetleges html formattálás levágása
	{
		cLine = cLine.Left( pos1 );
	}

	// szögletes zárójelek tartalmának megõrzése és leszedése a cLine-ról
	while( ( pos1 = cLine.ReverseFind( '[' ) ) != -1 )			// [] zárójelek leszedése
	{
		if( ( pos2 = cLine.ReverseFind( ']' ) ) != -1 )
		{
			if( pos1 < pos2 )
			{
				str		= cLine.Mid( pos1+1, pos2-pos1-1 );
				cLine	= cLine.Left( pos1-1 );
				if( ( pos1 = str.Find( L"törzs:" ) )!= -1 )
				{
					m_tableHeader.torzs = str.Mid( 7 );
				}
				else
					m_tableHeader.arm = str;
			}
		}
		else
			break;
	}

	
	LPCTSTR valid = L"IVXabcde";
	LPCTSTR valid2 = L"IVX";
	CString subs;
	CString subs2;


	
	
	CStringArray A;
	int n = wordList( &A, cLine, ' ', TRUE );      // cLine [] zárójelek, nem kapcs és comment nélkül

	// római szám leszedése
	if( n > 1 )
	{
		str = A.GetAt( 1 );
		subs = str.SpanIncluding( valid );
		if( subs.GetLength() == str.GetLength() )
		{
			subs2 = subs.SpanIncluding( valid2 );
			if( !subs2.IsEmpty() )
			{
				tableRoman = str;
				cLine	= packWords( &A, 0, 1 );			// római szám elõtti str
//				comment	= packWords( &A, 2, n-2 );			// római szám utáni
			}
		}
	}



	if( (pos = cLine.Find( '/' ) )  != -1 )						// - comment leszedése
		cLine = cLine.Left( pos );




	familyName = getWord( cLine, 1, &pos );

	if( ( pos = cLine.Find( ',' ) ) != -1 )
	{
		familyName.Replace( ',',' ' );
		familyName.Trim();
		titolo	= cLine.Mid( pos+1 );
		cLine	= cLine.Left( pos );
	}
	if( ( pos = cLine.Find( L"alias" ) ) != -1 )
	{
		alias = cLine.Mid( pos+6 );
	}

	if( ( pos = titolo.Find( '/' ) ) != -1 )
		titolo = titolo.Left( pos );

	if( ( pos = titolo.Find( '(' ) ) != -1 )
	{
		if( pos == 0 )
			titolo = titolo.Mid( 1, titolo.GetLength() - 2 ); // (Benedicti) kiveszi belõle az elõnevet
		else
			titolo = titolo.Left( pos );
	}
	titolo.Trim();
	CString word;
	UINT i;
	n = wordList( &A, titolo, ' ', true );
	for( i = 0; i < n; ++i )
	{
		word = A[i];
		if( word == L"és" ) continue;
		if( word.Right(1) != 'i' ) break;
	}
	titolo = packWords( &A, 0, i );

	if( m_tableHeader.percent == "%%%" )
		m_titolo = titolo;

	m_familyName				= familyName.Trim();

	m_tableHeader.familyName	= m_familyName;
	m_tableHeader.titolo		= m_titolo;
	m_tableHeader.tableRoman	= tableRoman;
	m_tableHeader.comment		= comment.Trim();
	m_tableHeader.joint			= joint;
	m_tableHeader.alias			= alias;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::clearTableHeader()
{
	m_tableHeader.arm.Empty();
	m_tableHeader.comment.Empty();
	m_tableHeader.familyName.Empty();
	m_tableHeader.alias.Empty();
	m_tableHeader.familyNumber = 0;
	m_tableHeader.folyt.Empty();
	m_tableHeader.joint = 0;
	m_tableHeader.known_as.Empty();
	m_tableHeader.lineNumber = 0;
	m_tableHeader.percent.Empty();
	m_tableHeader.torzs.Empty();
	m_tableHeader.tableHeader.Empty();
	m_tableHeader.tableNumber = 0;
	m_tableHeader.tableRoman.Empty();
	m_tableHeader.titolo.Empty();
}