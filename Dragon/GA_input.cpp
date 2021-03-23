#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
#include "GAtoDB.h"
#include "SelectTableName.h"
#include "ConnectCsaladTorzs.h"
#include "InputErrors.h"
#include "ProgressWnd.h"
#include "checkFirstNames.h"
#include "CheckNames.h"
#include "checkDateFormat.h"
//
// Egy leszármazotti string felbontása több lépésben történik, elõször kisebb substringekre, majd azokat elemeire szedjük szét

// I. szint
// 'folyt' levétel
// [] levétel
// G genrációs karakter levétele
// descendantString és vMarriageStrings( marriageString ) szétválasztása
//
// II. szint: rövidebb substringek

// marriageString:		marriagePlaceDate, spouseString, parentsString, spouseSpousesString-re
// parentsString:		fatherString, motherString

// A descendantString, spouseString, fatherString, motherString és spouseSpouseString azonos szerkezetûek,
// ezeket a splitPeopleString bontja fel az alábbi substringekre:
// nameString, birthString, deathString és commentString-re


// A descNameString és a fatherNameString némileg különbözik a többitõl, ezeket saját függvényük bontja fel,
// a spouseNameString, motherNameString és spouseSpouseNameString felbontását a splitSpouseNameString végzi.


// III. szint: substringek elemekere
// descNameString:		title, titolo, last_name, first_name, comment
// spouseNameString:	title, titolo, last_name, first_name, comment
// fatherNameString:	title, titolo, last_name, first_name, comment
// motherNameString:	title, titolo, last_name, first_name, comment
// spouseSpousesString:	title, titolo, last_name, first_name, comment


