#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A bementi fájlban a leszármazottak csak keresztnévvel szerepelnek.
// A leszármazottak elõneve a %%% tábla fejlécébõl van kiszedve az m_tableHeader.titolo-ba.
// Alapesetben a leszármazottak családneve a tábla fejlécébõl vannak leszedve.
// A családnév változást a leszármazott elõtti sorban lévõ % name2 jelzi. Ilyenkor az m_familyName
// ezt az értéket kapja meg. Ez csak egy újabb % name2 változtathatja meg.
//
// Ha a bemeneten nõk leszármazottai is szerepelnek, akkor azok apjuk elõnevét és családnevét kapják meg.
// Az apa a leszármazott házastársaként teljes névvel van megadva.
// 
// 
// 
// A d struktúra elõállítása insertálásra
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processDescendantSubstring(CString cLine)
{
	int i;
	int z;

	d.generation = cLine.GetAt(0);				// generáció karakterének leszedése
	cLine = cLine.Mid(2);						// G és szóköz leszedése

	if (m_tableAncestry)						// a tábla elsõ embere, õse
	{
		m_generationFirst = d.generation;
	}
	if (d.generation == m_generationFirst)
		d.tableRoman = m_tableHeader.tableRoman;  // csak az õst jelöli meg, hogy az összekötésnél megtalálják
	else
		d.tableRoman.Empty();

	// Megjelölöli minden tábla ösével azonos generációt, hogy a connectFolyt() mindegyiket beállíthassa az õt hívó másik táblában lévõ 
	// szülökre.

	d.tableAncestry = m_tableAncestry;
	d.source = 1;
	


	CString word;
	int pos;
	int n;
	int num;
	CString sex;
	CStringArray A;
	CString first_name;
	CString title;
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	splitToSubstr(cLine);		// felbontás:m_nameSubstr, m_birthSubstr, m_deathSubstr

	d.parentIndex = 0;

	if ((pos = m_nameSubstr.Find('?')) != -1)
	{
		m_nameSubstr.SetAt(pos, ' ');
		m_nameSubstr.Remove('?');
	}


	n = wordList(&A, m_nameSubstr, ' ', false);
	// anya index meghatározása
	for (i = 0; i < n; ++i)
	{
		word = A[i];
		if ((pos = word.Find('/')) != -1)
		{
			str = word.Left(pos);
			if ((sex = theApp.isFirstName(str)) != L"" )
			{
				d.parentIndex = _wtoi(word.Mid(pos + 1));
				A[i] = str;
				cLine = packWords(&A, 0, n);
				break;
			}
		}
	}
	
	n = getSuffix(&A, 0 );  
	d.peer = m_peer;
	d.posterior = m_posterior;
	d.comment = m_comment;




	for (i = 0; i < n; ++i)
	{
		A[i].Remove(',');
		A[i].Remove('?');
	}

	num = getFirstName(&A, 0, &first_name, &sex);

	d.sex_id = sex;
	if( sex == BISEX )
		d.sex_id = MAN;
	d.title = title;
	d.first_name = first_name;
	
	PLACE_DATE_BLOCK bb;
	PLACE_DATE_BLOCK db;

	processPlaceDateComment(m_birthSubstr, &bb);
	processPlaceDateComment(m_deathSubstr, &db);

	
	d.birth_place = bb.place;
	d.birth_date = bb.date;
	if (!bb.comment.IsEmpty())
		d.comment = bb.comment;

	d.death_place = db.place;
	d.death_date = db.date;
	if (!db.comment.IsEmpty())
		d.comment = db.comment;

	if (d.peer.IsEmpty())
		d.peer = setPeer(&d);

	d.whichHusband = 0;

// 	// d.whichHusband = m_genCnt;	// az a családon belüli generációk sorszáma!! Semmi köze az whichHusband-hez!!
							//Csak egy újgyakorlat.

	if (d.sex_id.IsEmpty() && !v_marriages.size()) d.sex_id = MAN;	// ha a leszármazott keresztnevébõl nem lehet megállapítani
																// nemét és házastársa sincs, akkor legyen férfi
	
	d.titolo	= m_tableHeader.titolo;
	d.last_name = m_familyName;	//a nõi leszármazottak gyerekei a 'setFatherMother'-ben kapják meg a családnevet!!! 
	if (d.first_name == L"Soma")
	{
		z = 1;
	}
	// a parentIndex csak ott van megadva, ahol változás történt, az ezt követõ gyerekek értelemszerûen ugyanannak az anyának a gyerekei.
	// alább kitertjesztjük a parentIndexet minden testvérre

	setParentIndex();	

	setFatherMother();

	m_generationPrev = d.generation;


	// csak a html listákhoz kell
	descS.nameSubstr = m_nameSubstr;
	descS.birthSubstr = m_birthSubstr;
	descS.deathSubstr = m_deathSubstr;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A bemenetben explicite nem megadott parentIndexek beállítása
// Ha elõ volt írva, akkor az marad.
// He nem volt elõírva és
//			nincs elõtte testvére, akkor a parentInedex 1 lesz
//			van elõtte testvére, akkor annak a parentIndexét kapja	
void CInputGA::setParentIndex()
{
	if (v_generations.size() < 1)
	{
		if (!d.parentIndex)
			d.parentIndex = 1;
		return;
	}

	int ix = v_generations.size() - 1;
	while (ix > 1 && d.generation < v_generations.at(ix).gen)		// eldobjuk a nagyobb generációkat, a legutulsó testvért keressük
	{
		v_generations.pop_back();
		--ix;
	}

//	if (ix >= 0)	// volt testvére
	{
		// parentIndex az elõzõ  testvértõl
		if (d.generation == v_generations.at(ix).gen)				// volt-e korábban testvére?
		{
			if (!d.parentIndex)									// ha nem volt megadva, akkor az elõzõ testvér parentIndex-ét kapja
				d.parentIndex = v_generations.at(ix).parentIndex;
			d.whichWife = d.parentIndex;
			++v_generations.at(ix).whichChildFather;
			d.whichChildFather = v_generations.at(ix).whichChildFather;
			++v_generations.at(ix).whichChildMother;				// csak akkor jó, ha egyetlen felesége van az apának
			d.whichChildMother = v_generations.at(ix).whichChildMother;
		}
		else
		{
			if (!d.parentIndex)										// ha meg volt adva, elfogadjuk
				d.parentIndex = 1;									// nem volt idõsebb testvére és nem volt elõírva a parentIndex
			d.whichChildFather = 1;
			d.whichChildMother = 1;
		}
	}
}
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// megállapítja a leszármazott apjának és anyjának rowid-jét és apja nevét veszi fel
bool CInputGA::setFatherMother1()
{
	int z;
	int ix;
	CString title;
	CString peer;

	// d.gen a descendant generációja, akinek az apját-anyját keressük
	// Megkeresi a v_generations vektorban a legközelebbi korábbi generációt
	// Ennek father_id-je és mother_id-je lesz az apja és az anyja.
	// A mother_id-t a parentIndex alapján választja ki.

	// az elsõ generációnak nincs apja,anyja ezért ha üres a v_generation, nincs semmi baj

	// a tábla elsõ generációjához tartozó leszármazottak szülei a vFolyt vektorból 
	// állapíthatóak meg, ha a tábla római száma megeyezik a vFolyt.at(i).folyt értékkel. 



//	d.father_id.Format(L"%d%s", NOFATHERID, d.tableNumber); 

	d.father_id = L"0";
	d.mother_id = L"0";
	if (d.generation == m_generationFirst)  // A tábla elsõ generációja
	{
		d.tableAncestry = true;
		for (int i = 0; i < vFolyt.size(); ++i)
		{
			if (d.tableRoman == vFolyt.at(i).folyt)
			{
				d.father_id = vFolyt.at(i).rowidD;
				d.mother_id = vFolyt.at(i).rowidS[d.parentIndex-1];
				return true;
			}
		}
	}

	// Más leszármazottak szülei a v_generation vektorból állapíthatóak meg
	if (v_generations.size())
	{
		ix = v_generations.size() - 1;
		if (d.generation < m_generationPrev)  // ha visszalép a generation, akkor törli a nála nagyaobb generációs bejegyzéseket
		{
			while (ix && d.generation < v_generations.at(ix).gen)
			{
				v_generations.pop_back();
				--ix;
			}
		}
		if (ix < 0)
		{
			AfxMessageBox(L"Generációs probléma!!");
			return false;
		}

// örökség az apától: father_is és mother_id meghatározása
		for (ix = v_generations.size() - 1; ix >= 0; --ix)	// hátulról elõre keresi az elõzõ generációt, aki a szülõ
		{
			if ( v_generations.at(ix).gen < d.generation)
				break;
		}
		if (ix != -1)										// az elõzõ ciklusban megtalálta az apát/anyát
		{
			if (v_generations.at(ix).descendant_sex_id == MAN)					// szülõ sex_id-ja
			{
				d.father_id = v_generations.at(ix).descendant_id;				// leszedi az apa azonosítóját
				if (d.parentIndex <= v_generations.at(ix).numOfSpouses)
				{
					d.mother_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];
//					if( v_generations.at(ix).numOfSpouses)
//						d.mother_id.Format(L"%d", _wtoi(d.father_id) + d.parentIndex);
				}
			}
			else																// ha a nõk leszármazottait is nyilvántartjuk
			{
				d.mother_id = v_generations.at(ix).descendant_id;
				d.father_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];				// leszedi az apa azonosítóját

// Az alábbi nem kell, mert a bemeneti gA.html.ben csak olyan nõk leszármazottai vannak megadva,
// akik a nevét átvették, tehát nem az apjuk neve fut tovább !! (KI tájékoztatása)
//				m_command.Format(L"SELECT last_name FROM people WHERE rowid='%s'", d.father_id);
//				if (!theApp.query4(m_command)) return false;
//				d.last_name = theApp.m_recordset4->GetFieldString(0);

//				if( v_generations.at(ix).numOfSpouses)
//				d.father_id.Format(L"%d", _wtoi(d.mother_id) + d.parentIndex);
			}
//			++v_generations.at(ix).numOfChildren;
		}
	}
	else
	{
		d.father_id = L"0";
		d.whichChildMother = 0;
		d.numOfChildren = 0;
	}
	if (d.father_id != L"0")
	{
		m_command.Format(L"UPDATE people SET numOfChildren = numOfChildren + 1 WHERE rowid = '%s'", d.father_id);
		if (!theApp.execute(m_command)) return false;
	}
	if (d.mother_id != L"0")
	{
		m_command.Format(L"UPDATE people SET numOfChildren = numOfChildren + 1 WHERE rowid = '%s'", d.mother_id);
		if (!theApp.execute(m_command)) return false;
	}

	return true;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Megállapítja a leszármazott apjának és anyjának rowid-jét, azaz a leszármazott father_id és mother-id értékeét
