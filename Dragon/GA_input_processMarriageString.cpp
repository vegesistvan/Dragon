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
void CGaInput::splitMarriageSubstrings2()
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

/*
		// a végérõl leszedi a [] substringeket Erre nincs szükség, mert korábban megtörténe
		if( cLine.Right( 1 ) == ']' )  // leszedi az [ág]-at
		{
			if( ( pos1 = cLine.ReverseFind( '[' ) ) != -1 && ( pos2 = cLine.Find( ']') ) != -1 )
			{
				arm = cLine.Mid( pos1+1, pos2-pos1-1 );
				v_marriages.at(i).arm = arm;
				cLine = cLine.Left( pos1-1 );
			}
			else
			{
				fwprintf( fh1, L"%s !!!<br>", cLine );   // ez a hibajelzés nem ide való!!!
			}
		}
*/		
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
		v_marriages.at(i).place = w.place;
		v_marriages.at(i).date	= w.date;

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
		if( !relativesSubstr.IsEmpty() )
		{
			moreSpouses.Empty();
			if( ( pos = relativesSubstr.Find( L"f." ) ) != -1 )
			{
				moreSpouses = relativesSubstr.Mid( pos - 1, relativesSubstr.GetLength() - pos + 1 );
				v_marriages.at(i).spousesSubstr = moreSpouses;
				relativesSubstr = relativesSubstr.Left( pos - 3 );
			}
			parentsSubstr = relativesSubstr;
		}


