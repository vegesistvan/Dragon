#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Egy tábla-fejet beolvas az m_tableHeader strukturába és azt a v_tableHeader vectorban gyûjti
//
int CGaInput::processTableHeader( CString cLine )
{
	splitTableHeader( cLine );
	v_tableHeader.push_back( m_tableHeader );

	return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLine: %%[%]nnnnnnn(, titolo) (tableNumber) [ág][törzs: mmmmmmmm ] 
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

// ezt azért tarjuk meg, mert ha htm-bõl készítünk táblázatot, akkor nem szükséges a V_tableHeader-be írni.
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

	clearTableHeader( &m_tableHeader);
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
	/*
	if( (pos = cLine.Find( '-' ) )  != -1 )						// - comment leszedése
	{
		comment = cLine.Mid( pos + 2 );
		cLine = cLine.Left( pos - 1 );
	}
	*/

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
/*
	if( ( pos1 = cLine.Find( '<' ) ) != -1 )   // az esetleges html formattálás levágása
	{
		if( ( pos2 = cLine.ReverseFind( '>' ) ) != -1 )
		{
			if( pos2 == cLine.GetLength() - 1 )
				cLine = cLine.Left( pos1 );
			else
			{
				str.Format( L"<> jelek a tábla fejlécen belül!!\n%s", cLine  );
				AfxMessageBox( str );
				exit(1);
			}
		}
	}
*/
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



	familyName = getWord( cLine, 1, &pos );

	if( ( pos = cLine.Find( ',' ) ) != -1 )
	{
		familyName.Replace( ',',' ' );
		familyName.Trim();
		titolo	= cLine.Mid( pos+1 );
		cLine	= cLine.Left( pos - 1 );
	}
	if( ( pos = cLine.Find( L"alias" ) ) != -1 )
	{
		alias = cLine.Mid( pos+6 );
	}


	m_tableHeader.familyName	= familyName;
	m_tableHeader.titolo		= titolo;
	m_tableHeader.tableRoman	= tableRoman;
	m_tableHeader.comment		= comment;
	m_tableHeader.joint			= joint;
	m_tableHeader.alias			= alias;

	m_familyName				= familyName;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
