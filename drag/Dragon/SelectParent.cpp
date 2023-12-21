// EditParent.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "SelectParent.h"
#include "SelectPeople.h"

// CSelectParent dialog

IMPLEMENT_DYNAMIC(CSelectParent, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectParent::CSelectParent(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectParent::IDD, pParent)
	, m_parent(_T(""))
	, m_order_parent(_T(""))
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectParent::~CSelectParent()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectParent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PARENT, m_parent);
	DDX_Text(pDX, IDC_EDIT, m_order_parent);
	DDX_Control(pDX, IDC_STATIC_PARENT, m_parent_color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectParent, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_PARENT, &CSelectParent::OnClickedStaticParent)
END_MESSAGE_MAP()


// CSelectParent message handlers

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectParent::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_recordset		= new CSqliteDBRecordSet;
	m_parent_color.SetTextColor( theApp.m_colorClick );
	SetWindowText( m_caption );

	CString parent;

	if( m_sex == MAN )
		parent = "apa:";
	else
		parent = "anya:";
//	m_parent_id	= m_rowid;
	GetDlgItem( IDC_STATIC_PARENT )->SetWindowTextW( parent );
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSelectParent::getParent( CString rowid )
{
	CString	last_name;
	CString	first_name;
	CString	birth_date;
	CString name;

	str.Empty();
	if( rowid )
	{
		m_command.Format( L"SELECT last_name, first_name, birth_date FROM people WHERE rowid ='%s'",rowid );
		if( !query( m_command ) ) return str;

		last_name	= m_recordset->GetFieldString( 0 );
		first_name	= m_recordset->GetFieldString( 1 );
		birth_date	= m_recordset->GetFieldString( 2 );

		name.Format(L"%s %s", last_name,first_name );
		str.Format( L"%5d    %-30s\t%s",rowid, name, birth_date );
		str.Format( L"%s ( %s rowid=%d )", name, birth_date, rowid );  
	}
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectParent::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectParent::OnClickedStaticParent()
{
	CSelectPeople dlg;	

	dlg.m_filter = m_filter;
	dlg.m_birth_date	= m_birth_date;
	
	if( dlg.DoModal() == IDCANCEL ) return;
	m_parent_id	= dlg.m_rowid;

	str = getParent( m_parent_id );
	GetDlgItem( IDC_PARENT )->SetWindowTextW( str );
}
