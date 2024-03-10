#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// marriage: spouse [*birth] [+death] [comment]
//
// place, name �s comment t�bb sz�b�l �llhat
// place is tartmazhat ut�nevet ( N, Buda)
// comment b�rmit tartalmazhat: name, date, '(', ')'
// name is tartalmazhat '(' ')' jeleket

// P�ld�k:
//
// Lipt�szentiv�ni Szent-Iv�ny Fruzsina
// N 1761.07.19 Pottornyai �s Mez�cs�ti Pottornyay Zs�fia
// Bp 1867.10.21 K�poln�ssy_(Kapeller) Krist�f 
// Rudelh�zi_(Ludrovai) Borb�la 1445 �zv 
//

// v_marriages.at(i).marriageSubstr tartalmazza a = ut�n k�vetkez� substringet, ami a k�vetkez� =-ig vagy a sor v�g�ig tart

//�rp�d/2 *Kaposdada 1864.08.03 +Sz�kesfeh�rv�r 1940.06.22 hv husz�r ezredes 
// =Kolozsv�r 1908.11.08 Vajesdi Ajtay Jol�n *Mez�domb 1872.07.15 +Bp 1942.07.02 (P�ter-�gotha Eszter, 1f. Cz�bel B�la, 2f. Windisch Jen�)
// Gy�rgy/3 * Eperjes 1920.04.29 = Prince George, Alberta 1955.10.31 T�gl�si Nagy Ir�n *1929.01.04
void CInputGA::processMarriageSubstrings()
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

		// a v�g�r�l leszedi a () substringeket   
		relativesSubstr.Empty();
		spouseSubstr = cLine;
		if( cLine.Right(1) == ')' )						// az utols� karakter )
		{
			if( (pos = cLine.ReverseFind('(') ) != - 1 )
			{
				relativesSubstr = cLine.Mid( pos+1, cLine.GetLength() - pos - 2 );			// ( ... ) h�zast�rs sokonainak blokkja
				v_marriages.at(i).relativesSubstr = relativesSubstr;			// ( ... ) h�zast�rs sokonainak blokkja
				spouseSubstr = cLine.Left( pos );									// ha nincs sz�k�z a ( el�tt!!
				spouseSubstr.Trim();
			}
		}


// spouseSubstr-r�l le kell venni az esetleges esk�v� hely�t �s d�tum�t
		peopleSubstr = processWedding( spouseSubstr, &w );
//		if( peopleSubstr.IsEmpty() )continue;			// el�fordul = 1848.03.15 minden tov�bbi adat n�lk�l

		v_marriages.at(i).place = w.place;
		v_marriages.at(i).date	= w.date;

		processPeopleStr( peopleSubstr, &s );

		v_marriages.at(i).nameSubstr	= m_nameSubstr;
		v_marriages.at(i).birthSubstr	= m_birthSubstr;
		v_marriages.at(i).deathSubstr	= m_deathSubstr;
		v_marriages.at(i).sex_id		= checkSex( s.sex_id );
		v_marriages.at(i).peer			= s.peer;
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

		// felbont�sok
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
			else if( ( pos = parentsSubstr.Find( L" l�nya" ) ) != -1 )
				parentsSubstr = parentsSubstr.Left( pos );

			v_marriages.at(i).parents		= parentsSubstr;
			v_marriages.at(i).moreSpouses	= moreSpouses;
		}



