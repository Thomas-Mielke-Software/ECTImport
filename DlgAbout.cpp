/*//////////////////////////////////////////////////////////////////////////////
// Name:        dlgabout.cpp
// Purpose:     Implementation of CDlgAbout
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "ECTImportX.h"
#include "ECTImportX.h"
#include "DlgAbout.h"
#include "Module.h"
#include "helpcontextmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDlgAbout::CDlgAbout(CWnd* pParent)
	: CDialog(CDlgAbout::IDD, pParent) {
	//{{AFX_DATA_INIT(CDlgAbout)
	//}}AFX_DATA_INIT
}


void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAbout)
	DDX_Control(pDX, IDC_EMAIL, m_EMail);
	DDX_Control(pDX, IDC_ABOUTICON, m_AboutIcon);
	DDX_Control(pDX, IDC_VERSION, m_Version);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAbout, CDialog)
	//{{AFX_MSG_MAP(CDlgAbout)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CDlgAbout::OnInitDialog()
{

	CDialog::OnInitDialog();
	
  // load and display logo icon
  HICON IconHandle = LoadIcon ( theApp.m_hInstance, MAKEINTRESOURCE ( IDI_ABOUTDLL ) );
  if ( IconHandle != NULL )
    m_AboutIcon.SetIcon ( IconHandle );

  // set mail address
  CString eMail = _T("");
  eMail.LoadString( IDS_EMAIL );
  m_EMail.SetWindowText( eMail );
  
  // get filename
  CString ModuleFilename;
  if ( GetModuleFileName ( theApp.m_hInstance, ModuleFilename.GetBufferSetLength ( MAX_PATH + 1 ), MAX_PATH ) == 0 )
    return TRUE;
  ModuleFilename.ReleaseBuffer();

  // determine version info size
  DWORD VersionInfoSize;
  DWORD NonsenseHandle;
  VersionInfoSize = GetFileVersionInfoSize ( ModuleFilename.GetBuffer ( 0 ), &NonsenseHandle );
  if ( VersionInfoSize == 0 )
    return TRUE;

	// allocate memory for and then get version info data block
  LPVOID VersionData;
  VersionData = malloc ( VersionInfoSize );
  if ( !GetFileVersionInfo ( ModuleFilename.GetBuffer ( 0 ), NonsenseHandle, VersionInfoSize, VersionData ) ) 
    return TRUE;
  
  // get fixed version info
  VS_FIXEDFILEINFO* FixedFileInfo = NULL;
  UINT FixedFileInfoSize;
  if ( !VerQueryValue ( VersionData, _T("\\"), (void**) &FixedFileInfo, &FixedFileInfoSize ) )
  {
    free ( VersionData );
    return TRUE;
  }
  
  CString Version; 
  Version.Format ( IDS_VERSION, HIWORD ( FixedFileInfo->dwProductVersionMS ), LOWORD ( FixedFileInfo->dwProductVersionMS ), HIWORD ( FixedFileInfo->dwProductVersionLS ), LOWORD ( FixedFileInfo->dwProductVersionLS ) );
  m_Version.SetWindowText ( Version );

  // free version info block
  free ( VersionData );

  return TRUE;
}


BOOL CDlgAbout::OnHelpInfo(HELPINFO* pHelpInfo)
{
	char exefilename[MAX_PATH];
	char *cp;
	if (::GetModuleFileName(theApp.m_hInstance, exefilename, MAX_PATH) && (cp = strrchr(exefilename, '\\')))
	{
		strcpy(cp, "\\ECTEImport.htm");
		::ShellExecute(NULL, "open", exefilename, NULL, NULL, SW_SHOWNORMAL);
	}
	return CDialog::OnHelpInfo ( pHelpInfo );
}
