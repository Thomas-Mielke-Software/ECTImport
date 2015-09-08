/*//////////////////////////////////////////////////////////////////////////////
// Name:        dlgimportstatus.cpp
// Purpose:     Implementation of CDlgImportStatus
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "ECTImportX.h"
#include "easycashdoc.h"
#include "DlgImportStatus.h"
#include "module.h"
#include "helpcontextmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDlgImportStatus::CDlgImportStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImportStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImportStatus)
	//}}AFX_DATA_INIT
}


void CDlgImportStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  
  if ( !pDX->m_bSaveAndValidate && m_MessagesList.m_hWnd ) {
    // clear content
    m_MessagesList.DeleteAllItems();

    CString LineNoStr;
    int i, NewItemIndex;
    for ( i = 0; i <= m_Messages.GetSize() - 1; i++ )
    {
      if ( m_Messages.GetAt ( i )->GetLineNo() == 0 )
      {
        LineNoStr = _T("");
      }
      else
      {
#pragma warning(disable : 4996)
        _itot ( m_Messages.GetAt ( i )->GetLineNo(), LineNoStr.GetBuffer ( 20 ), 10 );
        LineNoStr.ReleaseBuffer();
      }
    
      NewItemIndex = m_MessagesList.InsertItem ( i, LineNoStr );
      m_MessagesList.SetItemText ( NewItemIndex, 1, m_Messages.GetAt ( i )->GetMsg() );

    }
  }

	//{{AFX_DATA_MAP(CDlgImportStatus)
	DDX_Control(pDX, IDC_ERRORICON, m_ErrorIcon);
	DDX_Control(pDX, IDC_MESSAGES, m_MessagesList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImportStatus, CDialog)
	//{{AFX_MSG_MAP(CDlgImportStatus)
	ON_WM_HELPINFO()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDlgImportStatus::SetMessages ( const CImportErrorList& Value )
{
  // copy messages
  int i;
  for ( i = 0; i <= Value.GetSize() - 1; i++ )
  {
    m_Messages.Add ( Value.GetAt ( i )->GetLineNo(), Value.GetAt ( i )->GetMsg() );
  }
};


BOOL CDlgImportStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  // set error icon's image
  m_ErrorIcon.SetIcon ( LoadIcon ( NULL, IDI_ERROR ) );
  
  // set listview's extended style and create list columns
  m_MessagesList.SetExtendedStyle ( m_MessagesList.GetExtendedStyle() | LVS_EX_FULLROWSELECT );

  CString ColumnTitle ( MAKEINTRESOURCE ( IDS_IMPORTSTATUS_ERRORLIST_COL1 ) );
  m_MessagesList.InsertColumn ( 0, ColumnTitle, LVCFMT_LEFT, -1, -1 );

  ColumnTitle.LoadString ( IDS_IMPORTSTATUS_ERRORLIST_COL2 );
  m_MessagesList.InsertColumn ( 1, ColumnTitle, LVCFMT_LEFT, -1, -1 );

  //
  UpdateData ( FALSE );

  //
  UpdateColumnWidths();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


BOOL CDlgImportStatus::OnHelpInfo(HELPINFO* pHelpInfo)  {
	char exefilename[MAX_PATH];
	char *cp;
	if (::GetModuleFileName(theApp.m_hInstance, exefilename, MAX_PATH) && (cp = strrchr(exefilename, '\\')))
	{
		strcpy(cp, "\\ECTEImport.htm");
		::ShellExecute(NULL, "open", exefilename, NULL, NULL, SW_SHOWNORMAL);
	}
	return CDialog::OnHelpInfo(pHelpInfo);
}

void CDlgImportStatus::UpdateColumnWidths()
{
  if ( !m_MessagesList.m_hWnd )
    return;

  // declare variables
  HD_ITEM HeaderItem;
  HeaderItem.mask = HDI_TEXT;
  int i, Col, ColWidth, MaxColWidth;
  CString ColText;
  
  for ( Col = 0; Col <= m_MessagesList.GetHeaderCtrl()->GetItemCount() - 1; Col++ ) {
    // get width of header
    HeaderItem.pszText = NULL;
    m_MessagesList.GetHeaderCtrl()->GetItem ( Col, &HeaderItem );
    ColText = HeaderItem.pszText;
    MaxColWidth = m_MessagesList.GetStringWidth ( ColText );
    
    // get max width of entries in column
    for ( i = 0; i <= m_MessagesList.GetItemCount() - 1; i++ ) {
      ColWidth = m_MessagesList.GetStringWidth ( m_MessagesList.GetItemText ( i, Col ) );
      if ( ColWidth > MaxColWidth )
        MaxColWidth = ColWidth;
    }

    // set new col width
    m_MessagesList.SetColumnWidth ( Col, MaxColWidth + 15 );
  }
  /*
  int ItemWidth, HeaderWidth;
  m_MessagesList.GetStringWidth ( ColumnTitle ) + 15
  
  m_MessagesList.SetColumnWidth ( 0, m_MessagesList.GetStringWidth ( ColumnTitle ) + 15 );
  */
}

/*
// DlgImportStatus.cpp : implementation file
//

#include "stdafx.h"
#include "ECTImportX.h"
#include "DlgImportStatus.h"


// CDlgImportStatus dialog

IMPLEMENT_DYNAMIC(CDlgImportStatus, CDialog)

CDlgImportStatus::CDlgImportStatus(CWnd* pParent **=NULL**)
	: CDialog(CDlgImportStatus::IDD, pParent)
{

}

CDlgImportStatus::~CDlgImportStatus()
{
}

void CDlgImportStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImportStatus, CDialog)
END_MESSAGE_MAP()


// CDlgImportStatus message handlers
*/