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
//		if( peopleSubstr.IsEmpty() )continue;			// elõfordul = 1848.03.15 minden további adat nélkül

		v_marriages.at(i).place = w.place;
		v_marriages.at(i).date	= w.date;

		processPeopleStr( peopleSubstr, &s );

		v_marriages.at(i).nameSubstr	= m_nameSubstr;
		v_marriages.at(i).birthSubstr	= m_birthSubstr;
		v_marriages.at(i).deathSubstr	= m_deathSubstr;
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
			father = parentsSubstr.Left( pos );
			mother = parentsSubstr.Mid( pos + 1 );				// itt kellene tovább bontani a mothert, mert lehetnek adatai!! (164076 sor )
		}	
		else														// a zárójelben csak apa és leírás van!!
		{
			str = getUntil( parentsSubstr, ' ' );
			if( theApp.isFirstName( str ) != - 1 )
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
// visszadja:
// pdb->place
// pdb->date
// peopleSubstr  // ez a marasdák a sorból
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

	bool kilep = false;
	// megkeresi a név és a comment elválasztó indexét, hogy a dátumot a commentben ne keresse!!
	for( i = 1 ; i < n; ++i )			// az elsõ keresztmevet keresi, de az lehet helyiség (pl. Gyula is, és lehet családnév is (pl. péter Zoltán
	{
		word = A[i];
		word.Remove( ',' );						// a név után lehet vesszõ !!
		word.Remove( '?' );
		word.Trim();
		if( ( ret = theApp.isFirstName( word ) ) != -1 )			//  =Budaörs 1944.12.17 Richard Rostoczil Mária
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
			if( peopleSubstr.IsEmpty() )
				peopleSubstr = L"N N";
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
	if( peopleSubstr.IsEmpty() )
		peopleSubstr = L"N N";
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
// HÁZASTÁRS TOVÁBBI HÁZASTÁRSAI

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
	int pos3;
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
		{													// ha nincs vesszõ a házastársak között, csak az nf.
			spouse = spouses.Mid( pos+3 );
			if( (pos3 = spouse.Find( L"f." ) ) != -1 )
			{
				spouses = spouse.Mid( pos3 -1 );
				spouse = spouse.Left( pos3-2 );
				pos2 = 0;
				
			}
		}
		processPeopleStr( spouse,  &people );
		people.parentIndex = order; // a házastárs házastársának nincs megadva az anyja, ezért a parentIndex-et a házasság sorszámára használjuk
		v_p->push_back( people );
	};
}

