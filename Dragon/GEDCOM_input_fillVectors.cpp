// InputGedcom.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "GEDCOM_input.h"
#include <algorithm>
#include "utilities.h"
#include "ProgressWnd.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INDI rekordokat megkeresi a ged-fájlban és mindegyikre meghívja a recordINDI függvényt
bool CGEDCOMInput::fillVectors()
{
	m_linenumber = 0;
	if( !fill_v_indi() ) return false;
	if( !fill_v_fam() ) return false;

	if( v_fam.size() ) sync_fam_indi();   // ha több felesége van, akkor
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGEDCOMInput::gedCHAR( CString fileSpec )
{
	CString cLine;
	CString code;
	int		pos;
	int		kod = -1;

	theApp.m_inputCode = GetInputCode( theApp.m_gedPathName );
	file_ged.Open( theApp.m_gedPathName, CFile::modeRead );

	while( file_ged.ReadString( cLine ) )
	{
		if( cLine.Find( L"CHAR" ) != -1 )
			break;
	}
	file_ged.Close();
	if( ( pos= cLine.Find( L"CHAR" ) ) == -1 ) return -1; // nincs CHAR
	code = cLine.Mid( pos + 5 );
	if (code == L"IBM WINDOWS")
		kod = ANSI;
	else if( code == L"UTF8" )
		kod = UTF8;
	return kod;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INDI rekordokat megkeresi a ged-fájlban és mindegyikre meghívja a recordINDI függvényt
bool CGEDCOMInput::fill_v_indi()
{
	if( !theApp.gedFileExists() ) return false;


	str.Format( L"Emberek beolvasása folyik..." );
	CProgressWnd wndP( NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, (int)file_ged.GetLength() );
	wndP.SetPos(0);

	bool  vissza = true;
	CString cLine;	
	GEDLINE lxtv;
	int z;
	int cnt = 0;
	v_famc.clear();
	v_indi.clear();
	vPhotos.clear();

	theApp.execute( L"BEGIN" );
	while( file_ged.ReadString( cLine ) )
	{
		++cnt;
		cLine.Replace('\'', '|');
		++m_linenumber;
		if (!extract(cLine, &lxtv))
		{
			str.Format(L"%d. sor hibás: %s", cnt, cLine);
			AfxMessageBox(str);
			vissza = false;
			break;
		}
		if( lxtv.level < 0 ) continue;

		if( lxtv.tag.GetAt(0) == '_' ) continue;
		v_lxtv.clear();
		if( lxtv.tag == L"INDI" )
		{

			if (lxtv.xref == L"@I500911@")
				z = 1;

			v_lxtv.push_back( lxtv );
			recordINDI( &lxtv );
		}
cont:	wndP.SetPos( (int)file_ged.GetPosition() );
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	theApp.execute( L"COMMIT" );
	file_ged.Close();
	return vissza;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INDI rekordot beteszi a v_indi vektorba
// A INDIFAMC azonosítójaó családot beteszi a v_infifam vektorba, megadva a refI és refM értékeket
// Példák:
// 
// NAME Illényi Soma András //
// NAME Bíró Katalin homoródszentmártoni //
// NAME Rhédey Farkas gróf //
// NAME Rhédey Zsuzsanna Klaudia grófnõ //
// NAME Szigeti Orsolya //


// NAME Dorothy May / Lloyd /
// NAME Dora / Waters /


// NAME nemes porcsalmai Ferenc / BAJNAI /
bool CGEDCOMInput::recordINDI( GEDLINE* gl )
{
	CString cLine;
	GEDLINE lxtv;
	CString date;
	CString familyName;
	CString lastname;

	INDI	indi;
	PHOTOS	photos;
	int		last;
	TCHAR	kar;
	int z;
	INDIFAMC famc;
	INDIFAMS fams;

	bool vissza = true;
	int pos;
	int cnt = 0;
	indi.refI = gl->xref;
	indi.linenumber = m_linenumber;
	indi.numOfSpouses = 0;

	fams.order = 0;				// házasság sorszáma
	while( file_ged.ReadString( cLine ) )
	{
		++cnt;
		++m_linenumber;
	
		if( cLine.IsEmpty() ) break;
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM )
			cLine = Utf8ToAnsi( cLine );
		cLine.Replace( '\'', '|' );

		if ((pos = cLine.Find(L"Melinda")) != -1)
			z = 1;

		if (!extract(cLine, &lxtv))
		{
			str.Format(L"%d. sor hibás: %s", cnt, cLine);
			AfxMessageBox(str);
			vissza = false;
			break;
		}
		if (lxtv.level < 0) goto cont;
		if (lxtv.tag.GetAt(0) == '_') goto cont;
		if( lxtv.level == 0 ) break;   // INDI rekord vége, Új rekord következik!!
		
		last = last1Bellow( lxtv.level );
		if (last == -1)
			goto cont;

		if (lxtv.tag == L"NAME")					// minden név benne lehet, zagyvaság
			splitNAME(lxtv.value, &indi);
		else if (lxtv.tag == L"SURN")				// családnév
			splitSURN(lxtv.value, &indi);
		else if (lxtv.tag == L"GIVN")				// keresztnév
			indi.first_name = lxtv.value;
		else if (lxtv.tag == L"NPFX")				// name prefix
			indi.title = lxtv.value;
		else if (lxtv.tag == L"NSFX")				// name suffix
			indi.posterior = lxtv.value;
		else if (lxtv.tag == L"SPFX")				// elõnév
			indi.titolo = lxtv.value;

		else if (lxtv.tag == L"SEX")
			if (lxtv.value == L"M")
				indi.sex = L"1";
			else
				indi.sex = L"2";

		else if (lxtv.tag == L"DATE")				// az elõzõ tag-tõl függõen más helyre teszi
		{
			if (v_lxtv.at(last).tag == L"BIRT")
				indi.birth_date = date2date(lxtv.value);
			else if (v_lxtv.at(last).tag == L"DEAT")
				indi.death_date = date2date(lxtv.value);
			else if (v_lxtv.at(last).tag == L"EDUC")
			{
				indi.occupation += L" ";
				indi.occupation += date2date(lxtv.value);
			}
		}
		else if (lxtv.tag == L"PLAC")
		{
			if (v_lxtv.at(last).tag == L"BIRT")
				indi.birth_place = lxtv.value;
			else if (v_lxtv.at(last).tag == L"DEAT")
				indi.death_place = lxtv.value;
			else if (v_lxtv.at(last).tag == L"EDUC")
			{
				indi.occupation += L" ";
				indi.occupation += lxtv.value;
			}
		}
		else if (lxtv.tag == L"FAMC")				// az INDI mely családban szerepel gyerekként
		{
			famc.refI = v_lxtv.at(last).xref;		// INDI azonosítója
			famc.refF = lxtv.value;					// család azonosítója, amelyben született INDI
			v_famc.push_back(famc);					// az anya-index meghatározására használja a sync_fam_indi-ben
			indi.refF = lxtv.value;
		}
		else if (lxtv.tag == L"FAMS")				// INDI mely családban szerepel férjként vagy feleségként
		{
			fams.refI = v_lxtv.at(last).xref;		// INDI azonosítója
			fams.refF = lxtv.value;					// FAM azonosítója
			fams.order++;
			fams.sex = indi.sex;
			indi.numOfSpouses++;					// egy INDI több családban is lehet házastárs (többszörös házasságok )
			v_fams.push_back(fams);					// a házastársak sorrendjének meghatározására hsználja a sync_fam_indi-ben

		}


		else if (lxtv.tag == L"OCCU")
			indi.occupation = lxtv.value;
		else if (lxtv.tag == L"EDUC")
		{
			indi.occupation += L", ";
			indi.occupation += lxtv.value;
		}

		else if (lxtv.tag == L"NOTE")
		{
			lxtv.value.Replace(L"<p>", L"");
			lxtv.value.Replace(L"</p>", L"");
			indi.comment = lxtv.value;
		}
													// a szabálytalanul hosszú sorokat tartalmazó sorok kiszûrésére kihagytuk ( szücs.ged );
		else if (lxtv.tag == L"CONT")
		{
			if (v_lxtv.at(last).tag == L"SOUR")
			{
				indi.source += L" ";
				indi.source += lxtv.value;
			}
			else if (v_lxtv.at(last).tag == L"NOTE")
			{
				indi.comment += L" ";
				indi.comment += lxtv.value;
			}
		}
		else if (lxtv.tag == L"FILE")
		{
			photos.refI = indi.refI;
			photos.link = lxtv.value;
			vPhotos.push_back(photos);
		}
		else if (lxtv.tag == L"SOUR")
			indi.source = lxtv.value;
		else if (lxtv.tag == L"INFO")
		{
			indi.comment += L" ";
			indi.comment += lxtv.value;
		}
		v_lxtv.push_back(lxtv);

		lastname = indi.last_name.MakeLower();
		kar = towupper(lastname[0]);
		indi.last_name = kar + lastname.Mid(1);

	cont:	fpos = file_ged.GetPosition();		// az utoljára feldolgozott sor pozíciója
	}

	indi.tablenumber = getTablenumber(indi.last_name);
	indi.parentIndex = 0;
	indi.whichChildFather = 0;
	indi.whichChildMother = 0;
	v_indi.push_back(indi);

	file_ged.Seek(fpos, SEEK_SET);	// visszamegy a következõ O-ás level elé!!
	--m_linenumber;			// visszamegy a 0-elé, mert azt újra be fogja olvasni
	return vissza;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// körös.ged-ben:
// SURN lastname firstname
// SURN firstname lastname


void CGEDCOMInput::splitSURN(CString surn, INDI* indi)
{
	CString titolo(L"");
	CString last_name(L"");
	CStringArray A;
	CString word1;
	CString word2;

	int n;
	int k;
	int pos;

	if ((pos = surn.Find('(')) != -1 )
	{
		surn = surn.Left(pos);
		surn.Trim();
	}


	n = splitCString( surn, ' ', false, &A );
//	indi->last_name = A[0];
//	return;

	if (n == 1)
	{
		indi->last_name = A[0];
		return;
	}

	if (n > 1 )
	{
		word1 = A[0];
		word2 = A[1];
		if (word1 == indi->first_name)
			indi->last_name = word2;
		else
			indi->last_name = word1;
	}
	return;

	/*
	if( n > 1 )
	{
		if( (pos = surn.ReverseFind( L' ' )) != -1 )
		{
			titolo = surn.Left( pos );
			last_name = surn.Right( surn.GetLength() - pos );
		}
	}
	else
	{
		last_name = surn;
	}
	last_name.Trim();
	CStringArray B;
	TCHAR c;
	n = splitCString( titolo, ' ', false, &B );
	for( int i = 0; i< n; ++i )
	{
		if( B[i] != L"és" )
		{
			int sizeOfString = ( B[i].GetLength() + 1);
			LPTSTR lpsz = new TCHAR[sizeOfString];
			_tcscpy_s( lpsz, sizeOfString, B[i] );

//			c = towupper( lpsz[0] );
//			lpsz[0] = c;

			lpsz[0] = towupper(lpsz[0]); 

			B[i] = lpsz;
		}
 		titolo = packWords( &B, 0, n );
	}
	indi->titolo = titolo;
	indi->last_name = last_name;
	*/
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FAM rekordokat megkeresi a ged-fájlban és mindegyikra meghívja a recordFAM függvényt
bool CGEDCOMInput::fill_v_fam()
{
	if( !theApp.gedFileExists() ) return false;
	file_ged.Open( theApp.m_gedPathName, CFile::modeRead );
	if( theApp.m_inputCode == UTF8BOM )	file_ged.Seek( 3, SEEK_SET );

	str.Format( L"Családok beolvasása folyik..." );
	CProgressWnd wndP( NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, (int)file_ged.GetLength() );

	int		cnt = 0;
	int line = 0;
	CString cLine;
	GEDLINE lxtv;
	bool vissza = true;

	v_fam.clear();
	v_chil.clear();
	
	while( file_ged.ReadString( cLine ) )
	{
		++line;
		cLine.Replace('\'', '|');
		++m_linenumber;
		if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM)
			cLine = Utf8ToAnsi(cLine);
		
		fpos = file_ged.GetPosition();
		if( !extract( cLine, &lxtv ) )
		{
			str.Format(L"%d. sor hbás: %s", line, cLine);
			AfxMessageBox(str);
			vissza = false;
			break;
		}
		if( lxtv.level < 0 ) continue;

		if( lxtv.tag.GetAt(0) == '_' ) continue;
		v_lxtv.clear();
		if( lxtv.tag == L"FAM" )
		{
			v_lxtv.push_back( lxtv );
			++cnt;
			recordFAM( &lxtv, cnt );  
		}
cont:	wndP.SetPos((int)file_ged.GetPosition());
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	for( UINT i = 0; i < v_chil.size(); ++i )
		v_chil.at(i).cnt = i; // hoyg az eeredit sorrend visszaállítható legyen???

	file_ged.Close();
	return vissza;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A FAM rekordot beteszi a v_fam és a v_chil vektorokba,  cnt a házasság sorsszáma (az eredeti sorrend visszaállítására??? )
bool CGEDCOMInput::recordFAM( GEDLINE* gl, int cnt )
{
	CString cLine;
	GEDLINE lxtv;
	CString date;
	
	FAM		fam;
	CHIL	chil;
	int		last;
	int		numOfChildren = 0;
	bool	vissza = true;
	fam.cnt	= 0;
	fam.date.Empty();
	fam.refH.Empty();
	fam.marriageH = 1;
	fam.marriageHAll = 1;
	fam.marriageW = 1;
	fam.marriageWAll = 1;
	fam.numOfChildren = 0;
	fam.place.Empty();
	fam.refW.Empty();
	fam.refF = gl->xref;
	while( file_ged.ReadString( cLine ) )
	{

		cLine.Replace('\'', '|');
		++m_linenumber;
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM  ) cLine = Utf8ToAnsi( cLine );

		if( !extract( cLine, &lxtv ) )
		{
			str.Format(L"Hibás sor: %s", cLine);
			AfxMessageBox(str);
			vissza = false;
			break;
		}
		if( lxtv.level < 0 ) continue;
		if( lxtv.tag.GetAt(0) == '_' ) continue;
		if( lxtv.level == 0 ) break;
		
		last = last1Bellow( lxtv.level );
		if (last == -1) continue;

		if( lxtv.tag == L"HUSB" )
			fam.refH = lxtv.value;
		else if( lxtv.tag == L"WIFE" )
			fam.refW = lxtv.value;
		else if( lxtv.tag == L"DATE" )
		{
			if( v_lxtv.at(last).tag == L"MARR" )
				fam.date = date2date( lxtv.value );
		}
		else if( lxtv.tag == L"PLAC" )
		{
			if( v_lxtv.at(last).tag == L"MARR" )
				fam.place = lxtv.value;
		}
		else if( lxtv.tag == L"CHIL" )
		{
			str = lxtv.value;
			str.Replace( '@', ' ' );
			str.Replace( 'I', ' ' );
			str.Trim();
			chil.numRefI		= _wtoi( str );
			chil.refF			= fam.refF;
			chil.refH			= fam.refH;
			chil.refW			= fam.refW;
			chil.refC			= lxtv.value;
			chil.parentIndex	= 0;
			chil.whichChildFather	= numOfChildren + 1;
			chil.whichChildMother	= 0;
			v_chil.push_back( chil );
			++numOfChildren;
		}
cont:	v_lxtv.push_back( lxtv );
	}
	fam.cnt		= cnt;
	fam.marriageH = 1;
	fam.marriageHAll = 1;
	fam.marriageW = 1;
	fam.marriageWAll = 1;
	fam.numOfChildren	= numOfChildren;
	v_fam.push_back( fam );
	v_lxtv.clear();
	file_ged.Seek( fpos, 0 );
	return vissza;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A level paraméternél eggyel alacsonaybb level-û sor indexet keresi meg és adja vissza 
UINT CGEDCOMInput::last1Bellow( int level )
{
	int i;
	for( i = v_lxtv.size() - 1; i >= 0; --i )
	{
		if( v_lxtv.at(i).level == level - 1  )
			break;
	}
	return i;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::sync_fam_indi()
{
// whichChildFather - whichChildMother meghatározása ??
// v_fams vektorban lévõ család-azonosítók megkereséses a v_fam vektoran 
// és a házasság sorszámának valamint az összes házasságok számának áthelyezése a v_fam vektorba
	CString refF;  // család azonosító

	CProgressWnd wndP(NULL, L"Többszörös házasságok ...");
	wndP.GoModal();
	wndP.SetRange(0, v_fams.size());
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0; i < v_fams.size()-1; ++i )		// az INDI-ben megadott család azonosítók, amelybe az ember férj vagy feleségkén tartozik
	{
		refF = v_fams.at(i).refF;					// család azonosító 
		for( UINT j = 0; j < v_fam.size(); ++j )	// megkeresi az adott azonosítójú családot a FAM-ban
		{
			if( v_fam.at(j).refF == refF )
			{
				if( v_fams.at(i).sex == L"1" )		// ha a férj volt megadva
				{
					v_fam.at(j).marriageH = v_fams.at(i).order;								// férj házasságának sorszáma
					v_fam.at(j).marriageHAll = getMarriageAll( v_fams.at(i).refI );			// férj házasságainak száma
				}
				else							// ha a feleség volt megadva
				{
					v_fam.at(j).marriageW = v_fams.at(i).order;								// feleség házasságainka sorszáma
					v_fam.at(j).marriageWAll = getMarriageAll( v_fams.at(i).refI );			// feleség házasságainak száma
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();

// a családba tartozó gyerekek indi.parentIndex-ének beállítása az anya sorszámára

// a v_chil eleve a refF szerint rendezett

	int	cnt = 0;
	int	z;
	int	parentIndex;
	int	indexC;
	int	ixF;
	CString child;
	CString refC;

	CProgressWnd wndP2(NULL, L"Gyerek-szülõ indexek ...");
	wndP2.GoModal();
	wndP2.SetRange(0, v_chil.size()*3);
	wndP2.SetPos(0);
	wndP2.SetStep(1);

	for( UINT j = 0; j < v_chil.size()-1; ++j )
	{
		refF = v_chil.at(j).refF;
		if( v_chil.at(j).refF == v_chil.at(j+1).refF )
		{
			ixF = getIndexFam( v_chil.at(j).refF );
			child = getIndi( v_chil.at(j).refC );

			z = v_fam.at(ixF).marriageHAll;
			v_chil.at(j).parentIndex		= 0;
			v_chil.at(j+1).parentIndex		= 0;

			if( v_fam.at(ixF).marriageHAll > 1 )   // csak 1-nél több feleség esetén ad értékel neki
			{
				parentIndex = v_fam.at(ixF).marriageH;
		
				v_chil.at(j).parentIndex	= parentIndex;
				v_chil.at(j+1).parentIndex	= parentIndex;

				// csak az elsõ gyerek parentIndex-ét tölti ki
				if( !cnt )
				{
					v_chil.at(j).parentIndex		= parentIndex;
				}

			// ha az apának csak 1 felesége volt, akkor annak sorszámát nem tartja nyilván
				refC = v_chil.at(j).refC;
				if( ( indexC = getIndexIndi( refC ) ) != -1 )
				{
					v_indi.at( indexC ).parentIndex	= parentIndex; //v_chil.at(j).parentIndex;
					if( !cnt )
						v_indi.at( indexC ).parentIndex	= v_chil.at(j).parentIndex;  // ez lehet 0 is!!!!
					else
						v_indi.at( indexC ).parentIndex	= 0;
					v_indi.at( indexC ).whichChildFather		= v_chil.at(j).whichChildFather;
					v_indi.at( indexC ).whichChildMother		= v_chil.at(j).whichChildMother;
				}

				refC = v_chil.at(j+1).refC;
				if( ( indexC = getIndexIndi( refC ) ) != -1 )
				{
					v_indi.at( indexC ).parentIndex	= parentIndex;
//					if( !cnt )
//						v_indi.at( indexC ).parentIndex	= v_chil.at(j).parentIndex;
//					else
						v_indi.at( indexC ).parentIndex	= 0;
					v_indi.at( indexC ).whichChildFather		= v_chil.at(j).whichChildFather;
					v_indi.at( indexC ).whichChildMother		= v_chil.at(j).whichChildMother;
				}
				++cnt;
			}
		}
		else
		{
//			if( cnt == 1 )			// ha csak 2 gyerek volt, akkor 2.-at törli
//				v_chil.at(j).parentIndex		= 0;
			cnt = 0;
		}
		wndP2.StepIt();
		wndP2.PeekAndPump();
		if (wndP2.Cancelled()) break;
	}

// whichChildFather beállítása

	int	fix;
	int		whichChildFather;
	int		whichChildMother;

	std::sort( v_chil.begin(), v_chil.end(), sortChil_cnt );  // apa-anya szerint rendez
	for( UINT i = 0; i < v_chil.size()-2; ++i )
	{
		cnt = 1;
		fix = i;
		while(  i < v_chil.size() - 1 &&  v_chil.at(i).refH == v_chil.at(i+1).refH )	// megszámolja az apa gyerekeinek számát
		{
			++i;		// index a folytatáshoz
			++cnt;		// egy apának hány gyereke van
		}
		whichChildFather = 1;
		for( int j = 0; j < cnt; ++j )
		{
			v_chil.at(fix+j).whichChildFather = whichChildFather;

			str = v_chil.at(fix+j).refC;
			if( (_ix = getIndexIndi( v_chil.at(fix+j).refC )  ) != -1 )
				v_indi.at(_ix).whichChildFather	= whichChildFather;

			++whichChildFather;
		}
		wndP2.StepIt();
		wndP2.PeekAndPump();
		if (wndP2.Cancelled()) break;
	}


// whichChildMother beállítása
	std::sort( v_chil.begin(), v_chil.end(), sortChil_cnt );  // eredeti sorrend
	std::sort( v_chil.begin(), v_chil.end(), multiSort_refW );  // apa-anya szerint rendez
	for( UINT i = 0; i < v_chil.size()-1; ++i )
	{
		cnt = 1;
		fix = i;
		while(  i < v_chil.size() - 1 &&  v_chil.at(i).refW == v_chil.at(i+1).refW )	// megszámolja az anya gyerekeinek számát
		{
			++i;		// index a folytatáshoz
			++cnt;		// egy anyának hány gyereke van
		}
		whichChildMother = 1;
		for( int j = 0; j < cnt; ++j )
		{
			v_chil.at(fix+j).whichChildMother = whichChildMother;

			str = v_chil.at(fix+j).refC;
			if( (_ix = getIndexIndi( v_chil.at(fix+j).refC )  ) != -1 )
				v_indi.at(_ix).whichChildMother	= whichChildMother;

			++whichChildMother;
		}
		wndP2.StepIt();
		wndP2.PeekAndPump();
		if (wndP2.Cancelled()) break;
	}
	wndP2.DestroyWindow();
	std::sort( v_chil.begin(), v_chil.end(), sortChil_cnt );  // eredeti sorrend
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGEDCOMInput::getMarriageAll( CString refI )
{
	for( UINT i = 0; i < v_indi.size(); ++i )
	{
		if( v_indi.at(i).refI == refI )
		{
			return v_indi.at(i).numOfSpouses;
			break;
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGEDCOMInput::getTablenumber( CString lastname )
{
	return L"1";
	int cnt = 0;
	CString rowid;
	CString tableHeader;
	tableHeader.Format( L"%s %s", L"%%%", lastname );
	m_command.Format( L"SELECT rowid FROM tables WHERE familyName ='%s'", lastname );
	if( !theApp.query( m_command ) ) return L"0";

	rowid = theApp.m_recordset->GetFieldString( 0 );
	if( !theApp.m_recordset->RecordsCount() )
	{
		++cnt;
		m_command.Format( L"INSERT INTO tables ( familyNumber, tableHeader, percent, familyName) VALUES ( 1, '%s', '%s','%s' )", tableHeader, L"%%%", lastname );
		if( !theApp.execute( m_command ) ) return 0;
		m_command = L"SELECT last_insert_rowid() FROM tables";
		if( !theApp.query( m_command ) ) return L"";
		rowid = theApp.m_recordset->GetFieldString( 0 );
	}
	return rowid;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// v_indi parentIndex-ébe átteszi a v_fam marriageH-ját
CString CGEDCOMInput::getIndi(CString ref)
{
	CString name(L"");
	for (UINT i = 0; i < v_indi.size(); ++i)
	{
		if (v_indi.at(i).refI == ref)
		{
			name.Format(L"%s %s", v_indi.at(i).last_name, v_indi.at(i).first_name);
			break;
		}
	}
	return name;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a megadott xref-hez tartozó rowid-t adja vissza ( amit insertáláskor megõrzött )
CString CGEDCOMInput::getRowid(CString xref)
{
	for (UINT i = 0; i < v_indi.size(); ++i)
	{
		if (v_indi.at(i).refI == xref)
			return(v_indi.at(i).rowid);
	}
	return(L"");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGEDCOMInput::getIndexIndi(CString ref)
{
	for (UINT i = 0; i < v_indi.size(); ++i)
	{
		if (v_indi.at(i).refI == ref)
			return i;
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGEDCOMInput::findHref(CString refFam)
{
	for (UINT i = 0; i < v_fam.size(); ++i)
	{
		if (v_fam.at(i).refF == refFam)
			return(v_fam.at(i).refH);
	}
	return L"";
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGEDCOMInput::getIndexFam(CString ref)
{
	for (UINT i = 0; i < v_fam.size(); ++i)
	{
		if (v_fam.at(i).refF == ref)
			return i;
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool multiSort_refW(const CHIL& v1, const CHIL& v2)
{
	if (v1.refW == v2.refW)
		return(v1.refH < v2.refH);
	return(v1.refW < v2.refW);
}
bool sortChil_cnt(const CHIL& v1, const CHIL& v2)
{
	return(v1.cnt < v2.cnt);
}

