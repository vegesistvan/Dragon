#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"
CString getPreviousWord2(CString str, int pos);
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
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processSpouseSubstrings()
{
	if (v_marriages.size() == 0) return;

	CString spouseSubstr;

	for (UINT i = 0; i < v_marriages.size(); ++i)
	{
		s = { 0 };
		clearPeople(&s);
		spouseSubstr = cutParentsSposuses(i);	// h�zast�rs sz�leinek lev�laszt�sa (v_marriges->spouseSubstr �s v_marriges->relativeSubstr
		m_weddingNameSubstr = cutBirthDeath(spouseSubstr);
	
		// spouseSubstr-r�l le kell venni az esetleges esk�v� hely�t �s d�tum�t
		m_nameSubstr = processWedding(i);  
		splitSubstrs(&s);   

		v_marriages.at(i).nameSubstr = m_nameSubstr;
		v_marriages.at(i).birthSubstr = m_birthSubstr;
		v_marriages.at(i).deathSubstr = m_deathSubstr;
		v_marriages.at(i).sex_id = checkSex(s.sex_id);
		v_marriages.at(i).peer = s.peer;
		v_marriages.at(i).title = s.title;
		v_marriages.at(i).titolo = s.titolo;
		v_marriages.at(i).last_name = s.last_name;
		v_marriages.at(i).first_name = s.first_name;
		v_marriages.at(i).posterior = s.posterior;
		v_marriages.at(i).birth_place = s.birth_place;
		v_marriages.at(i).birth_date = s.birth_date;
		v_marriages.at(i).death_place = s.death_place;
		v_marriages.at(i).death_date = s.death_date;
		v_marriages.at(i).comment = s.comment;

		if (d.sex_id == MAN)
			v_marriages.at(i).whichWife = v_marriages.at(i).orderSpouse;
		else
			v_marriages.at(i).whichHusband = v_marriages.at(i).orderSpouse; 

		processRelatives(i);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////4
// Az i. h�zass�g felbont�se
void CInputGA::processRelatives(int i)
{
	CString parentsSubstr;
	CString moreSpouses;
	CString relativesSubstr;
	CString father;
	CString mother;
	CString sex;
	int pos;


	relativesSubstr = v_marriages.at(i).relativesSubstr;
	/*
	if (relativesSubstr.IsEmpty())
	{
	//	v_marriages.at(i).orderSpouse = 1;  // ha nincs megadva tov�bbi h�zast�rs, akkor az 1. h�zass�g
		return;
	}
	*/
	// felbont�sok

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
		sex = theApp.isFirstName(str);
		if( !sex.IsEmpty())
			father = str;
	}
	int z;
	CString sex1;
	CString sex2;
	if( !father.IsEmpty() )
	{
		str.Format( L"%s %s %s", v_marriages.at(i).titolo, v_marriages.at(i).last_name, father );
//		str.Format( L"XXX %s",  father );
		str.Trim();
			
		PEOPLE	spf;
		clearPeople(&spf);
		m_nameSubstr = str;
		m_nameSubstr = cutBirthDeath(str);
		splitSubstrs( &spf );
		sex1 = spf.sex_id;
		v_marriages.at(i).sex_idF		= spf.sex_id;
		v_marriages.at(i).peerF			= spf.peer;
		
		//if( spf.peer.IsEmpty())
		//	v_marriages.at(i).peerF		= v_marriages.at(i).peer;	// gyerek f�nemesi c�m�t �tveszi? Ez abszurd
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
		clearPeople(&spm);

		m_nameSubstr = cutBirthDeath(mother);
		splitSubstrs( &spm );
		sex1 = spm.sex_id;
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
	

	checkSexCouple(&sex1, &sex2);
	v_marriages.at(i).sex_idF = sex1;
	v_marriages.at(i).sex_idM = sex2;

// h�zast�rs tov�bbi h�zast�rsainak feldolgoz�sa

// 2f. Gaiger Gy�rgy, 3f. Nicky S�ndor
	if (moreSpouses.IsEmpty()) return;


	SPOUSESPOUSES ss;
	std::vector<PEOPLE> v_p;
	CString order;
	v_p.clear();

	if ((pos = moreSpouses.Find(L"f.")) != -1)
	{
		v_marriages.at(i).spouses = moreSpouses.Mid(pos - 1);
//		processSpousesSpouses(v_marriages.at(i).spouses, &v_p);	//v_p-be h�zast�rsank�nt felbontja a stringet
		processSpousesSpouses( i, &v_p);	//v_p-be h�zast�rsank�nt felbontja a stringet

		for (UINT j = 0; j < v_p.size(); ++j)
		{
			
	
			if (v_p.at(j).sex_id.IsEmpty() )
			{
				if (v_marriages.at(i).sex_id == MAN)
					v_p.at(j).sex_id = WOMAN;
				else if (v_marriages.at(i).sex_id == WOMAN)
					v_p.at(j).sex_id = MAN;
			}
			else if (v_p.at(j).sex_id == MAN)
			{
				if (v_marriages.at(i).sex_id.IsEmpty())
					v_marriages.at(i).sex_id = WOMAN;
			}
			else
			{
				if (v_marriages.at(i).sex_id.IsEmpty())
					v_marriages.at(i).sex_id = MAN;
			}

			sex1 = v_marriages.at(i).sex_id;
			sex2 = v_p.at(j).sex_id;
			checkSexCouple(&sex1, &sex2);
			v_marriages.at(i).sex_id = sex1;
			v_p.at(j).sex_id = sex2;

			ss.sex_id = v_p.at(j).sex_id;
			ss.peer = v_p.at(j).peer;
			ss.title = v_p.at(j).title;
			ss.titolo = v_p.at(j).titolo;
			ss.last_name = v_p.at(j).last_name;
			ss.first_name = v_p.at(j).first_name;
			ss.posterior = v_p.at(j).posterior;
			ss.birth_place = v_p.at(j).birth_place;
			ss.birth_date = v_p.at(j).birth_date;
			ss.death_place = v_p.at(j).death_place;
			ss.death_date = v_p.at(j).death_date;
			ss.comment = v_p.at(j).comment;
			ss.order = v_p.at(j).parentIndex;
			ss.spouseIndex = i;
			v_spouseSpouses.push_back(ss);				// h�zast�rsak h�zast�rsai 

			order.Format( L"%d", getSpouseOrder(&v_p) );
			if (v_marriages.at(i).sex_id == MAN)
				v_marriages.at(i).whichWife = order;
			else
				v_marriages.at(i).whichHusband = order;

			
		}
	}
/*
	// minden h�zast�rsnak kisz�m�tja a h�zass�g-sorsz�mot
	if (v_spouseSpouses.size() == 0)	// ha a h�zast�rsnak nincsenek tov�bbi h�zast�rsai, akkor neki ez az 1. h�zass�ga;
	{
		v_marriages.at(i).orderSpouse = 1;
	}
*/
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ai i-edik h�zast�rs substringr�l leveszi a sz�l�ket l�s tov�bbi h�zast�rsakat tartalmaz� z�r�jeles stringet
CString CInputGA::cutParentsSposuses(int i)
{
	CString relativesSubstr;
	CString spouseSubstr;
	CString cLine;
	CString munka;
	int pos;
	int b;
	int j;
	int b1;
	int b2;
	int j1;
	int j2;


	cLine = v_marriages.at(i).marriageSubstr.Trim();
	spouseSubstr = cLine;
	if (cLine.Right(1) == ')')	// a sor v�g�n van z�r�jel, akkor ez (sz�l�k,h�zast�rs )  ???
	{
		cLine = dropLastCharacter(cLine);
		munka = cLine;			// v�gz�r�jelt eldobja
// = T�thfalussy_(Glatzinger) Ludovika * 1792 + F 1815.02.17 (Lajos le�nya)
// = M�rkus �s Batizfalvi M�ri�ssy �va (Andr�s - G�rgey Margit, 2f.Gyulai_(Sz�ke) Ferenc)
// = Prib�c(Necp�l) 1720.01.14 Beniczi �s Micsinyei Beniczky Zsuzsa * Benic(Szocs�c) 1701.12.15 + Prib�c(Necp�l) 1743.01.06 (J�nos - Rajmanus Kata)
		while( true )
		{
			if( (b = munka.ReverseFind('(')) != -1 )
			{
				if ((j = munka.Find(')', b + 1)) == -1)	// nem tal�lt m�r benne 
				{
					spouseSubstr = cLine.Left(b - 1);
					relativesSubstr = cLine.Mid(b + 1);
					v_marriages.at(i).relativesSubstr = relativesSubstr;
					break;
				}
				// be�gyazott z�r�jelp�r
				munka.SetAt(b, '_'); // bal z�r�jel t�rl�se  
				munka.SetAt(j, '_'); // jobb z�r�jel t�rl�se
			}
			else   // a v�gz�r�jelnek nincs kezd�z�r�jele!!
			{
				break;
			}
		}
	}
	v_marriages.at(i).spouseSubstr = spouseSubstr;
	return spouseSubstr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// bemenet: a h�zast�rs neme
// kimenet: a lesz�rmazott neme
CString CInputGA::checkSex( CString sex_id )
{
	CString sexid = sex_id;
	// ha a lesz�rmazott neme nem meghat�rozhat�, de a h�zast�rs� igen, akkor be�ll�tjuk a lesz�rmazott nem�t
//	if( d.sex_id.IsEmpty() && !sex_id.IsEmpty())
	if (d.sex_id == L"0" && sex_id != L"0")
	{
		if( sex_id == MAN )
			d.sex_id = WOMAN;
		else
			d.sex_id = MAN;
	}
// ha a h�zast�rs neme nem meghat�rozhat�, de a lesz�rmazott� igen, akkor be�ll�tjuk a h�zast�rs nem�t
//	if( !d.sex_id.IsEmpty() && sex_id.IsEmpty() )
	if (d.sex_id != L"0" && sex_id == L"0")
	{
		if( d.sex_id == MAN )
			sexid = WOMAN;
		else
			sexid = MAN;
	}
// ha egyik�k neme sem meghat�rozhat�, akkor a lesz�rmazott legye f�rfi, a h�zast�rsa n�
//	else if( d.sex_id.IsEmpty() && sex_id.IsEmpty() )
	else if (d.sex_id == L"0" && sex_id == L"0" )
	{
		d.sex_id = MAN;				
		sexid = WOMAN;
	}
	return sexid;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sex1-bne �s sex2- ben adott �rt�kekket ellen�rzi, �s ha b�rmewlyik �rt�ke j�, akkor a m�sikat ahhoz igaz�tja
void CInputGA::checkSexCouple( CString* sex1, CString* sex2 )
{
	CString sexid1 = *sex1;
	CString sexid2 = *sex2;
	if ((sexid1 == MAN && sexid2 == WOMAN) || (sexid1 == WOMAN && sexid2 == MAN))
		return;  // minden rendben

	if (sexid1 == MAN)
		sexid2 = WOMAN;
	else if (sexid1 == WOMAN)
		sexid2 = MAN;
	else if (sexid2 == MAN)
		sexid1 = WOMAN;
	else if (sexid2 == WOMAN)
		sexid1 = MAN;
		
	*sex1 = sexid1;
	*sex2 = sexid2;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (=)Prince George, Alberta 1955.10.31 T�gl�si Nagy Ir�n * 1929.01.04
CString CInputGA::processWedding( int ix)
{
	PEOPLE dummy;
	CStringArray A;
	CString nameSubstr;
	CString word;
	CString place;
	CString date;
	TCHAR kar;
	int i, j;
	int pos;
	int db;
	CString sex;
	int start = 1;
	CString item1;
	CString item2;
	CString item3;

	int n = splitCString(m_weddingNameSubstr, ' ', false, &A);
	for (i = 0; i < n; ++i)
	{
		if ((db = isWeddingDate(&A, i, &date)))   // van d�tum
		{
			if (i)
			{
				word = A[i - 1];
				if (word == L"kb")
				{
					--i;
					++db;
				}
			}
			if (!i)		// d�tummal kezd�dik, place nincs
			{
				
				date = packWords(&A, i, db);
				v_marriages.at(ix).date = date;
				nameSubstr = packWords(&A, i + db, n - i - db);
				return nameSubstr;
			}
			if (i < 2)
			{
				date = packWords(&A, i, db);
				v_marriages.at(ix).date = date;
				v_marriages.at(ix).place = A[0];
				nameSubstr = packWords(&A, i + 1, n - i - 1);
				nameSubstr = packWords(&A, i + db, n - i - db);
				return nameSubstr;
			}

			nameSubstr = packWords(&A, i+db, n - db + 1); // ez nem biztos, hogy people, ez�rt kell a isname
//			if( nameSubstr.IsEmpty())		// nincs megadva a h�zst�rs neve, legyen N N Ez nem biztosm, hoyg helyes!!!!!!!!!!!!!!!!!!!!!!!!!!
//				nameSubstr = L"N N";
			if( isname(  nameSubstr, &dummy) )
			{
				if (i)  // van a d�tum el�tt valami,
				{
					place = packWords(&A, 0, i);
					if (place == L"kb") place.Empty();
				}
				nameSubstr = packWords(&A, i + db, n - i - db);
//				if (nameSubstr.IsEmpty())		// nincs megadva a h�zst�rs neve, legyen N N, Ez nem biztos, hogy helyes!!!!!!!!!!!!!!!!!!!!
//					nameSubstr = L"N N";
			}
			else 
			{
				date.Empty();		// a d�tum ut�n nem n�v k�vetkezik, akkor a date nem a h�zass�gk�t�s d�tuma
			}
			break;
		}
	}
	if (date.IsEmpty())  // nincs d�tum
	{

		if ((pos = m_weddingNameSubstr.Find(',')) != -1)
		{
			item1 = m_weddingNameSubstr.Left(pos);
			item2 = m_weddingNameSubstr.Mid(pos + 2);
			if (isname(item1, &dummy))
			{
				return m_weddingNameSubstr;				// nincs se hely, se id�
			}
			else if (isname(item2, &dummy))
			{
				v_marriages.at(ix).place = item1;
				return(m_weddingNameSubstr.Mid(pos + 2));
			}
			if ((pos = item2.Find(',')) != -1)  // Egy m�sodik vessz� is van!!
			{
				item3 = item2.Mid(pos + 2);
				if (isname(item3, &dummy))
				{
					v_marriages.at(ix).place = item1 + L", " + item2.Left(pos);
					return(item3);
				}
			}
		}

		for (i = 0; i < n; ++i)
		{
			// elv�lasztja a helyet �s a nevet vagy
			// elv�asztja nevet �s a posteriort
   			if (i < 3 && getLastCharacter(A[i]) == ',' && !iswlower(A[i][0]))
			{
				A[i].Remove(',');
				word = A[i];
				if ((sex = theApp.isFirstName(A[i])) != L"") // hely is lehet 'A,'!!
				{
					if (word.GetLength() != 1)					// Csak a t�bbbet�s sz�t fogadja el n�vnek!
					{
						nameSubstr = packWords(&A, 0, n);
						break;
					}
				}

				if (i + 1 < n && !iswlower(A[i + 1][0]))		// ut�na nagybet�s sz� j�n
				{
					if ((n - i - 1) > 1)  // 
					{
						place = packWords(&A, 0, i + 1);
						if (place == L"kb") place.Empty();
						nameSubstr = packWords(&A, i + 1, n - i - 1);
					}
					break;
				}
			}
		}
		if (i == n)  // nincs place, nincs date
		{
			nameSubstr = m_weddingNameSubstr;
		}
		if (nameSubstr.IsEmpty())
			nameSubstr = m_weddingNameSubstr;
	}
	v_marriages.at(ix).place = place;	
	v_marriages.at(ix).date = date;
	return nameSubstr;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vp-ben a lesz�rmazott h�zast�rs�nak tov�bbi h�zast�rsai vannak
//
//  1
//  2
// ->3
// visszadja a lesz�rmazott h�zast�rs�nak index�t
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
//void CInputGA::processSpousesSpouses( CString spouses,  std::vector<PEOPLE> *v_p )
void CInputGA::processSpousesSpouses( int ix, std::vector<PEOPLE>* v_p)
{
	PEOPLE people;
	int		order;
	CString spouse;
	CString spouses = v_marriages.at(ix).spouses;


	int pos;
	int pos2 = 0;
	int pos3;
	int posComma = 0;


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
		clearPeople(&people);
		m_nameSubstr = cutBirthDeath(spouse);
		splitSubstrs( &people );
		people.parentIndex = order; // a h�zast�rs h�zast�rs�nak nincs megadva az anyja, ez�rt a parentIndex-et a h�zass�g sorsz�m�ra haszn�ljuk
		v_p->push_back( people );
	};
}
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Meg�llap�tja, hogy az n elem� A t�mb i-edik szav�t�l kezdve n�v k�vetkezik-e
// 
// Kov�cs J�nos
// Ujh�zi Kiss Elem�r
// Abl�nci �s Kov�li Nagy J�zsef
// 
// minden fajta n�v el�tt �llhat title vagy peer
// az el�nevek ut�n �llhat title vagy peer
bool CInputGA::isName(CStringArray* A, int n, int i)
{
	int cnt = 0;
	int sex;
	CString word;
	CString titolo;
	CString name;

	bool ret = true;

	if (n - i < 2) return false;		// 2 sz�n�l kevesebb van, ez nem lehet

	return true;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::isKivetel(CString word)
{
	for( int i = 0; i < m_numOfKivetelek; ++i)
	{
		if (word.Left(4) == m_kivetelek[i].value) 
				return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a d�tum e�tt �ll� 'kb' ill. a d�tum ut�n �ll� 'ut�n','k�r�l','kb' a d�tum r�sze!
// visszadott ret �rt�ke megadja, h�ny sz�b�l �ll a d�tum
int CInputGA::isWeddingDate(CStringArray* A, int i, CString* datum)
{
	INT_PTR n = A->GetCount();
	int ret = 0;
	int pos;
	CString str1;
	CString str2;
	CStringArray M;
	CString date(L"");
//	CString miez;
	CString word;
	M.Add(L"ut�n");
	M.Add(L"k�r�l");
	M.Add(L"el�tt");
	M.Add(L"kb");

	CStringArray B;

	word = A->GetAt(i);						// 1944/45 alak�t is elfogadja d�tumnak
	word.Remove(',');

	int num;
	int m = wordList(&B, word, '.', false);
	if (m == 3 && isRomanNumber( B[1]) )
	{
		num = convRomanArabic(B[1]);
		word.Format(L"%4s.%02d.%02s", B[0], num, B[2]);
		A->SetAt(i, word);
	}


	if ((pos = word.Find('/')) != -1)
	{
		str1 = word.Left(pos);
		str2 = word.Mid(pos + 1);
		if (isNumeric(str1) && isNumeric(str2))
		{
			*datum = word;
			return 1;
		}
	}

//	word = A->GetAt(i);
	word.Remove('?');
	word.Remove('.');
	word.Remove(',');
	word.Trim();

	if (word == L"kb")
	{
		++i;
		if (i > A->GetCount() - 1)
		{
			*datum = date;
			return 0;
		}

		word = A->GetAt(i);
		word.Remove('?');
		word.Remove('.');
		word.Remove(',');
		word.Trim();
	}
	if (isNumeric(word))
	{
		++ret;
		if (i > 0 && !A->GetAt(i - 1).Compare(L"kb"))		// kb 1944
		{
			date = L"kb ";
			date += A->GetAt(i);
			++ret;
		}
		else
			date = A->GetAt(i);

		if (i + 1 < n - 1)
		{
			for (int j = 0; j < M.GetCount(); ++j)
			{
				if (!A->GetAt(i + 1).Compare(M[j]))
				{
					date += " ";
					date += M[j];
					++ret;
					break;
				}
			}
		}
	}
	*datum = date;
	return ret;
}
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
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::splitSubstrs( PEOPLE* any)
{
	*any = {};
	splitNameComment();  // m_nameSubstr felbont�sa m_name �s m_comment-re
	any->peer = m_peer;
	any->posterior = m_posterior;
	any->comment = m_comment;
	
	PLACE_DATE_BLOCK bb;
	PLACE_DATE_BLOCK db;

	isname(m_name, any);  // m_name felbont�sa
	processPlaceDateComment(m_birthSubstr, &bb);
	processPlaceDateComment(m_deathSubstr, &db);

	any->whichHusband = 0;
	any->birth_place = bb.place;
	any->birth_date = bb.date;
	if (any->comment.IsEmpty())
		any->comment = bb.comment;

	any->death_place = db.place;
	any->death_date = db.date;
	if (any->comment.IsEmpty())
		any->comment = db.comment;

	if (any->peer.IsEmpty())
		any->peer = setPeer(any);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::isPeer(CString p)
{
	if (p == L"gr" || p == L"br" || p == L"hg" || p == L"gr�f" || p == L"b�r�" || p == L"herceg")
	{
		return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::extractPeer(CString str)
{
	if (str == L"gr" || str == L"br" || str == L"hg" || str == L"gr�f" || str == L"b�r�" || str == L"herceg")
	{
		return(str);
	}
	return L"";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kamaragr�f, f�b�nyagr�f az nem gr�fi c�m!!!!
CString CInputGA::getPeerFromComment(CString comment)
{
	CString peer;
	CStringArray A;
	CString word;

	int n = wordList(&A, comment, ' ', false);
	for (int i = 0; i < n; ++i)
	{
		word = A[i];
		word.Remove(',');
		if (word == L"herceg")
			peer = L"hg";
		else if (word == L"gr�f")
			peer = L"gr";
		else if (word == L"b�r�")
			peer = L"br";
	}
	return peer;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
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
*/
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
	word = getWordFrom(comment, pos);
	return word;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Meg�llap�tja, hogy a line string elej�n szem�yln�v van-e. 
// Ha igen, akkor azt felbontja elemeire a PEOPLE struktur�ba, �s true �rt�kkel t�r vissza.
// Ha �gy tal�lja, hogy nem szem�lyn�vvel kezd�dik a string, akkor false �rt�kkel t�r vissza.
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::isname(CString line, PEOPLE* p)
{
	int pos;
	int i;
	int k;
	int m;				// az �sszes sz� sz�ma
	int n;				// csak a posteriorig terjed� szavak sz�ma
	int end;			// n�v v�g�nek indexe, ut�na a posterior k�vetkezik
	int cnt;
	int db;
	int num;
	int z;
	CStringArray A;
	CString word;

	CString sex;
	CString title;
	CString titolo;
	CString last_name;
	CString first_name;
	CString name;
	CString comment;


	m = wordList(&A, line, ' ', false);
	if (m < 2) return false;		// egy n�v legal�bb 2 tagb�l �ll

	n = getSuffix(&A, 1 );

		// az A-ban n-ig csak a n�v van, posterior lev�gva!
		// 
		// Rimisz�csi br Sz�chy Tam�s - b� is kiszedi a peere-t!
	// innent�l �j!	
	for (i = 0; i < n; ++i)
	{
		word = A[i];
		word.Remove('?');
		word.Remove('(');
		if (!iswalpha(word[0])) return false;		// a n�v minden elem�nek bet�vel kell kezd�dni

		if (isPeer(word) )
		{
			m_peer = word;
			A[i].Empty();
		}
		else if (isTitle(word) )
		{
			title += word;
			title += L" ";
			A[i].Empty();
		}


		/*
		A[i].Remove('?');
		A[i].Trim();
		if (!iswalpha(A[i][0])) return false;		// a n�v minden elem�nek bet�vel kell kezd�dni

		if (isPeer(A[i]))
		{
			m_peer = A[i];
			A[i].Empty();
		}
		else if (isTitle(A[i]))
		{
			title += A[i];
			title += L" ";
			A[i].Empty();
		}
		*/
	}
	title.Trim();

	name = packWords(&A, 0, n);
	n = wordList(&A, name, ' ', false);


	// = Kov�cs Ad�l, 
	for (i = 0; i < n; ++i)
	{
		word = A.GetAt(i);
		if (word.IsEmpty())
			z = 2;
		if (i && word.GetAt(word.GetLength() - 1) == ',')		// lehet benne vessz�: Giczi, Assa �s Abl�nck�rti Ghyczy Anna M�ria 
		{
			name = packWords(&A, 0, i + 1);
			name.Remove(',');
			comment = packWords(&A, i + 1, n - i - 1);
			break;
		}
	}

	n = wordList(&A, name, ' ', false);
	if (n < 2) return false;		// egy n�v legal�bb 2 tagb�l �ll

	if (n > 2)
		i = isTitolo(&A, n, 0, &titolo);  // a titolo csak akkor lehet, ha ut�na m�g legal�bb 2 sz� van
	else
		i = 0;

	num = getLastName(&A, n, i, &last_name);
	i += num;
	getFirstName(&A, i, &first_name, &sex);
	if (!iswupper(first_name[0])) return false;

	if (first_name.IsEmpty() || last_name.IsEmpty())
		return false;

	p->titolo		= titolo;
	p->title		= title;
	p->last_name	= last_name;
	p->first_name	= first_name;
	p->sex_id		= sex;
	if (p->peer.IsEmpty())
		p->peer = m_peer;
	if( p->posterior.IsEmpty())
		p->posterior = m_posterior;
	if (p->comment.IsEmpty())
		p->comment = m_comment;


	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CInputGA::isTitle(CString word)
{
	for (int i = 0; i < numOfTitles; ++i)
	{
		if (word == m_titles[i].value)
			return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az A[i]-t�l kezdve megn�zi, hogy titolo k�vetkezik-e.
// Ha igen, akkor beteszi a titolo-ba, �s visszadja a titolo szavainak sz�m�t
// Ha nem, akkor o-t ad vissza
// titolo egy sz�-sor, ha
// X �s Y
// X,Y �s Z
// Xi     ( i-re v�gz�d� sz�. Ez lehet csal�dn�v is!!!!!!!
int CInputGA::isTitolo(CStringArray* A, int n, int i, CString* titolo)
{
	CString word1;
	CString word2;
	CString word3;
	CString word4;
	CString word5;
	CString word6;
	TCHAR kar;
	int cnt = 0; // a titolo szavainak sz�ma
	// X �s Y t�pus� el�nevek azonos�t�sa

	word1 = A->GetAt(i);
	if (!iswalpha(word1[0])) return 0;
	if (iswlower(word1[0])) return 0;

	if (i + 1 < n)
		word2 = A->GetAt(i + 1);
	if (i + 2 < n)
		word3 = A->GetAt(i + 2);
	if (i + 3 < n)
		word4 = A->GetAt(i + 3);
	if (i + 4 < n)
		word5 = A->GetAt(i + 4);
	if (i + 5 < n)
		word6 = A->GetAt(i + 5);

	if (word2 == L"�s")  // M�rkus �s Batizfalvi Kir�ly Zolt�n
	{
		if (iswupper(word3[0]) && iswupper(word4[0]) && iswupper(word5[0]))
			cnt = 3;
	}
	else if (word3 == L"�s")	// Giczi, Assa �s Abl�nck�rti Ghyczy Anna M�ria 
	{
		if (iswupper(word2[0]) && iswupper(word4[0]) && iswupper(word5[0]) && iswupper(word6[0]))
			cnt = 4;
	}
	else if ((kar = getLastCharacter(word1)) == 'i')
	{
		if (iswupper(word2[0]) && iswupper(word3[0]))
			cnt = 1;
	}
	*titolo = packWords(A, i, cnt);
	return cnt;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// Az A t�nbb i-edik elem�t�l keresi a csal�dnevek
// Csaal�dn�vnek tekinti az els� sz�t �s a tov�bbi nem keresztneveket
int CInputGA::getLastName(CStringArray* A, int n, int i, CString* lastname)
{
	CString word;
	CString sex;
	int cnt = 1;
	*lastname = A->GetAt(i);
	*lastname += L" ";
	for (++i; i < n; ++i)
	{
		word = A->GetAt(i);
		sex = theApp.isFirstName(word);
		if( sex.IsEmpty() )
		{
			*lastname += word;
			*lastname += L" ";
			++cnt;
		}
		else
		{
			break;
		}
	}
	*lastname->TrimRight();
	return cnt;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// visszaadja a posterior kezdet�nek index�t �s a posterior-t a posteriorba teszi
CString CInputGA::cutBirthDeath( CString spouseSubstr )
{
	m_birthSubstr.Empty();
	m_deathSubstr.Empty();

	int		pos;
	if ((pos = spouseSubstr.Find('+')) != -1)
	{
		m_deathSubstr = spouseSubstr.Mid(pos + 1);		// hal�loz�si blokk
		spouseSubstr = spouseSubstr.Left(pos);
	}
	if ((pos = spouseSubstr.Find('*')) != -1)
	{
		m_birthSubstr = spouseSubstr.Mid(pos + 1);		// sz�let�si blokk
		spouseSubstr = spouseSubstr.Left(pos);
	}
	return spouseSubstr;
}
