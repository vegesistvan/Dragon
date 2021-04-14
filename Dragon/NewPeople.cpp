// Bemenõ adatok:
// m_caption		Az ablak felirata
// m_first_name		Az új ember vezetékneve, if any
//
// Bekéri az új ember adatait és ellenõrzi, hogy van-e már ilyen nevû ember az adatbázisban.
// Ha van, akkor felkéri a felhasználót, hogy válasszon az új és a régi emberek küzül.
// Ha az újat választotta, akkor insertálja az adatbázisba az embert.
// A megadott/választott ember rowid-ját adja vissz az m_rowid változóban.
// Ha a felhasználó Cancel-el mégsem akar új embert adni, akkor az m_rowid empty lesz. 

#include "stdafx.h"
#include "Dragon.h"
#include "NewPeople.h"
#include "afxdialogex.h"
#include "SameName.h"
#include "Pictures.h"
#include "utilities.h"
#include "EditComment.h"

const TCHAR* modiDate[] =
{
	L"",
	L"kb",
	L"elõtt",
	L"után"
};
const UINT modiDateCnt = sizeof( modiDate ) / sizeof( TCHAR* );

enum
{
	B_ROWID = 0,
	B_CNT,
	B_TITLE, 
	B_DATE,
	B_EXT,
	B_COMMENT,
};


IMPLEMENT_DYNAMIC(CNewPeople, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNewPeople::CNewPeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewPeople::IDD, pParent)
	, m_last_name(_T(""))
	, m_first_name(_T(""))
	, m_title(_T(""))
	, m_titolo(_T(""))
	, m_comment(_T(""))
	, m_death_date(_T(""))
	, m_death_place(_T(""))
	, m_birth_date(_T(""))
	, m_birth_place(_T(""))
	
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNewPeople::~CNewPeople()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BIRTH_COMBO, comboBirth);
	DDX_Control(pDX, IDC_DEATH_COMBO, comboDeath);
	DDX_Control(pDX, IDC_COMBO_SEX, comboSex);
	DDX_Text(pDX, IDC_LAST_NAME, m_last_name);
	DDX_Text(pDX, IDC_FIRST_NAME, m_first_name);
	DDX_Text(pDX, IDC_TITOLO, m_titolo);
	DDX_Text(pDX, IDC_COMMENT, m_comment);
	DDX_Text(pDX, IDC_DEATH_DATE, m_death_date);
	DDX_Text(pDX, IDC_DEATH_PLACE, m_death_place);
	DDX_Text(pDX, IDC_BIRTH_DATE, m_birth_date);
	DDX_Text(pDX, IDC_BIRTH_PLACE, m_birth_place);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_title);
	DDX_Control(pDX, IDC_BLOBS, m_ListCtrlB);
	DDX_Control(pDX, IDC_LEIRAS, colorLeiras);
	DDX_Control(pDX, IDC_PHOTOS, colorPhotos);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CNewPeople, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewPeople::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_BIRTH_DATE, &CNewPeople::OnKillfocusBirthDate)
	ON_EN_KILLFOCUS(IDC_DEATH_DATE, &CNewPeople::OnKillfocusDeathDate)
	ON_EN_KILLFOCUS(IDC_FIRST_NAME, &CNewPeople::OnKillfocusFirstName)
	ON_STN_CLICKED(IDC_PHOTOS, &CNewPeople::OnClickedPhotos)
	ON_STN_CLICKED(IDC_LEIRAS, &CNewPeople::OnClickedLeiras)