// A birthString, deathString és marriagePlaceDateString azonos szerkezetûek, ezeket a splitPlaceDate bontja fel 'place' és 'date' elemekre
// birthString:			place, date
// deathString:			place, date
// marriageString:		place, date




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// S P L I T  //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaInput::CGaInput(void)
{

	// people tábla menetett oszlopai	
m_fieldsP = L"\
fileNumber,\
familyNumber,\
tableNumber,\
lineNumber,\
source,\
united,\
spouse,\
spouseparent,\
spousespouse,\
generation,\
sex_id,\
title,\
titolo,\
first_name,\
last_name,\
posterior,\
other_names,\
birth_place,\
birth_date,\
death_place,\
death_date,\
comment,\
father_id,\
mother_id,\
parentIndex,\
parentIndexCalc,\
folyt,\
tableAncestry,\
tableRoman,\
arm,\
orderFather,\
orderMother,\
csalad,\
gap,\
numOfMarriages\
";

// a "marriages' tábla oszlopai
m_fieldsM = L"\
fileNumber,\
familyNumber,\
tableNumber,\
lineNumber,\
spouse1_id,\
spouse2_id,\
orderHusband,\
orderWife,\
place,\
date,\
source\
";

// a 'tables' tábla oszlopai
m_fieldsT = L"\
fileNumber,\
familyNumber,\
lineNumber,\
tableHeader,\
percent,\
familyName,\
alias,\
titolo,\
tableRoman,\
comment,\
torzs,\
known_as,\
arm,\
joint,\
folyt\
";

m_generationPrev = ' ';
_contractions = theApp.contractions();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaInput::~CGaInput(void)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGaInput::inputFile()
{
	GENERATIONS	gen;	// a generáció, az apa és max 10 feleség azonoosítója, amit a v_genertion-ben õriz késõbbi felhasználásra
	CString		caption;

	if( m_rollToLineFrom )
		caption.Format( L"A %s fájl beolvasása a %d sortól", theApp.m_htmlFileName, m_rollToLineFrom );
	else if( m_rollToLine )
		caption.Format( L"A %s fájl %d sorának beolvasása", theApp.m_htmlFileName, m_rollToLine );
	else if ( m_rollToFamily )
		caption.Format(L"A %s fájl %s (%d) családjának beolvasása", theApp.m_htmlFileName, m_familyName, m_rollToFamily );
	else if ( m_rollToTable )
		caption.Format(L"A %s fájl %s (%d) táblájának beolvasása", theApp.m_htmlFileName, m_tableName, m_rollToTable );
	else
		caption.Format( L"A %s fájl beolvasása", theApp.m_htmlFileName );

	// a bemeneti GA.html fájl megkérdezése
	CGaToDb dlgInput;
	dlgInput.m_inputMode = GAHTML;
	dlgInput.m_caption = caption;
	if( dlgInput.DoModal() == IDCANCEL ) return false;

	int position;
	
	m_command.Format( L"SELECT last_insert_rowid() FROM people" );
	if( !query( m_command ) ) return false;
	m_rowid = _wtoi( m_recordset.GetFieldString(0) );  // m_rowid az utoljára insertált people rowid-ja

	clearTableHeader( &m_tableHeader);  // ha nem táblát, hanem leszármazotti listát olvasunk be, akkor ez kell
	v_tableHeader.clear();

	theApp.m_cntPeople		= 0;

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );		// a bemeneti fájl kódrendszere: UTF8 vagy ANSI
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead);		// input ga.html fájl
	int fileLength = (int)file.GetLength();
	
	if( !rollFile( &file ) ) return false;		// eltekeri a fájlt a kívánt pozicióba, 
												// ami beállítja az m_lineNumber, m_tableNumber, ill. m_familyNUmber értékeket.

	CProgressWnd wndP(NULL, caption );
	wndP.GoModal();
	wndP.SetRange(0, fileLength );
	wndP.SetPos(0);
	wndP.SetStep(1);

	CString cLine;
	bool kilepett = false;
	if( !theApp.execute( L"BEGIN" ) ) return false;			// Ha nme itt lenne, hanem az insertEntries-ben, akkor nagyon lassú lenne!!!
	while(file.ReadString(cLine)) 
	{
		cLine.Trim();
		if( cLine.IsEmpty() ) continue;
		
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM )
			cLine = Utf8ToAnsi( cLine );

		cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
		++m_lineNumber;

		if(  cLine.Left( 2 ) == L"%%" )			// tábla fejléc feldolgozása
		{
			if( m_rollToTable )					// ha megadott táblát akarunk beolvasni
			{
				if( m_tableNumber >= m_rollToTable ) break;
			}

			if( cLine.Left(3) == L"%%%" && m_rollToFamily )		// ha megadott family-t akarunk beolvasni
			{
				if( m_familyNumber >= m_rollToFamily ) break;
			}

			processTableHeader( cLine );			// ez beállítja az m_tableNuber-t és az m_familyNumbert-t
			v_generations.clear();					// új táblánál újrakezdi a generációkat
			v_orderFather.clear();					// új tábla
			vParent2Index.clear();
			m_known_as.Empty();						// új táblánál megszûnik a known_as
			m_tableAncestry = TRUE;					// az új tábla elsõ embere lesz a tábla õse
		}
		else if( cLine.Left(2) == L"% " )			// known_as  % N új sorban van!! A tableHeader beolvasása után egy külön sorban olvashatjuk be!!
		{
			if( m_tableAncestry && v_tableHeader.size() )
				v_tableHeader.at( v_tableHeader.size()-1).known_as = cLine;  // az utolsó tableHeader-be beteszi a % N-t!

			m_known_as = getSecondWord( cLine );
			m_known_as.Replace( ',', ' ' );			// % Aba, aaaaa
			m_known_as.Trim() ;
			if( m_known_as == L"N;" ) continue;
			m_familyName	= m_known_as;
			updatePreviousDescendant( cLine );	// az elõzõ decendant know_as mezejébe updatei a "% nev"-et
		}
		else			// descendant line
		{
			if( cLine.GetAt(0) != '<' )
			{
				str.Format( L"A %d. sor hibás!\n%s", m_lineNumber, cLine );
				AfxMessageBox( str );
				return false;
			}

			cLine = cleanHtmlTags( cLine );
			if( cLine.IsEmpty() ) continue;
			
			splitLine( cLine );

			fillOrderFather();		// apja hanyadik gyermeke ?
			fillFatherMother();		// az apa-anya azonosítót beteszi a d struktúrába és 

			insertEntries();
			m_tableAncestry = FALSE;

			// v_generation vektorban nyilvántartja a most eltett leszármazott adatait
			for( UINT i = 0; i < 10; ++i )	gen.spouse_id[i] = L"0";	// egy leszármazottnak max 10 házastársának rowid-jét tartja nyilván majd
			gen.gen					= d.generation;						// generáció
			gen.descendant_sex_id	= d.sex_id;							// leszármazott neme
			gen.descendant_id		= d.rowid;							// leszármazott rowid-je
			gen.numOfSpouses		= v_marriages.size();				// házastársak száma
			gen.parentIndex			= d.parentIndexCalc;				// anya-index  ( apja hanyadik feleségae az anyja )
			gen.orderFather			= d.orderFather;					//új
			gen.orderMother			= d.orderMother;
			for( UINT i = 0; i < v_marriages.size(); ++i )
			{
				gen.spouse_id[i]	= v_marriages.at(i).rowid;			// beteszi az aktuális házastársak rowid-jeit
			}
			v_generations.push_back( gen );								// megõrzi a leendõ apák, anyák azonosítót késõbbio felhasználásra

			wndP.SetPos( (int)file.GetPosition() );
			wndP.StepIt();
			wndP.PeekAndPump();
			if (wndP.Cancelled())
			{
				kilepett = true;
				break;
			}
		}
		if( m_rollToLine && !m_rollToLineFrom ) break;		// ha egy sort akartunk beolvasni
	}
	if( !theApp.execute( L"COMMIT" ) ) return false;
	wndP.DestroyWindow();
	file.Close();
	if( kilepett ) return false;
	
	theApp.insertIntoFiles( theApp.m_htmlFileSpec, GA_HTML );
	insertTableHeader();

	connectBranches();
	setDummyFather();
	connectCsalad();

	CCheckFirstNames dlg;
	dlg.DoModal();

	CCheckNames dlgN;
	dlgN.DoModal();

	CDateFormat dlgD;
	dlgD.DoModal();

	str.Format( L"Az adatok beolvasása befejezõdött a\n\n%s fájlból a\n%s adatbázisba.\n\nVégezd el az összes öszevonás elõtti ellenõrzést\nés amit lehet, javítsd a ga.html fájlban!", theApp.m_htmlFileName, theApp.m_databaseName );
	AfxMessageBox( str, MB_ICONINFORMATION );

	theApp.m_inputMode = GAHTML;
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A bemeneti ga-html fájlt az elõírt pozicióba tekeri.
// m_rollToLine:	a kívánt sor száma
// m_rollToTable:	a kívánt tábla azonosító száma
// m_rollToFamily:	a kívánt család azoosító száma
int CGaInput::rollFile( CStdioFile* file )
{
	m_familyNumber	= 0;
	m_tableNumber	= 0;
	m_lineNumber	= 0;

	if( !m_rollToLine && !m_rollToTable && !m_rollToFamily ) return 1;
	
	CString	str;
	CString cLine;
	LONGLONG pos = 0;

	CProgressWnd wndProgress(NULL,L"html fájl tekerése a kívánt pozícióba ..." ); 
	wndProgress.GoModal();
	
	if( m_rollToFamily )
	{
		while(file->ReadString(cLine))
		{
			++m_lineNumber;
			if( cLine.Left(2) == L"%%" )
			{
				splitTableHeader( cLine );		// beállítja: m_familyNumber, m_table_number-t és mindent
				if( m_rollToFamily == m_tableHeader.familyNumber )
				{
					file->Seek( pos, 0 );	// visszamegy a tábla elejére ( az elõzõ sorra )
					--m_tableNumber;
					--m_familyNumber;
					--m_lineNumber;
					return 1;
				}
			}
			pos = file->GetPosition();		// a beolvasott sor poziciója
		}
		str.Format( L"Nincs %d család a fájlban!", m_rollToFamily );
	}


	if( m_rollToTable )
	{
		while(file->ReadString(cLine))
		{
			++m_lineNumber;
			if( cLine.Left(2) == L"%%" )
			{
				splitTableHeader( cLine );
				if( m_rollToTable == m_tableNumber )
				{
					file->Seek( pos, 0 );	// visszamegy a tábla elejére ( az elõzõ sorra )
					--m_tableNumber;
					--m_lineNumber;
					return 1;
				}
			}
			pos = file->GetPosition();		// a beolvasott sor poziciója
		}
		str.Format( L"Nincs %d tábla a fájlban!", m_rollToTable );
	}

	else if( m_rollToLine )
	{
		while(file->ReadString(cLine))
		{
			++m_lineNumber;
			if( cLine.Left(2) == L"%%" )
			{
				splitTableHeader( cLine );
			}
			if( m_lineNumber + 1 == m_rollToLine )		// csak az elõzõ sorig pörög
				return 1;
		}
		str.Format( L"A fájlban nincs %d sor!", m_rollToLine );
	}
	wndProgress.DestroyWindow();
	AfxMessageBox( str );
	return 0;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Táblánként: v_orderFather-ben generációnként nyilvántartja, hogy apja hanyadik gyermeke a leszármazott
// Ebbõl veszi ki az leszárm,azott orderFather értékét.
void CGaInput::fillOrderFather( )
{
	int i;
	TCHAR gen;
	ORDERFATHER orderFather;

	if( v_orderFather.size() )
	{
		for( i = v_orderFather.size()-1; i >= 0; --i )		// hátulról elõre töröl minden magasabb generációt
		{
			gen = v_orderFather.at(i).gen;					// a jelenleginél nagyobb ( késõbbi ) generációkat eldobja
			if( i && ( gen > d.generation )  )
			{
				v_orderFather.pop_back();
			}
		}

		i = v_orderFather.size() - 1;
		gen = v_orderFather.at( i ).gen;					// saját generációjánál van
		if( gen == d.generation  )							// az elõzõ ciklusban megtalálta a generációt
		{
			++v_orderFather.at(i).orderFather;
			d.orderFather = v_orderFather.at(i).orderFather;

			++v_orderFather.at(i).orderMother;
			d.orderMother = v_orderFather.at(i).orderMother;
		}
		else												// új, magasabb gereráció
		{
			orderFather.gen				= d.generation;
			orderFather.orderFather		= 1;
			orderFather.orderMother		= 1;
			v_orderFather.push_back( orderFather );
			d.orderFather = 1;
			d.orderMother = 1;
		}
	}
	else
	{
		orderFather.gen				= d.generation;
		orderFather.orderFather		= 1;
		orderFather.orderMother		= 1;
		v_orderFather.push_back( orderFather );
		d.orderFather = 1;
		d.orderMother = 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// megállapítja a leszármazott apjának és anyjának rowid-jét
void CGaInput::fillFatherMother( )
{
	int i;
	int ix;
	TCHAR gen;

	// d.gen a descendant generációja, akinek az apját-anyját keressük
	// Megkeresi a v_generations vektorban a legközelebbi korábbi generációt
	// Ennek father_id-je és mother_id-je lesz az apja és az anyja.
	// A mother_id-t a parentIndex alapján választja ki.

	// az elsõ generációnak nincs apja,anyja ezért ha üres a v_generation, nincs semmi baj
	if( v_generations.size() )
	{
		for( i = v_generations.size()-1; i >= 0; --i )			// hátulról elõre keresi az elõzõ generációt
		{
			gen = v_generations.at(i).gen;
			if( gen < d.generation )
				break;
		}
		if( i != -1  )															// az elõzõ ciklusban megtalálta a generációt
		{
			if( v_generations.at(i).descendant_sex_id == 1 )					// szülõ sex_id-ja
			{
				d.father_id = v_generations.at(i).descendant_id;				// leszedi az apa azonosítóját

				if( d.parentIndexCalc  <= v_generations.at(i).numOfSpouses )
					d.mother_id = v_generations.at(i).spouse_id[d.parentIndexCalc-1];
				d.orderMother	= v_generations.at(i).orderMother;
				++v_generations.at(i).orderMother;
			}
			else																// ha a nõk leszármazottait is nyilvántartjuk
			{
				d.mother_id = v_generations.at(i).descendant_id;
				d.father_id = v_generations.at(i).spouse_id[d.parentIndexCalc - 1 ];				// leszedi az apa azonosítóját
				if( v_marriages.size() > 1 )
					ix = 1;
			}
		}
	}
	else
	{
		d.father_id.Empty();
		d.orderMother	= 0;
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minden tábla õsének és az azzal azonos generációknak ( akikek nincsen apjuk ) közös apát állít be a html listázás miatt.
// Ha elágazás kapcsolódik a táblára, akkor ezeknek már van apjuk a connectBranches által, ezért azokat bkén hagyja
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::setDummyFather()
{
	// generációk azonosága is kellene?
	m_command.Format( L"UPDATE people SET father_id=%d+tableNumber WHERE father_id=0 AND source=1 ", NOFATHERID );
	if( !theApp.execute( m_command ) ) return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::connectBranches()
{
	CString rowid_father;
	CString familyNumber;
	CString folyt;

	CString spouse1_id;
	CString spouse2_id;
	CString rowidMother;

	CString lastName;
	CString firstName;

	CString rowid;
	UINT	parentIndexCalc;

	std::vector<CString> v_rowid;

	CProgressWnd wndProgress(NULL,L"Folytatás táblák összekötése (folyt XXX) ..." ); 
	wndProgress.GoModal();

	/// lekérdezi azokat az apákat, akiknél elágazás van
	m_command.Format( L"SELECT rowid, familyNumber, folyt, last_name, first_name FROM people WHERE folyt!=''" );
	if( !query( m_command ) ) return;
	wndProgress.SetRange(0, m_recordset.RecordsCount());
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);


	if( !theApp.execute( L"BEGIN" ) ) return;
	for( UINT i = 0; i < m_recordset.RecordsCount(); ++ i, m_recordset.MoveNext() )
	{
		rowid_father	= m_recordset.GetFieldString( 0 );
		familyNumber	= m_recordset.GetFieldString( 1 );
		folyt			= m_recordset.GetFieldString( 2 );

		// csak a degugoláshoz kell 
		lastName = m_recordset.GetFieldString( 3 );
		firstName = m_recordset.GetFieldString( 4 );

// az elágazás leszármazottja házastársainak lekérdezése
		m_command.Format( L"SELECT spouse1_id, spouse2_id FROM marriages WHERE spouse1_id ='%s' OR spouse2_id='%s'", rowid_father, rowid_father );
		if( !query2( m_command ) ) return;

		v_rowid.clear();
		rowidMother.Empty();  // ha nincs felesége az apának
		for( UINT i = 0; i < m_recordset2.RecordsCount(); ++i, m_recordset2.MoveNext() )
		{
			spouse1_id = m_recordset2.GetFieldString( 0 );
			spouse2_id = m_recordset2.GetFieldString( 1 );
			if( spouse1_id == rowid_father )
				rowidMother = spouse2_id;
			else
				rowidMother = spouse1_id;
			v_rowid.push_back( rowidMother );
		}
// feleségek (anyák ) rowid-i a v_rowid vektorban
		if( v_rowid.size() < 2 )		// csak 0 vagy 1 anya van
		{
			m_command.Format( L"UPDATE people SET father_id = '%s', mother_id='%s' WHERE familyNumber='%s' AND tableRoman='%s'", rowid_father, rowidMother, familyNumber, folyt ); 
			if( !theApp.execute( m_command ) ) return;
		}
		else if( v_rowid.size() )		// több anya van. Ki kell válastani, hogy melyik gyereknek ki az anyja
		{
			// a gyerekek lekérdezése
			m_command.Format( L"SELECT rowid, parentIndexCalc FROM people WHERE familyNumber='%s' AND tableRoman='%s'", familyNumber, folyt );
			if( !query3( m_command ) ) return;
				
			for( UINT i = 0; i < m_recordset3.RecordsCount(); ++i, m_recordset3.MoveNext() )
			{
				rowid = m_recordset3.GetFieldString( 0);
				parentIndexCalc = _wtoi( m_recordset3.GetFieldString(1 ) ) - 1;

				if( parentIndexCalc < v_rowid.size() )
				{
					m_command.Format( L"UPDATE people SET father_id = '%s', mother_id='%s' WHERE rowid = '%s'", rowid_father, v_rowid.at( parentIndexCalc ), rowid ); 
					if( !theApp.execute( m_command ) ) return;
				}
			}
		}
			
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	if( !theApp.execute( L"COMMIT" ) ) return;
	wndProgress.DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::connectCsalad()
{
	CConnectCsalad conn;
	conn.connectCsalad( TRUE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaInput::query( CString command )
{
	if( m_recordset.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaInput::query1( CString command )
{
	if( m_recordset1.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaInput::query2( CString command )
{
	if( m_recordset2.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaInput::query3( CString command )
{
	if( m_recordset3.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// A %N jelet az elõzõ ember comment mezejéhez hozzáteszi
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::updatePreviousDescendant( CString cLine )
{
	CString known_as;
	int		pos;
	known_as = getWord( cLine, 2, &pos );

	m_command.Format( L"UPDATE people SET known_as='%s' WHERE rowid ='%s'", known_as, m_rowidLastDescendant );
	if( !theApp.execute( m_command ) ) return;
}
