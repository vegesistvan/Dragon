// uniteParam.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "uniteParam.h"


IMPLEMENT_DYNAMIC(CUniteParam, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteParam::CUniteParam(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNITE_PARAM, pParent)
	, m_snameEnough(FALSE)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteParam::~CUniteParam()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, m_editCtrl);
	DDX_Radio(pDX, IDC_RADIO_ENOUGH, m_snameEnough);
	DDX_Radio(pDX, IDC_RADIO_NOTENOUGH, m_snameEnough);
}
BEGIN_MESSAGE_MAP(CUniteParam, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_NOTENOUGH, &CUniteParam::OnClickedRadioNotenough)
	ON_COMMAND(IDC_RADIO_ENOUGH, &CUniteParam::OnRadioEnough)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUniteParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString text = L"\
Az azonos nevû bejegyzések helyes összevonása alapvetõ a családi kapcsolatok létrehozására, a helyes leszármazotti és felmenõi listák elõállításához.\r\n\
Sok esetben az azonos nevû bejegyzések csak a házastárs nevében egyeznek, további információ nem áll rendelkezésre annak eldöntéséhez, \
hogy egyesíthetõek vagy sem. \r\n\
Ebben az esetben egyes kapcsolatoknál az összevonás a helyes, más esetekben pedig az, hogy nem vonjuk össze ezeket a bejegyzéseket.\r\n\
\r\n\
Nincs tehát minden esetben helyes megoldás!\r\n\
\r\n\
Ha egy adott leszármazotti vagy felmenõi listánál azt látjuk, hogy hibás a lista, akkor megpróbálkozhatunk a másik összevonással. Elõfordulhat, hogy \
más hibák keletkeznek a listában, de lehetséges, hogy megoldást ad a problémára.\r\n\
\r\n\
Az alapbeállítás minden esetben az, hogy ne vonja össze azokat a bejegyzéseket, amelyekben csak a házastársak neve azonos. \
Ha elmaradnak a kívánatos összevonások, azokat egyenként kézzel is elvégezhetjük az összevonás eredményének listáin, de leginkább az az ajánlás, hogy \
próbáljuk olyan további adatot megadni a ga.htm fájlban, amely biztosítja az összevonást.\r\n\
\r\n\
Az alábbi paraméterrel beállíthatjuk, hogy kívánjuk egyesíteni a bejegyzéseket.\r\n\
\r\n\
Akárhogy is választunk, az adatbázisban nagy valószínûséggel hibák lesznek, csak egy konkrét problémára remélhetünk megoldást!!!!!!!!!\
";

	m_editCtrl.SetWindowTextW(text);
	return TRUE; 
}

void CUniteParam::OnClickedRadioNotenough()
{
	m_snameEnough = false;
}
void CUniteParam::OnRadioEnough()
{
	m_snameEnough = true;
}
