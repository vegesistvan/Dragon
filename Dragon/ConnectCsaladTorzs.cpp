// ConnectCsalad.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "ConnectCsaladTorzs.h"
#include "ProgressWnd.h"
#include "utilities.h"

// CConnectCsalad
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCsaladTorzs()
//{
//	CString info = L"\
//A html f�jlban egy lesz�rmazott lehet egy �j csal�d alap�t�ja. Ezt az [xxx csal�d] jelzi �s a hivatkozott csal�d \
//ezt meger�s�ti a [t�rzs: yyy] jelz�ssel.\n\
//A html f�jl beolvas�s�sa sor�n ezt a sz�l�-gyerek kapcsolatot l�trehozzuk.\r\n\
//Ez a funkci� csak ellen�rzi az adatb�zisban ennek a kapcsolatnak a l�t�t �s list�zza a l�tez� �s nem l�tez� kapcsolatokat is.\r\n\r\n\
//Term�szetesen nem hiba ha az egyik vagy m�sik deklar�ci� nem l�tezik, hiszen lehet, hogy a jelzett alap�t� ill. t�rzs nincs ebben az \
//adatb�zisban.\r\n\r\n\
//Teh�t ez nem egy hibalista, csak egy kimutat�s a html f�jlban megadott csal�d->t�rzs kapcsolatokr�l.\
//\r\n\r\n\
// \
//";
//
//	CConnectCsalad conn;
//	conn.connectCsalad(FALSE);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CConnectCsalad, CWnd)

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
// connect	== true		Az �sszek�t�st l�trehozza az adatb�zisban
//			== false	Csak list�zza a kapcsolatokat
void CConnectCsalad::connectCsalad( BOOL connect )
{
	if( !connect )  // csak list�z�s
	{
		CString fname( L"csal�d-t�rzs" );
		CString title( L"csal�d-t�rzs kapcsolat ellen�rz�se" );
		
		if( theApp.openLogFile( fname,title )==NULL) return;
		CString info = L"\
A program ellen�rzi, hogy az [xxx csal�d]->[t�rzs: XXXX] kapcsolat l�tezik-e. Term�szetesen nem hiba ha az egyik\n\
vagy m�sik deklar�ci� nem l�tezik, teh�t ez nem egy hibalista, csak egy kimutat�s a html f�jlban megadott\n\
csal�d->t�rzs kapcsolatokr�l. Az al�bbi felsorol�sokban megtal�ljuk a neml�tez� �s l�tez� kapcsolatokat is.\n\n";

		fwprintf( theApp.fl, info );
		fwprintf( theApp.fl, L"Az adatb�zisban l�tez� csal�d ap�t�i �s lesz�rmazottai:\n\n" );
	}

	CString rowid;
	CString lineNumber;
	CString tableNumber;
	CString name;
	CString percent;
	CString csalad;
	CString csaladR;
	CString familyName;
	CString family;
	CString roman;
	CString birth_date;
	CString death_date;

	CString rowidF;
	CString nameC;
	CString generation;
	CStringArray A;

	CString rowidM;
	int n;

	CString caption;
	if (connect)
	{
		caption = L"Csal�dalap�t�k [xxx csal�d]->[t�rzs: yyyy] �sszek�t�se...";
	}
	else
	{
		caption = L"Csal�dalap�t�k [xxx csal�d]->[t�rzs: yyyy] kapcsolatok...";
		fwprintf(theApp.fl, L"\n\n%-8s %-20s %-8s %-30s]\n", L"line", L"family", L"rowid", L"name");
	}
	CProgressWnd wndP(NULL, caption); 
	wndP.GoModal();

	
	m_command = L"SELECT rowid, lineNumber, first_name, last_name, csalad, tableNumber, birth_date, death_date FROM people WHERE csalad != '' AND gap == 0 ORDER BY csalad";
	if( !theApp.query( m_command ) ) return;

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);
//#ifndef _DEBUG
//		wndP.SetText( caption );
//#endif


	CString torzs;
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
		n = splitCString( csalad, ' ', false, &A );
		csalad = A[0];

		roman.Empty();
		if( n == 2 )
			roman = A[1];

		csalad.Trim();
		csaladR.Format( L"%s %s", csalad,roman );
		csaladR.Trim();

		m_command.Format( L"SELECT wife_id FROM marriages WHERE husband_id='%s'", rowidF );
		if( !theApp.query3( m_command ) ) return;
		rowidM = theApp.m_recordset3->GetFieldString( 0 );

		m_command.Format( L"SELECT familyName, tableRoman FROM tables WHERE rowid = '%s'", tableNumber ); // a csal�dalap�t� ebbena t�bl�ban van
		if( !theApp.query3( m_command ) ) return;

		family.Format( L"%s %s", theApp.m_recordset3->GetFieldString( 0 ), theApp.m_recordset3->GetFieldString( 1 ) );
		family.Trim();


		if( roman.IsEmpty() )
			m_command.Format( L"SELECT rowid, torzs FROM tables WHERE familyName='%s'", csalad );		// erre a t�bl�ra mutat
		else
			m_command.Format( L"SELECT rowid, torzs FROM tables WHERE familyName='%s' AND tableRoman = '%s'", csalad, roman );		// erre a t�bl�ra mutat
		if( !theApp.query3( m_command ) ) return;

		cnt = 0;
		if( !theApp.execute( L"BEGIN" ) ) return;
		
		for( UINT j = 0; j < theApp.m_recordset3->RecordsCount(); ++j )  // t�bb azonos nev� csal�d is lehet!!!
		{
			tableNumber = theApp.m_recordset3->GetFieldString( 0 ); 
			torzs		= theApp.m_recordset3->GetFieldString( 1 ); 
			torzs.Trim();

			if( !torzs.Compare( family ) )
			{
				if( !connect )
					fwprintf( theApp.fl, L"%8s %-20s %8s %-30s [%s csal�d �se]\n", lineNumber, family, rowidF, name, csaladR );

				// gyerekek
				if( theApp.m_recordset3->RecordsCount() == 0 )
				{
					if( !connect )
						fwprintf( theApp.fl, L"A %s csal�dalap�t�nak nincsenek gyerekei\n", name );
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

							if( connect )
							{
								m_command.Format( L"UPDATE people SET father_id = '%s', mother_id='%s' WHERE rowid = '%s'", rowidF, rowidM, rowid );
								if( !theApp.execute( m_command ) ) return;
							}
							else
								fwprintf( theApp.fl, L"%8s %-20s %8s %-30s [t�rzs: %s]\n", lineNumber, csaladR, rowid, nameC, torzs );
	//							fwprintf( theApp.fl, L"%-20s %8s %8s %-30s %-12s %-12s\n", csaladR, lineNumber, rowid, nameC, birth_date, death_date );
						}
					}
				}
				if( !connect )	fwprintf( theApp.fl, L"\n" );
				break ;
			}
		}
		if( !theApp.execute( L"COMMIT" ) ) return;
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if( connect )
	{
		wndP.DestroyWindow();
		return;		// csak �ssze kellett k�tni
	}


	fwprintf( theApp.fl, L"\n\n" );
	fwprintf( theApp.fl, L"Az adatb�zisban nem l�tez� csal�d alap�t�i: ([XY csal�d �se])\n\n" );

	m_command = L"SELECT rowid, lineNumber, first_name, last_name, csalad, tableNumber FROM people WHERE csalad != '' AND gap == 1 ORDER BY csalad ";
	if( !theApp.query( m_command ) ) return;

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);
#ifndef _DEBUG
		wndP.SetText( L"Az adatb�zisban nem l�tez� csal�d alap�t�i" );
