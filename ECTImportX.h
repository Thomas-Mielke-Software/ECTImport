#pragma once

// ECTImportX.h : main header file for ECTImportX.DLL

#if !defined( __AFXCTL_H__ )
#error "include 'afxctl.h' before including this file"
#endif

#include "resource.h"       // main symbols


// CECTImportXApp : See ECTImportX.cpp for implementation.

class CDlgAbout;

class CECTImportXApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();

	// Keep-Alive-Referenz auf EasyCTX.ocx, gehalten von InitInstance bis
	// ExitInstance, damit MFCs Hosting der eingebetteten ActiveX-Controls
	// in CDlgImportDescr auch unter aelteren easyct.exe-Versionen sauber
	// laeuft. Ohne diese Referenz wirft FillListSitesOrWnds einen
	// ASSERT(IsWindow(m_hWnd)), weil die OCX zur Dialog-Init-Zeit nicht
	// im Prozess ist.
	LPUNKNOWN m_pEasyCTXKeepAlive = NULL;
};

extern CECTImportXApp theApp;

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

// globale Hilfsfunktionen
extern void Ansi2Utf8(CString ansiText, CStringA& utf8Text);
extern void Utf8toAnsi(CStringA utf8Text, CString& ansiText);