//
// A szülõk adatai a v_geneneratios vektorban vannak, amely mindig az éppen beolvasás alatt álló táblára vonatkozik
// 
// typedef struct
//{
//	TCHAR	gen;						// generáció sorszáma
//	CString	descendant_id;				// leszármazott rowid-je (d.rowid-t teszi bele)
//	CString spouse_id[MAXNUMOFSPOUSES];	// a v_marriages spouse_id értéket teszi bele
//	int		parentIndex;				// anya index ( 1,2,3,.... )
//	int		descendant_sex_id;			// leszármazott neme
//	int		whichChildFather;				// apja hagyadik gyereke
//	int		whichChildMother;				// anyja hanyadik gyereke	
//	int		numOfSpouses;				// házastársainak száma
//	CString familyName;					// családnév, 
//	CString title;						// elõnév
//	CString peer;						// fõnemesi rang
//} GENERATIONS;
//
// d.gen a descendant generációja, akinek az apját-anyját keressük.
// Megkeresi a v_generations vektorban a legközelebbi korábbi generációt, ami apjának a bejegyzését tartalmazza.
// Ennek descendant_id-je lesz az apa azonosítója, és a d.parentIndex alapján kiválasztzott spouse_id az anya azonosítója
//

//
bool CInputGA::setFatherMother()
{	
	int i;
	d.father_id.Empty();
	d.mother_id.Empty();
	// A tábla elsõ generációjához tartozó leszármazottak szülei a vFolyt vektorból állapíthatóak meg, ha a tábla római száma megegyezik a vFolyt.at(i).folyt értékkel. 	
	if (d.generation == m_generationFirst)  // A tábla elsõ generációja  ( több is lehet egy táblában, ha az folytatás tábla!!)
	{
		d.tableAncestry = true;
		for ( i = 0; i < vFolyt.size(); ++i)
		{
			if (d.tableRoman == vFolyt.at(i).folyt)
			{
				d.father_id = vFolyt.at(i).rowidD;
				d.mother_id = vFolyt.at(i).rowidS[d.parentIndex - 1];
				break;
			}
		}
	}
	else  // a tábla további leszármazottainak szülei a vGenerations vektorból állapíthatóak meg
	{
		int ix;
		if (v_generations.size())
		{
			ix = v_generations.size() - 1;
			if (d.generation < m_generationPrev)  // ha visszalép a generation, akkor törli a nála nagyaobb generációs bejegyzéseket
			{
				while (ix && d.generation < v_generations.at(ix).gen)
				{
					v_generations.pop_back();
					--ix;
				}
			}
			if (!m_rollToLine && ix < 0)
			{
				AfxMessageBox(L"Generációs probléma!!");
				return false;
			}

			for (ix = v_generations.size() - 1; ix >= 0; --ix)	// hátulról elõre keresi az elõzõ generációt, aki a szülõ
			{
				if (v_generations.at(ix).gen < d.generation)
					break;
			}
			if (ix != -1)										// az elõzõ ciklusban megtalálta az apát/anyát
			{
				if (v_generations.at(ix).descendant_sex_id == MAN)					// szülõ sex_id-ja
				{
					d.father_id = v_generations.at(ix).descendant_id;				// leszedi az apa azonosítóját
					if (d.parentIndex <= v_generations.at(ix).numOfSpouses)			// anya ismert ?
						d.mother_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];  // ismert
				}
				else																// ha a nõk leszármazottait is nyilvántartjuk
				{
					d.mother_id = v_generations.at(ix).descendant_id;
					if (d.parentIndex <= v_generations.at(ix).numOfSpouses)			// anya ismert ?
						d.father_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];  // ismert
				}
			}
		}
		if (d.father_id == L"0")
			d.father_id.Empty();
		if (d.mother_id == L"0")
			d.mother_id.Empty();


		if (!d.father_id.IsEmpty())
		{
			m_command.Format(L"UPDATE people SET numOfChildren = numOfChildren + 1 WHERE rowid = '%s'", d.father_id);
			if (!theApp.execute(m_command)) return false;

			m_command.Format(L"SELECT last_name FROM people WHERE rowid='%s'", d.father_id);
			if (!theApp.query4(m_command)) return false;
			d.last_name = theApp.m_recordset4->GetFieldString(0);
		}
		if (!d.mother_id.IsEmpty())
		{
			m_command.Format(L"UPDATE people SET numOfChildren = numOfChildren + 1 WHERE rowid = '%s'", d.mother_id);
			if (!theApp.execute(m_command)) return false;
		}
	}
	return true;
}