// ECTImportXCtrl.cpp : Implementation of the CECTImportXCtrl ActiveX Control class.

#include "stdafx.h"
#include "easycashdoc.h"
#include "DlgImportDescr.h"
#include "import.h"
#include "DlgAbout.h"
#include "ECTImportX.h"
#include "ECTImportXCtrl.h"
#include "DlgImportStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CECTImportXCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CECTImportXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CECTImportXCtrl, COleControl)
	DISP_FUNCTION_ID(CECTImportXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CECTImportXCtrl, "Init", dispidInit, Init, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CECTImportXCtrl, COleControl)
END_EVENT_MAP()



// Property pages


// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CECTImportXCtrl, "ECTIMPORTX.ECTImportXCtrl.1",
	0x163097f8, 0x9d6, 0x4437, 0xb1, 0xdc, 0x16, 0xdc, 0xab, 0x67, 0xbc, 0x4d)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CECTImportXCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DECTImportX =
		{ 0xC0940CD6, 0xFE5D, 0x4F94, { 0xA8, 0xD9, 0x8B, 0xC0, 0xB3, 0x1, 0x98, 0xBC } };
const IID BASED_CODE IID_DECTImportXEvents =
		{ 0x1380B78A, 0x5545, 0x4E30, { 0x89, 0x55, 0xA7, 0xFD, 0x60, 0x57, 0xAB, 0x4B } };



// Control type information

static const DWORD BASED_CODE _dwECTImportXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CECTImportXCtrl, IDS_ECTIMPORTX, _dwECTImportXOleMisc)



// CECTImportXCtrl::CECTImportXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CECTImportXCtrl

BOOL CECTImportXCtrl::CECTImportXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ECTIMPORTX,
			IDB_ECTIMPORTX,
			afxRegApartmentThreading,
			_dwECTImportXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CECTImportXCtrl::CECTImportXCtrl - Constructor

CECTImportXCtrl::CECTImportXCtrl()
{
	InitializeIIDs(&IID_DECTImportX, &IID_DECTImportXEvents);

	m_pImportDlg = NULL;
	m_pImport = NULL;
	m_pImportParamsList = NULL;
}



// CECTImportXCtrl::~CECTImportXCtrl - Destructor

CECTImportXCtrl::~CECTImportXCtrl()
{
	if (m_pImport) delete m_pImport;
	if (m_pImportDlg) delete m_pImportDlg;
	if (m_pImportParamsList) delete m_pImportParamsList;
}



// CECTImportXCtrl::OnDraw - Drawing function

void CECTImportXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	if (m_pImportDlg && m_pImportDlg->m_hWnd) m_pImportDlg->MoveWindow(rcBounds, TRUE);
}



// CECTImportXCtrl::DoPropExchange - Persistence support

void CECTImportXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CECTImportXCtrl::OnResetState - Reset control to default state

void CECTImportXCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CECTImportXCtrl::AboutBox - Display an "About" box to the user

void CECTImportXCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ECTIMPORTX);
	dlgAbout.DoModal();
}



// CECTImportXCtrl message handlers

int CECTImportXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CECTImportXCtrl::Init(LONG dokID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pDoc = dokID;

	// load settings from file
	m_pImportParamsList = new CImportParamsList;
	if ( !m_pImportParamsList->LoadFromIniFile() )
	{
		// display error message and remove the eventually already loaded items
		AfxMessageBox ( IDS_ERROR_LOADFROMINIFILE );
		m_pImportParamsList->RemoveAll();
	}

	// init CImport
	m_pImport = new CImport();
	m_pImport->SetParams ( NULL );
	m_pImport->SetDoc ( m_pDoc );

	m_pImportDlg = new CDlgImportDescr ( this );
	m_pImportDlg->SetImportParamsList ( m_pImportParamsList );

	m_pImport->SetDokumentCtrl ( &m_pImportDlg->m_DokumentCtrl );
	m_pImport->SetBuchungCtrl ( &m_pImportDlg->m_BuchungCtrl );

	//m_pImportDlg->DoModal();
	HRESULT nResult = m_pImportDlg->Create(IDD_IMPORTDESCR, this);
	if (!nResult) 
		AfxMessageBox("Dialog Fenster konnte nicht erzeugt werden.");
	else 
		m_pImportDlg->ShowWindow(SW_SHOW);

	m_pImport->SetDokumentID ( m_pDoc );	// jetzt, wo die EC&T-Objekte leben, kann ihnen das Dokument-Handle übergeben werden







/*	this went to OnDestroy()...
		m_pImport->AssignParams ( m_pImportDlg->GetParams() );

	  // depending on list box selection, set bytes in array to 0 or 1 so Execute knows which lines to import
	  CByteArray* pSelectionArray;
		  pSelectionArray = NULL;	// import all
	  
	  // user pressed OK: so do import the stuff
	  {
		// set wait cursor
		AfxGetApp()->BeginWaitCursor();

		// run import
		//m_pImport->m_pParams->GetFieldKonto()->SetFestwert
		if ( !m_pImport->Execute(pSelectionArray) )
		{
		  CDlgImportStatus StatusDlg;
		  StatusDlg.SetMessages ( m_pImport->GetErrors() );
		  StatusDlg.DoModal();
		}

		// clean up
		if (pSelectionArray) delete pSelectionArray;

		// reset wait cursor
		AfxGetApp()->EndWaitCursor();
	  }
*/




}

void CECTImportXCtrl::OnDestroy()
{	
	if (!m_pImport || !m_pImportDlg) return;

	  if ( m_pImportDlg->m_Result == IDOK || m_pImportDlg->m_Result == IDRETRY )
		m_pImport->AssignParams ( m_pImportDlg->GetParams() );

	  // save settings
	  if ( !m_pImportParamsList->SaveToIniFile () ) 
		AfxMessageBox ( IDS_ERROR_SAVETOINIFILE );

	  // depending on list box selection, set bytes in array to 0 or 1 so Execute knows which lines to import
	  CByteArray* pSelectionArray;
	  if (m_pImportDlg->m_Result == IDRETRY) // IDRETRY = nur selektierte Buchungen importieren
	  {
		  pSelectionArray = new CByteArray;
		  int n = m_pImportDlg->m_FileContent.GetItemCount();		  
		  pSelectionArray->SetSize(n);

		  int row;
		  for (row = 0; row < n; row++)
			  pSelectionArray->SetAt(row, (BYTE)m_pImportDlg->m_FileContent.GetItemState(row, LVIS_SELECTED) ? 1 : 0 );
	  }
	  else
		  pSelectionArray = NULL;	// import all
	  
	  // user pressed OK: so do import the stuff
	  if ( m_pImportDlg->m_Result == IDOK || m_pImportDlg->m_Result == IDRETRY ) 
	  {
		// set wait cursor
		AfxGetApp()->BeginWaitCursor();

		// run import
		//m_pImport->m_pParams->GetFieldKonto()->SetFestwert
		if ( m_pImport->Execute(pSelectionArray) )
		{
		  CString Msg;
		  Msg.Format ( IDS_IMPORTSUCCESFUL, m_pImport->GetImportCount() );
		  AfxMessageBox ( Msg, MB_OK | MB_ICONINFORMATION );
		}
		else 
		{
		  CDlgImportStatus StatusDlg;
		  StatusDlg.SetMessages ( m_pImport->GetErrors() );
		  StatusDlg.DoModal();
		}

		// clean up
		if (pSelectionArray) delete pSelectionArray;

		// reset wait cursor
		AfxGetApp()->EndWaitCursor();
	  }

	if (m_pImportDlg) 
	{
//		m_pImportDlg->DestroyWindow();
	}
	
	COleControl::OnDestroy();
}

BOOL CECTImportXCtrl::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: Add your message handler code here and/or call default

	return COleControl::OnHelpInfo(pHelpInfo);
}
