#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "utilities.h"
#include <algorithm>
#include "GetString.h"
#include "Languages.h"
#include "Table_places.h"
#include "Table_firstnames.h"
#include "Message.h"
#include "html_Edit.h"
#include "SaveFirstName.h"
#include "version.h"
#include "ProgressWnd.h"
#include "html_Edit2Lines.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool sortByName1(const CString &v1, const CString &v2);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::openTextFile( FILE ** fl, CString fileName, CString mode )
{
	CString fileSpec;
	fileSpec.Format(L"%s\\%s_%s.text",m_workingDirectory,fileName, getTimeTag() );
	if( !openFileSpec( fl, fileSpec, mode ) ) return NULL;
	return fileSpec;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::openHtmlFile( FILE** fh,  CString fileName, CString mode )
{
	CString fileSpec;
	fileSpec.Format(L"%s\\%s_%s.html",m_workingDirectory,fileName, getTimeTag() );
	if( !openFileSpec( fh, fileSpec, mode ) ) return NULL;

	setStartTime();
	return fileSpec;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FILE * CDragonApp::openLogFile(CString fname,CString title)
{
	CString	sor;
	int		n;
	
	int		errno_t;
	CString text;
	TCHAR	error_msg[100];
	CString mode(L"w+");

	m_listFile.Format(L"%s\\%s_%s.text",m_workingDirectory,fname, getTimeTag() );
	if( !openFileSpec( &fl, m_listFile, mode ) ) return NULL;


	if(title.GetLength() )
	{
		n = title.GetLength()/2;	
		n	= 40 + title.GetLength()/2;
		
		

		CStringA utf8;
		CStringW multi = L"öüóõúáûéí";

		LPCSTR szTemp = (LPCSTR)(LPCTSTR)multi;

		utf8 = MultiToUtf8( (LPCSTR)szTemp );

		fwprintf(fl,L"%*s\n",n,title);

		str.Format(L"Listázás dátuma: %s",getPresentDateTime());
		n = str.GetLength()/2;	
		n	= 40 + str.GetLength()/2;
		fwprintf(fl,L"%*s\n",n,str);

		str.Format(L"Dragon v. %s",VERSION);
		n = str.GetLength()/2;	
		n	= 40 + str.GetLength()/2;
		fwprintf(fl,L"%*s\n",n,str);
	
		str.Format( L"Adatbázis: %s", m_databaseSpec );
		n = str.GetLength()/2;	
		n	= 40 + str.GetLength()/2;
		fwprintf(fl,L"%*s\n",n,str);
	}


	fwprintf( fl, L"\n\n" );
	setStartTime();
	return fl;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::showLogFile()
{
	CString fileSpec;
	fileSpec.Format( L"\"%s\"", m_listFile );
	ShellExecute(NULL,L"open",m_editorName, fileSpec, m_editorFolder,SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::showFile(CString file )
{
	CString	fileSpec;
	fileSpec.Format(L"\"%s\"", file );
	ShellExecute(NULL,L"open",m_editorName, fileSpec, m_editorFolder,SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::showHtmlFile(CString file )
{
	CString	fileSpec;
	fileSpec.Format(L"\"%s\"", file );
	ShellExecute(NULL, L"open", fileSpec,NULL, NULL, SW_SHOWNORMAL);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::editNotepad( CString lineNumber )
{
	CString	fileSpec;
	fileSpec.Format( L"\"%s\" -n%s -alwaysOnTop -nosession", theApp.m_htmlFileSpec, lineNumber );
	ShellExecute(NULL,L"open",m_editorName, fileSpec, m_editorFolder,SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::notepad( CString file, CString option )
{
	CString	fileSpec;
	fileSpec.Format( L"\"%s\" %s",  file, option );
	ShellExecute(NULL,L"open",m_editorName, fileSpec, m_editorFolder,SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::getPresentDateTime() 
{
	CString		dateTime;
	CString		str;
	
	m_command = L"SELECT datetime('now','localtime')";
	if( !query( m_command ) ) return L""; 
	dateTime= m_recordset->GetFieldString(0);
	dateTime.Replace('-','.');
	return dateTime;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::getDateIFromStr( CString dateS ) 
{
	CString dateI;
	CString datum;

	dateI.Empty();
	if( !dateS.IsEmpty() )
	{
		dateS.Replace('.','-');
		datum.Format( L"%s 00:00:00.000", dateS );
		m_command.Format(L"SELECT strftime('%%s', '%s 00:00:00.000','utc')",dateS);  // pontos datum-idõ kell, hogy 1970.01.01 -et == 0-ra 
		if( !query2( m_command ) ) return dateI;
		dateI = m_recordset2->GetFieldString(0);
	}
	dateI.Trim();
	if(dateI.IsEmpty()) dateI=L"0";
	return dateI;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString	CDragonApp::getDateStrFromI( CString dateI ) 
{
	CString	str;
	CTime	ido;

	str.Empty();
	ido = _wtoi(dateI);
	if( !dateI.IsEmpty() && ido!= 0  )
	{
		m_command.Format(L"SELECT date('%s','unixepoch','localtime')",dateI);
		if( !query( m_command ) ) return str;

		str = m_recordset->GetFieldString(0);
		str.Replace('-','.');
		str.Replace(' ','-');
	}
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
_int64 CDragonApp::getPresentTime()
{
	m_command = L"SELECT strftime('%s','now' )";
	if( !query4( m_command ) ) return 0;
	return ( _wtoi( m_recordset4->GetFieldString( 0 ) ) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::setStartTime()
{
	m_command = L"SELECT strftime('%s','now' )";
	if( !query( m_command ) ) return;
	m_time_start = _wtoi( m_recordset->GetFieldString( 0 ) );	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::get_time_elapsed()
{
	int time_end;

	m_command = L"SELECT strftime('%s','now' )";
	if( !query( m_command ) ) return L"";
	time_end = _wtoi( m_recordset->GetFieldString( 0 ) );	

	CString format = L"%H:%M:%S";
	m_command.Format( L"SELECT strftime( '%s', '%d', 'unixepoch')", format, time_end-m_time_start );
	if( !query( m_command ) ) return L"";
	return( m_recordset->GetFieldString(0) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ellenõrtuzi, hogy a megadott 2 dátum között van-e 9 hónap különbség.
// return TRUE: ha 9 hónapnál nagyobb
//        FALSE: ha 9 hónapnál kisebb
BOOL CDragonApp::dateDiff(  CString date1, CString date2, int month  )
{
	CString str1;
	CString str2;
	CString d1;
	CString d2;
	int		pos;

	if( date1.GetLength() == 8 )
		date1 += "01";
	if( date2.GetLength() == 8 )
		date2 += "01";

	if( date1.GetLength() < 10 || date2.GetLength() < 10 || (pos=date1.Find('?')) != -1 || (pos=date2.Find('?')) != -1 ) return FALSE;


	str1 = getDateI( date1, month );
	str2 = getDateI( date2, 0 );

	_int64 time1 = _wtoi64( str1 );
	_int64 time2 = _wtoi64( str2 );


	d1 = getDateStrFromI( str1 );
	d2 = getDateStrFromI( str2 );

	if( time1 < time2 )			// apa halála + 9 hónap kisebb mint a gyerek születése
	{
		return TRUE;
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::getDateI( CString dateS, int month ) 
{
	CString dateI;
	CString datum;
	CString modifier;

	dateI.Empty();
	if( !dateS.IsEmpty() )
	{
		dateS.Replace('.','-');
		datum.Format( L"%s 00:00:00.000", dateS );
		modifier.Format( L"+%d month", month );
		m_command.Format(L"SELECT strftime('%%s', '%s 00:00:00.000', '%s')",dateS, modifier);  // pontos datum-idõ kell, hogy 1970.01.01 -et == 0-ra 
		if( !query2( m_command ) ) return dateI;
		dateI = m_recordset2->GetFieldString(0);
	}
	dateI.Trim();
	if(dateI.IsEmpty()) dateI=L"0";
	return dateI;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CStringA CDragonApp::MultiToUtf8( LPCSTR multi )
{
	CStringA utf8;
	utf8 = UnicodeToUtf8(MultiToUnicode( multi ));
	return utf8;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::getLanguage( int language_id )
{
	m_command.Format( L"SELECT language FROM languages WHERE rowid = '%d'", language_id );
	if( !querySystem( m_command ) ) return L"";
	return( m_recordsetSystem->GetFieldString( 0 ) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CDragonApp::getUserVersion()
{
	m_command.Format( L"PRAGMA %s", L"user_version" );
	if( !query( m_command ) ) return 0;
	return( _wtoi( m_recordset->GetFieldString( 0 ) ) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::setUserVersion( int user_version )
{
	int volt = getUserVersion();
	user_version |= getUserVersion();
	m_command.Format(L"PRAGMA user_version=%d", user_version );
	if( !theApp.execute( m_command ) ) return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a rowid emberre történõ hivatkozásokat kicseréli rowidBy-ra és magát a rowid-t törli
// 
void CDragonApp::replaceBy( CString rowid, CString rowidBy, CString sex_id, int source )
{
	int united;
	int	spouse;
	int	spouseparent;
	int spousespouse;

	if( sex_id == L"1" )
		m_command.Format( L"UPDATE marriages SET spouse1_id='%s' WHERE spouse1_id='%s'", rowidBy, rowid );
	else
		m_command.Format( L"UPDATE marriages SET spouse2_id='%s' WHERE spouse2_id='%s'", rowidBy, rowid );
	if( !theApp.execute( m_command ) ) return;

	if( sex_id == L"1" )
		m_command.Format( L"UPDATE people SET father_id = '%s' WHERE father_id='%s'", rowidBy, rowid );
	else
		m_command.Format( L"UPDATE people SET mother_id = '%s' WHERE mother_id='%s'", rowidBy, rowid );
	if( !theApp.execute( m_command ) ) return;

//	saveInfo( rowid, rowidBy );

	m_command.Format( L"DELETE FROM people WHERE rowid ='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;

// az egyesítések számát 1-el növeli
	m_command.Format( L"SELECT united, spouse, spouseparent, spousespouse FROM people WHERE rowid='%s'", rowidBy );
	if( !query1( m_command ) ) return;
	united			= _wtoi( m_recordset1->GetFieldString( 0 ) ) + 1;
	spouse			= _wtoi( m_recordset1->GetFieldString( 1 ) );
	spouseparent	= _wtoi( m_recordset1->GetFieldString( 2 ) );
	spousespouse	= _wtoi( m_recordset1->GetFieldString( 3 ) );

	switch( source )
	{
	case 2:
		++spouse;
		break;
	case 3:
		++spouseparent;
		break;
	case 4:
		++spouseparent;
		break;
	case 5:
		++spousespouse;
		break;
	}

	m_command.Format( L"UPDATE people SET united='%d', spouse='%d', spouseparent='%d', spousespouse='%d' WHERE rowid ='%s'",united, spouse, spouseparent, spousespouse, rowidBy );
	if( !theApp.execute( m_command ) ) return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::saveInfo( CString rowid, CString rowidBy )
{
	m_command.Format( L"SELECT rowid, * FROM people WHERE rowid == '%s'", rowid );
	if( !query1( m_command ) ) return;
	m_command.Format( L"SELECT rowid, * FROM people WHERE rowid == '%s'", rowidBy );
	if( !query2( m_command ) ) return;

	CString titolo		= m_recordset1->GetFieldString( PEOPLE_TITOLO );
	CString titoloBy	= m_recordset2->GetFieldString( PEOPLE_TITOLO );
	if( !titolo.IsEmpty() && titoloBy.IsEmpty() )
	{
		m_command.Format( L"UPDATE people SET titolo='%s' WHERE rowid ='%s'", titolo, rowidBy );
		if( !theApp.execute( m_command ) ) return;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::contractions()
{
	CString contr(L"");
	
	m_command = L"SELECT mode FROM united";
	if( !query( m_command ) ) return L"0";

	for( UINT i=0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		contr +=  m_recordset->GetFieldString( 0 );
		contr += L"+";
	}
	return( contr.Left( contr.GetLength() - 1 ) );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A _,-,? és ) karaktereket leszedi, a maradékot szétszedi. Csak az elsõ szó létezését vizsgálja 
// return:
// -1 nem keresztnév
// 0 biszex
// 1 férfi keresztnév
// 2 nõi keresztnév
int CDragonApp::isFirstName( CString name )
{
//	name.Replace( ')', ' ' );
	name.Replace( '?', ' ' );
	name.Trim();
	if( name.IsEmpty() ) return -1;


	int z;
	int pos;
	int i;
	int n;
	CStringArray A;

	if( (pos = name.Find( '_' ) ) != -1 )
		n = wordList( &A, name, '_', FALSE );
	else if( (pos = name.Find( '-' ) ) != -1 )
		n = wordList( &A, name, '-', FALSE );
	else
		n = wordList( &A, name, ' ', FALSE );


	for( i = 0; i < n; ++i )
	{
		m_command.Format( L"SELECT rowid, sex_id FROM firstnames WHERE first_name='%s'", A[i] );
		if( !theApp.querySystem( m_command ) )	return -1;
		if( !theApp.m_recordsetSystem->RecordsCount() ) break;
	}
	if( i == n )
		return _wtoi( theApp.m_recordsetSystem->GetFieldString( 1 ) );		// megvan az adatbázisban, visszadja a sex-et

	CSaveFirstName dlg;
	dlg.m_first_name = A[i];
	if( dlg.DoModal() == IDCANCEL ) return -1;

	m_command.Format( L"INSERT INTO firstnames (first_name, sex_id ) VALUES ('%s', '%d' )", A[i], dlg.m_sex_id );
	if( !executeSys( m_command ) ) return -1;
	return ( dlg.m_sex_id );

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CDragonApp::getNumberOfDb( std::vector<CString>* vE )
{
	int pos;
	splitFilespec( m_databaseSpec, &drive, &path, &filename, &ext );

	CString baseName;
	CString fileName;
	if( ( pos = theApp.m_databaseName.ReverseFind( '.' ) ) != -1 )
	{
		baseName = theApp.m_databaseName.Left( pos );
	}
	if( ( pos = m_databaseName.Find( '+' ) ) != -1 )
	{
		baseName = m_databaseName.Left( pos );
	}
	CString findFile;
	findFile.Format(L"%s:%s\\%s*.db", drive, path, baseName );

	
	CString fileSpec;
	CFileFind finder;
	BOOL bWorking = finder.FindFile( findFile );
	while (bWorking)
	{
		bWorking	= finder.FindNextFile();
		fileName	= finder.GetFileName();
		if( finder.IsDirectory() ) continue;
	
		fileSpec.Format( L"%s\\%s", theApp.m_databasePath, fileName );
		vE->push_back(fileSpec );
	}
	finder.Close();

	std::sort( vE->begin(), vE->end(), sortByName1 );

	return (int)vE->size();
}

bool sortByName1(const CString &v1, const CString &v2) 
{ 
    return( v1 > v2 ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnHusbandWife()
{
	CString rowid;
	CString spouse1_id;
	CString spouse2_id;
	int sex_id;

	if( AfxMessageBox( L"Férj legyen spouse1_id, feleség pedig spouse2_id.\nHa szükséges cserélje fel.", MB_OKCANCEL ) == IDCANCEL ) return;

	m_command.Format( L"SELECT rowid, spouse1_id, spouse2_id FROM marriages" );
	if( !query( m_command ) ) return;

	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() ) 
	{
		rowid		= m_recordset->GetFieldString( 0 );
		spouse1_id	= m_recordset->GetFieldString( 1 );
		spouse2_id	= m_recordset->GetFieldString( 2 );
		m_command.Format( L"SELECT sex_id FROM people WHERE rowid='%s'", spouse1_id );
		if( !query1( m_command ) ) return;

		if( m_recordset1->RecordsCount() )
		{
			sex_id = _wtoi( m_recordset1->GetFieldString( 0 ) );
			if( sex_id == WOMAN )
			{
				m_command.Format( L"UPDATE marriages SET spouse1_id = '%s', spouse2_id='%s' WHERE rowid = '%s'", spouse2_id, spouse1_id, rowid );
				if( !execute( m_command ) ) return;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::newDatabase( CString tag )
{
	CString databaseSpec;
	CString databaseName;

	splitFilespec( theApp.m_databaseSpec, &drive, &path,&filename,&ext );
	databaseName. Format( L"%s+%s.%s", filename, tag, ext ); 
	databaseSpec.Format( L"%s:%s\\%s", drive, path, databaseName );
	CopyFile( theApp.m_databaseSpec, databaseSpec, FALSE );

	theApp.m_databaseSpec = databaseSpec;
	theApp.m_databaseName = databaseName;

	theApp.WriteProfileStringW( L"Settings", L"database", theApp.m_databaseSpec );
	if( !theApp.openDatabase() ) return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::insertIntoFiles( CString fileSpec )
{
	CFileStatus status;
	CString mtime;
	CString ctime;
	CString atime;
	CString size;
	CString presentDate;

	if( CFile::GetStatus( fileSpec, status) )
	{
		mtime	= status.m_mtime.Format( L"%Y.%m.%d %H:%M:%S" );
		ctime	= status.m_ctime.Format( L"%Y.%m.%d %H:%M:%S" );
		atime	= status.m_atime.Format( L"%Y.%m.%d %H:%M:%S" );
		size	= thousend( status.m_size );
	}
	// a fájl azonosítóinak megõrzése
	presentDate = theApp.getPresentDateTime();

	m_command.Format( L"SELECT rowid FROM inputFiles WHERE filename ='%s'", fileSpec );
	if( !query( m_command ) )return;


	if( !m_recordset->RecordsCount() )
	{
		m_command.Format( L"INSERT INTO inputFiles ( filename, size, created, modified, loaded ) VALUES ( '%s', '%s', '%s', '%s', '%s' ) ", fileSpec, size, mtime, ctime, presentDate );
		if( !theApp.execute( m_command ) ) return;
		m_command = L"SELECT last_insert_rowid() FROM inputFiles";	
		if( !query( m_command ) ) return;
	}
	m_fileNumber = _wtoi( m_recordset->GetFieldString(0) );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDragonApp::selectFile(  CString fileSpec, BLOBSTAT* stat)
{
	if( fileSpec.IsEmpty() )
	{
//		CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_ALLOWMULTISELECT,
		CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"jpg files(*.jpg)|*.jpg|pdf files(*.pdf)|*.pdf|All Files (*.*)|*.*||" );
		if( dlg.DoModal( ) == IDCANCEL ) return false;
		POSITION pos = dlg.GetStartPosition( );
		fileSpec = dlg.GetNextPathName( pos );
	}

	CString str;
	CString ext(L"");
	int		ix;
	int rc;
	
	FILE* f;
	_wfopen_s( &f, fileSpec,  L"rb");
	if(NULL == f)
	{
		str.Format( L"Couldn't open file %s\n", fileSpec );
		AfxMessageBox( str );
		return false;
	}
	fseek(f, 0, SEEK_END);
	long filesize = ftell(f);
	fseek(f, 0, SEEK_SET);

	splitFilespec( fileSpec, &drive, &path, &filename, &ext );

	stat->filename	= filename;
	stat->fileSpec	= fileSpec;
	stat->size		= filesize; 
	stat->ext		= ext;
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDragonApp::selectFiles( )
{
	#define MAX_CFileDialog_FILE_COUNT 99
	#define FILE_LIST_BUFFER_SIZE ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1)

	CString fileName;
	wchar_t* p = fileName.GetBuffer( FILE_LIST_BUFFER_SIZE );

	BLOBSTAT stat;
	CString fileSpec;
	CString drive;
	CString dir;
	CString filename;
	CString ext;

	CString str;
	int		ix;
	int rc;
	int		nItem;
	CFileStatus status;

	vBlobs.clear();



	CFileDialog dlg(TRUE);

	OPENFILENAME& ofn = dlg.GetOFN( );
	ofn.Flags |= OFN_ALLOWMULTISELECT;
	ofn.lpstrFile = p;
	ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

	if( dlg.DoModal() == IDCANCEL ) return false;

	fileName.ReleaseBuffer();

	int fileCount = 0;
	POSITION pos = dlg.GetStartPosition( );
	while( pos )
	{
		++fileCount;
		fileSpec = dlg.GetNextPathName(pos);
		splitFilespec( fileSpec, &drive,&dir,&filename,&ext );
		CFile::GetStatus( fileSpec, status);

		stat.filename	= filename;
		stat.fileSpec	= fileSpec;
		stat.size		= status.m_size; 
		stat.ext		= ext;
		vBlobs.push_back( stat );
	}
	return true;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnEmail()
{
	CString str1("open"),str2("mailto:veges.istvan@gmail.com ? subject=Hi & cc=");
	ShellExecute(NULL, str1, str2, NULL , NULL, SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnPrivatFolyt()
{

	CString logFile( L"folyt" );
	CString title( L"%%% folyt sorok" );
	if(( fl=theApp.openLogFile( logFile,title ))==NULL)return;
	int		pos;

	CString cLine;


	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 

	CProgressWnd wndProgress(NULL,L"% folyt sorok a ga.html fájlban..." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, (int)file.GetLength() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	while(file.ReadString(cLine)) 
	{
		cLine.Trim();
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
		pos = (int)file.GetPosition();
		wndProgress.SetPos( pos );
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;

		if(  cLine.Find( L"%%% folyt" ) != -1 )
		{
			str = cleanCline2( cLine );
			fwprintf( theApp.fl, L"%s\n", str );
		}
	}
	file.Close();
	fclose( fl );
	showLogFile();	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A rowid+sex_id-vel megadott ember házastársait gyûjti össze a vektor-ban
// 
CString CDragonApp::getSpouses( CString rowid, CString sex_id, std::vector<SPOUSES>* vektor, SPOUSES2* sp )
{
	CString spouse_id;
	CString spouse;
	CString spouse2;
	CString names( L"" );
	CString names2(L"");
	SPOUSES nameid;
	CString firstName;
	CString lastName;
	CString source;
	int		pos;

	CString birthDate;
	CString deathDate;

	vektor->clear();
	if( sex_id == L"1" )
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE spouse1_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE spouse2_id = '%s'", rowid );
	if( !theApp.query1( m_command ) ) return L"" ;
	
	for( UINT i = 0; i < theApp.m_recordset1->RecordsCount(); ++i, theApp.m_recordset1->MoveNext() )
	{

		if( sex_id == L"1" )
			spouse_id = theApp.m_recordset1->GetFieldString( MARRIAGES_SPOUSE2_ID );
		else
			spouse_id = theApp.m_recordset1->GetFieldString( MARRIAGES_SPOUSE1_ID );
		m_command.Format( L"SELECT first_name, last_name, source, birth_date, death_date FROM people WHERE rowid='%s'", spouse_id );
		if( !theApp.query2( m_command )) return L"";
		
		firstName	= theApp.m_recordset2->GetFieldString( 0 );
		lastName	= theApp.m_recordset2->GetFieldString( 1 );
		source		= theApp.m_recordset2->GetFieldString( 2 );
		birthDate	= theApp.m_recordset2->GetFieldString( 3 );
		deathDate	= theApp.m_recordset2->GetFieldString( 4 );
		firstName = getWord( firstName, 1, &pos );

		spouse2.Format( L"%s %s %s", theApp.m_recordset2->GetFieldString( 2 ), theApp.m_recordset2->GetFieldString( 1 ), theApp.m_recordset2->GetFieldString( 0 ) );
//		spouse2.Format( L"%s %s %s", source, lastName, firstName );

//		spouse.Format( L"%s %s", theApp.m_recordset2->GetFieldString( 1 ), theApp.m_recordset2->GetFieldString( 0 ) );
		spouse.Format( L"%s %s", lastName, firstName );

		spouse.Replace( '|', '\'' );
		
		nameid.id	= spouse_id;
		nameid.name	= spouse;
		nameid.name2 = spouse2;	
		nameid.birth_date = birthDate;
		nameid.death_date = deathDate;
		vektor->push_back( nameid );
	}
	std::sort( vektor->begin(), vektor->end(), sortByName );

	names.Empty();
	for( UINT i = 0; i < vektor->size(); ++i )
	{
		names += vektor->at(i).name;
		names2 += vektor->at(i).name2;
		names += L", ";
		names2 += L", ";
	}
	sp->spouses		= names.Left( names.GetLength() - 2 );   
	sp->spouses2	= names2.Left( names2.GetLength() - 2 );   

//	if( sp->spouses == L"Sváby Károly" )
//		int z = 4;
	return sp->spouses;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnLanguages()
{
	CLanguages dlg;

	if( dlg.DoModal() == IDCANCEL ) return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnPlaces()
{
	CTablePlaces dlg;

	if( dlg.DoModal() == IDCANCEL ) return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::message( CString caption, CString str )
{
	CMessage dlg;
	dlg.caption	= caption;
	dlg.message	= str;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnUsermanual()
{
	if( _waccess( m_helpFileSpec, 0 ) )
	{
		AfxMessageBox( L"Nincs a DragonHelp.chm fájl a program könyvtárában!" );
		return;
	}
	else
	{
		ShellExecute(NULL, L"open", m_helpFileSpec,NULL, NULL, SW_SHOWNORMAL);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnTableFirstNames()
{
	CTableFirstnames dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::getPresentDate() 
{
	CString		dateTime;
	m_command=L"SELECT date('now','localtime')";
	if( !query( m_command ) ) return L""; 
	dateTime= m_recordset->GetFieldString(0);
	dateTime.Replace('-','.');
	return dateTime;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::getFamily( CString tableNumber )
{
	m_command.Format( L"SELECT familyName FROM  tables WHERE rowid='%s'", tableNumber );
	if( !query4( m_command ) ) return L"";
	return( m_recordset4->GetFieldString( 0 ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::getTable( CString tableNumber )
{
	m_command.Format( L"SELECT tableHeader FROM  tables WHERE rowid='%s'", tableNumber );
	if( !query4( m_command ) ) return L"";
	return( m_recordset4->GetFieldString( 0 ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::editHtmlLines( CListCtrlEx* p_ListCtrl, int col_linenumber )
{
	POSITION	pos = p_ListCtrl->GetFirstSelectedItemPosition();
	std::vector<CString> vLines;
	int nItem;
	int linenumber;
	while( pos )
	{
		nItem = p_ListCtrl->GetNextSelectedItem( pos );
		vLines.push_back( p_ListCtrl->GetItemText( nItem, col_linenumber ) );
	}
	if( vLines.size() == 1 )
	{
		linenumber = _wtoi( p_ListCtrl->GetItemText( nItem, col_linenumber ) );
		edit1line( linenumber );
	}
	else if( vLines.size() == 2 )
	{
		edit2lines( &vLines );
	}
	else
	{
		AfxMessageBox( L"1 vagy 2 sort kell kijelölni!" );
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::edit1line( int lineNumber )
{
	listHtmlLine( lineNumber );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::edit2lines( std::vector<CString>* vLines )
{
	CEditTwoLines dlg;
	dlg.vLines = vLines;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::change( CString linenumber, CString line )
{
	int ln = _wtoi( linenumber );
	ULONGLONG pos1;
	ULONGLONG pos2;

	CStdioFile file( m_htmlFileSpec, CFile::modeRead); 
	CString cLine;
	
	for( int i = 0; i < ln - 2; ++i ) file.ReadString( cLine );  // elõzõ sor elé megy

	file.ReadString( cLine );											// elõzõ sor
	pos1 = file.GetPosition();

	file.ReadString( cLine );											// kérdéses sor
	pos2 = file.GetPosition();
	file.Close();
	CString front;

	front = cLine.Left( cLine.Find( ';' ) + 1 );
	cLine = cLine.Mid( cLine.Find( ';' ) + 1 );
		
	CString linenew;
	linenew.Format( L"%s%s\r\n", front, line );

	USES_CONVERSION;
	char *pl = W2A((LPCTSTR)linenew);

	// create new html file with the modified line
	CString newFileSpec;
	newFileSpec.Format( L"%s\\%s_original.htm", m_htmlPath, m_htmlFileName );

	CString outputFileSpec;
	outputFileSpec.Format( L"%s\\modifiedFile.html", m_htmlPath );
	DeleteFile( outputFileSpec );

	CFile input( m_htmlFileSpec, CFile::modeRead | CFile::typeBinary );
	CFile output( outputFileSpec, CFile::modeCreate | CFile::modeWrite  | CFile::typeBinary );

	int		length = 1000000;
	TCHAR *buffer = new TCHAR[ length ];
	
	int numOfCycles;
	int	remains;

// I. rész másolása
	numOfCycles = int( pos1 / length );		// olvasás-írás ciklusok száma length hosszzal
	remains = int( pos1 % length );			// utolsó olvasás-írás	remains hosszal

	input.SeekToBegin();
	for( int i = 0; i < numOfCycles; ++i )
	{
		input.Read( buffer, length );
		output.Write( buffer, length );
	}
	if( remains )
	{
		input.Read( buffer, remains );
		output.Write( buffer, remains );
	}
// új sor beírása
	output.Write( pl, strlen( pl) );

// II. rész másolása
	ULONGLONG left = input.GetLength() - pos2;


	numOfCycles = int( left / length );		// olvasás-írás ciklusok száma length hosszzal
	remains = int( left % length );			// utolsó olvasás-írás	remains hosszal

	input.Seek( pos2, SEEK_SET );
	for( int i = 0; i < numOfCycles; ++i )
	{
		input.Read( buffer, length );
		output.Write( buffer, length );
	}
	if( remains )
	{
		input.Read( buffer, remains );
		output.Write( buffer, remains );
	}
	input.Close();
	output.Close();

		
	DeleteFile( newFileSpec );
	input.Rename( m_htmlFileSpec, newFileSpec );
	output.Rename( outputFileSpec, m_htmlFileSpec );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::listHtmlLine( int lineNumber )
{
	CHtmlEdit dlg;

	ULONGLONG pos1;
	ULONGLONG pos2;

	CStdioFile file( m_htmlFileSpec, CFile::modeRead); 
	CString cLine;
	
	for( int i = 0; i < lineNumber - 2; ++i ) file.ReadString( cLine );  // elõzõ sor elé megy

	file.ReadString( cLine );											// elõzõ sor
	pos1 = file.GetPosition();

	file.ReadString( cLine );											// kérdéses sor
	pos2 = file.GetPosition();
	file.Close();
	CString front;

	front = cLine.Left( cLine.Find( ';' ) + 1 );
	cLine = cLine.Mid( cLine.Find( ';' ) + 1 );
		
	dlg.m_line = cLine;
	dlg.m_lineNumber = lineNumber;
	if( dlg.DoModal() == IDCANCEL ) return;

	CString line;
	line.Format( L"%s%s\r\n", front, dlg.m_line );

	USES_CONVERSION;
	char *pl = W2A((LPCTSTR)line);

	// create new html file with the modified line
	CString newFileSpec;
	newFileSpec.Format( L"%s\\%s_original.htm", m_htmlPath, m_htmlFileName );

	CString outputFileSpec;
	outputFileSpec.Format( L"%s\\modifiedFile.html", m_htmlPath );
	DeleteFile( outputFileSpec );

	CFile input( m_htmlFileSpec, CFile::modeRead | CFile::typeBinary );
	CFile output( outputFileSpec, CFile::modeCreate | CFile::modeWrite  | CFile::typeBinary );

	int		length = 1000000;
	TCHAR *buffer = new TCHAR[ length ];
	
	int numOfCycles;
	int	remains;

// I. rész másolása
	numOfCycles = int( pos1 / length );		// olvasás-írás ciklusok száma length hosszzal
	remains = int( pos1 % length );			// utolsó olvasás-írás	remains hosszal

	input.SeekToBegin();
	for( int i = 0; i < numOfCycles; ++i )
	{
		input.Read( buffer, length );
		output.Write( buffer, length );
	}
	if( remains )
	{
		input.Read( buffer, remains );
		output.Write( buffer, remains );
	}
// új sor beírása
	output.Write( pl, strlen( pl) );

// II. rész másolása
	ULONGLONG left = input.GetLength() - pos2;


	numOfCycles = int( left / length );		// olvasás-írás ciklusok száma length hosszzal
	remains = int( left % length );			// utolsó olvasás-írás	remains hosszal

	input.Seek( pos2, SEEK_SET );
	for( int i = 0; i < numOfCycles; ++i )
	{
		input.Read( buffer, length );
		output.Write( buffer, length );
	}
	if( remains )
	{
		input.Read( buffer, remains );
		output.Write( buffer, remains );
	}
	input.Close();
	output.Close();

		
	DeleteFile( newFileSpec );
	input.Rename( m_htmlFileSpec, newFileSpec );
	output.Rename( outputFileSpec, m_htmlFileSpec );

//	theApp.message( L"Emberek listája", L"A html fájl módosítása megtörtént!" );
//	AfxMessageBox( L"A html fájl módisítása megtörtént!" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::saveHtmlLine( int lineNumber, CString line )
{
	ULONGLONG pos1;
	ULONGLONG pos2;

	CStdioFile file( m_htmlFileSpec, CFile::modeRead); 
	CString cLine;
	
	for( int i = 0; i < lineNumber - 2; ++i ) file.ReadString( cLine );  // elõzõ sor elé megy

	file.ReadString( cLine );											// elõzõ sor
	pos1 = file.GetPosition();

	file.ReadString( cLine );											// kérdéses sor
	pos2 = file.GetPosition();
	file.Close();
	CString front;

	front = cLine.Left( cLine.Find( ';' ) + 1 );
//	cLine = cLine.Mid( cLine.Find( ';' ) + 1 );

	CString linenew;
	linenew.Format( L"%s%s\r\n", front, line );

	USES_CONVERSION;
	char *pl = W2A((LPCTSTR)linenew);

	// create new html file with the modified line
	CString newFileSpec;
	newFileSpec.Format( L"%s\\%s_original.htm", m_htmlPath, m_htmlFileName );

	CString outputFileSpec;
	outputFileSpec.Format( L"%s\\modifiedFile.html", m_htmlPath );
	DeleteFile( outputFileSpec );

	CFile input( m_htmlFileSpec, CFile::modeRead | CFile::typeBinary );
	CFile output( outputFileSpec, CFile::modeCreate | CFile::modeWrite  | CFile::typeBinary );

	int		length = 1000000;
	TCHAR *buffer = new TCHAR[ length ];
	
	int numOfCycles;
	int	remains;

// I. rész másolása
	numOfCycles = int( pos1 / length );		// olvasás-írás ciklusok száma length hosszzal
	remains = int( pos1 % length );			// utolsó olvasás-írás	remains hosszal

	input.SeekToBegin();
	for( int i = 0; i < numOfCycles; ++i )
	{
		input.Read( buffer, length );
		output.Write( buffer, length );
	}
	if( remains )
	{
		input.Read( buffer, remains );
		output.Write( buffer, remains );
	}
// új sor beírása
	output.Write( pl, strlen( pl) );

// II. rész másolása
	ULONGLONG left = input.GetLength() - pos2;


	numOfCycles = int( left / length );		// olvasás-írás ciklusok száma length hosszzal
	remains = int( left % length );			// utolsó olvasás-írás	remains hosszal

	input.Seek( pos2, SEEK_SET );
	for( int i = 0; i < numOfCycles; ++i )
	{
		input.Read( buffer, length );
		output.Write( buffer, length );
	}
	if( remains )
	{
		input.Read( buffer, remains );
		output.Write( buffer, remains );
	}
	input.Close();
	output.Close();

		
	DeleteFile( newFileSpec );
	input.Rename( m_htmlFileSpec, newFileSpec );
	output.Rename( outputFileSpec, m_htmlFileSpec );

//	theApp.message( L"Emberek listája", L"A html fájl módosítása megtörtént!" );
//	AfxMessageBox( L"A html fájl módisítása megtörtént!" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::selectAll( CListCtrlEx * p_ListCtrl )
{
	for( int i=0; i < p_ListCtrl->GetItemCount(); ++i)
	{
		p_ListCtrl->SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::unselectAll( CListCtrlEx * p_ListCtrl )
{
	for( int i=0; i < p_ListCtrl->GetItemCount(); ++i)
	{
		p_ListCtrl->SetItemState(i, 0, LVIS_SELECTED|LVIS_FOCUSED);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::search( CString search, INT_PTR orderix,  CListCtrlEx* p_ListCtrl )
{
	int		n				= p_ListCtrl->GetItemCount();
	int		count_per_page	= p_ListCtrl->GetCountPerPage();
	int		length			= search.GetLength();
	int		nItem;
	int		up;
	CString	substring;
	CString	str;

	unselectAll( p_ListCtrl );
	for( nItem = 0; nItem < n; ++nItem )
	{
		str = p_ListCtrl->GetItemText( nItem, orderix);
		substring = str.Left(length);						// az aktuális search string hosszával azonos hosszúság leválasztása
		if( substring == search )
		{
			up = nItem + count_per_page - 1;
			if( up >= n ) up = n-1;

			p_ListCtrl->SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
//			p_ListCtrl->SetSelectionMark( nItem );
			p_ListCtrl->EnsureVisible( up, FALSE );
			break;
		}
	}
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::getHtmlLine( CString lineNumber )
{
	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
	
	CString cLine;
	int ln = _wtoi( lineNumber );

	for( int i = 0; i < ln - 2; ++i ) file.ReadString( cLine );  // elõzõ sor elé megy
	file.ReadString( cLine );											// elõzõ sor

	file.ReadString( cLine );											// kérdéses sor
	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
	
	cLine = cleanHtmlLine( cLine );
	file.Close();
	return cLine;
}