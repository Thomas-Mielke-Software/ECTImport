/*//////////////////////////////////////////////////////////////////////////////
// Name:        import.cpp
// Purpose:     Implementation of CImport
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include <math.h>
#include "easycashdoc.h"
#include "Import.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// CImportErrorList
//////////////////////////////////////////////////////////////////////

CImportErrorList::~CImportErrorList()
{
  RemoveAll();
}


int CImportErrorList::Add ( int LineNo, const CString& Msg )
{
  CImportError* NewElement = new CImportError ( LineNo, Msg );
  return CObArray::Add ( NewElement );
}


void CImportErrorList::FreeObjects ( int StartIndex, int Count )
{
  int i = StartIndex;
  while ( i < StartIndex + Count )
  {
    if ( GetAt ( i ) )
      delete GetAt ( i );
    i++;
  }
}


void CImportErrorList::RemoveAll ()
{
  // free mem used by the items
  FreeObjects ( 0, GetSize() );

  // call "inherited" 
  CObArray::RemoveAll();
}


void CImportErrorList::RemoveAt ( int nIndex, int nCount )
{
  // free mem used by the items
  FreeObjects ( nIndex, nCount );

  // call "inherited" 
  CObArray::RemoveAt ( nIndex, nCount );
}


//////////////////////////////////////////////////////////////////////
// CImport
//////////////////////////////////////////////////////////////////////

CImport::CImport()
{
  m_pDoc = NULL;
  m_pParams = NULL;
  m_ParamsOwner = FALSE;
}


CImport::~CImport()
{
  if ( m_ParamsOwner && m_pParams )
    delete m_pParams;
}


void CImport::SetParams ( CImportParams* const Value )
{ 
  if ( m_ParamsOwner && m_pParams )
    delete m_pParams;
  m_pParams = Value; 
  m_ParamsOwner = FALSE;
}


void CImport::AssignParams ( CImportParams* const Value )
{
  SetParams ( NULL );

  if ( !m_ParamsOwner )
    m_pParams = new CImportParams();

  *m_pParams = *Value;

  m_ParamsOwner = TRUE;
}


void CImport::Init()
{
  m_UserInterrupt = FALSE;
  m_CurrentLine = 0;
  m_ImportCount = 0;
  m_Errors.RemoveAll();
}


BOOL CImport::ConfirmUserInterrupt ( HWND Owner )
{
  CString Message, Caption;
  Message.LoadString ( IDS_CONFIRM_USERINTERRUPT_MESSAGE );
  Caption.LoadString ( IDS_CONFIRM_USERINTERRUPT_CAPTION );

  int Res = MessageBox ( Owner, Message, Caption, MB_TASKMODAL || MB_ICONQUESTION || MB_YESNO || MB_DEFBUTTON2 );

  if ( Res == IDOK || Res == IDYES )
    return TRUE;
  else
    return FALSE;
}


BOOL CImport::ExchangeField ( CFieldConversion* Field, CImportBuchung* pBuchung, CStringArray* LinePart, void* Dest ) 
{
  //
  Field->SetBuchung ( pBuchung );
  Field->SetLine ( LinePart );
  
  //
  if ( !Field->Exchange ( Dest ) )
  {
    if ( Field->GetErrorMsg().IsEmpty() )
    {
      CString ErrorMsg;
      if ( Field->GetSourceFieldID() >= 0 && Field->GetSourceFieldID() <= LinePart->GetSize() - 1 )
        ErrorMsg.Format ( IDS_IMPERR_CONVERSION_FALIED, Field->GetSourceFieldID() + 1, LinePart->GetAt ( Field->GetSourceFieldID() ), Field->GetDataTypeName() );
      else
        ErrorMsg.Format ( IDS_IMPERR_CONVERSION_FALIED, Field->GetSourceFieldID() + 1, _T(""), Field->GetDataTypeName() );
      m_Errors.Add ( m_CurrentLine, ErrorMsg );
    }
    else
    {
      m_Errors.Add ( m_CurrentLine, Field->GetErrorMsg() );
    }
    return FALSE;
  }

  return TRUE;
}


CImportBuchung* CImport::CreateNewBuchung()
{
	CImportBuchung* Result = new CImportBuchung();

	Result->Betrag = 0;
	Result->MWSt = 0;
	Result->AbschreibungNr = 1;
	Result->AbschreibungJahre = 1;
	Result->Beschreibung = _T("");
	Result->Datum = CTime::GetCurrentTime(); 

	Result->Konto = _T("");
	Result->next = NULL;
  
	Result->Belegnummer = _T("");
    Result->AbschreibungRestwert = 0;
    Result->AbschreibungDegressiv = FALSE;
    Result->AbschreibungSatz = 0;
    Result->AbschreibungGenauigkeit = 3;
	
    Result->Betrieb = _T("");
    Result->Bestandskonto = _T("");

	return Result;
}


void CImport::AppendBuchung ( CImportBuchung* const Buchung, CEAFieldConversion::EinnahmeAusgabe EA, CBruttoNettoFieldConversion::BruttoNetto BN )
{
  // adjust CImportBuchung::Betrag when importing net-bookings and VAT involved
  // ECT expects CImportBuchung::Betrag to be the amount incl. VAT
  if ( BN == CBruttoNettoFieldConversion::bnNetto && Buchung->MWSt != 0 ) {
    if ( Buchung->Betrag >= 0 && Buchung->MWSt >= 0 )
      Buchung->Betrag += (int)( (long long) ( (long long) Buchung->Betrag * (long long) Buchung->MWSt / 1000LL ) + 5LL ) / 10LL ;
    else
      Buchung->Betrag += (int)( (long long) ( (long long) Buchung->Betrag * (long long) Buchung->MWSt / 1000LL ) - 5LL ) / 10LL ;
  }

  // adjust VAT percentage in booking:
  // it's stored in 1/1000 units and we import 1/100 units
  // this MUST occur AFTER adjusting CImportBuchung:::Betrag
  Buchung->MWSt *= 10;
  
  // adjust EA
  if ( EA == CEAFieldConversion::eaUnknown )
  {
    if ( Buchung->Betrag >= 0 )
      EA = CEAFieldConversion::eaIncome;
    else 
    {
      EA = CEAFieldConversion::eaExpense;
      Buchung->Betrag = Buchung->Betrag * ( -1 );
    }
  }
  
  // adjust AfA settings
  {
    if ( Buchung->AbschreibungSatz != 0 )
    {
      Buchung->AbschreibungDegressiv = TRUE;
    }
    // calc restwert if not provided
    if ( m_pParams->GetFieldAfARestwert()->GetSourceFieldID() == -1  )
    {
			long netto = Buchung->GetNetto();
			for ( int i = Buchung->AbschreibungNr; i <= Buchung->AbschreibungJahre; i++ )
			{
				Buchung->AbschreibungRestwert += 
          ( netto / Buchung->AbschreibungJahre ) +
					( netto % Buchung->AbschreibungJahre >= Buchung->AbschreibungNr ? 1 : 0);	// Rundungsfehler auf die ersten Jahre aufteilen!
			}
    }
  }

  if ( m_pParams->GetFieldAfAGenauigkeit()->GetSourceFieldID() == -1  )
	  Buchung->AbschreibungGenauigkeit = 3; // entsp. den aktuellen Einstellungen

  long DokumentVersion = m_pDokumentCtrl->HoleDokumentVersion();

  // append booking according to type type (einnahme/ausgabe)
  LONG NewBooking;
  if ( EA == CEAFieldConversion::eaIncome ) 
	  NewBooking = m_pDokumentCtrl->NeueEinnahmenbuchungEinfuegen();
  else 
	  NewBooking = m_pDokumentCtrl->NeueAusgabenbuchungEinfuegen();

  // fill in values
  m_pBuchungCtrl->SetID(NewBooking);	// all the following value assignments belong to this booking
  CY cyBetrag;
  //cyBetrag.Hi = Buchung->Betrag / 100;
  //cyBetrag.Lo = Buchung->Betrag % 100 * 100; // 4 digits to the right of the decimal point
  HRESULT hResult = VarCyFromR8((double)Buchung->Betrag / 100.0, &cyBetrag);
  if (hResult != S_OK)
  {
	  TRACE("VarCyFromR8 returning %-08.8x\r\n", hResult);
  }
  m_pBuchungCtrl->SetBetrag(cyBetrag);
  m_pBuchungCtrl->SetMWSt((double)Buchung->MWSt / 1000);
  m_pBuchungCtrl->SetAbschreibungNr(Buchung->AbschreibungNr);
  m_pBuchungCtrl->SetAbschreibungJahre(Buchung->AbschreibungJahre);
  m_pBuchungCtrl->SetBeschreibung(Buchung->Beschreibung);
  SYSTEMTIME SysTime;
  double OleTime;
  if (Buchung->Datum.GetAsSystemTime(SysTime) && SystemTimeToVariantTime(&SysTime, &OleTime))
    m_pBuchungCtrl->SetDatum(OleTime);	// besser so!
  else 
  { // funktioniert nicht so gut: 
    CTimeSpan ctsConvert = Buchung->Datum - CTime(1980, 1, 1, 0, 0, 0); // CTime to OLE DATE
    m_pBuchungCtrl->SetDatum((double)ctsConvert.GetDays()+29222.01/*80 Jahre und 1 Tag dazu dazu, weil DATE ab 31.12.1899 gerechnet wird und GetDays kann nur ab 1970 rechnen. Außerdem: ~1 Stunde (0.04) hinzuaddieren wegen möglicher Real-Rundungsfehler */);	
  }
  // evtl. lieber SystemTimeToVariantTime() benutzen? 
  m_pBuchungCtrl->SetKonto(Buchung->Konto);
  m_pBuchungCtrl->SetBelegnummer(Buchung->Belegnummer);
  CY cyAbschreibungRestwert;
  //cyAbschreibungRestwert.Hi = Buchung->AbschreibungRestwert / 100;
  //cyAbschreibungRestwert.Lo = Buchung->AbschreibungRestwert % 100 * 100; // 4 digits to the right of the decimal point
  hResult = VarCyFromR8((double)Buchung->AbschreibungRestwert / 100.0, &cyAbschreibungRestwert);
  if (hResult != S_OK)
  {
	  TRACE("VarCyFromR8 returning %-08.8x\r\n", hResult);
  }
  m_pBuchungCtrl->SetAbschreibungRestwert(cyAbschreibungRestwert);
  m_pBuchungCtrl->SetAbschreibungDegressiv(Buchung->AbschreibungDegressiv);
  m_pBuchungCtrl->SetAbschreibungSatz(Buchung->AbschreibungSatz);
  if (DokumentVersion < 10) return;
  m_pBuchungCtrl->SetAbschreibungGenauigkeit(Buchung->AbschreibungGenauigkeit);
  if (DokumentVersion < 11) return;
  m_pBuchungCtrl->SetBetrieb(Buchung->Betrieb);
  m_pBuchungCtrl->SetBestandskonto(Buchung->Bestandskonto);
}


