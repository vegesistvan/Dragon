#include "stdafx.h"
#include "math.h"
#include "Dragon.h"
#include "stdlib.h"
#include "utilities.h"
#include "vector"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "structures.h"
#include "ProgressWnd.h"
#include "SqliteDB.h"
#include <ctype.h>
#include <wchar.h>
#include <afxpriv.h> // for USES_CONVERSION

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Log2( double n )  
{  
    return log( n ) / log( 2 );  
}  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR * thousend( unsigned _int64 val)
{
    static TCHAR result[128];
    TCHAR *pt;
    int n;
	swprintf_s( result, sizeof( result ) / sizeof(TCHAR), L"%I64d", val );
    // Point pt at end of string
    for( pt = result; *pt; pt++ ) {}
    
    n = 8;		// 3 digit + terminating 0 character
    while(1)
	{
        pt -= 3; // shift 3 digits
        if (pt > result)
		{
            memmove(pt + 1, pt, n);
            *pt = TCHAR(',');	// thousand separator
            n += 8;		// 3 digits + separator
        }
		else
            break;
    }
    return result;  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void splitFilespec( CString filespec, CString *drive, CString *path, CString *name, CString *ext )
{
	int length;
	int	p1;			// ':'
	int	p2;			// last '\'
	int	p3;			// last '.'

	length	= filespec.GetLength();
	p1		= filespec.Find( ':' );
	p2		= filespec.ReverseFind( '\\' );
	p3		= filespec.ReverseFind( '.' );

	*drive	= filespec.Left( p1 );
	*path	= filespec.Mid( p1+1, p2-p1-1 );
	*name	= filespec.Mid( p2+1, p3-p2-1 );
	*ext	= filespec.Right( length - p3 - 1 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Columns in 'line' are put into 'arrayP'. 
// Columns are separated by 'sep' character.
// 'single' == TRUE:	one separating character between columns. Between two adjecent separator an empty column is assumed 
//			== FALSE:	may be more separating characters between columns, which considered one separation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int wordList( CStringArray *arrayP,CString line, char sep, BOOL single ) 
{
	int pos = 0;			// position of sep
	int lastpos;			// previous position of sep
	int	length;

	arrayP->RemoveAll();
//	line.Replace( '\t', ' ' );
	while (pos!=-1)
	{
		lastpos = pos;
		pos=line.Find(sep,pos);											// find next sep	
		if(pos != -1)													// there is a sep!
		{
			length = pos-lastpos;										// length of the item between two sep
			if(length||single)arrayP->Add(line.Mid(lastpos,length));	// there is an item between two sep, or empty column if allowed
			pos++;														// step to next position and continue
		}
		else											
		{																// no more sep, save last item 
			if(lastpos<line.GetLength())								// if there is no sep after the last item
				arrayP->Add(line.Mid(lastpos));
		}
	}
	return int(arrayP->GetSize());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString dropUntil( CString str, char ch )
{
	int pos; 

	if( (pos = str.Find( ch )) == -1 ) return( L"" );

	str = str.Right( str.GetLength() - pos - 1 );
	return str;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString dropLastCharacter( CString cLine )
{
	CString str;
	int len = cLine.GetLength();
	str = cLine.Left( len-1 );
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int countWords( CString str )
{
	int pos; 
	int cnt = 1;
	str.Trim();
	while( (pos=str.Find( ' ' )) != -1 )
	{
		str = dropUntil( str, ' ' );
		str.Trim();
		++cnt;
	}
	return cnt;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getUntil( CString str, char ch )
{
	int pos; 

	if( (pos = str.Find( ch )) == -1 )
		return str;

	str = str.Left( pos );
	return str;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString dropLastWord( CString cLine )
{
	CString str( L"" );
	int pos;
	cLine.Trim();
	if( (pos = cLine.ReverseFind( ' ' )) != -1 )
		str = cLine.Left( pos );
	return str;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString dropFirstWord( CString cLine )
{
	CString str( L"" );
	int pos;
	cLine.Trim();
	if( (pos = cLine.Find( ' ' )) != -1 )
		str = cLine.Mid( pos+1 );
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a str n. szavát visszadja, pos-ba teszi a szó indexét 
// ha nincs n szó a str-ben, akkor a stringet adja vissza
CString getWord( CString str, int n, int *pos )
{
	CString word;
	word.Empty();
	int pozi;
	int poziS;
	int length = str.GetLength();

	str.Trim();
	if( ( pozi = str.Find( ' ' )) != -1 )
	{
		poziS = pozi + 1;
		for( int i = 1; i < n; ++i )
		{
			str = str.Mid( pozi+1 );
			str.Trim();
			if( ( pozi = str.Find( ' ' )) == -1 )
			{
				*pos = length;		// nincs több szó a stringben!!
				return str;
			}
			poziS += pozi+1;
		}
		word = str.Left( pozi );
		word.Trim();
	}
	else
	{
		poziS = -1;
		word = str;
	}
	*pos = poziS;
	return word;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getFirstWord( CString str )
{
	int pos;
	str.Trim();
	if( ( pos = str.Find( ' ' ) ) == -1 ) return str;
	return( str.Left( pos ) );
}
CString getSecondWord( CString str )
{
	CString out;
	int pos1;
	int pos2;
	str.Trim();
	if( ( pos1 = str.Find( ' ' ) ) == -1 ) return L"";			// csak legfeljebb 1 szó van benne;
	if( ( pos2 = str.Find( ' ', pos1+1 ) ) == -1 )
	{
		return( str.Mid( pos1+1 ) );			// csak 2 szó van benne
	}
	return( str.Mid( pos1+1, pos2-pos1-1 ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL isNumeric( CString word )
{
//	word.Trim();
  	CString str;
	str = word.SpanIncluding( L"0123456789");
	return( word.SpanIncluding( L"0123456789") == word );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL isRoman( CString word )
{
	LPCTSTR valid = L"IVXabcde";
	LPCTSTR valid2 = L"IVX";
	CString subs;
	CString subs2;

	subs = word.SpanIncluding( valid );
	if( subs.GetLength() == word.GetLength() )
	{
		subs2 = subs.SpanIncluding( valid2 );
		return( !subs2.IsEmpty() );
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
BOOL isFirstUpper( CString str )
{
	CString upper;
	upper = L"AÁBCDEÉFGHIÍJKLMNOÓÖÕPQRSTUÚÜÛVWXYZ";
	CString kar;
	kar = str[0];

	CString benne;
	benne = kar.SpanIncluding( upper );
	if( benne.IsEmpty() )
		return FALSE;
	else
		return TRUE;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString nagyra( CString word )
{
	int i = 0;
	TCHAR alma[100];
	for( int i = 0; i < word.GetLength(); ++i )
	{
		alma[i] = TCHAR(toupper( word[i] ));
	}
	alma[i] = 0;
	CString nagy;
	nagy = alma;
	return nagy;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString cleanCline( CString cLine )
{
	
	CString line;
	CString tag;
	CString str;

	line = dropUntil( cLine, '>' );						// leszedi	 <> jeleket
	if( line.Find( '>' ) != -1 )
		line = dropUntil( line, '>' );
	tag = getUntil( line, ';' );						// generáci után van;

	line = dropUntil( line, ';' );
	line.Remove( '%' ); 
	if( line == L"</ol>" ) line.Empty();
	line.Trim();
	str.Empty();
	if( !line.IsEmpty() )
		str.Format( L"%s %s", tag.Left(1), line );
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString cleanCline2( CString cLine )
{
	
	CString line;
	CString tag;
	CString str;

	line = dropUntil( cLine, '>' );						// leszedi	 <> jeleket
	if( line.Find( '>' ) != -1 )
		line = dropUntil( line, '>' );
	tag = getUntil( line, ';' );						// generáci után van;

	line = dropUntil( line, ';' );
	if( line == L"</ol>" ) line.Empty();
	line.Trim();
	str.Empty();
	if( !line.IsEmpty() )
		str.Format( L"%s %s", tag.Left(1), line );
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A soremlés és más kiszûrése
CString cleanHtmlLine( CString cLine )
{
	
	CString str;
	CString line;
	CString tag;
	int pos;

	if( ( pos = cLine.Find( '<' ) ) != -1 )
	{
		if( pos )
		{
			cLine = cLine.Left( pos );			// ha <  a sor végén van nem az elsõ helyen van
		}
		else
		{
			if( ( pos = cLine.ReverseFind( '>' ) )!= -1 )
			{
				cLine = cLine.Mid( pos+1 );
			}
		}
	}

	tag = getUntil( cLine, ';' );
	if( ( pos = cLine.Find( ';' ) ) != -1 )
		if( pos != cLine.GetLength() )
			cLine = dropUntil( cLine, ';' );

	str.Empty();
	if( !cLine.IsEmpty() )
		if( cLine.GetAt(0) != '%' ) 
			str.Format( L"%s %s", tag.Left(1), cLine );
		else
			str = cLine;
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A soremlés és más kiszûrése
CString cleanHtmlTags( CString cLine )
{
	CString str;
	CString str1;
	CString str2;
	CString tag;
	int	pos;
	int pos1 = 0;
	int pos2 = 0;

	while( (pos1 = cLine.Find( '<', pos1 ) ) != -1 )
	{
		str1 = cLine.Left( pos1 );
		if( ( pos2 = cLine.Find( '>', pos1  ) ) == - 1 )
		{
			str.Format( L"A '<' karakter után nincs '>' karakter!!\n%s", cLine );
			AfxMessageBox( str, MB_ICONERROR );
			return L"";
		}
		str2 = cLine.Mid( pos2+1 );
		cLine = str1 + str2;
	}

	tag = getUntil( cLine, ';' );
	if( ( pos = cLine.Find( ';' ) ) != -1 )
		if( pos != cLine.GetLength() )
			cLine = dropUntil( cLine, ';' );

	str.Empty();
	if( !cLine.IsEmpty() )
		if( cLine.GetAt(0) != '%' ) 
			str.Format( L"%s %s", tag.Left(1), cLine );
		else
			str = cLine;
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clearPeople( PEOPLE *p)
{
	p->generation  = L'';
	p->birth_date.Empty();
	p->birth_place.Empty();
	p->comment.Empty();
	p->death_date.Empty();
	p->death_place.Empty();
	p->father_id = L"0";
	p->first_name.Empty();
	p->last_name.Empty();
	p->known_as.Empty();
	p->mother_id = L"0";
	p->rowid.Empty();
	p->sex_id = 0;
	p->title.Empty();
	p->titolo.Empty();
	p->mother_index = 0;
	p->mother_index2 = 0;
	p->tableAncestry = 0;
	p->tableNumber.Empty();
	p->lineNumber = 0;
	p->tableRoman.Empty();
	p->orderFather = 1;
	p->orderMother = 1;
	p->folyt.Empty();
	p->arm.Empty();
	p->csalad.Empty();
	p->gap = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cleanNameBlock( NAMEBLOCK *n )
{
	n->fullname.Empty();
	n->comment.Empty();
	n->first_name.Empty();
	n->title.Empty();
	n->mother_index = 0;
	n->sex_id = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void clearGenerations( GENERATIONS *gen )
{
	gen->gen = TCHAR(' ');
	gen->descendant_id = L"0";
	gen->orderFather = 0;
	for( UINT i = 0; i < 10; ++i )	gen->spouse_id[i] = L"0";	// egy leszármazottnak max 10 házastársának rowid-jét tartja nyilván
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clearName( NAME* name ) 
{
	name->sex_id = 0;
	name->titolo.Empty();
	name->title.Empty();
	name->first_name.Empty();
	name->last_name.Empty();
	name->mother_index  = 0;
	name->fullname.Empty();
	name->comment.Empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clearSpouseBlock( SNAMEBLOCK* sp)
{
	sp->comment.Empty();
	sp->date.Empty();
	sp->name.Empty();
	sp->place.Empty();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int isThere( CStringArray* A,  CString word )
{
	int i;
	for( i= 0; i < A->GetCount(); ++i )
	{
		if( A->GetAt(i) == word ) break;
	}
	if( i == A->GetCount() )
		return -1;
	else
		return i;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az A arrajbõl az n-dik szótól m darab szót összerak szóközzel elválasztva, és azt visszadja
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString packWords( CStringArray* A, int n, int m )
{
	if( !m ) return L"";

	CString str(L"");
	int cnt = 0;

/*
	int count = A->GetCount();

	if( n + m > count )
	{
		int f = 1;
	}
*/
	int db = A->GetSize() ;
	if( db == 4 )
			int g = 0;

	for( int j=n; cnt < m; ++j )
	{
		if( j >= db || j < 0 )
			int z = 2;

		str += A->GetAt(j);
		str += L" ";
		++cnt;
	}
	if( db == 4 )
			int g = 0;
	return str.Trim();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getPeopleBlock( CString cLine )
{
	int pos;
	if( ( pos= cLine.Find( '=' ) ) != -1 )
		cLine = cLine.Left( pos-1 );
	return cLine.Trim();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getLastWord( CString cLine )
{
	CString ret(L"" );
	int pos;
	if( ( pos= cLine.ReverseFind( ' ' ) ) != -1 ) 
		ret = cLine.Mid( pos + 1 );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setCreationTime( CString filespec )
{
	SYSTEMTIME thesystemtime;
	FILETIME thefiletime;

 	GetSystemTime(&thesystemtime);
	SystemTimeToFileTime(&thesystemtime,&thefiletime);

	HANDLE fileHandle = CreateFile( filespec, FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	SetFileTime( fileHandle, &thefiletime, (LPFILETIME) NULL,(LPFILETIME) NULL );
	CloseHandle(fileHandle);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString birthDateMinus( CString birth_date, int minus )
{
	CString str;
	int year;
	
	year = _wtoi( birth_date.Left(4 ) ) - minus;
	str.Format( L"%d", year ); 
	if( year < 0  )
	{
		str.Empty();
	}
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString cleanWord( CString word )
{
	word.Replace( '?', ' ' );		// bizonytalan név
	word.Replace( ',', ' ' );		// Pl: János, Nemeslubella jelzi, hogy Nemeslubella nem tartozik a névhez, hanem comment
//	word.Replace( '_', ' ' );		// nevekben gyakran van összekötés
//	word.Replace( '-', ' ' );		// nevekben gyakran van összekötés
	word.Replace( ';', ' ' );		// Pl: =N; János de Divék
	word.Trim();
	return word;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sameClear( SAME * same )
{
	same->index = 0;
	same->repleceBy.Empty();
	same->rowid.Empty();
	same->same = ' ';
	same->group = 0;
	same->identical = L"";
	same->timeConflict = L"";
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString lowerToUpper( CString lowStr )
{
	CString str(L"");
	const TCHAR low[] =
	{
		TCHAR('a'),TCHAR('á'),TCHAR('b'),TCHAR('c'),TCHAR('d')
	};

	const TCHAR upp[] =
	{
		TCHAR('A'),TCHAR('Á'),TCHAR('B'),TCHAR('C'),TCHAR('D')
	};


	CString uppStr(L"");
	TCHAR	kar;
	for( int i = 0; i < lowStr.GetLength(); ++i )
	{
		kar = lowStr[i];
		for( int j = 0; j < sizeof( low ); ++j )
		{
			if( low[j] == kar )
			{
				str.Format( L"%s%c", uppStr, upp[j] );
				uppStr = str;
				break;
			}
			else
			{
				str.Format( L"%s%c", uppStr, low[j] );
				uppStr = str;
			}
		}
	}
	return uppStr;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString toLower( CString str )
{
	int tLen = str.GetLength()+1;
	LPTSTR strC = new TCHAR[tLen];
	_tcscpy_s( strC,tLen, str.GetBuffer());
	_wcslwr_s( strC, tLen );
	str = strC;
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clearTableHeader( TABLEHEADER* tableHeader)
{
	tableHeader->arm.Empty();
	tableHeader->comment.Empty();
	tableHeader->familyName.Empty();
	tableHeader->alias.Empty();
	tableHeader->familyNumber = 0;
	tableHeader->folyt.Empty();
	tableHeader->joint = 0;
	tableHeader->known_as.Empty();
	tableHeader->lineNumber = 0;
	tableHeader->percent.Empty();
	tableHeader->torzs.Empty();
	tableHeader->tableHeader.Empty();
	tableHeader->tableNumber = 0;
	tableHeader->tableRoman.Empty();
	tableHeader->titolo.Empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortByName(const SPOUSES &v1, const SPOUSES &v2) 
{ 
	if( ! v1.rowid.Compare( v2.rowid ) )
		return( v1.name < v2.name  );
	return( v1.name < v2.name ); 
}

bool sortByNameKids(const KIDS &v1, const KIDS &v2) 
{ 
	if( ! v1.rowid.Compare( v2.rowid ) )
		return( v1.name < v2.name  );
	return( v1.name < v2.name ); 
}
bool isLastCharacter( CString str, TCHAR kar )
{
	TCHAR kar1 = str.GetAt( str.GetLength() - 1 );
	if( kar == kar1 ) return TRUE;
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a dátum eõtt álló 'kb' ill. a dátum után álló 'után','körül','kb' a dátum része!
int isDate( CStringArray* A, int i, CString* datum )
{
	INT_PTR n = A->GetCount();
	int ret = 0;
	CStringArray M;
	CString date(L"");
	CString miez;
	M.Add( L"után");
	M.Add( L"körül" );
	M.Add( L"elõtt" );
	M.Add( L"kb" );
	
	miez = A->GetAt(i);
	miez.Replace( '?', ' ' );
	miez.Trim();
	if( isNumeric( miez) )
	{
		++ret;
		if( i > 0 && !A->GetAt( i-1 ).Compare( L"kb" ) )		// kb 1944
		{
			date = L"kb ";
			date += A->GetAt( i );
			++ret;
		}
		else
			date = A->GetAt( i );

		if( i+1 < n-1 )
		{
			for( int j = 0; j < M.GetCount(); ++j )
			{
				if( !A->GetAt( i+1 ).Compare( M[j] ) )
				{
					date += " ";
					date += M[j];
					++ret;
					break;
				}
			}
		}
	}
	*datum = date;
	return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a dátum eõtt álló 'kb' ill. a dátum után álló 'után','körül','kb' a dátum része!
BOOL isDateOK( CString datum )
{
	CString date;
	CString modifier;

	date = getFirstWord( datum );
	if( isNumeric( date ) )
	{
		modifier = getSecondWord( datum );
		if( modifier.IsEmpty() || modifier == L"után" || modifier == L"körül" || modifier == L"kb" )
			return TRUE ;
		else
			return FALSE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////
BOOL isValidBrace( CString brace )
{
	if( brace.Find( '-' )		!= -1 )	return TRUE;
	if( brace.Find( L"f." )		!= -1 )	return TRUE;
	if( brace.Find( L"lánya" )	!= -1 )	return TRUE;
	if( brace.Find( L"leánya" )	!= -1 )	return TRUE;
	if( brace.Find( L"fia" )	!= -1 )	return TRUE;
	return FALSE;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString date2date( CString date )
{
	CStringArray monthes;

	monthes.Add( L"Jan" );
	monthes.Add( L"Feb" );
	monthes.Add( L"Mar" );
	monthes.Add( L"Apr" );
	monthes.Add( L"May" );
	monthes.Add( L"Jun" );
	monthes.Add( L"Jul" );
	monthes.Add( L"Aug" );
	monthes.Add( L"Sep" );
	monthes.Add( L"Oct" );
	monthes.Add( L"Nov" );
	monthes.Add( L"Dec" );

	CStringArray A;
	int pos1 = -1;
	int pos2 = -1;
	int n;
	int	i;
	int year;
	int month = 0 ;
	int day;
	CString honap;
	CString modifier(L"");
	CString str;
	CString modi[] = { L"kb", L"elõtt", L"után", L"körül" };


	if( (pos1 = date.Find( L"BEF" ) ) != -1  || (pos2 = date.Find( L"AFT" ) ) != -1 )
	{
		if( pos1 != -1 ) modifier = L" elõtt";
		else if( pos2 != -1 ) modifier = L" után";
		date = date.Mid( 4 );
	}

	for( i = 0; i < sizeof( TCHAR )/ sizeof( modi ); ++i )
	{
		if( ( pos1 = date.Find( modi[i] ) ) != -1 )
		{
			modifier = modi[i];
			date = date.Left( pos1 );
			break;
		}
	}

	n = wordList( &A, date, ' ', FALSE );

	if( n != 3 )
	{
		date.Remove( '(' );
		date.Remove( ')' );
		date.Trim();

		int len = date.GetLength();
		if( len == 11 )
		{
			if( date.GetAt( len-1 ) == '.' )
			{
				str = date.Left( len-1 );
				date = str;
			}
		}
		return( date );
	}
	else
	{										// 17 DEC 1944
		day = _wtoi( A.GetAt(0) );
		year = _wtoi( A.GetAt( 2 ) );

		for( int i = 0; i < monthes.GetCount(); i++ )
		{
			honap = A.GetAt( 1 );
			honap.Trim();
			if( !honap.CompareNoCase( monthes[i] ) )
			{
				month = i + 1;
				break;
			}
		}
		str.Format( L"%4d.%02d.%02d", year, month, day );
	}
	str += modifier;
	str.Trim();
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString firstUpper( CString name )
{
	CString out;
	CStringArray A;
	int n;
	TCHAR kar;

	n = wordList( &A, name, ' ', false );
	for( int i = 0; i < n; ++i )
	{
		if( A[i] != L"és" )
		{
			kar = A[i].GetAt( 0 );
			kar = TCHAR(toupper( kar ));
			A[i].SetAt( 0, kar );
		}
	}
	out = packWords( &A, 0, n );
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString upper( CString str )
{
	int length = str.GetLength();
	TCHAR karI;
	TCHAR karO;
	CString out(L"");
	for( int i = 0; i < length; ++i )
	{
		karI = str.GetAt( i );
		karO = TCHAR(toupper( karI ));
		out += karO;
	}
	return out;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL isOdd( _int64 num )
{
	return( num % 2 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL isEven( _int64 num )
{
	return( !(num % 2) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getString( int n )
{
	CString str;
	str.Format( L"%d", n );
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR * string2char( CString str )
{
	int tLen = str.GetLength()+1;
	LPTSTR sT = new TCHAR[tLen];
	_tcscpy_s( sT, tLen, str.GetBuffer());
	return sT;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool checkDate( CString datum)
{
	int length = datum.GetLength();
	if( length < 4 ) return false;

	int	year = _wtoi( datum.Left( 4 ) );
	if( year < 1000 ) return false;

// év ellenõrzése
	char	sTime[50];
	time_t	now = time(0);
	ctime_s( sTime, sizeof( sTime ), &now);
	CString st;
	st = sTime;
	int	yearCurr = _wtoi( st.Right( 6 ) );		// "\n" miatt 6 karaktert vesz le, ami konverzinál ugyis lemarad
	if( year > yearCurr ) return false;

	// év utáni '.' ellenõrzése
	if( length == 4 ) return true;	// csak 2001 a dátum

	if( datum.GetAt( 4 ) != '.' ) return false;   
	if( length <= 7 ) return false;

// hónap ellenõrzése a 2019.11 
	if( !isdigit( datum.GetAt( 5 ) ) ) 
		return false;
	if( !isdigit( datum.GetAt( 6 ) ) ) 
		return false;
	int	month = _wtoi( datum.Mid( 5, 2 ) );
	if( month < 1 || month > 12 ) return false;
	
	if( length == 8  ) return true;	// csak 2019.11.  a dátum

	if( datum.GetAt( 7 ) != '.' ) return false;
	if( length < 10 ) return false;

// nap ellenõrzése 2019.11.12
	if( !isdigit( datum.GetAt( 8 ) ) ) 
		return false;
	if( !isdigit( datum.GetAt( 9 ) ) ) 
		return false;
	int	day = _wtoi( datum.Mid( 8, 2 ) );
	if( day < 1 ) return false;
	if( month == 2 && day > 29 ) return false;
	if(( month == 4 || month == 6 || month == 9 || month == 11 ) && day > 30 ) return false;
	if( day > 31 ) return false; 
	if( length > 10 ) return false;  // dátum után még van valami
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetInputCode( CString fileSpec )
{
	FILE* fl;
	int code = ANSI;
	int errno_t;

	CString str;
	
	if(( errno_t = _wfopen_s( &fl, fileSpec, L"rb" ) ))
	{
		str.Format(L"%s megnyitási hiba!", fileSpec );
		AfxMessageBox(str);
		return NULL;
	}
	byte bom1 = 0XEF;
	byte bom2 = 0XBB;
	byte bom3 = 0XBF;

	bool bomFlag = false;
	int cnt = 1;

#define LEN  1000
	UCHAR	buffer[ LEN + 1 ];
	int		length;
	buffer[LEN] = '\0';
	while( (length = fread( buffer, 1, LEN, fl ) ) ) 
	{
		if( cnt )
		{
			if( buffer[0] == bom1 && buffer[1] == bom2 && buffer[2] == bom3 )
				bomFlag = true;
		}
		cnt = 0;

		if( isUTF8( buffer, length ) )
		{
			if( bomFlag )
				code = UTF8BOM;
			else
				code = UTF8;
			break;
		}
		++cnt;
	}
	fclose( fl );
	return code;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isUTF8( UCHAR * bytes, int length )
{
	int i = 0;
//	const unsigned char * bytes = (const unsigned char *)string;
//	while( i < length )
	while( *bytes )
	{
		// ASCII use bytes[0] <= 0x7F to allow ASCII control characters
		if( bytes[0] == 0x09 || bytes[0] == 0x0A ||	bytes[0] == 0x0D || (0x20 <= bytes[0] && bytes[0] <= 0x7E) )
		{
			++bytes;
//			++i;
			continue;
		}

		// non-overlong 2-byte
		if( (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&  (0x80 <= bytes[1] && bytes[1] <= 0xBF)  )
		{
			bytes += 2;
//			i += 2;
			continue;
		}

		// excluding overlongs  // straight 3-byte // excluding surrogates
		if( ( bytes[0] == 0xE0 && (0xA0 <= bytes[1] && bytes[1] <= 0xBF) && (0x80 <= bytes[2] && bytes[2] <= 0xBF) ) ||
			( ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) || bytes[0] == 0xEE || bytes[0] == 0xEF) && (0x80 <= bytes[1] && bytes[1] <= 0xBF) && (0x80 <= bytes[2] && bytes[2] <= 0xBF) ) ||
			( bytes[0] == 0xED && (0x80 <= bytes[1] && bytes[1] <= 0x9F) && (0x80 <= bytes[2] && bytes[2] <= 0xBF) ))
		{
			bytes += 3;
//			i += 3;
			continue;
		}

		// planes 1-3 // planes 4-15  // plane 16
		if( ( bytes[0] == 0xF0 &&  (0x90 <= bytes[1] && bytes[1] <= 0xBF) && (0x80 <= bytes[2] && bytes[2] <= 0xBF) && (0x80 <= bytes[3] && bytes[3] <= 0xBF) ) ||
			( (0xF1 <= bytes[0] && bytes[0] <= 0xF3) && (0x80 <= bytes[1] && bytes[1] <= 0xBF) && (0x80 <= bytes[2] && bytes[2] <= 0xBF) && (0x80 <= bytes[3] && bytes[3] <= 0xBF) ) ||
			( bytes[0] == 0xF4 && (0x80 <= bytes[1] && bytes[1] <= 0x8F) && (0x80 <= bytes[2] && bytes[2] <= 0xBF) && (0x80 <= bytes[3] && bytes[3] <= 0xBF) ) )
		{
			bytes += 4;
//			i += 4;
			continue;
		}
		return 0;
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CStringA AnsiToUtf8( CString str )
{
	return( UnicodeToUtf8( str ) ); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString Utf8ToAnsi( CString cLine )
{		
	const char* pBuffer;
	int len=cLine.GetLength()+1;
	pBuffer = new char[ len];
	USES_CONVERSION;
	pBuffer = T2A( cLine );
	return( Utf8ToUnicode( pBuffer ) ); 
}
