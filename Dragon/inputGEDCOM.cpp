// inputGEDCOM.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "inputGEDCOM.h"
#include "GEDCOM_TAGS.h"
#include "utilities.h"
#include <algorithm>
#include "ProgressWnd.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnInputGedcom()
//{
//	CInputGEDCOM dlg;
//	if (dlg.DoModal() == IDCANCEL) return;
//
//	theApp.m_inputVersion = theApp.m_version;
//	theApp.m_inputMode = GEDCOM;
//	m_command.Format(L"INSERT INTO properties (inputVersion, inputMode) VALUES ('%s', '%s')", theApp.m_inputVersion, theApp.m_inputMode );
//	theApp.execute(m_command);
////	if (theApp.mainDB->IsConnected()) theApp.mainDB->Close();
////	theApp.openDatabase(true);
//
//	mainTitle();
//	ChangeMenu();
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CInputGEDCOM, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInputGEDCOM::CInputGEDCOM(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUT_GEDCOM, pParent)
{
	m_recordset = new CSqliteDBRecordSet;
	m_numberOfTags = sizeof(G::tags) / sizeof(G::GEDTAGS);

	m_columnsPeople.Format(L"\
fileNumber,\
linenumber,\
tableNumber,\
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
parentIndex,\
comment,\
source,\
occupation,\
whichChildFather,\
whichChildMother,\
numOfSpouses,\
numOfChildren,\
indi\
");


	m_columnsMarriages.Format(L"\
lineNumber,\
place, \
date, \
husband_id, \
wife_id \
");

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInputGEDCOM::~CInputGEDCOM()
{
	str = theApp.m_gedPathName;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGEDCOM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMENT, colorComment);
	DDX_Control(pDX, IDC_STATIC_GED, colorGED);
	DDX_Text(pDX, IDC_GED, theApp.m_gedPathName);
	DDX_Control(pDX, IDC_DATABASE, m_databaseCtrl);
	DDX_Control(pDX, IDC_MAPPA, m_mappaCtrl);
	DDX_Control(pDX, IDC_GED, m_gedCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CInputGEDCOM, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputGEDCOM::OnBnClickedOk)
	ON_STN_CLICKED(IDC_STATIC_GED, &CInputGEDCOM::OnClickedStaticGed)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInputGEDCOM::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	colorComment.SetTextColor(RED);
	colorGED.SetTextColor(theApp.m_colorClick);

	SetWindowTextW(L"GEDCOM fájl beolvasása adatbázisba");
	m_mappaCtrl.SetWindowTextW(theApp.m_dbFolderPath);
	m_databaseCtrl.SetWindowTextW(theApp.m_dbFileName);
	m_gedCtrl.SetWindowTextW(theApp.m_gedFileName);

	m_command = L"SELECT count() FROM people";
	if (!query(m_command)) return FALSE;

	m_count = _wtoi(m_recordset->GetFieldString(0));
	if (m_count)
	{
		str.Format(L"Az adatbázisban %d ember van nyilvántartva! Törölni fogom õket!!", m_count);
		GetDlgItem(IDC_COMMENT)->SetWindowTextW(str);
	}
	else
	{
		GetDlgItem(IDC_COMMENT)->SetWindowTextW(L"Az adatbázis üres");
	}
	if( theApp.m_gedFileName.IsEmpty() )
		GetDlgItem(IDOK)->EnableWindow(false);
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInputGEDCOM::query(CString command)
{
	if (m_recordset->Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGEDCOM::OnClickedStaticGed()
{
	while (true)
	{
		if (!theApp.selectGedcom()) break;
		if (theApp.m_gedFolderPath != theApp.m_dbFolderPath)
		{
			str.Format(L"Csak a\n%s\nmappából választhatsz GEDCOM fájlt!", theApp.m_dbFolderPath);
			AfxMessageBox(str, MB_ICONEXCLAMATION);
		}
		else
		{
			m_gedCtrl.SetWindowTextW(theApp.m_gedFileName);
			GetDlgItem(IDOK)->EnableWindow(true);
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGEDCOM::OnBnClickedOk()
{
	if (!fillVectors())
	{
		CDialogEx::OnCancel();
		return;
	}

	if (m_count)
	{
		if (!theApp.clearDatabase()) return;
		setCreationTime(theApp.m_dbPathName);
	}

	insertINDI();
	insertFAM();
	updateParents();

	theApp.insertIntoFiles( theApp.m_gedFileName, GEDCOM_FILE);
	theApp.getDBtype();
	CDialogEx::OnOK();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGEDCOM::fillVectors()
{
	if (!theApp.gedFileExists()) return false;

	bool vissza = true;
	void	(CInputGEDCOM:: * fcnPtr)();

	str.Format(L"%s fájl beolvasása...", theApp.m_gedFileName );
	CProgressWnd wndP(NULL, str );
	wndP.GoModal();

	theApp.m_inputCode = gedCHAR(theApp.m_gedPathName);
	if (theApp.m_inputCode == -1) theApp.m_inputCode = GetInputCode(theApp.m_gedPathName);

	gedFile.Open(theApp.m_gedPathName, CFile::modeRead);
	if (theApp.m_inputCode == UTF8BOM)	gedFile.Seek(3, SEEK_SET);


	m_blobFolder.Format(L"%s\\%s_BLOB", theApp.m_dbFolderPath, theApp.m_gedFileTitle);
	if (_waccess(m_blobFolder, 0)) _wmkdir(m_blobFolder);

//	theApp.insertIntoFiles(theApp.m_gedFileName, GEDCOM_FILE);
//	theApp.m_inputMode = GEDCOM;

#ifndef _DEBUG
	str.Format(L"Emberek beolvasása...");
	wndP.SetText(str);
#endif

	wndP.SetRange(0, (int)gedFile.GetLength());
	wndP.SetPos(0);

	CString cLine;
	UINT i;
	UINT j;
	int cnt = 0;

	v_INDI.clear();
	v_FAM.clear();
	v_indiFAMS.clear();
	v_CHIL.clear();
	
	v_lxtv.clear();
	while (gedFile.ReadString(cLine))
	{
		++cnt;
		cLine.Replace('\'', '|');
		++m_lineNumber;
		if (theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM) cLine = Utf8ToAnsi(cLine);
		if (!extract(cLine, &lxtv))
		{
			str.Format(L"%d. sor hibás: %s", cnt, cLine);
			AfxMessageBox(str);
			vissza = false;
			break;
		}

		if (lxtv.tag.GetAt(0) == '_') goto cont;


		for (i = 0; i < m_numberOfTags; ++i)
		{
			if (G::tags[i].name == lxtv.tag)  // ha ismert tag, akkor feldolgozzuk, ha nem, akkor átugorjuk
			{
				if (lxtv.level == 0)    // nullás level!!!!!!!!!!!!!!!!!!!!!!!!!
				{
					if (v_lxtv.size())
					{
						if (lxtv.tag == L"FAM" && !v_FAM.size() )
						{
#ifndef _DEBUG
							str.Format(L"Család azonosítók beolvasása...");
							wndP.SetText(str);
#endif
						}

						process0();			// új 0-ás level jött, aaz elõzõt feldolgozza
						v_lxtv.clear();
					}
					I.linenumber = m_lineNumber;
					F.linenumber = m_lineNumber;
				}
				if (v_lxtv.size())
				{
					while (v_lxtv.at(v_lxtv.size() - 1).level >= lxtv.level)
						v_lxtv.pop_back();									// az elõzõ tagakbõl eldobja azokat, amik nagyobbak nála
				}
				v_lxtv.push_back(lxtv);
				fcnPtr = G::tags[i].fcnPtr;
				if( fcnPtr != NULL)
					(this->*fcnPtr)(); 
				break;
			}
		}
cont:	wndP.SetPos((int)gedFile.GetPosition());
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	gedFile.Close();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Új 0 level tag lett, az elõzõt le kell zárni
bool CInputGEDCOM::process0()
{
	CString tag = v_lxtv.at(0).tag;
	if (tag == L"HEAD")
	{
//		fwprintf(fl, L"%s processed\n\n", tag);
	}
	else if (tag == L"INDI")
	{
		v_INDI.push_back(I);
		clearIndi();
	}
	else if (tag == L"FAM")
	{
		v_FAM.push_back(F);
		clearFAM();
	}
	else if (tag == L"OBJE")
	{
//		fwprintf(fl, L"OBJE skipped\n\n");
	}
	else if (tag == L"TRL")
	{
//		fwprintf(fl, L"TRL skipped\n\n");
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGEDCOM::insertINDI()
{
	CString rowid;
	CString id;
	CString fields;
	CString values;
	CString filespec;
	CString filename;
	CString ext;
	int		pos;
	int i;
	int parentIndex;
	CString lastname;
	CString firstname;
	CStringArray A;
	int n;
	TCHAR ch;
	int z;

	CProgressWnd wndP(NULL, L"Emberek feltöltése az adatbázisba...");
	wndP.GoModal();
	wndP.SetRange(0, (int)v_INDI.size() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	theApp.execute(L"BEGIN");
	for (i = 0; i < v_INDI.size(); ++i)
	{
		firstname = v_INDI.at(i).first_name;
		lastname = v_INDI.at(i).last_name;
		lastname.Trim();
		if (lastname.IsEmpty())
		{
			n = wordList(&A, firstname, ' ', false);
			if (n == 4)
				z = 1;
			if (n > 1)
			{
				lastname = A[0];
				firstname = A[1];
				switch (n)
				{
				case 2:
					break;
				case 3:
					if (iswupper(A[2][0]))  // ez is keresztnév
					{
						firstname += L" ";
						firstname += A[2];
					}
					else
					{

						ch = lastChar(A[2]);
						if (ch == 'i')
						{
							str = A[2];
							str.SetAt(0, towupper(A[2][0]));
							v_INDI.at(i).titolo = str;
						}
						else
							v_INDI.at(i).title = A[2];
					}
					break;
				case 4:
					if (iswupper(A[2][0]))  // ez is keresztnév
					{
						firstname += L" ";
						firstname += A[2];
					}
					ch = lastChar(A[2]);
					if (ch == 'i')
					{
						str = A[2];
						str.SetAt(0, towupper(A[3][0]));
						v_INDI.at(i).titolo = str;
					}
					else
						v_INDI.at(i).title = A[3];
					break;
				}
			}
		}


		firstname.Trim();
		if (firstname.IsEmpty())
			firstname = L"N";

		id = I.refI;
		parentIndex = v_INDI.at(i).parentIndex;
		values.Format(L" 1, '%d', '%s','%s', '%s','%s', '%s','%s','%s','%s','%s','%s','%s','%d', '%s','%s','%s','%d','%d','%d','%d', '%s'",
			v_INDI.at(i).linenumber, \
			v_INDI.at(i).tablenumber, \
			v_INDI.at(i).sex, \
			v_INDI.at(i).title, \
			v_INDI.at(i).titolo, \
			firstname, \
			lastname, \
			v_INDI.at(i).posterior, \
			v_INDI.at(i).birth_place, \
			v_INDI.at(i).birth_date, \
			v_INDI.at(i).death_place, \
			v_INDI.at(i).death_date, \
			v_INDI.at(i).parentIndex, \
			v_INDI.at(i).comment, \
			v_INDI.at(i).source, \
			v_INDI.at(i).occupation, \
			v_INDI.at(i).whichChildFather, \
			v_INDI.at(i).whichChildMother, \
			v_INDI.at(i).numOfSpouses,\
			v_INDI.at(i).numOfChildren,\
			v_INDI.at(i).refI\
		);
		m_command.Format(L"INSERT INTO people (%s) VALUES (%s)", m_columnsPeople, values);
		if (!theApp.execute(m_command)) return false;
		++theApp.m_cntPeople;

		rowid.Format(L"%d", i + 1);
		v_INDI.at(i).rowid = rowid;

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	theApp.execute(L"COMMIT");
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGEDCOM::insertFAM()
{
	CString refH;
	CString refW;
	CString values;
	
	FAM F;

	CString husband_id;
	CString wife_id;
	


	CProgressWnd wndP(NULL, L"Házasságok feltöltése az adatbázisba...");
	wndP.GoModal();
	wndP.SetRange(0, (int)v_FAM.size());
	wndP.SetPos(0);
	wndP.SetStep(1);
	theApp.execute(L"BEGIN");
	for (UINT i = 0; i < v_FAM.size(); ++i)
	{
		F = v_FAM.at(i);
		husband_id = getRowid( F.refH);
		wife_id = getRowid(F.refW);

		values.Format(L"'%d', '%s', '%s', '%s', '%s'",
			F.linenumber,
			F.place,
			F.date,
			husband_id,
			wife_id
		);

		m_command.Format(L"INSERT INTO marriages (%s) VALUES (%s)", m_columnsMarriages, values);
		if (!theApp.execute(m_command)) return false;

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	theApp.execute(L"COMMIT");
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGEDCOM::clearIndi()
{
	I.birth_date.Empty();
	I.birth_place.Empty();
	I.comment.Empty();
	I.death_date.Empty();
	I.death_place.Empty();
	I.father_id.Empty();
	I.first_name.Empty();
	I.https.Empty();
	I.last_name.Empty();
	I.linenumber = 0;
	I.mother_id.Empty();
	I.numOfSpouses = 0;
	I.occupation.Empty();
	I.whichChildFather = 0;
	I.whichChildMother = 0;
	I.parentIndex = 0;
	I.posterior.Empty();
	I.refI.Empty();
	I.rowid.Empty();
	I.sex.Empty();
	I.source.Empty();
	I.tablenumber = L"1";
	I.title.Empty();
	I.titolo.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGEDCOM::clearFAM()
{
	F.cnt = 0;
	F.date.Empty();
	F.marriageH = 0;
	F.marriageHAll = 0;
	F.marriageW = 0;
	F.marriageWAll = 0;
	F.numOfChildren = 0;
	F.place.Empty();
	F.refF.Empty();
	F.refH.Empty();
	F.refW.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CInputGEDCOM::gedCHAR(CString fileSpec)
{
	CString cLine;
	CString code;
	int		pos;
	int		kod = -1;

	if (!theApp.gedFileExists()) return -1;
	theApp.m_inputCode = GetInputCode(theApp.m_gedPathName);
	gedFile.Open(theApp.m_gedPathName, CFile::modeRead);

	while (gedFile.ReadString(cLine))
	{
		if (cLine.Find(L"CHAR") != -1)
			break;
	}
	gedFile.Close();
	if ((pos = cLine.Find(L"CHAR")) == -1) return -1; // nincs CHAR
	code = cLine.Mid(pos + 5);
	if (code == L"IBM WINDOWS")
		kod = ANSI;
	else if (code == L"UTF8")
		kod = UTF8;
	return kod;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGEDCOM::getRowid( CString ref )
{
	if (ref.IsEmpty()) return ref;

	for (UINT i = 0; i < v_INDI.size(); ++i)
	{
		if (ref == v_INDI.at(i).refI)
			return v_INDI.at(i).rowid;
	}
	return L"";
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A people táblában lévõ emberek father_id és mother_id értékeit update-eli a v_fam és v_chil alapján
void CInputGEDCOM::updateParents()
{
	CString refF;
	CString xref_child;

	CString rowid_father(L"");
	CString rowid_mother(L"");
	CString rowid_child(L"");

	CProgressWnd wndP(NULL, L"Szülõk és gyerekek összekapcsolása...");
	wndP.GoModal();
	wndP.SetRange(0, v_FAM.size());
	wndP.SetPos(0);
	wndP.SetStep(1);

	if (!theApp.execute(L"BEGIN")) return;
	for (UINT i = 0; i < v_FAM.size(); ++i)
	{
		refF = v_FAM.at(i).refF;
		rowid_father = getRowid(v_FAM.at(i).refH);
		rowid_mother = getRowid(v_FAM.at(i).refW);
		rowid_mother.Trim();
		for (UINT j = 0; j < v_CHIL.size(); ++j)
		{
			if (v_CHIL.at(j).refF == refF)
			{
				xref_child = v_CHIL.at(j).refC;
				for (UINT k = 0; k < v_INDI.size(); ++k)
				{
					if (v_INDI.at(k).refI == xref_child)
					{
						rowid_child = v_INDI.at(k).rowid;
						m_command.Format(L"UPDATE people SET father_id='%s', mother_id='%s' WHERE rowid = '%s'", rowid_father, rowid_mother, rowid_child);
						if (!theApp.execute(m_command)) return;
						break;
					}
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	if (!theApp.execute(L"COMMIT")) return;
}