// h�zast�rs sz�leinek feldolgoz�sa
		
		father.Empty();
		mother.Empty();
		if( ( pos = parentsSubstr.Find( '-' ) ) != -1 )					// sz�l�k sz�tv�laszt�sa
		{
			father = parentsSubstr.Left( pos );
			mother = parentsSubstr.Mid( pos + 1 );				// itt kellene tov�bb bontani a mothert, mert lehetnek adatai!! (164076 sor )
		}	
		else														// a z�r�jelben csak apa �s le�r�s van!!
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
			v_marriages.at(i).peerF			= spf.peer;
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
			v_marriages.at(i).peerM			= spm.peer;
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
// h�zast�rs tov�bbi h�zast�rsainak feldolgoz�sa

		// 2f. Gaiger Gy�rgy, 3f. Nicky S�ndor
		if( moreSpouses.IsEmpty() ) continue;


		SPOUSESPOUSES ss;
		std::vector<PEOPLE> v_p;
		v_spouseSpouses.clear();	
		v_p.clear();

		if( (pos = moreSpouses.Find( L"f." ) ) != -1 )
		{
			v_marriages.at(i).spouses = moreSpouses.Mid( pos -1 );
			processSpousesSpouses( v_marriages.at(i).spouses, &v_p );	//v_p-be h�zast�rsank�nt felbontja a stringet

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
				ss.peer			= v_p.at(j).peer;
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
				v_spouseSpouses.push_back( ss );				// h�zast�rsak h�zast�rsai 

				
			}
			// minden h�zast�rsnak kisz�m�tja a h�zass�g-sorsz�mot
			if( v_spouseSpouses.size() == 0 )	// ha a h�zast�rsnak nincsenek tov�bbi h�zast�rsai, akkor neki ez az 1. h�zass�ga;
				v_marriages.at(i).orderSpouse = 1;
			else
			{
				v_marriages.at(i).orderSpouse = getSpouseOrder( &v_p );
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CInputGA::checkSex( int sex_id )
{
	int sexid = sex_id;
	// ha a lesz�rmazott neme nem meghat�rozhat�, de a h�zast�rs� igen, akkor be�ll�tjuk a lesz�rmazott nem�t
	if( d.sex_id == 0  && sex_id != 0 )
	{
		if( sex_id == 1 )
			d.sex_id = 2;
		else
			d.sex_id = 1;
	}
// ha a h�zast�rs neme nem meghat�rozhat�, de a lesz�rmazott� igen, akkor be�ll�tjuk a h�zast�rs nem�t
	if( d.sex_id != 0 && sex_id == 0 )
	{
		if( d.sex_id == 1 )
			sexid = 2;
		else
			sexid = 1;
	}
// ha egyik�k neme sem meghat�rozhat�, akkor a lesz�rmazott legye f�rfi, a h�zast�rsa n�
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
// peopleSubstr  // ez a marasd�k a sorb�l
// 
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (=)Koros 1793.02.02 gr Tolvay J�lia *Nagyszombat 1768.08.03 +K�p�sd 1838.10.05 (Ferenc-P�terffy Anna)
// (=)Prince George, Alberta 1955.10.31 T�gl�si Nagy Ir�n * 1929.01.04
// (=) N 1761.07.19 Pottornyai �s Mez�cs�ti Pottornyay Zs�fia * Pottornya 1742.12.02 (Farkas - Kubinyi Anna_M�ria)
// (=) R 1763.09.17
// (=) Tesmeg, Hont m 1909.07.31 Folkush�zi Folkush�zy M�ria * 1890.01.10 + 1973.01.09
// (=) Pet�falvi Uzovics M�zes 1754 / 55 N�gr�d, Nyitra �s Pest vm 1745 husz�r kapit�ny, K�rolyi ezred
// (=) Drask�c 1731.07.15 ns �li�ssy P�l 1754 / 55 Nyitra vm, Komj�tr�l V�cra megy
// (=) N�gyesi Szepessy Kata 1724 �zv 1754 / 55 N�gr�d, Aba�j vm �zv
// (=) Vil�gossy G�sp�r szatm�ri lakos 1909 / 19 a Szatm�rmegyei Gazd Egyes�let titk�ra
CString CInputGA::processWedding2(CString cLine, PLACE_DATE_BLOCK* pdb)
{
	pdb->comment.Empty();  // ezt nem is keresi!!!
	pdb->date.Empty();
	pdb->place.Empty();
	if (cLine.IsEmpty()) return(cLine);


	CString peopleSubstr( L"N N");
/*
	CStringArray A;
	int n = wordList(&A, cLine, ' ', false);
	if (isdigit(A[0][0]))  // sz�mmal ( d�tummal kezd�dik, nincs place=   date people
	{
		pdb->date = A[0];
		peopleSubstr = packWords(&A, 1, n - 1);
		return peopleSubstr;
	}
	
	if (n == 1)
	{
		if (iswupper(A[0][0]))						// place
			pdb->place = A[0];
		else
			pdb->date = A[0];						// date
		return peopleSubstr;
	}
	if (n == 2)
	{
		if (iswupper(A[0][0]) && isdigit(A[1][0]))	// place date
		{
			pdb->place = A[0];
			pdb->date = A[1];
			return peopleSubstr;
		}
		if (iswupper(A[0][0]) && iswupper(A[1][0]))  // csalad nev
		{
			return cLine;
		}
	}

*/
	CString line = cLine;
	// hal�l �s sz�let�s lev�laszt�sa, hogy a d�tumok ne zavarodjanak meg
	int pos;
	if ((pos = line.Find('+')) != -1)
	{
		line = line.Left(pos-1);
	}
	if ((pos = line.Find('*')) != -1)
	{
		line = line.Left(pos-1);
	}

	CStringArray B;
	int m = wordList( &B, line, ' ', false );
	UINT i;


	if (isdigit(B[0][0]))  // sz�mmal ( d�tummal kezd�dik, nincs place=   date people
	{
		pdb->date = B[0];
		peopleSubstr = packWords(&B, 1, m - 1);
		return peopleSubstr;
	}
	else if (m == 1)
	{
		if (iswupper(B[0][0]))						// place
			pdb->place = B[0];
		else
			pdb->date = B[0];						// date
		return peopleSubstr;
	}
	else if (m == 2)
	{
		if (iswupper(B[0][0]) && isdigit(B[1][0]))	// place date
		{
			pdb->place = B[0];
			pdb->date = B[1];
			return peopleSubstr;
		}
		if (iswupper(B[0][0]) && iswupper(B[1][0]))  // csalad nev
		{
			return cLine;
		}
	}


	// (=) Vil�gossy G�sp�r szatm�ri lakos 1909 / 19 a Szatm�rmegyei Gazd Egyes�let titk�ra   hib�s lesz!!
	if (m > 6)
	{
		for (i = 0; i < m - 6; ++i)   // Place date el�n�v1 �s el�n�v2 csal�d n�v
		{
			if (!isdigit(B[i][0]) && isdigit(B[i + 1][0]) && iswupper(B[i + 2][0]) && !iswupper(B[i + 3][0]) && iswupper(B[i + 4][0]) && iswupper(B[i + 5][0]))   // nagybet� ut�n d�tum majd nagybet� , ez lehet a 'place' 'date'  = place date people
			{
				pdb->place = packWords(&B, 0, i + 1);
				pdb->date = B[i + 1];
				peopleSubstr = packWords(&B, i + 2, m - i - 2);
				return peopleSubstr;
			}
		}
	}

	if (m > 3)   // lehet benne "place date csal�d n�v"
	{
		for (i = 0; i < m - 3; ++i)
		{
			if (!isdigit(B[i][0]) && isdigit(B[i + 1][0]) && iswupper(B[i + 2][0]) && iswupper(B[i + 3][0]))   // nagybet� ut�n d�tum majd nagybet� , ez lehet a 'place' 'date'  = place date people
			{
				pdb->place = packWords(&B, 0, i + 1);
				pdb->date = B[i + 1];
				peopleSubstr = packWords(&B, i + 2, m - i - 2);
				return peopleSubstr;
			}
		}
	}

	if (m > 4)
	{
		for (i = 0; i < m - 4; ++i)   // Place date pre Csalad Nev 
		{
			if (!isdigit(B[i][0]) && isdigit(B[i + 1][0]) && !iswupper(B[i + 2][0]) && iswupper(B[i + 3][0]) && iswupper(B[i + 4][0]))   // nagybet� ut�n d�tum majd nagybet� , ez lehet a 'place' 'date'  = place date people
			{
				pdb->place = packWords(&B, 0, i + 1);
				pdb->date = B[i + 1];
				peopleSubstr = packWords(&B, i + 2, m - i - 2);
				return peopleSubstr;
			}
		}
	}

	
	

	// (=) Tesmeg, Hont m 1909.07.31 Folkush�zi Folkush�zy M�ria * 1890.01.10 + 1973.01.09

	// nincs place, data csak maga a h�zst�rs vagy =place date!!
	return cLine;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (=)Prince George, Alberta 1955.10.31 T�gl�si Nagy Ir�n * 1929.01.04
CString CInputGA::processWedding(CString cLine, PLACE_DATE_BLOCK* pdb)
{
	pdb->comment.Empty();
	pdb->date.Empty();
	pdb->place.Empty();

	if (cLine.IsEmpty()) return( cLine );

	CStringArray A;
	CStringArray B;
	CString place;
	CString datum;
	CString peopleSubstr;
	CString word;

	bool volt = false;
	int ret;
	int i, j;
	int pos;
	int pos1;

	cLine.Trim();
	int n = splitCString(cLine, ' ', false, &A);
/*
	// = place, date *************
	// = place, ******************
	// = place date **************
	// = place *******************
	// = date ********************
	// = *************************
	// az els� 2 vessz�s sor feldolgoz�sa
	if ((pos = cLine.Find('*')) != -1)  // ez az�rt kell, mert sok ilyen van: =Kov�cs K�zm�r, Debrecen
	{
		for (i = 0; i < n; ++i)
		{
			word = A[i];
			if (word.GetAt(word.GetLength() - 1) == ',' && i < 4)  // 4 sz�n�l csak nem t�bb a helys�g neve
			{
				if (i + 1 < n)
				{
					pdb->place = packWords(&A, 0, i + 1);
					if (iswdigit(A[i + 1][0]))  // k�vetkez� sz� a d�tum
					{
						pdb->date = A[i + 1];
						peopleSubstr = packWords(&A, i + 2, n - (i + 2));
					}
					else
					{
						peopleSubstr = packWords(&A, i + 1, n - (i + 1));
					}
					return peopleSubstr;
				}
				else  // csak egyetzlen sz� van 
				{
					peopleSubstr.Empty();
					if (iswdigit(A[i][0]))  // a sz� a d�tum
						pdb->date = A[0];
					else
						pdb->place = A[0];
					return peopleSubstr;
				}
			}
		}
		if (iswdigit(A[0][0]))				// date ********************
		{
			pdb->date = A[0];
			peopleSubstr = packWords(&A, 1, n - 1);
			return peopleSubstr;
		}
		if (n > 1)
		{
			if (iswdigit(A[1][0]))				// palce date ********************************
			{
				pdb->place = A[0];
				pdb->date = A[1];
				peopleSubstr = packWords(&A, 2, n - 2);
				return peopleSubstr;
			}
		}
		return cLine;						// ********************************
	}
*/
	bool kilep = false;
	// megkeresi a n�v �s a comment elv�laszt� index�t, hogy a d�tumot a commentben ne keresse!!
	for( i = 1 ; i < n; ++i )			// az els� keresztmevet keresi, de az lehet helyis�g (pl. Gyula is, �s lehet csal�dn�v is (pl. p�ter Zolt�n
	{
		word = A[i];
		word.Remove( ',' );						// a n�v ut�n lehet vessz� !!
		word.Remove( '?' );
		word.Trim();
		if( ( ret = theApp.isFirstName( word ) ) != -1 )			//  =Buda�rs 1944.12.17 Richard Rostoczil M�ria
		{
														// hamis firsName kisz�r�se
			if( i+1 < n && A[i+1] == L"�s" ) continue;		// M�rkus �s Batiizfalvi....ez nem keresztn�v!
			if( iswdigit( A[i-1].GetAt(0) ) ) continue;		// ha el�tte sz�m van, akkor nem fogadja el pl =1944.12.17 Gyula Kov�cs Katalin
			volt = true;									// ez m�r elfogadott keresztn�v
			if( isLastCharacter( A[i], ',' ) )
			{
//				++i;		// i: elv�laszt� index!
				break;
			}

		}
		else												
		{
			if( volt ) break;								// keresztnevet k�vet� els� nem kersztn�v. Elv�laszt� index!
		}
	}

	// megvan az i, ami a comment elv�laszt� indexe 

	//	[place] [date] name  vagy [date] name
	for( j = 0; j < i; ++j )						// j a 'date' �s 'name' hat�ra
	{
		ret = isDate( &A, j, &datum );
		if( ret )									// van datum!! �s az ret sz�b�l �ll!
		{
			if( datum.Left(2) == L"kb" ) --j;		// a place 1 sz�val kevesebb!  ( kb 1944)
			pdb->place	= packWords( &A, 0, j );	// d�tum el�tt 'place'  (ha van)
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
// vp-ben a h�zast�rsa h�zast�rsai
//
//  1
//  2
// ->3

int CInputGA::getSpouseOrder( std::vector<PEOPLE>* vp )
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
				return indexPrev + 1;				// a ferlsorolt xf. h�zast�rsak indexeiben lyuk van, ez lesz a kiemelt h�zast�rs indexe
			else
				return index-1;
		}
		++indexPrev;
	}
	return indexPrev + 1;  // a felsorolt xf. h�zast�rsak indexe 1-t�l folyamatosa n�, az �v� a k�vetkez� lesz
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// H�ZAST�RS TOV�BBI H�ZAST�RSAI

// 2f. Zahutrecky �va 1771
// 3f. Mnisek Szaniszl� 1602 n�hai lengyel
// 2f.Gaiger Gy�rgy, 3f. Niczky S�ndor	

// A spousese stringb�l a v_p vektorba �rja �t a h�zast�rs h�zast�rsait a splitPeopleString seg�ts�g�val
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processSpousesSpouses( CString spouses,  std::vector<PEOPLE> *v_p )
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
		if( (pos = spouses.Find( L"f.", pos2 ) ) == -1 )  break;	// a h�zast�rsdnak nincs t�bb h�zast�rsa
		pos2 = pos + 1;
 		order = _wtoi( spouses.Mid( pos-1, 1 ) );
		if( (posComma = spouses.Find( ',', posComma+1 ) ) != -1 )	// , is van, t�bb h�zast�rs is lesz
			spouse = spouses.Mid( pos+3, posComma - pos - 3 );
		else	
		{													// ha nincs vessz� a h�zast�rsak k�z�tt, csak az nf.
			spouse = spouses.Mid( pos+3 );
			if( (pos3 = spouse.Find( L"f." ) ) != -1 )
			{
				spouses = spouse.Mid( pos3 -1 );
				spouse = spouse.Left( pos3-2 );
				pos2 = 0;
				
			}
		}
		processPeopleStr( spouse,  &people );
		people.parentIndex = order; // a h�zast�rs h�zast�rs�nak nincs megadva az anyja, ez�rt a parentIndex-et a h�zass�g sorsz�m�ra haszn�ljuk
		v_p->push_back( people );
	};
}

