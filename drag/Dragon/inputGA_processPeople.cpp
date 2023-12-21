#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"
CString getPreviousWord2(CString str, int pos);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ez a függvény bontjha fel az 1-4. rangú bejegyzések személyes adatait
// bejegyzés
// 1. rangú: az egész sor a generációs kód nélkül, elé téve az XXXX dummy családnevet
// 2. rangú: az esküvõ adataitól megtisztított substring
// 3. rangú: a teljes sor, apa estén elé téve az XXX dummy családnevet
// 4. rangú: a sor elején lévõ N. utáni substring
//
// Ílymódon kaptt string egységesen kezelhetõ.
// name[*birth] [+death] [comment]
//
// A szétszedett adatrokat az "any" struktõúrába teszi el. 
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processPeopleStr( CString cLine,  PEOPLE* any )
{
	cLine.Trim();
	if( cLine.IsEmpty() ) return;
	splitToSubstr(cLine);		// felbontás:m_nameSubstr, m_birthSubstr, m_deathSubstr, m_name, m_comment
	splitNameSubstr( m_nameSubstr );
	PLACE_DATE_BLOCK bb;
	PLACE_DATE_BLOCK db;

//	if( isdigit( m_comment.GetAt(0) ) )
//	if (!m_comment.IsEmpty())
//		person( m_name, any );
//	else
		person(m_nameSubstr, any);   // posteriort leszedje
	processPlaceDateComment( m_birthSubstr, &bb );
	processPlaceDateComment( m_deathSubstr, &db );

	any->comment = m_comment;
	any->nFather = 0;
	any->birth_place	= bb.place;
	any->birth_date		= bb.date;
 	if( !bb.comment.IsEmpty() )
		any->comment = bb.comment;

	any->death_place	= db.place;
	any->death_date		= db.date;
	if( !db.comment.IsEmpty() )
		any->comment = db.comment;

	if( any->peer.IsEmpty() )
		any->peer = setPeer( any );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::isPeer(CString p )
{
	if (p==L"gr"||p==L"br"||p==L"hg"||p==L"gróf"||p==L"báró"||p==L"herceg" )
		return true;
	return false;
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
void CInputGA::processPlaceDateComment( CString placeDateComment, PLACE_DATE_BLOCK* ns )
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

	int n = splitCString( placeDateComment, ' ', false, &A );
	

	// megkeresi az elsõ dátumot szót
	for( i = 0; i < n; ++i )
	{
		ret = isDate( &A, i, &datum );
		datum.Remove(',');
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a fõnemnesi címek 3 helyen vannak:
// 1. title
// 2. posterior
// 3. comment 2.,4.,6. szavában
CString CInputGA::setPeer(PEOPLE* p)
{
	CString str;
	CString peer;
	int pos;

	if (p->birth_date == L"1656.04.26")
		pos = 1;

	if (!p->comment.IsEmpty())
	{
		peer = getPeerFromComment(p->comment);
	}
	if (peer.IsEmpty())
	{
		peer = extractPeer(p->title);
	}
	if (peer.IsEmpty())
	{
		str = p->posterior;
		if (str == L"gr" || str == L"br" || str == L"hg" || str == L"gróf" || str == L"báró" || str == L"herceg")
		{
			peer = str;
			p->posterior.Empty();
		}
		str = p->comment;
		if (str == L"gr" || str == L"br" || str == L"hg" || str == L"gróf" || str == L"báró" || str == L"herceg")
		{
			peer = str;
			p->posterior.Empty();
		}
	}
	return peer;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processPlaceDateComment2(CString placeDateComment, PLACE_DATE_BLOCK* ns)
{
	ns->place.Empty();
	ns->date.Empty();
	ns->comment.Empty();

	if (placeDateComment.IsEmpty()) return;

	CStringArray A;
	CString word;
	CString date;
	CString datum;
	CString place;
	CString comment;
	int i;
	int k;
	int pos;
	int	ret = 1;

	int n = splitCString(placeDateComment, ' ', false, &A);

	if ( isdigit( A[0][0] ) && (ret = isDate(&A, 0, &datum)))	// place nincs, az ret db szó datum ( 1944/45 alakú számot is elfogad!!)
	{
		date = packWords(&A, 0, ret);
		comment = packWords(&A, ret, n - ret);
	}
	else if (  n > 1 &&  (A[0] == L"kb" && isdigit( A[1][0]) ) )  // place nincs. kb 1944
	{
		date.Format(L"%s %s", A[0], A[1]);
		comment = packWords(&A, 2, n - 2);
	}
	else  // place mindenképpen van, 
	{
		// megkeresi az elsõ dátumot szót
		for (i = 0; i < n; ++i)
		{
			if( (ret = isDate(&A, i, &datum)))  // dátum!!
			{
				place = packWords(&A, 0, i);			// dátum elõtt 'place'  (ha van)
				k = i + ret;
				if (k < n)
				{
					comment = packWords(&A, k, n - k);
				}
				if (place.Right(1) == ',')
				{
					place = place.Left(place.GetLength() - 1);
					ns->comment = datum;
					ns->comment += L" ";
				}
				else
				{
					ns->date = datum;
				}
				ns->place = place;
				if ((i + ret) < n)
				{
					if (A[i + ret] == L"éves" || A[i + ret] == L"napos")	// x éves || x napos
					{
						ns->date.Empty();								// date nem lesz
						--i;											// comment: x éves || x napos
					}
					ns->comment += packWords(&A, i + ret, n - (i + ret));
				}
				return;
			}
		}

		// Nem talált numerikus szót a placeDateComment stringben. Csak place[, comment] van!!!
		// Ha van comment, akkor vesszõnek kell lenni a place és comment között!!
		if ((pos = placeDateComment.Find(',')) != -1)
		{
			ns->place = placeDateComment.Left(pos);
			ns->comment = placeDateComment.Mid(pos + 2);   // 2: mert , és szóköz is van!
		}
		else
			ns->place = placeDateComment;   // nem talált vesszõt, az egész sor place
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::extractPeer(CString str)
{
	if (str == L"gr" || str == L"br" || str == L"hg" || str == L"gróf" || str == L"báró" || str == L"herceg")
	{
		return(str);
	}
	return L"";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::getPeerFromComment(CString str)
{
	int pos;
	CString peer;
	if (str.Find(L"herceg") != -1)
		peer = L"hg";
	else if (str.Find(L"gróf") != -1)
		peer = L"gr";
	else if (str.Find(L"báró") != -1)
		peer = L"br";

	if ((pos = str.Find(L".hg")) != -1)
	{
		peer = str.Mid(pos - 2, 5);  // lehet kétjegyû a hg sorszáma!!!
		peer.TrimLeft();
	}

	return peer;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::findPeer(CString comment, CString peer)
{
	int pos;
	CString date;

	if ((pos = comment.Find(peer)) != -1 && pos > 2)
	{
		date = getPreviousWord2(comment, pos - 2);
		if (isNumeric(date.Left(4)))
			return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getPreviousWord2(CString comment, int pos)
{
	TCHAR kar = comment[pos];
	CString word;
	if (pos < 1) return L"";
	while (kar != ' ' && pos > 0)
	{
		--pos;
		kar = comment[pos];
	}
	word = getWordFrom(comment, pos );
	return word;
}
