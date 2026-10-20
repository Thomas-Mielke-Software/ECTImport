// ECTImportX.cpp : Implementation of CECTImportXApp and DLL registration.

#include "stdafx.h"
#include "ECTImportX.h"
#include "easycashdoc.h"
#include "DlgImportDescr.h"
#include "import.h"
#include "DlgAbout.h"
#include "BuchungCtrl.h"
#include "EinstellungCtrl.h"
#include "ECTImportXCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CECTImportXApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x3BB16BA6, 0xDAC5, 0x40ED, { 0xB3, 0xB9, 0x8A, 0x2F, 0x36, 0x44, 0xE3, 0x26 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CECTImportXApp::InitInstance - DLL initialization

BOOL CECTImportXApp::InitInstance()
{
	//AfxOleInit(); --> in Debug-Mode 'calling it twice' assertion
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		AfxEnableControlContainer();	// wichtig, um auf die EC&T-Controls zuzugreifen!

		// Pre-Load von EasyCTX.ocx in den Prozess. Ohne diesen Schritt
		// scheitert in der alten easyct.exe v3.6 das In-Place-Aktivieren
		// der drei in CDlgImportDescr eingebetteten ActiveX-Controls
		// (DokumentCtrl, BuchungCtrl, EinstellungCtrl) mit einem
		// ASSERT(IsWindow(pTemp->m_hWnd)) in
		// COleControlContainer::FillListSitesOrWnds -- weil die alte
		// Host-Apartment EasyCTX.ocx zur Dialog-Init-Zeit nicht
		// nachladen kann. Frueher wurde die OCX als Seiteneffekt eines
		// COleDispatchDriver::CreateDispatch-Aufrufs aus LoadFromIniFile
		// vorgeladen; den habe ich entfernt, also holen wir das hier
		// explizit nach. CLSID DFA091A6-D040-444F-800A-656BEA3361C3 ist
		// CEinstellungCtrl aus EasyCTX.ocx.
		static const CLSID clsid_Einstellung =
			{ 0xDFA091A6, 0xD040, 0x444F,
			  { 0x80, 0x0A, 0x65, 0x6B, 0xEA, 0x33, 0x61, 0xC3 } };
		LPUNKNOWN pUnk = NULL;
		HRESULT hr = CoCreateInstance ( clsid_Einstellung, NULL,
		                                CLSCTX_INPROC_SERVER,
		                                IID_IUnknown, (void**)&pUnk );
		if ( SUCCEEDED(hr) && pUnk )
			pUnk->Release();	// EasyCTX.ocx bleibt nach Release im
		                        // Prozess geladen, das reicht uns.
		else
			TRACE ( "ECTImportX: CoCreateInstance(EinstellungCtrl) fuer "
			        "Pre-Load von EasyCTX.ocx fehlgeschlagen: 0x%08X\n",
			        hr );
	}

	return bInit;
}



// CECTImportXApp::ExitInstance - DLL termination

int CECTImportXApp::ExitInstance()
{
	//CoUninitialize();

	// Vom Host nicht releasede Plugin-Instanzen vor dem CRT-Teardown
	// freigeben. Sonst meldet der Leak-Detector das CECTImportXCtrl-Objekt
	// (536 B) plus seinen ctlview.cpp-Tracking-Eintrag (12 B) als Leak.
	// Die im Ctor angelegte und im Dtor gepflegte intrusive Liste
	// CECTImportXCtrl::s_pFirstInstance macht das Aufraeumen O(N) und
	// allokiert selbst keinen zusaetzlichen Speicher.
	CECTImportXCtrl::CleanupLeftoverInstances();

	// Keep-Alive-Referenz auf EasyCTX.ocx freigeben.
	if ( m_pEasyCTXKeepAlive )
	{
		m_pEasyCTXKeepAlive->Release();
		m_pEasyCTXKeepAlive = NULL;
	}

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


// globale Hilfsfunktionen

extern void Ansi2Utf8(CString ansiText, CStringA& utf8Text)
{
	// UTF-8-Konvertierung (für Konvertierung in UTF-8 erst mal in UTF-16 konvertieren: CP_ACP -> CP_UTF16 -> CP_UTF8)
	CStringW utf16String('\0', MultiByteToWideChar(CP_ACP, 0, ansiText.GetBuffer(), -1, NULL, 0));
	MultiByteToWideChar(CP_ACP, 0, ansiText.GetBuffer(), -1, utf16String.GetBuffer(), utf16String.GetLength());
	CStringA utf8String('\0', WideCharToMultiByte(CP_UTF8, 0, utf16String.GetBuffer(), -1, NULL, 0, NULL, NULL));
	WideCharToMultiByte(CP_UTF8, 0, utf16String.GetBuffer(), -1, utf8String.GetBuffer(), utf8String.GetLength(), NULL, NULL);
	utf8Text = utf8String;
}

extern void Utf8toAnsi(CStringA utf8Text, CString& ansiText)
{
	// UTF-8-Konvertierung (für Konvertierung in ANSI erst mal in UTF-16 konvertieren: CP_UTF8 -> CP_UTF16 -> CP_ACP)
	CStringW utf16String('\0', MultiByteToWideChar(CP_UTF8, 0, utf8Text.GetBuffer(), -1, NULL, 0));
	MultiByteToWideChar(CP_UTF8, 0, utf8Text.GetBuffer(), -1, utf16String.GetBuffer(), utf16String.GetLength());
	CStringA ansiString('\0', WideCharToMultiByte(CP_ACP, 0, utf16String.GetBuffer(), -1, NULL, 0, NULL, NULL));
	WideCharToMultiByte(CP_ACP, 0, utf16String.GetBuffer(), -1, ansiString.GetBuffer(), ansiString.GetLength(), NULL, NULL);
	ansiText = ansiString;
}