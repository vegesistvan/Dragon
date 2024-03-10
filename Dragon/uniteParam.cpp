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
Az azonos nev� bejegyz�sek helyes �sszevon�sa alapvet� a csal�di kapcsolatok l�trehoz�s�ra, a helyes lesz�rmazotti �s felmen�i list�k el��ll�t�s�hoz.\r\n\
Sok esetben az azonos nev� bejegyz�sek csak a h�zast�rs nev�ben egyeznek, tov�bbi inform�ci� nem �ll rendelkez�sre annak eld�nt�s�hez, \
hogy egyes�thet�ek vagy sem. \r\n\
Ebben az esetben egyes kapcsolatokn�l az �sszevon�s a helyes, m�s esetekben pedig az, hogy nem vonjuk �ssze ezeket a bejegyz�seket.\r\n\
\r\n\
Nincs teh�t minden esetben helyes megold�s!\r\n\
\r\n\
Ha egy adott lesz�rmazotti vagy felmen�i list�n�l azt l�tjuk, hogy hib�s a lista, akkor megpr�b�lkozhatunk a m�sik �sszevon�ssal. El�fordulhat, hogy \
m�s hib�k keletkeznek a list�ban, de lehets�ges, hogy megold�st ad a probl�m�ra.\r\n\
\r\n\
Az alapbe�ll�t�s minden esetben az, hogy ne vonja �ssze azokat a bejegyz�seket, amelyekben csak a h�zast�rsak neve azonos. \
Ha elmaradnak a k�v�natos �sszevon�sok, azokat egyenk�nt k�zzel is elv�gezhetj�k az �sszevon�s eredm�ny�nek list�in, de legink�bb az az aj�nl�s, hogy \
pr�b�ljuk olyan tov�bbi adatot megadni a ga.htm f�jlban, amely biztos�tja az �sszevon�st.\r\n\
\r\n\
Az al�bbi param�terrel be�ll�thatjuk, hogy k�v�njuk egyes�teni a bejegyz�seket.\r\n\
\r\n\
Ak�rhogy is v�lasztunk, az adatb�zisban nagy val�sz�n�s�ggel hib�k lesznek, csak egy konkr�t probl�m�ra rem�lhet�nk megold�st!!!!!!!!!\
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
