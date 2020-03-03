// EditBlob.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "EditBlob.h"
#include "afxdialogex.h"

// CEditBlob dialog

IMPLEMENT_DYNAMIC(CEditBlob, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditBlob::CEditBlob(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditBlob::IDD, pParent)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditBlob::~CEditBlob()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditBlob::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_DISPLAY, m_CheckDisplay);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEditBlob, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CEditBlob::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CEditBlob::OnClickedButtonSave)
	ON_BN_CLICKED(IDCANCEL, &CEditBlob::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, &CEditBlob::OnClickedButtonView)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEditBlob::OnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CEditBlob::OnClickedButtonNext)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditBlob::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW( m_caption );

	int		errno_t;
	FILE* fl;
	CString str;

	m_paint = false;
	
	CString command;
	CString title;
	CString comment;
	CString date;
	int		primary;

	command.Format( L"SELECT rowid, * FROM pictures WHERE rowid ='%s' ", m_rowid );
	if( !theApp.queryBlob( command ) ) return false;
	if( !theApp.m_recordsetBlob->RecordsCount() ) return false;


	date	= theApp.m_recordsetBlob->GetFieldString( PIC_DATE );
	comment	= theApp.m_recordsetBlob->GetFieldString( PIC_COMMENT );
	m_ext	= theApp.m_recordsetBlob->GetFieldString( PIC_EXT );
	m_id	= theApp.m_recordsetBlob->GetFieldString( PIC_ID );
	primary	= _wtoi( theApp.m_recordsetBlob->GetFieldString( PIC_PRIMARY ) );

	title	= theApp.m_recordsetBlob->GetFieldString( PIC_TITLE );
	if( title.IsEmpty() )
	{
		title = m_title;
		if( title == L"Arckép" ) primary = 1;
	}

	m_CheckDisplay.SetCheck( primary );
	GetDlgItem( IDC_TITLE )->SetWindowTextW( title );
	GetDlgItem( IDC_DATE )->SetWindowTextW( date );
	GetDlgItem( IDC_COMMENT )->SetWindowTextW( comment );


	// blob fájlba írása, hogy az ábrázoláshoz fájlból tudjuk beolvasni (image.Load( m_fileSpec ); 
	block = theApp.blobDB->blobRead( "pictures", "picture", m_rowid, &blob_size );
	if( block == NULL ) return false;
	if( !writeBlockToFile( block, blob_size ) ) return false;
	m_paint = true;
	free( block );

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditBlob::OnBnClickedOk()
{
	CString command;
	CString title;
	CString comment;
	CString date;

	m_primary = m_CheckDisplay.GetCheck();
	GetDlgItem( IDC_TITLE )->GetWindowTextW( title );
	GetDlgItem( IDC_DATE )->GetWindowTextW( date );
	GetDlgItem( IDC_COMMENT )->GetWindowTextW( comment );

	command.Format( L"UPDATE pictures SET primaryPic='%d', date='%s', title='%s', comment='%s' WHERE rowid = '%s'", m_primary, date, title, comment, m_rowid );
	theApp.executeBlob( command );
	
	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditBlob::OnClickedButtonSave()
{
	CString fileName;
	CString fileSpec;
	CString title;
	CString extension = L"jpg files(*.jpg)|*.jpg|All Files (*.*)|*.*||";

	if( m_ext == L"pdf" )
		extension = L"pdf files(*.pdf)|*.pdf|All Files (*.*)|*.*||";
	else if( m_ext == L"jpg" )
		extension = L"jpg files(*.jpg)|*.jpg|All Files (*.*)|*.*||";
	else if( m_ext == L"png" )
		extension = L"png files(*.png)|*.png|All Files (*.*)|*.*||";


	extension.Format( L"*.%s|*.%s||", m_ext, m_ext );
	GetDlgItem( IDC_TITLE )->GetWindowTextW( title );

	fileName.Format( L"%s_%s.%s", title, m_id, m_ext ); 

	CFileDialog dlg( FALSE, NULL, fileName, OFN_HIDEREADONLY | OFN_EXPLORER, extension );

	dlg.m_ofn.lpstrTitle = L"Add meg a kívánt fájl nevét és válaszd ki az útvonalát!";
	if( dlg.DoModal( ) == IDCANCEL ) return;

	POSITION pos = dlg.GetStartPosition( );
	fileSpec = dlg.GetNextPathName( pos );

	CopyFile( m_fileSpec, fileSpec, true );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditBlob::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditBlob::OnClickedButtonView()
{
	CString command;
	CString fileSpec;

	CString szFileNameA;
	CString szAppPathA;
	CString title;

	fileSpec = m_fileSpec;
	szFileNameA	= L"AcroRd32.exe";
	szAppPathA	= L"C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader" ;

	if( m_ext == L"pdf" )
	{
		ShellExecute(NULL,L"open", szFileNameA, fileSpec, szAppPathA,SW_SHOW);
	}
	else
	{
		title	= theApp.m_recordsetBlob->GetFieldString( PIC_FILENAME );
		command.Format(  L"/file=%s /bf /title=%s", fileSpec, title );
		ShellExecute(NULL,L"open", theApp.m_viewerName, command,theApp.szAppPathV,SW_SHOW);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditBlob::OnClickedButtonDelete()
{
	
	CString str;
	str.Format( L"Tényleg törölni akarod a dokumentumot?" ); 
	if( ( AfxMessageBox( str, MB_YESNO ) ) == IDNO ) return;

	str.Format( L"DELETE FROM pictures WHERE rowid='%s'", m_rowid );
	if( !theApp.executeBlob( str ) ) return;

	CDialogEx::OnOK();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditBlob::OnPaint()
{
	if( !m_paint ) return;
	if( m_ext == L"pdf" ) 
	{
		CDialogEx::OnPaint();
		return;
	}
	

	CPaintDC dc(this); // device context for painting

	CDC dcMemory;
	CImage image;
	CBitmap bitmap;


	image.Load( m_fileSpec ); // just change extension to load jpg
	bitmap.Attach(image.Detach());

	dcMemory.CreateCompatibleDC( &dc);

//	if( bitmap.m_hObject != NULL) bitmap.DeleteObject();

    dcMemory.SelectObject(&bitmap);

	BITMAP bm;						// bitmap in memory
	bitmap.GetBitmap( &bm );

	int bmWidth		= bm.bmWidth;
	int bmHeight	= bm.bmHeight;
	float bmRatio	= (float)bm.bmWidth/(float)bmHeight;

	CRect rect;		// a dlg ablak méretei 0,0 pontból ( Windows koordinátákban )
	CRect rectW;	// a dlg ablak adatai  ( 0,0 pont a menü alatt!!
	CRect rectE;	

	GetClientRect(&rect);
	GetWindowRect( rectW );
	GetDlgItem( IDC_TITLE )->GetWindowRect( rectE );

	int height = rectW.Height()/2;				// a transzformát kép magassága az ablak feléig!
	height = rectE.top - rectW.top - 72 ;

	int width = (int) (height * bmRatio );		// a transzformált kép szélessége a bmRatio-val arányos

	int cW = (rectW.right - rectW.left)/2;  // ablak középpontjának x koordinátája
	int cB = width/2;						// a sarokban lévõ kép középpontjának x koordinátája
	int x = cW - cB;						// az ablak új sarka
	int	y = 10;

	dc.SetStretchBltMode( STRETCH_HALFTONE );
	dc.StretchBlt( x, y, width, height, &dcMemory, 0,0, bmWidth, bmHeight, SRCCOPY);
//	dc.BitBlt(0,0, bmWidth, bmHeight, &dcMemory, 0, 0, SRCCOPY);	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEditBlob::writeBlockToFile( void* block, int blob_size  )
{
	int		errno_t;
	FILE* fl;
	CString str;

	m_fileSpec.Format( L"%s\\tmp.%s", theApp.m_workingDirectory, m_ext );
	if((errno_t=_wfopen_s(&fl, m_fileSpec,L"wb") )!=0)
	{	
		str.Format(L"%s megnyitási hiba!", m_fileSpec );
		AfxMessageBox(str);
		return false;
	}
	if( fwrite( block, blob_size, 1, fl ) != 1 )
	{
		AfxMessageBox( L"fwrite failed!" );
		fclose( fl );
		return false;
	}
	fclose( fl );
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditBlob::OnClickedButtonNext()
{

}
