// SamePeopleInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "ContractInfo.h"
#include "afxdialogex.h"


// CContractInfo dialog

IMPLEMENT_DYNAMIC(CContractInfo, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractInfo::CContractInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CContractInfo::IDD, pParent)
	, m_info(_T(""))
{

	m_title = L"Azonos nev� emberek bejegyz�seinek vizsg�lata �s �sszevon�sa";
	m_info = L"\
Az al�bbi adatokat vizsg�ljuk az azonos nev� emberek azonoss�g�nak eld�nt�s�hez:\r\n\
\r\n\
 1. sz�let�si d�tuma\r\n\
 2. hal�loz�si d�tuma\r\n\
 3. apa neve\r\n\
 4. apa sz�let�si d�tuma\r\n\
 5. apa hal�loz�si d�tuma\r\n\
 6. anya neve\r\n\
 7. anya sz�let�si d�tuma\r\n\
 8. anya hal�loz�si d�tuma\r\n\
 9. h�zast�rs neve\r\n\
10. h�zast�rs sz�let�si d�tuma\r\n\
11. h�zast�rs hal�loz�si d�tuma\r\n\
12. esk�v� d�tuma\r\n\
\r\n\
Ha k�t bejegyz�s fenti adatokp�rjai k�z�tt ak�r egy ellentmond�s van, akkor nyilv�nval�an nem azonos szem�ly bejegyz�seir�l van sz�.\r\n\
Ha nincs ellentmond�s, akkor 2 adatp�r l�tez�s�t �s egyez�se�t k�vetelj�k meg az azonoss�g meg�llap�t�s�hoz.\r\n\
Lesz�rmazottakat akkor sem egyes�t�nk, ha mindenben megfelelel az �sszevon�si krit�riumoknak, mert lesz�rmazottk�nt \n\
csak egyszer szerepelhet egy ember.\r\n\
T�bb h�zast�rs eset�n az azonos�t�shoz elegend�, ha egy azonos nev� h�zast�rs szerepel mindk�t bejegyz�sben.\r\n\
A kett�s keresztnevekb�l csak az els� haszn�ljuk az �sszehasonl�t�shoz.\r\n\
\r\n\
Ha van olyan 2., 3. vagy 4. rend� bejegyz�s, amelyben csak a n�v van megadva, �s az megegyezik egy 1. rend� bejegyz�s \
nev�vel valamint h�zast�rsaik neve is azonos, tov�bb� az ilyen 1. rend� bejegyz�sb�l csak egy l�tezik, akkor azokat a \
bejegyz�seket �sszevonjuk, annak ellen�re, hogy nincs legal�bb 2 azonos adata. Hiszen nem is lehet, mert a n�ven k�v�l nincs m�s adat megadva.\r\n\
\r\n\
Az adathib�k nyilv�n egyes�t�si hib�khoz vezetnek, val�j�ban k�l�nb�z� emberekhez tartoz� bejegyz�sket t�vesen \
egyes�t�nk, vagy azonos emberek bejegyz�seinek �sszevon�sa elmarad. Ez�rt az egyes�t�s el�tt minden hib�t \
ki kell jav�tani!.\r\n\
A hib�kra az \"Ellen�rz�se egyes�t�s el�tt\" funkci�k vil�g�tanak r�. Az egyes�t�s sor�n az azonos�t�s erdm�ny�t tartalmaz� \
t�bl�zat, f�jlok is felfedhetnek hib�kat.\r\n\
\r\n\
Az oszlopok jelent�se:\r\n\
\r\n\
line\ta bejegyz�s sorsz�ma a GA html f�jlban.\r\n\
U\tunited, az ember u sz�m� bejegyz�s �sszevon�sa.\r\n\
G\tgener�ci�, az ember gener�ci�s jele a GA f�jlban.\r\n\
S\taz ember el�fordul�sa a GA.html f�jlban ( 1-2-3-4 )\r\n\
\r\n\
rowid\ta bejegyz�s azonos�t�ja\r\n\
n�v\taz ember neve (*sz�let�s +hal�loz�s)\r\n\
rowid\taz apa bejegyz�s�nek azonos�t�ja\r\n\
apa neve (*sz�let�s +hal�loz�s)\r\n\
rowid\taz anya bejegyz�s�nek azonos�t�ja\r\n\
anya neve (*sz�let�s +hal�loz�s)\r\n\
\r\n\
rowid\t az els� h�zast�rs azonos�t�ja\r\n\
h�zast�rsak felsorol�sa sz�let�si �s hal�loz�si d�tumukkal valamint az esk�v� d�tum�val\r\n\
\r\n\
Az azonos�t�s eredm�ny�t a sorok h�tt�r sz�nez�se is kiemeli:\r\n\
\r\n\
feh�r\tegyik bejegyz�ssel sem azonos\r\n\
sz�nes\taz ezt k�vet� sz�rke h�tter� bejegyz�sekkel azonos, azokkal �ssze lett vonva\r\n\
\r\n\
A jobb eg�rgombbal a t�bl�zat egy sor�ra kattintva egy leg�rd�l� men�b�l v�laszthatunk hasznos funkci�kat a GA-html f�jl vizsg�lat�hoz, jav�t�s�hoz.\r\n\
Az �sszehasonl�t�s, �sszevon�s eredm�ny�t k�t html f�jlban is el��ll�tja a program, amiket a html men�ponttal n�zhet�nk meg.\
";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractInfo::~CContractInfo()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, EditCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContractInfo, CDialogEx)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW( m_title );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( m_info );
	GetDlgItem(IDC_EDIT)->ModifyStyle(0, WS_DISABLED);
	return FALSE; 
}

