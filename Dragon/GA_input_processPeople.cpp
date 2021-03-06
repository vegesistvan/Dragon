#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ez a f�ggv�ny bontjha fel az 1-4. rang� bejegyz�sek szem�lyes adatait
// bejegyz�s
// 1. rang�: az eg�sz sor a gener�ci�s k�d n�lk�l, el� t�ve az XXXX dummy csal�dnevet
// 2. rang�: az esk�v� adatait�l megtiszt�tott substring
// 3. rang�: a teljes sor, apa est�n el� t�ve az XXX dummy csal�dnevet
// 4. rang�: a sor elej�n l�v� N. ut�ni substring
//
// �lym�don kaptt string egys�gesen kezelhet�.
// name[*birth] [+death] [comment]
//
// A sz�tszedett adatrokat az "any" strukt��r�ba teszi el. 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processPeopleStr( CString cLine,  PEOPLE* any )
{
	if( cLine.IsEmpty() ) return;

	PLACE_DATE_BLOCK bb;
	PLACE_DATE_BLOCK db;
	int		pos;

	m_nameSubstr.Empty();
	m_birthSubstr.Empty();
	m_deathSubstr.Empty();

	cLine.Trim();


	
 	if( (pos = cLine.Find('+') ) != - 1 )
	{
		m_deathSubstr	= cLine.Mid( pos + 1 );		// hal�loz�si blokk
		cLine = cLine.Left( pos );
	}
	if( (pos = cLine.Find('*') ) != - 1 )
	{
		m_birthSubstr = cLine.Mid( pos + 1);		// sz�let�si blokk
		cLine = cLine.Left( pos );
	}
/*
	CString twoWords;
	twoWords = getTwoWords( cLine );
	if( (pos = twoWords.Find( ',' ) ) != -1 )
	{
		m_nameSubstr = cLine.Left( cLine.Find( ',') );
		any->comment = cLine.Mid( cLine.Find( ',' ) + 1 );
		any->comment.Trim();
	}
	else
*/
	m_nameSubstr = cLine;
	processNameSubstr( any );
	processPlaceDateComment( m_birthSubstr, &bb );
	processPlaceDateComment( m_deathSubstr, &db );

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
// name [[,] [posterior][comment]] sz�tszed�se elemeire
// A name[[,] [comment] elv�laszt� index keres�se.
// Az elv�laszt� index biztos nagyobb mint 1, mert vezet�kn�vnek mindenk�ppen kell lenni
// A 'name' v�ge: 
// 'name,' vagy 
//	a balr�l-jobbra els� keresztn�v, amit nem keresztn�v k�vet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processNameSubstr( PEOPLE* any )
{
	if( m_nameSubstr.IsEmpty() ) return;

	CStringArray A;
	int i;
	int j;
	int z;
	int pos;
	CString word;
	CString fullName;
	CString comment;
	CString title;
	CString posterior;
	int parentIndex;
	int ret;
	int sex_id;
	bool volt = false;
	int numOfFirstNames = 0;


	int n = wordList(&A, m_nameSubstr, ' ', FALSE );
	for( i = 0; i < n; ++i )
	{
		word = A[i];
//		word.Replace( '?', ' ' );  // az egy sz�n bel�l a ? hely�n maradjon space, hogy a M�ria???Emese-f�le neveket kezeni tudja!!
		word.Trim();
		A[i] = word;
	}

	// parentIndex leszed�se, ha van
	for( i = 0; i < n; ++i )
	{
		word = A[i];
//		word.Remove( '?' );
		if( ( pos = word.Find( '/' ) ) != -1 )
		{
			str = word.Left( pos );
			if( ( ret = theApp.isFirstName( str ) ) != -1 )
			{
				parentIndex = _wtoi(word.Mid(pos+1));
				any->parentIndex = parentIndex;
				A[i] = str;
				break;
			}
		}
	}


	// [titolo][title][csal�dn�v][keresztn�v][posterior][le�r�s]

	// [le�r�s] azonos�t�sa
	if( m_nameSubstr == L"Pal�sthy Judit 1689" )
		z = 1;

	// megkeresi a n�v �s az esetleges comment elv�laszt� index�t, ami az utols� keresztnevet k�vet� nemkeresztn�vn�l van
 	for( i = 1 ; i < n; ++i )	// az els� keresztnevet keresi, de az lehet helyis�g (pl. Gyula is, �s lehet csal�dn�v is (pl. p�ter Zolt�n
	{
		word = A[i];
		word.Replace( ',', ' ' );						// a n�v ut�n lehet vessz� !!
		word.Replace( '?', ' ' );
		word.Remove( '(' );
		word.Remove( ')' );
		
		word.TrimRight();
		
		if( ( ret = theApp.isFirstName( word ) ) != -1 )			//  =Buda�rs 1944.12.17 Richard Rostoczil M�ria
		{
			sex_id = ret;									// az utols� keresztn�v sz�m�t. pl  Beh�rfalvi Urb�n Anna
															// hamis firsName kisz�r�se
			if( A[i-1].Right(1) == ',' ) continue;			// ha az el�z� sz� v�g�n , van  Mikor???
			if( i+1 < n && A[i+1] == L"�s" ) continue;		// M�rkus �s Batiizfalvi....ez nem keresztn�v!
			if( iswdigit( A[i-1].GetAt(0) ) ) continue;		// ha el�tte sz�m van, akkor nem fogadja el mi�rt????

			volt = true;									// ez m�r elfogadott keresztn�v, de m�g keres tov�bbiakat
			if( isLastCharacter( A[i], ',' ) )				// ha vessz� van, akkor az �ppen a nevet �s a commentet v�lasztja el
			{
				A[i].Replace( ',', ' ' );
				A[i].TrimRight();
				++i;
				break;										// i: elv�laszt� index!
			}												// ha nincs elv�laszt� index, akkor tov�bbi keresztnevet keres!!
		}
		else												
		{
			if( volt ) break;								// keresztnevet k�vet� els� nem kersztn�v. Elv�laszt� index!
		}
	}
	if( !volt )												// nem tal�lt kersztnevet, baj van!!
	{
		m_nameSubstr.Remove( '?' );
		m_nameSubstr.Trim();
		any->last_name = getFirstWord( m_nameSubstr );
		any->first_name = getLastWord( m_nameSubstr );
		return;
	}
	// i az elv�laszt� index a n�v �s comment( posterior) k�z�tt
	// megvan a  keresztn�v. Ha van ut�na valami, akkor az posterior, ha volt birth vagy death. He nem volt, akkor comment
	any->sex_id = sex_id;
	posterior = packWords( &A, i, n-i );
	posterior.Trim();
	if( !m_birthSubstr.IsEmpty() || !m_deathSubstr.IsEmpty() && !iswdigit( posterior.GetAt(0) ) )
		any->posterior = posterior;
	else
	{
		if( !posterior.IsEmpty() )
			any->comment = posterior;
	}


//// FULLNAME FELBONT�SA ///////////////////////////////////////////////////

	fullName = packWords(&A, 0, i );
	n = wordList(&A, fullName, ' ', FALSE );
	if( n < 2 ) return;
	i = 0;
// [title][el�n�v][title][csal�dn�v][keresztn�v][ut�tag]
// a title k�t helyen is el�fordulhat!!
// EL�NEVEK EL�TTI C�MEK AZONOS�T�SA
// kisbet�vel nem lehet megk�l�nb�ztetni a rangot, mert sz�mos idegen csal�dn�v kezd�dik kisbet�vel
// pl. di_, la_, von_, des_, del_

	for( i = 0; i < n; ++i )
	{
		if( iswlower( A[i].GetAt( 0 ) ) )
		{
			if( A[i].Find( '_' ) != -1 ) break;		// ezek elfordulhatnak idegen csal�dnevekben
			if( A[i].Find( '|' ) != -1 ) break;		// a csal�dnevekben el�fordul� '-t helyettes�ti a | karkter

			title += A[i];
			title += L" ";
		}
		else
			break;
	}
	any->title = title.TrimRight();

// x �s y EL�NEVEK AZONOS�T�SA

	if( i+1 < n && A[i+1] == L"�s" )					// X �s Y, ahol X nem i-re v�gz�dik
	{
		any->titolo = packWords( &A, i,  3 );
		i += 3;
	}
														// Giczi, Assa �s Abl�nck�rti Ghyczy Anna M�ria
	if( ((i + 2)  < n) && A[i+2] == L"�s" )
	{
		any->titolo = packWords( &A, i, 4 );
		i += 4;
	}

	
// Xi el�nevek azonos�t�sa	
	if( isLastCharacter( A[i], 'i' ) )					// i-re v�gz�d� egytag� el�nebek azonos�t�sa 
	{													// el�n�v k�vetkezhet, de lehet csal�dn�v is!!
		if( ( ret = theApp.isFirstName( A[i+1] )) != -1 )		// az ezt k�vet� sz� kereszn�v, ami lehet csal�dn�v is
		{
			if( i+2 < n )								// azzaz m�r csak 3 tag van h�tra, akkor ezek: titolo, lastname, firstname
			{
				any->titolo		= A[i];
				any->last_name	= A[i+1];
				any->first_name	= A[i+2];			
				return;
			}
			else if( i+1 < n )							// m�r csak 2 sz� van h�tra, akkor azok: lasstname, firstname
			{
				any->last_name = A[i];
				any->first_name = A[i+1];
				return;
			}
		}
		any->titolo = A[i];
		++i;
	}

// most m�r csak ilyen lehet: [title][csal�dn�v][keresztn�v][posterior]
// title azons�t�sa
// kisbet�vel nem lehet megk�l�nb�ztetni a rangot, mert sz�mos idegen csal�dn�v kezd�dik kisbet�vel
// pl. di_, la_, von_, des_, del_

// EL�NEVEK EL�TTI C�MEK AZONOS�T�SA
	title.Empty();
	for( ; i < n; ++i )
	{
		if( iswlower( A[i].GetAt( 0 ) ) )
		{
			if( A[i].Find( '_' ) != -1 ) break;		// ezek elfordulhatnak idegen csal�dnevekben
			if( A[i].Find( '|' ) != -1 ) break;		// a csal�dnevekben el�fordul� '-t helyettes�ti a | karkter

			title += A[i];
			title += L" ";
		}
		else
			break;
	}
	if( any->title.IsEmpty() )
		any->title = title.TrimRight();

	
	// a v�g�r�l leszedi az ut�tagot
	if( iswlower( A[n-1][0] ) )
	{
		any->posterior = A[n-1];
		--n;
	}


	// kett�s vezet�knevek kezel�se
	// i-n a csal�d- �s keresztenevek sorozata
	// a csal�dn�v is lehet kett�s �s a kerezstn�v is lehet kett�s, vagy t�bbsz�r�s

	for( int j = i+1; j < n; ++j )
	{
		word = A[j];
		if( ( ret = theApp.isFirstName( word ) ) != -1 )		// az ezt k�vet� sz� kereszn�v, ami lehet csal�dn�v is
			++numOfFirstNames;
	}
	int k = n - i;			// a szavak sz�ma
	any->first_name = packWords( &A, n-numOfFirstNames, numOfFirstNames );
	any->last_name	= packWords( &A, i, k - numOfFirstNames );
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
// Ha a place ut�n ',' van, akkor az ut�na �ll� sz�m m�r comment
// Ha tal�l numerikus sz�t, akkor az el�tte l�v� string a place, az ut�na l�v� a comment
// Ha nem tal�l numerikus sz�t, akkor a place-ben adja vissza az eg�sz stringet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processPlaceDateComment( CString placeDateComment, PLACE_DATE_BLOCK* ns )
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
	

	// megkeresi az els� numerikus sz�t
	for( i = 0; i < n; ++i )
	{
		ret = isDate( &A, i, &datum );
        if( ret )  // van datum!!
		{
			place	= packWords( &A, 0, i );			// d�tum el�tt 'place'  (ha van)
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
				if( A[i+ret] == L"�ves" || A[i+ret] == L"napos" )	// x �ves || x napos
				{
					ns->date.Empty();								// date nem lesz
					--i;											// comment: x �ves || x napos
				}
				ns->comment	+= packWords( &A, i + ret , n - (i + ret) );
			}
			return;
		}
	}


	// Nem tal�lt numerikus sz�t a placeDateComment stringben. Csak place[, comment] van!!!
	// Ha van comment, akkor vessz�nek kell lenni a place �s comment k�z�tt!!
	if( ( pos = placeDateComment.Find( ',' ) ) != -1 )
	{
		ns->place = placeDateComment.Left( pos );
		ns->comment = placeDateComment.Mid( pos + 2 );   // 2: mert , �s sz�k�z is van!
	}
	else
		ns->place = placeDateComment;   // nem tal�lt vessz�t, az eg�sz sor place
}
