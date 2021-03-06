#include "stdafx.h"
#include "Dragon.h"
#include "gedcomIn.h"
#include "GAtoDB.h"
#include <algorithm>
#include "ProgressWnd.h"
#include "utilities.h"

// Nem �rtelmezett TAG-ek list�j�t k�sz�ti el
void CGedcomIn::checkTags()
{
	CString fileName( L"TAGS" );
	CString fileSpec;
	

	fileSpec = theApp.openTextFile( &theApp.fl, fileName, L"w+" );
		
	theApp.m_inputCode = GetInputCode( theApp.m_gedFileSpec );
	
	file_ged.Open( theApp.m_gedFileSpec, CFile::modeRead );

	fwprintf( theApp. fl, L"%s\n\n", theApp.m_gedFileSpec );
	fwprintf( theApp. fl, L"%s\n\n", L"Nem �rtelmezett tag-ek" );

	str.Format( L"%s f�jl beolvas�sa folyik...", theApp.m_gedFileName );
	CProgressWnd wndP( NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, (int)file_ged.GetLength() );

	CString cLine;
	CString tag;
	CString args;

	BOOL VAN = FALSE;
	GEDLINE gl;

	int cnt = 0;
	v_indi.clear();
	while( file_ged.ReadString( cLine ) )
	{
		++cnt;
		cLine.Trim();
		if( cLine.IsEmpty() ) continue;
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM  ) cLine = Utf8ToAnsi( cLine );
		cLine.Replace( '\'', '|' );
		extract( cLine, &gl );


		if( gl.tag.Left(1) == L"_" ) continue;
		for( int i = 0; i < sizeof( tags )/sizeof( GEDTAGS ); ++ i )
		{
			if( gl.tag == tags[i].name )
			{
				VAN = TRUE;
				break;
			}
		}
		if( !VAN )
			fwprintf( theApp.fl, L"%5d %s\n", cnt, cLine );
		VAN  = FALSE;


	}
	
	wndP.DestroyWindow();
	file_ged.Close();
	fclose( theApp.fl );
	theApp.showFile( fileSpec );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::zeroLevel()
{
	CString fileName(L"zerolevel");
	CString fileSpec;
	fileSpec = theApp.openTextFile( &theApp.fl, fileName, L"w+" );
	fwprintf( theApp. fl, L"%s\n\n", theApp.m_gedFileSpec );
	fwprintf( theApp. fl, L"%s\n\n", L"Zero level list�ja" );

	theApp.m_inputCode = GetInputCode( theApp.m_gedFileSpec );

	file_ged.Open( theApp.m_gedFileSpec, CFile::modeRead );

	CString cLine;
	GEDLINE lxtv;
	
	while( file_ged.ReadString( cLine ) )
	{
		cLine.Replace( '\'', '|' );
		extract( cLine, &lxtv );
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
		if( lxtv.tag.GetAt(0) == '_' ) continue;
		if( lxtv.level == 0 )
			fwprintf( theApp.fl, L"%s\n", cLine );
	}
	file_ged.Close();
	fclose( theApp.fl );
	theApp.showFile( fileSpec );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::listINDI()
{
	listZeroRecord( L"INDI" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::listFAM()
{
	listZeroRecord( L"FAM" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::listHEAD()
{
	listZeroRecord( L"HEAD" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::indiFams()
{
	listIndiFams();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::listIndiFams()
{
	fillVectors();

	CString fileName( L"indiFams" );
	CString fileSpec;
	CString name1;
	CString name2;
	CString refSpouse;
	int		total;
	fileSpec = theApp.openTextFile( &theApp.fl, fileName, L"w+" );
	fwprintf( theApp. fl, L"  %s\n\n", theApp.m_gedFileSpec );
	
	std::sort( v_fams.begin(), v_fams.end(), sortSexRefI ); // refH szerint rendezi: a f�rj feles�geivel k�t�tt h�zass�gai egym�s ut�n lesznek

	fwprintf( theApp.fl, L"%20s %2s %20s\n", L"h�zast�rs", L"m", L"csal�d" ); 
	for( UINT i= 0; i < v_fams.size(); ++i )
	{
		name1 = getIndi( v_fams.at(i).refI );
		total = getMarriageAll( v_fams.at(i).refI );
		refSpouse = getRefSpouse( v_fams.at(i).sex, v_fams.at(i).refF );
		name2 = getIndi( refSpouse );
		str.Format( L"%s %20s %2d/%2d %20s %20s %20s", v_fams.at(i).sex, v_fams.at(i).refI, v_fams.at(i).order, total,  v_fams.at(i).refF, name1, name2 );
		fwprintf( theApp.fl, L"%s\n", str );
	}
	fclose( theApp.fl );
	theApp.showFile( fileSpec );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGedcomIn::getRefSpouse( CString sex, CString refF ) 
{
	CString ref;
	for( int i = 0; i < v_fam.size(); ++i )
	{
		if( v_fam.at(i).refF == refF )
		{
			if( sex == L"1" )
				ref = v_fam.at(i).refW;
			else
				ref =v_fam.at(i).refH;
			break;
		}
	}
	return( ref );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::listPeople()
{
	fillVectors();

	CString fileName( L"peopleList" );
	CString fileSpec;
	fileSpec = theApp.openTextFile( &theApp.fl, fileName, L"w+" );
	fwprintf( theApp. fl, L"%s\n\n", theApp.m_gedFileSpec );
	CString name;

	fwprintf( theApp.fl, L"1. az ember azonos�t�ja\n" );
	fwprintf( theApp.fl, L"2. Neve\n" );
	fwprintf( theApp.fl, L"3. Apja hanyadik gyermeke\n" );
	fwprintf( theApp.fl, L"4. Anyja hanyadik gyermeke\n" );
	fwprintf( theApp.fl, L"5. /Apja hanyadik feles�g�nek gyermeke\n" );
	fwprintf( theApp.fl, L"6. /Apja hanyadik feles�g�nek gyermeke\n" );
	fwprintf( theApp.fl, L"7. Anyja hanyadik gyermeke\n" );
	fwprintf( theApp.fl, L"8. H�zast�rsainak sz�ma\n\n\n" );

	for( UINT i= 0; i < v_indi.size(); ++i )
	{
		name.Format( L"%s %s", v_indi.at(i).last_name, v_indi.at(i).first_name );
	
		fwprintf( theApp.fl, L"%-10s %-30s %2d %2d /%d /%d %2d %-10s %-15s %-10s %-15s %-30s %s\n",
v_indi.at(i).refI,\
name,\
v_indi.at(i).orderFather,\
v_indi.at(i).orderMother,\
v_indi.at(i).parentIndex,\
v_indi.at(i).parentIndexCalc,\
v_indi.at(i).numOfSpouses,\
v_indi.at(i).birth_place,\
v_indi.at(i).birth_date,\
v_indi.at(i).death_place,\
v_indi.at(i).death_date,\
v_indi.at(i).comment,\
v_indi.at(i).occupation\
);
	}
	v_indi.clear();
	fclose( theApp.fl );
	theApp.showFile( fileSpec );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::listFamiliesByName()
{
	CString info = L"\
A csal�d els� sora: csal�dindex, a f�rj �s a feles�g neve valamint\n\
X/Y a f�rj h�zass�g�nak sorsz�ma/�sszes h�zass�ga,\n\
X/Y a fels�g h�zass�g�nak sorsz�ma/�sszes h�zass�ga,\n\
A k�vetkez� sorokban a gyerekek nevei vannak, �s ha az ap�nak t�bb feles�ge volt, akkor az anya index�vel."; 

	fillVectors();

	int indexH;
	int indexW;
	CString husband;
	CString wife;
	
	CString refF;
	CString refH;
	CString refW;
	CString refC;
	
//	int		numOfSpousesH = 0;
//	int		numOfSpousesW = 0;

	CString fileName( L"familiesByName" );
	CString fileSpec;
	fileSpec = theApp.openTextFile( &theApp.fl, fileName, L"w+" );
	fwprintf( theApp.fl, L"%20s\n\n", L"CSAL�DOK A GEDCOM F�JLBAN" );
	fwprintf( theApp. fl, L"GEDCOM f�jl: %s\n", theApp.m_gedFileSpec );
	fwprintf( theApp. fl, L"D�tum	   : %s\n\n", theApp.getPresentDate() );
	fwprintf( theApp.fl, L"%s\n\n", info );

	fwprintf( theApp.fl, L"1. csal�d azonos�t�\n" );
	fwprintf( theApp.fl, L"2. f�rj azobos�t�\n" );
	fwprintf( theApp.fl, L"3. f�rj neve\n" );
	fwprintf( theApp.fl, L"4. feles�g azonos�t�\n" );
	fwprintf( theApp.fl, L"5. felas�g neve\n" );
	fwprintf( theApp.fl, L"6. f�rj h�zass�g�nak sorsz�ma/�sszes h�zass�ga\n" );
	fwprintf( theApp.fl, L"7. feles�g h�zass�gainak sz�am/�sszes h�zass�ga\n\n" );


	fwprintf( theApp.fl, L"Gyerekekn�l:\n" );
	fwprintf( theApp.fl, L"1. Apja hanyadik gyermeke\n" );
	fwprintf( theApp.fl, L"2. Anyja hanyadik gyermeke\n" );
	fwprintf( theApp.fl, L"3. gyerek azonos�t�ja\n" );
	fwprintf( theApp.fl, L"4. gyerek neve\n" );
	fwprintf( theApp.fl, L"5. /anyja hagyadik feles�ge apj�nak\n" );
	fwprintf( theApp.fl, L"6. /anyja hagyadik feles�ge apj�nak\n\n" );;

	std::sort( v_fam.begin(), v_fam.end(), sortByCnt ); 
	for( UINT i= 0; i < v_fam.size(); ++i )
	{
		refF = v_fam.at(i).refF;
		refH = v_fam.at(i).refH;
		refW = v_fam.at(i).refW;
		
		indexH = getIndexIndi(refH );
		indexW = getIndexIndi(refW );
		 
		husband = getIndi( refH );
		wife	= getIndi( refW );

//		if( indexH != -1 ) numOfSpousesH = v_indi.at(indexH).numOfSpouses;
//		if( indexW != -1 ) numOfSpousesW = v_indi.at(indexW).numOfSpouses;

		fwprintf( theApp.fl, L"%-10s %-10s %-28s %-10s %-28s %2d/%d %2d/%d\n",
		refF,\
		refH,\
		husband,\
		refW,\
		wife,\
		v_fam.at(i).marriageH,\
		v_fam.at(i).marriageHAll,\
		v_fam.at(i).marriageW,\
		v_fam.at(i).marriageWAll\
		);

		for( UINT j = 0; j < v_chil.size(); ++j )
		{
			if( v_chil.at(j).refF == v_fam.at(i).refF )
			{
				refC	= v_chil.at(j).refC;
				if( v_fam.at(i).marriageHAll > 1 )
				{
//				if( v_chil.at(j).parentIndex != 0 )
					fwprintf( theApp.fl, L"%4d. %3d. %-10s %s/%d /%d\n",\
					v_chil.at(j).orderFather,\
					v_chil.at(j).orderMother,\
					refC,\
					getIndi( refC),\
					v_chil.at(j).parentIndex,\
					v_chil.at(j).parentIndexCalc );
				}
				else
				{
					fwprintf( theApp.fl, L"%4d. %3d. %-10s %s\n",\
					v_chil.at(j).orderFather,\
					v_chil.at(j).orderMother,\
					refC,\
					getIndi( refC ) );
				}
			}
		}
		fwprintf( theApp.fl, L"\n" );

	}
	listMultiMarriages();
//	v_indi.clear();
//	v_fam.clear();

	int indexI;
	CString refFPrev(L"");
	std::sort( v_famc.begin(), v_famc.end(), sort_refF );
	fwprintf( theApp.fl, L"\n\nindifam rekords rendezett\n\n" );
	for( UINT i = 0; i < v_famc.size(); ++i )
	{
		indexI = getIndexIndi( v_famc.at(i).refI );
		if( refFPrev != v_famc.at(i).refF ) fwprintf( theApp.fl, L"\n" );
		fwprintf( theApp.fl, L"%s %s %d\n", v_famc.at(i).refF, v_famc.at(i).refI, indexI );
		refFPrev = v_famc.at(i).refF;
	}


	fclose( theApp.fl );
	theApp.showFile( fileSpec );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomIn::listVindi()
{
	fill_v_indi();

	CString fileName( L"v_indi" );
	CString fileSpec;
	fileSpec = theApp.openTextFile( &theApp.fl, fileName, L"w+" );

	CString ref;
	CString last_name;
	CString first_name;

	fwprintf( theApp. fl, L"%s\n\n", theApp.m_gedFileSpec );
	for( UINT i = 0; i < v_indi.size(); ++i )
	{
		ref =		v_indi.at(i).refI;
		last_name	= v_indi.at(i).last_name;
		first_name	= v_indi.at(i).first_name;
		fwprintf( theApp.fl, L"%4d. %s %s %s\n", i, ref, last_name, first_name ); 
	}
	v_indi.clear();

	fclose( theApp.fl );
	theApp.showFile( fileSpec );
}
// A megadott tag-� rekordokat list�zza
void CGedcomIn::listZeroRecord( CString tag )
{
	CString fileName = tag;
	CString fileSpec;
	fileSpec = theApp.openTextFile( &theApp.fl, fileName, L"w+" );

	theApp.m_inputCode = GetInputCode( theApp.m_gedFileSpec );
	file_ged.Open( theApp.m_gedFileSpec, CFile::modeRead );

	fwprintf( theApp. fl, L"%s\n\n", theApp.m_gedFileSpec );
	
	str.Format( L"%s f�jl beolvas�sa folyik...", theApp.m_gedFileName );
	CProgressWnd wndP( NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, (int)file_ged.GetLength() );

	CString cLine;
	GEDLINE lxtv;
	int		cnt = 0;
	while( file_ged.ReadString( cLine ) )
	{
		cLine.Replace( '\'', '|' );
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
		extract( cLine, &lxtv );
		if( lxtv.tag == tag )
		{
			++cnt;
			list( cLine, cnt );
		}
	}
	wndP.DestroyWindow();
	file_ged.Close();
	fclose( theApp.fl );
	theApp.showFile( fileSpec );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A legk�zelebbi 0 level-� sorig mindent ki�r
void CGedcomIn::list( CString tag, int cnt  )
{
	GEDLINE lxtv;
	CString cLine;

	fwprintf( theApp.fl, L"%d\n", cnt );
	fwprintf( theApp.fl, L"%s\n", tag );
	while( TRUE )
	{
		fpos = file_ged.GetPosition();
		if( !file_ged.ReadString( cLine ) ) break;
		cLine.Replace( '\'', '|' );
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi( cLine );
		extract( cLine, &lxtv );
		if( lxtv.level == 0 )
		{
			fwprintf( theApp.fl, L"\n" );
			break;
		}
		fwprintf( theApp.fl, L"%s\n", cLine );
	}
	file_ged.Seek( fpos, 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a v_fam vektor elemeinek marriage �s marraigeHAll �rt�kad�s
void CGedcomIn::listMultiMarriages()
{
	std::sort( v_fam.begin(), v_fam.end(), sortByCnt );		// az�rt kell, hogy a RefW szerinti rendez�sban az azonosak sorrendje rekonstru�lhat� legyen
	std::sort( v_fam.begin(), v_fam.end(), multiSort_FAM_refH ); // refH szerint rendezi: a f�rj feles�geivel k�t�tt h�zass�gai egym�s ut�n lesznek

	CString refF;

	CString refW;
	CString refH;

	CString refW1;
	CString refW2;

	CString refH1;
	CString refH2;

	CString refHPrev(L"");
	CString refWPrev(L"");

	CString husband;
	CString wife;

	int		marriageHX;
	int		marriageHAll;
	int		marriageWX;
	int		marriageWAll;
	int		numOfChildren;


	fwprintf( theApp.fl, L"\n\n\nF�rj t�bbsz�r�s h�zass�gai\n\n\n" );
	fwprintf( theApp.fl, L"1. X/Y - a f�rj h�zass�g�nak sorsz�ma/�sszes h�zass�ga\n" ); 
	fwprintf( theApp.fl, L"2. X/Y - a feles�g h�zass�gnak sorsz�ma/�sszes h�zass�ga\n" ); 
	fwprintf( theApp.fl, L"3. sz�m: a h�zass�gban sz�letett gyerekek sz�ma\n\n" ); 
	for( UINT i = 0; i < v_fam.size()-1; ++i )
	{
		refH1 = v_fam.at(i).refH;
		refH2 = v_fam.at(i+1).refH;

		if( refH1 == refH2 )						// i-ben �s i+i-ben azonosf�rjek 
		{
			if( refHPrev != refH1 )					// nem ugyanaz a f�rj mint az el�z�, teh�t �j p�r!!
			{
				refF			= v_fam.at(i).refF;
				refW			= v_fam.at(i).refW;
				husband			= getIndi( refH1 );
				wife			= getIndi( refW );
				marriageHX		= v_fam.at(i).marriageH;
				marriageHAll	= v_fam.at(i).marriageHAll;
				marriageWX		= v_fam.at(i).marriageW;
				marriageWAll	= v_fam.at(i).marriageWAll;
				numOfChildren	= v_fam.at(i).numOfChildren;
				fwprintf( theApp.fl, L"\n%-10s %-10s %-28s %-10s %-28s %d/%d %d/%d %d\n", refF, refH1, husband, refW, wife, marriageHX, marriageHAll, marriageWX, marriageWAll, numOfChildren ); 
			}
			refF			= v_fam.at(i+1).refF;
			refW			= v_fam.at(i+1).refW;
			husband			= getIndi( refH2 );
			wife			= getIndi( refW );
			marriageHX		= v_fam.at(i+1).marriageH;
			marriageHAll	= v_fam.at(i+1).marriageHAll;
			marriageWX		= v_fam.at(i+1).marriageW;
			marriageWAll	= v_fam.at(i+1).marriageWAll;
			numOfChildren	= v_fam.at(i+1).numOfChildren;
			fwprintf( theApp.fl, L"%-10s %-10s %-28s %-10s %-28s %d/%d %d/%d %d\n", refF, refH1, husband, refW, wife, marriageHX, marriageHAll, marriageWX, marriageWAll, numOfChildren ); 
			refHPrev = refH1;
		}
	}


	std::sort( v_fam.begin(), v_fam.end(), sortByCnt ); 
	std::sort( v_fam.begin(), v_fam.end(), multiSort_FAM_refW); // refH szerint rendezi: a f�rj feles�geivel k�t�tt h�zass�gai egym�s ut�n lesznek

	fwprintf( theApp.fl, L"\n\n\nFeles�g t�bbsz�r�s h�zass�gai\n\n\n" );
	for( UINT i = 0; i < v_fam.size()-1; ++i )
	{
		refW1 = v_fam.at(i).refW;
		refW2 = v_fam.at(i+1).refW;
		if( refW1 == refW2 )						// i-bne �s i+1-ben azonos feles�
		{
			if( refWPrev != refW1 )					// nem ugyanaz, mint az el�z�, �j p�r!
			{
				refF			= v_fam.at(i).refF;
				refH			= v_fam.at(i).refH;
				husband			= getIndi( refH );
				wife			= getIndi( refW1 );
				marriageHX		= v_fam.at(i).marriageH;
				marriageHAll	= v_fam.at(i).marriageHAll;
				marriageWX		= v_fam.at(i).marriageW;
				marriageWAll	= v_fam.at(i).marriageWAll;
				numOfChildren	= v_fam.at(i).numOfChildren;
				fwprintf( theApp.fl, L"\n%-10s %-10s %-28s %-10s %-28s %d/%d %d/%d %2d\n", refF, refH, husband, refW1, wife, marriageHX, marriageHAll, marriageWX, marriageWAll, numOfChildren ); 
			}
			refF			= v_fam.at(i+1).refF;
			refH			= v_fam.at(i+1).refH;
			husband			= getIndi( refH );
			wife			= getIndi( refW2 );
			marriageHX		= v_fam.at(i+1).marriageH;
			marriageHAll	= v_fam.at(i+1).marriageHAll;
			marriageWX		= v_fam.at(i+1).marriageW;
			marriageWAll	= v_fam.at(i+1).marriageWAll;
			numOfChildren	= v_fam.at(i+1).numOfChildren;
			fwprintf( theApp.fl, L"%-10s %-10s %-28s %-10s %-28s %d/%d %d/%d %2d\n", refF, refH, husband, refW2, wife, marriageHX, marriageHAll, marriageWX, marriageWAll, numOfChildren ); 
			refWPrev = refW1;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool multiSort_refH(const CHIL &v1, const CHIL &v2) 
{ 
	if( v1.refH == v2.refH ) 
		return( v1.refW < v2.refW ); 
    return( v1.refH < v2.refH ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool multiSort_refW(const CHIL &v1, const CHIL &v2) 
{ 
	if( v1.refW == v2.refW ) 
		return( v1.refH < v2.refH ); 
    return( v1.refW < v2.refW ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool multiSort_FAM_refH(const FAM &v1, const FAM &v2) 
{ 
	if( v1.refH == v2.refH ) 
		return( v1.refW < v2.refW ); 
    return( v1.refH < v2.refH ); 
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool multiSort_FAM_refW(const FAM &v1, const FAM &v2) 
{ 
	if( v1.refW == v2.refW ) 
		return( v1.refH < v2.refH ); 
    return( v1.refW < v2.refW ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByCnt(const FAM &v1, const FAM &v2) 
{ 
    return( v1.cnt < v2.cnt ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortChil_cnt(const CHIL &v1, const CHIL &v2) 
{ 
    return( v1.cnt < v2.cnt ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sort_refF(const INDIFAMC &v1, const INDIFAMC &v2) 
{ 
	if( v1.refF == v2.refF ) 
		return( v1.refI < v2.refI ); 
	return( v1.refF < v2.refF ); 
}
bool sortRefF(const INDIFAMS &v1, const INDIFAMS &v2) 
{ 
	return( v1.refF < v2.refF ); 
}
bool sortSexRefI(const INDIFAMS &v1, const INDIFAMS &v2) 
{ 
	if( v1.sex == v2.sex ) 
		return( v1.refI < v2.refI ); 
    return( v1.sex < v2.sex ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// v_indi parentIndex-�be �tteszi a v_fam marriageH-j�t
CString CGedcomIn::getIndi( CString ref )
{
	CString name(L"");
	for( UINT i = 0; i < v_indi.size(); ++i )
	{
		if( v_indi.at(i).refI == ref )
		{
			name.Format( L"%s %s", v_indi.at(i).last_name, v_indi.at(i).first_name );
			break;
		}
	}
	return name;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGedcomIn::getIndexIndi( CString ref )
{
	for( UINT i = 0; i < v_indi.size(); ++i )
	{
		if( v_indi.at(i).refI == ref )
			return i;
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGedcomIn::getIndexFam( CString ref )
{
	for( UINT i = 0; i < v_fam.size(); ++i )
	{
		if( v_fam.at(i).refF == ref )
			return i;
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a megadott xref-hez tartoz� rowid-t adja vissza ( amit insert�l�skor meg�rz�tt )
CString CGedcomIn::getRowid( CString xref )
{
	for( UINT i = 0; i < v_indi.size(); ++i )
	{
		if( v_indi.at(i).refI == xref )
			return( v_indi.at(i).rowid );
	}
	return( L"" );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGedcomIn::findHref( CString refFam )
{
	for( UINT i = 0; i < v_fam.size(); ++i )
	{
		if( v_fam.at(i).refF == refFam )
			return( v_fam.at(i).refH );
	}
	return L"";
}