#endif

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid		= theApp.m_recordset->GetFieldString( 0 );
		lineNumber	= theApp.m_recordset->GetFieldString( 1 );
		tableNumber	= theApp.m_recordset->GetFieldString( 5 );
		name.Format( L"%s %s", theApp.m_recordset->GetFieldString( 3 ), theApp.m_recordset->GetFieldString( 2 ) );
		csalad		= theApp.m_recordset->GetFieldString( 4 );
		csalad.Trim();

		m_command.Format( L"SELECT familyName, tableRoman FROM tables WHERE rowid = '%s'", tableNumber ); // a csal�dalap�t� ebbena t�bl�ban van
		if( !theApp.query3( m_command ) ) return;
		familyName	= theApp.m_recordset3->GetFieldString( 0 );
		roman		= theApp.m_recordset3->GetFieldString( 1 );
		family.Format( L"%s %s", familyName, roman );
		family.Trim();

		fwprintf( theApp.fl, L"%8s %-20s %8s %-30s [%s csal�d �se]\n", lineNumber, family, rowid, name, csalad );

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
//	wndP.DestroyWindow();

	fwprintf( theApp.fl, L"\n\n" );
	fwprintf( theApp.fl, L"Az al�bbi [t�rzs: xxxx] jelz�s� csal�doknak nincs alap�t�juk ([xxxx csalad]):\n\n" );
	m_command = L"SELECT rowid, * FROM tables WHERE torzs != '' ORDER BY torzs";
	if( !theApp.query1( m_command ) ) return;


	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);
#ifndef _DEBUG
	wndP.SetText( L"Alap�t�k n�lk�li t�rzsek" );
#endif

	for( UINT i = 0; i < theApp.m_recordset1->RecordsCount(); ++i, theApp.m_recordset1->MoveNext() )
	{
		lineNumber	= theApp.m_recordset1->GetFieldString( DBT_LINENUMBER );
		tableNumber = theApp.m_recordset1->GetFieldString( DBT_ROWID );
		familyName	= theApp.m_recordset1->GetFieldString( DBT_FAMILY_NAME );
		roman		= theApp.m_recordset1->GetFieldString( DBT_TABLENUMBERROMAN );
		percent		= theApp.m_recordset1->GetFieldString( DBT_PERCENT );
		torzs		= theApp.m_recordset1->GetFieldString( DBT_TORZS );
		torzs.Trim();
		csaladR.Format( L"%s %s", familyName, roman );
		csaladR.Trim();

		if( !torzs.IsEmpty() )
		{
			m_command.Format( L"SELECT rowid FROM people WHERE csalad = '%s'", csaladR );
			if( !theApp.query2( m_command ) ) return;
		
			if( !theApp.m_recordset2->RecordsCount() )
			{
				fwprintf( theApp.fl, L"%6s %-3s %-20s [t�rzs: %s]\n", lineNumber, percent, csaladR,  torzs ); 
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


