#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "dragon.h"
#include "MainFrm.h"
#include "about.h"
#include "dragonDoc.h"
#include "dragonView.h"
#include "utilities.h"
#include "locale.h"
#include "OpenDatabase.h"
#include "version.h"
#include "Table_people.h"
#include <algorithm>
#include "GetString.h"
#include "Languages.h"
#include "Table_firstnames.h"
#include "Message.h"
#include "editHtmlLine.h"
#include "SaveFirstName.h"
#include "ProgressWnd.h"
#include "editHtmlLines.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortByName(const SPOUSES &v1, const SPOUSES &v2) 
{ 
	if( ! v1.rowid.Compare( v2.rowid ) )
		return( v1.name < v2.name  );
	return( v1.name < v2.name ); 
}
bool sortByName1(const CString &v1, const CString &v2);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::openTextFile( FILE ** fl, CString fileName, CString mode )
{
	CString fileSpec;
	fileSpec.Format(L"%s\\%s_%s.text",m_workingDirectory,fileName, getTimeTag() );
	if( !openFileSpec( fl, fileSpec, mode ) ) return NULL;
	return fileSpec;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::openHtmlFile( FILE** fh,  CString fileName, CString mode )
{
	CString fileSpec;
	fileSpec.Format(L"%s\\%s_%s.html",theApp.m_dbFolderPath,fileName, getTimeTag() );
	if( !openFileSpec( fh, fileSpec, mode ) ) return( L"" );

	m_startTime = getStartTime();
	return fileSpec;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FILE * CDragApp::openLogFile(CString fname,CString title)
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

		str.Format(L"Dragon v. %s",m_version );

		n = str.GetLength()/2;	
		n	= 40 + str.GetLength()/2;
		fwprintf(fl,L"%*s\n",n,str);
	
		str.Format( L"Adatbázis: %s", m_dbPathName );
		n = str.GetLength()/2;	
		n	= 40 + str.GetLength()/2;
		fwprintf(fl,L"%*s\n",n,str);
	}


	fwprintf( fl, L"\n\n" );
	m_startTime = getStartTime();
	return fl;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::showLogFile()
{
	CString fileSpec;
	fileSpec.Format( L"\"%s\"", m_listFile );
	fileSpec = m_listFile;
	if( _waccess( fileSpec, 0 ) )
	{
		str.Format( L"%s file nem létezik!", fileSpec );
		AfxMessageBox( str, MB_ICONEXCLAMATION );
		return;
	}
	ShellExecute(NULL,L"open",m_editorFileName, fileSpec, m_editorFolderPath,SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::showFile(CString file )
{
	if( _waccess( file, 0 ) )
	{
		str.Format( L"%s\nfile nem létezik!", file );
		AfxMessageBox( str, MB_ICONEXCLAMATION );
		return;
	}
	CString	fileSpec;
	fileSpec.Format(L"\"%s\"", file );
	ShellExecute(NULL,L"open",m_editorFileName, fileSpec, m_editorFolderPath,SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::showHtmlFile(CString file )
{

	CString	fileSpec;
	fileSpec.Format(L"\"%s\"", file );
	if( _waccess( file, 0 ) )
	{
		str.Format( L"%s file nem létezik!", fileSpec );
		AfxMessageBox( str, MB_ICONEXCLAMATION );
		return;
	}
		ShellExecute(NULL, L"open", fileSpec,NULL, NULL, SW_SHOWNORMAL);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::editNotepad( CString file, CString lineNumber )
{
	if( _waccess( file, 0 ) )
	{
		str.Format( L"%s file nem létezik!", file );
		AfxMessageBox( str, MB_ICONEXCLAMATION );
		return;
	}
	CString	fileSpec;
	fileSpec.Format( L"\"%s\" -n%s -alwaysOnTop -nosession", file, lineNumber );
	fileSpec.Format(L"%s -n%s -alwaysOnTop -nosession", file, lineNumber);
	ShellExecute(NULL,L"open",m_editorFileName, fileSpec, m_editorFolderPath,SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::notepad( CString file, CString option )
{
	if( _waccess( file, 0 ) )
	{
		str.Format( L"%s file nem létezik!", file );
		AfxMessageBox( str, MB_ICONEXCLAMATION );
		return;
	}
	CString	fileSpec;
	fileSpec.Format( L"\"%s\" %s",  file, option );
	ShellExecute(NULL,L"open",m_editorFileName, fileSpec, m_editorFolderPath,SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getPresentDateTime() 
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
_int64 CDragApp::getDateIFromStr( CString dateS ) 
{
	_int64 dateI = 0;
	CString datum;


	if( !dateS.IsEmpty() )
	{
		dateS.Replace('.','-');
		if( dateS.GetLength() == 4 )
			dateS += L"-01-01";

		datum.Format( L"%s 00:00:00.000", dateS );
		m_command.Format(L"SELECT strftime('%%s', '%s','utc')",dateS);  // pontos datum-idõ kell, hogy 1970.01.01 -et == 0-ra 
		if( !query2( m_command ) ) return dateI;
		dateI = _wtoi64( m_recordset2->GetFieldString(0) );
	}
	return dateI;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString	CDragApp::getDateStrFromI( CString dateI ) 
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
_int64 CDragApp::dateDiff( CString date1, CString date2 )
{
	_int64 d1;
	_int64 d2;
	float	diffD;
	int		diffI;
	
	if( date1 == L"1802-07-05" || date2 == L"1802-07-05" )
		diffI = 0;
	date1.Replace( '.', '-' );
	date2.Replace( '.', '-' );
	m_command.Format( L"SELECT julianday( '%s')", date1 );
	if( !query4( m_command ) ) return 0;
	d1 = _wtoi64( m_recordset4->GetFieldString( 0 ) );

	m_command.Format( L"SELECT julianday( '%s')", date2 );
	if( !query4( m_command ) ) return 0;
	d2 = _wtoi64( m_recordset4->GetFieldString( 0 ) );

	diffD = (float)(d1 - d2)/365 + 0,5;

	diffI = (int)diffD;

	return ( diffI );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getYear( _int64 diff )
{
	m_command.Format( L"SELECT strftime( '%s', '%d', 'unixepoch' )", L"%Y.%m.%d",  diff );
	if( !query( m_command ) ) return 0;
	return( m_recordset->GetFieldString(0) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
_int64 CDragApp::getStartTime()
{
	m_command = L"SELECT strftime('%s','now' )";
	if( !query4( m_command ) ) return 0;
	return ( _wtoi( m_recordset4->GetFieldString( 0 ) ) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getTimeElapsed( _int64 startTime )
{
	int endTime;

	m_command = L"SELECT strftime('%s','now' )";
	if (!query(m_command)) return L"";
	endTime = _wtoi(m_recordset->GetFieldString(0));

	CString format = L"%H:%M:%S";
	m_command.Format(L"SELECT strftime( '%s', '%d', 'unixepoch')", format, endTime - startTime );
	if (!query(m_command)) return L"";
	return(m_recordset->GetFieldString(0));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ellenõrtuzi, hogy a megadott 2 dátum között van-e 9 hónap különbség.
// return TRUE: ha 9 hónapnál nagyobb
//        FALSE: ha 9 hónapnál kisebb
BOOL CDragApp::dateDiff(  CString date1, CString date2, int month  )
{
	int		pos;

	if( !checkDate( date1 ) || !checkDate( date2 )  ) return false;
	if( date1.GetLength() < 10 || date2.GetLength() < 10 || (pos=date1.Find('?')) != -1 || (pos=date2.Find('?')) != -1 ) return FALSE;

	_int64 time1 = getDateI( date1, month );
	_int64 time2 = getDateI( date2, 0 );

	if( time1 < time2 ) return true;			// apa halála + 9 hónap kisebb mint a gyerek születése
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
_int64 CDragApp::getDateI( CString dateS, int month ) 
{
	_int64 dateI = 0;
	CString modifier;

	if( !dateS.IsEmpty() )
	{
		dateS.Replace('.','-');
		modifier.Format( L"+%d month", month );
		m_command.Format(L"SELECT strftime('%%s', '%s 00:00:00.000', '%s', 'utc' )",dateS, modifier);  // pontos datum-idõ kell, hogy 1970.01.01 -et == 0-ra 
		if( !query2( m_command ) ) return dateI;
		dateI = _wtoi64( m_recordset2->GetFieldString(0) );
	}
	return dateI;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CStringA CDragApp::MultiToUtf8( LPCSTR multi )
{
	CStringA utf8;
	utf8 = UnicodeToUtf8(MultiToUnicode( multi ));
	return utf8;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getLanguage( int language_id )
{
	m_command.Format( L"SELECT language FROM languages WHERE rowid = '%d'", language_id );
	if( !querySystem( m_command ) ) return L"";
	return( m_recordsetSystem->GetFieldString( 0 ) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CDragApp::getUserVersion()
{
	m_command.Format( L"PRAGMA %s", L"user_version" );
	if( !query( m_command ) ) return 0;
	CString userversion;
	userversion = m_recordset->GetFieldString(0);
	m_userVersion = _wtoi( userversion);
	return( m_userVersion );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::setUserVersion( int user_version )
{
	m_command.Format(L"PRAGMA user_version=%d", user_version );
	if( !theApp.execute( m_command ) ) return;
}
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::saveInfo( CString rowid, CString rowidBy )
{
	m_command.Format( L"SELECT rowid, * FROM people WHERE rowid == '%s'", rowid );
	if( !query1( m_command ) ) return;
	m_command.Format( L"SELECT rowid, * FROM people WHERE rowid == '%s'", rowidBy );
	if( !query2( m_command ) ) return;

	CString titolo		= m_recordset1->GetFieldString( DBP_TITOLO );
	CString titoloBy	= m_recordset2->GetFieldString( DBP_TITOLO );
	if( !titolo.IsEmpty() && titoloBy.IsEmpty() )
	{
		m_command.Format( L"UPDATE people SET titolo='%s' WHERE rowid ='%s'", titolo, rowidBy );
		if( !theApp.execute( m_command ) ) return;
	}
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::contractions()
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
// A _,-,? és ) karaktereket leszedi, a maradékot szétszedi.
// return:
// Empty	nem ismmert
// MAN		férfi keresztnév
// WOMAN	nõi keresztnév
// BISEX	bisex
CString CDragApp::isFirstName( CString name )
{
	name.Replace( ',', ' ');
	name.Replace( '-', ' ' );
	name.Replace( '_', ' ' );
	name.Replace( '?', ' ' );
	name.Replace( ')', ' ' );
	name.Replace( '(', ' ' );		// Az (Ede) is keresztnév legyen
	name.Trim();
	if( name.IsEmpty() ) return L"";
	

	std::vector<CString> vSexId;

	CStringArray A;
	CString sex_id;
	int i;
	int n = splitCString( name, ' ', false, &A );

// többszörös keresztnevek minden tagját megvizsgálja
	for( i = 0; i < n; ++i )
	{
		m_command.Format( L"SELECT sex_id FROM firstnames WHERE first_name='%s'", A[i] );
		if( !querySystem( m_command ) )	return L"";
		sex_id = m_recordsetSystem->GetFieldString( 0 );
		vSexId.push_back( sex_id ); 
 	}
	if (vSexId.size() > 1 )
	{
		for (i = 0; i < vSexId.size() - 1; ++i)
		{
			if (vSexId.at(i) != vSexId.at(i + 1))			// a két sex_id nem azonos
			{
				if (vSexId.at(i).IsEmpty() )
					return vSexId.at(i + 1);	// ha az elsõ 0 és a második nemnulla, akkor a második jó
				else
					return vSexId.at(i);		// ha a második 0 és az elsõ nemnulla, akkor az elsõ jó
			}
		}
	}
	return vSexId.at(0);									// minden sexid azonos
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CDragApp::getNumberOfDb( std::vector<CString>* vE )
{
	int pos;
	splitFilespec( m_dbPathName, &drive, &path, &filename, &ext );

	CString baseName;
	CString fileName;
	if( ( pos = theApp.m_dbFileName.ReverseFind( '.' ) ) != -1 )
	{
		baseName = theApp.m_dbFileName.Left( pos );
	}
	if( ( pos = m_dbFileName.Find( '+' ) ) != -1 )
	{
		baseName = m_dbFileName.Left( pos );
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
	
		fileSpec.Format( L"%s\\%s", theApp.m_dbFolderPath, fileName );
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
void CDragApp::OnHusbandWife()
{
	CString rowid;
	CString husband_id;
	CString wife_id;
	CString sex_id;

	if( AfxMessageBox( L"Férj legyen husband_id, feleség pedig wife_id.\nHa szükséges cserélje fel.", MB_OKCANCEL ) == IDCANCEL ) return;

	m_command.Format( L"SELECT rowid, husband_id, wife_id FROM marriages" );
	if( !query( m_command ) ) return;

	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() ) 
	{
		rowid		= m_recordset->GetFieldString( 0 );
		husband_id	= m_recordset->GetFieldString( 1 );
		wife_id	= m_recordset->GetFieldString( 2 );
		m_command.Format( L"SELECT sex_id FROM people WHERE rowid='%s'", husband_id );
		if( !query1( m_command ) ) return;

		if( m_recordset1->RecordsCount() )
		{
			sex_id = m_recordset1->GetFieldString( 0 );
			if( sex_id == WOMAN )
			{
				m_command.Format( L"UPDATE marriages SET husband_id = '%s', wife_id='%s' WHERE rowid = '%s'", wife_id, husband_id, rowid );
				if( !execute( m_command ) ) return;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDragApp::insertIntoFiles( CString filename, int type )
{
	CFileStatus status;
	CString mtime;
	CString ctime;
	CString atime;
	CString size;
	CString presentDate;
	CString filespec;
	filespec.Format(L"%s\\%s", theApp.m_dbFolderPath, filename);
	if( CFile::GetStatus( filespec, status) )
	{
		mtime	= status.m_mtime.Format( L"%Y.%m.%d %H:%M:%S" );
		ctime	= status.m_ctime.Format( L"%Y.%m.%d %H:%M:%S" );
		atime	= status.m_atime.Format( L"%Y.%m.%d %H:%M:%S" );
		size	= thousand( status.m_size );
	}
	// a fájl azonosítóinak megõrzése
	presentDate = theApp.getPresentDateTime();

	m_command.Format( L"INSERT INTO filespec ( type, filespec, created, modified, loaded ) VALUES ( '%d', '%s', '%s', '%s', '%s' ) ", type, filename, mtime, ctime, presentDate );
	if( !theApp.execute( m_command ) ) return false;


	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getFileCreationDate( CString fileSpec)
{
	CFileStatus status;
	CString mtime;
	if (CFile::GetStatus(fileSpec, status))
	{
		mtime = status.m_mtime.Format(L"%Y.%m.%d %H:%M:%S");
	}
	return mtime;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDragApp::selectFile(  CString fileSpec, BLOBSTAT* stat)
{
	if( fileSpec.IsEmpty() )
	{
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
bool CDragApp::selectFiles( )
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragApp::OnPrivatFolyt()
//{
//
//	CString logFile( L"folyt" );
//	CString title( L"%%% folyt sorok" );
//	if(( fl=theApp.openLogFile( logFile,title ))==NULL)return;
//	int		pos;
//
//	CString cLine;
//	CString folyt;
//	CString tableRoman;
//
//	theApp.m_inputCode = GetInputCode( theApp.m_htmlPathName );
//	CStdioFile file( theApp.m_htmlPathName, CFile::modeRead); 
//
//	CProgressWnd wndProgress(NULL,L"% folyt sorok a ga.html fájlban..." ); 
//	wndProgress.GoModal();
//	wndProgress.SetRange(0, (int)file.GetLength() );
//	wndProgress.SetPos(0);
//	wndProgress.SetStep(1);
//
//	while(file.ReadString(cLine)) 
//	{
//		cLine.Trim();
//		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
//		pos = (int)file.GetPosition();
//		wndProgress.SetPos( pos );
//		wndProgress.PeekAndPump();
//		if (wndProgress.Cancelled()) break;
//
//		if( ( pos = cLine.Find( L"%%% folyt" )) != -1 )
//		{
//			folyt = cLine.Mid( pos );
//			str = cLine.Mid( pos + 10 );
//			tableRoman = getFirstWord( str );
////			str = cleanCline2( cLine );
//			str = cleanCline( cLine );
//			//fwprintf( theApp.fl, L"%s\n", str );
//			fwprintf( theApp.fl, L"%-30s %s\n", folyt.Left(30), tableRoman );
//		}
//	}
//	file.Close();
//	fclose( fl );
//	showLogFile();	
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A rowid+sex_id-vel megadott ember házastársait gyûjti össze a vektor-ban
// 
CString CDragApp::getSpouses( CString rowid, CString sex_id, std::vector<SPOUSES>* vektor, SPOUSES2* sp )
{
	CString spouse_id;
	CString spouse;
	CString spouse2;
	CString names( L"" );
	CString names2(L"");
	CString wedding;
	
	SPOUSES nameid;
	CString firstName;
	CString lastName;
	CString source;
	int		pos;

	CString birthDate;
	CString deathDate;

	vektor->clear();
	if( sex_id == MAN )
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE husband_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE wife_id = '%s'", rowid );
	if( !theApp.query1( m_command ) ) return L"" ;
	
	for( UINT i = 0; i < theApp.m_recordset1->RecordsCount(); ++i, theApp.m_recordset1->MoveNext() )
	{
		wedding = theApp.m_recordset1->GetFieldString( DBM_DATE );

		if( sex_id == MAN )
			spouse_id = theApp.m_recordset1->GetFieldString( DBM_WIFE_ID );
		else
			spouse_id = theApp.m_recordset1->GetFieldString( DBM_HUSBAND_ID );
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
		nameid.wedding = wedding;
		nameid.birth = birthDate;
		nameid.death = deathDate;
		vektor->push_back( nameid );
	}
	std::sort( vektor->begin(), vektor->end(), sortByName );

	names.Empty();
	for( UINT i = 0; i < vektor->size(); ++i )
	{
		names += vektor->at(i).name;
		names2 += getNameBD( vektor->at(i).name, vektor->at(i).birth, vektor->at(i).death, vektor->at(i).wedding );
		names += L", ";
		names2 += L",";
	}
/*
	for( UINT i = 0; i < vektor->size(); ++i )
	{
		names += vektor->at(i).name;
		names2 += vektor->at(i).name2;
		names += L", ";
		names2 += L", ";
	}
*/
	sp->spouses		= names.Left( names.GetLength() - 2 );   
	sp->spouses2	= names2.Left( names2.GetLength() - 1 );   

//	if( sp->spouses == L"Sváby Károly" )
//		int z = 4;
	return sp->spouses;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::message( CString caption, CString str )
{
	CMessage dlg;
	dlg.caption	= caption;
	dlg.message	= str;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUsermanual()
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
void CDragApp::OnTableFirstNames()
{
	CTableFirstnames dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getPresentDate() 
{
	CString		dateTime;
	m_command=L"SELECT date('now','localtime')";
	if( !query( m_command ) ) return L""; 
	dateTime= m_recordset->GetFieldString(0);
	dateTime.Replace('-','.');
	return dateTime;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getFamily( CString tableNumber )
{
	m_command.Format( L"SELECT familyName FROM  tables WHERE rowid='%s'", tableNumber );
	if( !query4( m_command ) ) return L"";
	return( m_recordset4->GetFieldString( 0 ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getTable( CString tableNumber )
{
	m_command.Format( L"SELECT tableHeader FROM  tables WHERE rowid='%s'", tableNumber );
	if( !query4( m_command ) ) return L"";
	return( m_recordset4->GetFieldString( 0 ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::change( CString linenumber, CString line )
{
	int ln = _wtoi( linenumber );
	ULONGLONG pos1;
	ULONGLONG pos2;

	CStdioFile file( m_htmlPathName, CFile::modeRead); 
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
	newFileSpec.Format( L"%s\\%s_original.htm", m_dbFolderPath, m_htmlFileName );

	CString outputFileSpec;
	outputFileSpec.Format( L"%s\\modifiedFile.html", m_dbFolderPath );
	DeleteFile( outputFileSpec );

	CFile input( m_htmlPathName, CFile::modeRead | CFile::typeBinary );
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
	input.Rename( m_htmlPathName, newFileSpec );
	output.Rename( outputFileSpec, m_htmlPathName );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::listHtmlLine( int lineNumber )
{
	CEditHtmlLines dlg;
	dlg.m_linenumber = lineNumber;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::saveHtmlLine( int lineNumber, CString line )
{
	ULONGLONG pos1;
	ULONGLONG pos2;

	CStdioFile file( m_htmlPathName, CFile::modeRead); 
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
	newFileSpec.Format( L"%s\\%s_original.htm", m_dbFolderPath, m_htmlFileName );

	CString outputFileSpec;
	outputFileSpec.Format( L"%s\\modifiedFile.html", m_dbFolderPath );
	DeleteFile( outputFileSpec );

	CFile input( m_htmlPathName, CFile::modeRead | CFile::typeBinary );
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
	input.Rename( m_htmlPathName, newFileSpec );
	output.Rename( outputFileSpec, m_htmlPathName );

//	theApp.message( L"Emberek listája", L"A html fájl módosítása megtörtént!" );
//	AfxMessageBox( L"A html fájl módisítása megtörtént!" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::selectAll( CListCtrlEx * p_ListCtrl )
{
	for( int i=0; i < p_ListCtrl->GetItemCount(); ++i)
	{
		p_ListCtrl->SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::unselectAll( CListCtrlEx * p_ListCtrl )
{
	for( int i=0; i < p_ListCtrl->GetItemCount(); ++i)
	{
		p_ListCtrl->SetItemState(i, 0, LVIS_SELECTED|LVIS_FOCUSED);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getHtmlLine( CString lineNumber )
{
	theApp.m_inputCode = GetInputCode( theApp.m_htmlPathName );
	CStdioFile file( theApp.m_htmlPathName, CFile::modeRead); 
	
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::htmlEditLines( CListCtrlEx* p_ListCtrl, int col_linenumber, CString title )
{
	POSITION	pos = p_ListCtrl->GetFirstSelectedItemPosition();
	std::vector<CString> vLines;
	int nItem;
	CString linenumber;


	while( pos )
	{
		nItem = p_ListCtrl->GetNextSelectedItem( pos );
		vLines.push_back( p_ListCtrl->GetItemText( nItem, col_linenumber ) );
	}
	if( vLines.size() == 1 )
	{
		linenumber = p_ListCtrl->GetItemText( nItem, col_linenumber );
		CEditHtmlLine dlg1;
		dlg1.m_title = title;
		dlg1.m_linenumber = linenumber;
		dlg1.DoModal();
	}
	else 
	{
		CEditHtmlLines dlg;
		dlg.m_title = title;
		dlg.vLines = &vLines;
		dlg.DoModal();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::HtmlNotepadParents( CString rowid )
{
	CString linenumber;
	CString father_id;
	m_command.Format( L"SELECT father_id FROM people WHERE rowid='%s'", rowid );
	if( !query( m_command ) ) return;
	if( !m_recordset->RecordsCount() )
	{
		str.Format(L"%s rowid nincs az adatbázisban!", rowid);
		AfxMessageBox( str );
		return;
	}
	father_id = m_recordset->GetFieldString( 0 );
	if( father_id.IsEmpty() || father_id == L"0" )
	{
		AfxMessageBox( L"Nincs apja!" );
		return;
	}

	m_command.Format( L"SELECT linenumber FROM people WHERE rowid='%s'", father_id );
	if( !query( m_command ) ) return;
	linenumber = m_recordset->GetFieldString( 0 );
	editNotepad( theApp.m_htmlPathName, linenumber );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::keress( CString search, CListCtrlEx* p_ListCtrl, int column, int start, bool exact )
{
	if( search.IsEmpty() ) return;
	int sel = p_ListCtrl->GetNextItem(-1, LVNI_SELECTED);

	int	itemCnt	= p_ListCtrl->GetItemCount();
	CProgressWnd wndProgress(NULL, L"Folyik a keresés.." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, itemCnt );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	
	int		length	= search.GetLength();
	int		nItem;
	int		topIndex = p_ListCtrl->GetTopIndex();
	CString	str;
	int pos;

	theApp.unselectAll( p_ListCtrl );

	if (start < 0)
	{
		for (nItem = sel -1; nItem >= 0; --nItem)
		{
			str = p_ListCtrl->GetItemText(nItem, column);
//			str = str.Left(length);	// az aktuális search string hosszával azonos hosszúság leválasztása
			if( exact )
			{
				if (str == search)
				{
					break;
				}
			}
			else
			{
				if ((pos = str.Find(search)) != -1) break;
			}


			wndProgress.StepIt();
			wndProgress.PeekAndPump();
			if (wndProgress.Cancelled()) break;
		}
	}
	else
	{
		for (nItem = start; nItem < itemCnt - 1; ++nItem)
		{
			str = p_ListCtrl->GetItemText(nItem, column);
//			str = str.Left(length);						// az aktuális search string hosszával azonos hosszúság leválasztása
			if (exact)
			{
				if (str == search)
				{
					break;
				}
			}
			else
			{
				if ((pos = str.Find(search)) != -1) break;
			}
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
			if (wndProgress.Cancelled()) break;
		}
	}
	wndProgress.DestroyWindow();
	if( 0 <= nItem && nItem < itemCnt -1 )			// megtalálta a keresett embert,. aki az nItem-1 sorban van
	{
		showItem( nItem, p_ListCtrl );
	}
	else
	{
		HDITEM hditem = { 0 };
		TCHAR  headertxt[100];
		hditem.pszText = headertxt;
		hditem.mask = HDI_TEXT;
		hditem.cchTextMax = 100;
		VERIFY(p_ListCtrl->GetHeaderCtrl()->GetItem(column, &hditem));

		str.Format( L"\"%s\" stringet nem találtam az '%s' oszlopban!", search, headertxt );
		AfxMessageBox( str );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A p_ListCtrl-al megadott list control nItem-edik  sorát képbe hozza és kijelölöi
void CDragApp::showItem( int nItem, CListCtrlEx* p_ListCtrl )
{
	int	itemCount	= p_ListCtrl->GetItemCount();	// összesen ennyi sor van
	int topIndex	= p_ListCtrl->GetTopIndex();	// jelenleg ez van az ablak tetjén
	int countPP		= p_ListCtrl->GetCountPerPage();// oldalanként ennyi sor van
	int nVisible	= nItem;						// default láttatás

	if( nItem > topIndex )							// felhúzza a tábla tetejére, vagy ha már nincs annyi, akkor  a megtalált sort
	{
		if( (nItem + countPP) < itemCount ) 
			nVisible = nItem + countPP - 1;
		else
			nVisible = itemCount - 1;
	}																	
	p_ListCtrl->EnsureVisible( nVisible, FALSE );
	p_ListCtrl->SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::selectPeople( CString rowid, CString* linenumber )
{
	CString sexid;
	CString lastname;
	CString firstname;
	CString birthplace;
	CString birthdate;
	CString deathplace;
	CString deathdate;
	CString profession;
	CString comment;
	CString father_id;
	CString mother_id;

	CString birth;
	CString death;
	
	CString columns = L"rowid, linenumber, sex_id, last_name, first_name, birth_place, birth_date, death_place, death_date, occupation, comment, father_id, mother_id";

	m_command.Format( L"SELECT %s FROM people WHERE rowid = '%s'", columns, rowid );
	if( !query4( m_command ) ) return L"";
	*linenumber	= m_recordset4->GetFieldString( LINENUMBER );
	sexid		= m_recordset4->GetFieldString( SEXID );
	lastname	= m_recordset4->GetFieldString( LASTNAME );
	firstname	= m_recordset4->GetFieldString( FIRSTNAME );
	birthplace	= m_recordset4->GetFieldString( BIRTHPLACE );
	birthdate	= m_recordset4->GetFieldString( BIRTHDATE );
	deathplace	= m_recordset4->GetFieldString( DEATHPLACE );
	deathdate	= m_recordset4->GetFieldString( DEATHDATE );
	profession	= m_recordset4->GetFieldString( PROFESSION );
	comment		= m_recordset4->GetFieldString( COMMENT );
	father_id	= m_recordset4->GetFieldString( FATHERID );
	mother_id	= m_recordset4->GetFieldString( MOTHERID );

	birth = pack( L"*", birthplace, birthdate );
	death = pack( L"+", deathplace, deathdate );
	
	str.Format( L"%s %s", lastname, firstname );
	str.Format( L"%s %s", (CString)str, birth );
	str.TrimRight() ;
	str.Format( L"%s %s", (CString)str, death );
	str.TrimRight();
	str.Format( L"%s %s", (CString)str, comment ); 
	str.TrimRight();
	str.Format( L"%s %s", (CString)str, profession ); 
	str.TrimRight();

	return  str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDragApp::gedFileExists()
{
	if( _waccess( m_gedPathName, 0 ) )
	{
		str.Format( L"gedFileExists\n%s\nfájl nem létezik!", m_gedPathName );
		AfxMessageBox( str, MB_ICONEXCLAMATION );
		return false;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragApp::getFileSpec( int type )
{
	CString filespec;
	CString created;
	CString warning;
	m_command.Format(L"SELECT filespec, created  FROM filespec WHERE type = '%d'", type);
	if (!query(m_command)) return(L"");
	
	m_dbFileExists = false;
	if (m_recordset->RecordsCount() )
	{
		filespec.Format(L"%s\\%s", m_dbFolderPath, m_recordset->GetFieldString(0));
		created = m_recordset->GetFieldString(1);
	
		if (_waccess(filespec, 0))   // a nyilvántartott html fájl nem létezik!!
		{
			warning.Format( L"Az adatbázisban nyilvántartott\n%s\nfájl nem létezik!\nA program azért mûködik, de a fájlt kezelõ funkciók nem!", filespec );
			AfxMessageBox( warning, MB_ICONASTERISK);
			return filespec;
		}

		if( type == HTML_FILE || type == GEDCOM_FILE )
		{
			CFileStatus status;
			CString ctime;
			if (CFile::GetStatus(filespec, status))
			{
				ctime = status.m_mtime.Format(L"%Y.%m.%d %H:%M:%S");
				if (ctime != created)
				{
					warning = L"A program azért mûködik, de az adatbázis és a bemeneti fájl nem felel meg egymásnak!";
					str.Format(L"%s\nfájl módosításának\nnyilvántartott dátuma:\t%s\nmegállapított dátuma:\t%s\n\n%s", filespec, created, ctime, warning);
					AfxMessageBox(str, MB_ICONASTERISK);
				}
			}
			m_dbFileExists = true;
		}
	}
	return filespec;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CString CDragApp::UTF16toUTF8( CString str)
//{

//	CString data = File.ReadAllBytes(filePath);
//	CString text = Encoding.UTF8.GetString(data);

//	data = Encoding.Unicode.GetBytes(text);
//	File.WriteAllBytes(filePath, data);
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateConv16to8(CCmdUI* pCmdUI)
{

}
void CDragApp::OnConv16to8()
{

	DeleteFileW(m_systemDatabaseSpec8);
	DeleteFileW(m_systemDatabaseSpec16);
	COpenDatabase open;

	open.m_dbPathName = m_systemDatabaseSpec8;
	open.m_connDB = systemDB8;
	open.m_databaseTables = systemTables;
	open.m_databaseIndexes = systemIndexes;
	open.m_numberOfTables = numberOfSystemTables;
	open.m_numberOfIndexes = numberOfSystemIndexes;

	if (!open.openDatabase()) return;

	CString values;
	CString fields;

	fields = L"first_name, comment, sex_id, occurance, origin, meaning, language_id";



	m_command = L"DELETE FROM firstnames";
	if (!executeSys8(m_command)) return;

	m_command = L"DELETE FROM languages";
	if (!executeSys8(m_command)) return;


	if (!executeSys8(L"BEGIN")) return;

	m_command.Format(L"SELECT %s FROM firstnames", fields);
	if (!querySystem(m_command)) return;

	CProgressWnd wndP(NULL, L"Lineáris teljes lista készítése...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, m_recordsetSystem->RecordsCount());

	for (int i = 0; i < m_recordsetSystem->RecordsCount(); ++i)
	{
		values = L"";
		for (int j = 0; j < m_recordsetSystem->ColumnsCount(); ++j)
		{
			values += L"'";
			values += m_recordsetSystem->GetFieldString(j);
			values += L"'";
			if (j < m_recordsetSystem->ColumnsCount() - 1)
				values += L",";
		}
		m_command.Format(L"INSERT INTO firstnames ( %s ) VALUES ( %s )", fields, values);
		if (!executeSys8(m_command)) return;
		m_recordsetSystem->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

	m_command.Format(L"SELECT language FROM languages");
	if (!querySystem(m_command)) return;
	for (int i = 0; i < m_recordsetSystem->RecordsCount(); ++i)
	{
		values = m_recordsetSystem->GetFieldString(0);
		m_command.Format(L"INSERT INTO languages ( language ) VALUES ( '%s' )", values);
		if (!executeSys8(m_command)) return;

		m_recordsetSystem->MoveNext();
	}
	wndP.DestroyWindow();
	if (!executeSys8(L"COMMIT")) return;


	systemDB->Close();
	_wrename(m_systemDatabaseSpec, m_systemDatabaseSpec16);
	systemDB8->Close();
	_wrename(m_systemDatabaseSpec8, m_systemDatabaseSpec);

	openSystemDatabase();
	AfxMessageBox(L"Vége");
}
