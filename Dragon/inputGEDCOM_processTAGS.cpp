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
void CInputGEDCOM::famc()	// Egfy család azonosítója, amelyben az INDI gyerekként Mely családban
{
	I.FAMC = lxtv.value;
};
void CInputGEDCOM::fams()	// Egy család azonosítója, amelyben az INDI az egyik házastárs. 
							// Eldobhatjuk ezt az adatot, hiszen minden benne van a FAM rekordokban,
							// Csak az INDI házasságainak számolására használjuk (többszörös házasságok)
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
	CString value = lxtv.value;
	CString lastname;
	CString firstname;
	CString titolo;
	CString title;
	CString comment;
	CStringArray A;
	CString rest;
	CString word;
	int n;
	int m;
	CString ret;
	CString sex;
	TCHAR lastCharacter;
	TCHAR firstCharacter;
	TCHAR kar;

	value.Remove('/');
	value.Remove('\\');
	value.Trim();

	n = splitCString(value, ' ', false, &A);
	m = n;

	int k;

	// title meghatározása
	for (int i = 0; i < n; ++i)
	{
		word = A[i];
		k = sizeof(t) / sizeof(TITLE);
		for (int j = 0; j < sizeof(t)/sizeof(TITLE); ++j)
		{
			if (word == t[j].title)
			{
				title = word;
				A[i].Empty();
				--m;
				break;
			}
		}
	}

	
	// keresztnév meghatározása
	// elteker az elsõ betûig
	int i;
	for( i = 0; i < n; ++i)
	{
		if (!A[i].IsEmpty())
			break;
	}
	++i;	// az elsõ nem empty szót átugorja, mert az a vezetéknév
	for( i; i <n; ++i )
	{
		if (!A[i].IsEmpty() && m > 1)

		{
			word = A[i];
			if ((sex = theApp.isFirstName(word)) != L"")
			{
				firstname += word;
				firstname += L" ";
				A[i].Empty();
				--m;
			}
		}
	}
	firstname.TrimRight();

	for (int i = 0; i < n; ++i)
	{
		if (!A[i].IsEmpty())
		{
			word = A[i];
			firstCharacter = word.GetAt(0);
			lastCharacter = word.GetAt(word.GetLength() - 1);
			if (m > 1 && iswupper(firstCharacter) && lastCharacter == 'i')
			{
				titolo = word;
				A[i].Empty();
				--m;
			}
		}
	}

	for (int i = 0; i < n; ++i)
	{
		word = A[i];
		if (!word.IsEmpty() && iswupper( word[0] ) )
		{
			lastname += word;
			lastname += L" ";
			A[i].Empty();

		}
	}
	lastname.TrimRight();
	if (lastname.IsEmpty())
	{
		lastname = titolo;
		titolo.Empty();
	}


	for (int i = 0; i < n; ++i)
	{
		word = A[i];
		if (!word.IsEmpty() )
		{
			comment += word;
			comment += L" ";
			A[i].Empty();
		}
	}
	comment.TrimRight();

	if (titolo.IsEmpty() && !comment.IsEmpty())
	{
		firstCharacter = comment.GetAt(0);
		lastCharacter = comment.GetAt(comment.GetLength() - 1);
		if (IsCharLowerW(firstCharacter) && lastCharacter == 'i')
		{
			titolo = comment;
			kar = towupper(titolo[0]);
			titolo.SetAt(0, kar);
			comment.Empty();
		}
	}


	I.title = title;
	I.titolo = titolo;
	I.first_name = firstname;
	I.last_name = lastname;
	I.comment = comment;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGEDCOM::note()
{
	if (I.comment.IsEmpty())
		I.comment = lxtv.value;
	else
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

	int pos;										// Szücs.ged-ben a néve betettek '( hely)' 
	if ((pos = I.last_name.Find('(')) != -1)
		I.last_name = I.last_name.Left(pos);

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

