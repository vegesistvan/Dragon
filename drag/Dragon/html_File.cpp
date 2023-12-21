// html_File.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "html_File.h"
#include "html_Descendants.h"
#include "ConnectCsaladTorzs.h";
#include "html_Brackets.h"
#include "SelectTableName.h"
#include "GetString.h"
#include "Brackets.h"
#include "utilities_dragon.h"
#include "html_Spouses.h"
#include "html_SpouseSpouses.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckFolyt()
//{
//	CHtmlFile w;
//	w.folytLines();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnGaEditor()
//{
//	CHtmlFile w;
//	w.GaEditor();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnGaOpen()
//{
//	CHtmlFile w;
//	w.GaOpen();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnDescendantFile()
//{
//	CHtmlFile w;
//	w.descendantFile();
//}
//void CDragonDlg::OnDescendantLine()
//{
//	CHtmlFile w;
//	w.descendantLine();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnDescendantTable()
//{
//	CHtmlFile w;
//	w.descendantTable();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnMarriagesFile()
//{
//	CHtmlFile w;
//	w.marriagesFile();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnMarriagesTable()
//{
//	CHtmlFile w;
//	w.marriagesTable();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnMarriagesLine()
//{
//	CHtmlFile w;
//	w.marriagesLine();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnSsFile()
//{
//	CHtmlFile w;
//	w.ssFile();
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnSsTable()
//{
//	CHtmlFile w;
//	w.ssTable();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnSsLine()
//{
//	CHtmlFile w;
//	w.ssLine();
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CHtmlFile, CWnd)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlFile::CHtmlFile()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlFile::~CHtmlFile()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CHtmlFile, CWnd)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::GaEditor()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}
	theApp.showFile(htmlPathName);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::GaOpen()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}
	ShellExecute(NULL, L"open", htmlPathName, NULL, NULL, SW_SHOWNORMAL);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::folytLines()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	FILE* fl;
	CString str;
	CString logFile(L"folyt");
	CString title(L"%%% folyt sorok");
	if (( fl = theApp.openLogFile(logFile, title)) == NULL)return;
	int		pos;



	CString cLine;
	CString folyt;
	CString roman;
	CString percent;
	CString family;

	theApp.m_inputCode = GetInputCode( htmlPathName);
	CStdioFile file( htmlPathName, CFile::modeRead);

