
#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"

#include "GA_descendants.h"
#include "version.h"
#include "utilities.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaDescendants::query( CString command )
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
BOOL CGaDescendants::query1( CString command )
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
BOOL CGaDescendants::query2( CString command )
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
BOOL CGaDescendants::query3( CString command )
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
BOOL CGaDescendants::query4( CString command )
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
BOOL CGaDescendants::openHtml( CString file, CString title, UINT colorBgrnd )
{
	CString today;
	today = theApp.getPresentDateTime();

	if( m_woman )
		str = L"igen";
	else
		str = L"nem";

	CString numbering;
	switch( m_numbering )
	{
		case SZLUHA:
			numbering = L"Szluha-K�sm�rky";
			break;
		case VIL:
			numbering = L"de Villers/Pama";
			break;
		case TUP:
			numbering = L"Meurgey de Tupigny";
	}
	numbering += L" sz�moz�si rendszer";

	CString header;
	header.Format( L"\n\
<pre>\n\
Program:             %s\n\
Adatb�zis:           %s\n\
K�sz�lt:             %s\n\
Lista:               %s\n\
Lesz�rmazotti lista: %s\n\
N�k lesz�rmazottai:  %s\n\
</pre>\n\n\
", L"Dragon version " VERSION, theApp.m_databaseSpec, today, title, numbering, str  );

	
//	CString style;
//	style.Format( L"<style>\n%s\n</style>\n", createStyle() );

	CString body;
	body.Format( L"<body bgcolor=\"%06x\">", colorBgrnd );
	CString font;
	font = L"<body font-family: Avenir Next, Calibri, Verdana, sans-serif>";

	m_htmlFile.Format(L"%s\\%s_%s.html",theApp.m_workingDirectory,file, getTimeTag() );
	if( !openFileSpec( &fl, m_htmlFile, L"w+" ) ) return false;

	print( L"<!DOCTYPE html>\n" );
	print( L"<html lang=hu>\n" );
	print( L"<head>\n" );
	print( L"<title>Descendant's list</title>\n" );
	if( m_code == UTF8 ) print( L"<meta charset=\"UTF-8\">\n" );
	createStyle();
	print( L"</head>\n" );
	print( body );
	print( header );
	





	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::closeHtml()
{
	for( int i =0; i < m_indent; ++i )			// a kih�z�sok megsz�ntet�se, vissza az alapvonalra
		fwprintf( fl, L"%s\n", m_tag2 );
	fwprintf( fl, L"</body>\n" );
	fwprintf( fl, L"</html>" );
	fclose( fl );
	ShellExecute(NULL, L"open", m_htmlFile,NULL, NULL, SW_SHOWNORMAL);
//	CDialogEx::EndDialog( true );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::createStyle()
{

	fwprintf( fl, L"<style>\n" );
	if( !m_wrap )
		fwprintf( fl, L"html{width:100cm;}\n" );

	CString font;
	font = L"body {font-family: courier, serif;}";
	font = L"body {font-family: Arial, Helvetica, sans-serif;}";
	font = L"body {font-family: Verdana;}";
	font = L"<body class=\"w3-container\">";
//	fwprintf( fl, L"%s\n", font );
	CString fontSize;
	fontSize.Format( L"body {font-size: %dpx;}", 10 + 2* m_ixFontSize );
	fwprintf( fl, L"%s\n", fontSize );

//	fwprintf( fl, L"body {font-size: 10px;}" );
	

//	CString font;
//	font = L"<font-family: Avenir Next, Calibri, Verdana, sans-serif>\n";
//	fwprintf( fl, font );

	if( m_numbering != 0 )
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
void CGaDescendants::print( CString str )
{
	if( m_code == UTF8 )
		str	= UnicodeToUtf8( str ); 
	fwprintf( fl, L"%s", str );  // a soreleji %-okat printel�si karakterekenk �rtelmezn�, ha k�zvetlen�l nyomtatn�nk!!! 
}