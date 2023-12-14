#include "stdafx.h"
#include "Dragon.h"
#include "GEDCOM_input.h"
#include <algorithm>
#include "ProgressWnd.h"
#include "utilities.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::printVectors()
{
	fl = theApp.openLogFile(L"vectors", L"");
	if (fl == NULL) return;

	listVindi();
	printFAM();

	listFamiliesByName();
	listPeople();
	
	fclose(fl);
	theApp.showFile(theApp.m_listFile);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::listVindi()
{
	CString ref;
	CString last_name;
	CString first_name;
	CString name;
	CString birth;
	CString death;
	CString families;
	CString child;
	std::sort( v_indi.begin(), v_indi.end(), multiSort_INDI_name );
	fwprintf(fl, L"\n\n v_indi f�jlban l�v� emberek\n\n");
	fwprintf(fl, L"%-5s %-10s %-30s  %-10s  %-10s %-10s csal�dok, amelyekben f�rj vagy feles�g\n", L" ", L"INDI", L"n�v", L"sz�letett", L"meghalt", L"FAMC");
	for (UINT i = 0; i < v_indi.size(); ++i)
	{
		ref = v_indi.at(i).refI;
		last_name = v_indi.at(i).last_name;
		first_name = v_indi.at(i).first_name;
		name.Format(L"%s %s", last_name, first_name);
		birth = v_indi.at(i).birth_date;
		death = v_indi.at(i).death_date;
		child = v_indi.at(i).refF;
		fwprintf(fl, L"%4d. %-10s %-30s *%-10s +%-10s %10s ", i+1, ref, name, birth, death, child );


		families.Empty();
		for (UINT j = 0; j < v_fams.size(); ++j)
		{
			if (v_fams.at(j).refI == v_indi.at(i).refI)
			{
				families += v_fams.at(j).refF;
				families += L" ";
			}
		}
		fwprintf(fl, L"%s\n", families);
	}
	v_indi.clear();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::printFAM()
{
	str.Format(L"%s f�jlban l�v� csal�dok list�z�sa...", theApp.m_gedFileTitle);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, (int)v_fam.size());
	wndP.SetPos(0);
	wndP.SetStep(1);

	CString father;
	CString mother;
	CString f;
	CString m;

	fwprintf(fl, L"\n\nv_fam vektor list�ja\n\n");
	for (int i = 0; i < v_fam.size(); ++i)
	{
		father = v_fam.at(i).refH;
		mother = v_fam.at(i).refW;
		fwprintf(fl, L"%10s %10s %10s ", v_fam.at(i).refF, father, mother );
		for (int j = 0; j < v_chil.size(); ++j)
		{
			if (v_chil.at(j).refH == father && v_chil.at(j).refW == mother)
			{
				fwprintf(fl, L"%10s ", v_chil.at(j).refC);
			}
		}
		fwprintf(fl, L"\n");


		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::listPeople()
{
	CString name;

	fwprintf( fl, L"\n\nEmberek �s csal�djaik \n\n");
	fwprintf( fl, L"1. az ember azonos�t�ja\n" );
	fwprintf( fl, L"2. Neve\n" );
	fwprintf( fl, L"3. Apja hanyadik gyermeke\n" );
	fwprintf( fl, L"4. Anyja hanyadik gyermeke\n" );
	fwprintf( fl, L"5. /Apja hanyadik feles�g�nek gyermeke\n" );
	fwprintf( fl, L"6. /Apja hanyadik feles�g�nek gyermeke\n" );
	fwprintf( fl, L"7. Anyja hanyadik gyermeke\n" );
	fwprintf( fl, L"8. H�zast�rsainak sz�ma\n\n\n" );

	for( UINT i= 0; i < v_indi.size(); ++i )
	{
		name.Format( L"%s %s", v_indi.at(i).last_name, v_indi.at(i).first_name );
	
		fwprintf( fl, L"%-10s %-30s %2d %2d /%d /%d %2d *%-10s +%-10s\n",
v_indi.at(i).refI,\
name,\
v_indi.at(i).whichChildFather,\
v_indi.at(i).whichChildMother,\
v_indi.at(i).parentIndex,\
v_indi.at(i).parentIndex,\
v_indi.at(i).numOfSpouses,\
v_indi.at(i).birth_date,\
v_indi.at(i).death_date\
);
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::listFamiliesByName()
{
	CString info = L"\n\n\
A csal�d els� sora: csal�dindex, a f�rj �s a feles�g neve valamint\n\
X/Y a f�rj h�zass�g�nak sorsz�ma/�sszes h�zass�ga,\n\
X/Y a fels�g h�zass�g�nak sorsz�ma/�sszes h�zass�ga,\n\
A k�vetkez� sorokban a gyerekek nevei vannak, �s ha az ap�nak t�bb feles�ge volt, akkor az anya index�vel."; 


	int indexH;
	int indexW;
	CString husband;
	CString wife;
	
	CString refF;
	CString refH;
	CString refW;
	CString refC;
	

	fwprintf( fl, L"\n\n%20s\n\n", L"CSAL�DOK A GEDCOM F�JLBAN" );
	fwprintf( fl, L"GEDCOM f�jl: %s\n", theApp.m_gedPathName );
	fwprintf( fl, L"D�tum	   : %s\n\n", theApp.getPresentDate() );
	fwprintf( fl, L"%s\n\n", info );

	fwprintf( fl, L"1. csal�d azonos�t�\n" );
	fwprintf( fl, L"2. f�rj azobos�t�\n" );
	fwprintf( fl, L"3. f�rj neve\n" );
	fwprintf( fl, L"4. feles�g azonos�t�\n" );
	fwprintf( fl, L"5. felas�g neve\n" );
	fwprintf( fl, L"6. f�rj h�zass�g�nak sorsz�ma/�sszes h�zass�ga\n" );
	fwprintf( fl, L"7. feles�g h�zass�gainak sz�am/�sszes h�zass�ga\n\n" );


	fwprintf( fl, L"Gyerekekn�l:\n" );
	fwprintf( fl, L"1. Apja hanyadik gyermeke\n" );
	fwprintf( fl, L"2. Anyja hanyadik gyermeke\n" );
	fwprintf( fl, L"3. gyerek azonos�t�ja\n" );
	fwprintf( fl, L"4. gyerek neve\n" );
	fwprintf( fl, L"5. /anyja hagyadik feles�ge apj�nak\n" );
	fwprintf( fl, L"6. /anyja hagyadik feles�ge apj�nak\n\n" );;

	std::sort( v_fam.begin(), v_fam.end(), sortByCnt ); 
	for( UINT i= 0; i < v_fam.size(); ++i )
	{
		refF = v_fam.at(i).refF;
		refH = v_fam.at(i).refH;
		refW = v_fam.at(i).refW;
		
		indexH = getIndexIndi(refH );
		indexW = getIndexIndi(refW );
		 
		husband = getIndi( refH );
		wife	= getIndi( refW );


		fwprintf( fl, L"%-10s %-10s %-28s %-10s %-28s %2d/%d %2d/%d\n",
		refF,\
		refH,\
		husband,\
		refW,\
		wife,\
		v_fam.at(i).marriageH,\
		v_fam.at(i).marriageHAll,\
		v_fam.at(i).marriageW,\
		v_fam.at(i).marriageWAll\
		);

		for( UINT j = 0; j < v_chil.size(); ++j )
		{
			if( v_chil.at(j).refF == v_fam.at(i).refF )
			{
				refC	= v_chil.at(j).refC;
				if( v_fam.at(i).marriageHAll > 1 )
				{
					fwprintf( fl, L"%4d. %3d. %-10s %s/%d /%d\n",\
					v_chil.at(j).whichChildFather,\
					v_chil.at(j).whichChildMother,\
					refC,\
					getIndi( refC),\
					v_chil.at(j).parentIndex,\
					v_chil.at(j).parentIndex );
				}
				else
				{
					fwprintf( fl, L"%4d. %3d. %-10s %s\n",\
					v_chil.at(j).whichChildFather,\
					v_chil.at(j).whichChildMother,\
					refC,\
					getIndi( refC ) );
				}
			}
		}
		fwprintf( fl, L"\n" );

	}
	listMultiMarriages();

	int indexI;
	CString refFPrev(L"");
	std::sort( v_famc.begin(), v_famc.end(), sort_refF );
	fwprintf( fl, L"\n\nindifam rekords rendezett\n\n" );
	for( UINT i = 0; i < v_famc.size(); ++i )
	{
		indexI = getIndexIndi( v_famc.at(i).refI );
		if( refFPrev != v_famc.at(i).refF ) fwprintf( fl, L"\n" );
		fwprintf( fl, L"%s %s %d\n", v_famc.at(i).refF, v_famc.at(i).refI, indexI );
		refFPrev = v_famc.at(i).refF;
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool multiSort_refH(const CHIL &v1, const CHIL &v2) 
{ 
	if( v1.refH == v2.refH ) 
		return( v1.refW < v2.refW ); 
    return( v1.refH < v2.refH ); 
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool multiSort_INDI_name(const INDI& v1, const INDI& v2)
{
	if (v1.last_name == v2.last_name)
		return(v1.first_name < v2.first_name );
	return(v1.last_name < v2.last_name);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool multiSort_FAM_refH(const FAM &v1, const FAM &v2) 
{ 
	if( v1.refH == v2.refH ) 
		return( v1.refW < v2.refW ); 
    return( v1.refH < v2.refH ); 
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool multiSort_FAM_refW(const FAM &v1, const FAM &v2) 
{ 
	if( v1.refW == v2.refW ) 
		return( v1.refH < v2.refH ); 
    return( v1.refW < v2.refW ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByCnt(const FAM &v1, const FAM &v2) 
{ 
    return( v1.cnt < v2.cnt ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sort_refF(const INDIFAMC &v1, const INDIFAMC &v2) 
{ 
	if( v1.refF == v2.refF ) 
		return( v1.refI < v2.refI ); 
	return( v1.refF < v2.refF ); 
}
bool sortRefF(const INDIFAMS &v1, const INDIFAMS &v2) 
{ 
	return( v1.refF < v2.refF ); 
}
bool sortSexRefI(const INDIFAMS &v1, const INDIFAMS &v2) 
{ 
	if( v1.sex == v2.sex ) 
		return( v1.refI < v2.refI ); 
    return( v1.sex < v2.sex ); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// O B S O L A T E ////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INDI melyik FAm-ban van mint HUSB vagy WIFE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::indiFams()
{
	CString name1;
	CString name2;
	CString refSpouse;
	int		total;
	fwprintf(fl, L"\n\nINDI mely FAM-ban szerepel mint HUSB vagy WIFE (v_ini �s v_fam alapj�n)\n\n");

	std::sort(v_fams.begin(), v_fams.end(), sortSexRefI); // refH szerint rendezi: a f�rj feles�geivel k�t�tt h�zass�gai egym�s ut�n lesznek

	fwprintf(fl, L"%20s %2s %20s\n", L"h�zast�rs", L"m", L"csal�d");
	for (UINT i = 0; i < v_fams.size(); ++i)
	{
		name1 = getIndi(v_fams.at(i).refI);
		total = getMarriageAll(v_fams.at(i).refI);
		refSpouse = getRefSpouse(v_fams.at(i).sex, v_fams.at(i).refF);
		name2 = getIndi(refSpouse);
		str.Format(L"%s %20s %2d/%d %20s %-20s %-20s", v_fams.at(i).sex, v_fams.at(i).refI, v_fams.at(i).order, total, v_fams.at(i).refF, name1, name2);
		fwprintf(fl, L"%s\n", str);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGEDCOMInput::getRefSpouse(CString sex, CString refF)
{
	CString ref;
	for (int i = 0; i < v_fam.size(); ++i)
	{
		if (v_fam.at(i).refF == refF)
		{
			if (sex == MAN )
				ref = v_fam.at(i).refW;
			else
				ref = v_fam.at(i).refH;
			break;
		}
	}
	return(ref);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a v_fam vektor elemeinek marriage �s marraigeHAll �rt�kad�s
void CGEDCOMInput::listMultiMarriages()
{
	std::sort(v_fam.begin(), v_fam.end(), sortByCnt);		// az�rt kell, hogy a RefW szerinti rendez�sban az azonosak sorrendje rekonstru�lhat� legyen
	std::sort(v_fam.begin(), v_fam.end(), multiSort_FAM_refH); // refH szerint rendezi: a f�rj feles�geivel k�t�tt h�zass�gai egym�s ut�n lesznek

	CString refF;

	CString refW;
	CString refH;

	CString refW1;
	CString refW2;

	CString refH1;
	CString refH2;

	CString refHPrev(L"");
	CString refWPrev(L"");

	CString husband;
	CString wife;

	int		marriageHX;
	int		marriageHAll;
	int		marriageWX;
	int		marriageWAll;
	int		numOfChildren;


	fwprintf(fl, L"\n\n\nF�rj t�bbsz�r�s h�zass�gai\n\n\n");
	fwprintf(fl, L"1. X/Y - a f�rj h�zass�g�nak sorsz�ma/�sszes h�zass�ga\n");
	fwprintf(fl, L"2. X/Y - a feles�g h�zass�gnak sorsz�ma/�sszes h�zass�ga\n");
	fwprintf(fl, L"3. sz�m: a h�zass�gban sz�letett gyerekek sz�ma\n\n");
	for (UINT i = 0; i < v_fam.size() - 1; ++i)
	{
		refH1 = v_fam.at(i).refH;
		refH2 = v_fam.at(i + 1).refH;

		if (refH1 == refH2)						// i-ben �s i+i-ben azonosf�rjek 
		{
			if (refHPrev != refH1)					// nem ugyanaz a f�rj mint az el�z�, teh�t �j p�r!!
			{
				refF = v_fam.at(i).refF;
				refW = v_fam.at(i).refW;
				husband = getIndi(refH1);
				wife = getIndi(refW);
				marriageHX = v_fam.at(i).marriageH;
				marriageHAll = v_fam.at(i).marriageHAll;
				marriageWX = v_fam.at(i).marriageW;
				marriageWAll = v_fam.at(i).marriageWAll;
				numOfChildren = v_fam.at(i).numOfChildren;
				fwprintf(fl, L"\n%-10s %-10s %-28s %-10s %-28s %d/%d %d/%d %d\n", refF, refH1, husband, refW, wife, marriageHX, marriageHAll, marriageWX, marriageWAll, numOfChildren);
			}
			refF = v_fam.at(i + 1).refF;
			refW = v_fam.at(i + 1).refW;
			husband = getIndi(refH2);
			wife = getIndi(refW);
			marriageHX = v_fam.at(i + 1).marriageH;
			marriageHAll = v_fam.at(i + 1).marriageHAll;
			marriageWX = v_fam.at(i + 1).marriageW;
			marriageWAll = v_fam.at(i + 1).marriageWAll;
			numOfChildren = v_fam.at(i + 1).numOfChildren;
			fwprintf(fl, L"%-10s %-10s %-28s %-10s %-28s %d/%d %d/%d %d\n", refF, refH1, husband, refW, wife, marriageHX, marriageHAll, marriageWX, marriageWAll, numOfChildren);
			refHPrev = refH1;
		}
	}


	std::sort(v_fam.begin(), v_fam.end(), sortByCnt);
	std::sort(v_fam.begin(), v_fam.end(), multiSort_FAM_refW); // refH szerint rendezi: a f�rj feles�geivel k�t�tt h�zass�gai egym�s ut�n lesznek

	fwprintf(fl, L"\n\n\nFeles�g t�bbsz�r�s h�zass�gai\n\n\n");
	for (UINT i = 0; i < v_fam.size() - 1; ++i)
	{
		refW1 = v_fam.at(i).refW;
		refW2 = v_fam.at(i + 1).refW;
		if (refW1 == refW2)						// i-bne �s i+1-ben azonos feles�
		{
			if (refWPrev != refW1)					// nem ugyanaz, mint az el�z�, �j p�r!
			{
				refF = v_fam.at(i).refF;
				refH = v_fam.at(i).refH;
				husband = getIndi(refH);
				wife = getIndi(refW1);
				marriageHX = v_fam.at(i).marriageH;
				marriageHAll = v_fam.at(i).marriageHAll;
				marriageWX = v_fam.at(i).marriageW;
				marriageWAll = v_fam.at(i).marriageWAll;
				numOfChildren = v_fam.at(i).numOfChildren;
				fwprintf(fl, L"\n%-10s %-10s %-28s %-10s %-28s %d/%d %d/%d %2d\n", refF, refH, husband, refW1, wife, marriageHX, marriageHAll, marriageWX, marriageWAll, numOfChildren);
			}
			refF = v_fam.at(i + 1).refF;
			refH = v_fam.at(i + 1).refH;
			husband = getIndi(refH);
			wife = getIndi(refW2);
			marriageHX = v_fam.at(i + 1).marriageH;
			marriageHAll = v_fam.at(i + 1).marriageHAll;
			marriageWX = v_fam.at(i + 1).marriageW;
			marriageWAll = v_fam.at(i + 1).marriageWAll;
			numOfChildren = v_fam.at(i + 1).numOfChildren;
			fwprintf(fl, L"%-10s %-10s %-28s %-10s %-28s %d/%d %d/%d %2d\n", refF, refH, husband, refW2, wife, marriageHX, marriageHAll, marriageWX, marriageWAll, numOfChildren);
			refWPrev = refW1;
		}
	}
}