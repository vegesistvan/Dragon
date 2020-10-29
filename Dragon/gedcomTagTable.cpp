#include "stdafx.h"
#include "Dragon.h"
#include "gedcomIn.h"
#include "GAtoDB.h"
#include <algorithm>
#include "ProgressWnd.h"
#include "utilities.h"

int cnt = 1;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::tagTable()
{
	CString fileSpec;
	CString cLine;
	GEDLINE gl;

	theApp.m_inputCode = GetInputCode( theApp.m_gedFileSpec );

	fileSpec = theApp.openTextFile( &theApp.fl, L"TAGTABLE", L"w+" );
	fwprintf( theApp. fl, L"%s\n\n", theApp.m_gedFileSpec );
	file_ged.Open( theApp.m_gedFileSpec, CFile::modeRead );

	str.Format( L"%s fájl beolvasása folyik...", theApp.m_gedFileName );
	CProgressWnd wndP( NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, (int)file_ged.GetLength() );

	int cnt = 0;
	while( file_ged.ReadString( cLine ) )
	{
		++cnt;
		cLine.Trim();
		if( cLine.IsEmpty() ) continue;
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM  ) cLine = Utf8ToAnsi( cLine );
		extract( cLine, &gl );

		for( int i = 0; i < sizeof( tags )/sizeof( GEDTAGS ); ++ i )
		{
			if( gl.tag == tags[i].name )
			{
				if( tags[i].foo != NULL )
				{
					tags[i].foo( gl.level, gl.xref, gl.tag, gl.value );
				}
				break;
			}
		}
	}
	wndP.DestroyWindow();
	file_ged.Close();
	fclose( theApp.fl );
	theApp.showFile( fileSpec );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void indi( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"\n%d\n%d %s %s\n", cnt, level, tag, value );
	++cnt;
}
void name( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
}
void cont( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
}
void surn( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
}
void givn( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
}
void abbr( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
}
void birt( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
}
void deat( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
}

void addr( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void adr1( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void adr2( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void date( int level, CString xref, CString tag, CString value )
{
	CString date2;

	date2 = date2date( value );
	fwprintf( theApp.fl, L"%d %s %s %s\n", level, tag, value, date2 );
};
void fams( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void time( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void titl( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void form( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void obje( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void npfx( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void sex( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void file( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void sour( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void note( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void plac( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void famc( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void chan( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void head( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void subm( int level, CString xref, CString tag, CString value )
{
	if( level==0 )
		fwprintf( theApp.fl, L"\n" );
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void gedc( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void vers( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void charx( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};

void fam( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"\n%d %s %s\n", level, tag, value );
};

void husb( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void wife( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void chil( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};
void marr( int level, CString xref, CString tag, CString value )
{
	fwprintf( theApp.fl, L"%d %s %s\n", level, tag, value );
};