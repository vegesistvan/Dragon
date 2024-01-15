#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A bementi f�jlban a lesz�rmazottak csak keresztn�vvel szerepelnek.
// A lesz�rmazottak el�neve a %%% t�bla fejl�c�b�l van kiszedve az m_tableHeader.titolo-ba.
// Alapesetben a lesz�rmazottak csal�dneve a t�bla fejl�c�b�l vannak leszedve.
// A csal�dn�v v�ltoz�st a lesz�rmazott el�tti sorban l�v� % name2 jelzi. Ilyenkor az m_familyName
// ezt az �rt�ket kapja meg. Ez csak egy �jabb % name2 v�ltoztathatja meg.
//
// Ha a bemeneten n�k lesz�rmazottai is szerepelnek, akkor azok apjuk el�nev�t �s csal�dnev�t kapj�k meg.
// Az apa a lesz�rmazott h�zast�rsak�nt teljes n�vvel van megadva.
// 
// 
// 
// A d strukt�ra el��ll�t�sa insert�l�sra
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processDescendantSubstring(CString cLine)
{
	int i;
	int z;

	d.generation = cLine.GetAt(0);				// gener�ci� karakter�nek leszed�se
	cLine = cLine.Mid(2);						// G �s sz�k�z leszed�se

	if (m_tableAncestry)						// a t�bla els� embere, �se
	{
		m_generationFirst = d.generation;
	}
	if (d.generation == m_generationFirst)
		d.tableRoman = m_tableHeader.tableRoman;  // csak az �st jel�li meg, hogy az �sszek�t�sn�l megtal�lj�k
	else
		d.tableRoman.Empty();

	// Megjel�l�li minden t�bla �s�vel azonos gener�ci�t, hogy a connectFolyt() mindegyiket be�ll�thassa az �t h�v� m�sik t�bl�ban l�v� 
	// sz�l�kre.

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
	splitToSubstr(cLine);		// felbont�s:m_nameSubstr, m_birthSubstr, m_deathSubstr

	d.parentIndex = 0;

	if ((pos = m_nameSubstr.Find('?')) != -1)
	{
		m_nameSubstr.SetAt(pos, ' ');
		m_nameSubstr.Remove('?');
	}


	n = wordList(&A, m_nameSubstr, ' ', false);
	// anya index meghat�roz�sa
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

// 	// d.whichHusband = m_genCnt;	// az a csal�don bel�li gener�ci�k sorsz�ma!! Semmi k�ze az whichHusband-hez!!
							//Csak egy �jgyakorlat.

	if (d.sex_id.IsEmpty() && !v_marriages.size()) d.sex_id = MAN;	// ha a lesz�rmazott keresztnev�b�l nem lehet meg�llap�tani
																// nem�t �s h�zast�rsa sincs, akkor legyen f�rfi
	
	d.titolo	= m_tableHeader.titolo;
	d.last_name = m_familyName;	//a n�i lesz�rmazottak gyerekei a 'setFatherMother'-ben kapj�k meg a csal�dnevet!!! 
	if (d.first_name == L"Soma")
	{
		z = 1;
	}
	// a parentIndex csak ott van megadva, ahol v�ltoz�s t�rt�nt, az ezt k�vet� gyerekek �rtelemszer�en ugyanannak az any�nak a gyerekei.
	// al�bb kitertjesztj�k a parentIndexet minden testv�rre

	setParentIndex();	

	setFatherMother();

	m_generationPrev = d.generation;


	// csak a html list�khoz kell
	descS.nameSubstr = m_nameSubstr;
	descS.birthSubstr = m_birthSubstr;
	descS.deathSubstr = m_deathSubstr;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A bemenetben explicite nem megadott parentIndexek be�ll�t�sa
// Ha el� volt �rva, akkor az marad.
// He nem volt el��rva �s
//			nincs el�tte testv�re, akkor a parentInedex 1 lesz
//			van el�tte testv�re, akkor annak a parentIndex�t kapja	
void CInputGA::setParentIndex()
{
	if (v_generations.size() < 1)
	{
		if (!d.parentIndex)
			d.parentIndex = 1;
		return;
	}

	int ix = v_generations.size() - 1;
	while (ix > 1 && d.generation < v_generations.at(ix).gen)		// eldobjuk a nagyobb gener�ci�kat, a legutuls� testv�rt keress�k
	{
		v_generations.pop_back();
		--ix;
	}

//	if (ix >= 0)	// volt testv�re
	{
		// parentIndex az el�z�  testv�rt�l
		if (d.generation == v_generations.at(ix).gen)				// volt-e kor�bban testv�re?
		{
			if (!d.parentIndex)									// ha nem volt megadva, akkor az el�z� testv�r parentIndex-�t kapja
				d.parentIndex = v_generations.at(ix).parentIndex;
			d.whichWife = d.parentIndex;
			++v_generations.at(ix).whichChildFather;
			d.whichChildFather = v_generations.at(ix).whichChildFather;
			++v_generations.at(ix).whichChildMother;				// csak akkor j�, ha egyetlen feles�ge van az ap�nak
			d.whichChildMother = v_generations.at(ix).whichChildMother;
		}
		else
		{
			if (!d.parentIndex)										// ha meg volt adva, elfogadjuk
				d.parentIndex = 1;									// nem volt id�sebb testv�re �s nem volt el��rva a parentIndex
			d.whichChildFather = 1;
			d.whichChildMother = 1;
		}
	}
}
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// meg�llap�tja a lesz�rmazott apj�nak �s anyj�nak rowid-j�t �s apja nev�t veszi fel
bool CInputGA::setFatherMother1()
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

	// a t�bla els� gener�ci�j�hoz tartoz� lesz�rmazottak sz�lei a vFolyt vektorb�l 
	// �llap�that�ak meg, ha a t�bla r�mai sz�ma megeyezik a vFolyt.at(i).folyt �rt�kkel. 



//	d.father_id.Format(L"%d%s", NOFATHERID, d.tableNumber); 

	d.father_id = L"0";
	d.mother_id = L"0";
	if (d.generation == m_generationFirst)  // A t�bla els� gener�ci�ja
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

	// M�s lesz�rmazottak sz�lei a v_generation vektorb�l �llap�that�ak meg
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

// �r�ks�g az ap�t�l: father_is �s mother_id meghat�roz�sa
		for (ix = v_generations.size() - 1; ix >= 0; --ix)	// h�tulr�l el�re keresi az el�z� gener�ci�t, aki a sz�l�
		{
			if ( v_generations.at(ix).gen < d.generation)
				break;
		}
		if (ix != -1)										// az el�z� ciklusban megtal�lta az ap�t/any�t
		{
			if (v_generations.at(ix).descendant_sex_id == MAN)					// sz�l� sex_id-ja
			{
				d.father_id = v_generations.at(ix).descendant_id;				// leszedi az apa azonos�t�j�t
				if (d.parentIndex <= v_generations.at(ix).numOfSpouses)
				{
					d.mother_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];
//					if( v_generations.at(ix).numOfSpouses)
//						d.mother_id.Format(L"%d", _wtoi(d.father_id) + d.parentIndex);
				}
			}
			else																// ha a n�k lesz�rmazottait is nyilv�ntartjuk
			{
				d.mother_id = v_generations.at(ix).descendant_id;
				d.father_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];				// leszedi az apa azonos�t�j�t

// Az al�bbi nem kell, mert a bemeneti gA.html.ben csak olyan n�k lesz�rmazottai vannak megadva,
// akik a nev�t �tvett�k, teh�t nem az apjuk neve fut tov�bb !! (KI t�j�koztat�sa)
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
// Meg�llap�tja a lesz�rmazott apj�nak �s anyj�nak rowid-j�t, azaz a lesz�rmazott father_id �s mother-id �rt�ke�t
//
// A sz�l�k adatai a v_geneneratios vektorban vannak, amely mindig az �ppen beolvas�s alatt �ll� t�bl�ra vonatkozik
// 
// typedef struct
//{
//	TCHAR	gen;						// gener�ci� sorsz�ma
//	CString	descendant_id;				// lesz�rmazott rowid-je (d.rowid-t teszi bele)
//	CString spouse_id[MAXNUMOFSPOUSES];	// a v_marriages spouse_id �rt�ket teszi bele
//	int		parentIndex;				// anya index ( 1,2,3,.... )
//	int		descendant_sex_id;			// lesz�rmazott neme
//	int		whichChildFather;				// apja hagyadik gyereke
//	int		whichChildMother;				// anyja hanyadik gyereke	
//	int		numOfSpouses;				// h�zast�rsainak sz�ma
//	CString familyName;					// csal�dn�v, 
//	CString title;						// el�n�v
//	CString peer;						// f�nemesi rang
//} GENERATIONS;
//
// d.gen a descendant gener�ci�ja, akinek az apj�t-anyj�t keress�k.
// Megkeresi a v_generations vektorban a legk�zelebbi kor�bbi gener�ci�t, ami apj�nak a bejegyz�s�t tartalmazza.
// Ennek descendant_id-je lesz az apa azonos�t�ja, �s a d.parentIndex alapj�n kiv�lasztzott spouse_id az anya azonos�t�ja
//

//
bool CInputGA::setFatherMother()
{	
	int i;
	d.father_id.Empty();
	d.mother_id.Empty();
	// A t�bla els� gener�ci�j�hoz tartoz� lesz�rmazottak sz�lei a vFolyt vektorb�l �llap�that�ak meg, ha a t�bla r�mai sz�ma megegyezik a vFolyt.at(i).folyt �rt�kkel. 	
	if (d.generation == m_generationFirst)  // A t�bla els� gener�ci�ja  ( t�bb is lehet egy t�bl�ban, ha az folytat�s t�bla!!)
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
	else  // a t�bla tov�bbi lesz�rmazottainak sz�lei a vGenerations vektorb�l �llap�that�ak meg
	{
		int ix;
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
			if (!m_rollToLine && ix < 0)
			{
				AfxMessageBox(L"Gener�ci�s probl�ma!!");
				return false;
			}

			for (ix = v_generations.size() - 1; ix >= 0; --ix)	// h�tulr�l el�re keresi az el�z� gener�ci�t, aki a sz�l�
			{
				if (v_generations.at(ix).gen < d.generation)
					break;
			}
			if (ix != -1)										// az el�z� ciklusban megtal�lta az ap�t/any�t
			{
				if (v_generations.at(ix).descendant_sex_id == MAN)					// sz�l� sex_id-ja
				{
					d.father_id = v_generations.at(ix).descendant_id;				// leszedi az apa azonos�t�j�t
					if (d.parentIndex <= v_generations.at(ix).numOfSpouses)			// anya ismert ?
						d.mother_id = v_generations.at(ix).spouse_id[d.parentIndex - 1];  // ismert
				}
				else																// ha a n�k lesz�rmazottait is nyilv�ntartjuk
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