#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"
#include "GAtoDB.h"
#include "SelectTableName.h"
#include "ConnectCsaladTorzs.h"
#include "ProgressWnd.h"
#include "checkFirstNames.h"
#include "CheckNames.h"
#include "checkDateFormat.h"
//
// Egy lesz�rmazotti string felbont�sa t�bb l�p�sben t�rt�nik, el�sz�r kisebb substringekre, majd azokat elemeire szedj�k sz�t

// I. szint
// 'folyt' lev�tel
// [...csal�d] �s [..csal�d �se]lev�tel
// G gener�ci�s karakter lev�tele
// descendantString �s vMarriageStrings( marriageString ) sz�tv�laszt�sa
//
// II. szint: r�videbb substringek

// marriageString:		marriagePlaceDate, spouseString, parentsString, spouseSpousesString-re
// parentsString:		fatherString, motherString

// A descendantString, spouseString, fatherString, motherString �s spouseSpouseString azonos szerkezet�ek,
// ezeket a splitPeopleString bontja fel az al�bbi substringekre:
// nameString, birthString, deathString �s commentString-re


// A descNameString �s a fatherNameString n�mileg k�l�nb�zik a t�bbit�l, ezeket saj�t f�ggv�ny�k bontja fel,
// a spouseNameString, motherNameString �s spouseSpouseNameString felbont�s�t a splitSpouseNameString v�gzi.


// III. szint: substringek elemekere
// descNameString:		title, titolo, last_name, first_name, comment
// spouseNameString:	title, titolo, last_name, first_name, comment
// fatherNameString:	title, titolo, last_name, first_name, comment
// motherNameString:	title, titolo, last_name, first_name, comment
// spouseSpousesString:	title, titolo, last_name, first_name, comment


// A birthString, deathString �s marriagePlaceDateString azonos szerkezet�ek, ezeket a splitPlaceDate bontja fel 'place' �s 'date' elemekre
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

	// people t�bla menetett oszlopai	
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

// a "marriages' t�bla oszlopai
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

