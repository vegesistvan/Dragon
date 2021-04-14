// Input.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "utilities.h"
#include "EditPeopleManual.h"
#include "afxdialogex.h"
#include "Table_tables.h"
#include "EditTable.h"
#include "SelectMother.h"
#include "GetLastFirst.h"
#include "GetPeople.h"
#include "GetMarriage.h"
#include "Pictures.h"
#include "SaveFirstName.h"
#include "SameName.h"
/*
enum
{
	L_ROWID = 0,
	L_ROWIDS, 
	L_ORDERH,
	L_PLACE,
	L_DATE,
	L_SPOUSE,
	L_ORDERW,
};

enum
{
	B_ROWID = 0,
	B_CNT,
	B_TITLE, 
	B_DATE,
	B_EXT,
	B_COMMENT,
};
*/
const TCHAR* dateModi[] =
{
	L"",
	L"kb",
	L"elõtt",
	L"után"
};
const UINT dateModiCnt = sizeof( dateModi ) / sizeof( TCHAR* );


IMPLEMENT_DYNAMIC(CEditPeopleManual, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditPeopleManual::CEditPeopleManual(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditPeopleManual::IDD, pParent)
	, m_table(_T(""))
	, m_titolo(_T(""))
	, m_last_name(_T(""))
	, m_first_name(_T(""))
	, m_comment(_T(""))
	, m_death_date(_T(""))
	, m_death_place(_T(""))
	, m_birth_date(_T(""))
	, m_nameChanged(_T(""))
	, m_rowid(_T(""))
	, m_birth_place(_T(""))
	, m_title(_T(""))
	, m_posterior(_T(""))
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditPeopleManual::~CEditPeopleManual()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TABLE, colorTable);
	DDX_Control(pDX, IDC_COMBO_SEX, m_ComboSex);
	DDX_Text(pDX, IDC_TITOLO, m_titolo);
	DDX_Text(pDX, IDC_LAST_NAME, m_last_name);
	DDX_Text(pDX, IDC_FIRST_NAME, m_first_name);
	DDX_Text(pDX, IDC_COMMENT, m_comment);
	DDX_Text(pDX, IDC_DEATH_DATE, m_death_date);
	DDX_Text(pDX, IDC_DEATH_PLACE, m_death_place);
	DDX_Text(pDX, IDC_BIRTH_DATE, m_birth_date);
	DDX_Text(pDX, IDC_NAMECHANGED, m_nameChanged);
	DDX_Text(pDX, IDC_TABLE, m_table);
	DDX_Control(pDX, IDC_PHOTOS, colorPhotos);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_title);
	DDX_Text(pDX, IDC_EDIT_POSTERIOR, m_posterior);
	DDX_Text(pDX, IDC_BIRTH_PLACE, m_birth_place);
	DDX_Control(pDX, IDC_BIRTH_COMBO, comboBirth);
	DDX_Control(pDX, IDC_DEATH_COMBO, comboDeath);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEditPeopleManual, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_TABLE, &CEditPeopleManual::OnClickedStaticTable)
	ON_BN_CLICKED(IDOK, &CEditPeopleManual::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_FIRST_NAME, &CEditPeopleManual::OnKillfocusEditFirstname)
	ON_EN_KILLFOCUS(IDC_BIRTH_DATE, &CEditPeopleManual::OnKillfocusEditBirthd)
	ON_EN_KILLFOCUS(IDC_DEATH_DATE, &CEditPeopleManual::OnKillfocusEditDeathd)
	ON_CBN_KILLFOCUS(IDC_COMBO_SEX, &CEditPeopleManual::OnKillfocusComboSex)
	ON_EN_KILLFOCUS(IDC_LAST_NAME, &CEditPeopleManual::OnKillfocusLastName)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_PHOTOS, &CEditPeopleManual::OnClickedPhotos)
	ON_EN_KILLFOCUS(IDC_COMMENT, &CEditPeopleManual::OnKillfocusComment)
	ON_EN_KILLFOCUS(IDC_EDIT_TITLE, &CEditPeopleManual::OnKillfocusEditTitle)
	ON_EN_KILLFOCUS(IDC_TITOLO, &CEditPeopleManual::OnKillfocusTitolo)
	ON_EN_KILLFOCUS(IDC_EDIT_POSTERIOR, &CEditPeopleManual::OnKillfocusEditPosterior)
	ON_EN_KILLFOCUS(IDC_NAMECHANGED, &CEditPeopleManual::OnKillfocusNamechanged)
	ON_CBN_KILLFOCUS(IDC_BIRTH_COMBO, &CEditPeopleManual::OnKillfocusBirthCombo)
	ON_CBN_KILLFOCUS(IDC_DEATH_COMBO, &CEditPeopleManual::OnKillfocusDeathCombo)
	ON_EN_KILLFOCUS(IDC_BIRTH_PLACE, &CEditPeopleManual::OnKillfocusBirthPlace)
	ON_EN_KILLFOCUS(IDC_DEATH_PLACE, &CEditPeopleManual::OnKillfocusDeathPlace)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditPeopleManual::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CString birth;
	CString death;


	colorTable.SetTextColor( theApp.m_colorClick );
	colorPhotos.SetTextColor( theApp.m_colorClick );


	m_ComboSex.AddString( L"" );
	m_ComboSex.AddString( L"férfi" );
	m_ComboSex.AddString( L"nõ" );
	m_ComboSex.SetCurSel( 0 );

	for( UINT i = 0; i < dateModiCnt; ++i )
	{
		comboBirth.AddString( dateModi[i] );
		comboDeath.AddString( dateModi[i] );
	}
	comboBirth.SetCurSel( 0 );
	comboDeath.SetCurSel( 0 );

