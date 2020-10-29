#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "GA_input.h"
#include "utilities.h"



// who: 1 házastárs apja
// who: 2 házastárs anyja
// who: 2 házastárs további házastársa
//
// cLine:		nameString [*birthString] [+deathstring]
// nameString:	[title][titoloString][lastName][firstNameString][comment]
// birthString: [place][date][comment]
// deathString: [place][date][comment]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitPeopleString( int who,  CString cLine, PEOPLE * p )
{
	clearPeople( p );
	if( cLine.IsEmpty() ) return;

	CString nameString;
	CString birthString;
	CString deathString;

	CString comment(L"");

	NAME		name;
	PLACE_DATE_BLOCK b;
	PLACE_DATE_BLOCK d;

	int pos;

	
	nameString.Empty();

	birthString.Empty();
	deathString.Empty();

	if( (pos = cLine.Find('+') ) != - 1 )
	{
		deathString = cLine.Mid( pos + 1 );
		cLine = cLine.Left( pos );
	}
	if( (pos = cLine.Find('*') ) != - 1 )
	{
		birthString = cLine.Mid( pos + 1);
		cLine = cLine.Left( pos );
	}
	nameString = cLine;

	if( who == 1 )
		splitSpFatherName( nameString, &name );
	else
		splitSpouseNameString( nameString, &name );
//		splitSName( nameString, &name, who  );

	comment += name.comment;
	comment += L" ";

	splitPlaceDateComment( birthString, &b );
	comment += b.comment;
	comment += L" ";

	splitPlaceDateComment( deathString, &d );
	comment += d.comment;
	comment.Trim();

	p->sex_id		= name.sex_id;
	p->title		= name.title;
	p->titolo		= name.titolo;
	p->first_name	= name.first_name;
	p->last_name	= name.last_name;

	p->birth_place	= b.place;
	p->birth_date	= b.date;

	p->death_place	= d.place;
	p->death_date	= d.date;

	p->comment		= comment;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Csak keresztnév van, nincs title,titolo,last_name!!!
// nameStr: [keresztnév] [comment]
void CGaInput::splitSpFatherName( CString nameStr, NAME* name ) 
{
	CString word;
	CStringArray A;
	int n;
	int i = 0;
	CString last( L"" );
	CString first( L"" );
	CString comment( L"" );

	clearName( name );
	nameStr.Trim();
	if( nameStr.GetLength() == 0 ) return;		// ha üres a sor, nincs semmi

	n = wordList( &A, nameStr, ' ', FALSE );

	// hátulról az elsõ keresztnév keresése
	for( i = n - 1; i >= 0; --i )
	{
		word = A[i];
		word.Trim();
		if( isFirstName( word ) != - 1 ) break;
	}
	first = packWords( &A, 0, i+1 );  
	if( i < n -1 ) 	comment = packWords( &A, i+1, n - i - 1 );

	name->first_name	= first;
	name->comment		= comment;
	name->sex_id		= MAN;
}
