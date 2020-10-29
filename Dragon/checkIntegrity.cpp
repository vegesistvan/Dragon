// CouplesPeople.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkIntegrity.h"


// CCheckIntegrity
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CCheckIntegrity, CWnd)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckIntegrity::CCheckIntegrity()
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckIntegrity::~CCheckIntegrity()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckIntegrity, CWnd)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckIntegrity::integrity()
{
	CString fname = L"integrity";
	CString title = L"Az szülõkre és házastársakra történõ hivatkozások létezésének ellenõrzése";
	fl = theApp.openLogFile( fname, title);


	wndP.Create( NULL, title );
	wndP.GoModal();

	couples();
	fathers();

	wndP.DestroyWindow();
	fclose( fl );
	theApp.showLogFile();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckIntegrity::couples()
{

	CString spouse1_id;
	CString spouse2_id;
	CString line;
	int		db;
	int summa = 0;


	m_command = L"SELECT rowid, spouse1_id, spouse2_id, linenumber FROM marriages";
	if( !query( m_command ) ) return;

	wndP.SetRange( 0, m_recordset->RecordsCount()*2 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

#ifndef _DEBUG
	str.Format( L"Házasságok ellenõrzése..." );
	wndP.SetText( str );
#endif

	CString rowid;
	bool first = true;

	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i )
	{
		spouse1_id = m_recordset->GetFieldString( 1 );
		m_command.Format( L"SELECT count() FROM people WHERE rowid='%s'", spouse1_id );
		if( !query1( m_command ) ) return;
		db = _wtoi( m_recordset1->GetFieldString( 0 ) );
		if( !db )
		{
			++summa;
			if( first )
			{
				fwprintf( fl, L"Az alábbiak nemlétezõ férjre való hivatkozások a házaspárok táblában.\n\n" );
				fwprintf( fl, L"%10s %8s %9s\n", L"line", L"marriage", L"spouse_id" ); 
				first = false;
			}
			rowid	= m_recordset->GetFieldString( 0 );
			line	= m_recordset->GetFieldString( 3 );
			fwprintf( fl, L"%10s %8s %9s\n", line, rowid, spouse1_id );
		}
		m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	fwprintf( fl, L"\n" );
	if( !summa )
		fwprintf( fl, L"Minden férj létezik!\n\n" );


	first = true;
	summa = 0;
	m_recordset->MoveFirst();
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i )
	{
		spouse2_id = m_recordset->GetFieldString( 2 );
		m_command.Format( L"SELECT count() FROM people WHERE rowid='%s'", spouse2_id );
		if( !query1( m_command ) ) return;
		db = _wtoi( m_recordset1->GetFieldString( 0 ) );
		if( !db )
		{
			++summa;
			if( first )
			{
				fwprintf( fl, L"Az alábbiak nemlétezõ feleségre való hivatkozások a házaspárok táblában.\n\n" );
				fwprintf( fl, L"%8s %9s\n", L"marriage", L"spouse_id" ); 
				first = false;
			}
			rowid = m_recordset->GetFieldString( 0 );
			fwprintf( fl, L"%8s %9s\n", rowid, spouse2_id );
		}
		m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	fwprintf( fl, L"\n" );
	if( !summa	 )
		fwprintf( fl, L"Minden feleség létezik!\n\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckIntegrity::fathers()
{
#ifndef _DEBUG
	str.Format( L"Apák létezésének ellenõrzése..." );
	wndP.SetText( str );
#endif

	m_command = L"SELECT rowid, last_name, first_name, father_id, mother_id FROM people";
	if( !query( m_command ) ) return;

	wndP.SetRange( 0, m_recordset->RecordsCount()*2 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	CString rowid;
	CString name;
	int		father_id;
	CString mother_id;
	int		db;
	int		summa = 0;
	bool	first = true;

	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i )
	{
		father_id = _wtoi( m_recordset->GetFieldString( 3 ) );
		if( father_id > FATHERID || !father_id ) goto cont1;
	//	if( father_id == L"0" || father_id.IsEmpty()  ) goto cont1;
	
		m_command.Format( L"SELECT count() FROM people WHERE rowid='%d'", father_id );
		if( !query1( m_command ) ) return;
		db = _wtoi( m_recordset1->GetFieldString( 0 ) );
		if( !db )
		{
			if( first )
			{
			
				fwprintf( fl, L"Az alábbiak nemlétezõ apára való hivatkozások\n\n" );
				fwprintf( fl, L"%8s %-30s %8s\n", L"rowid", L"name", L"father_id" ); 
				first = false;
			}
			rowid = m_recordset->GetFieldString( 0 );
			name.Format( L"%s %s", m_recordset->GetFieldString(1), m_recordset->GetFieldString( 2 ) );
			
			str.Format( L"%8s %-30s %9d\n", rowid, name, father_id );
			fwprintf( fl, str );
			++summa;
		}
cont1:	m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	fwprintf( fl, L"\n" );
	if( !summa )
		fwprintf( fl, L"Minden apa létezik!\n\n" );

#ifndef _DEBUG
	str.Format( L"Anyák létezésének ellenõrzése..." );
	wndP.SetText( str );
#endif
	first = true;
	summa = 0;
	m_recordset->MoveFirst();
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i )
	{
		mother_id = m_recordset->GetFieldString( 4 );
		if( mother_id == L"0" || mother_id.IsEmpty()  ) goto cont2;
	
		m_command.Format( L"SELECT count() FROM people WHERE rowid='%s'", mother_id );
		if( !query1( m_command ) ) return;
		db = _wtoi( m_recordset1->GetFieldString( 0 ) );
		if( !db )
		{
			if( first )
			{
				fwprintf( fl, L"Az alábbiak nemlétezõ anyára való hivatkozások\n\n" );
				fwprintf( fl, L"%8s %-30s %8s\n", L"rowid", L"name", L"mother_id" ); 
				first = false;
			}
			rowid = m_recordset->GetFieldString( 0 );
			name.Format( L"%s %s", m_recordset->GetFieldString(1), m_recordset->GetFieldString( 2 ) );
			
			str.Format( L"%8s %-30s %9s\n", rowid, name, mother_id );
			fwprintf( fl, str );
			++summa;
		}
cont2:	m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
		fwprintf( fl, L"\n" );
	if( !summa )
		fwprintf( fl, L"Minden anya létezik!\n\n" );

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckIntegrity::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckIntegrity::query1( CString command )
{
	if( m_recordset1->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}

