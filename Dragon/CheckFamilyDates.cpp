// CCheckFamilyDates.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "CheckFamilyDates.h"
#include "utilities.h"


// SELECT  FROM people father
enum
{
	X_ROWID = 0,
	X_LINENUMBER,
	X_FIRST_NAME,
	X_LAST_NAME,
	X_BIRTH,
	X_DEATH,
};

// SELECT from people mother
enum
{
	Y_ROWID = 0,
	Y_LINENUMBER,
	Y_FIRST_NAME,
	Y_LAST_NAME,
	Y_BIRTH,
	Y_DEATH,
};


// SELECT FROM people child
enum
{
	Z_ROWID = 0,
	Z_LINENUMBER,
	Z_FIRST_NAME,
	Z_LAST_NAME,
	Z_BIRTH,
	Z_DEATH,
	Z_MOTHER_ID,
	Z_MOTHERINDEX,
};


// CCheckFamilyDates dialog

IMPLEMENT_DYNAMIC(CCheckFamilyDates, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFamilyDates::CCheckFamilyDates(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckFamilyDates::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFamilyDates::~CCheckFamilyDates()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_KERES, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckFamilyDates, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_STN_CLICKED(IDC_KERES, &CCheckFamilyDates::OnClickedKeres)
	ON_STN_CLICKED(IDC_NEXT, &CCheckFamilyDates::OnClickedNext)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();

	colorKeres.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );

	CString fileName = L"childrenFAmily";
	m_filespec = theApp.openTextFile( &textF, fileName, L"w+" );

	fwprintf( textF, L"Az anya 16 éves kora után született gyerekeket fogadj el a program helyesnek.\n\n" ); 

	
	collectChildren();

	fclose( textF );
	theApp.showFile( m_filespec );
	return TRUE; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnClickedKeres()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnClickedNext()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCheckFamilyDates::collectChildren()
{
	wndP.Create( NULL, L"Szülõk és gyerekek lekérdezése..." );
	wndP.GoModal();

	m_command =L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date FROM people ORDER BY last_name, first_name";
	if( !theApp.query( m_command ) ) return false;

	
	wndP.SetRange( 0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	int z;
	UINT j;
	


	
	CString name;
	bool push;

	// férjek cikluse
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		h.rowid = theApp.m_recordset->GetFieldString( X_ROWID );

		m_command.Format( L"SELECT rowid,* FROM marriages WHERE spouse1_id='%s'", h.rowid );
		if( !theApp.query1( m_command ) ) break;
		if( theApp.m_recordset1->RecordsCount() < 2 ) goto cont;  // nincs vagy csak 1 felesége van
		h.numOfSpouses	= theApp.m_recordset1->RecordsCount();

		//gyerekek lekérdezése
		m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date, mother_id, parent2IndexCalc FROM people WHERE father_id ='%s' ORDER BY parent2IndexCalc", h.rowid );
		if( !theApp.query2( m_command ) ) break;
		if( !theApp.m_recordset2->RecordsCount() ) goto cont;		// nincsenek gyerekek
		
		h.linenumber	= theApp.m_recordset->GetFieldString( X_LINENUMBER );
		h.name.Format( L"%s %s", theApp.m_recordset->GetFieldString( X_LAST_NAME), getFirstWord( theApp.m_recordset->GetFieldString( X_FIRST_NAME) ) );
		h.birth			= theApp.m_recordset->GetFieldString( X_BIRTH );
		h.death			= theApp.m_recordset->GetFieldString( X_DEATH );
	
		vHusband.push_back( h );

		vChildren.clear();
		for( UINT k = 0; k < theApp.m_recordset2->RecordsCount() ; ++k, theApp.m_recordset2->MoveNext() )
		{
			c.rowid		= theApp.m_recordset2->GetFieldString( Z_ROWID );
			c.name.Format( L"%s %s", theApp.m_recordset2->GetFieldString( Z_LAST_NAME), getFirstWord( theApp.m_recordset2->GetFieldString( Z_FIRST_NAME) ) );
			c.birth		= theApp.m_recordset2->GetFieldString( Z_BIRTH );
			c.death		= theApp.m_recordset2->GetFieldString( Z_DEATH );
			c.father_id	= h.rowid;
			c.mother_id	= theApp.m_recordset2->GetFieldString( Z_MOTHER_ID );
			c.mother_index = _wtoi( theApp.m_recordset2->GetFieldString( Z_MOTHERINDEX ) );


			vChildren.push_back( c );
		}

		// házasságok lekérdezése
		vWifes.clear();
		for( j = 0; j < theApp.m_recordset1->RecordsCount() ; ++j, theApp.m_recordset1->MoveNext() )
		{
			w.rowidH	= h.rowid;
			w.rowid		= theApp.m_recordset1->GetFieldString( MARRIAGES_SPOUSE2_ID );
			w.marriage	= theApp.m_recordset1->GetFieldString( MARRIAGES_DATE );

			// feleség lekérdezése
			m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date FROM people WHERE rowid='%s'", w.rowid );
			if( !theApp.query3( m_command ) ) break;

			w.linenumber	= theApp.m_recordset3->GetFieldString( Y_LINENUMBER );
			w.name.Format( L"%s %s", theApp.m_recordset3->GetFieldString( Y_LAST_NAME), getFirstWord( theApp.m_recordset3->GetFieldString( Y_FIRST_NAME) ) );
			w.birth			= theApp.m_recordset3->GetFieldString( Y_BIRTH );
			w.death			= theApp.m_recordset3->GetFieldString( Y_DEATH );
		
			w.motherOrder	= j+1;

			vWifes.push_back( w );
		}
		if( j < theApp.m_recordset1->RecordsCount() ) break;

		checkMother();

cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::printVector()
{
	HUSBAND		h;
	WIFES		w;
	CHILDREN	c;
//	for( UINT i = 0; i < vHusband.size(); ++i )
	for( UINT i = 0; i < 500; ++i )
	{
		h = vHusband.at(i);
		fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s\n", h.linenumber, L"", h.numOfSpouses, h.rowid, h.name, h.birth, h.death );

		for( UINT j = 0; j < vWifes.size(); ++j )
		{
			w = vWifes.at(j);
			if( w.rowidH == h.rowid )
				fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s\n", w.linenumber, w.marriage, w.motherOrder, w.rowid, w.name, w.birth, w.death );
		}
		for( UINT k = 0; k < vChildren.size(); ++k )
		{
			c = vChildren.at(k );
			if( c.father_id ==  h.rowid )
				fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s\n", c.linenumber, L"", c.mother_index, c.rowid, c.name, c.birth, c.death );
		}
		fwprintf( textF, L"\n" );
		fflush( textF );
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::checkMother()
{
	WIFES		w;
	CHILDREN	c;
	int	mx;
	bool hiba = false;;
	
	CString dateC;
	CString dateW;
	CString msg;
	_int64 birthC;
	_int64 birthW;

	for( UINT i = 0; i < vChildren.size(); ++i )
	{
		c = vChildren.at(i);
		if( c.birth.IsEmpty() && c.death.IsEmpty() ) return;		// nincs mit ellenõrizni;

		mx = c.mother_index -1;
		if( mx + 1 > vWifes.size() )
		{
			vChildren.at(i).message.Format( L"Anya index nagyobb, mint %d", vWifes.size() );
			printFamily();
			break;
		}

		if( vWifes.at( mx ).birth.IsEmpty() && vWifes.at(mx).death.IsEmpty() && vWifes.at( mx ).marriage.IsEmpty() )
			return;  // a megadott feleségnek nincsenek összehasonlítható adatai



		if( !c.birth.IsEmpty() && !vWifes.at(mx).birth.IsEmpty() )
		{
			if( c.birth.GetLength() == 4 ) c.birth += L".01.01";
			if( vWifes.at(mx).birth.GetLength() == 4 ) vWifes.at(mx).birth += L".01.01";

			birthC = theApp.getDateI( c.birth, 0 );
			birthW = theApp.getDateI( vWifes.at(mx).birth, 16*12 );
			if( birthC < birthW )
			{
				vChildren.at(i).message = L"!!!!";
				hiba = true;
				for( UINT j = 0; j < vWifes.size(); ++j )
				{
					if( j == mx ) continue;

					if( vWifes.at(j).birth.GetLength() == 4 ) vWifes.at(j).birth += L".01.01";
					if( !vWifes.at(j).birth.IsEmpty() )
					{
						birthW = theApp.getDateI( vWifes.at(j).birth, 16*12 );
						if( birthC > birthW )
						{
							msg.Format( L"     %d index jó lenne", j+1 );
							vChildren.at(i).message += msg;
						}
					}
				}
			}
		}
	}
	if( hiba )	printFamily();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::printFamily()
{
	WIFES		w;
	CHILDREN	c;

	fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s\n", h.linenumber, L"", h.numOfSpouses, h.rowid, h.name, h.birth, h.death );

	for( UINT j = 0; j < vWifes.size(); ++j )
	{
		w = vWifes.at(j);
		fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s\n", w.linenumber, w.marriage, w.motherOrder, w.rowid, w.name, w.birth, w.death );
	}
	for( UINT k = 0; k < vChildren.size(); ++k )
	{
		c = vChildren.at(k );
		fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s %s", c.linenumber, L"", c.mother_index, c.rowid, c.name, c.birth, c.death, c.message );
		fwprintf( textF, L"\n" );
	}
	fwprintf( textF, L"\n" );
	fflush( textF );
}