// a 'tables' t�bla oszlopai
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
folyt,\
csalad\
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
	GENERATIONS	gen;	// a gener�ci�, az apa �s max 10 feles�g azonoos�t�ja, amit a v_genertion-ben �riz k�s�bbi felhaszn�l�sra
	CString		caption;
	int z;

	if( m_rollToLineFrom )
		caption.Format( L"A %s f�jl beolvas�sa a %d sort�l", theApp.m_htmlFileName, m_rollToLineFrom );
	else if( m_rollToLine )
		caption.Format( L"A %s f�jl %d sor�nak beolvas�sa", theApp.m_htmlFileName, m_rollToLine );
	else if ( m_rollToFamily )
		caption.Format(L"A %s f�jl %s (%d) csal�dj�nak beolvas�sa", theApp.m_htmlFileName, m_familyName, m_rollToFamily );
	else if ( m_rollToTable )
		caption.Format(L"A %s f�jl %s (%d) t�bl�j�nak beolvas�sa", theApp.m_htmlFileName, m_tableName, m_rollToTable );
	else
		caption.Format( L"A %s f�jl beolvas�sa", theApp.m_htmlFileName );

	// a bemeneti GA.html f�jl megk�rdez�se
	CGaToDb dlgInput;
	dlgInput.m_inputMode = GAHTML;
	dlgInput.m_caption = caption;
	if( dlgInput.DoModal() == IDCANCEL ) return false;

	int position;
	
	m_command.Format( L"SELECT last_insert_rowid() FROM people" );
	if( !query( m_command ) ) return false;
	m_rowid = _wtoi( m_recordset.GetFieldString(0) );  // m_rowid az utolj�ra insert�lt people rowid-ja


	clearTableHeader();  // ha nem t�bl�t, hanem lesz�rmazotti list�t olvasunk be, akkor ez kell
	v_tableHeader.clear();

	theApp.m_cntPeople		= 0;

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );		// a bemeneti f�jl k�drendszere: UTF8 vagy ANSI
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead);		// input ga.html f�jl
	int fileLength = (int)file.GetLength();
	
	if( !rollFile( &file ) ) return false;		// eltekeri a f�jlt a k�v�nt pozici�ba, 
												// ami be�ll�tja az m_lineNumber, m_tableNumber, ill. m_familyNUmber �rt�keket.

	m_tableNumber1 = 0;

	CProgressWnd wndP(NULL, caption );
	wndP.GoModal();
	wndP.SetRange(0, fileLength );
	wndP.SetPos(0);
	wndP.SetStep(1);

	CString cLine;
	CString known_as;
	bool kilepett = false;
	m_tableHeader.tableHeader.Empty();
	if( !theApp.execute( L"BEGIN" ) ) return false;			// Ha nme itt lenne, hanem az insertEntries-ben, akkor nagyon lass� lenne!!!
	while(file.ReadString(cLine)) 
	{
		cLine.Trim();
		if( cLine.IsEmpty() ) continue;
		
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM )
			cLine = Utf8ToAnsi( cLine );

		cLine.Replace( '\'', '|' );	 // a nevekben el�fordulhat az ' jel, amit az SQLite nem szeret
		++m_lineNumber;
		if( m_lineNumber == 22 )
			z = 1;
		if(  cLine.Left( 2 ) == L"%%" )			// t�bla fejl�c feldolgoz�sa
		{
			if( m_rollToTable )					// ha megadott t�bl�t akarunk beolvasni
			{
				if( m_tableNumber >= m_rollToTable ) break;
			}

			if( cLine.Left(3) == L"%%%" && m_rollToFamily )		// ha megadott family-t akarunk beolvasni
			{
				if( m_familyNumber >= m_rollToFamily ) break;
			}

			if( !m_tableHeader.tableHeader.IsEmpty() )
				v_tableHeader.push_back( m_tableHeader );
		
			splitTableHeader( cLine );				// m_familyName-et be�ll�tja
			++m_tableNumber1;						// 1-t�l egyes�vel n� a tableNumber, f�hggetlen�l att�l, hoyg hanyadikat olvassuk be!!
			v_generations.clear();					// �j t�bl�n�l �jrakezdi a gener�ci�kat
			v_orderFather.clear();					// �j t�bla
			vParent2Index.clear();
			m_tableAncestry = TRUE;					// az �j t�bla els� embere lesz a t�bla �se
		}
		else if( cLine.Left(2) == L"% " )			// %known_as A tableHeader/lesz�rmazott beolvas�sa ut�n egy k�l�n sorban olvashatjuk be!!
		{
			m_known_as = getSecondWord( cLine );
			if( m_tableAncestry )					// fejl�cet k�veti
				m_tableHeader.known_as = m_known_as;
			else if( !m_known_as.IsEmpty() )									// lesz�rmazottat k�veti
			{
				m_known_as.Replace( ',', ' ' );			// % Aba, aaaaa
				m_known_as.Trim() ;

				if( m_known_as == L"N;" ) continue;
				m_familyName = m_known_as;  // ????
				updatePreviousDescendant( m_known_as );	// az el�z� decendant know_as mezej�be updatei a "% nev"-et
			}
		}
		else			// descendant line
		{
			if( cLine.GetAt(0) != '<' )
			{
				str.Format( L"A %d. sor hib�s!\n%s", m_lineNumber, cLine );
				AfxMessageBox( str );
				return false;
			}

			cLine = cleanHtmlTags( m_lineNumber, cLine );
			if( cLine.IsEmpty() ) continue;
			
			splitLine( cLine );

			fillOrderFather();		// apja hanyadik gyermeke ?
			fillFatherMother();		// az apa-anya azonos�t�t beteszi a d strukt�r�ba �s 

			insertEntries();
			m_tableAncestry = FALSE;

			// v_generation vektorban nyilv�ntartja a most eltett lesz�rmazott adatait
			for( UINT i = 0; i < 10; ++i )	gen.spouse_id[i] = L"0";	// egy lesz�rmazottnak max 10 h�zast�rs�nak rowid-j�t tartja nyilv�n majd
			gen.gen					= d.generation;						// gener�ci�
			gen.descendant_sex_id	= d.sex_id;							// lesz�rmazott neme
			gen.descendant_id		= d.rowid;							// lesz�rmazott rowid-je
			gen.numOfSpouses		= v_marriages.size();				// h�zast�rsak sz�ma
			gen.parentIndex			= d.parentIndexCalc;				// anya-index  ( apja hanyadik feles�gae az anyja )
			gen.orderFather			= d.orderFather;					//�j
			gen.orderMother			= d.orderMother;
			gen.familyName			= m_familyName;
			if( v_marriages.size() > 10 )
			{
				str.Format( L"%d h�zast�rs nem f�r el a v_generation vektorban!!", v_generations.size() );
				AfxMessageBox( str, MB_ICONERROR );
				break;
			}
			for( UINT i = 0; i < v_marriages.size(); ++i )
			{
				gen.spouse_id[i]	= v_marriages.at(i).rowid;			// beteszi az aktu�lis h�zast�rsak rowid-jeit
			}
			v_generations.push_back( gen );								// meg�rzi a leend� ap�k, any�k azonos�t�t k�s�bbio felhaszn�l�sra

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

	v_tableHeader.push_back( m_tableHeader );
	insertTableHeader();

	connectBranches();
	connectCsalad();
	setDummyFather();

	CCheckFirstNames dlg;
	dlg.m_message = false;
	dlg.DoModal();

	CCheckNames dlgN;
	dlgN.m_message = false;
	dlgN.DoModal();

	CDateFormat dlgD;
	dlgD.m_massage = false;
	dlgD.DoModal();

	str.Format( L"Az adatok beolvas�sa befejez�d�tt a\n\n%s f�jlb�l a\n%s adatb�zisba.\n\nV�gezd el az �sszes �szevon�s el�tti ellen�rz�st\n�s amit lehet, jav�tsd a ga.html f�jlban!", theApp.m_htmlFileName, theApp.m_databaseName );
	AfxMessageBox( str, MB_ICONINFORMATION );

	theApp.m_inputMode = GAHTML;
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A bemeneti ga-html f�jlt az el��rt pozici�ba tekeri.
// m_rollToLine:	a k�v�nt sor sz�ma
// m_rollToTable:	a k�v�nt t�bla azonos�t� sz�ma
// m_rollToFamily:	a k�v�nt csal�d azoos�t� sz�ma
int CGaInput::rollFile( CStdioFile* file )
{
	m_familyNumber	= 0;
	m_tableNumber	= 0;
	m_lineNumber	= 0;

	if( !m_rollToLine && !m_rollToTable && !m_rollToFamily ) return 1;
	
	CString	str;
	CString cLine;
	LONGLONG pos = 0;

	CProgressWnd wndProgress(NULL,L"html f�jl teker�se a k�v�nt poz�ci�ba ..." ); 
	wndProgress.GoModal();
	
	if( m_rollToFamily )
	{
		while(file->ReadString(cLine))
		{
			++m_lineNumber;
			if( cLine.Left(2) == L"%%" )
			{
				splitTableHeader( cLine );		// be�ll�tja: m_familyNumber, m_table_number-t �s mindent
				if( m_rollToFamily == m_tableHeader.familyNumber )
				{
					file->Seek( pos, 0 );	// visszamegy a t�bla elej�re ( az el�z� sorra )
					--m_tableNumber;
					--m_familyNumber;
					--m_lineNumber;
					return 1;
				}
			}
			pos = file->GetPosition();		// a beolvasott sor pozici�ja
		}
		str.Format( L"Nincs %d csal�d a f�jlban!", m_rollToFamily );
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
					file->Seek( pos, 0 );	// visszamegy a t�bla elej�re ( az el�z� sorra )
					--m_tableNumber;
					--m_lineNumber;
					return 1;
				}
			}
			pos = file->GetPosition();		// a beolvasott sor pozici�ja
		}
		str.Format( L"Nincs %d t�bla a f�jlban!", m_rollToTable );
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
			if( m_lineNumber + 1 == m_rollToLine )		// csak az el�z� sorig p�r�g
				return 1;
		}
		str.Format( L"A f�jlban nincs %d sor!", m_rollToLine );
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
// T�bl�nk�nt: v_orderFather-ben gener�ci�nk�nt nyilv�ntartja, hogy apja hanyadik gyermeke a lesz�rmazott
// Ebb�l veszi ki az lesz�rm,azott orderFather �rt�k�t.
void CGaInput::fillOrderFather( )
{
	int i;
	int z;
	TCHAR gen;
	ORDERFATHER orderFather;
	
	if( d.generation == 'D' )
		str = d.first_name;

	if( v_orderFather.size() )
	{
		for( i = v_orderFather.size()-1; i >= 0; --i )		// h�tulr�l visszafele t�r�l minden magasabb gener�ci�t
		{
			gen = v_orderFather.at(i).gen;					// a jelenlegin�l nagyobb ( k�s�bbi ) gener�ci�kat eldobja
			if( i && ( gen > d.generation )  )
			{
				v_orderFather.pop_back();
			}
		}

		i = v_orderFather.size() - 1;
		gen = v_orderFather.at( i ).gen;					// saj�t gener�ci�j�n�l van
		if( gen == d.generation  )							// az el�z� ciklusban megtal�lta a gener�ci�t
		{
			++v_orderFather.at(i).orderFather;
			d.orderFather	= v_orderFather.at(i).orderFather;
			m_familyName	= v_orderFather.at(i).familyname;
			++v_orderFather.at(i).orderMother;
			d.orderMother = v_orderFather.at(i).orderMother;
			if( gen == 'D' )
				str = d.first_name;
		}
		else												// �j, magasabb gerer�ci�
		{
			orderFather.gen				= d.generation;
			orderFather.orderFather		= 1;
			orderFather.orderMother		= 1;
			orderFather.familyname		= m_familyName;
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
		orderFather.familyname		= m_familyName;
		v_orderFather.push_back( orderFather );
		d.orderFather = 1;
		d.orderMother = 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// meg�llap�tja a lesz�rmazott apj�nak �s anyj�nak rowid-j�t
void CGaInput::fillFatherMother( )
{
	int i;
	int ix;
	TCHAR gen;

	// d.gen a descendant gener�ci�ja, akinek az apj�t-anyj�t keress�k
	// Megkeresi a v_generations vektorban a legk�zelebbi kor�bbi gener�ci�t
	// Ennek father_id-je �s mother_id-je lesz az apja �s az anyja.
	// A mother_id-t a parentIndex alapj�n v�lasztja ki.

	// az els� gener�ci�nak nincs apja,anyja ez�rt ha �res a v_generation, nincs semmi baj
	if( v_generations.size() )
	{
		for( i = v_generations.size()-1; i >= 0; --i )			// h�tulr�l el�re keresi az el�z� gener�ci�t
		{
			gen = v_generations.at(i).gen;
			if( gen < d.generation )
				break;
		}
		if( i != -1  )															// az el�z� ciklusban megtal�lta a gener�ci�t
		{
			if( v_generations.at(i).descendant_sex_id == 1 )					// sz�l� sex_id-ja
			{
				d.father_id = v_generations.at(i).descendant_id;				// leszedi az apa azonos�t�j�t

				if( d.parentIndexCalc  <= v_generations.at(i).numOfSpouses )
					d.mother_id = v_generations.at(i).spouse_id[d.parentIndexCalc-1];
				d.orderMother	= v_generations.at(i).orderMother;
				++v_generations.at(i).orderMother;
			}
			else																// ha a n�k lesz�rmazottait is nyilv�ntartjuk
			{
				d.mother_id = v_generations.at(i).descendant_id;
				d.father_id = v_generations.at(i).spouse_id[d.parentIndexCalc - 1 ];				// leszedi az apa azonos�t�j�t
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
// Minden t�bla �s�nek �s az azzal azonos gener�ci�knak ( akikek nincsen apjuk ) k�z�s ap�t �ll�t be a html list�z�s miatt.
// Ha el�gaz�s kapcsol�dik a t�bl�ra, akkor ezeknek m�r van apjuk a connectBranches �ltal, ez�rt azokat bk�n hagyja
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::setDummyFather()
{

	CProgressWnd wndP(NULL, L"Apa n�lk�li lesz�rmazottaknak dummy ap�k (1000000 + tableNumber" );
	wndP.GoModal();
	wndP.SetRange(0, 10 );
	wndP.SetPos(0);
	wndP.SetStep(1);

	m_command.Format( L"UPDATE people SET father_id=%d+tableNumber WHERE (father_id=0 OR father_id='' OR typeof(father_id)='null') AND source=1", NOFATHERID );
	if( !theApp.execute( m_command ) ) return;

	wndP.DestroyWindow();
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

	CProgressWnd wndProgress(NULL,L"Folytat�s t�bl�k �sszek�t�se (folyt XXX) ..." ); 
	wndProgress.GoModal();

	/// lek�rdezi azokat az ap�kat, akikn�l el�gaz�s van
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

		// csak a debugol�shoz kell 
		lastName = m_recordset.GetFieldString( 3 );
		firstName = m_recordset.GetFieldString( 4 );

// az el�gaz�s lesz�rmazottja h�zast�rsainak lek�rdez�se
		m_command.Format( L"SELECT spouse1_id, spouse2_id FROM marriages WHERE spouse1_id ='%s' OR spouse2_id='%s'", rowid_father, rowid_father );
		if( !query2( m_command ) ) return;

		v_rowid.clear();
		rowidMother.Empty();  // ha nincs feles�ge az ap�nak
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
// feles�gek (any�k ) rowid-i a v_rowid vektorban
		if( v_rowid.size() < 2 )		// csak 0 vagy 1 anya van
		{
			m_command.Format( L"UPDATE people SET father_id = '%s', mother_id='%s' WHERE familyNumber='%s' AND tableRoman='%s'", rowid_father, rowidMother, familyNumber, folyt ); 
			if( !theApp.execute( m_command ) ) return;
		}
		else if( v_rowid.size() )		// t�bb anya van. Ki kell v�lastani, hogy melyik gyereknek ki az anyja
		{
			// a gyerekek lek�rdez�se
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
void CGaInput::updatePreviousDescendant( CString known_as )
{

//	CString known_as = cLine;
	
//	int		pos;
//	known_as = getWord( cLine, 2, &pos );

	m_command.Format( L"UPDATE people SET known_as='%s' WHERE rowid ='%s'", known_as, m_rowidLastDescendant );
	m_command.Format( L"UPDATE people SET last_name = '%s', known_as='%s' WHERE rowid ='%s'", known_as, known_as, m_rowidLastDescendant );
	if( !theApp.execute( m_command ) ) return;
}