//	ON_WM_PAINT()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CNewPeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText( m_caption );


	colorPhotos.SetTextColor( theApp.m_colorClick );
	colorLeiras.SetTextColor( theApp.m_colorClick );

	comboSex.ResetContent();
	comboSex.AddString( L"" );
	comboSex.AddString( L"ffi" );
	comboSex.AddString( L"nõ" );
	comboSex.SetCurSel( 0);




	for( UINT i = 0; i < modiDateCnt; ++i )
	{
		comboBirth.AddString( modiDate[i] );
		comboDeath.AddString( modiDate[i] );
	}
	comboBirth.SetCurSel( 0 );
	comboDeath.SetCurSel( 0 );

	// blob table

	m_ListCtrlB.SortByHeaderClick(TRUE);
	m_ListCtrlB.SetExtendedStyle(m_ListCtrlB.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrlB.InsertColumn( B_ROWID,			L"rowid",		LVCFMT_RIGHT,	40,-1,COL_TEXT );
	m_ListCtrlB.InsertColumn( B_CNT,			L"#",			LVCFMT_RIGHT,	20,-1,COL_NUM );
	m_ListCtrlB.InsertColumn( B_TITLE,			L"cím",			LVCFMT_LEFT,	255,-1,COL_TEXT );
	m_ListCtrlB.InsertColumn( B_DATE,			L"dátum",		LVCFMT_LEFT,	80,-1,COL_TEXT );
	m_ListCtrlB.InsertColumn( B_EXT,			L"ext",			LVCFMT_LEFT,	30,-1,COL_TEXT );
	m_ListCtrlB.InsertColumn( B_COMMENT,		L"leírás",		LVCFMT_LEFT,	500,-1,COL_TEXT );


	if( !m_rowid.IsEmpty() )
		setScreen();
	
	m_inserted = false;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );
	if( m_last_name.IsEmpty() )
	{
		AfxMessageBox( L"Családnevet meg kell adni!" );
		return;
	}

	m_sex_id = comboSex.GetCurSel();
	if( m_rowid.IsEmpty() && peopleExists() )
		insertPeople();
	else
		updatePeople();

	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////44////////////////////////////////////////////