BOOL CImport::Execute(CByteArray *pSelectionArray) {
  // invalid parameters?
  if ( !m_pDoc || !m_pParams )
  {
    CString ErrorMsg;
    m_Errors.Add ( 0, CString ( MAKEINTRESOURCE ( IDS_IMPERR_INVALIDPARAMS ) ) );
    return FALSE;
  }

  // are the import parameters correct?
  CString ErrorMsg;
  if ( !m_pParams->Validate ( &ErrorMsg ) )
  {
    m_Errors.Add ( 0, ErrorMsg );
    return FALSE;
  }

  // initialize
  Init();

  // vars
  BOOL OK = TRUE;
  CCSVFile File;
  CFileException Error;
  CFileStatus FileStatus;
  CString Line;
  CStringArray LinePart;
  CImportBuchung* pBuchung;
  CEAFieldConversion::EinnahmeAusgabe EA;
  CBruttoNettoFieldConversion::BruttoNetto BN;
  DWORD StartTime;
  CDlgProgress* DlgProgress = NULL;

  //
  StartTime = GetTickCount();

  try 
  {
    // try to open file
    if ( !File.Open ( m_pParams->GetFilename(), CFile::modeRead | CFile::shareDenyWrite, &Error ) )
    {
      LPTSTR MsgBuf = NULL;
      if ( FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, Error.m_lOsError, 0, (LPTSTR) &MsgBuf, 0, NULL ) )
      {
        m_Errors.Add ( 0, MsgBuf );
        LocalFree ( MsgBuf );
      }
      return FALSE;
    }

    // is file empty?
    if ( !File.GetStatus ( FileStatus ) )
      return FALSE;
    if ( FileStatus.m_size == 0 )
    {
	  CString csErr;
	  csErr.LoadStringA(IDS_IMPERR_FILE_EMPTY);
      m_Errors.Add ( 0, csErr );
      return FALSE;
    }

    // read the header lines and ignore them
    int i;
    for ( i = 1; i <= m_pParams->GetHeaderCount(); i++ )
    {
      if ( !File.ReadString ( Line ) ) 
      {
        CString ErrorMsg;
        ErrorMsg.Format ( IDS_IMPERR_EOFINHEADER, m_CurrentLine );
        m_Errors.Add ( 0, ErrorMsg );
        return FALSE;
      }
      m_CurrentLine++;
    }

    while ( File.ReadString ( Line ) )
    {
      // increment line counter
      m_CurrentLine++;

      // is it time to show a progress dialog? 
      if ( !DlgProgress && GetTickCount() - StartTime > 100 /* && File.GetPosition() * 100 / FileStatus.m_size <= 25  */ )
      {
        // init and show progress window
        DlgProgress = new CDlgProgress ( IDS_PROGRESS_CAPTION );
        DlgProgress->Create ( NULL );
		DlgProgress->ShowWindow(SW_SHOW);
        DlgProgress->SetStep ( 1 );
        DlgProgress->SetRange ( 0, (int)FileStatus.m_size );
        DlgProgress->SetStatus ( IDS_PROGRESS_STATUS_IMPORTING );
      }
      
      // update progress and check for user interrupt; only if progress window is active
      if ( DlgProgress )
      {
        DlgProgress->SetPos ( (int)File.GetPosition() );
        if ( DlgProgress->CheckCancelButton() && ConfirmUserInterrupt ( DlgProgress->m_hWnd ) )
        {
          m_UserInterrupt = TRUE;
          break;
        }
      }

	  if (!pSelectionArray || (m_CurrentLine-1-m_pParams->GetHeaderCount() < pSelectionArray->GetSize() && pSelectionArray->GetAt(m_CurrentLine-1-m_pParams->GetHeaderCount())))
	  {
		  // split line
		  File.ParseString ( Line, LinePart, m_pParams->GetSeparatorChar() );
	      
		  // Empty (means no fields, not no chars) line?
		  int i;
		  BOOL IsEmpty = TRUE;
		  for ( i = 0; i <= LinePart.GetSize() - 1; i++ )
			if ( !LinePart.GetAt ( i ).IsEmpty() ) 
			{
			  IsEmpty = FALSE;
			  break;
			}

		  if ( LinePart.GetSize() == 0 )
		  {
			CString ErrorMsg;
			ErrorMsg.Format ( IDS_IMPERR_LINE_EMPTY, m_CurrentLine );
			m_Errors.Add ( 0, ErrorMsg );
			continue;
		  }

		  // create new CImportBuchung object
		  pBuchung = CreateNewBuchung();
		  //pBuchung = new CImportBuchung();

		  // load fieldvalues into CImportBuchung object
		  EA = CEAFieldConversion::eaUnknown;
		  BOOL exchangeSucceeded;
		  exchangeSucceeded = ExchangeField ( m_pParams->GetFieldEA(), pBuchung, &LinePart, &EA ) &&
							  ExchangeField ( m_pParams->GetFieldBruttoNetto(), pBuchung, &LinePart, &BN ) &&
							  ExchangeField ( m_pParams->GetFieldDatum(), pBuchung, &LinePart, &(pBuchung->Datum) ) &&
							  ExchangeField ( m_pParams->GetFieldBetrag(), pBuchung, &LinePart, &(pBuchung->Betrag) ) &&
							  ExchangeField ( m_pParams->GetFieldBeschreibung(), pBuchung, &LinePart, &(pBuchung->Beschreibung) ) &&
							  ExchangeField ( m_pParams->GetFieldKonto(), pBuchung, &LinePart, &(pBuchung->Konto) ) &&
							  ExchangeField ( m_pParams->GetFieldSteuersatz(), pBuchung, &LinePart, &(pBuchung->MWSt) ) &&
							  ExchangeField ( m_pParams->GetFieldAfAAktJahr(), pBuchung, &LinePart, &(pBuchung->AbschreibungNr) ) &&
							  ExchangeField ( m_pParams->GetFieldAfADauer(), pBuchung, &LinePart, &(pBuchung->AbschreibungJahre) ) && 
							  ExchangeField ( m_pParams->GetFieldBelegnummer(), pBuchung, &LinePart, &(pBuchung->Belegnummer) ) &&
							  ExchangeField ( m_pParams->GetFieldAfADegSatz(), pBuchung, &LinePart, &(pBuchung->AbschreibungSatz) ) &&
							  ExchangeField ( m_pParams->GetFieldAfARestwert(), pBuchung, &LinePart, &(pBuchung->AbschreibungRestwert) ) &&
							  ExchangeField ( m_pParams->GetFieldAfAGenauigkeit(), pBuchung, &LinePart, &(pBuchung->AbschreibungGenauigkeit) ) &&
							  ExchangeField ( m_pParams->GetFieldBetrieb(), pBuchung, &LinePart, &(pBuchung->Betrieb) ) &&
							  ExchangeField ( m_pParams->GetFieldBestandskonto(), pBuchung, &LinePart, &(pBuchung->Bestandskonto) );
/*		  {time_t ttt =1242770400; 
		  CTime ct(ttt); 
		  int d = ct.GetDay(); 
		  d = d;
		  }
*/
		  // finally add the booking, if everything went well
		  if ( exchangeSucceeded )
		  {
			AppendBuchung ( pBuchung, EA, BN );
			m_ImportCount++;
		  }
		  else
		  {
			OK = FALSE;
		  }
		  delete pBuchung;
	  }
    }

    // close file
    File.Close();
  }
  catch ( CFileException E )
  {
      LPTSTR MsgBuf = NULL;
      if ( FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, E.m_lOsError, 0, (LPTSTR) &MsgBuf, 0, NULL) )
      {
        m_Errors.Add ( 0, MsgBuf );
        LocalFree ( MsgBuf );
      }
	  if ( DlgProgress ) delete DlgProgress;
      return FALSE;
  }
  catch ( ... )
  {
    m_Errors.Add ( 0, CString ( MAKEINTRESOURCE ( IDS_IMPERR_UNKNOWN ) ) );
	if ( DlgProgress ) delete DlgProgress;
    return FALSE;
  }
  
  if ( DlgProgress ) {
    DlgProgress->SetPos( 0 );
    DlgProgress->SetStatus( IDS_PROGRESS_STATUS_SORTING );
  }

  // sort
  m_pDokumentCtrl->Sortieren();

  // remove progress window if it was created
  if ( DlgProgress ) delete DlgProgress;
  
  return OK;
}
