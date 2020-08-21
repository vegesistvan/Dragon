#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"
//#include "Relations.h"
//#include "about.h"
#include "SelectOne.h"
#include "SelectedFiles.h"
#include "GetString.h"
#include "utilities.h"
#include "GAtoDB.h"
#include "GA_input.h"
#include "SelectTableName.h"
#include "SelectFamily.h"
#include "SelectFamilyName.h"
#include "SelectTableHtml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnInputGahtmlFile()
{
	CGaInput split;


	split.m_rollToLineFrom	= 0;
	split.m_rollToLine		= 0;
	split.m_rollToTable		= 0;
	split.m_rollToFamily	= 0;
	split.m_fileNumber		= 1;


	split.inputFile();
	mainTitle();
	ChangeMenu();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnInputGahtmlFamily()
{
	CSelectFamilyName dlg;	
	if( dlg.DoModal() == IDCANCEL ) return;
	

	CGaInput split;


	split.m_rollToLineFrom	= 0;
	split.m_rollToLine		= 0;
	split.m_rollToTable		= 0;
	split.m_rollToFamily	= dlg.m_familyNumber;
	split.m_familyName		= dlg.m_familyName;
	split.m_fileNumber		= 1;

	split.inputFile();
	mainTitle();
	ChangeMenu();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnInputGahtmlTable()
{
	CSelectTableName dlg;


	if( dlg.DoModal() == IDCANCEL ) return;
	
	CGaInput split;

	split.m_rollToLineFrom	= 0;
	split.m_rollToLine		= 0;
	split.m_rollToTable		= dlg.m_tableNumberSelected;
	split.m_rollToFamily	= 0;
	split.m_tableName		= dlg.m_tableNameSelected;
	split.m_fileNumber		= 1;

	split.inputFile();
	mainTitle();
	ChangeMenu();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnInputGahtmlLine()
{
	CGetString dlg;

	int lineNumber;
	lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
	dlg.m_string.Format( L"%d", lineNumber ); 
	dlg.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát amit be akarsz olvasni!", theApp.m_htmlFileName );
	if( dlg.DoModal() == IDCANCEL ) return;
	lineNumber = _wtoi( dlg.m_string );
	theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );


	CGaInput split;

	split.m_rollToLineFrom	= 0;
	split.m_rollToLine		= lineNumber;
	split.m_rollToTable		= 0;
	split.m_rollToFamily	= 0;
	split.m_fileNumber		= 1;


	split.inputFile();
	mainTitle();
	ChangeMenu();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnInputGahtmlFromline()
{
	CGetString dlg;

	int lineNumber;
	lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
	dlg.m_string.Format( L"%d", lineNumber ); 
	dlg.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát amitõl kezdve be akarod olvasni a fájlt!", theApp.m_htmlFileName );
	if( dlg.DoModal() == IDCANCEL ) return;
	lineNumber = _wtoi( dlg.m_string );
	theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );


	CGaInput split;

	split.m_rollToLineFrom	= lineNumber;
	split.m_rollToLine		= lineNumber;
	split.m_rollToTable		= 0;
	split.m_rollToFamily	= 0;
	split.m_fileNumber		= 1;

	split.inputFile();
	mainTitle();
	ChangeMenu();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnFromtable()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnFromfamily()
{

}
