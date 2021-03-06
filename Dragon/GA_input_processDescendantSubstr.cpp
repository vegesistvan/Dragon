#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A d strukt�ra el��ll�t�sa insert�l�sra
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processDescendantSubstring( CString cLine )
{
	TCHAR	generation;
	int	pos;
	int i;


	if( (pos = cLine.Find( ',' ) ) != -1 )
		pos = 0;


	generation = cLine.GetAt(0);				// gener�ci� karakter�nek leszed�se
	
	
	if( generation < m_generationPrev ) m_known_as.Empty();
	
	if( vParent2Index.size() )			// ha visszal�p a generation, akkor t�rli az utols� gener�ci�s bejegyz�seket
	{
		if( generation < m_generationPrev )
		{
			i = vParent2Index.size() - 1;
			while( i && generation < vParent2Index.at(i).generation )
			{
				vParent2Index.pop_back();
				--i;
			}
		}
	}
	m_generationPrev = generation;

	cLine = cLine.Mid(2);						// G �s sz�k�z leszed�se
	d.generation		= generation;
	if( generation == '%' )						// ez mi????????
	{
		d.generation = generation;
		d.last_name = cLine.Trim();
		return;
	}
	if( m_tableAncestry )
		m_generationFirst = generation;
	if( generation == m_generationFirst )
		d.tableRoman = m_tableHeader.tableRoman;  // csak az �st jel�li meg, hogy az �sszek�t�sn�l megtal�lj�k
	else
		d.tableRoman.Empty();

// Megjel�l�li minden t�bla �s�vel azonos gener�ci�t, hogy a connectBranches() mindegyiket be�ll�thassa az �t h�v� m�sik t�bl�ban l�v� 
// sz�l�kre.

	d.tableAncestry	= m_tableAncestry;
	d.source		= 1;

	// egy dummy csal�dnevet tesz el�, �s ut�na �rja be a helyes el�- �s csal�dnevet
	// Ez akkor j� pl. ha Farkas J�nost kellene sz�tbontani, ahol Farkas kerszetn�v is lehet
	str.Format( L"XXXX %s", cLine );
	cLine = str.Trim();

	processPeopleStr( cLine, &d );

	if( d.sex_id == 0 && ! v_marriages.size() ) d.sex_id = 1;	// ha a lesz�rmazott keresztnev�b�l nem lehet meg�llap�tani
																// nem�t �s h�zast�rsa sincs, akkor legyen f�rfi
	desc.nameSubstr		= m_nameSubstr;
	desc.birthSubstr	= m_birthSubstr;
	desc.deathSubstr	= m_deathSubstr;
	d.titolo = m_tableHeader.titolo;
//	d.last_name = m_tableHeader.familyName;
	d.last_name = m_familyName;

	// a parentIndex csak ott van megadva, ahol v�ltoz�s t�rt�nt, az ezt k�vet� gyerekek �rtelemszer�en ugyanannak az any�nak a gyerekei.
	// parentIndex-t kell ki�rni gyerekhez a lesz�rmazotti list�n�l ( 0-t nem kell )
	// parentIndexCalc-t pedig minden gyerekn�l meg van adva, az  anya meghat�roz�s�hoz haszn�ljuk
	d.parentIndexCalc	= getParent2Index( generation, d.parentIndex );		// a fel�l�rt index
//	d.orderMother		= m_orderMother;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A generation-parentIndex-b�l meghat�rozza az parentIndexCalc-t, ami az anya indexe a v_generation.spouse_id[parentIndexCalc]-ben 
// Mert a lesz�rmazoitti list�n csak az els� gyerekn�l van a parentIndex, az ut�na k�vetkez�kn�l nem!
// Ez�rt hat�rozza meg a gener�ci� utols� parentIndex-�t, amit visszaad 
//
// Visszamegy az vParent2Index vektorban a megadott gener�ci�hoz, ha volt ilyen. �s annak mother-index�t visszadja
// �s a ezzel a parentIndex-el �s az aktu�lis gener�ci�val beteszi a vParent2Index vektorba.
// Ha nem tal�l azono sgener�ci�t, akkor mother_indexbe 1-et tesz.
// A vParent2Index vektorban gy�jti a t�bl�ban l�v� gener�ci�k utols� parentIndex-�t, ami a lesz�rmazott keresztneve ut�n van megadva. (/n)
// Ha nincs megadva a lesz�rmazott neve ut�n index, akkor 1-et tesz bele
int CGaInput::getParent2Index( TCHAR generation, int motherIndex )
{
	PARENT2INDEX mx;
	
	int parentIndex = motherIndex;
	m_orderMother = 1;

	if( motherIndex == 0 )   // ha a /n nincs megadva, akkor megn�zi hogy volt-e m�r kor�bban ez a gener�ci�?
	{
		int i;
		for( i= vParent2Index.size() - 1; i >= 0; i--)		// visszafele keresi a legut�bbi azonos gener�ci�t
		{
			if( vParent2Index.at(i).generation == generation )
			{
				parentIndex = vParent2Index.at(i).parentIndex;  // ha tal�lt, akkor azt haszn�lja
				if( parentIndex == motherIndex )
				{
					m_orderMother = vParent2Index.at(i).orderMother;  // ha tal�lt, akkor azt haszn�lja
					++vParent2Index.at(i).orderMother;
				}
				else
					
				break;
			}
		}

		if( i == -1 )														// ha nem tal�lt, akkor 1
		{
			parentIndex = 1;
			m_orderMother = 1;
		}
		else
			++vParent2Index.at(i).orderMother = 1;
	}

	mx.generation	= generation;									// az adott gener�ci� utols� indexe
	mx.parentIndex = parentIndex;									// elteszi az aktu�lis be�ll�t�st
	mx.orderMother	= 1;
	vParent2Index.push_back( mx );
	return parentIndex;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaInput::getOrderMother( TCHAR generation, int parentIndex )
{
	PARENT2INDEX mx;
	int orderMother = 1;

	if( parentIndex == 0 )   // ha a /n nincs megadva, akkor megn�zi hogy volt-e m�r kor�bban ez a gener�ci�?
	{
		int i;
		for( i= vParent2Index.size() - 1; i >= 0; i--)		// visszafele keresi a legut�bbi azonos gener�ci�t
		{
			if( vParent2Index.at(i).generation == generation )
			{
				parentIndex = vParent2Index.at(i).parentIndex;  // ha tal�lt, akkor azt haszn�lja
				orderMother = vParent2Index.at(i).orderMother;
				break;
			}
		}
		if( i == -1 )														// ha nem tal�lt, akkor 1
		{
			orderMother = 1;
		}
	}

	mx.generation	= generation;									// az adott gener�ci� utols� indexe
	mx.parentIndex = parentIndex;									// elteszi az aktu�lis be�ll�t�st
	vParent2Index.push_back( mx );
	return parentIndex;
}

