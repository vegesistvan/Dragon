#include "stdafx.h"
#include "Fa.h"
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
void CGaInput::splitMarriageSubstrings()
{
	if( v_marriages.size() == 0 ) return;


	CString marriage;
	CString place;
	CString date;
	CString spouse;
	CString birthSubstr;
	CString deathSubstr;
	CString brace;
	CString arm;

	CString parents;
	CString comment;
	CString spouse_spouses;

	PEOPLE	spf;
	SNAMEBLOCK snb;
	PLACE_DATE_BLOCK bs;
	PLACE_DATE_BLOCK ds;

	NAME		name;

	UINT	i;
	int		pos;
	int		pos1;
	int		pos2;
	int		pos3;
	int		pos4;
	CString rest;

	v_spouseSpouses.clear();
	for( i = 0; i < v_marriages.size(); ++i )
	{
		marriage = v_marriages.at(i).marriageSubstr;

		spouse_spouses.Empty();
		parents.Empty();
		place.Empty();
		date.Empty();
		comment.Empty();
		brace.Empty();;
		arm.Empty();
		rest.Empty();
		deathSubstr.Empty();
		birthSubstr.Empty();

		name.comment.Empty();
		name.first_name.Empty();
		name.fullname.Empty();
		name.last_name.Empty();
		name.mother_index = 0;
		name.sex_id	= 0;
		name.title.Empty();
		name.titolo.Empty();
// a végérõl leszedi a () és [] substringeket


		if( marriage.Right( 1 ) == ']' )  // leszedi az [ág]-at
		{
			if( ( pos1 = marriage.ReverseFind( '[' ) ) != -1 && ( pos2 = marriage.Find( ']') ) != -1 )
			{
				arm = marriage.Mid( pos1+1, pos2-pos1-1 );
				v_marriages.at(i).arm = arm;
				marriage = marriage.Left( pos1-1 );
			}
			else
			{
				fwprintf( fh1, L"%s !!!<br>", marriage );   // ez a hibajelzés nem ide való!!!
			}
		}

//		if( marriage.Right( 1 ) == ')' )	// leszedi a rokonokat
		if( (pos2 = marriage.ReverseFind( ')' ) ) != - 1 )  	// leszedi a rokonokat
		{
			if( (pos1 = marriage.Find( '(' ) ) != -1 && ( pos2 = marriage.ReverseFind( ')' ) ) != - 1 )
			{
				if( (pos3 =  marriage.ReverseFind( '(' )) != -1 && (pos4 =  marriage.Find( ')' ))!= -1 )
				{
					if( pos1 != pos3 && pos2 != pos4 )
					{
						if( pos3 < pos4 )												// (.....(...).... )
						{
							brace		= marriage.Mid( pos1+1, pos2-pos1-1 );
							marriage	= marriage.Left( pos1-1 );
						}
						else															//  (...).....(....)
						{
							brace		= marriage.Mid( pos3+1, pos2-pos3-1 );
							marriage	= marriage.Left( pos3-1 );
						}
					}
					else
					{
						brace		= marriage.Mid( pos1+1, pos2-pos1-1 );
						marriage	= marriage.Left( pos1-1 );
					}
				}
				else
				{
					brace		= marriage.Mid( pos1+1, pos2-pos1-1 );
					marriage	= marriage.Left( pos1-1 );
				}

				if( isValidBrace( brace ) )
					v_marriages.at(i).brace = brace;
				else
				{
					brace.Empty();
					marriage = v_marriages.at(i).marriageSubstr;
				}
			}
		}
// death substring leszedése		
		if( ( pos = marriage.Find( '+' ) ) != -1 )
		{
			deathSubstr	= marriage.Mid( pos + 1 );
			v_marriages.at(i).deathSubstr = deathSubstr;
			marriage		= marriage.Left( pos -1 );
		}
		if( ( pos = marriage.Find( '*' ) ) != -1 )
		{
			birthSubstr	= marriage.Mid( pos + 1 );
			v_marriages.at(i).birthSubstr = birthSubstr;
			marriage		= marriage.Left( pos -1 );
		}
		spouse = marriage;
		
		splitSpouseString( spouse, &snb );
		v_marriages.at(i).place		+= snb.place;
		v_marriages.at(i).date		= snb.date;
		v_marriages.at(i).spouseFullname	= snb.name;

		comment						+= snb.comment;
		comment						+= L" ";

		splitSpouseNameString( snb.name, &name );


		CString word;
		if( name.last_name.Left( 5  ) == L"elõtt" )
		{
			word.Format( L"%s elõtt ", v_marriages.at(i).date );
			v_marriages.at(i).date = word;

			word = name.first_name;
			name.last_name	= getWord( word, 1, &pos );	
			name.first_name = word.Mid( pos );
		}

		// ha a leszármazott neme nem meghatározható, de a házastársé igen, akkor beállítjuk a leszármazott nemét
		if( d.sex_id == 0  && name.sex_id != 0 )
		{
			if( name.sex_id == 1 )
				d.sex_id = 2;
			else
				d.sex_id = 1;
		}
// ha a házastárs neme nem meghatározható, de a leszármazotté igen, akkor beállítjuk a házastárs nemét
		if( d.sex_id != 0 && name.sex_id == 0 )
		{
			if( d.sex_id == 1 )
				name.sex_id	= 2;
			else
				name.sex_id	= 1;
		}
// ha egyikük neme sem meghatározható, akkor a leszármazott legye férfi, a házastársa nõ
		else if( d.sex_id == 0 && name.sex_id == 0 )
		{
			d.sex_id	= 1;				
			name.sex_id = 2;
		}
//		if( d.sex_id == name.sex_id )
//		{
//			fwprintf( fh1, L"%8dT %8dL <font color='blue'>%10s==%-15s(%d)</font> %s<br>", m_tableNumber, m_lineNumber, d.first_name, name.first_name, d.sex_id, m_cLine );
//			++m_error_cnt1;
//		}
		v_marriages.at(i).spouseFullname		= name.fullname;  // ezt most tettem ide
		v_marriages.at(i).sex_id		= name.sex_id;
		v_marriages.at(i).title			= name.title;
		v_marriages.at(i).titolo		= name.titolo;
		v_marriages.at(i).last_name		= name.last_name;
		v_marriages.at(i).first_name	= name.first_name;

		comment += name.comment;
		comment += L" ";

		splitPlaceDateComment( birthSubstr, &bs );
		v_marriages.at(i).birth_place	= bs.place;
		v_marriages.at(i).birth_date	= bs.date;
		comment += bs.comment;
		comment += L" ";

		splitPlaceDateComment( deathSubstr, &ds );
		v_marriages.at(i).death_place	= ds.place;
		v_marriages.at(i).death_date	= ds.date;
		comment += ds.comment;
		comment += L" ";
		v_marriages.at(i).comment			= comment.Trim();

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
		brace = v_marriages.at(i).spouse_spouses;
		if( (pos = brace.Find( L"f." ) ) != -1 )
		{
			v_marriages.at(i).spouses = brace.Mid( pos -1 );
		}

		SPOUSESPOUSES ss;
		std::vector<PEOPLE> v_p;

		v_p.clear();
		if( !v_marriages.at(i).spouses.IsEmpty() )				// vannak házastársak  ez üres!!!!!!
		{
			splitSpousesSpouses( v_marriages.at(i).spouses, &v_p );	//v_p-be házastársanként felbontja a stringet
			
			for( UINT j = 0; j < v_p.size(); ++j )
			{
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
				v_spouseSpouses.push_back( ss );
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitSpouseString( CString marriageString, SNAMEBLOCK *snb )
{
	snb->place.Empty();
	snb->date.Empty();
	snb->name.Empty();
	snb->comment.Empty();
	
	if( marriageString.IsEmpty() ) return;

	CStringArray A;
	CString datum;
	int	n;
	int	i;
	int	j;
	int	ret;
	BOOL volt1 = FALSE;
	CString name;
	int sex_id = -2;


	n = wordList( &A, marriageString, ' ', FALSE );

	// dátum kereséssel kellene kezdeni!!!!!

	// ha csak 1 szám van a stringben, akkor plac

// [place][date] name [[,] [comment]] szétszedése elemeire
// A name[[,] [comment] elválasztó index keresése.
// Az elválasztó index biztos nagyobb mint 1, mert vezetéknévnek mindenképpen kell lenni
// A 'name' vége: 
// 'name,' vagy 
//	a balról-jobbra elsõ keresztnév, amit nem keresztnév követ


	for( i = 1 ; i < n; ++i )			// az elsõ keresztmevet keresi, de az lehet helyiség (pl. Gyula is, és lehet családnév is (pl. péter Zoltán
	{
		name = A[i];
		name.Replace( ',', ' ' );						// a név után lehet vesszõ !!
		name.Replace( '?', ' ' );
		name.TrimRight();
		
		if( ( ret = isFirstName( name ) ) != -1 )			//  =Budaörs 1944.12.17 Richard Rostoczil Mária
		{
			sex_id = ret;		// az utolsó keresztnév számít. pl  Behárfalvi Urbán Anna
			if( A[i-1].Right(1) == ',' ) continue;					// ha az elõzõ szó végén , van
			if( i+1 < n && A[i+1] == L"és" ) continue;				// Márkus és Batiizfalvi....ez nem keresztnév!
			if( iswdigit( A[i-1].GetAt(0) ) ) continue;				// ha elõtte szám van, akkor nem fogadja el
//			if( i > 1 && !iswupper( A[i-1].GetAt(0) ) ) continue;		// csak az a keresztnevet fogadja el, ami elõtt nagybetûs szó áll
																		// kivéve: van_der_Schmidt Vilmos, ilyen sok van, tehát ez nem jó!!
//			snb->name	+= name;
//			snb->name	+= L" "; 
			volt1 = TRUE;
			if( isLastCharacter( A[i], ',' ) )
			{
				A[i].Replace( ',', ' ' );
				A[i].TrimRight();
				++i;
				break;									// name, : elválasztó index!
			}
		}
		else
		{
			if( volt1 ) break;							// keresztnevet követõ elsõ nem kersztnév. Elválasztó index!
		}
	}
	if( !volt1 )
	{
		if( fh4 != NULL )  // Privát->Házastársak nem nyitja meg az fh4-et!!!
		{
			fwprintf( fh4, L"%6d %s<br>\n", m_lineNumber, marriageString );
		}
		++m_error_cnt4;
		return;
	}

	if( i < n )   // megvan a  keresztnév. Ha van utána valami, akkor az comment
	{
		snb->comment = packWords( &A, i, n-i );
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
			snb->place	= packWords( &A, 0, j );	// dátum elõtt 'place'  (ha van)
			snb->date	= datum;
			snb->name	= packWords( &A, j + ret , i-j - ret );
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
			snb->place = packWords( &A, 0, j );
			snb->name  = packWords( &A, j+1, i - j -1 );
		}
		else					// name
		{
			snb->name = packWords( &A, 0, i );
		}
	}


	if( d.sex_id == sex_id || sex_id == -1 )
	{
		splitSpouseStringNew2( marriageString, snb );
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AZ elsõ dátumot keresi
void CGaInput::splitSpouseStringNew2( CString marriageString, SNAMEBLOCK *snb )
{
	snb->place.Empty();
	snb->date.Empty();
	snb->name.Empty();
	snb->comment.Empty();
	
	if( marriageString.IsEmpty() ) return;

	CStringArray A;
	CString datum;
	NAME name;
	CString str;
	CString namecomment;

	int	n;
	int	i;
	int db;
	int dbn;
	
	n = wordList( &A, marriageString, ' ', FALSE );
	for( i = 0; i < n; ++i )
	{
		if( !( db = isDate( &A, i, &datum ) ) ) continue; // nem dátum
		// Két eset lehetséges:
		// 1. vagy [place] date name [comment]		// dátum a házasságkötés ideje	
		// 2. vagy [place] name comment				// dátum a commentben!!
		// A dátum után van-e név-struktúrájú string?
		str = packWords( &A, i+db, n-i-1 );			// dátum utáni substring
		if( dbn = isName( str, &name ) )  // a str elején név teljes név van-e? dbn = a név szószáma  
		{
			if( db )
			{
				snb->date		= packWords( &A, i, db );
				snb->place		= packWords( &A, 0, i );
				snb->name		= name.fullname;
				snb->comment	= name.comment;
			}
			return;
		}
	}
// nincs dátum 
	snb->name = marriageString;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kérdés: a str névvel kezdõdik-e
// név struktúrák:
//
// 1. Kutyahegy és Libanyaki Kovács Erzsébet
// 2. Libanyaki Kovács Lajos
// 3. Kovács Lajos
//

// az elsõ szó elõtt állhat tite
// a keresztnév lehet többszörös
//
// visszadja a name-ben a teljes nevet és a nevet alkotó szavak számát
// 0-t ad vissza, ha nem talált név struktúrát
int CGaInput::isName( CString str, NAME* name )
{
	CStringArray A;
	int	i;
	int j;
	int	n;
	int	sex_id;
	CString title;
	CString titolo;
	CString last_name;
	CString first_name;
	CString fullname;
	CString miez;

	name->title.Empty();
	name->titolo.Empty();
	name->last_name.Empty();
	name->first_name.Empty();
	name->fullname.Empty();

	if( str.IsEmpty() ) return false;
	n = wordList( &A, str, ' ', FALSE );
	if( n < 2 ) return 0;					// a név legalább 2 szóból áll, nem lehet név

	i = 0;
	if( isTitle( A[i] ) )
	{
		title = A[i];						// title-t leszedi. Folytatódik, mintha ott se lenne
		++i;
	}
	if( !iswupper( A[i].GetAt(0) ) )		// az elsõ szónak mindenképpen nagybetûvel kell kezdõdni
		return 0;
	// A[i] nagybetûs szó

	if( (i + 1) < n && A[i+1] == L"és" && i+2 < n && iswupper( A[i+2].GetAt(0) ) && A[i+2].Right(1) == L"i" )
	{
		// 1. Kutyahegyi és Lóhegyi Nagy Barnabás Elek 
		titolo.Format( L"%s és %s", A[i], A[i+2] );
		i += 3;
		if( i < n && iswupper( A[i].GetAt(0) ) )
		{
			last_name = A[i];
			++i;
			while( i < n && isFirstName( A[i] ) != -1 )
			{
				first_name += A[i];
				first_name += L" ";
				++i;
			}
			first_name.TrimRight();
		}
	}
	else if( i < n &&  A[i].Right(1) ==  L"i" && i + 1 < n && iswupper( A[i+1].GetAt(0) ) )
	{
		// 2. Kutyahegyi Nagy Barnabás Elek
		titolo = A[i];
		++i;
		last_name = A[i];
		++i; 
		while( i < n && isFirstName( A[i] ) != - 1 )
		{
			first_name += A[i];
			first_name += L" ";
			++i;
		}
		first_name.TrimRight();
	}
	else if( i < n && iswupper( A[i].GetAt(0) )  )
	{
		// 3. Nagy Barnabás Elek				// utónév lehet többes
		// 3. Richard Rostoczil Mária			// családnév lehet többes
		last_name = A[i];						// a családnév után 2. családnév vagy keresztnév jön
		++i;
		miez = A[i];
		if( miez.Right(1) == ',' )
			miez = miez.Left( miez.GetLength() -1 );
		if( (sex_id = isFirstName( miez ) ) == -1 )		// nem keresztnév, tehát ez a 2. utónév
		{
			last_name += L" ";
			last_name += miez;
			++i;
		}

		for( j = i; j < n; ++j )
		{
			miez = A[i];
			if( miez.Right(1) == ',' )
				miez = miez.Left( miez.GetLength() -1 );

			if( (sex_id = isFirstName( miez ) ) != -1 )
			{
				first_name += miez;
				first_name += L" ";
				++i;
			}
			else
				break;
		}
		first_name.TrimRight();
		name->comment = packWords( &A, i, n-i );
		if( first_name.IsEmpty() )   // a dátum után nem név van, tehát elõtte kell keresni
			return 0;
	}
	else
		return 0;
	
	if( titolo.IsEmpty() )
		fullname.Format( L"%s %s %s", title, last_name, first_name );
	else
		fullname.Format( L"%s %s %s %s", title, titolo, last_name, first_name );
	fullname.Trim();
	name->title			= title;
	name->titolo		= titolo;
	name->last_name		= last_name;
	name->first_name	= first_name;
	name->fullname		= fullname;
	return i;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A str lehetséges struktúrái:
// 
// place, name comment
// name comment
// name
void CGaInput::noDate( CString marriageString, SNAMEBLOCK *snb )
{
	int pos;
	int n;
	int i;
	int sex_id;

	CStringArray A;

	str = marriageString;
	
	// place, name comment
	if( ( pos = marriageString.Find( ',' ) ) != -1 )			// van vesszõ
	{
		snb->place = str.Left( pos );
		str = str.Mid( pos + 1 );
	}
	// a második szótók keresem az elsõ olyan keresztnevet, ami után nem keresztnév következik
	
	n = wordList( &A, str, ' ', false );
	for( i = 1; i < n - 1; ++i )
	{
		if(  ( sex_id = isFirstName( A[i] ) ) != -1 &&  isFirstName( A[i+1] ) == -1 )
			break;
	}
	if( i == n-1 && ( sex_id = isFirstName( A[n] )) != -1 )
		i = n;  // az utolsó szó a keresztnév
	else
		AfxMessageBox( L"Nincs keresztnév a sorban!" );

	// i-edik szó a keresztnév

	
}
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// marriageString: [place] [date] name [[,][comment]]
//
// A felbontás kulcsa a 'name' keresztnevének megkeresése.
// A problémát az okozza, hogy a [place] is lehet keresztnév, és a 'comment' is tartalmazhat akár több keresztnevet is.
// Az ezt követõ substring a 'comment',
// Ha van dátum, akkor ez után kezdõdik a 'name', és emi elõtte van a 'place'
// Ha nincs dátum, akkor ',' választja el a 'place'-t a 'name'-tõl.
void CGaInput::splitSpouseStringNew( CString marriageString, SNAMEBLOCK *snb )
{
	snb->place.Empty();
	snb->date.Empty();
	snb->name.Empty();
	snb->comment.Empty();
	
	if( marriageString.IsEmpty() ) return;

	CStringArray A;
	CString datum;
	CString namecomment;
	int	n;
	int	i;
	int	ret;
	int pos;
	BOOL volt1 = FALSE;
	BOOL lastChar = FALSE;
	CString name;
	BOOL commentYes = TRUE;
	BOOL dateYes = TRUE;

	snb->comment.Empty();
	snb->date.Empty();
	snb->name.Empty();
	snb->place.Empty();

	n = wordList( &A, marriageString, ' ', FALSE );
	name = A[n-1];
	name.Replace( '?', ' ' );
	name.Replace( ',', ' ' );
	name.TrimRight();
	if( isFirstName( name ) != -1 )	// az utolsó szó keresztnév? Igen, nincs comment a végén. Az alábbiak lehetnek nagy valószínûséggel:
	{								// ez téves feltételezés, több comment is keresztnévre végzõdik.
									// name					Ez téves feltételezé
									// date name 
									// place, name
									// place date name
		for( i = 0; i < n; ++i )
		{
			ret = isDate( &A, i, &datum );
			if( ret ) break;
		}

		if( ret )									// van datum!!					ret = a date ennyi szóból áll:
		{											// date name
													// place date name
			if( datum.Left(2) == L"kb" ) --i;		// a place 1 szóval kevesebb!   i = a date indexe
			snb->place	= packWords( &A, 0, i );	// dátum elõtt 'place'  (ha van)
			snb->date	= datum;
			snb->name	= packWords( &A, i + ret , n - i - ret );
		}
		else										// nincs dátum
		{											// name
													// place, name
			if( ( pos = marriageString.ReverseFind( L',' ) ) != -1 )
			{										// place, name
				snb->place	= marriageString.Left( pos + 1 );
				snb->name	= marriageString.Mid( pos + 1 );
			}
			else
			{										// name
				snb->name = marriageString;
			}
		}
	}
/*
	else												// az utolsó szó nem keresztnév, Van comment  végén. Az alábbiak lehetnek
	{
														// name comment
														// date name comment
														// place date name comment
														// place, name comment
													
		for( i = 0; i < n; ++i )
		{
			ret = isDate( &A, i, &datum );
			if( ret ) break;
		}
		if( ret )
		{
			if( datum.Left(2) == L"kb" ) --i;		// a place 1 szóval kevesebb!   i = a date indexe
			snb->place	= packWords( &A, 0, i );	// dátum elõtt 'place'  (ha van)
			snb->date	= datum;
			namecomment	= packWords( &A, i + ret , n - i - ret );
			if( ( pos = namecomment.Find( L',' ) ) != -1 )
			{
				snb->name		= namecomment.Left( pos );
				snb->comment	= namecomment.Mid( pos+1 );
			}
		}
	}
}
*/

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
void CGaInput::splitSpouseNameString( CString nameComment, NAME* name ) 
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
		splitFullname( &A, name );
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
void CGaInput::splitFullname( CStringArray* A, NAME* name ) 
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 2f. Zahutrecky Éva 1771
// 3f. Mnisek Szaniszló 1602 néhai lengyel
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::splitSpousesSpouses( CString spouses,  std::vector<PEOPLE> *v_p )
{
	PEOPLE people;
	int		order;
	CString spouse;


	int pos;
	int pos2;


	v_p->clear();
	while( !spouses.IsEmpty() )
	{
		if( (pos = spouses.Find( L"f." ) ) != -1 )
		{
			order = _wtoi( spouses.Left(1) );
			people.mother_index = _wtoi( spouses.Left(1) );
			if( (pos2 = spouses.Find( L"f.", pos + 2 ) ) != -1 )
			{
				spouse = spouses.Mid( pos+3, pos2-pos-5 );
				spouses = spouses.Mid( pos2-1 );
			}
			else
			{
				spouse = spouses.Mid( 3 );
//				spouse.Replace( ')', ' ' );   // miért akartam ezt?
				spouse.Trim();
				spouses.Empty();
			}
			spouse.Replace( ',', ' ' );
			spouse.Trim();
		}
		else
		{
			order = 1;
			spouse = spouses;
		}
		spouse.Replace( '?', ' ' );
		spouse.Trim();

		splitPeopleString( 0,  spouse,  &people );
		people.mother_index = order;
		v_p->push_back( people );
		if( pos == - 1)
			break;;

	}
}
