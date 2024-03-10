#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A d strukt�ra el��ll�t�sa insert�l�sra
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processDescendantSubstring(CString cLine)
{
	int i;

	d.generation = cLine.GetAt(0);				// gener�ci� karakter�nek leszed�se
	cLine = cLine.Mid(2);						// G �s sz�k�z leszed�se

	if (m_tableAncestry)
		m_generationFirst = d.generation;
	if (d.generation == m_generationFirst)
		d.tableRoman = m_tableHeader.tableRoman;  // csak az �st jel�li meg, hogy az �sszek�t�sn�l megtal�lj�k
	else
		d.tableRoman.Empty();

	// Megjel�l�li minden t�bla �s�vel azonos gener�ci�t, hogy a connectFolyt() mindegyiket be�ll�thassa az �t h�v� m�sik t�bl�ban l�v� 
	// sz�l�kre.

	d.tableAncestry = m_tableAncestry;
	d.source = 1;
	

	// egy dummy csal�dnevet tesz el�, �s ut�na �rja be a helyes el�- �s csal�dnevet
	// Ez akkor j� pl. ha Farkas J�nost kellene sz�tbontani, ahol Farkas kerszetn�v is lehet
	str.Format(L"XXXX %s", cLine);
	cLine = str.Trim();

	processPeopleStr(cLine, &d);   // ez �res d.last_name �rt�ket ad vissza, hiszen az nincs a cLine-ban!!

	d.nFather = m_genCnt;

	if (d.sex_id == 0 && !v_marriages.size()) d.sex_id = 1;	// ha a lesz�rmazott keresztnev�b�l nem lehet meg�llap�tani
																// nem�t �s h�zast�rsa sincs, akkor legyen f�rfi
	desc.nameSubstr = m_nameSubstr;
	desc.birthSubstr = m_birthSubstr;
	desc.deathSubstr = m_deathSubstr;
	d.titolo = m_tableHeader.titolo;

	d.last_name = m_familyName;   // a % megv�ltozttahatja, ez�rt nem az m_tableHeader.familiName-et haszn�ljuk!!

	// a parentIndex csak ott van megadva, ahol v�ltoz�s t�rt�nt, az ezt k�vet� gyerekek �rtelemszer�en ugyanannak az any�nak a gyerekei.
	// al�bb kitertjesztj�k a parentIndexet minden testv�rre
	setFatherMother();
	setFamilyName();
	m_generationPrev = d.generation;
}
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// parentIndex a html f�jlban explicite megadott index, amit �gy kell �rtelmezni, hogy foolyamatosan �r�kl�dik 
void CInputGA::setParent2Index()
{
	PARENT2INDEX mx;
	int i;

	if (vParent2Index.size())			// ha visszal�p a generation, akkor t�rli az utols� gener�ci�s bejegyz�seket
	{
		if (d.generation < m_generationPrev)  // ha visszal�p a generation, akkor t�rli az utols� gener�ci�s bejegyz�seket
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
		if (d.parentIndex == 0)   // ha a /n nincs megadva, akkor megn�zi hogy ebben a gener�ci�ban mi volt az utols� parentIndex
			d.parentIndex= vParent2Index.at(ix).parentIndex;  // �s azt haszn�lja
		else
			vParent2Index.at(ix).parentIndex = d.parentIndex;  // ha meg volt adva a parentIndex, akkor azt elteszi
	}
	else
	{
		mx.generation = d.generation;					// az adott gener�ci� utols� indexe
		mx.parentIndex = d.parentIndex;				// elteszi az aktu�lis be�ll�t�st
		mx.rowid.Format(L"%d", m_rowid + 1 );		// majd ez lesz a rowid-ja, ha insert�lj�k
		mx.sex_id = d.sex_id;
		vParent2Index.push_back(mx);
	}
	d.nFather = m_rowid + 1;					// csak kis�rlethez
	d.nMother = d.sex_id;
	if (d.parentIndex == 0) ++d.parentIndex;	// az egyetlen h�zass�gb�l sz�letett gyerekn�l nincs kit�ve az /1, da az�rt � az 1 feles�g gyereke

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// meg�llap�tja a lesz�rmazott apj�nak �s anyj�nak rowid-j�t �s apja nev�t veszi fel
void CInputGA::fillFatherMother()
{
	int i;
	int ix;
	TCHAR gen;
	CString title;

	// d.gen a descendant gener�ci�ja, akinek az apj�t-anyj�t keress�k
	// Megkeresi a v_generations vektorban a legk�zelebbi kor�bbi gener�ci�t
	// Ennek father_id-je �s mother_id-je lesz az apja �s az anyja.
	// A mother_id-t a parentIndex alapj�n v�lasztja ki.

	// az els� gener�ci�nak nincs apja,anyja ez�rt ha �res a v_generation, nincs semmi baj

	if (v_generations.size())			
	{
		if (d.generation < m_generationPrev)  // ha visszal�p a generation, akkor t�rli az utols� gener�ci�s bejegyz�seket
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
		for (i = v_generations.size() - 1; i >= 0; --i)			// h�tulr�l el�re keresi az el�z� gener�ci�t, aki az apa
		{
			gen = v_generations.at(i).gen;
			if (gen < d.generation)
				break;
		}
		if (i != -1)															// az el�z� ciklusban megtal�lta az ap�t/any�t
		{
			if (v_generations.at(i).descendant_sex_id == 1)					// sz�l� sex_id-ja
			{
				d.father_id = v_generations.at(i).descendant_id;				// leszedi az apa azonos�t�j�t

				if (d.parentIndex <= v_generations.at(i).numOfSpouses)
					d.mother_id = v_generations.at(i).spouse_id[d.parentIndex - 1];

				d.orderMother = v_generations.at(i).orderMother;
				++v_generations.at(i).orderMother;
			}
			else																// ha a n�k lesz�rmazottait is nyilv�ntartjuk
			{
				d.mother_id = v_generations.at(i).descendant_id;
				d.father_id = v_generations.at(i).spouse_id[d.parentIndex - 1];				// leszedi az apa azonos�t�j�t
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
// meg�llap�tja a lesz�rmazott apj�nak �s anyj�nak rowid-j�t �s apja nev�t veszi fel
bool CInputGA::setFatherMother()
{
	int z;
	int ix;
	CString title;
	CString peer;

	// d.gen a descendant gener�ci�ja, akinek az apj�t-anyj�t keress�k
	// Megkeresi a v_generations vektorban a legk�zelebbi kor�bbi gener�ci�t
	// Ennek father_id-je �s mother_id-je lesz az apja �s az anyja.
	// A mother_id-t a parentIndex alapj�n v�lasztja ki.

	// az els� gener�ci�nak nincs apja,anyja ez�rt ha �res a v_generation, nincs semmi baj

	if (v_generations.size())
	{
		ix = v_generations.size() - 1;
		if (d.generation < m_generationPrev)  // ha visszal�p a generation, akkor t�rli a n�la nagyaobb gener�ci�s bejegyz�seket
		{
			while (ix && d.generation < v_generations.at(ix).gen)
			{
				v_generations.pop_back();
				--ix;
			}
		}
		if (ix < 0)
		{
			AfxMessageBox(L"Gener�ci�s probl�ma!!");
			return false;
		}
// �r�ks�g az el�z�  testv�rt�l: parentIndex meghat�roz�sa
		if (ix >= 0 && d.generation == v_generations.at(ix).gen)   // volt-e kor�bban testv�re?
		{															// volt m�r beolvasott testv�re
			if (d.parentIndex == 0)   // ha a /n nincs megadva, akkor megn�zi hogy a m�r beolvasott testv�r�nak mi volt a parentIndex-e
				d.parentIndex = v_generations.at(ix).parentIndex;  // �s azt haszn�lja
			else
				v_generations.at(ix).parentIndex = d.parentIndex;  // ha meg volt adva a parentIndex, akkor azt elteszi
		}
		if (d.parentIndex == 0) ++d.parentIndex;	// az egyetlen h�zass�gb�l sz�letett gyerekn�l nincs kit�ve az /1, da az�rt � az 1 feles�g gyereke
		if (d.parentIndex < 0)
			z = 1;



// �r�ks�g az ap�t�l: father_is �s mother_id meghat�roz�sa

		for (ix = v_generations.size() - 1; ix >= 0; --ix)			// h�tulr�l el�re keresi az el�z� gener�ci�t, aki a sz�l�
		{
			if ( v_generations.at(ix).gen < d.generation)
				break;
		}

		if (ix != -1)															// az el�z� ciklusban megtal�lta az ap�t/any�t
		{
			if (v_generations.at(ix).descendant_sex_id == 1)					// sz�l� sex_id-ja
			{
				d.father_id = v_generations.at(ix).descendant_id;				// leszedi az apa azonos�t�j�t

				if (d.parentIndex <= v_generations.at(ix).numOfSpouses)
					d.mother_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];

				d.orderMother = v_generations.at(ix).orderMother;
				++v_generations.at(ix).orderMother;
			}
			else																// ha a n�k lesz�rmazottait is nyilv�ntartjuk
			{
				d.mother_id = v_generations.at(ix).descendant_id;
				d.father_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];				// leszedi az apa azonos�t�j�t
			}
			
			if (d.sex_id == 1 && v_generations.at(ix).descendant_sex_id == 1)	// a f�nemesi c�meket az�rt nem tudjuk �r�k�teni
			{																	// mert el�tzte a folytat�s t�bl�k �s a csal�d-t�rzs	 
				title = v_generations.at(ix).title;								// kapcsoilatokat kell �sszek�tni
				if (title == L"gr" || title == L"br" || title == L"hg")
					d.title = v_generations.at(ix).title;
			}
			// a f�nemesi rangot csak azut�n lehet �r�k�teni, ha a foly-t�bl�kat �s a csal�d-t�rzs t�bl�kat is �sszek�t�tt�k. Ez�rt ez ki van kommentzve
			//peer = v_generations.at(ix).peer;			
			//if (d.peer.IsEmpty())						// ha mag�nak nincs f�nemesi rangja, akkor az apj��t �r�kli
			//{
			//	if (peer = L"b�r�") peer = L"br";
			//	else if (peer = L"gr�f") peer = L"gr";
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
// Meg�llap�tja a lesz�rmazott csal�dnev�t, ami alapesetben az apja neve vagy ha nincs apja, akkor a t�blafejl�c csal�dneve.
// Ha a lesz�rmazotti sor el�tt % newnam (m_known_as) volt, akkor az lesz a csal�dneve, az fog meg�rz�dni az adtab�zisban a gyermekei sz�m�ra
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
// T�bl�nk�nt: v_orderFather-ben gener�ci�nk�nt nyilv�ntartja, hogy apja hanyadik gyermeke a lesz�rmazott
// Ebb�l veszi ki az lesz�rm,azott orderFather �rt�k�t.
void CInputGA::fillOrderFather()
{
	int i;
	int z;
	TCHAR gen;
	ORDERFATHER orderFather;

	d.orderFather = m_lineNumber;

	if (v_orderFather.size())
	{
		for (i = v_orderFather.size() - 1; i >= 0; --i)		// h�tulr�l visszafele t�r�l minden magasabb gener�ci�t. Ez nem lesz j�!! A magasabb gener�ci� m�g visszat�rjhet!!
		{
			gen = v_orderFather.at(i).gen;					// a jelenlegin�l nagyobb ( k�s�bbi ) gener�ci�kat eldobja
			if (i && (gen > d.generation))
			{
				v_orderFather.pop_back();
			}
		}

		i = v_orderFather.size() - 1;
		gen = v_orderFather.at(i).gen;					// saj�t gener�ci�j�n�l van
		if (gen == d.generation)							// az el�z� ciklusban megtal�lta a gener�ci�t
		{
			++v_orderFather.at(i).orderFather;
			d.orderFather = v_orderFather.at(i).orderFather;
			m_familyName = v_orderFather.at(i).familyname;
			++v_orderFather.at(i).orderMother;
			d.orderMother = v_orderFather.at(i).orderMother;
		}
		else												// �j, magasabb gerer�ci�
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