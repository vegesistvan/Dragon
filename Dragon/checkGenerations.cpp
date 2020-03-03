// Generations.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "checkGenerations.h"
#include "afxdialogex.h"

enum
{
	G_TABLENUMBER = 0,
	G_LINENUMBER,
	G_GENERATION,
	G_ROWID,
	G_LAST_NAME,
	G_FIRST_NAME
};

IMPLEMENT_DYNAMIC(CcheckGenerations, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CcheckGenerations::CcheckGenerations(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcheckGenerations::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CcheckGenerations::~CcheckGenerations()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckGenerations::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CcheckGenerations, CDialogEx)
	ON_BN_CLICKED(IDOK, &CcheckGenerations::OnBnClickedOk)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CcheckGenerations::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString info = L"\
A program az adatbázisban lévõ táblák egymást követõ leszármazottainak generációs kódjait ellenõrzi. Az alábbi esetekben jelez hibát:\r\n\r\n\
-ha  egy leszármazott generációs kódja kisebb, mint a tábla õsének generációs kódja,\r\n\
-ha egy leszármazott generációs kódja nem egyet lépett a szülõhöz képest.\r\n\r\n\
A hiba lehet a html fájlban vagy oka lehet az emberek hibás azonosítása miatt tévesen egyesített emerek.\
\r\n\
";

	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CcheckGenerations::OnBnClickedOk()
{
	CString fileName( L"generationCodes" );
	CString fileSpec;
	CString explanation( L"Az alábbi esetekben jelez hibáz az ellenõrzés:<br>\
-ha  egy leszármazott generációs kódja kisebb, mint a tábla õsének generációs kódja,<br>\
-ha egy leszármazott generációs kódja nem egyet lépett a szülõhöz képest.<br>" );

	CString title( L"Generációs kódok ellenõrzése");

//	m_command = L"SELECT tableNumber, lineNumber, generation, source, rowid, last_name, first_name FROM people ORDER BY lineNumber";

	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );

	fwprintf( fh1, L"<HEAD>\n" );
	fwprintf( fh1, L"<style>\n" );
	fwprintf( fh1, L"</style>\n" );
	fwprintf( fh1, L"</HEAD>\n" );
	fwprintf( fh1, L"<BODY>\n" );
	fwprintf( fh1, L"<font color='red'><center>%s</center><br><br></font>\n", title );
	fwprintf( fh1, explanation );
	
	fwprintf( fh1, L"<pre>" );
	fwprintf( fh1, L"%-25s %s<br>", L"Adatbázis", theApp.m_databaseSpec );
	fwprintf( fh1, L"%-25s %s<br>", L"lista készült", theApp.getPresentDateTime() );
	fwprintf( fh1, L"%-25s %d<br><br><br>", L"Elvégzett összevonások:", theApp._iterationCount );

	CProgressWnd wndProgress(NULL, title ); 
	wndProgress.GoModal();
#ifndef _DEBUG
	wndProgress.SetText( L"Generációs kódok beolvasása folyik..." );
#endif
	m_command = L"SELECT tableNumber, lineNumber, generation, rowid, last_name, first_name FROM people WHERE source == 1 ORDER BY lineNumber";
	if( !theApp.query( m_command ) ) return;

	wndProgress.SetRange(0, theApp.m_recordset->RecordsCount()*2);
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);
#ifndef _DEBUG
	wndProgress.SetText( L"Generációs kódok ellenõrzése..." );
