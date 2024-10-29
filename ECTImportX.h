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
};

extern CECTImportXApp theApp;

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

// globale Hilfsfunktionen
extern void Ansi2Utf8(CString ansiText, CStringA& utf8Text);
extern void Utf8toAnsi(CStringA utf8Text, CString& ansiText);