bool CNewPeople::setScreen()
{
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid = '%s'", m_rowid );
	if( !query( m_command ) ) return false;
	
	m_sex_id		= _wtoi( m_recordset->GetFieldString( PEOPLE_SEX_ID ) );
	m_titolo		= m_recordset->GetFieldString( PEOPLE_TITOLO );
	m_last_name		= m_recordset->GetFieldString( PEOPLE_LAST_NAME );
	m_first_name	= m_recordset->GetFieldString( PEOPLE_FIRST_NAME );
	m_birth_place	= m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE );
	m_birth_date	= m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
	m_death_place	= m_recordset->GetFieldString( PEOPLE_DEATH_PLACE );
	m_death_date	= m_recordset->GetFieldString( PEOPLE_DEATH_DATE );
	m_comment		= m_recordset->GetFieldString( PEOPLE_COMMENT );

	comboSex.SetCurSel( m_sex_id );
	fillBlobTable();
	UpdateData( TOSCREEN );
	return true;
}
///////////////////////////////////////////////////////////////////////////////////44////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::insertPeople()
{
	CString fields;
	CString values;
	CString command;


	fields.Format( L"\
title,\
titolo,\
sex_id,\
first_name,\
last_name,\
birth_place,\
birth_date,\
death_place,\
death_date,\
comment\
" );
	values.Format( L"'%s','%s','%d','%s','%s','%s','%s','%s','%s','%s'",
m_title,\
m_titolo,\
m_sex_id,\
m_first_name,\
m_last_name,\
m_birth_place,\
m_birth_date,\
m_death_place,\
m_death_date,\
m_comment\
);

	m_rowid.Empty();
	command.Format( L"INSERT INTO people (%s) VALUES (%s)", fields, values );
	if( !theApp.execute( command ) ) return false;
	
	m_inserted = true;

	command = L"SELECT last_insert_rowid() FROM people";
	if( !theApp.query( command ) ) return false;
	m_rowid = theApp.m_recordset->GetFieldString(0);
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::updatePeople()
{
	str.Format( L"title='%s',titolo='%s',sex_id='%d',first_name='%s',last_name='%s',birth_place='%s',birth_date='%s',death_place='%s',death_date='%s',comment='%s'",\
m_title,\
m_titolo,\
m_sex_id,\
m_first_name,\
m_last_name,\
m_birth_place,\
m_birth_date,\
m_death_place,\
m_death_date,\
m_comment\
);


	m_command.Format( L"UPDATE people SET %s WHERE rowid='%s'", str, m_rowid );
	if( !theApp.execute( m_command ) ) return false;
	m_inserted = true;
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnKillfocusBirthDate()
{
	GetDlgItem( IDC_BIRTH_DATE )->GetWindowText( m_birth_date ); 
	if( !m_birth_date.IsEmpty() && !checkDate( m_birth_date ) )
	{
		GetDlgItem( IDC_BIRTH_DATE )->SetWindowText( L"" ); 
		GetDlgItem( IDC_BIRTH_DATE )->SetFocus();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnKillfocusDeathDate()
{
	GetDlgItem( IDC_DEATH_DATE )->GetWindowText( m_death_date ); 
	if( !m_birth_date.IsEmpty() && !checkDate( m_birth_date ) )
	{
		GetDlgItem( IDC_DEATH_DATE )->SetWindowText( L"" ); 
		GetDlgItem( IDC_DEATH_DATE )->SetFocus();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnKillfocusFirstName()
{
	GetDlgItem( IDC_FIRST_NAME )->GetWindowText( m_first_name ); 
	if( m_first_name.IsEmpty() ) return;


	m_sex_id = theApp.isFirstName( m_first_name );
	if( m_sex_id == -1 )
	{
		str.Format( L"%s keresztnév nincs a nyilvántartásban!", m_first_name );
		AfxMessageBox( str );
		m_first_name.Empty();
		GetDlgItem( IDC_FIRST_NAME )->SetWindowText( L"" );
		GetDlgItem( IDC_FIRST_NAME )->SetFocus();
		m_sex_id = 0;
	}
	comboSex.ResetContent();
	comboSex.AddString( L"" );
	comboSex.AddString( L"ffi" );
	comboSex.AddString( L"nõ" );
	comboSex.SetCurSel( m_sex_id );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::peopleExists()
{
	CString people;
	people.Format( L"%s %s %s %s", m_title, m_titolo, m_last_name, m_first_name );
	people.Trim();
	
	CString birth;
	CString death;
	birth.Format( L"%s %s", m_birth_place, m_birth_date );
	death.Format( L"%s %s", m_death_place, m_death_date );
	birth.Trim();
	death.Trim();
	if( !birth.IsEmpty() )
		people.Format( L"%s *%s", (CString)people, birth );
	if( !death.IsEmpty() )
		people.Format( L"%s +%s", (CString)people, death );
	people.Trim();

	if( m_first_name.IsEmpty() )
		m_command.Format( L"SELECT rowid, * FROM people WHERE last_name LIKE '%s%c'", m_last_name,'%' );
	else
		m_command.Format( L"SELECT rowid, * FROM people WHERE first_name LIKE '%s%c' AND last_name LIKE '%s%c'", m_first_name, '%', m_last_name, '%' );
	if( !theApp.query2( m_command ) ) return false;
	if( theApp.m_recordset2->RecordsCount() )
	{
		CSameName dlg;
		dlg.m_last_name		= m_last_name;
		dlg.m_first_name	= m_first_name;
		dlg.m_birth_place	= m_birth_place;
		dlg.m_birth_date	= m_birth_date;
		dlg.m_death_place	= m_death_place;
		dlg.m_death_date	= m_death_date;
		dlg.m_comment		= m_comment;
		dlg.m_people		= people;

		if( dlg.DoModal() == IDCANCEL ) return false;
		m_rowid = dlg.m_rowid;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::checkDate( CString date )
{
	CString error;
	error.Format( L"%s illegális dátum!\n\nmegengedett:\nYYYY\nYYYY.MM.DD ahol MM/DD '??' vagy XX szám", date ); 
	int length = date.GetLength();
	if( length != 4 && length != 10 )
	{
		AfxMessageBox( error );
		return false;
	}
	
	int year	= _wtoi( date.Left( 4 ) );
	int month;
	int day;
	if( year < 1000 || 2500 < year )
	{
		str.Format( L"%d évszám nem megengedett!", year );
		AfxMessageBox( str );
		return false;
	}
	
	if( length == 4 ) return true;
	if( length != 10 )
	{
		AfxMessageBox( error );
		return false;
	}
	if( date.GetAt( 4 ) != '.' || date.GetAt( 7 ) != '.' )
	{
		AfxMessageBox( error );
		return false;
	}

	CString monthS	= date.Mid( 5, 2 );
	if( monthS.Compare( L"??" ) )
	{
		month	= _wtoi( monthS );
		if( month < 1 || 12 < month )
		{
			AfxMessageBox( error );
			return false;
		}
	}

	CString dayS	= date.Mid( 8 );
	if( dayS.Compare( L"??" ) )
	{
		day	= _wtoi( dayS );
		if( day < 1 || 31 < day )
		{
			AfxMessageBox( error );
			return false;
		}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////44
BOOL CNewPeople::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_TAB:
			PostMessage( WM_NEXTDLGCTL,0,0 );
			break;
		case VK_RETURN:
			PostMessage( WM_NEXTDLGCTL,0,0 );
			break;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CNewPeople::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CNewPeople::query1( CString command )
{
	if( m_recordset1->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CNewPeople::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnClickedPhotos()
{
	CString rowidB(L"");
//	if( !all )
	{
		int nItem	= m_ListCtrlB.GetNextItem(-1, LVNI_SELECTED);
		if( nItem == -1 ) return;
		rowidB = m_ListCtrlB.GetItemText( nItem, B_ROWID );
	}

	CPictures dlg;

	dlg.m_filter.Format( L"WHERE table_id='%d' AND id='%s'", PEOPLEX, m_rowid ); 
	dlg.m_name		= m_name;	// az ember neve
	dlg.m_rowidP	= m_rowid;	// ember rowid-ja
	dlg.m_rowidB	= rowidB;	// kép rowid-ja:  Az ember összes képeit a rowidB-vel kezdve mutassa be

	dlg.DoModal();
	fillBlobTable();
	displayPicture();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::OnClickedLeiras()
{
	CEditComment dlg;

	str.Format( L"%s %s leírása", m_last_name, m_first_name );
	dlg.m_caption = str;
	dlg.m_comment = m_comment;
	if( dlg.DoModal() == IDCANCEL ) return;
	m_comment = dlg.m_comment;
	UpdateData( TOSCREEN );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::fillBlobTable()
{
//	m_command.Format( L"SELECT rowid,* FROM pictures WHERE table_id=1 AND id='%s'", m_rowid );
	m_command.Format( L"SELECT rowid,* FROM pictures WHERE id='%s'", m_rowid );
	if( !theApp.queryBlob( m_command ) ) return;

	CString title;
	u_int64 sizeTotal = 0;
	u_int64	sizeBlob;
	m_ListCtrlB.DeleteAllItems();
	int nItem = 0;
	int ret;

	for( UINT i = 0; i < theApp.m_recordsetBlob->RecordsCount(); ++i, theApp.m_recordsetBlob->MoveNext() )
	{
		nItem = m_ListCtrlB.InsertItem( i, theApp.m_recordsetBlob->GetFieldString( PIC_ROWID) );

		str.Format( L"%d", i+1 );
		m_ListCtrlB.SetItemText( nItem, B_CNT, str );
		
		title = theApp.m_recordsetBlob->GetFieldString( PIC_TITLE);
		m_ListCtrlB.SetItemText( nItem, B_TITLE, theApp.m_recordsetBlob->GetFieldString( PIC_TITLE) );

		m_ListCtrlB.SetItemText( nItem, B_DATE, theApp.m_recordsetBlob->GetFieldString( PIC_DATE) );
		m_ListCtrlB.SetItemText( nItem, B_EXT, theApp.m_recordsetBlob->GetFieldString( PIC_EXT) );
		m_ListCtrlB.SetItemText( nItem, B_COMMENT, theApp.m_recordsetBlob->GetFieldString( PIC_COMMENT) );

		sizeBlob = _wtoi( theApp.m_recordsetBlob->GetFieldString( PIC_SIZE) );
		sizeTotal += sizeBlob;
	}
	m_ListCtrlB.SetSortOrder( 1, 2 );

	if( m_ListCtrlB.GetItemCount() )
		GetDlgItem( IDC_PHOTOS )->EnableWindow( true );
	else
		GetDlgItem( IDC_PHOTOS )->EnableWindow( false );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNewPeople::displayPicture()
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNewPeople::writeBlockToFile( void* block, int blob_size  )
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
void CNewPeople::OnPaint()
{
	return;
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
//	CRect rectW;	// a dlg ablak adatai  ( 0,0 pont a menü alatt!!
	CRect rectE;	

	GetClientRect(&rect);
	GetWindowRect( rectW );
//	GetDlgItem( IDC_TITLE )->GetWindowRect( rectE );

	int height = rectW.Height()/2;				// a transzformát kép magassága az ablak feléig!
//	height = rectE.top - rectW.top - 72 ;
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