#include "stdafx.h"
#include "Fa.h"
#include "afxdialogex.h"
#include <algorithm>
#include "GA_ascendants.h"
#include <math.h>
#include <cmath>



bool sortByGenAndId(const FELMENOK &v1, const FELMENOK &v2);
bool sortById(const FELMENOK &v1, const FELMENOK &v2);
bool sortByOrig(const FELMENOK &v1, const FELMENOK &v2);
bool sortBy2(const FELMENOK &v1, const FELMENOK &v2) ;
bool sortBy3(const FELMENOK &v1, const FELMENOK &v2);
bool sortBySex(const FELMENOK &v1, const FELMENOK &v2) ;
bool sortByNameR(const SAME_A &v1, const SAME_A &v2) ;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CGaAscendants::fillVfel( int gMax )
{
	FELMENOK	fel;
	_int64	kMin;
	_int64	kMax;
	_int64	maxK;
	_int64	k;
	_int64 kFather;			// egy generációban a legnagyobb férfiági Kekule szám
	int		peoplePerG;
	int		ix = 0;			// a keresett ember szõlõjét tartalmazó vFel index
	CString rowid;
	CString father_id;
	CString mother_id;
	CString parent_id;
	CString name;
	BOOL kell;
	int i;


	m_generation_previous	= -1;
	m_maxK = 0;
	maxK = (_int64)pow( 2, gMax ) -1 ;


	vFel.clear();

	fel.orig		= 1;
	fel.id			= L"";
	fel.rowid		= m_rowid;
	fel.generation	= 0;
	fel.name		= m_name;
	fel.sex_id		= m_sex_id;
	fel.kekule		= 1;
	vFel.push_back( fel );

	CProgressWnd wndProgress(NULL,L"Felmenõk összegyûjtése...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, gMax );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);
	
	SAME_A same;

	_int64 kW;
	vSame.clear();
	for( int g = 1; g <= gMax; ++g )
	{
		str.Format( L"%d generáció gyûjtése...", g );
#ifndef _DEBUG
		wndProgress.SetText( str );
#endif


		peoplePerG = 0;
		m_maxG = g;
		kMin = (_int64) pow( 2, g );
		kMax = (_int64) pow( 2, g + 1 ) - 1;

		kFather = kMin + (kMax - kMin )/2;
		for( k = kMin; k <= kMax; k += 2 )
		{
			// megkerssük a k kekule számú ember gyerekét a vFel vektorban
			for( i = 0; i < vFel.size(); ++i )		// a k Kekulé számú ember gyerekének azonosítása a vFel vektorban.
			{										// azért szükséges, hogy a father_id, mother_id-t meghatározzuk és
				if( vFel.at(i).kekule == k/2 )		// ezzel azonosítsuk az apát és anyát. Azért kell keresni, mert a Vfel index
				{									// ismertelen felmenõk miatt különbözhet k/2 tõl.
					ix = k/2 - 1;
					ix = i;
					break;
				}
			}
			if( k != 1 && i == vFel.size() )		// 0 enerációnak vagy nemlétezõ gerekkel nem foglalkozunk
				continue;

			rowid = vFel.at(ix).rowid;
			m_command.Format( L"SELECT father_id, mother_id FROM people WHERE rowid='%s'", rowid );
			if( !query( m_command ) ) return;

			for( int i = 0; i < 2; ++i )
			{
				parent_id =m_recordset.GetFieldString( i );
				if( parent_id.IsEmpty() || parent_id == L"0" || _wtoi(parent_id ) > FATHERID ) 
					continue;		// nincs apa vagy anya

				// ha egy ember többször szerepel, azokat kigyûjti vSame-be
				for( int j = 0; j < vFel.size(); ++j )
				{
					if( vFel.at(j).rowid == parent_id )
					{
						kW = k;
						if( vFel.at(j).sex_id == WOMAN && isEven( k) )
							kW = k+1;

//						fwprintf( theApp.fh1, L"%8s %10I64d %10I64d %s<br>", parent_id, vFel.at(j).kekule, k, vFel.at(j).name );
						same.rowid = parent_id;
						same.k1		= vFel.at(j).kekule;
						same.k2		= kW;
						same.name	= vFel.at(j).name;
						vSame.push_back( same );
					}
				}

				name = getName( parent_id );							// emberük szülõjét elteszi
				fel.orig		= vFel.size() + 1;
				fel.id			= rowid;
				fel.rowid		= parent_id;
				fel.spouse_id	= m_recordset.GetFieldString( 1 );
				fel.generation	= g;
				fel.name		= name;
				fel.sex_id		= i + 1;
				fel.kekule		= k + i;
				vFel.push_back( fel );
				
				if( k <= kFather )
				{
					m_fatherG = g;
					m_fatherK = k;
				}
				else
				{
					m_motherG = g;
					m_motherK = k;
				}

				++peoplePerG;
				if( k > m_maxK ) m_maxK = k;					// kekule szám maximumát számolja
			}
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
		if( peoplePerG == 0 ) break;  // a generációban nincs egyetlen ember sem! Vége!
	}
	printVsame();
	wndProgress.DestroyWindow();
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CGaAscendants::printVsame()
{
	if( !vSame.size() ) return;
	
	std::sort( vSame.begin(), vSame.end(), sortByNameR );			// sorrend: név és rowid szerint rendezés

	fwprintf( theApp.fh1, L"<pre>\n" );
	fwprintf( theApp.fh1, L"Az alábbi emberek többször szerepelnek a családfában. A \"rowid név\" alatt a kekule számok vannak.<br><br>" );
	fwprintf( theApp.fh1, L"%10s %s<br>", L"rowid", L"név" );
	
	CString rowid = L"";
	
	_int64 kP = -1;
	for( int i = 0; i < vSame.size(); ++i )
	{
		if( rowid != vSame.at(i).rowid )
		{
			if( vK.size() )
			{
				std::sort( vK.begin(), vK.end(), sortByK );
				for( int j = 0; j < vK.size(); ++j )
				{
					if( vK.at(j) != kP )
					{
						fwprintf( theApp.fh1, L"%10I64d<br>", vK.at(j) );
						kP = vK.at(j);
					}
				}
			}
			vK.clear();
			fwprintf( theApp.fh1, L"<br>" );
		}
		rowid = vSame.at(i).rowid;
		fwprintf( theApp.fh1, L"%10s %s<br>", vSame.at(i).rowid, vSame.at(i).name );
		
		vK.push_back( vSame.at(i).k1 );
		vK.push_back( vSame.at(i).k2 );
		for( ++i ; i < vSame.size(); ++i )
		{
			if( rowid == vSame.at(i).rowid )
			{
				vK.push_back( vSame.at(i).k1 );
				vK.push_back( vSame.at(i).k2 );
			}
			else
			{
				--i;
				break;
			}
		}

	}
	fwprintf( theApp.fh1, L"</pre>" );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CGaAscendants::printAscendants( CString rowid )
{
	switch( m_lista )
	{
	case COUPLES:
		print0( rowid );		// házaspárok páronkánt
		break;
	case APAIANYAI:
		print1( rowid );		// apai-anyai ágak
		break;
	case APAI:
		print2( rowid );
		break;
	case ANYAI:
		print3( rowid );
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A kekule számok folyamtosan nõvekszenek
void CGaAscendants::print0( CString rowid )
{
	CString	people;
	CString	ss;
	int		gen;

	_int64	kekule;
	_int64	k = 0;
	int		genPrev = -1;

	_int64 kMax = (_int64)pow( 2 , m_numOfG ) -1;

//	printVfel( L"Felmenõk" );

	m_cnt_ul = 0;
	k = 0;
	for( UINT i = 0; i < vFel.size(); ++i )
	{
		if( vFel.at(i).generation <= m_numOfG )
		{
			kekule	= vFel.at(i).kekule;
			if( kekule != k )
			{
				if( m_NN )
				{
					people = L"N N";
					kekule = k;
					fwprintf( theApp.fh1,  L"%s%I64d &diams;%s<br>\n", ss, kekule, people );
					--i;  // ennek az embernek a kiírását próbálja megismételni, ha sorra kerül
					++k;
					continue;
				}
			}
			gen = vFel.at(i).generation;
			ss = indent( gen, genPrev );
			genPrev = gen;

			people = getPeopleString( i );
			fwprintf( theApp.fh1,  L"%s%I64d &diams;%s<br>\n", ss, kekule, people );
			++k;
		}
	}

	if( m_NN && k < kMax )
	{
		for( ; k <= kMax; ++k )
		{
			fwprintf( theApp.fh1,  L"%s%I64d &diams;%s<br>\n", ss, k, L"N N" );
		}
	}
	for( int i =0; i < m_cnt_ul; ++i )
		fwprintf( theApp.fh1, L"</ul>\n" );
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::print1( CString rowid )
{
	CString people;
	CString ss;
	int		gPrev = -1;
	BOOL	volt;
	int		g;
	_int64	k;

	fillVKekule( 1 );
//	printKekule( L"Apai-anyai ág" );
//	printVfel( L"vFel" );

	CProgressWnd wndProgress(NULL,L"Felmenõk listázása...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, vKekule.size() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	m_cnt_ul = 0;
	for( UINT i = 0; i < vKekule.size(); ++i )
	{
		volt = FALSE;
		g = vKekule.at(i).g;
		k = vKekule.at(i).k;

		for( UINT j = 0; j < vFel.size(); ++j )
		{
			if( vFel.at(j).kekule == k )
			{
				ss	= indent( g, gPrev );
				gPrev	= g;

				people = getPeopleString( j );
				fwprintf( theApp.fh1,  L"%s%I64d &diams;%s <div style=display:none>%s</div><br>\n", ss, k, people, vFel.at(j).rowid );
				volt = TRUE;
				break;
			}
		}
		if( !volt && m_NN )
		{
			ss	= indent( g, gPrev );
			gPrev	= g;
			people = L"N N";
			fwprintf( theApp.fh1,  L"%s%I64d &diams;%s<br>\n", ss, k, people );
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	for( int i =0; i < m_cnt_ul; ++i )
		fwprintf( theApp.fh1, L"</ul>\n" );
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::print2( CString rowid )
{
	CString people;
	CString ss;
	int		gPrev = -1;
	BOOL	volt;
	int		g;
	_int64	k;

	fillVKekule( 2 );


	CProgressWnd wndProgress(NULL,L"Felmenõk listázása...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, vKekule.size() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	m_cnt_ul = 0;
	for( UINT i = 0; i < vKekule.size(); ++i )
	{
		volt = FALSE;
		g = vKekule.at(i).g;
		k = vKekule.at(i).k;

		for( UINT j = 0; j < vFel.size(); ++j )
		{
			if( vFel.at(j).kekule == k )
			{
				ss	= indent( g, gPrev );
				gPrev	= g;

				people = getPeopleString( j );
				fwprintf( theApp.fh1,  L"%s%I64d &diams;%s<br>\n", ss, k, people );
				volt = TRUE;
				break;
			}
		}
		if( !volt && m_NN )
		{
			ss	= indent( g, gPrev );
			gPrev	= g;
			people = L"N N";
			fwprintf( theApp.fh1,  L"%s%I64d &diams;%s<br>\n", ss, k, people );
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	for( int i =0; i < m_cnt_ul; ++i )
		fwprintf( theApp.fh1, L"</ul>\n" );
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::print3( CString rowid )
{
	CString people;
	CString ss;
	int		gPrev = -1;
	BOOL	volt;
	int		g;
	_int64	k;

	fillVKekule( 3 );
//	printKekule( L"Apai-anyai ág" );
//	printVfel( L"vFel" );

	CProgressWnd wndProgress(NULL,L"Felmenõk listázása...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, vKekule.size() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	m_cnt_ul = 0;
	for( UINT i = 0; i < vKekule.size(); ++i )
	{
		volt = FALSE;
		g = vKekule.at(i).g;
		k = vKekule.at(i).k;

		for( UINT j = 0; j < vFel.size(); ++j )
		{
			if( vFel.at(j).kekule == k )
			{
				ss	= indent( g, gPrev );
				gPrev	= g;

				people = getPeopleString( j );
				fwprintf( theApp.fh1,  L"%s%I64d &diams;%s<br>\n", ss, k, people );
				volt = TRUE;
				break;
			}
		}
		if( !volt && m_NN )
		{
			ss	= indent( g, gPrev );
			gPrev	= g;
			people = L"N N";
			fwprintf( theApp.fh1,  L"%s%I64d &diams;%s<br>\n", ss, k, people );
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	for( int i =0; i < m_cnt_ul; ++i )
		fwprintf( theApp.fh1, L"</ul>\n" );
	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBy3(const FELMENOK &v1, const FELMENOK &v2) 
{ 
	if( v1.generation == v2.generation ) 
		return( v1.orig < v2.orig ); 
	return( (v1.generation < v2.generation ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBy2(const FELMENOK &v1, const FELMENOK &v2) 
{ 
	if( v1.generation == v2.generation ) 
		return( v1.id < v2.id ); 
	return( (v1.generation < v2.generation ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySex(const FELMENOK &v1, const FELMENOK &v2) 
{ 
	if( v1.id == v2.id ) 
		return( v1.sex_id < v2.sex_id ); 
	return( (v1.id < v2.id ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByGenAndId(const FELMENOK &v1, const FELMENOK &v2) 
{ 
	return( v1.generation < v2.generation ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortById(const FELMENOK &v1, const FELMENOK &v2) 
{ 
    return( v1.id < v2.id ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByOrig(const FELMENOK &v1, const FELMENOK &v2) 
{ 
    return( v1.orig < v2.orig ); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_int64 CGaAscendants::getMaxK()
{
	_int64 maxK = 0;

	for( UINT i = 0; i < vFel.size(); ++i )
	{
		if( vFel.at(i).kekule > maxK )
			maxK = vFel.at(i).kekule;
	}
	return maxK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_int64 CGaAscendants::getFatherK()
{
	_int64 fatherK = 0;

	for( UINT i = 0; i < vFel.size(); ++i )
	{
		if( vFel.at(i).kekule > fatherK )
			fatherK = vFel.at(i).kekule;
		else
			break;
	}
	return fatherK;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendants::query1( CString command )
{
	if( m_recordset1.Query(command,theApp.m_sqliteDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendants::query2( CString command )
{
	if( m_recordset2.Query(command,theApp.m_sqliteDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Létrehgozza a vKekule vektort, ami tartralmazza az adott generációszámig az
// apai+anyai, apai vagy anyai ágakban elõforduéó összes kekule számot a kiírás sorrendjeben.
// Ez fogja meghatározni, hogy a vFel vektorban összegyûjtött emberek közül kit és milyen sorrendben ír ki a printAscendats.
//
//	gk.g	generációk száma
//	gk.k	kekule szám
//	gl.w	1: ennek az embernek a felesége is benne van már a vKekule vektorban
//			A legnagyobb generációból történõ visszalépésekhez szükséges
void CGaAscendants::fillVKekule( int ag )
{
	KEKULE gk;
	_int64 i;

	_int64	K;
	int		G;
//	int		ix;

	_int64	KMAX = (_int64)pow( (double)2, (double)m_numOfG ) - 1;

	vKekule.clear();

	gk.g	= 1;											//
	gk.k	= 1;
	gk.w	= 0;
	vKekule.push_back( gk );

	if( ag == 1 || ag == 2 )								// apai generációval folytatja
	{
		gk.g	= 2;
		gk.k	= 2;
		gk.w	= 0;
		vKekule.push_back( gk );

		G = 3;
		K = 4;
	}
	else													// anyai generációval kezdi
	{
		gk.g	= 2;
		gk.k	= 3;
		gk.w	= 0;
		vKekule.push_back( gk );

		G = 3;
		K = 6;
	}
	
	while( K <= KMAX )
	{
		gk.g = G;
		gk.k = K;
		gk.w = 0;
		vKekule.push_back( gk );
		
		if( K == KMAX ) break;

		if( isOdd( K ) )									// nõt írtunk ki
		{
			if( G == m_numOfG )						// az utolsó generációban, akkor 1-el vagy többel  vissza
			{
															// megkeressük az elsõ férfit, akinek a felesége még nem volt kiírva.
				for( i = vKekule.size() - 1; 0 <= i; --i )	// azt kell kiírnunk.
				{
					if( isEven( vKekule.at(i).k ) && vKekule.at(i).w == 0 )
					{
						G = vKekule.at(i).g;
						K = vKekule.at(i).k + 1;
						vKekule.at(i).w = 1;
						break;
					}
				}
				if( i == 0 ) break;							// minden feleség ki lett már írva!! Vége.
				if( ag == 2 && G == 2 ) break;				// apai ág vége
			}
			else											// még nem az utolsó generációban vagyunk
			{
				G = G + 1;									// a következõ generációban
				K = K * 2;									// a nõ apját írjuk ki
			}
		}
		else												// férfit írtunk ki
		{
			if( G < m_numOfG )						// még van további generáció
			{
				G = G + 1;
				K = K * 2;									// következik: következõ generációban az apa
			}
			else											// nincs több generáció
			{
				vKekule.at( vKekule.size()- 1 ).w	= 1;	// a feleségét kiírja!
				K = K + 1;									// következik feleség ugyanabban a generációban
			}
		}
	}
//	printKekule(L"");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Létrehgozza a vKekule vektort, ami tartralmazza az adott generációszámig az
// apai+anyai, apai vagy anyai ágakban elõforduéó összes kekule számot a kiírás sorrendjeben.
// Ez fogja meghatározni, hogy a vFel vektorban összegyûjtött emberek közül kit és milyen sorrendben ír ki a printAscendats.
//
//	gk.g	generációk száma
//	gk.k	kekule szám
//	gl.w	1: ennek az embernek a felesége is benne van már a vKekule vektorban
//			A legnagyobb generációból történõ visszalépésekhez szükséges
void CGaAscendants::structure()
{
	ASCENDANTS gk;

	_int64 i;
	_int64	K = 1;
	int		G = 1;
	int		ix;
	BOOL apaiAg = true;

	m_numOfFG = -1;
	m_numOfMG = -1;

	vA.clear();

	getPeople( m_rowid );

	gk.g	= G;
	gk.k	= K;
	gk.w	= 0;
	gk.name			= m_name;
	gk.rowid		= m_rowid;
	gk.sex_id		= m_sex;
	gk.father_id	= m_father_id;
	gk.mother_id	= m_mother_id;
	m_spouse_id		= m_mother_id;
	vA.push_back( gk );

	++G;
	++K;


//	_int64 KMAX = 9223372036854775807;

	int GMAX	= 21;
	_int64 KMAX	= (_int64)pow( 2, GMAX ) -1;

	fwprintf( theApp.fh1, L"<pre><br>\n" );
	CProgressWnd wndProgress(NULL,L"Felmenõk összegyûjtése...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, (int)KMAX );
	wndProgress.SetPos(0);
	wndProgress.SetStep( 1 );
	while( K < KMAX )
	{
		if( isEven( K ) )
			getPeople( m_father_id );
		else
			getPeople( m_mother_id );
	
		
		if( !m_rowid.IsEmpty() )			// csak az ismert felmenõt gyûjti be!!
		{
			if( _wtoi(m_father_id) > FATHERID ) m_father_id.Empty();
			gk.g			= G;
			gk.k			= K;
			gk.w			= 0;
			gk.name			= m_name;
			gk.rowid		= m_rowid;
			gk.sex_id		= m_sex;
			gk.father_id	= m_father_id;
			gk.mother_id	= m_mother_id;
			gk.spouse_id	= m_spouse_id;

			m_spouse_id		= m_mother_id;
			vA.push_back( gk );
		
			fwprintf( theApp.fh1, L"%4d %20I64d %9s %9s %9s %9s %3s %s<br>\n",\
			G,\
			K,\
			m_rowid,\
			m_father_id,\
			m_mother_id,\
			m_spouse_id,\
			m_sex,\
			m_name  ); 
			fflush( theApp.fh1 );

			if( !m_rowid.IsEmpty() )
			{
				if( apaiAg )
					m_numOfFG = __max( G, m_numOfFG );
				else
					m_numOfMG = __max( G, m_numOfMG );
			}
		}

		if( m_rowid == L"143568" )
			int z = 1;
		if( isOdd( K ) )									// nõt gyûjtöttünk be vagy szerettünk volna begyûjteni
		{
			if( m_rowid.IsEmpty() )							// nem volt felmenõ
			{
															// megkeressük az elsõ férfit, akinek a felesége még nem volt kiírva.
				for( i = vA.size() - 1; 0 <= i; --i )		// azt kell kiírnunk.
				{
					if( isEven( vA.at(i).k ) && vA.at(i).w == 0 )
					{
						G = vA.at(i).g;
						K = vA.at(i).k + 1;
						vA.at(i).w = 1;
						m_mother_id = vA.at(i).spouse_id;
						break;
					}
				}
				if( i == -1 ) break;							// minden feleség ki lett már írva!! Vége.

				if( K == 3 )
				{
					 apaiAg = false;
				}
			}
			else											// volt ismert feleség
			{
				G = G + 1;									// a következõ generációban
				K = K * 2;									// a nõ apját írjuk ki
			}
		}
		else												// férfit írtunk ki
		{
			if( m_rowid.IsEmpty() )							// nem találtunk ismmert felmenõt
			{
				vA.at( vA.size()- 1 ).w	= 1;				// a feleségét kiírja!
				K = K + 1;									// következik feleség ugyanabban a generációban ( ha van )
				m_mother_id = vA.at( vA.size()-1).spouse_id;
				m_mother_id = vA.at( vA.size()-1).mother_id;
			}
			else											// találtunk ismert apát
			{
				G = G + 1;									// a következõ generációban
				K = K * 2;									// keressük az apját
			}
		}
		if( K == KMAX )
			int u = 6;
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	printVA( L"vA" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::printVfel( CString title )
{
	fwprintf( theApp.fh1, L"<br>\n<pre>" );
	fwprintf( theApp.fh1, L"%s<br><br>", title );

	fwprintf( theApp.fh1, L"%4s" L"%4s %14s %8s %8s %8s %3s %s\n", L"orig", L"gen", L"kekule", L"gyerek", L"rowid", L"feleség", L"sex", L"név" );
		
	for( UINT i = 0; i < vFel.size(); ++i )
	{
		fwprintf( theApp.fh1, L"%4d %4d %14I64d %8s %8s %8s %3d %s<br>", vFel.at(i).orig, vFel.at(i).generation, vFel.at(i).kekule,  vFel.at(i).id, vFel.at(i).rowid, vFel.at(i).spouse_id, vFel.at(i).sex_id, vFel.at(i).name ); 
	}
	fwprintf( theApp.fh1, L"</pre>" );
	fflush( theApp.fh1 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::printKekule( CString title )
{
	fwprintf( theApp.fh1, L"<br>\n<pre>" );
	fwprintf( theApp.fh1, L"%s\n\n", title );
	for( _int64 i = 0; i < vKekule.size(); ++i )
	{
		fwprintf( theApp.fh1, L"%4d. %4d %12d %d\n", i+1, vKekule.at(i).g, vKekule.at(i).k, vKekule.at(i).w );
	}
	fwprintf( theApp.fh1, L"</pre>" );
	fflush( theApp.fh1 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Létrehgozza a vKekule vektort, ami tartralmazza az adott generációszámig az
// apai+anyai, apai vagy anyai ágakban elõforduéó összes kekule számot a kiírás sorrendjeben.
// Ez fogja meghatározni, hogy a vFel vektorban összegyûjtött emberek közül kit és milyen sorrendben ír ki a printAscendats.
//
//	gk.g	generációk száma
//	gk.k	kekule szám
//	gl.w	1: ennek az embernek a felesége is benne van már a vKekule vektorban
//			A legnagyobb generációból történõ visszalépésekhez szükséges
//
// Ez abban különbözik a findAscendants-tõl, hogy az ismeretlen embereknek is csinál egy rekordot a VA-vectorban!!
//
void CGaAscendants::fillA()
{
	ASCENDANTS gk;

	_int64 i;
	_int64	K;
	int		G;
	int		ix;
	CString rowid;
	BOOL apaiAg = true;

	_int64	KMAX = (_int64)pow( (double)2, (double)m_numOfG ) - 1;

	vA.clear();

	rowid = getPeople( m_rowid );

	gk.g	= 1;
	gk.k	= 1;
	gk.w	= 0;
	gk.name			= m_name;
	gk.rowid		= rowid;
	gk.sex_id		= m_sex;
	gk.father_id	= m_father_id;
	gk.mother_id	= m_mother_id;

	m_spouse_id = m_mother_id;
	vA.push_back( gk );

	m_numOfFG = -1;
	m_numOfMG = -1;

	if( m_lista == COUPLES || m_lista == APAIANYAI || m_lista == APAI )								// apai generációval folytatja
	{
		rowid = getPeople( m_father_id );

		gk.g	= 2;
		gk.k	= 2;
		gk.w	= 0;
		gk.name			= m_name;
		gk.rowid		= rowid;
		gk.sex_id		= m_sex;
		gk.father_id	= m_father_id;
		gk.mother_id	= m_mother_id;
		gk.spouse_id	= m_spouse_id;
		vA.push_back( gk );

		m_spouse_id		= m_mother_id;
		G = 3;
		K = 4;
	}
	else													// anyai generációval kezdi
	{
		rowid = getPeople( m_mother_id );
		gk.g	= 2;
		gk.k	= 3;
		gk.w	= 0;
		gk.name			= m_name;
		gk.rowid		= rowid;
		gk.sex_id		= m_sex;
		gk.father_id	= m_father_id;
		gk.mother_id	= m_mother_id;
		vA.push_back( gk );
		m_spouse_id = m_mother_id;

		G = 3;
		K = 6;
	}
	
	CProgressWnd wndProgress(NULL,L"Felmenõk összegyûjtése...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, (int)KMAX );
	wndProgress.SetPos(0);
	wndProgress.SetStep( 1 );
	

	while( K <= KMAX )
	{
		if( isEven( K ) )
			rowid = getPeople( m_father_id );
		else
			rowid = getPeople( m_mother_id );
	
		gk.g			= G;
		gk.k			= K;
		gk.w			= 0;
		gk.name			= m_name;
		gk.rowid		= rowid;
		gk.sex_id		= m_sex;
		gk.father_id	= m_father_id;
		gk.mother_id	= m_mother_id;
		gk.spouse_id	= m_spouse_id;

		m_spouse_id		= m_mother_id;
		vA.push_back( gk );

		if( !rowid.IsEmpty() )
		{
			++m_numOfA;
			if( apaiAg )
			{
				m_numOfFG = __max( G, m_numOfFG );
				++m_numOfFA;
			}
			else
			{
				m_numOfMG = __max( G, m_numOfMG );
				++m_numOfMA;
			}
		}

		if( K == KMAX ) break;

		if( isOdd( K ) )									// nõt írtunk ki
		{
			for( int j = vA.size()-1; j >= 0; --j )
			{
				if( vA.at(j).k	== K - 1 )
				{
					vA.at(j).w = 1;		// a férjénél (ha van ) bejegyezzük, hogy már vizsgáltuk a feleségét
					break;
				}
			}

			if( G == m_numOfG )						// az utolsó generációban, akkor 1-el vagy többel  vissza
			{
															// megkeressük az elsõ férfit, akinek a felesége még nem volt kiírva.
				for( i = vA.size() - 1; 0 <= i; --i )		// azt kell kiírnunk.
				{
					if( isEven( vA.at(i).k ) && vA.at(i).w == 0 )
					{
						G = vA.at(i).g;
						K = vA.at(i).k + 1;
						vA.at(i).w = 1;
						m_mother_id = vA.at(i).spouse_id;
						break;
					}
				}
				if( i == -1 ) break;							// minden feleség ki lett már írva!! Vége.

				if( K == 3 )
				{
					 apaiAg = false;
					if( m_lista == APAI ) break;					// apai ág vége
				}
			}
			else											// még nem az utolsó generációban vagyunk
			{
				G = G + 1;									// a következõ generációban
				K = K * 2;									// a nõ apját írjuk ki
			}
		}
		else												// férfit írtunk ki
		{
			if( G < m_numOfG )						// még van további generáció
			{
				G = G + 1;
				K = K * 2;									// következik: következõ generációban az apa
			}
			else											// nincs több generáció
			{
				vA.at( vA.size()- 1 ).w	= 1;				// a feleségét kiírja!
				K = K + 1;									// következik feleség ugyanabban a generációban ( ha van )
				m_mother_id = vA.at( vA.size()-1).mother_id;
			}
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
//	printVA();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByNameR(const SAME_A &v1, const SAME_A &v2) 
{ 
	if( v1.name == v2.name ) 
		return( v1.rowid < v2.rowid ); 
	return( (v1.name < v2.name ) );
}

