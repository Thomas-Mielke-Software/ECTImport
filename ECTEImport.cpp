/*//////////////////////////////////////////////////////////////////////////////
// Name:        ecteimport.cpp
// Purpose:     DLL main file, includes exports for EasyCash&Tax plugin infrastructure
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include <afxdllx.h>

#include "DlgImportDescr.h"
#include "DlgImportStatus.h"
#include "Import.h"
#include "Module.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern "C" int APIENTRY DllMain ( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
	
  if ( dwReason == DLL_PROCESS_ATTACH )
  {
		TRACE0("ECTEIMPORT.DLL Initializing!\n");

    if ( !AfxInitExtensionModule ( ECTEImportDLL, hInstance ) )
			return 0;

		new CDynLinkLibrary ( ECTEImportDLL );
	}
	else 
    if ( dwReason == DLL_PROCESS_DETACH )
    {
		  TRACE0 ("ECTEIMPORT.DLL Terminating!\n");
		  AfxTermExtensionModule ( ECTEImportDLL );
	  }

  return 1;
}


// Callback funktionen, die von EasyCT.exe aufgerufen werden:
extern "C" __declspec(dllexport) LPCSTR ECTE_Init(LPCSTR version);
extern "C" __declspec(dllexport) void ECTE_Exit(void *unused);
extern "C" __declspec(dllexport) void ECTE_OpenDocument(CEasyCashDoc *pDoc);
extern "C" __declspec(dllexport) void ECTE_UpdateDocument(CEasyCashDoc *pDoc);
extern "C" __declspec(dllexport) void ECTE_CloseDocument(CEasyCashDoc *pDoc);
extern "C" __declspec(dllexport) void ECTE_Menu(CEasyCashDoc *pDoc);



// Wird von der ECT-Applikationsklasse bei OnInitInstance direkt 
// nach dem Laden der DLL aufgerufen.
// 'version' enthält die Versionsnummer der aufrufenden EasyCT.exe
// in der Form (Beispiel) "v1.09"
extern "C" LPCSTR ECTE_Init ( LPCSTR version )
{
	int major = atoi ( strchr ( version, 'v') + 1 );
	int minor = atoi ( strchr ( version, '.') + 1 );
	int majorminor = major * 100 + minor;

	if ( majorminor < 109 )
  {
    AfxMessageBox ( IDS_PLUGIN_VERSIONERROR, MB_OK | MB_ICONSTOP );
    return "";
  }

  // Name des Eintrages im Erweiterungs-Menü
  // MenuItemName ist in modules.h definiert
  LoadString ( ECTEImportDLL.hResource, IDS_PLUGIN_MENUENTRY, MenuItemName, sizeof ( MenuItemName ) / sizeof ( TCHAR ) );
  return MenuItemName;
}


// Wird von der ECT-Applikationsklasse bei OnExitInstance aufgerufen bevor EasyCT beendet wird.
extern "C" void ECTE_Exit ( void *unused )
{
  return;
}


// wird von der CDocument-Klasse OnNewDocument und OnOpenDocument aufgerufen  
extern "C" void ECTE_OpenDocument ( CEasyCashDoc *pDoc )
{
	return;
}


// wird von der CView-Klasse OnUpdate aufgerufen, wenn sich das Dokument ändert
extern "C" void ECTE_UpdateDocument ( CEasyCashDoc *pDoc )
{
  return;
}


// wird von der CDocument-Klasse OnCloseDocument aufgerufen  
extern "C" void ECTE_CloseDocument ( CEasyCashDoc *pDoc )
{
	return;
}



// wird von der CView-Klasse bei der Anwahl des Menüpunktes im plug-in Menü aufgerufen  
extern "C" void ECTE_Menu ( CEasyCashDoc *pDoc )
{
	// no document, no work!
  if ( !pDoc )
    return;

  // declare vars 
  CImport Import;
  CImportParamsList ImportParamsList;
  CDlgImportDescr *StandardDlg = NULL;
  int Res;

  // load settings from file
  if ( !ImportParamsList.LoadFromIniFile() )
  {
    // display error message and remove the eventually already loaded items
    AfxMessageBox ( IDS_ERROR_LOADFROMINIFILE );
    ImportParamsList.RemoveAll();
  }

  // init CImport
  Import.SetDoc ( pDoc );
  Import.SetParams ( NULL );

  StandardDlg = new CDlgImportDescr ( NULL );
  StandardDlg->SetDoc ( pDoc );
  StandardDlg->SetImportParamsList ( &ImportParamsList );
  Res = StandardDlg->DoModal();
  
  if ( Res == IDOK )
    Import.AssignParams ( StandardDlg->GetParams() );

  delete StandardDlg;

  // save settings
  if ( !ImportParamsList.SaveToIniFile () ) 
    AfxMessageBox ( IDS_ERROR_SAVETOINIFILE );

  // user pressed OK: so do import the stuff
  if ( Res == IDOK ) 
  {
    // set wait cursor
    AfxGetApp()->BeginWaitCursor();

    // run import
    if ( Import.Execute() ) 
    {
      CString Msg;
      Msg.Format ( IDS_IMPORTSUCCESFUL, Import.GetImportCount() );
      AfxMessageBox ( Msg, MB_OK | MB_ICONINFORMATION );
      pDoc->UpdateAllViews ( NULL, 0, NULL );
    }
    else 
    {
      CDlgImportStatus StatusDlg;
      StatusDlg.SetMessages ( Import.GetErrors() );
      StatusDlg.DoModal();
    }

    // reset wait cursor
    AfxGetApp()->EndWaitCursor();
  }
}
