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
	while( pos != -1 )
	{
		lastpos = pos;
		pos = line.Find( sep, pos );											// find next sep	
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
// a str n. szav�t visszadja, pos-ba teszi a sz� index�t 
// ha nincs n sz� a str-ben, akkor a stringet adja vissza
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
				*pos = length;		// nincs t�bb sz� a stringben!!
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
CString sepFirstName( CString str )
{
	str.Replace( '_', ' ' );
	str.Replace( '-', ' ' );
	return( getFirstWord( str ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getFirstWord( CString str )
{
	int pos;
	str.Trim();
	if( ( pos = str.Find( ' ' ) ) == -1 ) return str;
	return( str.Left( pos ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getSecondWord( CString str )
{
	CString out;
	int pos1;
	int pos2;
	str.Trim();
	if( ( pos1 = str.Find( ' ' ) ) == -1 ) return L"";			// csak legfeljebb 1 sz� van benne;
	if( ( pos2 = str.Find( ' ', pos1+1 ) ) == -1 )
	{
		out = str.Mid( pos1+1 );			// csak 2 sz� van benne
	}
	out = str.Mid( pos1+1, pos2-pos1-1 );
	return( out );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getTwoWords( CString str )
{
	CString out;
	int pos1;
	int pos2;
	str.Trim();
	if( ( pos1 = str.Find( ' ' ) ) == -1 ) return L"";			// csak legfeljebb 1 sz� van benne;
	if( ( pos2 = str.Find( ' ', pos1+1 ) ) == -1 )
	{
		return( str );			// csak 2 sz� van benne
	}
	return( str.Left( pos2 -1 ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL isNumeric( CString word )
{
	if( word.SpanIncluding( L"?" ) == word )  // csak '?' karakter van a sz�ban 
		return false;
	return( word.SpanIncluding( L"0123456789 ") == word );
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString cleanCline( CString cLine )
{
	
	CString line;
	CString tag;
	CString str;

	line = dropUntil( cLine, '>' );						// leszedi	 <> jeleket
	if( line.Find( '>' ) != -1 )
		line = dropUntil( line, '>' );
	tag = getUntil( line, ';' );						// gener�ci ut�n van;

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
/*
CString cleanCline2( CString cLine )
{
	
	CString line;
	CString tag;
	CString str;

	line = dropUntil( cLine, '>' );						// leszedi	 <> jeleket
	if( line.Find( '>' ) != -1 )
		line = dropUntil( line, '>' );
	tag = getUntil( line, ';' );						// gener�ci ut�n van;

	line = dropUntil( line, ';' );
	if( line == L"</ol>" ) line.Empty();
	line.Trim();
	str.Empty();
	if( !line.IsEmpty() )
		str.Format( L"%s %s", tag.Left(1), line );
	return str;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A soreml�s �s m�s kisz�r�se
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
			cLine = cLine.Left( pos );			// ha <  a sor v�g�n van nem az els� helyen van
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
// A soreml�s �s m�s kisz�r�se
CString cleanHtmlTags( int lineNumber, CString cLine )
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
			str.Format( L"A '<' karakter ut�n nincs '>' karakter!!\nL%d %s", lineNumber, cLine );
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az A arrajb�l az n-dik sz�t�l m darab sz�t �sszerak sz�k�zzel elv�lasztva, �s azt visszadja
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString packWords( CStringArray* A, int n, int m )
{
	if( !m ) return L"";

	CString str(L"");
	int cnt = 0;

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
bool isLastCharacter( CString str, TCHAR kar )
{
	if( str.IsEmpty() ) return false;
	TCHAR kar1 = str.GetAt( str.GetLength() - 1 );
	if( kar == kar1 ) return TRUE;
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a d�tum e�tt �ll� 'kb' ill. a d�tum ut�n �ll� 'ut�n','k�r�l','kb' a d�tum r�sze!
// visszadott ret �rt�ke megadja, h�ny sz�b�l �ll a d�tum
int isDate( CStringArray* A, int i, CString* datum )
{
	INT_PTR n = A->GetCount();
	int ret = 0;
	CStringArray M;
	CString date(L"");
	CString miez;
	M.Add( L"ut�n");
	M.Add( L"k�r�l" );
	M.Add( L"el�tt" );
	M.Add( L"kb" );
	
	miez = A->GetAt(i);
	miez.Replace( '?', ' ' );
	miez.Replace( '.', ' ' );
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
// a d�tum e�tt �ll� 'kb' ill. a d�tum ut�n �ll� 'ut�n','k�r�l','kb' a d�tum r�sze!
BOOL isDateOK( CString datum )
{
	if( datum.IsEmpty() ) return true;
	CString date;
	CString modifier;

	date = getFirstWord( datum );
	if( checkDate( date ) )
	{
		modifier = getSecondWord( datum );
		if( modifier.IsEmpty() || modifier == L"ut�n" || modifier == L"k�r�l" || modifier == L"kb" )
			return TRUE ;
	}
	return FALSE;
}
/*
//////////////////////////////////////////////////////////////////////////////////////////
BOOL isValidBrace( CString brace )
{
	if( brace.Find( '-' )		!= -1 )	return TRUE;
	if( brace.Find( L"f." )		!= -1 )	return TRUE;
	if( brace.Find( L"l�nya" )	!= -1 )	return TRUE;
	if( brace.Find( L"le�nya" )	!= -1 )	return TRUE;
	if( brace.Find( L"fia" )	!= -1 )	return TRUE;
	return FALSE;
}
*/
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
	CString modi[] = { L"kb", L"el�tt", L"ut�n", L"k�r�l" };


	if( (pos1 = date.Find( L"BEF" ) ) != -1  || (pos2 = date.Find( L"AFT" ) ) != -1 )
	{
		if( pos1 != -1 ) modifier = L" el�tt";
		else if( pos2 != -1 ) modifier = L" ut�n";
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
		if( A[i] != L"�s" )
		{
			kar = A[i].GetAt( 0 );
			kar = TCHAR(toupper( kar ));
			A[i].SetAt( 0, kar );
		}
	}
	out = packWords( &A, 0, n );
	return out;
}
/*
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
*/
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
/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR * string2char( CString str )
{
	int tLen = str.GetLength()+1;
	LPTSTR sT = new TCHAR[tLen];
	_tcscpy_s( sT, tLen, str.GetBuffer());
	return sT;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Elfogadott form�tum:
// az �vnek 1000 �s az aktu�lis �v k�z� kell esni

// 1944
// 1944.12.
// 1944.12.17

// az �res d�tumot is hib�snak �rt�keli

bool checkDate( CString datum)
{
	datum.Trim();
	int length = datum.GetLength();
	if( length < 4 ) return false;

	int	year = _wtoi( datum.Left( 4 ) );
	if( year < 1000 ) return false;

// �v ellen�rz�se
	char	sTime[50];
	time_t	now = time(0);
	ctime_s( sTime, sizeof( sTime ), &now);
	CString st;
	st = sTime;
	int	yearCurr = _wtoi( st.Right( 6 ) );		// "\n" miatt 6 karaktert vesz le, ami konverzin�l ugyis lemarad
	if( year > yearCurr ) return false;

	// �v ut�ni '.' ellen�rz�se
	if( length == 4 ) return true;	// csak 2001 a d�tum

	if( datum.GetAt( 4 ) != '.' ) return false;   
	if( length <= 7 ) return false;

// h�nap ellen�rz�se a 2019.11 
	if( !iswdigit( datum.GetAt( 5 ) ) ) 
		return false;
	if( !iswdigit( datum.GetAt( 6 ) ) ) 
		return false;
	int	month = _wtoi( datum.Mid( 5, 2 ) );
	if( month < 1 || month > 12 ) return false;
	
	if( length == 8  ) return true;	// csak 2019.11.  a d�tum

	if( datum.GetAt( 7 ) != '.' ) return false;
	if( length < 10 ) return false;

// nap ellen�rz�se 2019.11.12
	if( !iswdigit( datum.GetAt( 8 ) ) ) 
		return false;
	if( !iswdigit( datum.GetAt( 9 ) ) ) 
		return false;
	int	day = _wtoi( datum.Mid( 8, 2 ) );
	if( day < 1 ) return false;
	if( month == 2 && day > 29 ) return false;
	if(( month == 4 || month == 6 || month == 9 || month == 11 ) && day > 30 ) return false;
	if( day > 31 ) return false; 
	if( length > 10 ) return false;  // d�tum ut�n m�g van valami
	return true;
}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString roundDate( CString date )
{
	int length = date.GetLength();
	switch( length ) 
	{
	case 4:
		date += L".06.30";
		break;
	case 7:
		date += L".01";
		break;
	case 8:
		date += L"01";
	}
	return date;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Addig olvassa a f�jlt, am�g a v�g�re nem �r vagy tal�lt benne UTF8BOM-ot vagy UTF8 k�dot
// Default k�dol�s: ANSI
int GetInputCode( CString fileSpec )
{
	FILE* fl;
	int errno_t;
	CString str;
	
	if(( errno_t = _wfopen_s( &fl, fileSpec, L"rb" ) ))
	{
		str.Format(L"%s megnyit�si hiba!", fileSpec );
		AfxMessageBox(str);
		return NULL;
	}

	byte bom1 = 0XEF;
	byte bom2 = 0XBB;
	byte bom3 = 0XBF;

	int cnt = 1;

#define LEN  100
	UCHAR	buffer[ LEN ];
	int		length;
	while( (length = fread( buffer, 1, LEN, fl ) ) ) 
	{
		if( cnt )
		{
			if( buffer[0] == bom1 && buffer[1] == bom2 && buffer[2] == bom3 )
			{
				fclose( fl );
				return UTF8BOM;
			}
		}
		cnt = 0;
		if( isUTF8( buffer, length ) )
		{
			fclose( fl );
			return UTF8;
		}
	}
	fclose( fl );
	return ANSI;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Layout of UTF-8 byte sequences
// Number of bytes	First code point	Last code point	Byte 1		Byte 2		Byte 3		Byte 4
// 1				U+0000				U+007F			0xxxxxxx	
// 2				U+0080				U+07FF			110xxxxx	10xxxxxx	
// 3				U+0800				U+FFFF			1110xxxx	10xxxxxx	10xxxxxx	
// 4				U+10000	[nb 2]		U+10FFFF		11110xxx	10xxxxxx	10xxxxxx	10xxxxxx
bool isUTF8( UCHAR * bytes, int length )
{
	int i = 0;
	UCHAR* b = bytes;
	UCHAR byte;
	while( i < length )
	{
		byte = bytes[i];
		if(  (bytes[i] >> 3) == 0X1E && i+3 < length && (bytes[i+1] >> 6 ) == 0X02 && (bytes[i+2] >> 6 ) == 0X02 && (bytes[i+3] >> 6 ) == 0X02 )
			return UTF8;																			// 4 bytes 
		if( (bytes[i] >> 4 ) == 0X0E && i+2 < length && (bytes[i+1] >> 6 ) == 0X02 && (bytes[i+2] >> 6 ) == 0X02 )
			return UTF8;																			// 3 bytes
		if( (bytes[i] >> 5 ) == 0X06 && i+1 < length && (bytes[i+1] >> 6 ) == 0X02 )
			return UTF8;																			// 2 bytes
		++i;
	}
	return ANSI;
}

/*
bool isUTF8( UCHAR * bytes, int length )
{
	int i = 0;
	UCHAR* b = bytes;
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
		return 0;		// ANSI
	}
	return 1;			// UTF8
}
*/
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  1 : mindkett� meg van adva �s egyezik;
//  0 : csak az egyik vagy egy sincs van megadva, �gy nincs ellentmond�s
//  -1 : mindkett� meg van adva �s nem egyezik
int same( CString ref, CString par1, CString par2 )
{
	int len1 = par1.GetLength();
	int len2 = par2.GetLength();
	int len;
	if( len1 && len2 )
	{
		len = ( len1 < len2) ? (len1):(len2);
		if( par1.Left(len) == par2.Left(len ) )
			return 1;
		else
			return -1;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int getYearFromDate( CString date )
{
	date.Replace( '?', ' ' );
	date.Trim();
	if( date.Left(2) == L"kb" )
		date = date.Mid( 3 );
	date = date.Left(4);
	date.Trim();
	int year = _wtoi( date );
	if( year < 1000 ) year = 0;
	return( year );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString getTimeTag()
{
	return( CTime::GetCurrentTime().Format("%Y%m%d%H%M") );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool openFileSpec( FILE** ff, CString fileSpec, CString mode )
{
	int		errno_t;
	TCHAR buffer[100];
	CString str;
	if((errno_t=_wfopen_s( ff, fileSpec, mode ) )!=0)  // w+ = reading and writing, existing file destroyed
	{	
		_wcserror_s( buffer, sizeof( buffer ) );
		str.Format(L"%s\nmegnyit�si hiba!\n%s", fileSpec, buffer );
		AfxMessageBox(str);
		return false;
	}
	return true;
}
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int dateDiff( CString date1, CString date2 )
{
	int diff = _wtoi( date1.Left(4) ) - _wtoi( date2.Left(4) );
	return diff;
}
*/
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return:
// 1000000 - nem �rt�kelhet�

//  0  date1 == date2
// -1  date1 < date2
//  1  date1 > date2
int compDate( CString date1, CString date2 )
{
	int invalid = 100000;
	if( !checkDate( date1 ) || !checkDate(date2 ) ) return invalid;

	int n1 = date1.GetLength();
	int n2 = date2.GetLength();
	if( n1 < n2 )		date2 = date2.Left( n1 );
	else if( n1> n2 )	date1 = date1.Left( n2 );
	return( date1.Compare( date2 ) );
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// K�t d�tum YYYY[[.MM.]DD[ form�tum�
// a hosszabb d�tumot csonk�tja a r�videbb hossz�ra �s �gy hasonl�tja �ssze
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int diffD( CString date1, CString date2, int* pYear )
{
	if( date1.IsEmpty() || date2.IsEmpty() ) return INT_MAX;
	*pYear  = _wtoi( date1.Left(4) ) - _wtoi( date2.Left(4) );

	int len1 = date1.GetLength();
	int len2 = date2.GetLength();
	int len  = len1;

	if( len1 < len2 )
		date2 =date2.Left(len1);
	else if( len1 > len2 )
	{
		date1 = date1.Left( len2 );
		len = len2;
	}
	return( date1.Compare( date2 ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString  getNameBD( CString name, CString birth, CString death, CString wedding )
{
	CString nameBD = name;

	if( wedding.IsEmpty() )
	{
		if( !birth.IsEmpty() && death.IsEmpty() )
			nameBD.Format( L"%s (*%s)", name, birth );
		else if( birth.IsEmpty() && !death.IsEmpty()  )
			nameBD.Format( L"%s (+%s)", name, death );
		else if( !birth.IsEmpty() && !death.IsEmpty() )
			nameBD.Format( L"%s (*%s +%s)", name, birth, death );
	}
	else
	{
		if( !birth.IsEmpty() && death.IsEmpty() )
			nameBD.Format( L"%s (*%s =%s)", name, birth, wedding );
		else if( birth.IsEmpty() && !death.IsEmpty()  )
			nameBD.Format( L"%s (+%s =%s)", name, death, wedding);
		else if( !birth.IsEmpty() && !death.IsEmpty() )
			nameBD.Format( L"%s (*%s +%s =%s)", name, birth, death, wedding );
		else if( birth.IsEmpty() && death.IsEmpty() )
			nameBD.Format( L"%s (=%s)", name, wedding );
	}



	return nameBD;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString pack( CString tag, CString place, CString date )
{
	CString str;
	str.Format( L"%s %s", place, date );
	str.Trim();
	if( !str.IsEmpty() )
		str.Format( L"%s%s", tag, (CString)str );
	return str;
}