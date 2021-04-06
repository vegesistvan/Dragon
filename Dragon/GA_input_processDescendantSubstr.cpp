#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_input.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A d struktúra elõállítása insertálásra
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaInput::processDescendantSubstring( CString cLine )
{
	TCHAR	generation;
	int	pos;
	int i;


	if( (pos = cLine.Find( ',' ) ) != -1 )
		pos = 0;


	generation = cLine.GetAt(0);				// generáció karakterének leszedése
	
	
	if( generation < m_generationPrev ) m_known_as.Empty();
	
	if( vParent2Index.size() )			// ha visszalép a generation, akkor törli az utolsó generációs bejegyzéseket
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

	cLine = cLine.Mid(2);						// G és szóköz leszedése
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
		d.tableRoman = m_tableHeader.tableRoman;  // csak az õst jelöli meg, hogy az összekötésnél megtalálják
	else
		d.tableRoman.Empty();

// Megjelölöli minden tábla ösével azonos generációt, hogy a connectBranches() mindegyiket beállíthassa az õt hívó másik táblában lévõ 
// szülökre.

	d.tableAncestry	= m_tableAncestry;
	d.source		= 1;

	// egy dummy családnevet tesz elé, és utána írja be a helyes elõ- és családnevet
	// Ez akkor jó pl. ha Farkas Jánost kellene szétbontani, ahol Farkas kerszetnév is lehet
	str.Format( L"XXXX %s", cLine );
	cLine = str.Trim();

	processPeopleStr( cLine, &d );
	d.titolo = m_tableHeader.titolo;
//	d.last_name = m_tableHeader.familyName;
	d.last_name = m_familyName;

	// a parentIndex csak ott van megadva, ahol változás történt, az ezt követõ gyerekek értelemszerûen ugyanannak az anyának a gyerekei.
	// parentIndex-t kell kiírni gyerekhez a leszármazotti listánál ( 0-t nem kell )
	// parentIndexCalc-t pedig minden gyereknél meg van adva, az  anya meghatározásához használjuk
	d.parentIndexCalc	= getParent2Index( generation, d.parentIndex );		// a felülírt index
//	d.orderMother		= m_orderMother;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A generation-parentIndex-bõl meghatározza az parentIndexCalc-t, ami az anya indexe a v_generation.spouse_id[parentIndexCalc]-ben 
// Mert a leszármazoitti listán csak az elsõ gyereknél van a parentIndex, az utána következõknél nem!
// Ezért határozza meg a generáció utolsó parentIndex-ét, amit visszaad 
//
// Visszamegy az vParent2Index vektorban a megadott generációhoz, ha volt ilyen. és annak mother-indexét visszadja
// és a ezzel a parentIndex-el és az aktuális generációval beteszi a vParent2Index vektorba.
// Ha nem talál azono sgenerációt, akkor mother_indexbe 1-et tesz.
// A vParent2Index vektorban gyûjti a táblában lévõ generációk utolsó parentIndex-ét, ami a leszármazott keresztneve után van megadva. (/n)
// Ha nincs megadva a leszármazott neve után index, akkor 1-et tesz bele
int CGaInput::getParent2Index( TCHAR generation, int motherIndex )
{
	PARENT2INDEX mx;
	
	int parentIndex = motherIndex;
	m_orderMother = 1;

	if( motherIndex == 0 )   // ha a /n nincs megadva, akkor megnézi hogy volt-e már korábban ez a generáció?
	{
		int i;
		for( i= vParent2Index.size() - 1; i >= 0; i--)		// visszafele keresi a legutóbbi azonos generációt
		{
			if( vParent2Index.at(i).generation == generation )
			{
				parentIndex = vParent2Index.at(i).parentIndex;  // ha talált, akkor azt használja
				if( parentIndex == motherIndex )
				{
					m_orderMother = vParent2Index.at(i).orderMother;  // ha talált, akkor azt használja
					++vParent2Index.at(i).orderMother;
				}
				else
					
				break;
			}
		}

		if( i == -1 )														// ha nem talált, akkor 1
		{
			parentIndex = 1;
			m_orderMother = 1;
		}
		else
			++vParent2Index.at(i).orderMother = 1;
	}

	mx.generation	= generation;									// az adott generáció utolsó indexe
	mx.parentIndex = parentIndex;									// elteszi az aktuális beállítást
	mx.orderMother	= 1;
	vParent2Index.push_back( mx );
	return parentIndex;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaInput::getOrderMother( TCHAR generation, int parentIndex )
{
	PARENT2INDEX mx;
	int orderMother = 1;

	if( parentIndex == 0 )   // ha a /n nincs megadva, akkor megnézi hogy volt-e már korábban ez a generáció?
	{
		int i;
		for( i= vParent2Index.size() - 1; i >= 0; i--)		// visszafele keresi a legutóbbi azonos generációt
		{
			if( vParent2Index.at(i).generation == generation )
			{
				parentIndex = vParent2Index.at(i).parentIndex;  // ha talált, akkor azt használja
				orderMother = vParent2Index.at(i).orderMother;
				break;
			}
		}
		if( i == -1 )														// ha nem talált, akkor 1
		{
			orderMother = 1;
		}
	}

	mx.generation	= generation;									// az adott generáció utolsó indexe
	mx.parentIndex = parentIndex;									// elteszi az aktuális beállítást
	vParent2Index.push_back( mx );
	return parentIndex;
}

