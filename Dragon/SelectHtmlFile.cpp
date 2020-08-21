// SelectHtmlFile.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectHtmlFile.h"
#include "afxdialogex.h"


// CSelectHtmlFile dialog

IMPLEMENT_DYNAMIC(CSelectHtmlFile, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectHtmlFile::CSelectHtmlFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectHtmlFile::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectHtmlFile::~CSelectHtmlFile()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectHtmlFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HTMLFILESPEC, colorHtmlFileSpec);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectHtmlFile, CDialogEx)
	ON_STN_CLICKED(IDC_HTMLFILESPEC, &CSelectHtmlFile::OnClickedHtmlfilespec)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectHtmlFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	if( !m_htmlFileSpec.IsEmpty() ) 
		GetDlgItem( IDC_HTMLFILESPEC )->SetWindowTextW( m_htmlFileSpec );

	colorHtmlFileSpec.SetTextColor( theApp.m_colorClick );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectHtmlFile::OnClickedHtmlfilespec()
{

	CString drive;
	CString path;
	CString filename;
	CString ext;
	CString initialDir;

	CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"html files (*.html)|*.html|All Files (*.*)|*.*||" );

	if( !m_htmlFileSpec.IsEmpty() )
	{
		splitFilespec( m_htmlFileSpec, &drive, &path,&filename,&ext );
		initialDir.Format( L"%s:%s", drive,path );
		dlg.m_ofn.lpstrInitialDir = initialDir;
	}

	if( dlg.DoModal( ) == IDCANCEL ) return;

	POSITION pos = dlg.GetStartPosition( );
	m_htmlFileSpec = dlg.GetNextPathName( pos );
	GetDlgItem( IDC_HTMLFILESPEC )->SetWindowTextW( m_htmlFileSpec );
}
