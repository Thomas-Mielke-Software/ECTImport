//  DlgProgress.cpp : Implementierungsdatei
// CG: Diese Datei wurde von der Komponente "Dialogfeld Fortschritt" hinzugefügt

#include "stdafx.h"
#include "resource.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress Dialogfeld

CDlgProgress::CDlgProgress(UINT nCaptionID)
{
	m_nCaptionID = IDS_PROGRESS_CAPTION;
	if (nCaptionID != 0)
		m_nCaptionID = nCaptionID;

    m_bCancel = FALSE;
    m_nLower = 0;
    m_nUpper = 100;
    m_nStep = 1;
    //{{AFX_DATA_INIT(CDlgProgress)
    //}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;
}


CDlgProgress::~CDlgProgress()
{
  if ( m_hWnd != NULL )
    DestroyWindow();
}


BOOL CDlgProgress::DestroyWindow()
{
  ReEnableParent();
  return CDialog::DestroyWindow();
}


void CDlgProgress::ReEnableParent()
{
  if( m_bParentDisabled && ( m_pParentWnd != NULL ) )
    m_pParentWnd->EnableWindow ( TRUE );

  m_bParentDisabled = FALSE;
}


BOOL CDlgProgress::Create(CWnd *pParent)
{
  // Tatsächliches übergeordnetes Element des Dialogfelds bereitstellen
  m_pParentWnd = CWnd::GetSafeOwner ( pParent );

  // m_bParentDisabled dient dem Neuaufbau des übergeordneten Fensters,
  // wenn das Dialogfeld entfernt wurde. Deshalb sollte sie nicht auf TRUE 
  // gesetzt werden, es sei denn das übergeordnete Fenster war bereits aktiviert.

  if ( ( m_pParentWnd != NULL ) && m_pParentWnd->IsWindowEnabled() )
  {
    m_pParentWnd->EnableWindow ( FALSE );
    m_bParentDisabled = TRUE;
  }

  if ( !CDialog::Create ( CDlgProgress::IDD, pParent ) )
  {
    ReEnableParent();
    return FALSE;
  }

  return TRUE;
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDlgProgress)
  DDX_Control(pDX, CG_IDC_PROGDLG_PROGRESS, m_Progress);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
    //{{AFX_MSG_MAP(CDlgProgress)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


bool CDlgProgress::SetStatus ( UINT MessageID )
{
  CString Message;
  if ( !Message.LoadString ( MessageID ) )
    return FALSE;
  return SetStatus ( Message );
}


bool CDlgProgress::SetStatus(LPCTSTR lpszMessage)
{
  if ( !m_hWnd )
    return FALSE;
  CWnd *pWndStatus = GetDlgItem ( CG_IDC_PROGDLG_STATUS );

  if ( pWndStatus == NULL )
    return FALSE;

  pWndStatus->SetWindowText ( lpszMessage );
  return TRUE;
}


void CDlgProgress::OnCancel()
{
  m_bCancel = TRUE;
}


void CDlgProgress::SetRange(int nLower,int nUpper)
{
  m_nLower = nLower;
  m_nUpper = nUpper;
  m_Progress.SetRange ( nLower, nUpper );
}
  

int CDlgProgress::SetPos(int nPos)
{
  PumpMessages();
  int iResult = m_Progress.SetPos ( nPos );
  return iResult;
}


int CDlgProgress::SetStep(int nStep)
{
  m_nStep = nStep;
  return m_Progress.SetStep ( nStep );
}


int CDlgProgress::OffsetPos(int nPos)
{
  PumpMessages();
  int iResult = m_Progress.OffsetPos(nPos);
  return iResult;
}


int CDlgProgress::StepIt()
{
  PumpMessages();
  int iResult = m_Progress.StepIt();
  return iResult;
}


void CDlgProgress::PumpMessages()
{
  // Muss Create() aufrufen, bevor das Dialogfeld verwendet wird
  ASSERT(m_hWnd!=NULL);

  MSG msg;

  // Verarbeiten von Dialogfeld-Nachrichten
  while ( PeekMessage (& msg, NULL, 0, 0, PM_REMOVE ) )
  {
    if ( !IsDialogMessage ( &msg ) )
    {
      TranslateMessage ( &msg );
      DispatchMessage ( &msg );  
    }
  }
}


BOOL CDlgProgress::CheckCancelButton()
{
  // Anstehende Nachrichten verarbeiten
  PumpMessages();

  // Setzen Sie  m_bCancel auf FALSE zurück, damit 
  // CheckCancelButton FALSE zurückgibt, bis der Benutzer
  // erneut auf "Abbrechen" klickt. Dies ermöglicht den Aufruf von CheckCancelButton
  // aufgerufen werden, und gleichzeitig kann die Operation noch fortgesetzt werden.
  // Behält m_bCancel den Wert TRUE, dann gibt der nächste Aufruf von
  // CheckCancelButton immer TRUE zurück

  BOOL bResult = m_bCancel;
  m_bCancel = FALSE;

  return bResult;
}


BOOL CDlgProgress::OnInitDialog() 
{
  CDialog::OnInitDialog();
  m_Progress.SetRange ( m_nLower, m_nUpper );
  m_Progress.SetStep ( m_nStep );
  m_Progress.SetPos ( m_nLower );

	CString strCaption;
	VERIFY ( strCaption.LoadString ( m_nCaptionID ) );

  SetWindowText ( strCaption );

  return TRUE;  
}

/*
// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "ECTImportX.h"
#include "DlgProgress.h"


// CDlgProgress dialog

IMPLEMENT_DYNAMIC(CDlgProgress, CDialog)

CDlgProgress::CDlgProgress(CWnd* pParent **=NULL**)
	: CDialog(CDlgProgress::IDD, pParent)
{

}

CDlgProgress::~CDlgProgress()
{
}

void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
END_MESSAGE_MAP()


// CDlgProgress message handlers

*/