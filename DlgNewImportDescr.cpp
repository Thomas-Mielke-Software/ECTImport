/*//////////////////////////////////////////////////////////////////////////////
// Name:        dlgwewimportdescr.cpp
// Purpose:     Implementation of CDlgNewImportDescr
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "ECTImportX.h"
#include "easycashdoc.h"
#include "DlgNewImportDescr.h"
#include "ImportParams.h"
#include "module.h"
#include "helpcontextmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDlgNewImportDescr::CDlgNewImportDescr ( CWnd* pParent )
	: CDialog ( CDlgNewImportDescr::IDD, pParent ) {
	//{{AFX_DATA_INIT(CDlgNewImportDescr)
	m_Name = _T("");
	m_CopyExisting = FALSE;
	//}}AFX_DATA_INIT
  m_ImportDescrList = NULL;
  m_CurrentImportParams = NULL;
}


void CDlgNewImportDescr::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewImportDescr)
	DDX_Control(pDX, IDC_COPYEXISTING, m_CopyExistingCheckBox);
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDX_Check(pDX, IDC_COPYEXISTING, m_CopyExisting);
	//}}AFX_DATA_MAP

  // validation
  if ( pDX->m_bSaveAndValidate ) {
    // name must not be empty
    if ( m_Name.IsEmpty() ) {
      AfxMessageBox ( IDS_NAMEISEMPTY, MB_OK | MB_ICONSTOP, 0 );
      pDX->Fail();
    }
    // test if there is already an entry with same name
    if ( m_ImportDescrList != NULL && m_ImportDescrList->FindByName ( m_Name ) != -1 ) {
      AfxMessageBox ( IDS_NAMEALREADYEXISTS, MB_OK | MB_ICONSTOP, 0 );
      pDX->Fail();  
    }
  }
  else
  {
    if ( m_CopyExistingCheckBox.m_hWnd )
      m_CopyExistingCheckBox.EnableWindow ( m_CurrentImportParams != NULL );
  }
}


BEGIN_MESSAGE_MAP(CDlgNewImportDescr, CDialog)
	//{{AFX_MSG_MAP(CDlgNewImportDescr)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgNewImportDescr::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	char exefilename[MAX_PATH];
	char *cp;
	if (::GetModuleFileName(theApp.m_hInstance, exefilename, MAX_PATH) && (cp = strrchr(exefilename, '\\')))
	{
		strcpy(cp, "\\ECTEImport.htm");
		::ShellExecute(NULL, "open", exefilename, NULL, NULL, SW_SHOWNORMAL);
	}
	return CDialog::OnHelpInfo(pHelpInfo);
}


void CDlgNewImportDescr::BuildDefaultName() {
  
  if ( !m_ImportDescrList )
    return;

  int Counter = 1;
  CString DefaultName;
  DefaultName.Format ( IDS_DEFAULTNAME, Counter );
  
  BOOL Found;
  int i;

  while ( TRUE ) {
    Found = FALSE;
    for ( i = 0; i <= m_ImportDescrList->GetSize() - 1; i++ ) 
      if ( m_ImportDescrList->FindByName ( DefaultName ) != -1 ) {
        Found = TRUE;
        break;
      }

    if ( !Found )
      break;

    Counter++;
    DefaultName.Format ( IDS_DEFAULTNAME, Counter );
  }
  
  m_Name = DefaultName;
}

BOOL CDlgNewImportDescr::OnInitDialog() 
{
  BuildDefaultName();	

	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


/*
// DlgNewImportDescr.cpp : implementation file
//

#include "stdafx.h"
#include "ECTImportX.h"
#include "DlgNewImportDescr.h"


// CDlgNewImportDescr dialog

IMPLEMENT_DYNAMIC(CDlgNewImportDescr, CDialog)

CDlgNewImportDescr::CDlgNewImportDescr(CWnd* pParent **=NULL**)
	: CDialog(CDlgNewImportDescr::IDD, pParent)
{

}

CDlgNewImportDescr::~CDlgNewImportDescr()
{
}

void CDlgNewImportDescr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNewImportDescr, CDialog)
END_MESSAGE_MAP()


// CDlgNewImportDescr message handlers
*/