//	std::vector<FOLYT> vFolyt;


	CProgressWnd wndProgress(NULL, L"% folyt sorok a ga.html fájlban...");
	wndProgress.GoModal();
	wndProgress.SetRange(0, (int)file.GetLength());
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	while (file.ReadString(cLine))
	{
		cLine.Trim();
		if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi(cLine);
		pos = (int)file.GetPosition();
		wndProgress.SetPos(pos);
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;

		if ((pos = cLine.Find(L"%%% folyt")) != -1)
		{
			folyt = cLine.Mid(pos);
			str = cLine.Mid(pos + 10);
			roman = getFirstWord(str);
			//			str = cleanCline2( cLine );
			str = cleanCline(cLine);
			//fwprintf( theApp.fl, L"%s\n", str );
			fwprintf(theApp.fl, L"%-30s %s\n", folyt.Left(30), roman);
		}
	}
	file.Close();
	fclose(fl);
	theApp.showLogFile();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::roundBrackets()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CBrackets dlg;
	dlg.m_bracketCode = ROUND;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::squareBrackets()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CBrackets dlg;
	dlg.m_bracketCode = SQUARE;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::bracesBrackets()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CBrackets dlg;
	dlg.m_bracketCode = BRACES;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::descendantFile()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CHtmlDescendants dlg;
	dlg.m_familyNumber = 0;
	dlg.m_tableNumber = 0;
	dlg.m_lineNumber = 0;
	dlg.m_tableName = L"";
	dlg.m_htmlPathName = htmlPathName;
	dlg.m_htmlFileName = htmlFileName;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::descendantTable()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CSelectTableName dlgT;
	int tableNumber;
	CString tableName;

	tableNumber = theApp.GetProfileInt(L"dragon", L"tableNumber", 0);
	if (dlgT.DoModal() == IDCANCEL)
	{
		return;
	}
	tableNumber = dlgT.m_tableNumberSelected;
	tableName = dlgT.m_tableNameSelected;
	theApp.WriteProfileInt(L"dragon", L"tableNumber", tableNumber);

	CHtmlDescendants dlg;
	dlg.m_familyNumber = 0;
	dlg.m_tableNumber = tableNumber;
	dlg.m_lineNumber = 0;
	dlg.m_numberOfTables = _wtoi(dlgT.m_num);
	dlg.m_tableName = tableName;
	dlg.m_htmlFileName = htmlFileName;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::descendantLine()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}
	CGetString dlgL;
	int lineNumber;
	
	lineNumber = theApp.GetProfileInt(	L"dragon", L"lineNumber", 0 );
	dlgL.m_string.Format( L"%d", lineNumber ); 

	dlgL.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", htmlFileName );
	if( dlgL.DoModal() == IDCANCEL ) return;
	
	lineNumber = _wtoi( dlgL.m_string );
	theApp.WriteProfileInt( L"dragon", L"lineNumber", lineNumber  );

	CHtmlDescendants dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber	= lineNumber;
	dlg.m_tableName		= L"";
	dlg.m_htmlFileName = htmlFileName;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::marriagesFile()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CHtmlSpouses dlg;
	dlg.m_familyNumber = 0;
	dlg.m_tableNumber = 0;
	dlg.m_lineNumber = 0;
	dlg.m_tableName = L"";
	dlg.m_numberOfTables = 0;
	dlg.m_htmlFileName = htmlFileName;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::marriagesTable()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CSelectTableName dlgT;
	int tableNumber;
	CString tableName;


	tableNumber = theApp.GetProfileInt(L"dragon", L"tableNumber", 0);
	if (dlgT.DoModal() == IDCANCEL) return;

	tableNumber = dlgT.m_tableNumberSelected;
	tableName = dlgT.m_tableNameSelected;
	theApp.WriteProfileInt(L"dragon", L"tableNumber", tableNumber);


	CHtmlSpouses dlg;
	dlg.m_familyNumber = 0;
	dlg.m_tableNumber = tableNumber;
	dlg.m_lineNumber = 0;
	dlg.m_tableName = tableName;
	dlg.m_numberOfTables = _wtoi(dlgT.m_num);
	dlg.m_htmlFileName = htmlFileName;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::marriagesLine()
{
	int lineNumber;

	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CGetString dlgL;
	dlgL.m_string.Format( L"%d", theApp.GetProfileInt(L"dragon", L"lineNumber", 0) );
//	dlgL.m_string.Format(L"%d", lineNumber);

	dlgL.m_caption.Format(L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName);
	if (dlgL.DoModal() == IDCANCEL) return;

	lineNumber = _wtoi(dlgL.m_string);
	theApp.WriteProfileInt(L"dragon", L"lineNumber", lineNumber);


	CHtmlSpouses dlg;
	dlg.m_familyNumber = 0;
	dlg.m_tableNumber = 0;
	dlg.m_lineNumber = lineNumber;
	dlg.m_numberOfTables = 0;
	dlg.m_htmlFileName = htmlFileName;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::ssFile()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CSpouseSpouses dlg;
	dlg.m_familyNumber = 0;
	dlg.m_tableNumber = 0;
	dlg.m_lineNumber = 0;
	dlg.m_htmlFileName = htmlFileName;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::ssTable()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CSelectTableName dlgT;
	int tableNumber;
	CString tableName;

	tableNumber = theApp.GetProfileInt(L"dragon", L"tableNumber", 0);
	if (dlgT.DoModal() == IDCANCEL) return;
	tableNumber = dlgT.m_tableNumberSelected;
	tableName = dlgT.m_tableNameSelected;
	theApp.WriteProfileInt(L"dragon", L"tableNumber", tableNumber);


	CSpouseSpouses dlg;
	dlg.m_familyNumber = 0;
	dlg.m_tableNumber = tableNumber;
	dlg.m_lineNumber = 0;
	dlg.m_tableName = tableName;
	dlg.m_numberOfTables = _wtoi(dlgT.m_num);
	dlg.m_htmlFileName = htmlFileName;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlFile::ssLine()
{
	htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		getGAHtmlPathName();
		if (htmlPathName.IsEmpty()) return;
	}

	CGetString dlgL;
	int lineNumber;


	lineNumber = theApp.GetProfileInt(L"dragon", L"lineNumber", 0);
	dlgL.m_string.Format(L"%d", lineNumber);
	dlgL.m_caption.Format(L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName);
	if (dlgL.DoModal() == IDCANCEL) return;

	lineNumber = _wtoi(dlgL.m_string);
	theApp.WriteProfileInt(L"dragon", L"lineNumber", lineNumber);

	CSpouseSpouses dlg;
	dlg.m_familyNumber = 0;
	dlg.m_tableNumber = 0;
	dlg.m_lineNumber = lineNumber;
	dlg.m_htmlFileName = htmlFileName;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CHtmlFile::getGAHtmlPathName()
{
	if( ! theApp.selectHtml() ) return false;
	splitPathName(theApp.m_htmlPathName, &htmlFileTitle, &htmlFileName, &theApp.m_dbFolderPath);
	return true;
}