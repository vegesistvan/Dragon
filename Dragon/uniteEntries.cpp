// CUniteEntriesNew.cpp : implementation file
//

#include "pch.h"
#include "stdafx.h"
#include "Dragon.h"
#include <algorithm>
#include "displayInfo.h"
#include "uniteEntries.h"
#include "unitedEntries.h"

enum
{
	SLOW,
	FAST,
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// return TRUE ne cser�ljen
// return FALS cser�ljen
bool sortByGStatus(const USAME3& v1, const USAME3& v2)
{
	if (v1.group > v2.group)
		return false;
	if (v1.group == v2.group)
	{
		if (v1.status > v2.status)
			return true;
		else
			return false;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySourceU3(const USAME3& v1, const USAME3& v2)
{
	return(v1.source < v2.source);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cs�kken� gyereksz�m szerint rendezi
bool sortByCS(const USAME3& v1, const USAME3& v2)
{
	if (v1.numOfChildren > v2.numOfChildren)
		return true;
	if (v1.numOfChildren == v2.numOfChildren)
		return(v1.source < v2.source);
	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CUniteEntries, CWnd)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteEntries::CUniteEntries()
{
	m_rset = new CSqliteDBRecordSet;
	m_rset1 = new CSqliteDBRecordSet;
	m_rset2 = new CSqliteDBRecordSet;
	m_rset3 = new CSqliteDBRecordSet;
	m_rset4 = new CSqliteDBRecordSet;

	nItem = 0;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteEntries::~CUniteEntries()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUniteEntries, CWnd)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUniteEntries::parameteres()
{
	m_name = L"Abhortis M�ria";
	m_name.Empty();
	m_loopMax = 3;
	R1 = L"7125";
	R2 = L"7142";
	m_algorithm = FAST;

	m_filenameTextU.Format(L"%sU.txt", theApp.m_dbFileTitle);
	m_fileSpecTextU.Format(L"%s\\%s", theApp.m_dbFolderPath, m_filenameTextU);
	if (!openFileSpec(&textU, m_fileSpecTextU, L"w+")) return false;
	str.Format(L"Azonos nev� �s r�szben �sszevont bejegyz�sek a %s adatb�zisb�l\n", theApp.m_dbFileName );
	fwprintf( textU, str ); 
	
	m_filenameTextD.Format(L"%sD.txt", theApp.m_dbFileTitle);
	m_fileSpecTextD.Format(L"%s\\%s", theApp.m_dbFolderPath, m_filenameTextD);
	if (!openFileSpec(&textD, m_fileSpecTextD, L"w+")) return false;
	str.Format( L"Azonos nev� de egyetlen �sszevon�st sem tartalmaz� bejegyz�sek a %s adatb�zisb�l\n", theApp.m_dbFileName );
	fwprintf(textD, str );

	m_filenameTextX.Format(L"%sX.txt", theApp.m_dbFileTitle);
	m_fileSpecTextX.Format(L"%s\\%s", theApp.m_dbFolderPath, m_filenameTextX);
	
	m_filenameTextN.Format(L"%sN.txt", theApp.m_dbFileTitle);
	m_fileSpecTextN.Format(L"%s\\%s", theApp.m_dbFolderPath, m_filenameTextN);


	CString dbPathName;
	CString dbFileTitle;
	CString dbFileName;
	CString dbFolderPath;

	CString blobPathName;
	CString blobFileTitle;
	CString blobFileName;
	CString blobFolderPath;

	m_loop = 1;
	while (m_loop <= m_loopMax)
	{
		m_modified = false;				/// jelzi, ha �sszevon�s t�rt�nt a ciklusban (hoyg le�lljon, ha nincs )

		dbFileTitle.Format(L"%sU", theApp.m_dbFileTitle);
		dbFileName.Format(L"%s.db", dbFileTitle);
		dbPathName.Format(L"%s\\%s", theApp.m_dbFolderPath, dbFileName);
		CopyFile(theApp.m_dbPathName, dbPathName, false);
		theApp.m_dbPathName = dbPathName;
		theApp.m_dbFileTitle = dbFileTitle;
		theApp.m_dbFileName = dbFileName;

/*
		blobFileTitle.Format(L"%sU_blob", theApp.m_dbFileTitle);
		blobFileName.Format(L"%s.db", blobFileTitle);
		blobPathName.Format(L"%s\\%s", theApp.m_dbFolderPath, blobFileName);
		CopyFile(theApp.m_blobPathName, blobPathName, false);
		theApp.m_blobPathName = blobPathName;
		theApp.m_blobFileTitle = blobFileTitle;
		theApp.m_blobFileName = blobFileName;
*/			

		theApp.openDatabase( true );
		
		if (!openFileSpec(&textN, m_fileSpecTextN, L"w+")) return false;
		str.Format(L"Azonos nev�, egyez� �s k�l�nb�z� adatokat is tartalmaz� bejegyz�sek, amelyek nem lettek �sszevonva\n" );
		fwprintf(textN, str );

		if (!openFileSpec(&textX, m_fileSpecTextX, L"w+")) return false;
		str.Format(L"Azonos nev�, ellentmond�s mentes, de nem �sszevonhat� bejegyz�sek a %s adatb�zisb�l\n", theApp.m_dbFileName);
		fwprintf(textX, str);

		switch (m_algorithm)
		{
		case SLOW:
			slow();
			break;
		case FAST:
			if (!fast())
				m_loop = m_loopMax;
			break;
		}
		fclose(textN);
		fclose(textX);
		++m_loop;

		if (!m_modified) break;		// nincs �sszevont ember, v�ge a programnak
	}
	fclose(textU);
	fclose(textD);

	theApp.insertIntoFiles(m_filenameTextU, UNITED_FILE);
	theApp.insertIntoFiles(m_filenameTextD, DIFFERENT_FILE);
	theApp.insertIntoFiles(m_filenameTextN, ERROR_FILE);
	theApp.insertIntoFiles(m_filenameTextX, X_FILE);

	CUnitedEntries dlg;
	dlg.DoModal();

	return true;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Nagyon lass�!!! Egy�bk�nt nem rossz.
bool CUniteEntries::slow()
{
	LASTFIRST	lf;
	CString namePrev;
	CString nameR;
	CString name;

	CString lastname;
	CString firstname;

	USAME3 same;
	USPOUSES3 vspouses;
	int pos;
	int z;
	bool newName = true;
	CString spouse_id;
	CString birth;
	CString death;
	CString father_id;
	CString mother_id;

	CString spouse;
	CString nameBD;
	CString spouses;  // az �sszes h�zast�rs
	CString father;
	CString mother;

	m_term = false;

	str.Format(L"Azonos nev� bejegyz�sek �sszegy�jt�se �s vizsg�lata (%d. ciklus)", m_loop);
	CProgressWnd pWnd(NULL, str);
	pWnd.GoModal();

	vLF.clear();
	if (m_name.IsEmpty())
	{
		m_command.Format(L"SELECT last_name, first_name FROM people WHERE last_name != '' GROUP BY last_name, first_name ORDER BY last_name, first_name");
		if (!query(m_rset, m_command)) return false;

		for (UINT i = 0; i < m_rset->RecordsCount() - 1; ++i)
		{
			lf.lastname = m_rset->GetFieldString(0);
			lf.firstname = m_rset->GetFieldString(1);
			vLF.push_back(lf);
			m_rset->MoveNext();
		}
	}
	else
	{
		lf.lastname = getFirstWord(m_name);
		lf.firstname = lastWord(m_name);
		vLF.push_back(lf);
	}

#ifndef _DEBUG
	pWnd.SetText(L"Azonos nev� bejegyz�sek vizsg�lata....");
#endif

	pWnd.SetRange(0, vLF.size());
	pWnd.SetPos(0);
	pWnd.SetStep(1);

	theApp.execute(L"BEGIN");
	for (UINT i = 0; i < vLF.size(); ++i)
	{
		lastname = vLF.at(i).lastname;
		firstname = vLF.at(i).firstname;
		name.Format(L"%s %s", lastname, sepFirstName(firstname));
#ifndef _DEBUG
		pWnd.SetText(name);
#endif
		m_command.Format(L"SELECT rowid, lineNumber, source, sex_id, birth_date, death_date, father_id, mother_id, united, titolo, peer FROM people WHERE last_name='%s' AND first_name='%s'", lastname, firstname);
		if (!query(m_rset1, m_command)) return false;
		if (m_rset1->RecordsCount() < 2) goto cont;

		vSameNames.clear();
		vSpouses.clear();
		for (UINT j = 0; j < m_rset1->RecordsCount(); ++j, m_rset1->MoveNext())
		{
			same.status = 1;
			same.group = 0;
			same.rgbcolor = 0;
			same.match = 0;
			same.rowid = m_rset1->GetFieldString(0);

			same.name = name.Trim();
			same.line = m_rset1->GetFieldString(1);
			same.source = m_rset1->GetFieldString(2);
			same.sex_id = m_rset1->GetFieldString(3);
			same.birth = m_rset1->GetFieldString(4);
			same.death = m_rset1->GetFieldString(5);

			same.rowidF = m_rset1->GetFieldString(6);
			same.rowidM = m_rset1->GetFieldString(7);

			same.united = _wtoi(m_rset1->GetFieldString(8));
			same.titolo = m_rset1->GetFieldString(9);
			same.peer = m_rset1->GetFieldString(10);

			if (same.rowidF == L"0") same.rowidF.Empty();
			if (same.rowidM == L"0") same.rowidM.Empty();


			// apa
			if (!same.rowidF.IsEmpty())
			{
				getPeople(same.rowidF);
				same.lineF = p.line;
				same.sourceF = p.source;
				same.father = p.name;
				same.father = m_first_name;
				same.birthF = p.birth;
				same.deathF = p.death;
			}
			else
			{
				same.lineF.Empty();
				same.sourceF.Empty();
				same.father.Empty();
				same.birthF.Empty();
				same.deathF.Empty();
			}

			// anya
			if (!same.rowidM.IsEmpty())
			{
				getPeople(same.rowidM);
				same.lineM = p.line;
				same.sourceM = p.source;
				same.mother = p.name;
				same.birthM = p.birth;
				same.deathM = p.death;
			}
			else
			{
				same.lineM.Empty();
				same.sourceM.Empty();
				same.mother.Empty();
				same.birthM.Empty();
				same.deathM.Empty();
			}
			vSameNames.push_back(same);


			// h�zast�rsak vSpouses vektor�nak elk�sz�t�se

			// h�zast�rsak �s annak sz�lei
			if (same.sex_id == MAN )
				m_command.Format(L"SELECT wife_id FROM marriages WHERE husband_id = '%s'", same.rowid);
			else
				m_command.Format(L"SELECT husband_id FROM marriages WHERE wife_id = '%s'", same.rowid);
			if (!query(m_rset2, m_command)) return false;

			spouses.Empty();
			for (UINT j = 0; j < m_rset2->RecordsCount(); ++j)
			{
				vspouses.rowid = same.rowid;		// az ember azonos�t�ja!!!

				// h�zast�rs
				spouse_id = m_rset2->GetFieldString(0);
				if (spouse_id == L"251415" || spouse_id == L"133393")
					z = 1;

				getPeople(spouse_id);

				vspouses.rowidS = spouse_id;
				vspouses.nameS = p.name;
				vspouses.birthS = p.birth;
				vspouses.deathS = p.death;

				father_id = p.father_id;
				mother_id = p.mother_id;


				// h�zast�rs apja


				getPeople(father_id);
				vspouses.father = p.name;
				vspouses.father = m_first_name;
				vspouses.birthF = p.birth;
				vspouses.deathF = p.death;

				// h�zast�rs anyja

				getPeople(mother_id);
				vspouses.mother = p.name;
				vspouses.birthM = p.birth;
				vspouses.deathM = p.death;

				str = getNRBD( vspouses.nameS, vspouses.birthS, vspouses.deathS);
				if (vspouses.father.IsEmpty() && vspouses.mother.IsEmpty())
					vspouses.nameBD = str;
				else
					vspouses.nameBD.Format(L"%s (%s-%s)", str, vspouses.father, vspouses.mother);

				spouses += vspouses.nameBD.Trim();
				spouses += L", ";
				
				vSpouses.push_back(vspouses);

				m_rset2->MoveNext();
			}

			if (m_rset2->RecordsCount())
				spouses = spouses.Left(spouses.GetLength() - 2); // L", " lehagy�sa
			vspouses.spouses = spouses;
			vSameNames.at(vSameNames.size() - 1).spouses = spouses;
		}


		if (vSameNames.size())
			processSameNames();
	cont:	pWnd.StepIt();
		pWnd.PeekAndPump();
		if (pWnd.Cancelled())
		{
			m_term = true;
			break;
		}
	}
	pWnd.DestroyWindow();
	theApp.execute(L"COMMIT");
	deleteSameMarriages();
	whichHW();

	theApp.m_uniteVersion = theApp.m_version;
	m_command.Format(L"UPDATE properties SET uniteVersion = '%s'", theApp.m_uniteVersion );
	theApp.execute(m_command);

	if (m_term) return false;
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUniteEntries::fast()
{
	CString name1;
	CString name2;
	CString nameR;
	CString name;
	CString rowid;
	CString last_name;
	CString first_name;
	int i;
	int j;
	int z;
	m_term = false;

	ENTRIES sEntries;

	vSameNames.clear();
	vSpouses.clear();

	str.Format(L"Azonos nev� bejegyz�sek �sszegy�jt�se �s vizsg�lata (%d. ciklus)", m_loop);
	CProgressWnd pWnd(NULL, str);
	pWnd.GoModal();

	theApp.execute(L"BEGIN");
	m_command.Format(L"SELECT rowid, last_name, first_name FROM people ORDER BY last_name, first_name, source");
	if (!query(m_rset, m_command)) return false;

	// �tteszi a vEntries-be, hogy az �sszevon�skor t�r�lt bejegyz�seket megjel�lhesse �res rowid-val
	vEntries.clear();
	for (UINT i = 0; i < m_rset->RecordsCount(); ++i)
	{
		rowid		= m_rset->GetFieldString(0);
		last_name	= m_rset->GetFieldString(1);
		first_name	= m_rset->GetFieldString(2);
		name.Format(L"%s %s", last_name, sepFirstName(first_name));
		name.Trim();
		if (!name.IsEmpty())
		{
			sEntries.rowid = rowid;
			sEntries.name = name;
			vEntries.push_back(sEntries);
			if (!m_name.IsEmpty() && name.GetAt(0) > m_name.GetAt(0))
				break;
		}
		m_rset->MoveNext();
	}

	pWnd.SetRange(0, vEntries.size());
	pWnd.SetPos(0);
	pWnd.SetStep(1);
	for ( i = 0; i < vEntries.size(); ++i)
	{
		if (vEntries.at(i).rowid.IsEmpty()) goto cont;			// t�rl�sre ker�lt a kor�bbiakban �sszevon�s sor�n
		name1 = vEntries.at(i).name;
		
#ifndef _DEBUG
		pWnd.SetText(name1);
#endif
		
		if (!m_name.IsEmpty())							// ha m_name-ben mneg van adva egy n�v, akkor csak azt pr�b�lja �sszevonni.
		{
			if( m_name != name1.Left(m_name.GetLength() ) )		// csak vezet�k nevet is meg lehet adni m_name-ben!!
				goto cont;
		}

		if (name1 == L"Abaffy Anna")
		{
			z = 0;
			z = 1;
			m_name = name1.Left(m_name.GetLength());


		}

		for( j = i + 1; j < vEntries.size(); ++j )
		{
			name2 = vEntries.at(j).name;
			if (name1 == name2)
			{
				if (!vSameNames.size())
				{
					push(vEntries.at(i).rowid);
				}
				push(vEntries.at(j).rowid);
			}
			else
			{
				if (vSameNames.size())
				{
					if (!processSameNames()) goto cont;
				}
				vSameNames.clear();
				vSpouses.clear();
				break;
			}
		}
		i = j - 1; // az i ciklus for-ja megn�veli i �rt�k�t!!!
cont:	pWnd.StepIt();
		pWnd.PeekAndPump();
		if (pWnd.Cancelled())
		{
			m_term = true;
			break;
		}
	}
	pWnd.DestroyWindow();
	if (m_term)
	{
		theApp.execute(L"COMMIT");
		return false;
	}
	if (vSameNames.size())
	{
		if (!processSameNames())
		{
			theApp.execute(L"COMMIT");
			return false;
		}
	}
	theApp.execute(L"COMMIT");
	deleteSameMarriages();

	whichHW();

	theApp.m_uniteVersion = theApp.m_version;
	m_command.Format(L"UPDATE properties SET uniteVersion = '%s'", theApp.m_uniteVersion );
	theApp.execute(m_command);
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUniteEntries::push( CString rowid )
{
	USAME3 same;
	CString name;
	CString lastname;
	CString source;
	CString spouse_id;
	USPOUSES3 vspouses;
	int pos;
	int z;

//	m_rset->MoveTo(i);

	// ember�nk
//	same.rowid = m_rset->GetFieldString(0);

	same.rowid = rowid;
	if (same.rowid == L"108508")
		z = 1;
	m_command.Format(L"SELECT lineNumber, source, sex_id, last_name, first_name, birth_date, death_date, father_id, mother_id, united, titolo, peer,birth_place, death_place, comment FROM people WHERE rowid='%s'", same.rowid);
	if (!query(m_rset2, m_command)) return false;
	if (!m_rset2->RecordsCount())
	{
		z = 1;
		return false; // a bejegyz�st m�r t�r�lt�k, mert azonos emberek azonos rokona volt
	}

	same.source = m_rset2->GetFieldString(1);
	same.status = 1;
	same.group = 0;
	same.rgbcolor = 0;
	same.match = 0;

	lastname = m_rset2->GetFieldString(3);
	name.Format(L"%s %s", lastname, sepFirstName(m_rset2->GetFieldString(4)));
	same.name = name.Trim();

	same.birth = m_rset2->GetFieldString(5);
	same.death = m_rset2->GetFieldString(6);
	same.united = _wtoi(m_rset2->GetFieldString(9));
	same.titolo = m_rset2->GetFieldString(10);
	same.peer = m_rset2->GetFieldString(11);
	same.line = m_rset2->GetFieldString(0);
	same.source = m_rset2->GetFieldString(1);
	same.sex_id = m_rset2->GetFieldString(2);
	same.rowidF = m_rset2->GetFieldString(7);
	same.rowidM = m_rset2->GetFieldString(8);

	same.birthPlace = m_rset2->GetFieldString(12);
	same.deathPlace = m_rset2->GetFieldString(13);
	same.comment = m_rset2->GetFieldString(14);

	if (same.rowidF == L"0") same.rowidF.Empty();
	if (same.rowidM == L"0") same.rowidM.Empty();

	// apa
	if (!same.rowidF.IsEmpty())
	{
		getPeople(same.rowidF);
		same.lineF = p.line;
		same.sourceF = p.source;
		same.father = p.name;
		same.father = m_first_name;
		same.birthF = p.birth;
		same.deathF = p.death;
	}
	else
	{
		same.lineF.Empty();
		same.sourceF.Empty();
		same.father.Empty();
		same.birthF.Empty();
		same.deathF.Empty();
	}

	// anya
	if (!same.rowidM.IsEmpty())
	{
		getPeople(same.rowidM);
		same.lineM = p.line;
		same.sourceM = p.source;
		same.mother = p.name;
		same.birthM = p.birth;
		same.deathM = p.death;
	}
	else
	{
		same.lineM.Empty();
		same.sourceM.Empty();
		same.mother.Empty();
		same.birthM.Empty();
		same.deathM.Empty();
	}
	/*
	//h�zast�rs rowidja
	if (same.sex_id == MAN)
		m_command.Format(L"SELECT wife_id FROM marriages WHERE husband_id = '%s'", same.rowid);
	else
		m_command.Format(L"SELECT husband_id FROM marriages WHERE wife_id = '%s'", same.rowid);
	if (!query(m_rset3, m_command)) return false;

	spouse_id = m_rset3->GetFieldString(0);
	*/
	// gyerekek meghat�roz�sa

	CString first;
	CString birth;
	CString death;
	CString child;
	CString children;
	if (same.sex_id == MAN)
		m_command.Format(L"SELECT first_name, birth_date, death_date FROM people WHERE father_id = '%s'", same.rowid );
	else
		m_command.Format(L"SELECT first_name, birth_date, death_date FROM people WHERE mother_id = '%s'", same.rowid );
	if (!query(m_rset3, m_command)) return false;
	for (int i = 0; i < m_rset3->RecordsCount(); ++i, m_rset3->MoveNext())
	{
		first = m_rset3->GetFieldString(0);
		birth = m_rset3->GetFieldString(1);
		death = m_rset3->GetFieldString(2);
		child = getNRBD(first, birth, death);
/*
		if (birth.IsEmpty() && death.IsEmpty())
			child = first;
		else if (birth.IsEmpty())
			child.Format(L"%s +%s", first, death);
		else if (death.IsEmpty())
			child.Format(L"%s *%s", first, birth);
		else
			child.Format(L"%s *%s +%s", first, birth, death);
*/
		children += child;
		children += L",";
	}
	same.children = dropLastCharacter(children);
	same.numOfChildren = m_rset3->RecordsCount();
	vSameNames.push_back(same);


	// h�zast�rsak vSpouses vektor�nak elk�sz�t�se



	CString father_id;
	CString mother_id;

	CString spouse;
	CString nameBD;
	CString spouses;  // az �sszes h�zast�rs
	CString father;
	CString mother;

	CString sbd;
	CString fbd;
	CString mbd;

	if (same.rowid == R1)
		z = 1;

	// h�zast�rsak �s annak sz�lei
	if (same.sex_id == MAN )
		m_command.Format(L"SELECT wife_id FROM marriages WHERE husband_id = '%s'", same.rowid);
	else
		m_command.Format(L"SELECT husband_id FROM marriages WHERE wife_id = '%s'", same.rowid);
	if (!query(m_rset3, m_command)) return false;

	for (UINT j = 0; j < m_rset3->RecordsCount(); ++j)
	{
		vspouses.rowid = same.rowid;		// az ember azonos�t�ja!!!
		if (same.rowid == R1)
			z = 1;

		// h�zast�rs
		spouse_id = m_rset3->GetFieldString(0);
		if (spouse_id == L"251415" || spouse_id == L"133393")
			z = 1;

		getPeople(spouse_id);

		vspouses.rowidS = spouse_id;
		vspouses.nameS = p.name;
		vspouses.birthS = p.birth;
		vspouses.deathS = p.death;
		vspouses.sex = p.sex_id;
		vspouses.sourceS = p.source;

		sbd = getNRBD(vspouses.nameS, vspouses.birthS, vspouses.deathS);


		father_id = p.father_id;
		mother_id = p.mother_id;


		// h�zast�rs apja


		getPeople(father_id);
		vspouses.father = p.name;
		vspouses.father = m_first_name;
		vspouses.birthF = p.birth;
		vspouses.deathF = p.death;
		vspouses.sourceF = p.source;
		vspouses.rowidF = father_id;
		fbd = getNRBD(vspouses.father, vspouses.birthF, vspouses.deathF);

		// h�zast�rs anyja

		getPeople(mother_id);
		vspouses.mother = p.name;
		vspouses.birthM = p.birth;
		vspouses.deathM = p.death;
		vspouses.sourceM = p.source;
		vspouses.rowidM = mother_id;
		mbd = getNRBD(vspouses.mother, vspouses.birthM, vspouses.deathM);

		if (vspouses.father.IsEmpty() && vspouses.mother.IsEmpty())
			vspouses.nameBD = sbd;
		else
			vspouses.nameBD.Format(L"%s (%s-%s)", sbd, fbd, mbd );

		spouses += vspouses.nameBD.Trim();
		spouses += L", ";


		vSpouses.push_back(vspouses);

		m_rset3->MoveNext();
	}
	if (m_rset3->RecordsCount())
		spouses = spouses.Left(spouses.GetLength() - 2); // L", " lehagy�sa
	vspouses.spouses = spouses;
	vSameNames.at(vSameNames.size() - 1).spouses = spouses;
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUniteEntries::getPeople( CString rowid )
{

	m_command.Format(L"SELECT last_name, first_name, birth_date, death_date, father_id, mother_id, linenumber, sex_id, source FROM people WHERE rowid='%s'", rowid );
	if (!query(m_rset4, m_command)) return false;


	m_first_name = sepFirstName(m_rset4->GetFieldString(1));
	p.rowid = rowid;
	p.name.Format(L"%s %s", m_rset4->GetFieldString(0), sepFirstName(m_rset4->GetFieldString(1)));
	p.birth = m_rset4->GetFieldString(2);
	p.death = m_rset4->GetFieldString(3);
	p.father_id = m_rset4->GetFieldString(4);
	p.mother_id = m_rset4->GetFieldString(5);
	p.line = m_rset4->GetFieldString(6);
	p.sex_id = m_rset4->GetFieldString(7);
	p.source = m_rset4->GetFieldString(8);

	p.name.Trim();
	if (p.father_id == L"0") p.father_id.Empty();
	if (p.mother_id == L"0") p.mother_id.Empty();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUniteEntries::processSameNames()
{

	if (vSameNames.size() < 2) return false;
	UINT group = 1;
	UINT concordant;
	bool newGroup = false;
	CString rowid1;
	CString rowid2;
	int ret;
	int z;
	// alapbe�ll�t�s:	vSameNames.at(i).group == 0;	ebben a csoportban maradnak azok a bejegyz�sek, amelyek nem azonosak egyetlen m�s bejegyz�ssel sem 
	//					vSameNames.at(i).status == 1,	teh�t meg kell tartani
	//                  alaphelyzetben teh�t mindent meg kell tartani a 0 group-ban

	if (vSameNames.at(0).name == L"Agha Kata")
	{
		z = 0;
		z = 1;
		rowid1 = vSameNames.at(0).rowid;
		newGroup = false;
	}

//	std::sort(vSameNames.begin(), vSameNames.end(), sortBySourceU3);		// az azonos�t�s sorrendje fonotos! source= 1->4

	int questionable = 0;
	std::sort(vSameNames.begin(), vSameNames.end(), sortByCS);		// az azonos�t�s sorrendje fonotos! source= 1->4
	for ( UINT i = 0; i < vSameNames.size() - 1; ++i)
	{
		if (vSameNames.at(i).group) continue; // m�r csoporthoz tartozik, nem vizsg�lja
		rowid1 = vSameNames.at(i).rowid;
		newGroup = false;
		for (UINT j = i + 1; j < vSameNames.size(); ++j)
		{
			rowid2 = vSameNames.at(j).rowid;
			if (rowid1 == R1 && rowid2 == R2)
			{
				z = 1;
			}

			if (vSameNames.at(j).group) continue;												// m�r csoporthoz tartozik, nem vizsg�lja
			if (vSameNames.at(i).source == L"1" && vSameNames.at(j).source == L"1") continue;	// 2 lesz�rmazottatt nem lehet �sszevonni

			ret = identical(i, j);				// nincs ellentmond�s
			if( ret == 1)
			{
				vSameNames.at(i).group = group;		// csoportf�n�k
				vSameNames.at(j).group = group;		// i-vel azonos group;
				vSameNames.at(j).status = 0;		// t�r�lni kell 
				newGroup = true;
			}
			else if (ret == -1)
			{
				if (m_match || m_matchP || m_matchS)				// ellenmond�s van, de van azonos adat is!!! A textN f�jlba list�zzuk!
				{
					listSameNames(textN, i);
					listSameNames(textN, j);
					emptyLine(textN);
				}
			}
			else if( ret == 0 )					// nincs ellentmond�s, de azonosadat sincs;
			{
				++questionable;
			}

		}
		if (newGroup)
		{
			++group;			// volt csoportk�pz�s, j�n a k�vetkez�
		}
	}
	std::sort(vSameNames.begin(), vSameNames.end(), sortByGStatus);
	if (group > 1)
	{
		saveValues();	// A megtartand� bejegyz�sbe �tvissz�k amit akarunk
		uniteSamePeople();
		listPeople(textU);	// volt �sszevon�s
	}
	else
	{
		if (questionable)
			listPeople(textX);
		else
			listPeople(textD);	// nem volt �sszevon�s
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bemenet: i-vel �s j-vel adott bejegyz�s adatait �sszehosnl�tja.
// Kimenet: m_match - az azonos adatp�rok sz�ma
//	 return false - valamely adatp�rban ellentmond�s van
//          true - nincs ellentmond�s

int CUniteEntries::identical(UINT i, UINT j)
{

	vOthers.clear();
	USAME3 a = vSameNames.at(i);
	USAME3 b = vSameNames.at(j);
	int z;

	CString rowid1 = vSameNames.at(i).rowid;
	CString rowid2 = vSameNames.at(j).rowid;
	
	if (rowid1 == R1 && rowid2 == R2)
	{
		z = 1;
	}

	if (rowid1 == 110618 || rowid2 == 110618 )
		m_match = 1;

	m_match = 0; // same az m_match-et n�veli, ha azonos a k�t adat
	m_matchP = 0;
	m_matchS = 0;
	m_matchDummy = 0;


	B = same(a.birth, b.birth, &m_match);
	D = same(a.death, b.death, &m_match);
	F = same(a.father, b.father, &m_matchDummy);
	FB = same(a.birthF, b.birthF, &m_matchP);
	FD = same(a.deathF, b.deathF, &m_matchP);
	M = same(a.mother, b.mother, &m_matchDummy);
	MB = same(a.birthM, b.birthM, &m_matchP);
	MD = same(a.deathM, b.deathM, &m_matchP);

	S = sameSpouses(i, j);

	// az�rt csak az �sszes �sszehasonl�t�s ut�n �rt�kel�nk, hogy m_match-nek �s m_matchP-nek legyen �rt�ke,
	// hogy az ellentmon�st tartalmaz�, de egyez�st is mutat� bejegyz�seket elk�l�n�thess�k a 3. list�ba
	
	if( B==-1 || D==-1 || F==-1|| FB==-1 || FD==-1 || M==-1 || MB==-1 || MD==-1 || S==-1 )	// b�rmi ellentmond�s van
		return -1;
	
	// az adatok ellentmond�s mentessek
	if (m_match || m_matchP || m_matchS)
	{
		return 1;			// van legal�bb 1 egyez�s
	}
	else
		return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az azonos nev� rowid1 �s rowid 2 emberk h�zast�rsai a vSpouses vektorban vannak, amely az �sszes azonos nev�
// ember �sszes h�zast�rs�t tartalmazza. A vSpouse.rowid jelzi, hogy melyik f�rjhez tatozik egy feles�g.
// Ha a rowid1 �s rowid2 h�zast�rsai k�z�tt van azonos nev�, akkor ellen�rzi azok sz�let�si �s hal�loz�si d�tum�nak 
// egyez�s�t is, ha meg vannak adva.
// a h�zast�rsak a f�rjek rowid-ja szerint vannak rendezve

// return -1		kiz�r� ok, mert vannak azonos nev� h�zast�rsak, de azok adatai ellentmond�ak, nem azonosak.
// return 1			a h�zast�rsak egyez�s�t meger�s�ti:
// return 0			nincs kiz�r� ok, de meger�s�temi sem lehet az egyez�st
int CUniteEntries::sameSpouses(int i1, int i2)
{
	CString rowid1 = vSameNames.at(i1).rowid;
	CString rowid2 = vSameNames.at(i2).rowid;

	int z;
	int ret = -1;

	if (rowid1 == R1 && rowid2 == R2)
	{
		z = 1;
	}
	vSpouses1.clear();
	vSpouses2.clear();
	for (UINT i = 0; i < vSpouses.size(); ++i)
	{
		if (vSpouses.at(i).rowid == rowid1)
		{
			vSpouses1.push_back(vSpouses.at(i));
		}
		if (vSpouses.at(i).rowid == rowid2)
		{
			vSpouses2.push_back(vSpouses.at(i));
		}
	}
	if (vSpouses1.size() == 0 || vSpouses2.size() == 0) return 0;  // nem lehetnek �rt�kelni
	ret = oneSpouse( i1, i2 );
	return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Van e a k�t bejegyz�s h�zast�rsai k�z�tt van azonosnak �rt�kelhet� 
// return: false
// return: true; 
int CUniteEntries::oneSpouse(int i1, int i2)
{
	OTHERS sOthers;
	int i;
	int j;
	int z;

	CString name1;
	CString name2;

	CString wedding1;
	CString wedding2;

	CString birth1;
	CString birth2;

	CString death1;
	CString death2;

	CString father1;
	CString father2;
	CString mother1;
	CString mother2;
//	bool nameDiff = false;
	int retB;
	int retD;

	int retF;
	int retFB;
	int retFD;

	int retM;
	int retMB;
	int retMD;

	CString rowid1 = vSameNames.at(i1).rowid;
	CString rowid2 = vSameNames.at(i2).rowid;

	if (rowid1 == R1 && rowid2 == R2)
	{
		z = 2;
	}
	m_matchS = 0;
	for (i = 0; i < vSpouses1.size(); ++i)			// a vSpouses1 vektor az els� bejegyz�shez tartoz� h�zast�rsakat tartalmazza
	{
		for (j = 0; j < vSpouses2.size(); ++j)	// a vSpouses2 vektor a m�sodik bejegyz�shez tartoz� h�zast�rsakat tartalmazza
		{
			name1 = vSpouses1.at(i).nameS;
			name2 = vSpouses2.at(j).nameS;
			if (name1.IsEmpty()) continue;
			if (name1 == name2)				// azonos nev� h�zast�rsak
			{
				birth1 = vSpouses1.at(i).birthS;
				birth2 = vSpouses2.at(j).birthS;
				retB = same(birth1, birth2, &m_matchS);

				death1 = vSpouses1.at(i).deathS;
				death2 = vSpouses2.at(j).deathS;
				retD = same(death1, death2, &m_matchS);

				father1 = vSpouses1.at(i).father;
				father2 = vSpouses2.at(j).father;
				retF = same(father1, father2, &m_matchS);

				birth1 = vSpouses1.at(i).birthF;
				birth2 = vSpouses2.at(j).birthF;
				retFB = same(birth1, birth2, &m_matchS);

				death1 = vSpouses1.at(i).deathF;
				death2 = vSpouses2.at(j).deathF;
				retFD = same(death1, death2, &m_matchS);

				mother1 = vSpouses1.at(i).mother;
				mother2 = vSpouses2.at(j).mother;
				retM = same(mother1, mother2, &m_matchS);

				birth1 = vSpouses1.at(i).birthM;
				birth2 = vSpouses2.at(j).birthM;
				retMB = same(birth1, birth2, &m_matchS);

				death1 = vSpouses1.at(i).deathM;
				death2 = vSpouses2.at(j).deathM;
				retMD = same(death1, death2, &m_matchS);

				if ( retB == -1 || retD == -1 || retF == -1 || retFB == -1 || retFD == -1 || retM == -1 || retMB == -1 || retMD == -1)
					return -1;		// ellentmond�s van k�z�tt�k

				if ( childNameSame( i, j ) )
				{
					++m_matchS;
				}

				if (m_matchS)  // a k�t h�zasast�rs azonos, �ket is egyes�t�sre kijel�li a vOthers vektorban.
				{
					// ezt nem haszn�lja semmire!!!
					sOthers.rowid1 = vSpouses1.at(i).rowidS;
					sOthers.source1 = vSpouses1.at(i).sourceS;
					sOthers.rowid2 = vSpouses2.at(j).rowidS;
					sOthers.source2 = vSpouses2.at(j).sourceS;
					sOthers.sex = vSpouses1.at(i).sex;
					vOthers.push_back(sOthers);
					return 1;			// van tov�bbi azonos�t�, teh�t elfogadjuk!
					// ha sem ellentmond�s sem azonos�t�s nincs, akkor tov�bb megy, h�tha m�sik h�zast�rssal sikeres lesz. 
				}
			}
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  0 : csak az egyik vagy egy sincs van megadva, nme �rt�kelhet�
//  1 : mindkett� meg van adva �s egyezik; N�vel m_match �rt�k�t, ami az egyez� adatp�rok sz�ma
//  -1 : mindkett� meg van adva �s nem egyezik
int CUniteEntries::same(CString par1, CString par2, int* match)
{
	CString p1 = par1.Trim();
	CString p2 = par2.Trim();
	int len1 = p1.GetLength();
	int len2 = p2.GetLength();
	int len;
	
	if (len1 && len2)
	{
		len = (len1 < len2) ? (len1) : (len2);
		if (p1.Left(len) == p2.Left(len))
		{
			++*match;
			return 1;
		}
		else
			return -1;
	}
	return 0;
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// van-e azonos nev� gyereke???
bool CUniteEntries::childNameSame(int i, int j)
{
	CString rowidI = vSpouses1.at(i).rowid;
	CString rowidJ = vSpouses2.at(j).rowid;

	CString rowidS1 = vSpouses1.at(i).rowidS;
	CString rowidS2 = vSpouses2.at(j).rowidS;

	CString sex = vSpouses1.at(i).sex;  // ez a h�zast�rs sex-e!! 
	CString command1;
	CString command2;
	CString name1;
	CString name2;
	CString birth1;
	CString birth2;
	CString death1;
	CString death2;
	CString firstname1;
	CString firstname2;
	int retB;
	int retD;

	if (sex == MAN)
	{
		command1.Format(L"SELECT first_name, last_name, birth_date, death_date FROM people WHERE father_id == '%s'", rowidS1);
		command2.Format(L"SELECT first_name, last_name, birth_date, death_date FROM people WHERE father_id == '%s'", rowidS2);
	}
	else
	{
		command1.Format(L"SELECT first_name, last_name, birth_date, death_date FROM people WHERE father_id == '%s'", rowidI);
		command2.Format(L"SELECT first_name, last_name, birth_date, death_date FROM people WHERE father_id == '%s'", rowidJ);
	}
	if (!query( m_rset1, command1 )) return false;
	if (!query( m_rset2, command2 )) return false;

	m_matchC == 0;
	for (int i = 0; i < m_rset1->RecordsCount(); ++i, m_rset1->MoveNext() )
	{
		firstname1 = m_rset1->GetFieldString(0);
		name1.Format(L"%s %s", m_rset1->GetFieldString(1), m_rset1->GetFieldString(0));
		birth1 = m_rset1->GetFieldString(2);
		death1 = m_rset1->GetFieldString(3);
	
		for (int i = 0; i < m_rset2->RecordsCount(); ++i, m_rset2->MoveNext())
		{
			firstname2 = m_rset2->GetFieldString(0);
			name2.Format(L"%s %s", m_rset2->GetFieldString(1), m_rset2->GetFieldString(0));
			if (name1 == name2)
			{

				birth2 = m_rset2->GetFieldString(2);
				death2 = m_rset2->GetFieldString(3);
				retB = same(birth1, birth2, &m_matchC);
				retD = same(death1, death2, &m_matchC);

				if (retB == -1 || retD == -1)
					return false;
				return true;
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteEntries::uniteSamePeople()
{
	CString rowid1;
	CString rowid0;
	CString sex_id;

	CString rowidF1;
	CString rowidF2;
	CString sourceF1;
	CString sourceF2;

	CString rowidM1;
	CString rowidM2;
	CString sourceM1;
	CString sourceM2;
	CString temp;

	CString rowidS;
	CString source1;
	CString source2;
	CString rowid2;
	CString sex;
	
	UINT united;

	int z;
	int i;
	int j;
	
	for (int i = 0; i < vSameNames.size(); ++i)  // a vektorban t�bb csoport lehet, ezek sorrendben vannak, teh�t j�l m�k�dik
	{
		if (vSameNames.at(i).status == 1)
		{
			rowid1 = vSameNames.at(i).rowid;   // megtartand� bejegyz�s
			rowidF1 = vSameNames.at(i).rowidF;
			rowidM1 = vSameNames.at(i).rowidM;
			sourceF1 = vSameNames.at(i).sourceF;
			sourceM1 = vSameNames.at(i).sourceM;
		}
		else
		{
			rowid0 = vSameNames.at(i).rowid;   // t�r�lj�k
			sex_id = vSameNames.at(i).sex_id;
			deleteEntry(rowid1, rowid0, sex_id);

			// ap�k bejegyz�seinek �sszevon�sa
			if (!rowidF1.IsEmpty())
			{
				rowidF2 = vSameNames.at(i).rowidF;
				if (!rowidF2.IsEmpty())
				{
					sourceF2 = vSameNames.at(i).sourceF;
					if (sourceF1 < sourceF2)
						deleteEntry(rowidF1, rowidF2, L"1");
					else
						deleteEntry(rowidF2, rowidF1, L"1");
				}
			}


			// any�k bejegyz�seinek �sszevon�sa
			if (!rowidM1.IsEmpty())
			{
				rowidM2 = vSameNames.at(i).rowidM;
				if (!rowidM2.IsEmpty())
				{
					sourceM2 = vSameNames.at(i).sourceM;
					if (sourceM1 < sourceM2)
						deleteEntry(rowidM1, rowidM2, L"2");
					else
						deleteEntry(rowidM2, rowidM1, L"2");
				}
			}
			// h�zast�rsak �s sz�leinek egyes�t�se
/*
			for (int j = 0; j < vOthers.size(); ++j)
			{
				rowid1 = vOthers.at(j).rowid1;
				rowid2 = vOthers.at(j).rowid2;
				source1 = vOthers.at(j).source1;
				source2 = vOthers.at(j).source2;
				sex = vOthers.at(j).sex;

				if (source1 < source2)
					deleteEntry(rowid1, rowid2, sex);
				else
					deleteEntry(rowid2, rowid1, sex);
			}
*/
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// rowid1-et megtartja
// rowid0-t t�rli
bool CUniteEntries::deleteEntry(CString rowid1, CString rowid0, CString sex_id)
{
	int z;
	if (rowid1 == rowid0) return true;
	
	if (rowid0 == R2 && rowid1 == R1)
	{
		z = 1;
	}


	m_command.Format(L"DELETE FROM people WHERE rowid ='%s'", rowid0);
	if (!theApp.execute(m_command)) return false;

	//	deleteFromvEntries(rowid0);
		// Az emberek nyilv�ntart�s�ban �t�rja a gyerekek t�r�lt sz�l�j�nek bejegyz�s�t a megtartott bejegyz�sre
	if (sex_id == MAN )
		m_command.Format(L"UPDATE people SET father_id = '%s' WHERE father_id='%s'", rowid1, rowid0);
	else
		m_command.Format(L"UPDATE people SET mother_id = '%s' WHERE mother_id='%s'", rowid1, rowid0);
	if (!theApp.execute(m_command)) return false;

	
	// A h�zass�gok nyilv�ntart�s�ban �t�rja a t�r�lt bejegyz�st a megtartott bejegyz�sre
	// �lym�don az �sszevon�sok v�g�n egy h�zass�gr�l t�bb bejegyz�s is maradhat, amikb�l csak egyet kell megtartani!!
	if (sex_id == MAN )
		m_command.Format(L"UPDATE marriages SET husband_id='%s' WHERE husband_id ='%s'", rowid1, rowid0);
	else
		m_command.Format(L"UPDATE marriages SET wife_id='%s' WHERE wife_id ='%s'", rowid1, rowid0);
	if (!theApp.execute(m_command)) return false;

	m_command.Format(L"UPDATE people SET united=united+1 WHERE rowid ='%s'", rowid1);
	if (!theApp.execute(m_command)) return false;

	m_modified = true;	// inter�ci�n bel�l
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteEntries::deleteFromvEntries(CString rowid)
{
	return;
	for (int i = 0; i < vEntries.size(); ++i)
	{
		if (vEntries.at(i).rowid == rowid)
		{
			vEntries.at(i).rowid = L"";
			break;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUniteEntries::deleteSameMarriages()
{
	if (!m_modified) return true;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Az azonos (==rowid) h�zasp�rokat tartalmaz� h�zass�gokat t�rli.
	// A legkisebb szerepk�d� h�zass�got tartja meg ( ORDER BY ... source )

	int		i;
	int		j;
	int		z;
	CString rowid;

	CString husband_id1;
	CString husband_id2;
	CString wife_id1;
	CString wife_id2;

	CString place1;
	CString place2;
	CString date1;
	CString date2;

	CString whichHusband1;
	CString whichHusband2;
	CString whichWife1;
	CString whichWife2;

	CProgressWnd pWnd(NULL, L"Azonos emberek h�zass�gi bejegyz�seinek �sszevon�sa.");
	pWnd.GoModal();
	pWnd.SetPos(0);

	m_command = L"SELECT rowid, * FROM marriages ORDER BY husband_id, wife_id, source";
	if (!query(m_rset2, m_command)) return false;
	pWnd.SetRange(0, m_rset2->RecordsCount());


	theApp.execute(L"BEGIN");
	for ( i = 0; i < m_rset2->RecordsCount() - 1; ++i)
	{
		rowid			= m_rset2->GetFieldString(DBM_ROWID);	// az esetleg megtartand� h�zass�g rowid-je
		husband_id1		= m_rset2->GetFieldString(DBM_HUSBAND_ID);
		wife_id1		= m_rset2->GetFieldString(DBM_WIFE_ID );

		place1			= m_rset2->GetFieldString(DBM_PLACE);
		date1			= m_rset2->GetFieldString(DBM_DATE);
		whichHusband1	= m_rset2->GetFieldString(DBM_WHICHHUSBAND);
		whichWife1		= m_rset2->GetFieldString(DBM_WHICHWIFE);
		
		j = i+1;
		m_rset2->MoveTo(j);
		for ( ; j < m_rset2->RecordsCount() - 1; ++j)
		{
			husband_id2 = m_rset2->GetFieldString(DBM_HUSBAND_ID);
			wife_id2	= m_rset2->GetFieldString(DBM_WIFE_ID);
			if (husband_id1 != husband_id2 || wife_id1 != wife_id2)  // 1 �s 2 azonos emberek h�zass�gai. Az els�be pakolom az ott neml�tez� adatokat, azt�n t�rl�m �ket
				break;

			place2 = m_rset2->GetFieldString(DBM_PLACE);
			if (place1.IsEmpty() && !place2.IsEmpty())
			{
				m_command.Format(L"UPDATE marriages SET place= '%s' WHERE rowid = '%s'", place2, rowid);
				if (!theApp.execute(m_command)) return false;
			}

			date2 = m_rset2->GetFieldString(DBM_DATE);
			if (date1.IsEmpty() && !date2.IsEmpty())
			{
				m_command.Format(L"UPDATE marriages SET date= '%s' WHERE rowid = '%s'", date2, rowid);
				if (!theApp.execute(m_command)) return false;
			}

			whichHusband2 = m_rset2->GetFieldString(DBM_WHICHHUSBAND);
			if (whichHusband1.IsEmpty() && !whichHusband2.IsEmpty())
			{
				m_command.Format(L"UPDATE marriages SET whichHusband = '%s' WHERE rowid = '%s'", whichHusband2, rowid);
				if (!theApp.execute(m_command)) return false;
			}

			whichWife2 = m_rset2->GetFieldString(DBM_WHICHWIFE);
			if (whichWife1.IsEmpty() && !whichWife2.IsEmpty())
			{
				m_command.Format(L"UPDATE marriages SET whichWife = '%s' WHERE rowid = '%s'", whichWife2, rowid);
				if (!theApp.execute(m_command)) return false;
			}
			// kipakol�s ut�n t�rl�s
			m_command.Format(L"DELETE FROM marriages WHERE rowid = '%s'", m_rset2->GetFieldString(DBM_ROWID));
				if (!theApp.execute(m_command)) return false;
			m_rset2->MoveNext();
		}
		i = j-1;  // for ciklus n�velni fogja i-t!!!
		pWnd.StepIt();
		pWnd.PeekAndPump();
		if (pWnd.Cancelled()) break;
	}
	theApp.execute(L"COMMIT");
	pWnd.DestroyWindow();
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUniteEntries::saveValues()
{
	// vSameNames group �s cs�kken� status szerint rendezve
	int z;
	if (vSameNames.at(0).name == L"Andre�nszky Zsuzsa")
		z = 1;
	CString rowidI;
	CString rowidJ;
	CString itemI;
	CString itemJ;
	CString mark = L" ^^^";
	int group;
	int i;
	int j;
	int spouseIndexI;
	int spouseIndexJ;
	
	for ( i = 0; i < vSameNames.size(); ++i)
	{
		//megkeresi a k�vetket�  megtartand� bejegyz�s�t
		if (vSameNames.at(i).status == 1 )
		{
			group = vSameNames.at(i).group;
			rowidI = vSameNames.at(i).rowid;   // megtartand� bejegyz�s
			if (rowidI == L"26433")
				z = i;
			for( j = i; j < vSameNames.size(); ++j )
			{
				rowidJ = vSameNames.at(j).rowid;
				if( group == vSameNames.at(j).group && !vSameNames.at(j).status ) // az azonos csoportba tartoz� t�rlend� bejegyz�st keresi 
				{

					itemI = vSameNames.at(i).peer;
					itemJ = vSameNames.at(j).peer;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET peer='%s' WHERE rowid = '%s'", itemJ, rowidI);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).peer += mark;
					}
			
					itemI = vSameNames.at(i).titolo;
					itemJ = vSameNames.at(j).titolo;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET titolo='%s' WHERE rowid = '%s'", itemJ, rowidI);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).titolo += mark;
					}
					
					if (vSameNames.at(i).rowidF.IsEmpty() && !vSameNames.at(j).rowidF.IsEmpty() )
					{
						m_command.Format(L"UPDATE people SET father_id='%s' WHERE rowid = '%s'", vSameNames.at(j).rowidF, rowidI);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).father += mark;
					}
					
					if (vSameNames.at(i).rowidM.IsEmpty() && !vSameNames.at(j).rowidM.IsEmpty())
					{
						m_command.Format(L"UPDATE people SET mother_id='%s' WHERE rowid = '%s'", vSameNames.at(j).rowidM, rowidI);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).mother += mark;					
					}
					
					itemI = vSameNames.at(i).birth;
					itemJ = vSameNames.at(j).birth;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET birth_date='%s' WHERE rowid = '%s'", itemJ, rowidI);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).birth += mark;
					}
					
					itemI = vSameNames.at(i).death;
					itemJ = vSameNames.at(j).death;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET death_date='%s' WHERE rowid = '%s'", itemJ, rowidI);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).death += mark;
					}
					
					itemI = vSameNames.at(i).birthPlace;
					itemJ = vSameNames.at(j).birthPlace;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET birth_place='%s' WHERE rowid = '%s'", itemJ, rowidI);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).birthPlace += mark;
					}
					
					itemI = vSameNames.at(i).deathPlace;
					itemJ = vSameNames.at(j).deathPlace;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET death_place='%s' WHERE rowid = '%s'", itemJ, rowidI);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).deathPlace += mark;
					}
					
					itemI = vSameNames.at(i).comment;
					itemJ = vSameNames.at(j).comment;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET comment='%s' WHERE rowid = '%s'", itemJ, rowidI);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).comment += mark;
					}
					
					itemI = vSameNames.at(i).birthF;
					itemJ = vSameNames.at(j).birthF;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET birth_date='%s' WHERE rowid = '%s'", itemJ, vSameNames.at(i).rowidF);
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).birthF += mark;
					}
					
					itemI = vSameNames.at(i).deathF;
					itemJ = vSameNames.at(j).deathF;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET death_date='%s' WHERE rowid = '%s'", itemJ, vSameNames.at(i).rowidF );
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).deathF += mark;
					}

					itemI = vSameNames.at(i).birthM;
					itemJ = vSameNames.at(j).birthM;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET birth_date='%s' WHERE rowid = '%s'", itemJ, vSameNames.at(i).rowidM );
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).birthM += mark;
					}

					itemI = vSameNames.at(i).deathM;
					itemJ = vSameNames.at(j).deathM;
					if (itemI.GetLength() < itemJ.GetLength())
					{
						m_command.Format(L"UPDATE people SET death_date='%s' WHERE rowid = '%s'", itemJ, vSameNames.at(i).rowidM );
						if (!theApp.execute(m_command)) return false;
						vSameNames.at(i).deathM += mark;
					}

					spouseIndexI = vSameNames.at(i).spouseIndex;
					spouseIndexJ = vSameNames.at(j).spouseIndex;
					if (!spouseIndexI && spouseIndexJ)
					{
						m_command.Format(L"UPDATE people SET spouseIndex='%d' WHERE rowid = '%s'", spouseIndexJ, vSameNames.at(i).rowidM);
						if (!theApp.execute(m_command)) return false;
					}
				}
			}
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteEntries::listPeople( FILE* fl)
{
	for (UINT i = 0; i < vSameNames.size(); ++i)
	{
		listSameNames(fl, i);
	}
	emptyLine(fl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteEntries::listSameNames(FILE* fl, int i)
{
	USAME3 x;
	int status;
	int	group;
	int colorIndex;
	int	rgbColor;

	CString name;
	CString nameF;
	CString nameM;

	x = vSameNames.at(i);

	str.Format(L"\
0\t%d\t%d\t%d\t%d\t\
%d\t%s\t%s\t\
%s\t%s\t%s\t%s\t%s\t%s\t\
%s\t%s\t%s\t%s\t%s\t\
%s\t%s\t%s\t%s\t%s\t\
%s\t%s\t\
\n", \
m_loop, x.group, x.match, x.status, \
x.united, x.index, x.source, \
x.line, x.rowid, x.name, x.sex_id, x.birth, x.death, \
x.lineF, x.rowidF, x.father, x.birthF, x.deathF, \
x.lineM, x.rowidM, x.mother, x.birthM, x.deathM,\
x.spouses, x.children\
);

	fwprintf(fl, str);
	fflush(fl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteEntries::emptyLine(FILE* fl )
{
	for ( UINT i = 0; i < U_COLUMNSCOUNT; i++)	// egy �res sor az azonos nev� emberek ut�n
		fwprintf(fl, L"\t");
	fwprintf(fl, L"\n");

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUniteEntries::query(CSqliteDBRecordSet* r, CString command)
{
	if (r->Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, r->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A people t�bla lesz�rmazottainak whichHusband, whichWife �s numOfSpouses oszlop�nak meghat�roz�sa a marriages t�bl�b�l.
// Az�rt j�, hogy ezek az �rt�kerk a people t�bl�ban is megvannak, hogy a lesz�rmazotti lista k�sz�t�sekor
// ne kelljen a marriges t�bl�b�l meghat�rozni ezeket az �rt�keket.
void CUniteEntries::whichHW()
{
	int i;
	int j;
	CString rowid;
	CString sex;
	CString mother_id;
	CString father_id;
	CString child_id;
	CString parentIndex;
	CString whichHusband;
	CString whichWife;

	CString numOfSpouses;

	CProgressWnd pWnd(NULL, L"Apa �s anya index�nek meghat�roz�sa..." );
	pWnd.GoModal();
	
	// lesz�rmazottak lek�rdez�se
	m_command.Format(L"SELECT rowid, father_id, mother_id, sex_id FROM people WHERE source=1");
	if (!theApp.query(m_command)) return;

	pWnd.SetRange(0, theApp.m_recordset->RecordsCount() ) ;
	pWnd.SetPos(0);
	pWnd.SetStep(1);

	theApp.execute(L"BEGIN");
	for (i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid		= theApp.m_recordset->GetFieldString(0);
		father_id	= theApp.m_recordset->GetFieldString(1);
		mother_id	= theApp.m_recordset->GetFieldString(2);
		sex			= theApp.m_recordset->GetFieldString(3);

		// h�ny h�zast�rsa van rowid-nak?
		if (sex == MAN )
			m_command.Format(L"SELECT count(*)  FROM marriages WHERE husband_id = '%s'", rowid);
		else
			m_command.Format(L"SELECT count(*)  FROM marriages WHERE wife_id = '%s'", rowid);
		if (!theApp.query2(m_command)) return;
		numOfSpouses = theApp.m_recordset2->GetFieldString(0);


		m_command.Format(L"SELECT whichHusband, whichWife FROM marriages WHERE husband_id='%s' AND wife_id='%s'", father_id, mother_id);
		if (!theApp.query1(m_command)) return;
		whichHusband = theApp.m_recordset1->GetFieldString(0);
		whichWife = theApp.m_recordset1->GetFieldString(1);
		if (whichHusband.IsEmpty()) whichHusband = L"1";
		if (whichWife.IsEmpty()) whichWife = L"1";
		m_command.Format(L"UPDATE people SET whichHusband='%s', whichWife='%s', numOfSpouses='%s' WHERE rowid = '%s'", whichHusband, whichWife, numOfSpouses, rowid);
		if (!theApp.execute(m_command)) return;
		
		pWnd.StepIt();
		pWnd.PeekAndPump();
		if (pWnd.Cancelled())
			break;
	}
	pWnd.DestroyWindow();
	theApp.execute(L"COMMIT");
}