
#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"

#include "descendants.h"
#include "utilities.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::query( CString command )
{
	if( m_recordset.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::query1( CString command )
{
	if( m_recordset1.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::query2( CString command )
{
	if( m_recordset2.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::query3( CString command )
{
	if( m_recordset3.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::query4( CString command )
{
	if( m_recordset4.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::openHtml( CString file, CString title, UINT colorBgrnd )
{
	CString today;
	today = getPresentDateTime();
	m_startTime = theApp.getStartTime();
	if( m_checkMother )
		str = L"igen";
	else
		str = L"nem";

	CString numbering;
	switch( m_radioNumbering )
	{
		case SZLUHA:
			numbering = L"Szluha-Késmárky";
			break;
		case VIL:
			numbering = L"de Villers/Pama";
			break;
		case TUP:
			numbering = L"Meurgey de Tupigny";
	}
	CString kihagy;
	if (m_checkDeleteRepeated)
		kihagy = L"igen";
	else
		kihagy = L"nem";
	CString maxGen;
	if (m_editGenMax.IsEmpty())
		maxGen = L"minden generáció";
	else
		maxGen = m_editGenMax;

	CString inputFile;
	CString created;
	m_command = L"SELECT filespec, created FROM filespec";
	if (!theApp.query(m_command)) return false;
	inputFile	= theApp.m_recordset->GetFieldString(0);
	created		= theApp.m_recordset->GetFieldString(1);
	CString header;
	header.Format( L"\n\
<pre>\n\
Program:             %s %s\n\
Bemeneti fájl:       %s %s\n\
Adatbázis:           %s\n\
Készült:             %s\n\
Õs:                  %s\n\
Generációk száma:    %s\n\
Számozási rendszer:  %s\n\
Nõk leszármazottai:  %s\n\
Ismétlõdõk kihagyása:%s\n\
</pre>\n\n\
", L"Dragon version ", theApp.m_version, inputFile, created, theApp.m_dbPathName, today, title, maxGen, numbering, str, kihagy );

	
//	CString style;
//	style.Format( L"<style>\n%s\n</style>\n", createStyle() );

	CString body;
	body.Format( L"<body bgcolor=\"%06x\">", colorBgrnd );
	CString font;
	font = L"<body font-family: Avenir Next, Calibri, Verdana, sans-serif>";



	m_htmlFile.Format(L"%s\\%s_%s.html",theApp.m_workingDirectory,file, getTimeTag() );
	m_htmlFile.Format(L"%s\\%s_%s.html", m_descendantsPath, file, getTimeTag());
	m_htmlFile.Format(L"%s\\%s_%s.html", m_descendantsPath, file, getTimeTag());
	if( !openFileSpec( &fl, m_htmlFile, L"w+" ) ) return false;

	print( L"<!DOCTYPE html>\n" );
	print( L"<html lang=hu>\n" );
	print( L"<head>\n" );
	str.Format(L"<title>%s</title>\n", today );
	print( str );
	if( m_radioCode == UTF8 ) print( L"<meta charset=\"UTF-8\">\n" );
	createStyle();
	print( L"</head>\n" );
	print( body );
	print( header );
	





	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::closeHtml()
{
	for( int i =0; i < m_indent; ++i )			// a kihúzások megszüntetése, vissza az alapvonalra
		fwprintf( fl, L"%s", m_tag2 );
	fwprintf( fl, L"</body>\n" );
	fwprintf( fl, L"</html>" );
	str.Format(L"Listázott: %d leszármazott<br>", m_db);
	print(str);
	str.Format(L"Futási idõ: %s\n", theApp.getTimeElapsed(m_startTime));
	print(str);

	fclose( fl );
//	if( m_oneFile )
		ShellExecute(NULL, L"open", m_htmlFile,NULL, NULL, SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::createStyle()
{

	fwprintf( fl, L"<style>\n" );
	if( m_editWidth != L"0" )
		fwprintf( fl, L"html{width:%scm;}\n", m_editWidth );


	CString font;
	font = L"body {font-family: courier, serif;}";
	font = L"body {font-family: Arial, Helvetica, sans-serif;}";
	font = L"body {font-family: Verdana;}";
	font = L"<body class=\"w3-container\">";
//	fwprintf( fl, L"%s\n", font );
	CString fontSize;
	fontSize.Format( L"body {font-size: %dpx;}", 10 + 2* m_comboFontSize );
	fwprintf( fl, L"%s\n", fontSize );

//	fwprintf( fl, L"body {font-size: 10px;}" );
	

//	CString font;
//	font = L"<font-family: Avenir Next, Calibri, Verdana, sans-serif>\n";
//	fwprintf( fl, font );

	if( m_radioNumbering != SZLUHA )
	{
	fwprintf( fl, L"ul {\n" );
	fwprintf( fl, L"display: block;\n" );
	fwprintf( fl, L"max-width: 200%;\n" );
//	fwprintf( fl, L"list-style-type: square;\n" );
	fwprintf( fl, L"list-style-type: none;\n" );
	fwprintf( fl, L"margin-top: 0 em;\n" );
	fwprintf( fl, L"margin-bottom: 0 em;\n" );
	fwprintf( fl, L"margin-left: 0;\n" );
	fwprintf( fl, L"margin-right: 0;\n" );
	fwprintf( fl, L"padding-left: 40px;\n" );
	fwprintf( fl, L"}\n" );
	}
	else
	{
	fwprintf( fl, L"ol {\n" );
	fwprintf( fl, L"display: block;\n" );
	fwprintf( fl, L"list-style-type: decimal;\n" );
	fwprintf( fl, L"margin-top: 0 em;\n" );
	fwprintf( fl, L"margin-bottom: 0 em;\n" );
	fwprintf( fl, L"margin-left: 0;\n" );
	fwprintf( fl, L"margin-right: 0;\n" );
	fwprintf( fl, L"padding-left: 40px;\n" );
	fwprintf( fl, L"}\n" );
	}
	fwprintf( fl, L"</style>\n" );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::print( CString str )
{
	str.Replace( '|', '\'');
	if( m_radioCode == UTF8 )
		str	= UnicodeToUtf8( str ); 
	fwprintf( fl, L"%s", str );  // a soreleji %-okat printelési karakterekenk értelmezné, ha közvetlenül nyomtatnánk!!! 
}