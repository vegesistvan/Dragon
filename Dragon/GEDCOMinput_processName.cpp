//#include "stdafx.h"
#include "Dragon.h"
#include "GEDCOMInput.h"
// P�ld�k:

// 
// NAME Ill�nyi Soma Andr�s //
// NAME B�r� Katalin homor�dszentm�rtoni //
// NAME Rh�dey Farkas gr�f //
// NAME Rh�dey Zsuzsanna Klaudia gr�fn� //
// NAME Szigeti Orsolya //


// NAME Dorothy May / Lloyd /
// NAME Dora / Waters /


// NAME nemes porcsalmai Ferenc / BAJNAI /
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::processNAME(CString value, INDI* indi)
{
	int pos1;
	int pos2;

	if ((pos1 = value.Find('/')) != -1)
	{
		if ((pos2 = value.Find('/', pos1+1)) != -1. && pos2 == pos1 + 1)
		{
			getFirst(value, indi);
		}
		else
			getName(value, indi);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A NAME tag� sor value felbont�sa el�n�v-csal�dn�v-keresztn�v- comment substringekre 
// NAME Ill�nyi Soma Andr�s //
// NAME B�r� Katalin homor�dszentm�rtoni //
// NAME Rh�dey Farkas gr�f //
// NAME Rh�dey Zsuzsanna Klaudia gr�fn� //
// NAME Szigeti Orsolya //

void CGEDCOMInput::getFirst(CString value, INDI* indi)
{
	CString first_name(L"");
	CString titolo(L"");
	CString title(L"");
	CString comment(L"");
	CStringArray A;
	CString rest(L"");
	CString str;
	int n;
	int i;
	int ret;
	TCHAR kar;

	n = splitCString(value, ' ', false, &A);

	indi->last_name = A.GetAt(0);
	for ( i = 1; i < n-1; ++i)
	{
		if( iswlower( A[i][0] ) ) break;
	}
	if (i == n - 1)   // nincs kisbet�s sz�
	{
	indi->first_name = packWords(&A, 1, n - 2);  // marad�k szavak mind keresztn�v
	}
	else
	{
	indi->first_name = packWords(&A, 1, i - 1);  // az els� kisbet�s sz�ig keresztn�v
	str = A[n - 2];
	if (str.GetAt(str.GetLength() - 1) == 'i')
	{
		kar = towupper(str.GetAt(0));
		indi->titolo = kar + str.Mid(1);
	}
	else
		indi->title = A[n - 2];
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A NAME tag� sor value felbont�sa el�n�v-csal�dn�v-keresztn�v- comment substringekre 
// Ez specifikusan egy ged f�jlra k�sz�lt. nem val�sz�n�, hogy a standard NAME-ben �g lenn�nek az el�nevek �s a comment
void CGEDCOMInput::getName(CString value, INDI* indi)
{
	return;
	CString lastname;
	CString first_name(L"");
	CString titolo(L"");
	CString title(L"");
	CString comment(L"");
	CStringArray A;
	CString rest(L"");
	int n;
	int ret;

	if ((n = value.Find(L"nemes �s nemzetes")) != -1)
		n = 2;
	n = splitCString(value, ' ', false, &A);

	lastname = A[0];
	TCHAR k = towupper(lastname[0]);
	lastname = k + lastname.Mid(1);

	indi->last_name = lastname;
	for (int i = 1; i < n; ++i)
	{
		if ((ret = theApp.isFirstName(A[i])) != -1)
		{
			first_name += L" ";
			first_name += A[i];
		}
		else
		{
			rest = packWords(&A, i, n - i - 1);
			break;
		}
	}
	first_name.Trim();
	indi->first_name = first_name;
	if (!rest.IsEmpty())
	{
		n = splitCString(rest, ' ', false, &A);
		if (n == 1)
		{
			if (rest.Right(1) == 'i')
				titolo = rest;
			else
				indi->comment = rest;
		}
		else
		{
			for (int i = 0; i < n; ++i)
			{
				if (A[i] == L"�s")
				{
					if ((i - 1) > 0 && (i + 1) < n)
						titolo.Format(L"%s �s %s", A[i - 1], A[i + 1]);
					break;
				}
			}
		}
		if (!titolo.IsEmpty())
			titolo = firstUpper(titolo);
		else if (n > 1)
			titolo = rest;
		indi->title = title;
		indi->titolo = titolo;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A NAME tag� sor value felbont�sa el�n�v-csal�dn�v-keresztn�v- comment substringekre 
// Ez specifikusan egy ged f�jlra k�sz�lt. nem val�sz�n�, hogy a standard NAME-ben �g lenn�nek az el�nevek �s a comment
void CGEDCOMInput::getNameL(CString value, INDI* indi)
{
	int pos1;
	int pos2;
	CString firstnames;
	CString lastname;
	TCHAR k;

	indi->first_name.Empty();
	indi->last_name.Empty();
	if ((pos1 = value.Find('(')) != -1 )
	{
		value = value.Left(pos1);
		value.Trim();	
	}

	if ((pos1 = value.Find('/')) != -1)
	{
		if (pos2 = value.ReverseFind('/') != -1)
		{
			firstnames = value.Left(pos1 - 1);
			lastname = value.Mid(pos1 + 1, value.GetLength() - pos1 - 2);
			lastname.Trim();
			k = towupper(lastname[0]);
			lastname = k + lastname.Mid(1);
			

			indi->first_name = firstnames;
			indi->last_name = lastname;
		}
	}
}