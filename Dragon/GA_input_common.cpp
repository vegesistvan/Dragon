#include "stdafx.h"
#include "Fa.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
#include "GA_input_parameters.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// S E G É D   F Ü G G V É N Y E K ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////û
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ellenõrzi, hogy a megadott first_name létezik-e a nyilvántartásban.
// Ha létezik, akkor megadja a nemének kódját ( férfi=0, nõ=2 )
// Ha nem létezik, akkor a hiba listába írja és 0 sex_id-t ad vissza!!
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaInput::getSexId( CString first_name )
{
	int sex_id1 = 0;
	int sex_id2 = 0;
	CString name;
	CString utonev;
	
	int pos;

	first_name.Trim();

	int n = countWords( first_name );

	first_name.Replace( '?', ' ' );
	first_name.Replace( '_', ' ' );
	first_name.Replace( '-', ' ' );
	first_name.Trim();

	utonev = getWord( first_name, 1, &pos );
	m_command.Format( L"SELECT sex_id FROM firstnames WHERE first_name = '%s'", utonev );
	if( !theApp.querySystem( m_command ) ) return 0;
	if( theApp.m_recordsetSystem->RecordsCount() )
	sex_id1 = _wtoi( theApp.m_recordsetSystem->GetFieldString( 0 ) );
	if( n > 1 )
	{
		utonev = getWord( first_name, 2, &pos );
		m_command.Format( L"SELECT sex_id FROM firstnames WHERE first_name = '%s'", utonev );
		if( !theApp.querySystem( m_command ) ) return 0;
		if( theApp.m_recordsetSystem->RecordsCount() )
			sex_id2 = _wtoi( theApp.m_recordsetSystem->GetFieldString( 0 ) );
		if( sex_id1 != sex_id2 )
			sex_id1 = -1;
	}
	return sex_id1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaInput::isTitle( CString cLine ) 
{
	m_command = L"SELECT title FROM titles";
	if( !theApp.querySystem( m_command ) ) return FALSE;

	for( UINT i = 0; i < theApp.m_recordsetSystem->RecordsCount(); ++i, theApp.m_recordsetSystem->MoveNext() )
	{
		if( cLine == theApp.m_recordsetSystem->GetFieldString(0 ) ) return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A _,-,? és ) karaktereket leszedi, a maradékot szétszedi. Csak az elsõ szó létezését vizsgálja 
// return:
// -1 nem keresztnév
// 0 biszex
// 1 férfi keresztnév
// 2 nõi keresztnév
int CGaInput::isFirstName( CString name )
{
	name.Replace( ')', ' ' );
	name.Replace( '?', ' ' );
	name.Trim();
	if( name.IsEmpty() ) return -1;

	int pos;
	int i;
	int n;
	CStringArray A;

	if( (pos = name.Find( '_' ) ) != -1 )
		n = wordList( &A, name, '_', FALSE );
	else
		n = wordList( &A, name, '-', FALSE );

	for( i = 0; i < n; ++i )
	{
		m_command.Format( L"SELECT rowid, sex_id FROM firstnames WHERE first_name='%s'", A[i] );
		if( !theApp.querySystem( m_command ) )	return -1;
		if( !theApp.m_recordsetSystem->RecordsCount() ) break;
	}
	if( i == n )
		return _wtoi( theApp.m_recordsetSystem->GetFieldString( 1 ) );
	return -1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
