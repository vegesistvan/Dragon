// SamePeopleInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "uniteInfo.h"
#include "afxdialogex.h"


// CUniteInfo dialog

IMPLEMENT_DYNAMIC(CUniteInfo, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteInfo::CUniteInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUniteInfo::IDD, pParent)
	, m_info(_T(""))
{

	m_title = L"Azonos nev� emberek bejegyz�seinek vizsg�lata �s �sszevon�sa";
	m_info = L"\
K�t azonos nev� ember bejegyz�seinek �sszevonhat�s�g�hoz az al�bbi rokons�g nev�t, sz�let�si �s hal�loz�si d�tum�t vizsg�ljuk:\r\n\
\r\n\
1. maguk a k�t azonos nev� emberek\r\n\
2. az ap�kat\r\n\
3. az any�kat\r\n\
4. az apai nagyap�kat\r\n\
5. az apai nagyany�kat\r\n\
6. az anyai nagyap�kat\r\n\
7. az anyai nagyany�kat\r\n\
8. egy k�z�s nev� h�zast�rsat\r\n\
9. a h�zast�rsak apj�t\r\n\
10.a h�zast�rsak anyj�t\r\n\
\r\n\
Ha k�t bejegyz�s fenti adatp�rjai k�z�tt ak�r egy ellentmond�s van, akkor azokat k�l�nb�z� emberek bejegyz�seinek tekintj�k.\r\n\
\r\n\
A kett�s keresztnevekb�l csak az els� haszn�ljuk az �sszehasonl�t�shoz.\r\n\
Lesz�rmazottakat semmik�ppen nem egyes�t�nk, mert �k csak egyszer fordulhatnak el�.\r\n\
T�bb h�zast�rs eset�n az azonos�t�shoz elegend�, ha egy azonos nev� h�zast�rs ellentmond�s menetesen szerepel mindk�t bejegyz�sben.\r\n\
\r\n\
Az adathib�k nyilv�n egyes�t�si hib�khoz vezetnek, k�l�nb�z� emberekhez tartoz� bejegyz�sket t�vesen egyes�thet�nk\
vagy azonos emberek bejegyz�seinek �sszevon�sa elmarad. Ez�rt az egyes�t�s el�tt t�rkedni kell a hib�k kijav�tan�s�ra.\r\n\
A hib�kra az \"Ellen�rz�se egyes�t�s el�tt\" funkci�k vil�g�tanak r�. Innne ind�tva, ezeket az egyes�t�si list�kat �gy k�sz�ti el, hogy nem \
t�rli a t�rlend� bejegyz�seket, csak sz�rke sz�nnel jelzi �ket. �gy lehet�s�g�nk van r� hogy megvizsg�juk, mi�rt min�s�ttte �gy a bejegyz�seket, \
ahogy min�s�tette.\r\n\
\r\n\
Az oszlopok jelent�se:\r\n\
\r\n\
loop\taz �sszevon�s iter�ci�j�nak sorsz�ma\r\n\
gr\tcsoport azonos�t�, amibe az �sszevont emberek tartoznak\r\n\
st\tst�tusz: 1-megtartott bejegyz�s, 0-t�r�lt bejegyz�s\r\n\
u\tunited, az ember u sz�m� bejegyz�s �sszevon�sa.\r\n\
s\taz ember el�fordul�sa a GA.html f�jlban(1 - 2 - 3 - 4)\r\n\
\r\n\
rowid\ta bejegyz�s azonos�t�ja, neve, *sz�let�se, +hal�loz�sa\r\n\
rowid\taz apa bejegyz�s�nek azonos�t�ja, neve, *sz�let�se, +hal�loz�sa\r\n\
rowid\taz anya bejegyz�s�nek azonos�t�ja, neve, *sz�let�se, +hal�loz�sq)\r\n\
h�zast�rsak felsorol�sa azonos�t� sz�mukkal, sz�let�si �s hal�loz�si d�tumukkal\r\n\
\r\n\
A 'List�k' men�pontb�l h�rom t�bl�zat valamelyik�t �br�zolhatjuk:\r\n\
\r\n\
1. Azonos nev� bejegyz�sek, amelyek k�z�tt tal�lt �sszevonhat�kat.\r\n\
2. N�v szerint csoportos�tott bejegyz�sek, amelyek k�z�tt nem tal�lt �sszevonhat�kat.\r\t\
3. Gyan�s bejegyz�sek, amelyek k�z�tt az el�rt sz��m� adatp�r egyez�st megtal�lta, de ellentmond�sos adatp�rok is vannak, \
ez�rt nem egyes�ti azokat.\r\n\
\r\n\
Az azonos�t�s eredm�ny�t a sorok h�tt�r sz�nez�se is kiemeli:\r\n\
\r\n\
feh�r\tegyik bejegyz�ssel sem azonos\r\n\
sz�nes\taz ezt k�vet� sz�rke h�tter� bejegyz�sekkel azonos, azokkal �ssze lett vonva, a sz�rk�k t�rlve lettek.\r\n\
\r\n\
A jobb eg�rgombbal a t�bl�zat egy sor�ra kattintva egy leg�rd�l� men�b�l v�laszthatunk hasznos funkci�kat a GA-html f�jl \
vizsg�lat�hoz, jav�t�s�hoz.\r\n\
A bal eg�rombbal k�tszer kattintva egy bejegyz�se, a sorban megadott azonos�t�j� ember rokoni kapcsolatait mutatja meg, amiben az ismert \
m�don navig�lhatunk. Ha k�t sort jel�l�nk ki, �s a bal eg�rgomb kattint�ssal leg�rd�l� men�r�l az \"Egyes�t�si adatok\" funkci�t v�lasztjuk, \
akkor a kijel�lt bejegyz�s fent felsorolt adtai, amiket az azonoss�g eld�nt�s�hetz haszn�lunk, egy ablakban megmutatja, jelezve a k�l�nbs�geket.\
";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteInfo::~CUniteInfo()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, EditCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUniteInfo, CDialogEx)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUniteInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW( m_title );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( m_info );
	GetDlgItem(IDC_EDIT)->ModifyStyle(0, WS_DISABLED);
	return FALSE; 
}

