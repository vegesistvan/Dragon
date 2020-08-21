// ConnectCsalad.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "ConnectCsalad.h"


// CConnectCsalad

IMPLEMENT_DYNAMIC(CConnectCsalad, CWnd)

	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCsaladTorzs()
{
	CString info = L"\
A html fájlban egy leszármazott lehet egy új család alapítója. Ezt az [xxx család] jelzi és a hivatkozott család \
ezt megerõsíti a [törzs: yyy] jelzéssel.\n\
A html fájl beolvasásása során ezt a szülõ-gyerek kapcsolatot létrehozzuk.\r\n\
Ez a funkció csak ellenõrzi az adatbázisban ennek a kapcsolatnak a létét és listázza a létezõ és nem létezõ kapcsolatokat is.\r\n\r\n\
Természetesen nem hiba ha az egyik vagy másik deklaráció nem létezik, hiszen lehet, hogy a jelzett alapító ill. törzs nincs ebben az \
adatbázisban.\r\n\r\n\
Tehát ez nem egy hibalista, csak egy kimutatás a html fájlban megadott család->törzs kapcsolatokról.\
\r\n\r\n\
 \
";
	if( AfxMessageBox( info, MB_OKCANCEL|MB_ICONINFORMATION ) == IDCANCEL )
	{
		return;
	}

	CConnectCsalad conn;
	conn.connectCsalad( FALSE );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnCsalad()
{
	CConnectCsalad conn;
	conn.connectCsalad( TRUE );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnConnectCsalad()
{
	CConnectCsalad conn;
	conn.connectCsalad( FALSE );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CConnectCsalad::CConnectCsalad()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CConnectCsalad::~CConnectCsalad()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CConnectCsalad, CWnd)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// flag	== true		Az összekötést létrehozza az adatbázisban
//		== false	Csak listázza a kapcsolatokat
void CConnectCsalad::connectCsalad( BOOL flag )
{
	CString fname( L"root" );
	CString title( L"új család-root kapcsolat ellenõrzése" );
	CString attention;
	CString torzs;

	if( theApp.openLogFile( fname,title )==NULL)
		return;

	CString info = L"\
A program ellenõrzi, hogy az [xxx család]->[törzs: XXXX] kapcsolat létezik-e. Természetesen nem hiba ha az egyik\n\
vagy másik deklaráció nem létezik, tehát ez nem egy hibalista, csak egy kimutatás a html fájlban megadott\n\
család->törzs kapcsolatokról. Az alábbi felsorolásokban megtaláljuk a nemlétezõ és létezõ kapcsolatokat is.\n\n";

	fwprintf( theApp.fl, info );

	CString rowid;
	CString lineNumber;
	CString tableNumber;
	CString name;
	CString percent;
	CString csalad;
	CString csaladR;
	CString newFamilies;
	CString familyName;
	CString family;
	CString roman;
	CString birth_date;
	CString death_date;

	CString rowidF;
	CString nameF;
	CString rowidM;
	CString nameM;
	CString children;
	CString rowidC;
	CString nameC;
	CString generation;
	CStringArray A;
	int		n;
	
	CProgressWnd wndP(NULL, L"Családalapítók összekötése..."); 
	wndP.GoModal();

	fwprintf( theApp.fl, L"Az adatbázisban létezõ család apítói és leszármazottai:\n\n" );
	m_command = L"SELECT rowid, lineNumber, first_name, last_name, csalad, tableNumber, birth_date, death_date FROM people WHERE csalad != '' AND gap == 0 ORDER BY csalad";
	if( !theApp.query( m_command ) ) return;

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);
#ifndef _DEBUG
		wndP.SetText( L"Családalapítók és gyeremekeik összekötése" );
#endif



	int cnt = 0;
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowidF		= theApp.m_recordset->GetFieldString( 0 );
		lineNumber	= theApp.m_recordset->GetFieldString( 1 );
		tableNumber	= theApp.m_recordset->GetFieldString( 5 );
		birth_date	= theApp.m_recordset->GetFieldString( 6 );
		death_date	= theApp.m_recordset->GetFieldString( 7 );
		name.Format( L"%s %s", theApp.m_recordset->GetFieldString( 3 ), theApp.m_recordset->GetFieldString( 2 ) );
		csalad		= theApp.m_recordset->GetFieldString( 4 );
		n = wordList( &A, csalad, ' ', FALSE );
		csalad = A[0];

		roman.Empty();
		if( n == 2 )
			roman = A[1];

		csalad.Trim();
		csaladR.Format( L"%s %s", csalad,roman );
		csaladR.Trim();

		m_command.Format( L"SELECT familyName, tableRoman FROM tables WHERE rowid = '%s'", tableNumber ); // a családalapító ebbena táblában van
		if( !theApp.query3( m_command ) ) return;

		family.Format( L"%s %s", theApp.m_recordset3->GetFieldString( 0 ), theApp.m_recordset3->GetFieldString( 1 ) );
		family.Trim();


		if( roman.IsEmpty() )
			m_command.Format( L"SELECT rowid, torzs FROM tables WHERE familyName='%s'", csalad );		// erre a táblára mutat
		else
			m_command.Format( L"SELECT rowid, torzs FROM tables WHERE familyName='%s' AND tableRoman = '%s'", csalad, roman );		// erre a táblára mutat
		if( !theApp.query3( m_command ) ) return;

		cnt = 0;
		for( UINT j = 0; j < theApp.m_recordset3->RecordsCount(); ++j )  // több azonos nevû család is lehet!!!
		{
			tableNumber = theApp.m_recordset3->GetFieldString( 0 ); 
			torzs		= theApp.m_recordset3->GetFieldString( 1 ); 
			torzs.Trim();

			if( !torzs.Compare( family ) )
			{
				fwprintf( theApp.fl, L"%-20s %8s %8s %-30s %-12s %-12s [%s család]\n", family, lineNumber, rowidF, name, birth_date, death_date, csaladR );

				// gyerekek
				if( theApp.m_recordset3->RecordsCount() == 0 )
				{
					fwprintf( theApp.fl, L"!!!!!!!!!!!!!!!!!!!!\n" );
				}
				else
				{
					m_command.Format( L"SELECT generation FROM people WHERE tableNumber='%s' AND tableAncestry", tableNumber );
					if( !theApp.query3( m_command ) ) return;
					if( theApp.m_recordset3->RecordsCount() )
					{
						generation = theApp.m_recordset3->GetFieldString( 0 );
						m_command.Format( L"SELECT rowid, lineNumber, last_name, first_name, birth_date, death_date  FROM people WHERE tableNumber='%s' AND generation ='%s'", tableNumber, generation );
						if( !theApp.query3( m_command ) ) return;
						for( UINT i = 0; i < theApp.m_recordset3->RecordsCount(); ++i, theApp.m_recordset3->MoveNext() )
						{
							rowid		= theApp.m_recordset3->GetFieldString( 0 );
							lineNumber	= theApp.m_recordset3->GetFieldString( 1 );
							nameC.Format( L"%s %s", theApp.m_recordset3->GetFieldString(2), theApp.m_recordset3->GetFieldString( 3 ));
							birth_date	= theApp.m_recordset3->GetFieldString( 4 );
							death_date	= theApp.m_recordset3->GetFieldString( 5 );

							fwprintf( theApp.fl, L"%-20s %8s %8s %-30s %-12s %-12s\n", csaladR, lineNumber, rowid, nameC, birth_date, death_date );

							if( flag )
							{
								m_command.Format( L"UPDATE people SET father_id = '%s' WHERE rowid = '%s'", rowidF, rowid );
								if( !theApp.execute( m_command ) ) return;
							}
						}
					}
				}
				fwprintf( theApp.fl, L"\n" );
				break ;
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	fwprintf( theApp.fl, L"\n\n" );

	if( flag )
	{
		fclose( theApp.fl );
		return;		// csak az õsöket és leszármazottakat kellett összekötni
	}


	fwprintf( theApp.fl, L"Az adatbázisban nem létezõ család alapítói: ([XY család õse])\n\n" );

	m_command = L"SELECT rowid, lineNumber, first_name, last_name, csalad, tableNumber FROM people WHERE csalad != '' AND gap == 1 ORDER BY csalad ";
	if( !theApp.query( m_command ) ) return;

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);
#ifndef _DEBUG
		wndP.SetText( L"Aadtbázisban nem létezõ család alapítói" );
#endif

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid		= theApp.m_recordset->GetFieldString( 0 );
		lineNumber	= theApp.m_recordset->GetFieldString( 1 );
		tableNumber	= theApp.m_recordset->GetFieldString( 5 );
		name.Format( L"%s %s", theApp.m_recordset->GetFieldString( 3 ), theApp.m_recordset->GetFieldString( 2 ) );
		csalad		= theApp.m_recordset->GetFieldString( 4 );
		csalad.Trim();

		m_command.Format( L"SELECT familyName, tableRoman FROM tables WHERE rowid = '%s'", tableNumber ); // a családalapító ebbena táblában van
		if( !theApp.query3( m_command ) ) return;
		familyName	= theApp.m_recordset3->GetFieldString( 0 );
		roman		= theApp.m_recordset3->GetFieldString( 1 );
		family.Format( L"%s %s", familyName, roman );
		family.Trim();

		fwprintf( theApp.fl, L"%-20s %8s %8s %-30s [%s család õse]\n", family, lineNumber, rowid, name, csalad );

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	fwprintf( theApp.fl, L"\n\n" );


	fwprintf( theApp.fl, L"Az alábbi [törzs: xxxx] jelzésû családoknak nincs alapítójuk ([xxxx csalad]):\n\n" );

	m_command = L"SELECT rowid, * FROM tables WHERE torzs != '' ORDER BY torzs";
	if( !theApp.query1( m_command ) ) return;
	

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);
#ifndef _DEBUG
		wndP.SetText( L"Alapítók nélküli törzsek" );
#endif


	for( UINT i = 0; i < theApp.m_recordset1->RecordsCount(); ++i, theApp.m_recordset1->MoveNext() )
	{
		lineNumber	= theApp.m_recordset1->GetFieldString( TABLES_LINENUMBER );
		tableNumber = theApp.m_recordset1->GetFieldString( TABLES_ROWID );
		familyName	= theApp.m_recordset1->GetFieldString( TABLES_FAMILY_NAME );
		roman		= theApp.m_recordset1->GetFieldString( TABLES_TABLENUMBERROMAN );
		percent		= theApp.m_recordset1->GetFieldString( TABLES_PERCENT );
		torzs		= theApp.m_recordset1->GetFieldString( TABLES_TORZS );
		torzs.Trim();
		csaladR.Format( L"%s %s", familyName, roman );
		csaladR.Trim();


		if( !torzs.IsEmpty() )
		{
//			m_command.Format( L"SELECT rowid FROM tables WHERE familyName='%s' AND newFamilies LIKE '%c%s%c'", torzs, '%', familyName, '%' );
			m_command.Format( L"SELECT rowid FROM people WHERE csalad = '%s'", csaladR );
			if( !theApp.query2( m_command ) ) return;
		
			if( !theApp.m_recordset2->RecordsCount() )
			{
	
				fwprintf( theApp.fl, L"%6s %-3s %-20s [törzs: %s]\n", lineNumber, percent, csaladR,  torzs ); 
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if(wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	fwprintf( theApp.fl, L"\n\n" );
	fclose(theApp.fl);
	theApp.showLogFile();	
}


