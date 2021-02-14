// Setup.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectedFiles.h"
#include "afxdialogex.h"
#include "utilities.h"


IMPLEMENT_DYNAMIC(CSelectedFiles, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectedFiles::CSelectedFiles(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectedFiles::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectedFiles::~CSelectedFiles()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectedFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXTEDITOR, theApp.m_texteditor_default );
	DDX_Control(pDX, IDC_STATIC_TEXTEDITOR, m_color_texteditor);
	DDX_Control(pDX, IDC_STATIC_HTML, m_color_htmlFileSpec);
	DDX_Control(pDX, IDC_STATIC_GED, colorGedFile);
	DDX_Control(pDX, IDC_STATIC_VIEWER, colorViewer);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectedFiles, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_TEXTEDITOR, &CSelectedFiles::OnClickedStaticTexteditor)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CSelectedFiles::OnClickedButtonDefault)
	ON_STN_CLICKED(IDC_STATIC_HTML, &CSelectedFiles::OnClickedStaticHtml)
	ON_STN_CLICKED(IDC_STATIC_GED, &CSelectedFiles::OnClickedStaticGed)
	ON_STN_CLICKED(IDC_STATIC_VIEWER, &CSelectedFiles::OnClickedStaticViewer)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectedFiles::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem( IDC_TEXTEDITOR )->SetWindowText( theApp.m_texteditor );
	GetDlgItem( IDC_HTMLFILESPEC)->SetWindowText( theApp.m_htmlFileSpec );
	GetDlgItem( IDC_GED_FILESPEC )->SetWindowTextW( theApp.m_gedFileSpec );
	GetDlgItem( IDC_VIEWER )->SetWindowTextW( theApp.m_viewerSpec );

	m_color_texteditor.SetTextColor( theApp.m_colorClick );
	m_color_htmlFileSpec.SetTextColor( theApp.m_colorClick );
	colorGedFile.SetTextColor( theApp.m_colorClick );
	colorViewer.SetTextColor( theApp.m_colorClick );

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectedFiles::OnClickedButtonDefault()
{
//	CString texteditor;
	CString drive;
	CString	dir;
	CString	fname;
	CString	ext;

	GetDlgItem( IDC_TEXTEDITOR )->SetWindowTextW( theApp.m_texteditor_default );

	theApp.m_texteditor = theApp.m_texteditor_default;
	theApp.WriteProfileStringW( L"Settings", L"texteditor", theApp.m_texteditor_default );

	splitFilespec( theApp.m_texteditor_default, &drive,&dir,&fname,&ext );
	theApp.m_editorName.Format( L"%s.%s", fname, ext );
	theApp.m_editorFolder.Format( L"%s:%s", drive, dir );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectedFiles::OnClickedStaticHtml()
{
	if( theApp.selectHtml( FALSE ) )
		GetDlgItem( IDC_HTMLFILESPEC )->SetWindowTextW( theApp.m_htmlFileSpec );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectedFiles::OnClickedStaticTexteditor()
{
	if( theApp.selectTextEditor() )
		GetDlgItem( IDC_TEXTEDITOR )->SetWindowTextW( theApp.m_texteditor );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectedFiles::OnClickedStaticGed()
{
	if( theApp.selectGedcom( FALSE) )
		GetDlgItem( IDC_GED_FILESPEC )->SetWindowTextW( theApp.m_gedFileSpec );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectedFiles::OnClickedStaticViewer()
{
	if( theApp.selectViewer() )
		GetDlgItem( IDC_VIEWER )->SetWindowText( theApp.m_viewerSpec );
}
