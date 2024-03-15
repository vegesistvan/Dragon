#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "inputGEDCOM.h"
#include "utilities_dragon.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGEDCOM::abbr()
{

}
void CInputGEDCOM::addr()
{

};
void CInputGEDCOM::adr1()
{

};
void CInputGEDCOM::adr2()
{

};
void CInputGEDCOM::birt()
{

}
void CInputGEDCOM::chan()
{

};
void CInputGEDCOM::charx()
{

};
void CInputGEDCOM::chil()
{
	str = lxtv.value;
	str.Replace('@', ' ');
	str.Replace('I', ' ');
	str.Trim();
	CHIL.numRefI = _wtoi(str);
	CHIL.refF = F.refF;
	CHIL.refH = F.refH;
	CHIL.refW = F.refW;
	CHIL.refC = lxtv.value;
	CHIL.parentIndex = 0;
	CHIL.whichChildFather = F.numOfChildren + 1;
	CHIL.whichChildMother = 0;
	v_CHIL.push_back(CHIL);
	++F.numOfChildren;
};
void CInputGEDCOM::cont()
{
	if (v_lxtv.at(v_lxtv.size() - 2).tag == L"NOTE")
		I.comment.Format( L"%s %s", (CString)I.comment, lxtv.value );
//	if (v_lxtv.at(v_lxtv.size() - 3).tag == L"INDI")
//		I.comment.Format(L"%s %s", (CString)I.comment, m_note);

}
void CInputGEDCOM::deat()
{
	if (!lxtv.value.IsEmpty())
		I.death_date = L"meghalt";
}
void CInputGEDCOM::date()
{
	CString date = date2date(lxtv.value);
	CString tagPrev = v_lxtv.at(v_lxtv.size() - 2).tag;
	if ( tagPrev == L"BIRT")
		I.birth_date = date;
	else if ( tagPrev == L"DEAT")
		I.death_date = date;
	else if (tagPrev == L"FAM")
		F.date = date;
	else if (tagPrev == L"MARR")
		F.date = date;

};
void CInputGEDCOM::educ()
{

}
void CInputGEDCOM::fam()
{
	F.refF = lxtv.xref;
};
void CInputGEDCOM::famc()	// Egfy csal�d azonos�t�ja, amelyben az INDI gyerekk�nt Mely csal�dban
{
	I.FAMC = lxtv.value;
};
void CInputGEDCOM::fams()	// Egy csal�d azonos�t�ja, amelyben az INDI az egyik h�zast�rs. 
							// Eldobhatjuk ezt az adatot, hiszen minden benne van a FAM rekordokban,
							// Csak az INDI h�zass�gainak sz�mol�s�ra haszn�ljuk (t�bbsz�r�s h�zass�gok)
{						
	++I.numOfSpouses;
};
void CInputGEDCOM::getFile()
{
//	photos.refI = indi.refI;
//	photos.link = lxtv.value;
//	vPhotos.push_back(photos);
};
void CInputGEDCOM::form()
{

};
void CInputGEDCOM::gedc()
{

};
void CInputGEDCOM::givn()
{
	I.first_name = lxtv.value;
}
void CInputGEDCOM::head()
{

};
void CInputGEDCOM::husb()
{
	F.refH = lxtv.value;
};

void CInputGEDCOM::indi()
{
	I.refI = lxtv.xref;
}
void CInputGEDCOM::marr()
{

};
void CInputGEDCOM::name()
{
	int pos;
	if ((pos = lxtv.value.Find('/')) != -1)
	{
		I.first_name = lxtv.value.Left(pos-1);
		I.last_name = lxtv.value.Mid(pos);
		I.last_name.Trim();
		I.last_name.Remove('/');
		I.first_name.Remove('/');
	}
}
void CInputGEDCOM::note()
{
	I.comment.Format(L"%s %s", (CString)I.comment, lxtv.value );
};
void CInputGEDCOM::nsfx()
{
	I.posterior = lxtv.value;
};
void CInputGEDCOM::npfx()
{
	I.title = lxtv.value;
};
void CInputGEDCOM::obje()
{

};
void CInputGEDCOM::occu()
{
	I.comment = lxtv.value;
};
void CInputGEDCOM::info()
{
	I.comment += L" ";
	I.comment += lxtv.value;
};
void CInputGEDCOM::plac()
{
	CString place = lxtv.value;
	if (v_lxtv.at(v_lxtv.size() - 2).tag == L"BIRT")
		I.birth_place = place;
	else if (v_lxtv.at(v_lxtv.size() - 2).tag == L"DEAT")
		I.death_place = place;
	else if (v_lxtv.at(v_lxtv.size() - 2).tag == L"FAM")
		F.place = place;
	else if (v_lxtv.at(v_lxtv.size() - 2).tag == L"MARR")
		F.place = place;
};
void CInputGEDCOM::reli()
{
	I.comment.Format(L"%s %s", (CString)I.comment, lxtv.value);
}
void CInputGEDCOM::sex()
{
	if (lxtv.value == L"M")
		I.sex = L"1";
	else
		I.sex = L"2";
};
void CInputGEDCOM::sour()
{

};
void CInputGEDCOM::spfx()
{
	I.titolo = lxtv.value;
};
void CInputGEDCOM::subm()
{

};
void CInputGEDCOM::surn()
{
	I.last_name = lxtv.value;

	int pos;										// Sz�cs.ged-ben a n�ve betettek '( hely)' 
	if ((pos = I.last_name.Find('(')) != -1)
		I.last_name = I.last_name.Left(pos);

	CStringArray A;
	int n = wordList(i.last_name, &A, ' ', false);
	if (n > 1)
	{
		if (A[0].GetAt(0).isLower())
		{
			I.last_name = A[1];
		}
	}
}
void CInputGEDCOM::text()
{
	I.comment.Format(L"%s %s", (CString)I.comment, lxtv.value);
}
void CInputGEDCOM::time()
{

};
void CInputGEDCOM::titl()
{

};
void CInputGEDCOM::trlr()
{

};
void CInputGEDCOM::vers()
{

};
void CInputGEDCOM::wife()
{
	F.refW = lxtv.value;
};

