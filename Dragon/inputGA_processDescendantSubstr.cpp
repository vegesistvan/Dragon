#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A d struktúra elõállítása insertálásra
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processDescendantSubstring(CString cLine)
{
	int i;

	d.generation = cLine.GetAt(0);				// generáció karakterének leszedése
	cLine = cLine.Mid(2);						// G és szóköz leszedése

	if (m_tableAncestry)
		m_generationFirst = d.generation;
	if (d.generation == m_generationFirst)
		d.tableRoman = m_tableHeader.tableRoman;  // csak az õst jelöli meg, hogy az összekötésnél megtalálják
	else
		d.tableRoman.Empty();

	// Megjelölöli minden tábla ösével azonos generációt, hogy a connectFolyt() mindegyiket beállíthassa az õt hívó másik táblában lévõ 
	// szülökre.

	d.tableAncestry = m_tableAncestry;
	d.source = 1;
	

	// egy dummy családnevet tesz elé, és utána írja be a helyes elõ- és családnevet
	// Ez akkor jó pl. ha Farkas Jánost kellene szétbontani, ahol Farkas kerszetnév is lehet
	str.Format(L"XXXX %s", cLine);
	cLine = str.Trim();

	processPeopleStr(cLine, &d);   // ez üres d.last_name értéket ad vissza, hiszen az nincs a cLine-ban!!

	d.nFather = m_genCnt;

	if (d.sex_id == 0 && !v_marriages.size()) d.sex_id = 1;	// ha a leszármazott keresztnevébõl nem lehet megállapítani
																// nemét és házastársa sincs, akkor legyen férfi
	desc.nameSubstr = m_nameSubstr;
	desc.birthSubstr = m_birthSubstr;
	desc.deathSubstr = m_deathSubstr;
	d.titolo = m_tableHeader.titolo;

	d.last_name = m_familyName;   // a % megváltozttahatja, ezért nem az m_tableHeader.familiName-et használjuk!!

	// a parentIndex csak ott van megadva, ahol változás történt, az ezt követõ gyerekek értelemszerûen ugyanannak az anyának a gyerekei.
	// alább kitertjesztjük a parentIndexet minden testvérre
	setFatherMother();
	setFamilyName();
	m_generationPrev = d.generation;
}
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// parentIndex a html fájlban explicite megadott index, amit úgy kell értelmezni, hogy foolyamatosan öröklõdik 
void CInputGA::setParent2Index()
{
	PARENT2INDEX mx;
	int i;

	if (vParent2Index.size())			// ha visszalép a generation, akkor törli az utolsó generációs bejegyzéseket
	{
		if (d.generation < m_generationPrev)  // ha visszalép a generation, akkor törli az utolsó generációs bejegyzéseket
		{
			i = vParent2Index.size() - 1;
			while (i && d.generation < vParent2Index.at(i).generation)
			{
				vParent2Index.pop_back();
				--i;
			}
		}
	}

	int ix = vParent2Index.size() - 1;


	if ( ix >= 0  && d.generation == vParent2Index.at(ix).generation)
	{
		if (d.parentIndex == 0)   // ha a /n nincs megadva, akkor megnézi hogy ebben a generációban mi volt az utolsó parentIndex
			d.parentIndex= vParent2Index.at(ix).parentIndex;  // és azt használja
		else
			vParent2Index.at(ix).parentIndex = d.parentIndex;  // ha meg volt adva a parentIndex, akkor azt elteszi
	}
	else
	{
		mx.generation = d.generation;					// az adott generáció utolsó indexe
		mx.parentIndex = d.parentIndex;				// elteszi az aktuális beállítást
		mx.rowid.Format(L"%d", m_rowid + 1 );		// majd ez lesz a rowid-ja, ha insertálják
		mx.sex_id = d.sex_id;
		vParent2Index.push_back(mx);
	}
	d.nFather = m_rowid + 1;					// csak kisérlethez
	d.nMother = d.sex_id;
	if (d.parentIndex == 0) ++d.parentIndex;	// az egyetlen házasságból született gyereknél nincs kitéve az /1, da azért õ az 1 feleség gyereke

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// megállapítja a leszármazott apjának és anyjának rowid-jét és apja nevét veszi fel
void CInputGA::fillFatherMother()
{
	int i;
	int ix;
	TCHAR gen;
	CString title;

	// d.gen a descendant generációja, akinek az apját-anyját keressük
	// Megkeresi a v_generations vektorban a legközelebbi korábbi generációt
	// Ennek father_id-je és mother_id-je lesz az apja és az anyja.
	// A mother_id-t a parentIndex alapján választja ki.

	// az elsõ generációnak nincs apja,anyja ezért ha üres a v_generation, nincs semmi baj

	if (v_generations.size())			
	{
		if (d.generation < m_generationPrev)  // ha visszalép a generation, akkor törli az utolsó generációs bejegyzéseket
		{
			i = v_generations.size() - 1;
			while (i && d.generation < v_generations.at(i).gen )
			{
				v_generations.pop_back();
				--i;
			}
		}
	}

	if (v_generations.size())
	{
		for (i = v_generations.size() - 1; i >= 0; --i)			// hátulról elõre keresi az elõzõ generációt, aki az apa
		{
			gen = v_generations.at(i).gen;
			if (gen < d.generation)
				break;
		}
		if (i != -1)															// az elõzõ ciklusban megtalálta az apát/anyát
		{
			if (v_generations.at(i).descendant_sex_id == 1)					// szülõ sex_id-ja
			{
				d.father_id = v_generations.at(i).descendant_id;				// leszedi az apa azonosítóját

				if (d.parentIndex <= v_generations.at(i).numOfSpouses)
					d.mother_id = v_generations.at(i).spouse_id[d.parentIndex - 1];

				d.orderMother = v_generations.at(i).orderMother;
				++v_generations.at(i).orderMother;
			}
			else																// ha a nõk leszármazottait is nyilvántartjuk
			{
				d.mother_id = v_generations.at(i).descendant_id;
				d.father_id = v_generations.at(i).spouse_id[d.parentIndex - 1];				// leszedi az apa azonosítóját
				if (v_marriages.size() > 1)
					ix = 1;
			}
			if (d.sex_id == 1 && v_generations.at(i).descendant_sex_id == 1)
			{
				title = v_generations.at(i).title;
				if (title == L"gr" || title == L"br" || title == L"hg")
					d.title = v_generations.at(i).title;
			}
			++v_generations.at(i).numOfChildren;
		}
	}
	else
	{
		d.father_id.Empty();
		d.orderMother = 0;
		d.numOfChildren = 0;
		

	}

}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// megállapítja a leszármazott apjának és anyjának rowid-jét és apja nevét veszi fel
bool CInputGA::setFatherMother()
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
// örökség az elõzõ  testvértõl: parentIndex meghatározása
		if (ix >= 0 && d.generation == v_generations.at(ix).gen)   // volt-e korábban testvére?
		{															// volt már beolvasott testvére
			if (d.parentIndex == 0)   // ha a /n nincs megadva, akkor megnézi hogy a már beolvasott testvérénak mi volt a parentIndex-e
				d.parentIndex = v_generations.at(ix).parentIndex;  // és azt használja
			else
				v_generations.at(ix).parentIndex = d.parentIndex;  // ha meg volt adva a parentIndex, akkor azt elteszi
		}
		if (d.parentIndex == 0) ++d.parentIndex;	// az egyetlen házasságból született gyereknél nincs kitéve az /1, da azért õ az 1 feleség gyereke
		if (d.parentIndex < 0)
			z = 1;



// örökség az apától: father_is és mother_id meghatározása

		for (ix = v_generations.size() - 1; ix >= 0; --ix)			// hátulról elõre keresi az elõzõ generációt, aki a szülõ
		{
			if ( v_generations.at(ix).gen < d.generation)
				break;
		}

		if (ix != -1)															// az elõzõ ciklusban megtalálta az apát/anyát
		{
			if (v_generations.at(ix).descendant_sex_id == 1)					// szülõ sex_id-ja
			{
				d.father_id = v_generations.at(ix).descendant_id;				// leszedi az apa azonosítóját

				if (d.parentIndex <= v_generations.at(ix).numOfSpouses)
					d.mother_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];

				d.orderMother = v_generations.at(ix).orderMother;
				++v_generations.at(ix).orderMother;
			}
			else																// ha a nõk leszármazottait is nyilvántartjuk
			{
				d.mother_id = v_generations.at(ix).descendant_id;
				d.father_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];				// leszedi az apa azonosítóját
			}
			
			if (d.sex_id == 1 && v_generations.at(ix).descendant_sex_id == 1)	// a fõnemesi címeket azért nem tudjuk örökíteni
			{																	// mert elõtzte a folytatás táblák és a család-törzs	 
				title = v_generations.at(ix).title;								// kapcsoilatokat kell összekötni
				if (title == L"gr" || title == L"br" || title == L"hg")
					d.title = v_generations.at(ix).title;
			}
			// a fõnemesi rangot csak azután lehet örökíteni, ha a foly-táblákat és a család-törzs táblákat is összekötöttük. Ezért ez ki van kommentzve
			//peer = v_generations.at(ix).peer;			
			//if (d.peer.IsEmpty())						// ha magának nincs fõnemesi rangja, akkor az apjáét örökli
			//{
			//	if (peer = L"báró") peer = L"br";
			//	else if (peer = L"gróf") peer = L"gr";
			//	else if (peer = L"herceg", peer = L"hg");
			//	d.peer == peer;
			//}

			++v_generations.at(ix).numOfChildren;
		}

	}
	else
	{
		d.father_id.Empty();
		d.orderMother = 0;
		d.numOfChildren = 0;
		d.parentIndex = 1;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Megállapítja a leszármazott családnevét, ami alapesetben az apja neve vagy ha nincs apja, akkor a táblafejléc családneve.
// Ha a leszármazotti sor elõtt % newnam (m_known_as) volt, akkor az lesz a családneve, az fog megõrzõdni az adtabázisban a gyermekei számára
void CInputGA::setFamilyName()
{
	if (m_known_as.IsEmpty())
	{
		if (m_tableAncestry)
			d.last_name = m_familyName;
		else
		{
			m_command.Format(L"SELECT last_name FROM people WHERE rowid='%s'", d.father_id);
			if (!theApp.query4(m_command)) return;
			d.last_name = theApp.m_recordset4->GetFieldString(0);
		}
	}
	else
	{
		d.last_name = m_known_as;
		d.known_as = m_known_as;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Táblánként: v_orderFather-ben generációnként nyilvántartja, hogy apja hanyadik gyermeke a leszármazott
// Ebbõl veszi ki az leszárm,azott orderFather értékét.
void CInputGA::fillOrderFather()
{
	int i;
	int z;
	TCHAR gen;
	ORDERFATHER orderFather;

	d.orderFather = m_lineNumber;

	if (v_orderFather.size())
	{
		for (i = v_orderFather.size() - 1; i >= 0; --i)		// hátulról visszafele töröl minden magasabb generációt. Ez nem lesz jó!! A magasabb generáció még visszatérjhet!!
		{
			gen = v_orderFather.at(i).gen;					// a jelenleginél nagyobb ( késõbbi ) generációkat eldobja
			if (i && (gen > d.generation))
			{
				v_orderFather.pop_back();
			}
		}

		i = v_orderFather.size() - 1;
		gen = v_orderFather.at(i).gen;					// saját generációjánál van
		if (gen == d.generation)							// az elõzõ ciklusban megtalálta a generációt
		{
			++v_orderFather.at(i).orderFather;
			d.orderFather = v_orderFather.at(i).orderFather;
			m_familyName = v_orderFather.at(i).familyname;
			++v_orderFather.at(i).orderMother;
			d.orderMother = v_orderFather.at(i).orderMother;
		}
		else												// új, magasabb gereráció
		{
			orderFather.gen = d.generation;
			orderFather.orderFather = 1;
			orderFather.orderMother = 1;
			orderFather.familyname = m_familyName;
			v_orderFather.push_back(orderFather);
			d.orderFather = 1;
			d.orderMother = 1;
		}
	}
	else
	{
		orderFather.gen = d.generation;
		orderFather.orderFather = 1;
		orderFather.orderMother = 1;
		orderFather.familyname = m_familyName;
		v_orderFather.push_back(orderFather);
		d.orderFather = 1;
		d.orderMother = 1;
	}
}