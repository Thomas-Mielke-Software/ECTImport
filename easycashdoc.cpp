// EasyCashDoc.cpp : Exzerpt aus der offiziellen EC&T Dokument Klassenfamilie
//

#include "stdafx.h"
#include <ctype.h>
#include "easycashdoc.h"

#define VERSION 9

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4996 )

#define _CRT_SECURE_NO_WARNINGS

/////////////////////////////////////////////////////////////////////////////
// CBetrag

CImportBuchung::CImportBuchung()
{
	Betrag					= 0;
	MWSt					= 0;
	AbschreibungNr			= 1;
	AbschreibungJahre		= 1;
	Beschreibung			= "";
	Datum					= CTime(2000, 1, 1, 0, 0, 0);
	Konto					= "";
	Belegnummer				= "";
	Erweiterung				= "";
	AbschreibungRestwert	= 0;
	AbschreibungDegressiv	= 0;
	AbschreibungSatz		= 0;
	AbschreibungGenauigkeit = 3;
	Bestandskonto			= "";	
	Betrieb					= "";		
	next = NULL;
}

CImportBuchung::~CImportBuchung()
{
	if (next) delete next;
}

// ohne next!!!
CImportBuchung& CImportBuchung::operator=(const CImportBuchung& buchungSrc)
{
	Betrag					= buchungSrc.Betrag;					// in Pfennigen
	MWSt					= buchungSrc.MWSt;
	AbschreibungNr			= buchungSrc.AbschreibungNr;
	AbschreibungJahre		= buchungSrc.AbschreibungJahre;
	Beschreibung			= buchungSrc.Beschreibung;
	Datum					= buchungSrc.Datum;
	Konto					= buchungSrc.Konto;
	Belegnummer				= buchungSrc.Belegnummer;
	Erweiterung				= buchungSrc.Erweiterung;
	AbschreibungRestwert	= buchungSrc.AbschreibungRestwert;	
	AbschreibungDegressiv	= buchungSrc.AbschreibungDegressiv;	
	AbschreibungSatz		= buchungSrc.AbschreibungSatz;		
	AbschreibungGenauigkeit	= buchungSrc.AbschreibungGenauigkeit;
	Bestandskonto			= buchungSrc.Bestandskonto;		
	Betrieb					= buchungSrc.Betrieb;		
	return *this;
}
long CImportBuchung::GetNetto()
{
	long netto;
	netto = Betrag;
	if (Betrag <= 1000000 && Betrag >= -1000000 && MWSt % 1000 == 0)
	{
		netto *= 1000;
		netto /= (100 + (MWSt / 1000));
		if (Betrag >= 0) // halber Pfennig zum Runden
			netto += 5;		
		else
			netto -= 5;
		netto /= 10;
	}
	else // bei zu großen Zahlen lieber double-float rechnen!
	{
		double temp = (double)netto * 100.0;
		temp /= (100.0 + ((double)MWSt / 1000.0));
		if (Betrag >= 0) // halber Pfennig zum Runden
			netto = (long)(temp + 0.5);
		else
			netto = (long)(temp - 0.5);
	}

	return netto;
}