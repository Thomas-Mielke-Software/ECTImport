// EasyCashDoc.h : Exzerpt aus der offiziellen EC&T Dokument Klassenfamilie
//
/////////////////////////////////////////////////////////////////////////////

// HINWEIS:
// Diese Klassen werden benötigt, weil das Plugin ursprünglich noch unter dem alten 
// Interface mit einer MFC extension dll geschrieben wurde. Die Buchung wird erst
// zum Schluss in OLE Typen verwandelt und übertragen. Intern arbeitet das Plugin 
// bis dahin mit diesen historischen Klassen. Ein Rückgriff auf die ECTIFace.dll ist
// leider nicht möglich, da MFC42 und MFC90 bei der CString Klasse inkompatibel sind.

class CImportBuchung : public CObject
{
public:
	CImportBuchung();
	~CImportBuchung();
	CImportBuchung& operator=(const CImportBuchung& buchungSrc);
	int Betrag;	// brutto, in Cents
	int MWSt;	// in Prozent*1000
	int AbschreibungNr;			// Achtung: bei der Abschreibungsprozedur kann sich noch was 
	int AbschreibungJahre;		//          ändern: Halbjahresregel und degressive Abschreibung
	CString Beschreibung;		// Der Beschreibungstext
	CTime Datum;				// Buchungsdatum
	CString Konto;				// virtuelles Konto in Reintext
	CImportBuchung *next;				// Zeiger auf die nächste Buchung in der Liste
	// dazugekommen ab v1.16 oder CDoc-VERSION 7 (siehe oben #define VERSION)
	CString Belegnummer;		// auf vielfachen Wunsch dazugekommen in Version 7 der Doc-Klasse
	int  AbschreibungRestwert;	// Restbuchwert des Anlagegegenstands
	BOOL AbschreibungDegressiv;	// TRUE wenn Abschreibungsmodus degressiv ist
	int  AbschreibungSatz;		// zur Zeit nur bei degressiver Abschreibung benötigt
	CString Erweiterung;		// member für benutzerdefinierte Daten der Erweiterungs-DLLs
	long GetNetto();			// Gesamt-Nettobetrag
	long AbschreibungGenauigkeit; // 0 = Jahres-AfA, 1 = Halbjahres-AfA, 2 = monatsgenaue AfA, 3 entsp. den aktuellen Einstellungen
	CString Betrieb;			// Betrieb in Reintext
	CString Bestandskonto;		// Bestandskonto (Finanzkonto) in Reintext
};

// this one actually comes from the ECTIFace.dll (possible due to no CString used)
extern "C" AFX_EXT_CLASS BOOL GetIniFileName(char *buffer, int size);