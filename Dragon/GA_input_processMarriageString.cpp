#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// marriage: spouse [*birth] [+death] [comment]
//
// place, name és comment több szóból állhat
// place is tartmazhat utónevet ( N, Buda)
// comment bármit tartalmazhat: name, date, '(', ')'
// name is tartalmazhat '(' ')' jeleket

// Példák:
//
// Liptószentiváni Szent-Ivány Fruzsina
// N 1761.07.19 Pottornyai és Mezõcsáti Pottornyay Zsófia
// Bp 1867.10.21 Kápolnássy_(Kapeller) Kristóf 
// Rudelházi_(Ludrovai) Borbála 1445 özv 
//

// v_marriages.at(i).marriageSubstr tartalmazza a = után következõ substringet, ami a következõ =-ig vagy a sor végéig tart

//Árpád/2 *Kaposdada 1864.08.03 +Székesfehérvár 1940.06.22 hv huszár ezredes 
// =Kolozsvár 1908.11.08 Vajesdi Ajtay Jolán *Mezõdomb 1872.07.15 +Bp 1942.07.02 (Péter-Ágotha Eszter, 1f. Czóbel Béla, 2f. Windisch Jenõ)
void CGaInput::processMarriageSubstrings()
{
	if( v_marriages.size() == 0 ) return;

	int pos;
	CString cLine;
	CString spouseSubstr;
	CString peopleSubstr;
	CString relativesSubstr;
	CString parentsSubstr;
	CString moreSpouses;
	CString father;
	CString mother;
	CString arm;

	PLACE_DATE_BLOCK w;
	PEOPLE	s;

	int pos1;
	int pos2;

	for( UINT i = 0; i < v_marriages.size(); ++i )
	{
		cLine = v_marriages.at(i).marriageSubstr.Trim();

		// a végérõl leszedi a () substringeket   
		relativesSubstr.Empty();
		spouseSubstr = cLine;
		if( cLine.Right(1) == ')' )						// az utolsó karakter )
		{
			if( (pos = cLine.ReverseFind('(') ) != - 1 )
			{
				relativesSubstr = cLine.Mid( pos+1, cLine.GetLength() - pos - 2 );			// ( ... ) házastárs sokonainak blokkja
				v_marriages.at(i).relativesSubstr = relativesSubstr;			// ( ... ) házastárs sokonainak blokkja
				spouseSubstr = cLine.Left( pos );									// ha nincs szóköz a ( elõtt!!
				spouseSubstr.Trim();
			}
		}


// spouseSubstr-rõl le kell venni az esetleges esküvõ helyét és dátumát
		peopleSubstr = processWedding( spouseSubstr, &w );
		if( peopleSubstr.IsEmpty() )continue;			// elõfordul = 1848.03.15 minden további adat nélkül

		v_marriages.at(i).place = w.place;
		v_marriages.at(i).date	= w.date;

// a people blokk felbontása
		processPeopleStr( peopleSubstr, &s );
		v_marriages.at(i).sex_id		= checkSex( s.sex_id );
		v_marriages.at(i).title			= s.title;
		v_marriages.at(i).titolo		= s.titolo;
		v_marriages.at(i).last_name		= s.last_name;
		v_marriages.at(i).first_name	= s.first_name;
		v_marriages.at(i).posterior		= s.posterior;
		v_marriages.at(i).birth_place	= s.birth_place;
		v_marriages.at(i).birth_date	= s.birth_date;
		v_marriages.at(i).death_place	= s.death_place;
		v_marriages.at(i).death_date	= s.death_date;
		v_marriages.at(i).comment		= s.comment;


		if( relativesSubstr.IsEmpty() ) continue;

		// felbontások
		parentsSubstr.Empty();
		moreSpouses.Empty();
		if( !relativesSubstr.IsEmpty() )
		{
			if( (pos = relativesSubstr.Find( L"f." ) ) != -1 )
			{
				if( pos > 1 )
				{
					parentsSubstr = relativesSubstr.Left( pos-2 );
					parentsSubstr.Replace( ',', ' ' );
					parentsSubstr.Trim();
					moreSpouses = relativesSubstr.Mid( pos-1 );
				}
				else
					moreSpouses = relativesSubstr;
			}
			else
				parentsSubstr = relativesSubstr;

			if( ( pos = parentsSubstr.Find( L" fia" ) ) != -1 )
				parentsSubstr = parentsSubstr.Left( pos );
			else if( ( pos = parentsSubstr.Find( L" lánya" ) ) != -1 )
				parentsSubstr = parentsSubstr.Left( pos );

			v_marriages.at(i).parents		= parentsSubstr;
			v_marriages.at(i).moreSpouses	= moreSpouses;
		}



// házastárs szüleinek feldolgozása
		
		father.Empty();
		mother.Empty();
		if( ( pos = parentsSubstr.Find( '-' ) ) != -1 )					// szülõk szétválasztása
		{
//			v_marriages.at(i).father = parentsSubstr.Left( pos );
//			v_marriages.at(i).mother = parentsSubstr.Mid( pos + 1 );				// itt kellene tovább bontani a mothert, mert lehetnek adatai!! (164076 sor )
			father = parentsSubstr.Left( pos );
			mother = parentsSubstr.Mid( pos + 1 );				// itt kellene tovább bontani a mothert, mert lehetnek adatai!! (164076 sor )
		}	
		else														// a zárójelben csak apa és leírás van!!
		{
			str = getUntil( parentsSubstr, ' ' );
			if( isFirstName( str ) != - 1 )
				father = str;
		}
		
		if( !father.IsEmpty() )

		{
		str.Format( L"%s %s %s", v_marriages.at(i).titolo, v_marriages.at(i).last_name, father );
		str.Format( L"XXX %s",  father );
		str.Trim();
			
		PEOPLE	spf;
		processPeopleStr( str, &spf );

		v_marriages.at(i).sex_idF		= spf.sex_id;
		v_marriages.at(i).titleF		= spf.title;
		v_marriages.at(i).titoloF		= v_marriages.at(i).titolo;
		v_marriages.at(i).firstNameF	= spf.first_name;
		v_marriages.at(i).lastNameF		= v_marriages.at(i).last_name;
		v_marriages.at(i).posteriorF	= spf.posterior;
		v_marriages.at(i).birthPlaceF	= spf.birth_place;
		v_marriages.at(i).birthDateF	= spf.birth_date;
		v_marriages.at(i).deathPlaceF	= spf.death_place;
		v_marriages.at(i).deathDateF	= spf.death_date;
		v_marriages.at(i).commentF		= spf.comment;
		}

		if( !mother.IsEmpty() )
		{

		PEOPLE	spm;
		processPeopleStr( mother, &spm );
		v_marriages.at(i).sex_idM		= spm.sex_id;
		v_marriages.at(i).titleM		= spm.title;
		v_marriages.at(i).titoloM		= spm.titolo;
		v_marriages.at(i).firstNameM	= spm.first_name;
		v_marriages.at(i).lastNameM		= spm.last_name;
		v_marriages.at(i).posteriorM	= spm.posterior;
		v_marriages.at(i).birthPlaceM	= spm.birth_place;
		v_marriages.at(i).birthDateM	= spm.birth_date;
		v_marriages.at(i).deathPlaceM	= spm.death_place;
		v_marriages.at(i).deathDateM	= spm.death_date;
		v_marriages.at(i).commentM		= spm.comment;

		}
// házastárs további házastársainak feldolgozása
// 2f. Gaiger György, 3f. Nicky Sándor
		if( moreSpouses.IsEmpty() ) continue;


		SPOUSESPOUSES ss;
		std::vector<PEOPLE> v_p;
		v_spouseSpouses.clear();	
		v_p.clear();

		if( (pos = moreSpouses.Find( L"f." ) ) != -1 )
		{
			v_marriages.at(i).spouses = moreSpouses.Mid( pos -1 );
			processSpousesSpouses( v_marriages.at(i).spouses, &v_p );	//v_p-be házastársanként felbontja a stringet

			for( UINT j = 0; j < v_p.size(); ++j )
			{
				switch( v_p.at(j).sex_id )
				{
				case 0:
					if( v_marriages.at(i).sex_id == 1 )
						v_p.at(j).sex_id = 2;
					else if ( v_marriages.at(i).sex_id == 2 )
						v_p.at(j).sex_id = 1;
					break;
				case 1:
					if( v_marriages.at(i).sex_id == 0 )
						v_marriages.at(i).sex_id = 2;
					break;
				case 2:
					if( v_marriages.at(i).sex_id == 0 )
						v_marriages.at(i).sex_id = 1;
					break;
				}
				ss.sex_id		= v_p.at(j).sex_id;
				ss.title		= v_p.at(j).title;
				ss.titolo		= v_p.at(j).titolo;
				ss.last_name	= v_p.at(j).last_name;
				ss.first_name	= v_p.at(j).first_name;
				ss.posterior	= v_p.at(j).posterior;

				ss.birth_place	= v_p.at(j).birth_place;
				ss.birth_date	= v_p.at(j).birth_date;
				ss.death_place	= v_p.at(j).death_place;
				ss.death_date	= v_p.at(j).death_date;
				ss.comment		= v_p.at(j).comment;
				ss.order		= v_p.at(j).parentIndex;
				ss.spouseIndex	= i;
				v_spouseSpouses.push_back( ss );				// házastársak házastársai 

				
			}
			// minden házastársnak kiszámítja a házasság-sorszámot
			if( v_spouseSpouses.size() == 0 )	// ha a házastársnak nincsenek további házastársai, akkor neki ez az 1. házassága;
				v_marriages.at(i).orderSpouse = 1;
			else
			{
				v_marriages.at(i).orderSpouse = getSpouseOrder( &v_p );
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaInput::checkSex( int sex_id )
{
	int sexid = sex_id;
	// ha a leszármazott neme nem meghatározható, de a házastársé igen, akkor beállítjuk a leszármazott nemét
	if( d.sex_id == 0  && sex_id != 0 )
	{
		if( sex_id == 1 )
			d.sex_id = 2;
		else
			d.sex_id = 1;
	}
// ha a házastárs neme nem meghatározható, de a leszármazotté igen, akkor beállítjuk a házastárs nemét
	if( d.sex_id != 0 && sex_id == 0 )
	{
		if( d.sex_id == 1 )
			sexid = 2;
		else
			sexid = 1;
	}
// ha egyikük neme sem meghatározható, akkor a leszármazott legye férfi, a házastársa nõ
	else if( d.sex_id == 0 && sex_id == 0 )
	{
		d.sex_id = 1;				
		sexid = 2;
	}
	return sexid;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// [place] [date]
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaInput::processWedding( CString cLine, PLACE_DATE_BLOCK* pdb )
{
	CStringArray A;
	CString place;
	CString datum;
	CString peopleSubstr;
	CString word;

	bool volt = false;
	int ret;
	int i, j;

	cLine.Trim();
	int n = wordList( &A, cLine, ' ', FALSE );

	pdb->comment.Empty();
	pdb->date.Empty();
	pdb->place.Empty();
	// megkeresi a név és a comment elválasztó indexét, hogy a dátumot a commentben ne keresse!!
	for( i = 1 ; i < n; ++i )			// az elsõ keresztmevet keresi, de az lehet helyiség (pl. Gyula is, és lehet családnév is (pl. péter Zoltán
	{
		word = A[i];
		word.Remove( ',' );						// a név után lehet vesszõ !!
		word.Remove( '?' );
		word.Trim();
		if( ( ret = isFirstName( word ) ) != -1 )			//  =Budaörs 1944.12.17 Richard Rostoczil Mária
		{
														// hamis firsName kiszûrése
			if( i+1 < n && A[i+1] == L"és" ) continue;		// Márkus és Batiizfalvi....ez nem keresztnév!
			if( iswdigit( A[i-1].GetAt(0) ) ) continue;		// ha elõtte szám van, akkor nem fogadja el pl =1944.12.17 Gyula Kovács Katalin
			volt = true;									// ez már elfogadott keresztnév
			if( isLastCharacter( A[i], ',' ) )
			{
//				++i;		// i: elválasztó index!
				break;
			}

		}
		else												
		{
			if( volt ) break;								// keresztnevet követõ elsõ nem kersztnév. Elválasztó index!
		}
	}
	if( !volt )												// nem talált kersztnevet, baj van!!
	{
		if( fh4 != NULL && !cLine.IsEmpty() )									// Privát->Házastársak nem nyitja meg az fh4-et!!!
		{
			fwprintf( fh4, L"%6d<br>\n", m_lineNumber, cLine );
		}
		++m_error_cnt4;
		return L"";
	}
// megvan az i, ami a comment elválasztó indexe 

	//	[place] [date] name  vagy [date] name
	for( j = 0; j < i; ++j )						// j a 'date' és 'name' határa
	{
		ret = isDate( &A, j, &datum );
		if( ret )									// van datum!! és az ret szóból áll!
		{
			if( datum.Left(2) == L"kb" ) --j;		// a place 1 szóval kevesebb!  ( kb 1944)
			pdb->place	= packWords( &A, 0, j );	// dátum elõtt 'place'  (ha van)
            pdb->date	= datum;
 			peopleSubstr = packWords( &A, j+ret , i-ret-j );
			peopleSubstr = packWords( &A, j+ret , n-ret-j );
			return peopleSubstr;
		}
	}
	
	// [place] name vagy csak name
	for( j = 0; j < i; ++j )
	{
		if( isLastCharacter( A[j], ',' ) ) break;
	}
	if( j < i )			// place, name
	{
		pdb->place = packWords( &A, 0, j );
		peopleSubstr = packWords( &A, j+1, i - j -1 );
		peopleSubstr = packWords( &A, j+1, n-j-1 );
	}
	else					// name
	{
		peopleSubstr = packWords( &A, 0, n );
	}
	return peopleSubstr;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vp-ben a házastársa házastársai
//
//  1
//  2
// ->3

int CGaInput::getSpouseOrder( std::vector<PEOPLE>* vp )
{
	UINT	i;
	int		index;
	int		indexPrev = 0;


	for( i = 0; i < vp->size(); ++i )
	{
		index = vp->at(i).parentIndex;
		if( index != indexPrev + 1 )
		{
			if( index < indexPrev + 1 )
				return indexPrev + 1;				// a ferlsorolt xf. házastársak indexeiben lyuk van, ez lesz a kiemelt házastárs indexe
			else
				return index-1;
		}
		++indexPrev;
	}
	return indexPrev + 1;  // a felsorolt xf. házastársak indexe 1-tõl folyamatosa nõ, az õvé a következõ lesz
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 2f. Zahutrecky Éva 1771
// 3f. Mnisek Szaniszló 1602 néhai lengyel
// 2f.Gaiger György, 3f. Niczky Sándor	

// A spousese stringbõl a v_p vektorba írja át a házastárs házastársait a splitPeopleString segítségéval
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processSpousesSpouses( CString spouses,  std::vector<PEOPLE> *v_p )
{
	PEOPLE people;
	int		order;
	CString spouse;


	int pos;
	int pos2 = 0;
	int posComma = 0;

	v_p->clear();

	while( true )
	{
		if( (pos = spouses.Find( L"f.", pos2 ) ) == -1 )  break;	// a házastársdnak nincs több házastársa
		pos2 = pos + 1;
		order = _wtoi( spouses.Mid( pos-1, 1 ) );
		if( (posComma = spouses.Find( ',', posComma+1 ) ) != -1 )	// , is van, több házastárs is lesz
			spouse = spouses.Mid( pos+3, posComma - pos - 3 );
		else
			spouse = spouses.Mid( pos+3 );

		processPeopleString( 0,  spouse,  &people );
		people.parentIndex = order;   // a házastárs házastársának nincs megadva az anyja, ezért a parentIndex-et a házasság sorszámára használjuk
		v_p->push_back( people );
	};
}

// who: 1 házastárs apja
// who: 2 házastárs anyja
// who: 2 házastárs további házastársa
//
// cLine:		nameString [*birthString] [+deathstring]
// nameString:	[title][titoloString][lastName][firstNameString][comment]
// birthString: [place][date][comment]
// deathString: [place][date][comment]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processPeopleString( int who,  CString cLine, PEOPLE * p )
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
		processSpFatherName( nameString, &name );
	else
		processSpouseNameString( nameString, &name );

	comment += name.comment;
	comment += L" ";

	processPlaceDateComment( birthString, &b );
	comment += b.comment;
	comment += L" ";

	processPlaceDateComment( deathString, &d );
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
void CGaInput::processSpFatherName( CString nameStr, NAME* name ) 
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bemenet:
// 
// comment kezdetének jellemzõi:
// vagy kisbetûvel kezdõdik, ami nem title és nem 'de'
// vagy számmal (ami utónév után van )
// vagy egy , végzõdésû utónév utáni szóval


// Elsõ lépés a "name" és "comment" szétválasztása.
// Felbontja a nameComment stringet n szóra. (index = 0->n-1 )
// Keresi az esõ, nem 0 indexû helyen lévõ utónevet, és megnézi, hogy ezt utónév követi-e ( kettõs utónév ).
// Ennek az utónévnek az indexe az indexEndOfName.
// A 0 indexû utónevet vezetéknévnek értelmezi.

// void CGaInput::splitSName( CString namecomment, NAME* name, int SEX ) helyett
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processSpouseNameString( CString nameComment, NAME* name ) 
{
	CString word;
	CStringArray A;
	CString title;
	int n;
	int i = 0;
	int dbComment = 0;
	BOOL	volt = FALSE;

	
	nameComment.Trim();
	if( nameComment.GetLength() == 0 ) return;		// ha üres a sor, nincs semmi
	

	A.RemoveAll();
	clearName( name );

	// a név végét a keresztnevet követõ nemkeresztnév jelzi
	n = wordList( &A, nameComment, ' ', FALSE ); 

	// a sor elejérõl leszedi a title-t
	for( i = 0; i < n; ++i )
	{
		if( isTitle( A[i] ) )
		{
			title += A[i];
			title += L" ";
		}
		else
			break;
	}
	name->title = title.TrimRight();
	A.RemoveAt( 0, i );
	n -= i;

	// megkeresi a név végét, ami az elsõ keresztnevet követõ nem keresztnév vagy az utolsó szó
	for( i = 0; i < n; ++i )
	{
		word = A[i];
		word.Replace( ',', ' ' );		// ezt most tettem bele
		word.TrimRight();				// ezt most tettem bele
		if( isFirstName( word ) != -1)
		{ 
			if( i != 0 ) volt = TRUE;  // ha az elsõ szó keresztnév, akkor azt nem fogadja el. pl. Márkus és Batizfalfi Máriássy Judit
		}
		else
		{
			if( volt ) break;		// már volt keresztnév és ez már nem az
		}
	}
	if( i <= n )   // megtalálta a name végét
	{
		dbComment = n - i;
		name->fullname = packWords( &A, 0, i );
		if( dbComment )
		{
			name->comment = packWords( &A, i, dbComment );
			A.RemoveAt( i, dbComment );		// eldobja a comment-et
			n -= dbComment;
		}
		splitFullnameA( &A, name );
	}
	else
	{	// nincs name csak comment. Ilyen azért nem fordulhat elõ
		name->comment = nameComment;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// title nélküli fullname szétszedése
// Ghyczi, Assa és Ablánczkürti Ghyczy János
// [titolo] last_name first_name [first_name] 
void CGaInput::splitFullnameA( CStringArray* A, NAME* name ) 
{
	CString word;
	CString titolo(L"");
	CString first_name( L"" );
	CString last_name( L"" );

	int sex_id;
	int n	= A->GetCount();
	int i = 0;
	int pos;
	// Liptószentiváni Szent Ivány Júlia

	for( i = 0; i < n; ++i )
	{
		word = A->GetAt(i);
		if( !word.Compare( L"és" ) )
		{
			name->titolo = packWords( A, 0, i + 2 );
			++i;
			++i;
			break;
		}
	}
	if( i == n ) i = 0;  // nem volt 'x és y' titolo

	
	if( n > 2 )   // Chempernai Anchel ??
	{
		if( A->GetAt(0).Right(1) == 'i' && name->titolo.IsEmpty() )  // egy tagú elõnév
		{
			name->titolo = A->GetAt(0);
			i = 1;
		}
	
/*
		if( A->GetAt( 1 ) == L"és" )
		{
			titolo.Format( L"%s és %s", A->GetAt(0), A->GetAt( 2 ) );
			i = 3;
		}
		titolo.Trim();
		name->titolo = titolo;
*/
	}

	word = A->GetAt( i );		// a titolo utáni elsõ szó mindenképpen vezetéknév. Nem!! Szentmiklósi és Óvári br Pongrácz Eszter
	if( isTitle( word ) )
	{
		name->title = word;
	}
	else
	{
		last_name = word;
		last_name += L" ";
	}
	++i;
	if( n - i == 1 )		// kéttagú neveknél nem vizsgál
	{
		first_name = A->GetAt( i );
	}
	else					// többtagú neveknél nézi az  isFirstName-et
	{
		for( ; i < n; ++i )
		{
			word = A->GetAt( i );
			if( isFirstName( word ) == - 1 )
			{
				last_name += word;
				last_name += L" ";
			}
			else
			{
				first_name += word;
				first_name += L" ";
			}
		}
	}
	last_name.TrimRight();
	first_name.TrimRight();
	if( last_name.IsEmpty() )  // a családnév egyúttal utónév is!!
	{
		last_name	= getWord( first_name, 1, &pos );
		first_name	= dropFirstWord( first_name );
	}
	first_name.Replace( ',', ' ' );
	first_name.TrimRight();
	name->first_name	= first_name;
	name->last_name		= last_name;

	sex_id = getSexId( first_name );
	if( sex_id != -1 )
		name->sex_id = sex_id;
	else						// többtagú keresztnév 1. és 2. nevének neme különbözõ. first_name1 talán vezetéknév? 
	{
		name->last_name += L" ";
		name->last_name += getWord( first_name, 1, &pos );
		name->first_name = getWord( first_name, 2, &pos );
	}
}