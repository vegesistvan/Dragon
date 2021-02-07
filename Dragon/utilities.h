#pragma once

#include "vector"
#include "structures.h"
#include "GA_structures.h"

#define	ONE	TRUE			// for wordList
#define	MORE FALSE

#define OPEN TRUE
#define CLOSED FALSE

#define ANSI	0
#define UTF8	1
#define UTF8BOM 2


TCHAR *		string2char( CString str );
CString		getString( int n );
double		Log2( double n );
TCHAR *		thousend( unsigned _int64 val);
void		splitFilespec( CString filespec, CString *drive, CString *path, CString *name, CString *ext );
int			wordList( CStringArray *arrayP,CString line, char sep, BOOL single );
int			printStructure ();

int			countWords( CString str );
CString		dropUntil( CString str, char ch );
CString		getUntil( CString str, char ch );
CString		dropLastWord( CString cLine );
CString		dropLastCharacter( CString cLine );
CString		dropFirstWord( CString cLine );
CString		getWord( CString str, int n, int *pos );
CString		getFirstWord( CString str );
CString		getSecondWord( CString str );
CString		getLastWord( CString str );
CString		sepFirstName( CString str );

CString		lowerToUpper( CString str );
CString		toLower( CString str );

CString		date2date( CString date );
int			isDate( CStringArray* A, int i, CString* datum );
BOOL		isDateOK( CString datum );
BOOL		isNumeric( CString word );
BOOL		isRoman( CString word );
BOOL		isValidBrace( CString brace );

void		clearPeople( PEOPLE *p);

CString		nagyra( CString word );
void		clearName( NAME* name );
void		cleanNameBlock( NAMEBLOCK *n );
CString		cleanCline( CString cLine );
CString		cleanCline2( CString cLine );
CString		cleanHtmlLine( CString cLine );
CString		cleanHtmlTags( CString cLine );
void		clearTableHeader( TABLEHEADER* m_tableHeader);
void		clearSpouseBlock( SNAMEBLOCK* sp);
CString		packWords( CStringArray* A, int from, int db );
CString		getPeopleBlock( CString cLine );
int			isThere( CStringArray* A,  CString word );
void		setCreationTime( CString filespec );

void		splitPlaceDateComment( CString birthString, PLACE_DATE_BLOCK* ns );
void		sameClear( SAME * same );

//bool operator<(const SAMEMARRIAGE &s1, const SAMEMARRIAGE &s2);

bool		isLastCharacter( CString str, TCHAR kar );

CString		getMultiItem( CStringArray A, int i, int j );
CString		birthDateMinus( CString m_birth_date, int minus );
CString		cleanWord( CString word );
CString		upper( CString str );

bool		sortByName(const SPOUSES &v1, const SPOUSES &v2);
bool		sortByNameKids(const KIDS &v1, const KIDS &v2);


CString		firstUpper( CString name );
bool		checkDate( CString datum);

BOOL		isOdd( _int64 num );
BOOL		isEven( _int64 num );
int			GetInputCode( CString fileSpec );
bool		isUTF8( UCHAR * string, int length );
CStringA	AnsiToUtf8( CString str );
CString		Utf8ToAnsi( CString cLine );
int			same( CString ref, CString par1, CString par2 );
int			getYearFromDate( CString date );
CString		getTimeTag();
bool		openFileSpec( FILE** ff, CString fileSpec, CString mode );
CString		roundDate( CString date );
int			dateDiff( CString date1, CString date2 );
