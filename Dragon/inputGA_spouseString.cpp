#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"
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
// György/3 * Eperjes 1920.04.29 = Prince George, Alberta 1955.10.31 Téglási Nagy Irén *1929.01.04
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processSpouseSubstrings()
{
	if (v_marriages.size() == 0) return;

	CString spouseSubstr;

	for (UINT i = 0; i < v_marriages.size(); ++i)
	{
		s = { 0 };
		spouseSubstr = cutParentsSposuses(i);	// házastárs szüleinek levélasztésa (v_marriges->spouseSubstr és v_marriges->relativeSubstr
		m_weddingNameSubstr = cutBirthDeath(spouseSubstr);

		processSpouse( &s);  
		

		if (!m_weddingPlace.IsEmpty())
		{
			CString date;
			CString str1;
			CStringArray A;

			m_weddingPlace.Remove(',');
			int n = wordList(&A, m_weddingPlace, ' ', false);
			if (n > 1)
			{
				str1 = splitStringAt(m_weddingPlace, n - 1, &date);
				if (isWeddingDate(&A, n - 1, &date))   // van dátum
				{
					m_weddingDate = date;
					m_weddingPlace = str1;
				}
			}
		}
		v_marriages.at(i).place = m_weddingPlace;
		v_marriages.at(i).date = m_weddingDate;
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
// Az i. házasság felbontáse
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
	// felbontások
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
		str.Trim();
			
		PEOPLE	spf;
		clearPeople(&spf);
		m_nameSubstr = str;
		m_nameSubstr = cutBirthDeath(str);
		splitSubstrs( &spf );
		sex1 = spf.sex_id;
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

// házastárs további házastársainak feldolgozása

// 2f. Gaiger György, 3f. Nicky Sándor
	if (moreSpouses.IsEmpty()) return;


	SPOUSESPOUSES ss;
	std::vector<PEOPLE> v_p;
	CString order;
	v_p.clear();

	if ((pos = moreSpouses.Find(L"f.")) != -1)
	{
		v_marriages.at(i).spouses = moreSpouses.Mid(pos - 1);
		processSpousesSpouses( i, &v_p);	//v_p-be házastársanként felbontja a stringet

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
			v_spouseSpouses.push_back(ss);				// házastársak házastársai 

			order.Format( L"%d", getSpouseOrder(&v_p) );
			if (v_marriages.at(i).sex_id == MAN)
				v_marriages.at(i).whichWife = order;
			else
				v_marriages.at(i).whichHusband = order;

			
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ai i-edik házastárs substringrõl leveszi a szûlõket lés további házastársakat tartalmazó zárójeles stringet
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
	if (cLine.Right(1) == ')')	// a sor végén van zárójel, akkor ez (szülõk,házastárs )  ???
	{
		cLine = dropLastCharacter(cLine);
		munka = cLine;			// végzárójelt eldobja
// = Tóthfalussy_(Glatzinger) Ludovika * 1792 + F 1815.02.17 (Lajos leánya)
// = Márkus és Batizfalvi Máriássy Éva (András - Görgey Margit, 2f.Gyulai_(Szõke) Ferenc)
// = Pribóc(Necpál) 1720.01.14 Beniczi és Micsinyei Beniczky Zsuzsa * Benic(Szocsóc) 1701.12.15 + Pribóc(Necpál) 1743.01.06 (János - Rajmanus Kata)
		while( true )
		{
			if( (b = munka.ReverseFind('(')) != -1 )
			{
				if ((j = munka.Find(')', b + 1)) == -1)	// nem talált már benne 
				{
					spouseSubstr = cLine.Left(b - 1);
					relativesSubstr = cLine.Mid(b + 1);
					v_marriages.at(i).relativesSubstr = relativesSubstr;
					break;
				}
				// beágyazott zárójelpár
				munka.SetAt(b, '_'); // bal zárójel törlése  
				munka.SetAt(j, '_'); // jobb zárójel törlése
			}
			else   // a végzárójelnek nincs kezdõzárójele!!
			{
				break;
			}
		}
	}
	v_marriages.at(i).spouseSubstr = spouseSubstr;
	return spouseSubstr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// bemenet: a házastárs neme
// kimenet: a leszármazott neme
CString CInputGA::checkSex( CString sex_id )
{
	CString sexid = sex_id;
	// ha a leszármazott neme nem meghatározható, de a házastársé igen, akkor beállítjuk a leszármazott nemét
//	if( d.sex_id.IsEmpty() && !sex_id.IsEmpty())
	if (d.sex_id == L"0" && sex_id != L"0")
	{
		if( sex_id == MAN )
			d.sex_id = WOMAN;
		else
			d.sex_id = MAN;
	}
// ha a házastárs neme nem meghatározható, de a leszármazotté igen, akkor beállítjuk a házastárs nemét
//	if( !d.sex_id.IsEmpty() && sex_id.IsEmpty() )
	if (d.sex_id != L"0" && sex_id == L"0")
	{
		if( d.sex_id == MAN )
			sexid = WOMAN;
		else
			sexid = MAN;
	}
// ha egyikük neme sem meghatározható, akkor a leszármazott legye férfi, a házastársa nõ
//	else if( d.sex_id.IsEmpty() && sex_id.IsEmpty() )
	else if (d.sex_id == L"0" && sex_id == L"0" )
	{
		d.sex_id = MAN;				
		sexid = WOMAN;
	}
	return sexid;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sex1-bne és sex2- ben adott értékekket ellenõrzi, és ha bármewlyik értéke jó, akkor a másikat ahhoz igazítja
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vp-ben a leszármazott házastársának további házastársai vannak
//
//  1
//  2
// ->3
// visszadja a leszármazott házastársának indexét
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
		clearPeople(&people);
		m_nameSubstr = cutBirthDeath(spouse);
		splitSubstrs( &people );
		people.parentIndex = order; // a házastárs házastársának nincs megadva az anyja, ezért a parentIndex-et a házasság sorszámára használjuk
		v_p->push_back( people );
	};
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a dátum eõtt álló 'kb' ill. a dátum után álló 'után','körül','kb' a dátum része!
// visszadott ret értéke megadja, hány szóból áll a dátum
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
	M.Add(L"után");
	M.Add(L"körül");
	M.Add(L"elõtt");
	M.Add(L"kb");

	CStringArray B;

	word = A->GetAt(i);						// 1944/45 alakút is elfogadja dátumnak
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
				word = A->GetAt(i + 1);
				word.Remove(',');
				if ( !word.Compare(M[j]))
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
void CInputGA::splitSubstrs( PEOPLE* any)
{
	*any = {};
	splitNameComment();  // m_nameSubstr felbontása m_name és m_comment-re
	any->peer = m_peer;
	any->posterior = m_posterior;
	any->comment = m_comment;
	
	PLACE_DATE_BLOCK bb;
	PLACE_DATE_BLOCK db;

	isname(m_name, any);  // m_name felbontása
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
	for (int i = 0; i < sizeof(peers); ++i)
	{
		if (p == peers[i].sh || p == peers[i].lg)
			return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kamaragróf, fõbányagróf az nem grófi cím!!!!
CString CInputGA::getPeerFromString(CString comment)
{
	CStringArray A;
	CString word;

	int n = wordList(&A, comment, ' ', false);
	for (int i = 0; i < n; ++i)
	{
		word = A[i];
		word.Remove(',');
		for (int j = 0; j < sizeof(peers); ++j)
		{
			if (word == peers[j].sh)
				return word;
			if (word == peers[j].lg)
				return peers[j].sh;
		}
	}
	return L"";
}
// Megállapítja, hogy a line string elején szeméylnév van-e. 
// Ha igen, akkor azt felbontja elemeire a PEOPLE strukturába, és true értékkel tér vissza.
// Ha úgy találja, hogy nem személynévvel kezdõdik a string, akkor false értékkel tér vissza.
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::isname(CString line, PEOPLE* p)
{
	int pos;
	int i;
	int k;
	int m;				// az összes szó száma
	int n;				// csak a posteriorig terjedõ szavak száma
	int end;			// név végének indexe, utána a posterior következik
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

	m_nameSubstr.Empty();

//	if( (pos=line.Find( ',')) != -1 )		// vesszõ nem lehet benne
//		return false;

	m = wordList(&A, line, ' ', false);
	if (m < 2) return false;		// egy név legalább 2 tagból áll

	n = getSuffix(&A, 1 );

		// az A-ban n-ig csak a név van, posterior levágva!
		// 
		// Rimiszécsi br Széchy Tamás - bó is kiszedi a peere-t!
	// innentõl új!	
	for (i = 0; i < n; ++i)
	{
		word = A[i];
		word.Remove('?');
		word.Remove('(');
		if (!iswalpha(word[0])) return false;		// a név minden elemének betûvel kell kezdõdni

		if (isPeer(word) )
		{
			m_peer = word;
			p->peer = word;
			A[i].Empty();
		}
		else if (isTitle(word) )
		{
			title += word;
			title += L" ";
			A[i].Empty();
		}
	}
	title.Trim();
	p->title = title;

	name = packWords(&A, 0, n);
	n = wordList(&A, name, ' ', false);
	if (n == 2)
	{
		str = theApp.isFirstName(A[1]);
		if( !str.IsEmpty())
		{
			p->sex_id = str;
			p->first_name = A[1];
			p->last_name = A[0];
			m_nameSubstr = line;
			return true;
		}
		return false;
	}
	// = Kovács Adél, 
	for (i = 0; i < n; ++i)
	{
		word = A.GetAt(i);
		if (word.IsEmpty())
			z = 2;
		if (i && word.GetAt(word.GetLength() - 1) == ',')		// lehet benne vesszõ: Giczi, Assa és Ablánckürti Ghyczy Anna Mária 
		{
			name = packWords(&A, 0, i + 1);
			name.Remove(',');
			comment = packWords(&A, i + 1, n - i - 1);
			break;
		}
	}

	n = wordList(&A, name, ' ', false);
	if (n < 2) return false;		// egy név legalább 2 tagból áll

	if (n > 2)
		i = isTitolo(&A, n, 0, &titolo);  // a titolo csak akkor lehet, ha utána még legalább 2 szó van
	else
		i = 0;

	num = getLastName(&A, n, i, &last_name);
	if (last_name.Find(',') != -1)
		return false;

	i += num;
	getFirstName(&A, i, &first_name, &sex);
	if (!iswupper(first_name[0])) return false;

	if (first_name.IsEmpty() || last_name.IsEmpty())
		return false;

	p->titolo		= titolo;
//	p->title		= title;
	p->last_name	= last_name;
	p->first_name	= first_name;
	p->sex_id		= sex;
//	if (p->peer.IsEmpty())
//		p->peer = m_peer;
	if( p->posterior.IsEmpty())
		p->posterior = m_posterior;
	if (p->comment.IsEmpty())
		p->comment = m_comment;

	m_nameSubstr = line;
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
// Az A[i]-tõl kezdve megnézi, hogy titolo következik-e.
// Ha igen, akkor beteszi a titolo-ba, és visszadja a titolo szavainak számát
// Ha nem, akkor o-t ad vissza
// titolo egy szó-sor, ha
// X és Y
// X,Y és Z
// Xi     ( i-re végzõdõ szó. Ez lehet családnév is!!!!!!!
int CInputGA::isTitolo(CStringArray* A, int n, int i, CString* titolo)
{
	CString word1;
	CString word2;
	CString word3;
	CString word4;
	CString word5;
	CString word6;
	TCHAR kar;
	int cnt = 0; // a titolo szavainak száma
	// X és Y típusú elõnevek azonosítása

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

	if (word2 == L"és")  // Márkus és Batizfalvi Király Zoltán
	{
		if (iswupper(word3[0]) && iswupper(word4[0]) && iswupper(word5[0]))
			cnt = 3;
	}
	else if (word3 == L"és")	// Giczi, Assa és Ablánckürti Ghyczy Anna Mária 
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
// Az A tönbb i-edik elemétõl keresi a családnevek
// Csaaládnévnek tekinti az elsõ szót és a további nem keresztneveket
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
// visszaadja a posterior kezdetének indexét és a posterior-t a posteriorba teszi
CString CInputGA::cutBirthDeath( CString spouseSubstr )
{
	m_birthSubstr.Empty();
	m_deathSubstr.Empty();

	int		pos;
	if ((pos = spouseSubstr.Find('+')) != -1)
	{
		m_deathSubstr = spouseSubstr.Mid(pos + 1);		// halálozási blokk
		spouseSubstr = spouseSubstr.Left(pos);
	}
	if ((pos = spouseSubstr.Find('*')) != -1)
	{
		m_birthSubstr = spouseSubstr.Mid(pos + 1);		// születési blokk
		spouseSubstr = spouseSubstr.Left(pos);
	}
	return spouseSubstr;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (=)Prince George, Alberta 1955.10.31 Téglási Nagy Irén * 1929.01.04
void CInputGA::processSpouse(PEOPLE* s)
{
	CStringArray A;
	CString nameSubstr;
	CString word;
	CString date;
	int i;
	int pos;
	int db;
	int n = splitCString(m_weddingNameSubstr, ' ', false, &A);

	if ( isname(m_weddingNameSubstr, s))
		return;								// névvel kezdõdik

	for (i = 0; i < n; ++i)
	{
		if ((db = isWeddingDate(&A, i, &date)))   // van dátum
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
			break;
		}
	}

	// lehet a dátum házasságkötés ???
	// dátum után nem fér el a név vagy a dátum elõtt nem nagybetûs szó áll vagy nem '('
	if (i && db) 
	{
		word = A[i - 1];
		word.Remove('(');
		if(  i > (n - 3) )
		{
			db = 0;
			i = 0;
		}
		else if (!iswupper( word.GetAt(0) ) ) 
		{
			db = 0;
			i = 0;
		}
	}
	date = packWords(&A, i, db);  // van dátum dummy!!!
	if (!db )  // nincs dátum,  Még mindig lehet "place, nameSubstr"
	{
		m_weddingDate.Empty();
		processPlaceNameSubstr(m_weddingNameSubstr, s);
		return;
	}

	date = packWords(&A, i, db);  // van dátum
	nameSubstr = packWords(&A, i + db, n - i - db);
	if (i == 0)		// dátummal kezdõdik, place nincs, ez a weddingDate;
	{
		m_weddingPlace.Empty();
		m_weddingDate = date;
		isname(nameSubstr, s);
	}
	else if (i == 1)	// dátum a 2. szó
	{
		m_weddingPlace = A[0];
		m_weddingDate = date;
		isname(nameSubstr, s);
	}
	else  // dátum a 3. vagy sokadikabb szó
	{
		if (isname(m_weddingNameSubstr, s))  // elején van?
		{
			m_weddingDate.Empty();
			m_weddingPlace.Empty();
			m_nameSubstr = m_weddingNameSubstr;
			return;
		}
		processPlaceNameSubstr( nameSubstr, s);  // nameSubstr a dátum utáni string
		if (!s->last_name.IsEmpty())			// név van az elején
		{
			m_weddingPlace = packWords(&A, 0, i );
			m_weddingDate = date;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processPlaceNameSubstr(CString placeName, PEOPLE* s)
{
	CString str1;
	CString str2;
	bool ret;
	if ((ret = isname(placeName, s)))  // névvel kezdõdött a string, nincs place
	{
		m_weddingPlace.Empty();
		return;
	}
	CStringArray A;
	int n = wordList(&A, placeName, ' ', false);

	for (int i = 1; i < n-1; ++i)
	{
		str1 = splitStringAt(placeName, i, &str2);
		if (isname(str2, s))  // ha nincs place, akkor felbontotta a nevet, str a place
		{
			m_weddingPlace = str1;
			return;
		}
	}
	// a dátum után nem név van, akkor biztos elõtte van
	m_weddingPlace.Empty();
	m_nameSubstr = m_weddingNameSubstr;
	isname(m_weddingNameSubstr, s);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::splitStringAt(CString str, int at, CString* str2)
{
	CStringArray A;

	int n = wordList(&A, str, ' ', false);

	CString str1;
	if( at < n )
		*str2 = packWords(&A, at, n - at);
	str1 = packWords(&A, 0, at);
	return str1;
}