//	m_orderix = 1;

	m_modified = false;
	initialScreen();

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::initialScreen()
{
	if( !m_rowid.IsEmpty() )
	{
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;

	m_tableNumber	= theApp.m_recordset->GetFieldString( PEOPLE_TABLENUMBER );
	m_title			= theApp.m_recordset->GetFieldString( PEOPLE_TITLE );
	m_titolo		= theApp.m_recordset->GetFieldString( PEOPLE_TITOLO);
	m_last_name		= theApp.m_recordset->GetFieldString( PEOPLE_LAST_NAME );
	m_first_name	= theApp.m_recordset->GetFieldString( PEOPLE_FIRST_NAME );
	m_posterior		= theApp.m_recordset->GetFieldString( PEOPLE_POSTERIOR );
	m_birth_place	= theApp.m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE );
	m_birth_date	= theApp.m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
	m_death_place	= theApp.m_recordset->GetFieldString( PEOPLE_DEATH_PLACE );
	m_death_date	= theApp.m_recordset->GetFieldString( PEOPLE_DEATH_DATE );
	m_comment		= theApp.m_recordset->GetFieldString( PEOPLE_COMMENT );
	m_occupation	= theApp.m_recordset->GetFieldString( PEOPLE_OCCUPATION );
	m_nameChanged	= theApp.m_recordset->GetFieldString( PEOPLE_KNOWN_AS );

	m_sex_id		= _wtoi( theApp.m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
	
	}
	m_name.Format( L"%s %s", m_last_name, m_first_name );

//	if( !m_rowid.IsEmpty() )
	m_table			= getTableName( m_tableNumber );

	
	m_ComboSex.SetCurSel( m_sex_id );
	UpdateData( TOSCREEN );

	CString caption;
	if( m_rowid.IsEmpty() )
		caption = L"Add meg egy új ember adatait!";
	else
		caption.Format( L"%s %s (rowid=%s) szerkesztése", m_last_name, m_first_name, m_rowid );
	SetWindowTextW( caption );

	displayPicture();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::displayPicture()
{

	m_command.Format( L"SELECT rowid FROM pictures WHERE id='%s' AND table_id=%d AND primaryPic=1", m_rowid, PEOPLEX );
	if( !theApp.queryBlob( m_command ) ) return;

	InvalidateRect( NULL, true );
	m_paint = false;
	CString rowid = theApp.m_recordsetBlob->GetFieldString( 0 );
	if( !rowid.IsEmpty() )
	{
		_int64 blob_size;
		void* block = theApp.blobDB->blobRead( "pictures", "picture", rowid, &blob_size );
		if( block == NULL ) return;
		if( !writeBlockToFile( block, blob_size ) ) return;
		m_paint = true;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEditPeopleManual::writeBlockToFile( void* block, int blob_size  )
{
	m_fileSpec.Format( L"%s\\tmp.jpg", theApp.m_workingDirectory );
	if( !openFileSpec( &theApp.fl, m_fileSpec, L"wb" ) ) return false;

	if( fwrite( block, blob_size, 1, theApp.fl ) != 1 )
	{
		AfxMessageBox( L"fwrite failed!" );
		fclose( theApp.fl );
		return false;
	}
	fclose( theApp.fl );
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnPaint()
{
	if( !m_paint ) 
	{
		CDialogEx::OnPaint();
		return;
	}
	CPaintDC dc(this);
	CDC dcMemory;
	CImage image;
	CBitmap bitmap;


	image.Load( m_fileSpec ); // just change extension to load jpg
	bitmap.Attach(image.Detach());

	dcMemory.CreateCompatibleDC( &dc);
    dcMemory.SelectObject(&bitmap);

	BITMAP bm;						// bitmap in memory
	bitmap.GetBitmap( &bm );
	int bmWidth		= bm.bmWidth;
	int bmHeight	= bm.bmHeight;
	float bmRatio	= (float)bm.bmWidth/(float)bmHeight;

	CRect rect;		// a dlg ablak méretei 0,0 pontból ( Windows koordinátákban )
	CRect rectE;	

	GetClientRect(&rect);
	GetWindowRect( rectW );

	int height = rectW.Height()/2;				// a transzformát kép magassága az ablak feléig!
	height = 160;

	int width = (int) (height * bmRatio );		// a transzformált kép szélessége a bmRatio-val arányos

	int cW = (rectW.right - rectW.left)/2;  // ablak középpontjának x koordinátája
	int cB = width/2;						// a sarokban lévõ kép középpontjának x koordinátája
	int x = cW - cB;						// az ablak új sarka

	x = rect.right - width - 10;

	int	y = 10;

	dc.SetStretchBltMode( STRETCH_HALFTONE );
	dc.StretchBlt( x, y, width, height, &dcMemory, 0,0, bmWidth, bmHeight, SRCCOPY);
	CDialogEx::OnPaint();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditPeopleManual::getTableName( CString tableNumber )
{
	m_command.Format( L"SELECT rowid,* FROM tables WHERE rowid='%s'", tableNumber );
	if( !theApp.query( m_command ) ) return L"";
	m_table = theApp.m_recordset->GetFieldString( TABLES_TABLEHEADER );
	return( m_table );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnClickedStaticTable()
{
	CTableTables dlg;
	dlg.m_select = true;			// csak táblát választ
	dlg.m_caption = L"Válaszd ki a családot!";
	if( dlg.DoModal() == IDCANCEL ) return;
	
	m_modified = true;
	m_table			= dlg.m_tableHeader;
	m_tableNumber	= dlg.m_id;
	m_last_name		= dlg.m_last_name;
	GetDlgItem( IDC_TABLE )->SetWindowText( m_table );
	GetDlgItem( IDC_LAST_NAME )->SetWindowText( m_last_name );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusComboSex()
{
	m_sex_id = m_ComboSex.GetCurSel();
	m_modified = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusEditFirstname()
{

	GetDlgItem( IDC_FIRST_NAME )->GetWindowText( m_first_name );
	CStringArray A;
	int n;

	m_modified = true;
	n = wordList( &A, m_first_name, ' ', FALSE );
	for( int i = 0; i < n; ++i )
	{
		m_sex_id = theApp.isFirstName( A[i] );
		if( m_sex_id == -1 )
		{
			CSaveFirstName dlg;
			dlg.m_first_name = m_first_name;
			if( dlg.DoModal()== IDCANCEL )
			{
				m_ComboSex.SetCurSel( 0 );
				GetDlgItem( IDC_FIRST_NAME )->SetWindowTextW( L"" );
				return;
			}
			m_sex_id = dlg.m_sex_id;
		}
		m_ComboSex.SetCurSel( m_sex_id );
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusLastName()
{
	CString lastname;
	GetDlgItem( IDC_LAST_NAME )->GetWindowTextW( lastname );
	if( m_last_name != lastname )
	{
	 	m_modified = true;
		GetDlgItem( IDC_LAST_NAME )->GetWindowText( m_last_name );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusBirthPlace()
{
	m_modified = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusDeathPlace()
{
	m_modified = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusEditBirthd()
{
	m_modified = true;
	GetDlgItem( IDC_BIRTH_DATE )->GetWindowTextW( m_birth_date );
	if( !isDateOK( m_birth_date ) )
	{
		str.Format( L"%s dátum szabálytalan!", m_birth_date );
		AfxMessageBox( str );
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusEditDeathd()
{
	m_modified = true;
	GetDlgItem( IDC_DEATH_DATE )->GetWindowTextW( m_death_date );
	if( !isDateOK( m_death_date ) )
	{
		str.Format( L"%s dátum szabálytalan!", m_death_date );
		AfxMessageBox( str );
		return;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnClickedPhotos() 
{
	CPictures dlg;

	dlg.m_filter.Format( L"WHERE table_id='%d' AND id='%s'", PEOPLEX, m_rowid ); 
	dlg.m_name		= m_name;	// az ember neve
	dlg.m_rowidP	= m_rowid;	// ember rowid-ja

	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusEditTitle()
{
	m_modified = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusTitolo()
{
	m_modified = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusEditPosterior()
{
	m_modified = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusNamechanged()
{
	m_modified = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusBirthCombo()
{
	m_modified = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusDeathCombo()
{
	m_modified = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnKillfocusComment()
{
	m_modified = true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kijelölt feleség szerkesztése
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditPeopleManual::getPeopleString( CString rowid )
{
	CString last_name;
	CString first_name;
	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
	CString comment;
	CString occupation;


	m_command.Format( L"SELECT rowid, * FROM people WHERE rowid='%s'", rowid );
	if( !theApp.query1( m_command ) ) return L"";
	last_name	= theApp.m_recordset1->GetFieldString( PEOPLE_LAST_NAME );
	first_name	= theApp.m_recordset1->GetFieldString( PEOPLE_FIRST_NAME );
	birth_place	= theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_PLACE );
	birth_date	= theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_DATE );
	death_place	= theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_PLACE );
	death_date	= theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_DATE );
	comment		= theApp.m_recordset1->GetFieldString( PEOPLE_COMMENT );
	occupation	= theApp.m_recordset1->GetFieldString( PEOPLE_OCCUPATION );

	str.Format( L"%s %s", last_name, first_name );
	str += getPlaceDateBlock( birth_place, birth_date, '*' );
	str += getPlaceDateBlock( death_place, death_date, '+' );
	str += getPlaceDateBlock( comment, occupation, ' ' );

	str.Trim();
	return str;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditPeopleManual::getPlaceDateBlock( CString place, CString date, TCHAR jel )
{
	
	CString block(L"");

	if( !place.IsEmpty() || !date.IsEmpty() )
	{
		block.Format( L" %c", jel );
		if( !place.IsEmpty() )
		{
			block += place;
			block += L" ";
		}
		if( !date.IsEmpty() )
		{
			block += date;
		}
		block.TrimRight();
		
	}
	return block;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditPeopleManual::getCommentBlock( CString comment, CString occupation  )
{
	CString block(L"");

	if( !occupation.IsEmpty() || !comment.IsEmpty() )
	{
		if( !comment.IsEmpty() )
		{
			block += comment;
			block += L" ";
		}
		if( !occupation.IsEmpty() )
		{
			block += occupation;
		}
		block.TrimRight();
	}
	return block;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeopleManual::OnBnClickedOk()
{
	if( !m_modified )
	{
		CDialogEx::OnOK();
		return;
	}

	UpdateData( FROMSCREEN );
	

//	theApp.WriteProfileInt( L"Settings", L"tableNumber", _wtoi(m_tableNumber) );
//	theApp.WriteProfileStringW( L"Settings", L"father_id", m_father_id );
//	theApp.WriteProfileStringW( L"Settings", L"mother_id", m_mother_id );

	m_sex_id = m_ComboSex.GetCurSel();

	if( m_last_name.IsEmpty() )
	{
		AfxMessageBox( L"Családnevet meg kell adni!" );
		return;
	}
	if( m_first_name.IsEmpty() )
	{
		AfxMessageBox( L"Keresztnevet meg kell adni!" );
		return;
	}
	if( m_rowid.IsEmpty() )
	{
		CSameName dlg;
		dlg.m_last_name		= m_last_name;
		dlg.m_first_name	= m_first_name;
		dlg.m_birth_place	= m_birth_place;
		dlg.m_birth_date	= m_birth_date;
		dlg.m_death_place	= m_death_place;
		dlg.m_death_date	= m_death_date;
		dlg.m_comment		= m_comment;
		if( dlg.DoModal() == IDCANCEL ) return;
		m_rowid = dlg.m_rowid;
	}


	CString fields;
	CString values;
	if( m_rowid.IsEmpty() )
	{

		fields.Format( L"\
tableNumber,\
source,\
sex_id,\
title,\
titolo,\
first_name,\
last_name,\
posterior,\
birth_place,\
birth_date,\
death_place,\
death_date,\
comment,\
known_as\
");
		values.Format( L"'%s', 1,'%d','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s'",
		m_tableNumber,
		m_sex_id,\
		m_title,\
		m_titolo,\
		m_first_name,\
		m_last_name,\
		m_posterior,\
		m_birth_place,\
		m_birth_date,\
		m_death_place,\
		m_death_date,\
		m_comment,\
		m_nameChanged\
		);
		m_command.Format( L"INSERT INTO people (%s) VALUES (%s)", fields, values );
		if( !theApp.execute( m_command ) ) return;

		++theApp.m_cntPeople;
		m_command = L"SELECT last_insert_rowid() FROM people";
		if( ! theApp.query( m_command ) ) return;
		m_rowid = theApp.m_recordset->GetFieldString(0);
	}
	else
	{
		m_command.Format( L"UPDATE people SET \
		tableNumber='%s',\
		source=1,\
		sex_id='%d',\
		title='%s',\
		titolo='%s',\
		first_name='%s',\
		last_name='%s',\
		posterior='%s',\
		birth_place='%s',\
		birth_date='%s',\
		death_place='%s',\
		death_date='%s',\
		comment='%s',\
		known_as='%s'\
		WHERE rowid='%s'\
		",\
		m_tableNumber,
		m_sex_id,\
		m_title,\
		m_titolo,\
		m_first_name,\
		m_last_name,\
		m_posterior,\
		m_birth_place,\
		m_birth_date,\
		m_death_place,\
		m_death_date,\
		m_comment,\
		m_nameChanged,\
		m_rowid\
		);
		if( !theApp.execute( m_command ) ) return;
	}
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditPeopleManual::PreTranslateMessage(MSG* pMsg) //hogy ne az ablak kapja az uzeneteket, hanem az edit control
{
	int x = (int)pMsg->wParam;
	if( pMsg->message == WM_KEYDOWN )	
	{		
		switch( x )
		{
			case VK_RETURN:
				PostMessage( WM_NEXTDLGCTL, 0, 0L);
				return TRUE;
			case VK_ESCAPE:
				CDialogEx::OnCancel();
				break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



