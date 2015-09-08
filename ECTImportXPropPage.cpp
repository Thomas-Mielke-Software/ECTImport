// ECTImportXPropPage.cpp : Implementation of the CECTImportXPropPage property page class.

#include "stdafx.h"
#include "ECTImportX.h"
#include "ECTImportXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CECTImportXPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CECTImportXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CECTImportXPropPage, "ECTIMPORTX.ECTImportXPropPage.1",
	0xe1d0d09e, 0xde62, 0x4859, 0x88, 0x4e, 0x43, 0x4, 0xbe, 0x98, 0xf7, 0x72)



// CECTImportXPropPage::CECTImportXPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CECTImportXPropPage

BOOL CECTImportXPropPage::CECTImportXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ECTIMPORTX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CECTImportXPropPage::CECTImportXPropPage - Constructor

CECTImportXPropPage::CECTImportXPropPage() :
	COlePropertyPage(IDD, IDS_ECTIMPORTX_PPG_CAPTION)
{
}



// CECTImportXPropPage::DoDataExchange - Moves data between page and properties

void CECTImportXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CECTImportXPropPage message handlers
