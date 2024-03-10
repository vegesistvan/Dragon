// GEDCOMFile.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "GEDCOMFile.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCheckTags()
{
	CGedcomFile w;
	w.checkTags();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnListFile()
{
	CGedcomFile w;
	w.listFile();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnListFileTab()
{
	CGedcomFile w;
	w.listFileTab();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnListZero()
{
	CGedcomFile w;
	w.listZero();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnListHEAD()
{
	CGedcomFile w;
	w.listHEAD();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnListChain()
{
	CGedcomFile w;
	w.listChain();
}
void CDragonDlg::OnFilterIrregular()
{
	CGedcomFile w;
	w.filterIrregular();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CGedcomFile, CWnd)

CGedcomFile::CGedcomFile()
{

}

CGedcomFile::~CGedcomFile()
{
}
BEGIN_MESSAGE_MAP(CGedcomFile, CWnd)
END_MESSAGE_MAP()
bool sortByTag(const TAGCNT& v1, const TAGCNT& v2);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomFile::listFile()
{
	getGedcomPathName();
	if (gedPathName.IsEmpty()) return;
	theApp.showFile(gedPathName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomFile::listFileTab()
{
	getGedcomPathName();
	if (gedPathName.IsEmpty()) return;


	theApp.m_inputCode = GetInputCode(gedPathName);

	gedFile.Open(gedPathName, CFile::modeRead);
	if (theApp.m_inputCode == UTF8BOM) gedFile.Seek(3, SEEK_SET);



	CString cLine;
	CString str;
	int level;
	int cnt = 0;

	CString fileSpec = theApp.openTextFile(&theApp.fl, gedFileTitle, L"w+");
	fwprintf(theApp.fl, L"%s\n\n", gedPathName);


	str.Format(L"Beolvas�sa a %s f�jlb�l...", gedFileName);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, (int)gedFile.GetLength());
	while (gedFile.ReadString(cLine))
	{
		++cnt;
		if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi(cLine);

		if (!extract(cLine, &lxtv))
		{
			str.Format(L"%d. sor hib�s: %s", cnt, cLine);
			AfxMessageBox(str);
			break;
		}
		
		level = lxtv.level;
		if( level < 0 || level > 10)
		{
			str.Format(L"%d. sorban level �rt�ke: %d!\n%s", cnt, level);
			AfxMessageBox(str, MB_ICONERROR);
			return;
		}
		if (!level)
			fwprintf(theApp.fl, L"\n");
		fwprintf(theApp.fl, L"%*c%s\n", 4*level, ' ', cLine);

		wndP.SetPos((int)gedFile.GetPosition());
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	gedFile.Close();
	fclose(theApp.fl);
	theApp.showFile(fileSpec);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomFile::listZero()
{
	CString fileName(L"zerolevel");
	CString fileSpec;
	CString cLine;
	CString str;
	CString lastTag;
	TCHAR chr;
	int cnt = 1;


	getGedcomPathName();
	if (gedPathName.IsEmpty()) return;


	theApp.m_inputCode = GetInputCode(gedPathName);

	gedFile.Open(gedPathName, CFile::modeRead);
	if (theApp.m_inputCode == UTF8BOM) gedFile.Seek(3, SEEK_SET);

	fileSpec = theApp.openTextFile(&theApp.fl, fileName, L"w+");
	fwprintf(theApp.fl, L"%s\n\n", gedPathName);
	fwprintf(theApp.fl, L"%s\n\n", L"Zero level list�ja");


	str.Format(L"Rekord fejl�cek beolvas�sa a %s f�jlb�l...", gedFileName );
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, (int)gedFile.GetLength());
	while (gedFile.ReadString(cLine))
	{
		if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi(cLine);
		if ((chr = cLine.GetAt(0)) == '0')
		{
			str = getLastWord(cLine);
			if (str != lastTag) cnt = 1;					// �j TAG sorsz�moz�sa 1-t�l
			fwprintf(theApp.fl, L"%5d %s\n", cnt, cLine);
			lastTag = str;
			++cnt;
		}

		wndP.SetPos((int)gedFile.GetPosition());
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	gedFile.Close();
	fclose(theApp.fl);
	theApp.showFile(fileSpec);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomFile::listHEAD()
{
	CString str;
	CString cLine;
	int		cnt = 0;
	int level;


	getGedcomPathName();
	if (gedPathName.IsEmpty()) return;
	theApp.m_inputCode = GetInputCode(gedPathName);
	gedFile.Open(gedPathName, CFile::modeRead);
	if (theApp.m_inputCode == UTF8BOM) gedFile.Seek(3, SEEK_SET);

	CString fileSpec = theApp.openTextFile(&theApp.fl, L"HEAD", L"w+");
	fwprintf(theApp.fl, L"%s\n\n", gedPathName);

	str.Format(L"Fejl�c rekordok beolvas�sa a %s f�jlb�l...", gedFileName);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, (int)gedFile.GetLength());

	
	while (gedFile.ReadString(cLine))
	{
		if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi(cLine);
		level = _wtoi(getFirstWord(cLine));
		if ( cnt && level == 0 ) break;
		++cnt;
		fwprintf(theApp.fl, L"%*c%s\n", level * 4, ' ', cLine);
		wndP.SetPos((int)gedFile.GetPosition());
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	gedFile.Close();
	fclose(theApp.fl);
	theApp.showFile(fileSpec);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomFile::listChain()
{
	CString fileName(L"tagChain");
	CString fileSpec;
	CString cLine;
	CString str;
	CString lastTag;
	TCHAR chr;
	int cnt = 0;
	bool vissza = true;

	getGedcomPathName();
	if (gedPathName.IsEmpty()) return;


	theApp.m_inputCode = GetInputCode(gedPathName);

	gedFile.Open(gedPathName, CFile::modeRead);
	if (theApp.m_inputCode == UTF8BOM) gedFile.Seek(3, SEEK_SET);

	fileSpec = theApp.openTextFile(&theApp.fl, fileName, L"w+");
	fwprintf(theApp.fl, L"%s\n\n", gedPathName);
	fwprintf(theApp.fl, L"%s\n\n", L"TAG �rt�kek l�nca");


	str.Format(L"%s f�jl beolvas�sa...", gedFileName);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, (int)gedFile.GetLength());


	while (gedFile.ReadString(cLine))
	{
		++cnt;
		cLine.Trim();
		if (cLine.IsEmpty()) goto cont;

		if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi(cLine);
		if (!extract(cLine, &lxtv))
		{
			str.Format(L"%d. sor hib�s: %s", cnt, cLine);
			AfxMessageBox(str);
			break;
		}

		if (lxtv.level == 0)    // null�s level!!!!!!!!!!!!!!!!!!!!!!!!!
		{
			list_vlxtv();											// az addigit ki�rja
			fwprintf(theApp.fl, L"\n");
			v_lxtv.clear();
			v_lxtv.push_back(lxtv);
		}
		if (lxtv.level > v_lxtv.at(v_lxtv.size() - 1).level)		// ha nagyobb, akkor elteszi
			v_lxtv.push_back(lxtv);
		else if (lxtv.level == v_lxtv.at(v_lxtv.size() - 1).level) // ha egyenl�, akkor 
		{
			list_vlxtv();											// az addigit ki�rja
			v_lxtv.pop_back();										// az el�z�t eldobja
			v_lxtv.push_back(lxtv);									// mag�t beleteszi
		}
		else														// ha kisebb
		{
			list_vlxtv();											// az addigit ki�rja
			while (v_lxtv.at(v_lxtv.size() - 1).level >= lxtv.level)
				v_lxtv.pop_back();									// az el�z� tagakb�l eldobja azokat, amik nagyobbak vagy egyenl� vele
			v_lxtv.push_back(lxtv);									// mag�t beleteszi
		}
	cont:	wndP.SetPos((int)gedFile.GetPosition());
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	gedFile.Close();
	fclose(theApp.fl);
	theApp.showFile(fileSpec);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomFile::filterIrregular()
{
	getGedcomPathName();
	if (gedPathName.IsEmpty()) return;


	theApp.m_inputCode = GetInputCode(gedPathName);

	gedFile.Open(gedPathName, CFile::modeRead);
	if (theApp.m_inputCode == UTF8BOM) gedFile.Seek(3, SEEK_SET);

	CString cLine;
	CString newGedFile;
	CString str;

	FILE* fl;

	int m_numberOfTags = sizeof(G::tags) / sizeof(G::GEDTAGS);

	newGedFile.Format(L"%s\\%s_regular.ged", gedFolderPath, gedFileTitle);

	if (!openFileSpec(&fl, newGedFile, L"w+")) return;

	str.Format(L"%s f�jl sz�r�se folyik...", gedFileName);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, (int)gedFile.GetLength());
	wndP.SetPos(0);

	while (gedFile.ReadString(cLine))
	{
		cLine.Trim();
		if (cLine.IsEmpty()) continue;
		if (extract(cLine, &lxtv))
		{
			fwprintf(fl, L"%s\n", cLine);
		}
		wndP.SetPos((int)gedFile.GetPosition());
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	gedFile.Close();
	fclose(fl);
	wndP.DestroyWindow();
	theApp.showFile(newGedFile);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByTag(const TAGCNT& v1, const TAGCNT& v2);
// Felhaszn�lt, eldobott �s user defined tag-ek a f�jlban
void CGedcomFile::checkTags()
{
	getGedcomPathName();
	if (gedPathName.IsEmpty()) return;


	theApp.m_inputCode = GetInputCode(gedPathName);

	gedFile.Open(gedPathName, CFile::modeRead);
	if (theApp.m_inputCode == UTF8BOM) gedFile.Seek(3, SEEK_SET);


	CString fileName(L"TAGS");
	CString fileSpec;
	CString str;
	CString tag;
	CString cLine;

	std::vector<TAGCNT> vOK;	// known and used	
	std::vector<TAGCNT> vNO;	// known, but not used
	std::vector<TAGCNT> vUN;	// unknown tags
	std::vector<TAGCNT> vPR;	// program defined tags
	

	int cnt = 0;
	int i;
	int j;

	int n = sizeof(G::tags) / sizeof(G::GEDTAGS);

	fileSpec = theApp.openTextFile(&theApp.fl, fileName, L"w+");
	fwprintf(theApp.fl, L"%s\n\n", gedPathName);


	str.Format(L"%s f�jl beolvas�sa folyik...", gedFileName);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, (int)gedFile.GetLength());
	wndP.SetPos(0);


	while (gedFile.ReadString(cLine))
	{
		++cnt;
		cLine.Trim();
		if (cLine.IsEmpty()) continue;
		if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi(cLine);
		tag = getSecondWord(cLine);  // a TAG vagy a masodik sz�, vagy az utols�
		if (tag.GetAt(0) == '@')
			tag = getLastWord(cLine);
	
		if (tag.Left(1) == L"_")								// program �ltal defini�lt tag
			collectTag(tag, &vPR);
		else
		{
			for (i = 0; i < n; ++i)
			{
				if (tag == G::tags[i].name)
				{
					if (G::tags[i].fcnPtr == NULL)					// Dragon �ltal nem �rtlemzett tag
						collectTag(tag, &vNO);
					else										// Dragon �ltal �rtelmezett tag
						collectTag(tag, &vOK);
					break;
				}

			}
		}
		if (i == n)
			collectTag(tag, &vUN);

		wndP.SetPos((int)gedFile.GetPosition());
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	gedFile.Close();
		

	std::sort(vOK.begin(), vOK.end(), sortByTag);
	std::sort(vNO.begin(), vNO.end(), sortByTag);
	std::sort(vPR.begin(), vPR.end(), sortByTag);
	std::sort(vUN.begin(), vUN.end(), sortByTag);

	fwprintf(theApp.fl, L"Dragon �ltal �rtelmezett tag-ek �s el�fordul�suk sz�ma:\n");
	for (j = 0; j < vOK.size(); ++j)
		fwprintf(theApp.fl, L"%-5s %6d\n", vOK.at(j).tag, vOK.at(j).cnt);

	fwprintf(theApp.fl, L"\nDragon �ltal ismert de nem feldolgozott tag-ek �s el�fordul�suk sz�ma:\n");
	for (j = 0; j < vNO.size(); ++j)
		fwprintf(theApp.fl, L"%-5s %6d\n", vNO.at(j).tag, vNO.at(j).cnt);

	fwprintf(theApp.fl, L"\nDragon �ltal nme ismert tag-ek �s el�fordul�suk sz�ma:\n");
	for (j = 0; j < vUN.size(); ++j)
		fwprintf(theApp.fl, L"%-30s %6d\n", vUN.at(j).tag, vUN.at(j).cnt);

	fwprintf(theApp.fl, L"\nProgram defined tags �s el�fordul�suk sz�ma:\n");
	for (j = 0; j < vPR.size(); ++j)
		fwprintf(theApp.fl, L"%-22s %6d\n", vPR.at(j).tag, vPR.at(j).cnt);

	fclose(theApp.fl);
	theApp.showFile(fileSpec);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByTag(const TAGCNT& v1, const TAGCNT& v2)
{
	return(v1.tag < v2.tag);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomFile::collectTag(CString tag, std::vector<TAGCNT>* v)
{
	TAGCNT tagcnt;
	int j;
	for (j = 0; j < v->size(); ++j)
	{
		if (v->at(j).tag == tag)
		{
			++v->at(j).cnt;
			break;
		}
	}
	if (j == v->size())
	{
		tagcnt.tag = tag;
		tagcnt.cnt = 1;
		v->push_back(tagcnt);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomFile::list_vlxtv()
{
	CString str;
	for (UINT i = 0; i < v_lxtv.size(); ++i)
	{
		str.Format(L"%s %-15s", (CString)str, v_lxtv.at(i).tag);
	}
	str.Trim();
	if( !str.IsEmpty() ) fwprintf(theApp.fl, L"%s\n", str);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGedcomFile::getGedPathName()
{
	CFileDialog dlg(TRUE, L".", (LPCTSTR)theApp.m_gedPathName, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"gedcom files (*.ged)|*.ged|All Files (*.*)|*.*||");

	dlg.m_ofn.lpstrTitle = L"V�laszd ki a k�v�nt GEDCOM f�jlt";
	if (dlg.DoModal() == IDCANCEL) return L"";

	return dlg.GetPathName();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGedcomFile::getGedcomPathName()
{
	CString drive;
	CString path;
	CString ext;

	if (theApp.m_inputMode == GEDCOM)
		gedPathName = theApp.m_gedPathName;				// ha felt�lt�tt adatb�zis van, akkor annak a ged-f�jlj�val fog dolghozni
	else
		gedPathName = getGedPathName();					// ha �res adatb�zis van, akkor megk�rdeti, hogy melyik f�jllal dolgozzon

	if (gedPathName.IsEmpty()) return false;
	if (_waccess(gedPathName, 0)) return false;

	splitFilespec(gedPathName, &drive, &path, &gedFileTitle, &ext);

	gedFolderPath.Format(L"%s:%s", drive, path);
	gedFileName.Format(L"%s.%s", gedFileTitle, ext);
	return true;
}