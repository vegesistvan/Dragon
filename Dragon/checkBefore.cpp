// checkBefore.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "checkBefore.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnFolytLines()
//{
//	CCheckBefore w;
//	w.checkFolyt();
//}
//
//void CDragonDlg::OnRepeatedFamilies()
//{
//	CCheckBefore w;
//	w.repeatedFamilies();
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CCheckBefore, CWnd)
CCheckBefore::CCheckBefore()
{

}

CCheckBefore::~CCheckBefore()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckBefore, CWnd)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckBefore::checkFolyt()
{
	CString folyt;
	CString percent;
	CString tableHeader;
	CString familyName;
	
	CString roman;
	CString name;

	bool first = true;
	CStringArray A;
	int n;
	bool hiba = false;

	fl = theApp.openLogFile(L"checkFoly", L"Folytat�s t�bl�k l�tez�s�nek ellen�rz�se");

	m_command.Format(L"SELECT rowid,* FROM tables ORDER BY familyName");
	if (!theApp.query(m_command)) return;

	CProgressWnd wndP(NULL, L"Folytat�s t�bl�k l�tezt�s�nek ellen�rz�se");
	wndP.GoModal();
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);


	for (int i = 0; i < theApp.m_recordset->RecordsCount(); ++i)
	{
		tableHeader = theApp.m_recordset->GetFieldString(DBT_TABLEHEADER);
		percent = theApp.m_recordset->GetFieldString(DBT_PERCENT);
		familyName = theApp.m_recordset->GetFieldString(DBT_FAMILY_NAME);
		roman = theApp.m_recordset->GetFieldString(DBT_TABLENUMBERROMAN);
		folyt = theApp.m_recordset->GetFieldString(DBT_FOLYT);
		name.Format(L"%s %s", familyName, roman);
		name.Trim();

		n = wordList(&A, folyt, ',', false);

		for (int j = 0; j < n; ++j)
		{
			m_command.Format(L"SELECT rowid FROM tables WHERE familyName = '%s' AND tableRoman='%s'", familyName, A[j]);
			if (!theApp.query1(m_command)) return;
			if (!theApp.m_recordset1->RecordsCount())
			{
				if (first)
				{
					fwprintf(fl, L"Az al�bbi t�bl�kban hivatkoz�sok vannak neml�tez� folytat�s t�bl�kra!\n\n");
					first = false;
				}
				fwprintf(fl, L"%-30s ->%s\n", name, A[j]);
				hiba = true;
			}
		}
		theApp.m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if (!hiba)
		fwprintf(fl, L"Minden hivatkozott folytat�st�bla l�tezik!");
	fclose(fl);
	theApp.showLogFile();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckBefore::repeatedFamilies()
{
	CString familyName;
	CString roman;
	CString name;
	CString namePrev;
	CString lineNumber;
	CString lineNumberPrev;
	CString tableHeader;
	CString tableHeaderPrev;

	bool uj = true;
	bool hiba = false;

	fl = theApp.openLogFile(L"repeatedFamilyes", L"T�bbsz�r elfordul� csal�dok l�tez�s�nek ellen�rz�se");
	fwprintf(fl, L"Az al�bbi t�bl�k t�bbsz�r fordulnak el� a %s f�jlban:\n\n", theApp.m_dbPathName);

	m_command.Format(L"SELECT rowid,* FROM tables ORDER BY familyName, tableRoman");
	if (!theApp.query(m_command)) return;

	CProgressWnd wndP(NULL, L"Folytat�s t�bl�k l�tezt�s�nek ellen�rz�se");
	wndP.GoModal();
	wndP.SetRange(0, theApp.m_recordset->RecordsCount());
	wndP.SetPos(0);
	wndP.SetStep(1);


	for (int i = 0; i < theApp.m_recordset->RecordsCount()-1; ++i)
	{
		tableHeader = theApp.m_recordset->GetFieldString(DBT_TABLEHEADER);
		lineNumber = theApp.m_recordset->GetFieldString(DBT_LINENUMBER);
		familyName = theApp.m_recordset->GetFieldString(DBT_FAMILY_NAME);
		roman = theApp.m_recordset->GetFieldString(DBT_TABLENUMBERROMAN);
		name.Format(L"%s %s", familyName, roman);
		name.Trim();

		if (name == namePrev)
		{
			if( uj )
			{
				fwprintf(fl, L"%6s %-30s\n", lineNumberPrev, tableHeaderPrev );
				uj = false;
				hiba = true;
			}
			fwprintf(fl, L"%6s %-30s\n", lineNumber, tableHeader );
		}
		else
		{
			if( !uj )	fwprintf(fl, L"\n");
			uj = true;
		}
		lineNumberPrev = lineNumber;
		namePrev = name;
		tableHeaderPrev = tableHeader;

		theApp.m_recordset->MoveNext();

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

	}
	fclose(fl);
	if (!hiba)
		AfxMessageBox( L"Minden t�bla csak egyszer szerepel az adatb�zisban!");
	else
		theApp.showLogFile();
}




