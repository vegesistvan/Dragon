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

	fl = theApp.openLogFile(L"checkFoly", L"Folytatás táblák létezésének ellenõrzése");

	m_command.Format(L"SELECT rowid,* FROM tables ORDER BY familyName");
	if (!theApp.query(m_command)) return;

	CProgressWnd wndP(NULL, L"Folytatás táblák léteztésének ellenõrzése");
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
					fwprintf(fl, L"Az alábbi táblákban hivatkozások vannak nemlétezõ folytatás táblákra!\n\n");
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
		fwprintf(fl, L"Minden hivatkozott folytatástábla létezik!");
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

	fl = theApp.openLogFile(L"repeatedFamilyes", L"Többször elforduló családok létezésének ellenõrzése");
	fwprintf(fl, L"Az alábbi táblák többször fordulnak elõ a %s fájlban:\n\n", theApp.m_dbPathName);

	m_command.Format(L"SELECT rowid,* FROM tables ORDER BY familyName, tableRoman");
	if (!theApp.query(m_command)) return;

	CProgressWnd wndP(NULL, L"Folytatás táblák léteztésének ellenõrzése");
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
		AfxMessageBox( L"Minden tábla csak egyszer szerepel az adatbázisban!");
	else
		theApp.showLogFile();
}