#endif

	CString tableName;
	CString tableNumber(L"");

	CString firstRowid;
	CString firstLineNumber;
	CString firstGeneration(L"");

	CString rowid;
	CString	lineNumber(L"");
	CString generation;

	CString name1;
	CString name2;

	CString prevRowid;
	CString prevLineNumber;
	CString prevGeneration;

	BOOL FIRST = TRUE;

	int diff;

	m_cnt = 0;
	
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid	= theApp.m_recordset->GetFieldString( G_ROWID );
		lineNumber	= theApp.m_recordset->GetFieldString( G_LINENUMBER );
		generation	= theApp.m_recordset->GetFieldString( G_GENERATION );
		name2.Format( L"%s %s", theApp.m_recordset->GetFieldString(G_LAST_NAME), theApp.m_recordset->GetFieldString(G_FIRST_NAME) );

		if( tableNumber.Compare( theApp.m_recordset->GetFieldString( G_TABLENUMBER ) ) )  // új tábla kezdõdik
		{
			tableNumber	= theApp.m_recordset->GetFieldString( G_TABLENUMBER );
			m_command.Format( L"SELECT tableheader FROM tables WHERE rowid == '%s'", tableNumber );
			if( !theApp.query1( m_command ) ) return;
			tableName	= theApp.m_recordset1->GetFieldString( 0 );

			firstLineNumber	= theApp.m_recordset->GetFieldString( G_LINENUMBER );
			firstRowid	= theApp.m_recordset->GetFieldString( G_ROWID );
			name1.Format( L"%s %s", theApp.m_recordset->GetFieldString(G_LAST_NAME), theApp.m_recordset->GetFieldString(G_FIRST_NAME) );
			firstGeneration	= generation;
//			prevGeneration	= generation;
		}
		else
		{
			if( generation < firstGeneration )
			{
				if( FIRST )
				{
					fwprintf( fh1, L"<font color='red'>Egy leszármazott generációs kódja kisebb, mint az õsé!</font><br><br>" );
					FIRST = FALSE;
				}
				fwprintf( fh1, L"%-30s %8sT %8sL %8sR %s %s\n", tableName.Left(30), tableNumber, firstLineNumber, firstRowid, firstGeneration, name1  );
				fwprintf( fh1, L"%49sL %8sR %s %s<br><br>",  lineNumber, rowid, generation, name2 );
				++m_cnt;
			}
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}


	// generációs kód egyesével csökken?
	FIRST = TRUE;
	theApp.m_recordset->MoveFirst();
	tableNumber.Empty();

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid	= theApp.m_recordset->GetFieldString( G_ROWID );
		lineNumber	= theApp.m_recordset->GetFieldString( G_LINENUMBER );
		generation	= theApp.m_recordset->GetFieldString( G_GENERATION );
		
		if( tableNumber.Compare( theApp.m_recordset->GetFieldString( G_TABLENUMBER ) ) )  // új tábla kezdõdik
		{
			tableNumber	= theApp.m_recordset->GetFieldString( G_TABLENUMBER );
			m_command.Format( L"SELECT tableheader FROM tables WHERE rowid == '%s'", tableNumber );
			if( !theApp.query1( m_command ) ) return;
			tableName	= theApp.m_recordset1->GetFieldString( 0 );
		}
		else
		{
			if( generation > prevGeneration )
			{
				diff = (int)generation[0] - (int)prevGeneration[0];
				if( diff != 1 && diff != 7  )   // diff = 7, ha Z-a generációs kódok követik egymást
				{
					if( FIRST )
					{
						fwprintf( fh1, L"<font color='red'>Egy leszármazott generációs kódja egynél nagyobbat lépett!</font><br><br>" );
						FIRST = FALSE;
					}
					fwprintf( fh1, L"%-30s %8sT ", tableName.Left(30), tableNumber );
					fwprintf( fh1, L"%8sL %8sR %s<br>", prevLineNumber, prevRowid, prevGeneration );
					fwprintf( fh1, L"%49sL %8sR %s<br><br>", lineNumber, rowid, generation );
					++m_cnt;
				}
			}
		}
		prevRowid		= rowid;
		prevLineNumber	= lineNumber;
		prevGeneration	= generation;

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	fwprintf( fh1, L"</pre>" );
	fclose( fh1 );

	if( !m_cnt )
		AfxMessageBox( L"Nem találtam hibás generációs kódot!" );
	else
		theApp.showHtmlFile( fileSpec );

	CDialogEx::OnOK();
}


