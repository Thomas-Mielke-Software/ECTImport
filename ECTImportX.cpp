// ECTImportX.cpp : Implementation of CECTImportXApp and DLL registration.

#include "stdafx.h"
#include "ECTImportX.h"
#include "DlgAbout.h"
#include "BuchungCtrl.h"
#include "EinstellungCtrl.h"


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
	}

	return bInit;
}



// CECTImportXApp::ExitInstance - DLL termination

int CECTImportXApp::ExitInstance()
{
	//CoUninitialize();

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