// házastárs szüleinek feldolgozása
		
		father.Empty();
		mother.Empty();
		if( ( pos = parentsSubstr.Find( wchar_t('-') ) ) != -1 )					// szülõk szétválasztása
		{
			father = parentsSubstr.Left( pos );
			mother = parentsSubstr.Mid( pos + 1 );				// itt kellene tovább bontani a mothert, mert lehetnek adatai!! (164076 sor )

		}	
		else														// a zárójelben csak apa és megjegyzés van!!
		{
			str = getUntil( parentsSubstr, ' ' );
			if( isFirstName( str ) != - 1 )
				father = str;
		}
		str.Format( L"%s %s %s", v_marriages.at(i).titolo, v_marriages.at(i).last_name, father );

		PEOPLE	spf;
		processPeopleStr( str, &spf );
		v_marriages.at(i).sex_idF		= spf.sex_id;
		v_marriages.at(i).titleF		= spf.title;
		v_marriages.at(i).titoloF		= spf.titolo;
		v_marriages.at(i).firstNameF	= spf.first_name;
		v_marriages.at(i).lastNameF		= spf.last_name;
		v_marriages.at(i).posteriorF	= spf.posterior;
		v_marriages.at(i).birthPlaceF	= spf.birth_place;
		v_marriages.at(i).birthDateF	= spf.birth_date;
		v_marriages.at(i).deathPlaceF	= spf.death_place;
		v_marriages.at(i).deathDateF	= spf.death_date;
		v_marriages.at(i).commentF		= spf.comment;

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
			splitSpousesSpouses( v_marriages.at(i).spouses, &v_p );	//v_p-be házastársanként felbontja a stringet

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

				ss.birth_place	= v_p.at(j).birth_place;
				ss.birth_date	= v_p.at(j).birth_date;
				ss.death_place	= v_p.at(j).death_place;
				ss.death_date	= v_p.at(j).death_date;
				ss.comment		= v_p.at(j).comment;
				ss.order		= v_p.at(j).mother_index;
				ss.spouseIndex	= i;
				v_spouseSpouses.push_back( ss );				// házastársak házastársai 

				
			}
			// minden házastársnak kiszámítja a házasság-sorszámot
			if( v_spouseSpouses.size() == 0 )	// ha a házastársnak nincsenek további házastársai, akkor neki ez az 1. házassága;
				v_marriages.at(i).orderSpouse = 1;
			else
			{
				v_marriages.at(i).orderSpouse = getOrderSpouse( &v_p );
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

	int n = wordList( &A, cLine, ' ', FALSE );

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
		if( fh4 != NULL )									// Privát->Házastársak nem nyitja meg az fh4-et!!!
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
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// [place][date] name [[,] [posterior][comment]] szétszedése elemeire
// A name[[,] [comment] elválasztó index keresése.
// Az elválasztó index biztos nagyobb mint 1, mert vezetéknévnek mindenképpen kell lenni
// A 'name' vége: 
// 'name,' vagy 
//	a balról-jobbra elsõ keresztnév, amit nem keresztnév követ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitNameSubstr( CString cLine, int ix )
{
	CStringArray A;
	int n;
	int i;
	int j;
	int z;
	CString name;
	CString datum;
	int ret;
	int sex_id;
	bool volt = false;


	if( cLine == L"Palásthy Judit 1689" )
		z = 1;
	n = wordList( &A, cLine, ' ', FALSE );

	// megkeresi a név és a comment elválasztó indexét
	for( i = 1 ; i < n; ++i )			// az elsõ keresztmevet keresi, de az lehet helyiség (pl. Gyula is, és lehet családnév is (pl. péter Zoltán
	{
		str = A[i];
		str.Replace( ',', ' ' );						// a név után lehet vesszõ !!
		str.Replace( '?', ' ' );
		str.TrimRight();
		
		if( ( ret = isFirstName( str ) ) != -1 )			//  =Budaörs 1944.12.17 Richard Rostoczil Mária
		{
			sex_id = ret;									// az utolsó keresztnév számít. pl  Behárfalvi Urbán Anna
															// hamis firsName kiszûrése
			if( A[i-1].Right(1) == ',' ) continue;			// ha az elõzõ szó végén , van  Mikor???
			if( i+1 < n && A[i+1] == L"és" ) continue;		// Márkus és Batiizfalvi....ez nem keresztnév!
			if( iswdigit( A[i-1].GetAt(0) ) ) continue;		// ha elõtte szám van, akkor nem fogadja el

			volt = true;									// ez már elfogadott keresztnév
			if( isLastCharacter( A[i], ',' ) )
			{
				A[i].Replace( ',', ' ' );
				A[i].TrimRight();
				++i;
				break;										// i: elválasztó index!
			}												// ha nincs elválasztó index, akkor további keresztnevet keres!!
		}
		else												
		{
			if( volt ) break;								// keresztnevet követõ elsõ nem kersztnév. Elválasztó index!
		}
	}
	if( !volt )												// nem talált kersztnevet, baj van!!
	{
		if( fh4 != NULL )									// Privát->Házastársak nem nyitja meg az fh4-et!!!
		{
			fwprintf( fh4, L"%6d<br>\n", m_lineNumber, cLine );
		}
		++m_error_cnt4;
		return;
	}

	if( i <= n )   // megvan a  keresztnév. Ha van utána valami, akkor az posterior vagy comment
	{
		for( ; i < n; ++i )
		{
			if( isTitle( A[i] ) )
			{
				v_marriages.at(ix).title += A[i];
			}
			else
				break;
		}

		v_marriages.at(ix).comment = packWords( &A, i, n-i );
		name = packWords(&A, 0, i );
		n = wordList(&A, name, ' ', FALSE );
	}

	//	[place] [date] name
	// ami i elõtt van az [place][date] name
	// Elölrõl keeesi van dátum ?
	for( j = 0; j < i; ++j )						// j a 'date' és 'name' határa
	{
		ret = isDate( &A, j, &datum );
		if( ret )									// van datum!! és az ret szóból áll!
		{
			if( datum.Left(2) == L"kb" ) --j;		// a place 1 szóval kevesebb!  ( kb 1944)
			v_marriages.at(ix).place	= packWords( &A, 0, j );	// dátum elõtt 'place'  (ha van)
 			v_marriages.at(ix).date	= datum;
			v_marriages.at(ix).fullname = packWords( &A, j + ret , i-j - ret );
			break;
		}
	}

	// [place] name
	if( j == i )		// nincs dátum, marad a [place,] name
	{
		for( j = 0; j < i; ++j )
		{
			if( isLastCharacter( A[j], ',' ) ) break;
		}
		if( j < i )			// place, name
		{
			v_marriages.at(ix).place = packWords( &A, 0, j );
			v_marriages.at(ix).fullname = packWords( &A, j+1, i - j -1 );
		}
		else					// name
		{
			v_marriages.at(ix).fullname = packWords( &A, 0, i );
		}
	}
	if( v_marriages.at(ix).fullname.IsEmpty() )
		z = 4;
	splitSpouseNameString( ix );
}
*/
/*
void CGaInput::splitRelativesSubstr( int ix )
{

// (brace) szétszedése parents és spouse_spouses-ra
		if( !brace.IsEmpty() )
		{
			if( (pos = brace.Find( L"f." ) ) != -1 )
			{
				if( pos > 1 )
				{
					parents = brace.Left( pos-2 );
					parents.Replace( ',', ' ' );
					parents.Trim();
					spouse_spouses = brace.Mid( pos-1 );
				}
				else
					spouse_spouses = brace;
			}
			else
				parents = brace;

			if( ( pos = parents.Find( L" fia" ) ) != -1 )
				parents = parents.Left( pos );
			else if( ( pos = parents.Find( L" lánya" ) ) != -1 )
				parents = parents.Left( pos );

			v_marriages.at(i).parents			= parents;
			v_marriages.at(i).spouse_spouses	= spouse_spouses;
		}

// házastárs szüleinek feldolgozása

		if( ( pos = parents.Find( wchar_t('-') ) ) != -1 || ( pos = parents.Find( wchar_t('–') ) ) !=  -1 )					// szülõk szétválasztása
		{
			v_marriages.at(i).father = parents.Left( pos );
			v_marriages.at(i).mother = parents.Mid( pos + 1 );				// itt kellene tovább bontani a mothert, mert lehetnek adatai!! (164076 sor )

		}	
		else														// a zárójelben csak apa és megjegyzés van!!
		{
			str = getUntil( parents, ' ' );
			if( isFirstName( str ) != - 1 )
				v_marriages.at(i).father = str;
		}

		splitPeopleString( 1, v_marriages.at(i).father, &spf );
		v_marriages.at(i).sex_idF		= 1; //spf.sex_id;
	
		if( !spf.first_name.IsEmpty() )						// van apa a () zárójelben?
		{
			if( spf.last_name.IsEmpty() )					// meg van ott adva a vezeték név?
			{												// nincs, akkor a gyerek vezeték nebvét veszi át
				v_marriages.at(i).titleF		= v_marriages.at(i).title;
				v_marriages.at(i).titoloF		= v_marriages.at(i).titolo;
				v_marriages.at(i).lastNameF		= v_marriages.at(i).last_name;
			}
			else									
			{												// igen, akkor ezt használja
				v_marriages.at(i).titoloF		= spf.titolo;
				v_marriages.at(i).titleF		= spf.title;
				v_marriages.at(i).lastNameF		= spf.last_name;
			}
		}

		v_marriages.at(i).firstNameF	= spf.first_name;
		v_marriages.at(i).birthPlaceF	= spf.birth_place;
		v_marriages.at(i).birthDateF	= spf.birth_date;
		v_marriages.at(i).deathPlaceF	= spf.death_place;
		v_marriages.at(i).deathDateF	= spf.death_date;
		v_marriages.at(i).commentF		= spf.comment;
		
		splitPeopleString( 2, v_marriages.at(i).mother, &spf );
		v_marriages.at(i).sex_idM		= 2;
		v_marriages.at(i).titleM		= spf.title;
		v_marriages.at(i).titoloM		= spf.titolo;
		v_marriages.at(i).lastNameM		= spf.last_name;
		v_marriages.at(i).firstNameM	= spf.first_name;
		v_marriages.at(i).birthPlaceM	= spf.birth_place;
		v_marriages.at(i).birthDateM	= spf.birth_date;
		v_marriages.at(i).deathPlaceM	= spf.death_place;
		v_marriages.at(i).deathDateM	= spf.death_date;
		v_marriages.at(i).commentM		= spf.comment;

// házastárs további házastársainak feldolgozása
// 2f. Gaiger György, 3f. Nicky Sándor
		brace = v_marriages.at(i).spouse_spouses;
		if( brace.IsEmpty() ) continue;


		SPOUSESPOUSES ss;
		std::vector<PEOPLE> v_p;
		v_spouseSpouses.clear();	
		v_p.clear();

		if( (pos = brace.Find( L"f." ) ) != -1 )
		{
			v_marriages.at(i).spouses = brace.Mid( pos -1 );
			splitSpousesSpouses( v_marriages.at(i).spouses, &v_p );	//v_p-be házastársanként felbontja a stringet

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

				ss.birth_place	= v_p.at(j).birth_place;
				ss.birth_date	= v_p.at(j).birth_date;
				ss.death_place	= v_p.at(j).death_place;
				ss.death_date	= v_p.at(j).death_date;
				ss.comment		= v_p.at(j).comment;
				ss.order		= v_p.at(j).mother_index;
				ss.spouseIndex	= i;
				v_spouseSpouses.push_back( ss );				// házastársak házastársai 

				
			}
			// minden házastársnak kiszámítja a házasság-sorszámot
			if( v_spouseSpouses.size() == 0 )	// ha a házastársnak nincsenek további házastársai, akkor neki ez az 1. házassága;
				v_marriages.at(i).orderSpouse = 1;
			else
			{
				v_marriages.at(i).orderSpouse = getOrderSpouse( &v_p );
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vp-ben a házastársa házastársai
//
//  1
//  2
// ->3

int CGaInput::getOrderSpouse( std::vector<PEOPLE>* vp )
{
	UINT	i;
	int		index;
	int		indexPrev = 0;


	for( i = 0; i < vp->size(); ++i )
	{
		index = vp->at(i).mother_index;
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
*/

