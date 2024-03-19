// FirstnamesMerge.cpp : implementation file
//

#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "FirstnamesMerge.h"


// CFirstnamesMerge dialog

IMPLEMENT_DYNAMIC(CFirstnamesMerge, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CFirstnamesMerge::CFirstnamesMerge(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FIRSTNAMES_MERGE, pParent)
	, m_systemdb_out(_T(""))
{

	systemDB2 = new CSqliteDB;
	m_rsetSystemIn = new CSqliteDBRecordSet;


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CFirstnamesMerge::~CFirstnamesMerge()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFirstnamesMerge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DATABASE_OUT, m_systemdb_out);
	DDX_Control(pDX, IDC_DATABASE_IN, colorSystemdbIn);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFirstnamesMerge, CDialogEx)
	ON_STN_CLICKED(IDC_DATABASE_IN, &CFirstnamesMerge::OnClickedDatabaseIn)
	ON_BN_CLICKED(IDOK, &CFirstnamesMerge::OnBnClickedOk)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFirstnamesMerge::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_systemdb_out = theApp.m_systemDatabaseSpec;
	GetDlgItem(IDC_DATABASE_IN)->SetWindowTextW(L"Kattints ide a bemeneti adatbázis kiválasztásához!");
	colorSystemdbIn.SetTextColor( theApp.m_colorClick );
	UpdateData(TOSCREEN);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFirstnamesMerge::OnClickedDatabaseIn()
{
	CString str;
	str.Format(L"Adatbázis fájl (*.db)|*.db|Minden fájl (*.*)|*.*|");
	CFileDialog dlg(TRUE, L".*", theApp.m_systemDatabaseSpec, OFN_HIDEREADONLY | OFN_EXPLORER, str);
	dlg.m_ofn.lpstrTitle = L"Válaszd ki a bemeneti keresztnév adatbázis fájlt!";
	if (dlg.DoModal() == IDCANCEL)  return;

	CString systemdb_in = dlg.GetPathName(); 
	GetDlgItem(IDC_DATABASE_IN)->SetWindowTextW(systemdb_in);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFirstnamesMerge::OnBnClickedOk()
{
	CString systemdb_in;
	GetDlgItem(IDC_DATABASE_IN)->GetWindowTextW(systemdb_in);

	if (_waccess(systemdb_in, 0))
	{
		str.Format(L"%s\nadatbázis nem létezik!", systemdb_in);
		AfxMessageBox(str);
		return;

	}

	if( m_systemdb_out == systemdb_in) 
	{
		AfxMessageBox(L"A bemeneti és kimeneti system-db nem lehet azonos!");
		return;
	}

	COpenDatabase open;

	open.m_dbPathName = systemdb_in;
	open.m_connDB = systemDB2;
	open.m_databaseTables = systemTables;
	open.m_databaseIndexes = systemIndexes;
	open.m_numberOfTables = numberOfSystemTables;
	open.m_numberOfIndexes = numberOfSystemIndexes;

	if (!open.openDatabase()) return;

	int cnt = 0;
	CString firstname;
	CString sexid;
	CString command = L"SELECT first_name, sex_id FROM firstnames";
	if (!query(command)) return;


	CString file = L"beszúrt keresztnevek";
	CString filePathName = theApp.openTextFile(&fl, file, L"w+");  // log fájl
	str.Format(L"A %s adatbázisból a %s adatbázisba insertált keresztnevek\n\n", systemdb_in, m_systemdb_out);
	fwprintf(fl, str);



	CProgressWnd wndP(NULL, L"Keresztnevek vizsgálata...");
	wndP.GoModal();
	wndP.SetRange(0, m_rsetSystemIn->RecordsCount());
	wndP.SetPos(0);
	wndP.SetStep(1);
	for (int i = 0; i < m_rsetSystemIn->RecordsCount(); ++i, m_rsetSystemIn->MoveNext())
	{
		firstname = m_rsetSystemIn->GetFieldString(0);
		command.Format(L"SELECT rowid FROM firstnames WHERE first_name = '%s'", firstname );
		if (!theApp.queryName(command)) return;

		if (!theApp.m_recordsetName->RecordsCount())
		{
			sexid = m_rsetSystemIn->GetFieldString(1);
			command.Format(L"INSERT INTO firstnames (first_name, sex_id) VALUES ('%s', '%s')", firstname, sexid);
			if (!theApp.executeSys(command)) return;
			++cnt;
			fwprintf(fl, L"%3s %s\n", sexid, firstname);
		}

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	fclose(fl);
	theApp.showFile(filePathName);
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFirstnamesMerge::executeSys2(CString command)
{
	if (systemDB2->Execute(command))
	{
		str.Format(L"%s\n%s", command, systemDB2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFirstnamesMerge::query(CString command)
{
	if (m_rsetSystemIn->Query(command, systemDB2))
	{
		str.Format(L"%s\n%s", command, m_rsetSystemIn->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}