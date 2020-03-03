#include "stdafx.h"
#include "fa.h"
#include "faDlg.h"
#include "GA_input.h"



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
void splitPlaceDateComment( CString placeDateComment, PLACE_DATE_BLOCK* ns )
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
/*
	for( i = 0; i < n; ++i )
	{
		word = A[i];
		if( (pos = word.Find( ',' ) ) != -1 )		// vesszõt talált.
		{
			A[i].Replace( ',', ' ' );
			A[i].TrimRight();
			ns->place = packWords( &A, 0, i+1 );
			ns->comment = packWords( &A, i+1, n-i-1 );
			return;
		}
	}
	// nem talált vesszõt, az egész sor place
	ns->place = placeDateComment;
*/
}

