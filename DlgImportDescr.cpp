/*//////////////////////////////////////////////////////////////////////////////
// Name:        dlgimportdescr.cpp
// Purpose:     Implementation of CDlgImportDescr, the main dialog
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "ECTImportX.h"
#include "easycashdoc.h"
#include "ResizableDialog.h"
#include "DlgImportDescr.h"
#include "ImportParams.h"
#include "DlgNewImportDescr.h"
#include "DlgAbout.h"
#include "module.h"
#include "helpcontextmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CDlgImportDescr, CImportUIBase)
	//{{AFX_MSG_MAP(CDlgImportDescr)
	ON_BN_CLICKED(IDC_SELFILENAME, OnSelFilename)
	ON_BN_CLICKED(IDC_SELGAWKSCRIPT, &CDlgImportDescr::OnBnClickedSelgawkscript)
	ON_BN_CLICKED(IDC_DELIMPORTDESCR, OnDelImportDescr)
	ON_BN_CLICKED(IDC_NEWIMPORTDESCR, OnNewImportDescr)
	ON_CBN_SELCHANGE(IDC_IMPORTDESCR, OnSelChangeImportDescr)
	ON_BN_CLICKED(IDC_VIEWFILECONTENT, OnViewFileContent)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDlgImportDescr::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CDlgImportDescr::OnBnClickedOk2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILECONTENT, &CDlgImportDescr::OnLvnItemchangedFilecontent)
	ON_CBN_SELCHANGE(IDC_FIELD_EA, &CDlgImportDescr::OnCbnSelchangeFieldEa)
	ON_BN_CLICKED(IDC_INFO, &CDlgImportDescr::OnBnClickedInfo)
	ON_NOTIFY(HDN_ENDTRACK, 0, &CDlgImportDescr::OnHdnEndtrackFilecontent)
	ON_CBN_SELCHANGE(IDC_FIELD_BESTANDSKONTO, &CDlgImportDescr::OnCbnSelchangeFieldBestandskonto)
	ON_BN_CLICKED(IDC_EDITGAWKSCRIPT, &CDlgImportDescr::OnBnClickedEditgawkscript)
	ON_BN_CLICKED(IDC_EXEGAWKSCRIPT, &CDlgImportDescr::OnBnClickedExegawkscript)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_UTF8CONVERT, &CDlgImportDescr::OnBnClickedUtf8convert)
END_MESSAGE_MAP()


CDlgImportDescr::CDlgImportDescr ( CWnd* pParent )
	: CImportUIBase ( CDlgImportDescr::IDD, pParent )
{
  //{{AFX_DATA_INIT(CDlgImportDescr)
	//}}AFX_DATA_INIT
	m_Result = IDCANCEL;
}


CDlgImportDescr::~CDlgImportDescr()
{
}


void CDlgImportDescr::DoDataExchange ( CDataExchange* pDX )
{
	CImportUIBase::DoDataExchange ( pDX );

	// transfer values from current Params Object to control wrapper objects
	if ( !pDX->m_bSaveAndValidate )
	{
		UpdateFieldComboBoxes ( FALSE );
		if ( m_CurrentImportParams )
		{
			if ( m_ImportDescr.m_hWnd )
			{
				if ( m_ImportDescr.SelectString ( -1, m_CurrentImportParams->GetName() ) == CB_ERR ) 
					m_ImportDescr.SetCurSel ( m_ImportDescr.AddString ( m_CurrentImportParams->GetName() ) );
			}

			if ( m_NumHeaderLines.m_hWnd )
				SetDlgItemInt ( m_NumHeaderLines.GetDlgCtrlID(), m_CurrentImportParams->GetHeaderCount(), TRUE );

			if ( m_SeparatorChar.m_hWnd )
				m_SeparatorChar.SetWindowText ( m_CurrentImportParams->GetSeparatorChar() );

			if ( m_Filename.m_hWnd )
				m_Filename.SetWindowText ( m_CurrentImportParams->GetFilename() );

			if ( m_GawkScript.m_hWnd )
				m_GawkScript.SetWindowText ( m_CurrentImportParams->GetGawkScript() );

			if ( m_FieldEAFormatE.m_hWnd )
				m_FieldEAFormatE.SetWindowText ( m_CurrentImportParams->GetFieldEA()->GetFormatIncome() );

			if ( m_FieldEAFormatA.m_hWnd )
				m_FieldEAFormatA.SetWindowText ( m_CurrentImportParams->GetFieldEA()->GetFormatExpense() );

			if ( m_FieldDatumFormat.m_hWnd )
				m_FieldDatumFormat.SetCurSel ( m_CurrentImportParams->GetFieldDatum()->GetFormat() );

			if ( m_FieldDatumDelimiter.m_hWnd )
				m_FieldDatumDelimiter.SetWindowText ( m_CurrentImportParams->GetFieldDatum()->GetDelimiter() );

			if ( m_FieldBetragFormat.m_hWnd )
				m_FieldBetragFormat.SetCurSel ( m_CurrentImportParams->GetFieldBetrag()->GetFormat() );

			if ( m_FieldBNFormatBrutto.m_hWnd )
				m_FieldBNFormatBrutto.SetWindowText( m_CurrentImportParams->GetFieldBruttoNetto()->GetFormatBrutto() );

			if ( m_FieldBNFormatNetto.m_hWnd )
				m_FieldBNFormatNetto.SetWindowText( m_CurrentImportParams->GetFieldBruttoNetto()->GetFormatNetto() );

			if ( m_FieldSteuersatzFormat.m_hWnd )
				m_FieldSteuersatzFormat.SetCurSel ( m_CurrentImportParams->GetFieldSteuersatz()->GetFormat() );

			if ( m_FieldAfADegSatzFormat.m_hWnd )
				m_FieldAfADegSatzFormat.SetCurSel ( m_CurrentImportParams->GetFieldAfADegSatz()->GetFormat() );

			if ( m_FieldAfARestwertFormat.m_hWnd )
				m_FieldAfARestwertFormat.SetCurSel ( m_CurrentImportParams->GetFieldAfARestwert()->GetFormat() );
		}
		else
		{
			if ( m_ImportDescr.m_hWnd )
				m_ImportDescr.SetCurSel ( -1 );

			if ( m_NumHeaderLines.m_hWnd )
				SetDlgItemInt ( m_NumHeaderLines.GetDlgCtrlID(), 0, TRUE );

			if ( m_SeparatorChar.m_hWnd )
				m_SeparatorChar.SetWindowText ( _T("") );

			if ( m_Filename.m_hWnd )
				m_Filename.SetWindowText ( _T("") );

			if ( m_GawkScript.m_hWnd )
				m_GawkScript.SetWindowText ( _T("") );

			if ( m_FieldEAFormatE.m_hWnd )
				m_FieldEAFormatE.SetWindowText ( _T("") );

			if ( m_FieldEAFormatA.m_hWnd )
				m_FieldEAFormatA.SetWindowText ( _T("") );

			if ( m_FieldDatumFormat.m_hWnd )
				m_FieldDatumFormat.SetCurSel ( -1 );

			if ( m_FieldDatumDelimiter.m_hWnd )
				m_FieldDatumDelimiter.SetWindowText ( _T("") );

			if ( m_FieldBetragFormat.m_hWnd )
				m_FieldBetragFormat.SetCurSel ( -1 );

			if ( m_FieldBNFormatBrutto.m_hWnd )
				m_FieldBNFormatBrutto.SetWindowText( _T("") );

			if ( m_FieldBNFormatNetto.m_hWnd )
				m_FieldBNFormatNetto.SetWindowText( _T("") );

			if ( m_FieldSteuersatzFormat.m_hWnd )
				m_FieldSteuersatzFormat.SetCurSel ( -1 );

			if ( m_FieldAfADegSatzFormat.m_hWnd )
				m_FieldAfADegSatzFormat.SetCurSel ( -1 );

			if ( m_FieldAfARestwertFormat.m_hWnd )
				m_FieldAfARestwertFormat.SetCurSel ( -1 );
		}
	}
	else
		if ( m_CurrentImportParams )
		{		
			// Spaltenbreiten in params abspeichern
			int i; 
			for (i = 0; i < m_CurrentImportParams->GetColumnNames()->GetSize(); i++)
				if (m_CurrentImportParams->GetColumnWidth(i) == -1)
					m_CurrentImportParams->SetColumnWidth(i, m_FileContent.GetColumnWidth(i));
		}

		//{{AFX_DATA_MAP(CDlgImportDescr)
		DDX_Control(pDX, IDC_SEPARATORCHAR, m_SeparatorChar);
		DDX_Control(pDX, IDC_FIELD_AFARESTWERT_FORMAT, m_FieldAfARestwertFormat);
		DDX_Control(pDX, IDC_FIELD_AFADEGSATZ_FORMAT, m_FieldAfADegSatzFormat);
		DDX_Control(pDX, IDC_FIELD_AFARESTWERT, m_FieldAfARestwert);
		DDX_Control(pDX, IDC_FIELD_AFADEGSATZ, m_FieldAfADegSatz);
		DDX_Control(pDX, IDC_FIELD_BELEGNUMMER, m_FieldBelegnummer);
		DDX_Control(pDX, IDC_FIELD_BN_FORMAT_NETTO, m_FieldBNFormatNetto);
		DDX_Control(pDX, IDC_FIELD_BETRAG_FORMAT, m_FieldBetragFormat);
		DDX_Control(pDX, IDC_FIELD_BN_FORMAT_BRUTTO, m_FieldBNFormatBrutto);
		DDX_Control(pDX, IDC_FIELD_BRUTTONETTO, m_FieldBruttoNetto);
		DDX_Control(pDX, IDC_FIELD_BETRAG, m_FieldBetrag);
		DDX_Control(pDX, IDC_FIELD_AFADAUER, m_FieldAfADauer);
		DDX_Control(pDX, IDC_FIELD_AFAAKTJAHR, m_FieldAfAAktJahr);
		DDX_Control(pDX, IDC_FIELD_STEUERSATZ_FORMAT, m_FieldSteuersatzFormat);
		DDX_Control(pDX, IDC_FIELD_DATUM_FORMAT, m_FieldDatumFormat);
		DDX_Control(pDX, IDC_FIELD_DATUM_DELIMITER, m_FieldDatumDelimiter);
		DDX_Control(pDX, IDC_FIELD_EA_FORMAT_E, m_FieldEAFormatE);
		DDX_Control(pDX, IDC_FIELD_EA_FORMAT_A, m_FieldEAFormatA);
		DDX_Control(pDX, IDC_NUMHEADERLINES_SPIN, m_NumHeaderLinesSpin);
		DDX_Control(pDX, IDC_FIELD_STEUERSATZ, m_FieldSteuersatz);
		DDX_Control(pDX, IDC_FIELD_KONTO, m_FieldKonto);
		DDX_Control(pDX, IDC_FIELD_DATUM, m_FieldDatum);
		DDX_Control(pDX, IDC_FIELD_EA, m_FieldEA);
		DDX_Control(pDX, IDC_FIELD_BESCHREIBUNG, m_FieldBeschreibung);
		DDX_Control(pDX, IDC_VIEWFILECONTENT, m_ViewFileContent);
		DDX_Control(pDX, IDC_IMPORTDESCR, m_ImportDescr);
		DDX_Control(pDX, IDC_SELFILENAME, m_SelFilename);
		DDX_Control(pDX, IDC_NUMHEADERLINES, m_NumHeaderLines);
		DDX_Control(pDX, IDC_NEWIMPORTDESCR, m_NewImportDescr);
		DDX_Control(pDX, IDC_FILENAME, m_Filename);
		DDX_Control(pDX, IDC_GAWKSCRIPT, m_GawkScript);
		DDX_Control(pDX, IDC_FILECONTENT, m_FileContent);
		DDX_Control(pDX, IDC_DELIMPORTDESCR, m_DelImportDescr);
		//}}AFX_DATA_MAP

		if ( pDX->m_bSaveAndValidate )
		{
			if ( m_CurrentImportParams )
			{
				// get filename from edit control
				CString Filename;
				m_Filename.GetWindowText ( Filename );
				m_CurrentImportParams->SetFilename ( Filename );

				CString GawkScript;
				m_GawkScript.GetWindowText ( GawkScript );
				m_CurrentImportParams->SetGawkScript ( GawkScript );
				
				// get separator char from edit control
				CString SeparatorChar;
				m_SeparatorChar.GetWindowText ( SeparatorChar );
				m_CurrentImportParams->SetSeparatorChar ( SeparatorChar );

				// get number of header/footer lines from edit controls
				BOOL Trans = 0;
				m_CurrentImportParams->SetHeaderCount ( GetDlgItemInt ( m_NumHeaderLines.GetDlgCtrlID(), &Trans, TRUE ) );

				//
				CString S;
				m_FieldEAFormatE.GetWindowText ( S );
				m_CurrentImportParams->GetFieldEA()->SetFormatIncome ( S );

				m_FieldEAFormatA.GetWindowText ( S );
				m_CurrentImportParams->GetFieldEA()->SetFormatExpense ( S );

				m_CurrentImportParams->GetFieldDatum()->SetFormat ( (CDateFieldConversion::DateFormat) m_FieldDatumFormat.GetCurSel () );

				m_FieldDatumDelimiter.GetWindowText ( S );
				m_CurrentImportParams->GetFieldDatum()->SetDelimiter ( S );

				m_CurrentImportParams->GetFieldBetrag()->SetFormat ( (CCurrencyFieldConversion::CurrencyFormat) m_FieldBetragFormat.GetCurSel() );

				m_FieldBNFormatBrutto.GetWindowText ( S );
				m_CurrentImportParams->GetFieldBruttoNetto()->SetFormatBrutto( S );

				m_FieldBNFormatNetto.GetWindowText ( S );
				m_CurrentImportParams->GetFieldBruttoNetto()->SetFormatNetto( S );

				m_CurrentImportParams->GetFieldSteuersatz()->SetFormat ( (CCurrencyFieldConversion::CurrencyFormat) m_FieldSteuersatzFormat.GetCurSel() );

				m_CurrentImportParams->GetFieldAfADegSatz()->SetFormat ( (CCurrencyFieldConversion::CurrencyFormat) m_FieldAfADegSatzFormat.GetCurSel() );

				m_CurrentImportParams->GetFieldAfARestwert()->SetFormat ( (CCurrencyFieldConversion::CurrencyFormat) m_FieldAfARestwertFormat.GetCurSel() );
			}
			UpdateFieldComboBoxes ( TRUE );
		}
		DDX_Control(pDX, IDC_DOKUMENTCTRL1, m_DokumentCtrl);
		DDX_Control(pDX, IDC_BUCHUNGCTRL1, m_BuchungCtrl);
		DDX_Control(pDX, IDC_EINSTELLUNGCTRL1, m_EinstellungCtrl);
		DDX_Control(pDX, IDC_FIELD_AFAGENAUIGKEIT, m_FieldAfAGenauigkeit);
		DDX_Control(pDX, IDC_FIELD_BETRIEB, m_FieldBetrieb);
		DDX_Control(pDX, IDC_FIELD_BESTANDSKONTO, m_FieldBestandskonto);
		DDX_Control(pDX, IDC_GAWKSCRIPT, m_GawkScript);
}



BOOL CDlgImportDescr::OnInitDialog()
{
	// call inherited
  CImportUIBase::OnInitDialog();
	
  // init controls
  m_SeparatorChar.SetLimitText ( 10 );
  m_FileContent.SetExtendedStyle ( m_FileContent.GetExtendedStyle() | LVS_EX_FULLROWSELECT );
  m_NumHeaderLines.SetLimitText ( 4 );
  m_NumHeaderLinesSpin.SetRange32 ( 0, 1000 );

  // assign help context id
  SetWindowContextHelpId ( IDH_DLGIMPORT );

	// ID_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT ( (ID_ABOUTBOX & 0xFFF0) == ID_ABOUTBOX );
	ASSERT ( ID_ABOUTBOX < 0xF000 );

  // modify system menu - add about menu entry
  CMenu* SysMenu = GetSystemMenu ( FALSE );
  CString MenuItemName = CString ( MAKEINTRESOURCE ( IDS_APP_ABOUT ) );
  if ( SysMenu != NULL && !MenuItemName.IsEmpty() ) {
    SysMenu->AppendMenu ( MF_SEPARATOR, 0, (LPCTSTR) NULL );
    SysMenu->AppendMenu ( MF_STRING, ID_ABOUTBOX, MenuItemName );
  }

  // insert default item if list is empty
  if ( m_ImportParamsList->IsEmpty() && m_CurrentImportParams )
    m_ImportParamsList->Add ( m_CurrentImportParams );

  // fill combo box with the names of the import descriptions
  for ( int i = 0; i <= m_ImportParamsList->GetSize() - 1; i++ )
    m_ImportDescr.AddString ( ((CImportParams*) m_ImportParamsList->GetAt ( i ))->GetName() );

  // select the last selected item in the combobox
  if ( m_ImportParamsList->GetDefaultImportParams() != NULL )
  {
    int Index = m_ImportDescr.FindStringExact( 0, m_ImportParamsList->GetDefaultImportParams()->GetName() );
    if ( Index == CB_ERR )
      m_ImportDescr.SetCurSel( 0 );
    else
      m_ImportDescr.SetCurSel( Index );
  }
  else   // ... if no setting exists, select the first item if any
    if ( m_ImportDescr.GetCount() > 0 )
      m_ImportDescr.SetCurSel( 0 );
    else
      m_ImportDescr.SetCurSel( -1 );
  OnSelChangeImportDescr();

  // update 
  UpdateData ( FALSE );
  UpdateControlState();
  UpdateControlState();

  // zum Schluss die Anker setzen für das Resizable-Verhalten
  AddAnchor(IDC_IMPORTDESCR, TOP_LEFT, TOP_RIGHT);	// oberer Teil
  AddAnchor(IDC_NEWIMPORTDESCR, TOP_RIGHT);
  AddAnchor(IDC_DELIMPORTDESCR, TOP_RIGHT);
  AddAnchor(IDC_GROUPBOX1, TOP_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC1, TOP_LEFT);
  AddAnchor(IDC_FILENAME, TOP_LEFT, TOP_RIGHT);
  AddAnchor(IDC_SELFILENAME, TOP_RIGHT);
  AddAnchor(IDC_STATIC31, TOP_LEFT);
  AddAnchor(IDC_GAWKSCRIPT, TOP_LEFT, TOP_CENTER);
  AddAnchor(IDC_STATIC32, TOP_CENTER, TOP_RIGHT);
  AddAnchor(IDC_SELGAWKSCRIPT, TOP_RIGHT);
  AddAnchor(IDC_EDITGAWKSCRIPT, TOP_RIGHT);
  AddAnchor(IDC_EXEGAWKSCRIPT, TOP_RIGHT);
  AddAnchor(IDC_STATIC2, TOP_LEFT);
  AddAnchor(IDC_SEPARATORCHAR, TOP_LEFT);
  AddAnchor(IDC_STATIC3, TOP_LEFT);
  AddAnchor(IDC_NUMHEADERLINES, TOP_LEFT);
  AddAnchor(IDC_NUMHEADERLINES_SPIN, TOP_LEFT);
  AddAnchor(IDC_UTF8CONVERT, TOP_RIGHT);
  AddAnchor(IDC_VIEWFILECONTENT, TOP_RIGHT);
  AddAnchor(IDC_STATIC4, TOP_LEFT);
  AddAnchor(IDC_FILECONTENT, TOP_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_GROUPBOX2, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC30, BOTTOM_CENTER);

  AddAnchor(IDC_STATIC5, BOTTOM_LEFT);	// 1. Spalte 
  AddAnchor(IDC_STATIC6, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC7, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC19, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC8, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC22, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC9, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC10, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC17, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC18, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC24, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC23, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC27, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC28, BOTTOM_LEFT);
  AddAnchor(IDC_STATIC29, BOTTOM_LEFT);
  AddAnchor(IDC_FIELD_EA, BOTTOM_LEFT, BOTTOM_RIGHT);	// 2. Spalte Felder
  AddAnchor(IDC_FIELD_DATUM, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_BETRAG, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_BRUTTONETTO, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_BESCHREIBUNG, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_BELEGNUMMER, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_KONTO, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_STEUERSATZ, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_AFAAKTJAHR, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_AFADAUER, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_AFARESTWERT, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_AFADEGSATZ, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_AFAGENAUIGKEIT, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_BETRIEB, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_BESTANDSKONTO, BOTTOM_LEFT, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC11, BOTTOM_RIGHT);			// 3. Spalte
  AddAnchor(IDC_STATIC13, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC15, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC20, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC16, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC25, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC26, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_EA_FORMAT_E, BOTTOM_RIGHT);	// 4. Spalte und dahinter
  AddAnchor(IDC_STATIC12, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_EA_FORMAT_A, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_DATUM_FORMAT, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC14, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_DATUM_DELIMITER, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_BETRAG_FORMAT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_BN_FORMAT_BRUTTO, BOTTOM_RIGHT);
  AddAnchor(IDC_STATIC21, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_BN_FORMAT_NETTO, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_STEUERSATZ_FORMAT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_AFARESTWERT_FORMAT, BOTTOM_RIGHT);
  AddAnchor(IDC_FIELD_AFADEGSATZ_FORMAT, BOTTOM_RIGHT);

  AddAnchor(IDC_INFO, BOTTOM_LEFT);				// Info, Import & Zurück Knöpfe
  AddAnchor(IDC_STATIC35, BOTTOM_LEFT);
  AddAnchor(IDOK, BOTTOM_RIGHT);
  AddAnchor(IDOK2, BOTTOM_RIGHT);
  AddAnchor(IDCANCEL, BOTTOM_RIGHT);

  //SetTimer(1, 1, NULL);
  return TRUE;
}


void CDlgImportDescr::OnSysCommand ( UINT nID, LPARAM lParam )
{
  if ( nID == ID_ABOUTBOX ) 
  {
		CDlgAbout Dlg;
		Dlg.DoModal();
	}
	else
    CImportUIBase::OnSysCommand ( nID, lParam );
}


LRESULT CDlgImportDescr::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
  if ( message == WM_DROPFILES )
  {
    // get the name of the dropped file (we only care about the first file)
    CString Filename;
    DragQueryFile ( (HDROP) wParam, 0, Filename.GetBuffer ( MAX_PATH ), MAX_PATH );
    Filename.ReleaseBuffer();

    // fill the edit control with the filename
	if (Filename.Right(4) != ".awk" && Filename.Right(4) != ".AWK")
		m_Filename.SetWindowText ( Filename );
	else
		m_GawkScript.SetWindowText ( Filename );
    
    // file dropping finished
    DragFinish ( (HDROP) wParam );
    
    // return 0 - indicating that we processed the message
    return 0;
  }

  // call inherited window procedure
  return CImportUIBase::WindowProc ( message, wParam, lParam );
}


void CDlgImportDescr::OnSelFilename()
{
  // init file dialog
  CFileDialog OpenDialog ( TRUE,                                                // OpenDialog
                           "csv",  //CString ( MAKEINTRESOURCE ( IDS_OPENDEFEXT ) ),  // DefExt
                           _T(""),                                              // Filename
                           OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_SHOWHELP | OFN_EXPLORER, // Flags 
                           CString ( MAKEINTRESOURCE ( IDS_OPENFILEFILTER ) ),  // Filter
                           NULL );                                              // ParentWnd 
  
  // display dialog 
  if ( OpenDialog.DoModal() == IDOK )
  {
    m_Filename.SetWindowText ( OpenDialog.GetPathName() );
    // save current data
    UpdateData ( TRUE );
    // clear file content list view
    ClearFileContent ();
  }
}


void CDlgImportDescr::OnBnClickedSelgawkscript()
{
  // init file dialog
  CFileDialog OpenDialog ( TRUE,                                                // OpenDialog
                           "awk", //CString ( MAKEINTRESOURCE ( IDS_OPENGAWKDEFEXT ) ),  // DefExt
                           _T(""),                                              // Filename
                           OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_SHOWHELP | OFN_EXPLORER, // Flags 
                           CString ( MAKEINTRESOURCE ( IDS_OPENGAWKFILTER ) ),  // Filter
                           NULL );                                              // ParentWnd 
  
  // display dialog 
  if ( OpenDialog.DoModal() == IDOK )
  {
    m_GawkScript.SetWindowText ( OpenDialog.GetPathName() );
    // save current data
    UpdateData ( TRUE );
    // clear file content list view
    ClearFileContent ();
  }
}


void CDlgImportDescr::UpdateEAFieldComboBox ( BOOL SaveAndValidate )
{
  // check for invalid parameter
  if ( !m_FieldEA.m_hWnd )
    return;
  
  if ( SaveAndValidate )
  {
    if ( m_CurrentImportParams )
    {
      m_CurrentImportParams->GetFieldEA()->SetConversionRule ( CEAFieldConversion::cUseFormat );
  
      int Index = m_FieldEA.GetCurSel();
    
      if ( Index == CB_ERR )
      {
        BOOL Trans;
        Index = GetDlgItemInt ( m_FieldEA.GetDlgCtrlID(), &Trans, TRUE ) - 1;
        if ( Trans == 0 )
          Index = -1;
        m_CurrentImportParams->GetFieldEA()->SetSourceFieldID ( Index );
      } 
      else
      {
        if ( Index == 0 ) // Income Only is selected
        {
          m_CurrentImportParams->GetFieldEA()->SetConversionRule ( CEAFieldConversion::cIncomeOnly );
        }
        else 
          if ( Index == 1 ) // Expense Only is selected
          {
            m_CurrentImportParams->GetFieldEA()->SetConversionRule ( CEAFieldConversion::cExpenseOnly );
          }
          else 
            if ( Index == 2 ) // Use Sign is selected
            {
              m_CurrentImportParams->GetFieldEA()->SetConversionRule ( CEAFieldConversion::cUseSign );
            }
            else
              m_CurrentImportParams->GetFieldEA()->SetSourceFieldID ( Index - 3 );
      }
    }
  }
  else 
  {
    // clear existing entries
    m_FieldEA.ResetContent();

    if ( m_CurrentImportParams )
    {
      // add special entries for conversion rules
      CString s;
      s.LoadString ( IDS_CONVERSIONRULE_INCOME_ONLY );  // cIncomeOnly
      m_FieldEA.AddString ( s );
      s.LoadString ( IDS_CONVERSIONRULE_EXPENSE_ONLY ); // cExpenseOnly
      m_FieldEA.AddString ( s );
      s.LoadString ( IDS_CONVERSIONRULE_USE_SIGN );     // cUseSign
      m_FieldEA.AddString ( s );

      // fill with values from column names array
      int i;
      for ( i = 0; i <= m_CurrentImportParams->GetColumnNames()->GetSize() - 1; i++ )
        m_FieldEA.AddString ( m_CurrentImportParams->GetColumnNames()->GetAt ( i ) );
      
      // select/fill in current value
      switch ( m_CurrentImportParams->GetFieldEA()->GetConversionRule() )
      {
        case CEAFieldConversion::cIncomeOnly:
          m_FieldEA.SetCurSel ( 0 );
          break;
        
        case CEAFieldConversion::cExpenseOnly:
          m_FieldEA.SetCurSel ( 1 );
          break;
        
        case CEAFieldConversion::cUseSign:
          m_FieldEA.SetCurSel ( 2 );
          break;          

        default: // cUseFormat
          if ( m_CurrentImportParams->GetFieldEA()->GetSourceFieldID() >= 0 && m_CurrentImportParams->GetFieldEA()->GetSourceFieldID() <= m_FieldEA.GetCount() )
            m_FieldEA.SetCurSel ( m_CurrentImportParams->GetFieldEA()->GetSourceFieldID() + 3 );
          else
            if ( m_CurrentImportParams->GetFieldEA()->GetSourceFieldID() >= 0 )
              SetDlgItemInt ( m_FieldEA.GetDlgCtrlID(), m_CurrentImportParams->GetFieldEA()->GetSourceFieldID() + 1, TRUE );
      }
    }

  }
}


void CDlgImportDescr::UpdateBNFieldComboBox ( BOOL SaveAndValidate )
{
  // check for invalid parameter
  if ( !m_FieldBruttoNetto.m_hWnd )
    return;
  
  if ( SaveAndValidate )
  {
    if ( m_CurrentImportParams )
    {
      m_CurrentImportParams->GetFieldBruttoNetto()->SetConversionRule ( CBruttoNettoFieldConversion::cUseFormat );
  
      int Index = m_FieldBruttoNetto.GetCurSel();
    
      if ( Index == CB_ERR )
      {
        BOOL Trans;
        Index = GetDlgItemInt ( m_FieldBruttoNetto.GetDlgCtrlID(), &Trans, TRUE ) - 1;
        if ( Trans == 0 )
          Index = -1;
        m_CurrentImportParams->GetFieldBruttoNetto()->SetSourceFieldID ( Index );
      } 
      else
      {
        if ( Index == 0 ) // Income Only is selected
        {
          m_CurrentImportParams->GetFieldBruttoNetto()->SetConversionRule ( CBruttoNettoFieldConversion::cBruttoOnly );
        }
        else 
          if ( Index == 1 ) // Expense Only is selected
          {
            m_CurrentImportParams->GetFieldBruttoNetto()->SetConversionRule ( CBruttoNettoFieldConversion::cNettoOnly );
          }
          else 
            m_CurrentImportParams->GetFieldBruttoNetto()->SetSourceFieldID ( Index - 2 );
      }
    }
  }
  else 
  {
    // clear existing entries
    m_FieldBruttoNetto.ResetContent();

    if ( m_CurrentImportParams )
    {
      // add special entries for conversion rules
      CString s;
      s.LoadString ( IDS_CONVERSIONRULE_BRUTTO_ONLY );  // cIncomeOnly
      m_FieldBruttoNetto.AddString ( s );
      s.LoadString ( IDS_CONVERSIONRULE_NETTO_ONLY ); // cExpenseOnly
      m_FieldBruttoNetto.AddString ( s );

      // fill with values from column names array
      int i;
      for ( i = 0; i <= m_CurrentImportParams->GetColumnNames()->GetSize() - 1; i++ )
        m_FieldBruttoNetto.AddString ( m_CurrentImportParams->GetColumnNames()->GetAt ( i ) );
      
      // select/fill in current value
      switch ( m_CurrentImportParams->GetFieldBruttoNetto()->GetConversionRule() )
      {
        case CBruttoNettoFieldConversion::cBruttoOnly:
          m_FieldBruttoNetto.SetCurSel ( 0 );
          break;
        
        case CBruttoNettoFieldConversion::cNettoOnly:
          m_FieldBruttoNetto.SetCurSel ( 1 );
          break;
        
        default: // cUseFormat
          if ( m_CurrentImportParams->GetFieldBruttoNetto()->GetSourceFieldID() >= 0 && m_CurrentImportParams->GetFieldBruttoNetto()->GetSourceFieldID() <= m_FieldBruttoNetto.GetCount() )
            m_FieldBruttoNetto.SetCurSel ( m_CurrentImportParams->GetFieldBruttoNetto()->GetSourceFieldID() + 2 );
          else
            if ( m_CurrentImportParams->GetFieldBruttoNetto()->GetSourceFieldID() >= 0 )
              SetDlgItemInt ( m_FieldBruttoNetto.GetDlgCtrlID(), m_CurrentImportParams->GetFieldBruttoNetto()->GetSourceFieldID() + 1, TRUE );
      }
    }

  }
}


void CDlgImportDescr::UpdateFieldComboBoxes ( BOOL SaveAndValidate )
{
  // fill or retrieve values from combo boxes
  UpdateEAFieldComboBox ( SaveAndValidate );
  UpdateBNFieldComboBox ( SaveAndValidate );
  if ( m_CurrentImportParams )
  {
    /*
    UpdateFieldComboBox ( &m_FieldEA, m_CurrentImportParams->GetFieldEA(), SaveAndValidate );
    */
    UpdateFieldComboBox ( &m_FieldDatum, m_CurrentImportParams->GetFieldDatum(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBetrag, m_CurrentImportParams->GetFieldBetrag(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBeschreibung, m_CurrentImportParams->GetFieldBeschreibung(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBelegnummer, m_CurrentImportParams->GetFieldBelegnummer(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldKonto, m_CurrentImportParams->GetFieldKonto(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldSteuersatz, m_CurrentImportParams->GetFieldSteuersatz(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfAAktJahr, m_CurrentImportParams->GetFieldAfAAktJahr(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfADauer, m_CurrentImportParams->GetFieldAfADauer(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfARestwert, m_CurrentImportParams->GetFieldAfARestwert(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfADegSatz, m_CurrentImportParams->GetFieldAfADegSatz(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfAGenauigkeit, m_CurrentImportParams->GetFieldAfAGenauigkeit(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBetrieb, m_CurrentImportParams->GetFieldBetrieb(), SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBestandskonto, m_CurrentImportParams->GetFieldBestandskonto(), SaveAndValidate );
  }
  else
  {
    /*
    UpdateFieldComboBox ( &m_FieldEA, NULL, SaveAndValidate );
    */
    UpdateFieldComboBox ( &m_FieldDatum, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBetrag, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBeschreibung, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBelegnummer, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldKonto, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldSteuersatz, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfAAktJahr, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfADauer, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfARestwert, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfADegSatz, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldAfAGenauigkeit, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBetrieb, NULL, SaveAndValidate );
    UpdateFieldComboBox ( &m_FieldBestandskonto, NULL, SaveAndValidate );
  }
}


void CDlgImportDescr::UpdateFileContentColumnWidths(void)
{
    int col;
    RECT r;
    m_FileContent.GetWindowRect(&r);
    int nFileContentBreite = r.right - r.left;
	int nPlatzbedarf = 20;	// für vert. Scrollbar reservieren
    for ( col = 0; col <= m_CurrentImportParams->GetColumnNames()->GetSize() - 1; col++ )
	{
		int nFesteSpaltenbreite = m_CurrentImportParams->GetColumnWidth(col);	// hat mal ein Benutzer die Spaltenbreite manuell verändert? Dann diesen Wert bevorzugen und fixieren!
		nPlatzbedarf += nFesteSpaltenbreite ? nFesteSpaltenbreite : m_FileContent.GetStringWidth ( m_CurrentImportParams->GetColumnNames()->GetAt ( col ));	// ansonsten übern Daumen peilen
	}
	int nSpielraumProSpalte = (nFileContentBreite - nPlatzbedarf) / m_CurrentImportParams->GetColumnNames()->GetSize();
    for ( col = 0; col <= m_CurrentImportParams->GetColumnNames()->GetSize() - 1; col++ )
	{
		int nFesteSpaltenbreite = m_CurrentImportParams->GetColumnWidth(col);	// hat mal ein Benutzer die Spaltenbreite manuell verändert? Dann diesen Wert bevorzugen und fixieren!
		m_FileContent.SetColumnWidth(col, nFesteSpaltenbreite > 0 ? nFesteSpaltenbreite : max(70, m_FileContent.GetStringWidth ( m_CurrentImportParams->GetColumnNames()->GetAt ( col ) ) + nSpielraumProSpalte));
	}
}


BOOL CDlgImportDescr::UpdateFileContent ()
{

  ClearFileContent ();
  
  // check if filename is available
  if ( !m_CurrentImportParams || m_CurrentImportParams->GetFilename().IsEmpty() )
    return TRUE;

  // declare variables
  CCSVFile File;
  CFileException Error;
  CString Line;

  // try to open file
  if ( !File.Open ( m_CurrentImportParams->GetFilename(), CFile::modeRead | CFile::shareDenyWrite, &Error ) )
  {
    LPTSTR MsgBuf = NULL;
    FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, Error.m_lOsError, 0, (LPTSTR) &MsgBuf, 0, NULL/* Error.m_strFileName.GetBuffer ( 0 ) */ );
    AfxMessageBox ( MsgBuf, MB_OK | MB_ICONSTOP, 0 );
    LocalFree ( MsgBuf );
    return FALSE;
  }

#if 0
  // determine how many columns are in the file
  // if we have headerlines, then use file content for column titles
  // otherwise we build them like Field 1, Field 2, Field 3, Field ...
  int i;
  Line.Empty(); 
  CStringArray TempColumnTitles, ColumnTitles;
  if ( m_CurrentImportParams->GetHeaderCount() > 0 )
  {
    for ( i = 1; i <= m_CurrentImportParams->GetHeaderCount(); i++ ) 
    {
      if ( !File.ReadString ( Line ) )
        break;
      File.ParseString ( Line, TempColumnTitles, m_CurrentImportParams->GetSeparatorChar() );
      if ( TempColumnTitles.GetSize() > ColumnTitles.GetSize() )
        ColumnTitles.Copy ( TempColumnTitles );
    }
  }
  else {
    if ( File.ReadString ( Line ) )
      File.ParseString ( Line, ColumnTitles, m_CurrentImportParams->GetSeparatorChar() );
    File.SeekToBegin();
    for ( i = 0; i <= ColumnTitles.GetSize() - 1; i++ )
      ColumnTitles.ElementAt ( i ).Format ( IDS_FIELDX, i + 1 );
  }
#endif
  
  // retrieve column titles
  GetColumnTitles ( &File );

  // create list-view columns
  if ( m_CurrentImportParams->GetColumnNames()->GetSize() == 0 )
    m_FileContent.InsertColumn ( 0, _T(""), LVCFMT_LEFT, 800, -1 );
  else
  {
	int col;
    for ( col = 0; col <= m_CurrentImportParams->GetColumnNames()->GetSize() - 1; col++ )
    {
      m_FileContent.InsertColumn ( col, m_CurrentImportParams->GetColumnNames()->GetAt ( col ), LVCFMT_LEFT, 70, -1 );
    }
    UpdateFileContentColumnWidths();
  }

  // fill list-view
  int l, c;
  CStringArray LinePart;

  for ( l = 0; /*l < 40*/ TRUE; l++ )
  {
    try
    {
      if ( !File.ReadString ( Line ) )
        break;
    }
    catch ( CFileException *e )
    {
	  delete e;
      break;
    }

    File.ParseString ( Line, LinePart, m_CurrentImportParams->GetSeparatorChar() );

    if ( !Line.IsEmpty() && LinePart.GetSize() == 0 )
      LinePart.Add ( Line );

    if ( LinePart.GetSize() == 0 )
      continue;

    m_FileContent.InsertItem ( l, LinePart[0] );
    for ( c = 1; c <= LinePart.GetSize() - 1; c++ )
      m_FileContent.SetItemText ( l, c, LinePart[c] );
  }

  // close file
  File.Close();


  /*
  int col = 0;
  for ( col = 0; col <= m_CurrentImportParams->GetColumnNames()->GetSize() - 1; col ++ )
  {
    int currentWidth = 0;
    int maxWidth = 0;
    LVCOLUMN colInfo;
    if ( m_FileContent.GetColumn ( col, &colInfo ) )
    {
      for ( int row = 0; row <= m_FileContent.GetItemCount() - 1; row++ )
      {
        currentWidth = m_FileContent.GetStringWidth( m_FileContent.GetItemText( row, col ) );
        if ( currentWidth > maxWidth )
          maxWidth = currentWidth;
      }
      colInfo.cx = maxWidth;
      m_FileContent.SetColumn ( col, &colInfo );
    }
  }
  */

  return TRUE;
}


void CDlgImportDescr::ClearFileContent ()
{
  // clear listview (including columns)
  m_FileContent.DeleteAllItems();
  while ( m_FileContent.DeleteColumn ( 0 ) );
}


void CDlgImportDescr::OnDelImportDescr()
{
  // save current data
  UpdateData ( TRUE );

  // confirm deletion
  CString Msg, Name;
  m_ImportDescr.GetWindowText ( Name );
  Msg.Format ( IDS_CONFIRMDELIMPORTDESCR, Name );
  if ( AfxMessageBox ( Msg, MB_YESNO | MB_ICONQUESTION, 0 ) != IDYES )
    return;
  
  // find currently selectec combobox-item in object array
  int Index = m_ImportParamsList->FindByName ( Name );
  if ( Index == -1 )
  {
    /* TODO: display error messsage "item not found"
    AfxMessageBox ( _T("") );
    */
    return;
  }

  // remove object from array
  m_ImportParamsList->RemoveAt ( Index );
  // remove entry from combobox
  m_ImportDescr.DeleteString ( m_ImportDescr.GetCurSel() );

  // clear selection and update dialog box
  m_CurrentImportParams = NULL;
  UpdateData ( FALSE );
  UpdateControlState();

  // update file content list view
  ClearFileContent ();

  // since delete button is disable now, give focus to ImportDescr ComboBox
  m_ImportDescr.SetFocus();
}


void CDlgImportDescr::OnNewImportDescr()
{
  // save current data
  UpdateData ( TRUE );

  // decl dialog class, init and show
  CDlgNewImportDescr Dlg;
  Dlg.SetImportDescrList ( m_ImportParamsList );
  Dlg.SetCurrentImportParams ( m_CurrentImportParams );
  if ( Dlg.DoModal() != IDOK )
    return;

  // create new description 
  CImportParams* NewImportParams = new CImportParams();
  if ( Dlg.GetCopyExisting() )
    *NewImportParams = *m_CurrentImportParams;
  
  m_ImportParamsList->Add ( NewImportParams );
  m_CurrentImportParams = NewImportParams;
  
  m_CurrentImportParams->SetName ( Dlg.m_Name );
  
  // update dialog controls and state
  UpdateData ( FALSE );
  UpdateControlState();

  // update file content list view
  ClearFileContent();
}


void CDlgImportDescr::OnSelChangeImportDescr()
{
  // save current data
  UpdateData ( TRUE );

  // find array object for selected name in combobox
  CString Name;
  m_ImportDescr.GetWindowText ( Name );
  
  int Index = m_ImportParamsList->FindByName ( Name );
  
  if ( Index == -1 )
  {
    m_CurrentImportParams = NULL;
    m_ImportDescr.SetCurSel ( -1 );
  }
  else 
  {
    m_CurrentImportParams = (CImportParams*) m_ImportParamsList->GetAt ( Index );
    if ( !m_ImportParamsList->SetDefaultImportParams ( m_CurrentImportParams ) )
      m_ImportParamsList->SetDefaultImportParams ( NULL );
  }

  // update controls
  UpdateData ( FALSE );
  
  // 
  UpdateControlState();

  // update file content list view
  ClearFileContent();
}


void CDlgImportDescr::UpdateControlState()
{
  // enable/disable controls
  m_DelImportDescr.EnableWindow ( m_CurrentImportParams != NULL );
  m_Filename.EnableWindow ( m_CurrentImportParams != NULL );
  m_SelFilename.EnableWindow ( m_CurrentImportParams != NULL );
  m_SeparatorChar.EnableWindow ( m_CurrentImportParams != NULL );
  m_NumHeaderLines.EnableWindow ( m_CurrentImportParams != NULL );
  m_FileContent.EnableWindow ( m_CurrentImportParams != NULL );
  m_ViewFileContent.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldEA.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldDatum.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldBetrag.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldBruttoNetto.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldBeschreibung.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldBelegnummer.EnableWindow( m_CurrentImportParams != NULL );
  m_FieldKonto.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldSteuersatz.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldAfAAktJahr.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldAfADauer.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldEAFormatA.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldEAFormatE.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldDatumFormat.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldDatumDelimiter.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldBetragFormat.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldSteuersatzFormat.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldBNFormatBrutto.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldBNFormatNetto.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldAfARestwert.EnableWindow ( m_CurrentImportParams != NULL  );
  m_FieldAfARestwertFormat.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldAfADegSatz.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldAfADegSatzFormat.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldAfAGenauigkeit.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldBetrieb.EnableWindow ( m_CurrentImportParams != NULL );
  m_FieldBestandskonto.EnableWindow ( m_CurrentImportParams != NULL );

  // special treatment for controls without wrapper classes (labels)
  EnableDialogItem ( IDC_GROUPBOX1, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_GROUPBOX2, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC1, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC2, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC3, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC4, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC5, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC6, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC7, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC8, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC9, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC10, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC11, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC12, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC13, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC14, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC15, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC16, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC17, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC18, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC19, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC20, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC21, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC22, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC23, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC24, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC25, m_CurrentImportParams != NULL );
  EnableDialogItem ( IDC_STATIC26, m_CurrentImportParams != NULL );
}


void CDlgImportDescr::OnViewFileContent()
{
  // save current data
  UpdateData ( TRUE );
  // update file content list view
  UpdateFileContent();
  // update field combo boxes
  UpdateFieldComboBoxes ( FALSE );
}


BOOL CDlgImportDescr::GetColumnTitles ( CCSVFile *File )
{
  // check for invalid parameter
  if ( !m_CurrentImportParams )
    return FALSE;

  // clear the array of cols
  m_CurrentImportParams->GetColumnNames()->RemoveAll();

  /*
  // return some dummy field names if there is no (opened) file
  if ( File == NULL || File->m_hFile == CFile::hFileNull ) {
    int i;
    CString Fieldname;
    for ( i = 1; i <= 10; i++ ) 
      Fieldname.Format ( IDS_FIELDX, i );
    return;
  }
  */

  // determine how many columns are in the file
  // we use a two-way method:
  // 1. if we have headerlines, read them all and use file content for column titles
  // 2. otherwise we read 10 lines and build the column names like Field 1, Field 2, Field 3, Field ...
  int i, LineCount;
  CString Line ( _T("") );
  CStringArray TempColumns;

  // determine how many lines we will to read:
  if ( m_CurrentImportParams->GetHeaderCount() > 0 )
    LineCount = m_CurrentImportParams->GetHeaderCount();
  else 
    LineCount = 40;

  // read LineCount lines from file and fill Columns array with the "biggest" line 
  for ( i = 1; i <= LineCount; i++ )
  {
    try
    {
      if ( !File->ReadString ( Line ) )
        break;
    }
    catch ( CFileException *e )
    {
	  delete e;
      break;
    }

    File->ParseString ( Line, TempColumns, m_CurrentImportParams->GetSeparatorChar() );
    
    if ( TempColumns.GetSize() > m_CurrentImportParams->GetColumnNames()->GetSize() )
      m_CurrentImportParams->GetColumnNames()->Copy ( TempColumns );
  }

  // a little is left to do if we don't have header lines
  // - reset the file pointer to the beginning of the file
  // - build the generic column names, the number of columns is right now
  if ( m_CurrentImportParams->GetHeaderCount() == 0 )
  {
    File->SeekToBegin();
    for ( i = 0; i <= m_CurrentImportParams->GetColumnNames()->GetSize() - 1; i++ )
      m_CurrentImportParams->GetColumnNames()->ElementAt ( i ).Format ( IDS_FIELDX, i + 1 );
  }
  return TRUE;
}


void CDlgImportDescr::UpdateFieldComboBox ( CComboBox* ComboBox, CFieldConversion* Field, BOOL SaveAndValidate )
{
  // check for invalid parameter
  if ( !ComboBox || !ComboBox->m_hWnd )
    return;
  
  if ( SaveAndValidate )
  {
    if ( Field ) 
    {
      int Index = ComboBox->GetCurSel();
      if ( Index == CB_ERR )
      {
        BOOL Trans;
        Index = GetDlgItemInt ( ComboBox->GetDlgCtrlID(), &Trans, TRUE ) - 1;
        if ( Trans == 0 )
          Index = -1;
      }  
      Field->SetSourceFieldID ( Index );
    }
  }
  else
  {
    // clear existing entries
    ComboBox->ResetContent();

    // fill with values from string array
    if ( m_CurrentImportParams )
    {
      int i;
      for ( i = 0; i <= m_CurrentImportParams->GetColumnNames()->GetSize() - 1; i++ )
        ComboBox->AddString ( m_CurrentImportParams->GetColumnNames()->GetAt ( i ) );
    }

    // select/fill in current value
    if ( Field )
	{
		// Festwerte anfügen
		if (ComboBox->GetDlgCtrlID() == IDC_FIELD_STEUERSATZ)
		{
			ComboBox->AddString ( "(Festwert) " + m_EinstellungCtrl.HoleEinstellung("vat1") + "%" );
			ComboBox->AddString ( "(Festwert) " + m_EinstellungCtrl.HoleEinstellung("vat2") + "%" );
			ComboBox->AddString ( "(Festwert) " + m_EinstellungCtrl.HoleEinstellung("vat3") + "%" );
		}
		else if (ComboBox->GetDlgCtrlID() == IDC_FIELD_KONTO)
		{
			int i;
			CString kontonr, kontoname;
			char *ea;
			if (((CComboBox *)GetDlgItem(IDC_FIELD_EA))->GetCurSel() == 0) ea = "e";
			else if (((CComboBox *)GetDlgItem(IDC_FIELD_EA))->GetCurSel() == 1) ea = "a";
			else ea = "ea";
			unsigned int kontoart;
			for (kontoart = 0; kontoart < strlen(ea); kontoart++)
			{
				for ( i = 0; i < 100; i++)
				{
					kontonr.Format("%c%-02.2d", ea[kontoart], i);
					kontoname = m_EinstellungCtrl.HoleEinstellung(kontonr);
					if (!kontoname.GetLength()) break;
					ComboBox->AddString ( "(Festwert) " + kontoname );
				}
			}
		}
		else if (ComboBox->GetDlgCtrlID() == IDC_FIELD_BETRIEB)
		{
			int i;
			CString betriebnr, betriebname;
			for ( i = 0; i < 100; i++)
			{
				betriebnr.Format("[Betriebe]Betrieb%-02.2dName", i);
				betriebname = m_EinstellungCtrl.HoleEinstellung(betriebnr);
				if (!betriebname.GetLength()) break;
				ComboBox->AddString ( "(Festwert) " + betriebname );
			}
		}
		else if (ComboBox->GetDlgCtrlID() == IDC_FIELD_BESTANDSKONTO)
		{
			int i;
			CString bestandskontonr, bestandskontoname;
			for ( i = 0; i < 100; i++)
			{
				bestandskontonr.Format("[Bestandskonten]Bestandskonto%-02.2dName", i);
				bestandskontoname = m_EinstellungCtrl.HoleEinstellung(bestandskontonr);
				if (!bestandskontoname.GetLength()) break;
				ComboBox->AddString ( "(Festwert) " + bestandskontoname );
			}
		}
		if (ComboBox->GetDlgCtrlID() == IDC_FIELD_STEUERSATZ || ComboBox->GetDlgCtrlID() == IDC_FIELD_KONTO 
			|| ComboBox->GetDlgCtrlID() == IDC_FIELD_BETRIEB || ComboBox->GetDlgCtrlID() == IDC_FIELD_BESTANDSKONTO)
			if (Field->GetSourceFieldID() >= m_CurrentImportParams->GetColumnNames()->GetSize())
			{
				char festwert[1000];
				int nID = Field->GetSourceFieldID();
				if (ComboBox->GetLBTextLen(nID) < sizeof(festwert))
				{
					ComboBox->GetLBText(nID, festwert);	
					if (!strncmp(festwert, "(Festwert) ", 11))
						Field->SetFestwert(festwert+11);
				}
			}

		if ( Field->GetSourceFieldID() >= 0 && Field->GetSourceFieldID() <= ComboBox->GetCount() )
			ComboBox->SetCurSel ( Field->GetSourceFieldID() );
		else
			if ( Field->GetSourceFieldID() >= 0 )
				SetDlgItemInt ( ComboBox->GetDlgCtrlID(), Field->GetSourceFieldID() + 1, TRUE );
	}
  }
}

#define ID_VIEW_JOURNAL_SWITCH          32788

void CDlgImportDescr::OnOK() 
{
  // ensure that m_CurrentImportParams is filled with actual values
  UpdateData ( TRUE );

  if ( m_CurrentImportParams )
  {
    CString ErrorMsg;	
    if ( !m_CurrentImportParams->Validate ( &ErrorMsg ) )
    {
      AfxMessageBox ( ErrorMsg, MB_OK | MB_ICONSTOP );
      return;
    }
  }
  else 
  {
    AfxMessageBox ( IDS_NO_IMPORTDESCR_SELECTED, MB_OK | MB_ICONSTOP );
    return;
  }

  // call inherited
  CImportUIBase::OnOK();
  m_Result = IDOK;
  AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_VIEW_JOURNAL_SWITCH, 0L);	// -- zum schnelleren Debuggen
}


void CDlgImportDescr::OnCancel() 
{
	// ensure that m_CurrentImportParams is filled with actual values
	UpdateData ( TRUE );

	// call inherited
	CImportUIBase::OnCancel();
	m_Result = IDCANCEL;
	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_VIEW_JOURNAL_SWITCH, 0L);
}

void CDlgImportDescr::OnSize(UINT nType, int cx, int cy)
{
	CImportUIBase::OnSize(nType, cx, cy);

	if (m_FileContent && m_FileContent.GetItemCount()) // Liste mit neuer Spaltenbreite aufbauen, wenn Fenstergröße sich ändert
		UpdateFileContentColumnWidths();
}

void CDlgImportDescr::OnBnClickedOk()
{
	CDlgImportDescr::OnOK();				// IDOK = alle Buchungen importieren
}

void CDlgImportDescr::OnBnClickedOk2()
{
  // ensure that m_CurrentImportParams is filled with actual values
  UpdateData ( TRUE );

  if ( m_CurrentImportParams )
  {
    CString ErrorMsg;	
    if ( !m_CurrentImportParams->Validate ( &ErrorMsg ) )
    {
      AfxMessageBox ( ErrorMsg, MB_OK | MB_ICONSTOP );
      return;
    }
  }
  else 
  {
    AfxMessageBox ( IDS_NO_IMPORTDESCR_SELECTED, MB_OK | MB_ICONSTOP );
    return;
  }

  // call inherited
  CImportUIBase::OnOK();	
  m_Result = IDRETRY;	// IDRETRY = nur selektierte Buchungen importieren
  AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_VIEW_JOURNAL_SWITCH, 0L);
}

void CDlgImportDescr::OnLvnItemchangedFilecontent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	(GetDlgItem(IDOK2))->EnableWindow(true);
	*pResult = 0;
}

void CDlgImportDescr::OnCbnSelchangeFieldEa()
{
	UpdateFieldComboBox ( &m_FieldKonto, m_CurrentImportParams->GetFieldKonto(), FALSE );
}

void CDlgImportDescr::OnBnClickedInfo()
{
	CDlgAbout dlg;
	dlg.DoModal();
}


void CDlgImportDescr::OnHdnEndtrackFilecontent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	m_CurrentImportParams->SetColumnWidth(phdr->iItem, -1);	// Seltsamerweise kennt die notification die neue column width noch nicht. müssen wir später nachtragen. hier erstmal auf '-1' 'touched' setzen!
	*pResult = 0;
}

void CDlgImportDescr::OnCbnSelchangeFieldBestandskonto()
{
	// TODO: Add your control notification handler code here
}

void CDlgImportDescr::OnBnClickedSelfilename2()
{
	// TODO: Add your control notification handler code here
}

void CDlgImportDescr::OnBnClickedEditgawkscript()
{
	CString GawkScript;
	m_GawkScript.GetWindowText ( GawkScript );
	if (GawkScript.IsEmpty()) 
	{
		AfxMessageBox("Nachdem Sie das GAWK-Script mit dem Texteditor erstellt haben, speichern Sie es am Besten mit einem sinnvollen Namen und der Endung '.awk' im Datenverzeichnis ab. Danach muss diese Datei noch einmal mit dem linksstehenden Durchsuchen-Knopf ausgewählt werden.");
		
		CString Filename;

		if ( GetIniFileName ( Filename.GetBuffer ( MAX_PATH ), MAX_PATH ) )
		{
			char *cp = Filename.GetBuffer ( MAX_PATH );
			if (cp = strrchr(cp, '\\'))
				strcpy(cp, "\\MeinSkript.awk");
		}

		::ShellExecute(NULL, "open", "notepad.exe", Filename.GetBuffer(0), NULL, SW_SHOWNORMAL);
	}
	else
		::ShellExecute(NULL, "edit", GawkScript, NULL, NULL, SW_SHOWNORMAL);
}

void CDlgImportDescr::OnBnClickedExegawkscript()
{
	extern CECTImportXApp theApp;
	char exefilename[MAX_PATH];
	char exefilenameescaped[MAX_PATH];
	char commandline[MAX_PATH+1000];
	char *cp, *cp2;
	if (::GetModuleFileName(theApp.m_hInstance, exefilename, MAX_PATH) && (cp = strrchr(exefilename, '\\')))
	{
		// hole Werte aus edit controls
		CString SeparatorChar;
		m_SeparatorChar.GetWindowText ( SeparatorChar );
		CString GawkScript;
		m_GawkScript.GetWindowText ( GawkScript );
		CString Filename;
		m_Filename.GetWindowText ( Filename );
		if (Filename.Right(6) == ".awked")
			Filename = Filename.Left(Filename.GetLength()-6);

		// Kommandozeile zusammensetzen und ausführen
		strcpy(cp, "\\gawk.exe");
		/* cp = exefilename;
		cp2 = exefilenameescaped;
		while (*cp2 = *cp) // blödes cmd.exe escaping...
		{
			if (*cp == ' ' || *cp == '&' || *cp == '<' || *cp == '>' || *cp == '(' || *cp == ')' || *cp == '@' || *cp == '^' || *cp == '|')
			{
				*cp2++ = '^';
				*cp2 = *cp;
			}
			cp2++;
			cp++;
		} ich gebs auf! GetShortPathName() stattdessen */ 
		int nChars = GetShortPathName(exefilename, exefilenameescaped, sizeof(exefilenameescaped));
		if (nChars == 0) strcpy(exefilenameescaped, exefilename);  // wenn es schon ein "short path" war, einfach kopieren
		sprintf(commandline, "/C %s -F \"%s\" -f \"%s\" \"%s\" > \"%s.awked\" & pause", exefilenameescaped, SeparatorChar.GetBuffer(0), (LPCTSTR)GawkScript, (LPCTSTR)Filename, (LPCTSTR)Filename);
		::ShellExecute(NULL, "open", "cmd.exe", commandline, NULL, SW_SHOWNORMAL);
		Filename += ".awked";

		CFile theFile;
		CTime now, start = CTime::GetCurrentTime();
		BOOL bTimeout = FALSE;
		while (!theFile.Open( Filename, CFile::modeRead|CFile::shareExclusive ))
		{
			now = CTime::GetCurrentTime();
			if (now - start > CTimeSpan(0, 0, 0, 10))
			{
				bTimeout = TRUE;
				break;
			}
		}
		if (!bTimeout) theFile.Close();

		m_Filename.SetWindowText(Filename);

		if (bTimeout)
			AfxMessageBox("Das Script ist noch nicht fertig. Bitte manuell auf 'Anzeigen' klicken, sobald es fertig ist.");
		else
			SetTimer(1, 1000, NULL);
	}
	else
		AfxMessageBox("Konnte das Plugin-Unterverzeichnis von CSVImport nicht ermitteln.");
}

void CDlgImportDescr::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(1);
		OnViewFileContent();
	}

	CImportUIBase::OnTimer(nIDEvent);
}

BOOL CDlgImportDescr::OnHelpInfo(HELPINFO* pHelpInfo)  
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

//conversion vrom UTF8 to ANSI
void CDlgImportDescr::OnBnClickedUtf8convert()
{
	CFile File;
	CFileException Error;
	CFileStatus FileStatus;

	UpdateData(TRUE);
	try
	{
		// remove ".awked" suffix
		CString OriginalFilename = m_CurrentImportParams->GetFilename();
		CString Filename = OriginalFilename;
		if (Filename.Right(6) == ".awked")
			Filename = Filename.Mid(0, Filename.GetLength() - 6);
		if (Filename.Right(5) == ".ansi")
			Filename = Filename.Mid(0, Filename.GetLength() - 5);

		// try to open file
		if (!File.Open(Filename, CFile::modeRead | CFile::shareDenyWrite, &Error))
		{
			LPTSTR MsgBuf = NULL;
			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, Error.m_lOsError, 0, (LPTSTR)&MsgBuf, 0, NULL))
			{
				AfxMessageBox(MsgBuf);
				LocalFree(MsgBuf);
			}
			return;
		}

		// is file empty?
		if (!File.GetStatus(FileStatus))
			return;
		if (FileStatus.m_size == 0)
		{
			CString csErr;
			csErr.LoadStringA(IDS_IMPERR_FILE_EMPTY);
			AfxMessageBox(csErr);
			File.Close();
			return;
		}

		// convert file
		CStringA Filebuffer;
		LPVOID cpFilebuffer = Filebuffer.GetBuffer(FileStatus.m_size + 1);
		UINT nBytesRead = File.Read(cpFilebuffer, FileStatus.m_size);
		if (nBytesRead != FileStatus.m_size)
		{	// sollte nicht passieren wegen "CFile::shareDenyWrite"
			AfxMessageBox(_T("Dateigröße schwankt. Wird die Datei gegenwärtig noch beschrieben? Dann bitte warten bis der Schreibvorgang beendet ist."));
			File.Close();
			return;
		}
		else
			((char *)cpFilebuffer)[nBytesRead] = '\0';
		Filebuffer.ReleaseBuffer();

		// close file
		File.Close();

		// convert
		CString FilebufferAnsi;
		Utf8toAnsi(Filebuffer, FilebufferAnsi);

		// check filename
		CFile FileAnsi;
		CString FilenameAnsi = Filename;
		int posAnsi = FilenameAnsi.Find(".ansi");
		if (posAnsi >= 0)
		{
			AfxMessageBox("Datei wurde anscheinend schon von UTF-8 in das ANSI-Format konvertiert. Bitte das '.ansi' im Dateinamen entfernen, wenn noch einmal konvertiert werden soll.");
			return;
		}

		// modify filename to point to converted file
		FilenameAnsi += ".ansi";

		// try to open file for writing
		if (!FileAnsi.Open(FilenameAnsi, CFile::modeWrite | CFile::modeCreate, &Error))
		{
			LPTSTR MsgBuf = NULL;
			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, Error.m_lOsError, 0, (LPTSTR)&MsgBuf, 0, NULL))
			{
				AfxMessageBox(MsgBuf);
				LocalFree(MsgBuf);
			}
			return;
		}

		LPVOID cpFilebufferAnsi;
		FileAnsi.Write(FilebufferAnsi.GetBuffer(0), FilebufferAnsi.GetLength()-1);
		FilebufferAnsi.ReleaseBuffer();
		FileAnsi.Close();

		// Dateinamen in Dialog aktualisieren
		CString csMessage = "Konvertierung Der Importdatei von UTF-8 nach ANSI erfolgreich. Bitte jetzt '";
		if (OriginalFilename.Right(6) == ".awked")  // und ggf. .awked-Endung restaurieren
		{
			csMessage += "Ausführen";
			FilenameAnsi += ".awked";
		}
		else
			csMessage += "Anzeigen...";
		csMessage += "' klicken.";
		AfxMessageBox(csMessage);
		m_CurrentImportParams->SetFilename(FilenameAnsi);
		UpdateData(FALSE);
	}
	catch (CFileException* e)
	{
		LPTSTR MsgBuf = NULL;
		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, e->m_lOsError, 0, (LPTSTR)&MsgBuf, 0, NULL))
		{
			AfxMessageBox(MsgBuf);
			LocalFree(MsgBuf);
		}
		delete e;
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("Unerwartete Ausnahme beim Konvertieren der Import-Datei von UTF-8 nach Ansi"));
		return;
	}
}
