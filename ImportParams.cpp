/*//////////////////////////////////////////////////////////////////////////////
// Name:        importparams.cpp
// Purpose:     Implementation of CImportParams, CFieldConversion (and descendants)
//              including several dependant classes
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "easycashdoc.h"
#include "ImportParams.h"
#include "module.h"
#include "utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CFieldConversion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL ( CFieldConversion, CObject, CSVIMPORT_FILE_VERSION );


CFieldConversion::CFieldConversion()
{
  m_DataTypeName.LoadString ( IDS_DATATYPE_UNKNOWN );
  m_Name = _T("");
  Init();
}


void CFieldConversion::AssignFrom ( CFieldConversion* const Source )
{
  if ( Source == NULL )
    Init();
  else {
    m_DataTypeName = Source->m_DataTypeName;
    m_pBuchung = Source->m_pBuchung;
    m_SourceFieldID = Source->m_SourceFieldID;
    m_Name = Source->m_Name;
    m_Festwert = Source->m_Festwert;
  }
}


void CFieldConversion::Init()
{
  m_pBuchung = NULL;
  m_Line = NULL;
  m_SourceFieldID = -1;
}


BOOL CFieldConversion::CheckSourceFieldID()
{
  //
  if ( m_SourceFieldID < 0 || ( m_SourceFieldID > m_Line->GetSize() - 1 && m_Name != "Konto" && m_Name != "Steuersatz"  && m_Name != "Betrieb"  && m_Name != "Bestandskonto" ) )
  {
    m_ErrorMsg.Format ( IDS_IMPERR_INVALID_FIELD_INDEX, m_SourceFieldID + 1, 1, m_Line->GetSize() );
    return FALSE;
  }
  return TRUE;
}


void CFieldConversion::Serialize ( CArchive& ar )
{
  CObject::Serialize ( ar );
  if ( ar.IsStoring() )
  {
    ar << m_SourceFieldID;
  }
  else
  {
    ar >> m_SourceFieldID;
  }
}


CFieldConversion& CFieldConversion::operator = ( CFieldConversion& RightSideArgument )
{
  AssignFrom ( &RightSideArgument );
  return *this;
}


BOOL CFieldConversion::Exchange ( void* Dest )
{
  m_ErrorMsg = _T("");
  if ( !m_pBuchung || !m_Line )
  {
    m_ErrorMsg.LoadString ( IDS_IMPERR_INVALID_PARAMETERS );
    return FALSE;
  }

  return TRUE;
}


BOOL CFieldConversion::LoadFromIniFile ( const CString &Filename, const CString& Sectionname )
{
  m_SourceFieldID = GetPrivateProfileInt ( Sectionname, INI_KEYNAME_SOURCEFIELDID + GetName(), -1, Filename );

  return TRUE;
}


BOOL CFieldConversion::SaveToIniFile ( const CString &Filename, const CString& Sectionname )
{
  WritePrivateProfileInt ( Sectionname, INI_KEYNAME_SOURCEFIELDID + GetName(), m_SourceFieldID, Filename );

  return TRUE;
}


//////////////////////////////////////////////////////////////////////
// CIntFieldConversion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL ( CIntFieldConversion, CFieldConversion, CSVIMPORT_FILE_VERSION );


CIntFieldConversion::CIntFieldConversion()
  : CFieldConversion()
{
  m_DataTypeName.LoadString ( IDS_DATATYPE_INT );
}


void CIntFieldConversion::Serialize ( CArchive& ar )
{
  CFieldConversion::Serialize ( ar );
}


BOOL CIntFieldConversion::Exchange ( void* Dest )
{
  // call inherited
  if ( !CFieldConversion::Exchange ( Dest ) )
    return FALSE;

  if ( GetSourceFieldID() == -1 )
    return TRUE;

  if ( !CheckSourceFieldID() )
    return FALSE;
  
  // declare vars
  CString Source = GetLine().GetAt ( GetSourceFieldID() );
  CString AllowedChars = _T("0123456789");

  // remove trailing spaces; leading spaces are ignored by _ttoi
  Source.TrimRight();

  // handle blank source
  if ( Source.IsEmpty() )
  {
    switch ( m_BlankHandling )
    {
      case bhBlankIsError:
        return FALSE;

      case bhBlankIsZero:
        Source = _T("0");
        break;

      case bhIgnoreBlank:
        return TRUE;

      default:
        return FALSE;
    }
  }

  // check for valid chars
  int i;
  for ( i = 0; i <= Source.GetLength() - 1; i++ )
    if ( AllowedChars.Find ( Source.GetAt ( i ) ) == -1 )
      return FALSE;

  *((int*) Dest) = _ttoi ( Source );

  return TRUE;
}


void CIntFieldConversion::AssignFrom ( CFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
}


void CIntFieldConversion::AssignFrom ( CIntFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
  SetBlankHandling ( Source->m_BlankHandling );
}


void CIntFieldConversion::Init()
{
  CFieldConversion::Init();
  m_BlankHandling = bhIgnoreBlank;
}


//////////////////////////////////////////////////////////////////////
// CDateFieldConversion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL ( CDateFieldConversion, CFieldConversion, CSVIMPORT_FILE_VERSION );


CDateFieldConversion::CDateFieldConversion()
  : CFieldConversion()
{
  m_DataTypeName.LoadString ( IDS_DATATYPE_DATE );
}


void CDateFieldConversion::Serialize ( CArchive& ar )
{
  CFieldConversion::Serialize ( ar );
  if ( ar.IsStoring() )
  {
    ar << m_Format;
    ar.Write ( &m_Format, sizeof ( DateFormat ) );
    ar << m_Delimiter;
  }
  else
  {
    ar.Read ( &m_Format, sizeof ( DateFormat ) );
    ar >> m_Delimiter;
  }
}


void CDateFieldConversion::AssignFrom ( CFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
}


void CDateFieldConversion::AssignFrom ( CDateFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
  SetFormat ( Source->GetFormat () );
  SetDelimiter ( Source->GetDelimiter() );
}


void CDateFieldConversion::Init()
{
  CFieldConversion::Init();
  m_Format = dfDMYY;
  m_Delimiter = _T(".");
}


BOOL CDateFieldConversion::Exchange ( void* Dest )
{
  if ( !CFieldConversion::Exchange ( Dest ) )
    return FALSE;

  if ( GetSourceFieldID() == -1 )
    return TRUE;

  if ( !CheckSourceFieldID() )
    return FALSE;
  
  CString Source = GetLine().GetAt ( GetSourceFieldID() );
  CString AllowedChars = _T("0123456789") + m_Delimiter;
  CString DatePartStr[] = { CString ( _T("") ), CString ( _T("") ), CString ( _T("") ) };  // 0=YY 1=MM 2=DD
  CString YearStr, MonthStr, DayStr;

  if ( !m_Delimiter.IsEmpty() )
  {
    int CurrentDatePart = 0;
    int i;
    for ( i = 0; i <= Source.GetLength() - 1; i++ )
    {
	  if ( Source.GetAt ( i ) == ' ' && CurrentDatePart >= 2 ) break; // --- just ignore possibly attached time 
      if ( AllowedChars.Find ( Source.GetAt ( i ) ) == -1 ) 
        return FALSE;
      if ( Source.GetAt ( i ) == m_Delimiter.GetAt ( 0 ) )
      {
        CurrentDatePart++;
        if ( CurrentDatePart > 2 ) 
		   return FALSE;	
      }
      else 
        DatePartStr[CurrentDatePart] += Source.GetAt ( i );
    }
    if ( m_Format == dfDMY || m_Format == dfDMYY )
    {
      YearStr = DatePartStr[2];
      MonthStr = DatePartStr[1];
      DayStr = DatePartStr[0]; 
    }
    else if ( m_Format == dfMDY || m_Format == dfMDYY )
    {
      YearStr = DatePartStr[2];
      MonthStr = DatePartStr[0];
      DayStr = DatePartStr[1]; 
    }
    else if ( m_Format == dfYMD || m_Format == dfYYMD )
    {
      YearStr = DatePartStr[0];
      MonthStr = DatePartStr[1];
      DayStr = DatePartStr[2]; 
    }
    else 
      return FALSE;
  }
  else 
  {
    switch ( m_Format ) 
    {
      case dfDMY: {
        DayStr = Source.Mid ( 0, 2 );
        MonthStr = Source.Mid ( 2, 2 );
        YearStr = Source.Mid ( 4 );
        break;
      }
      case dfMDY: {
        MonthStr = Source.Mid ( 0, 2 );
        DayStr = Source.Mid ( 2, 2 );
        YearStr = Source.Mid ( 4 );
        break;
      }
      case dfYMD: {
        YearStr = Source.Mid ( 0, 2 );
        MonthStr = Source.Mid ( 2, 2 );
        DayStr = Source.Mid ( 4 );
        break;
      }
      case dfDMYY: {
        DayStr = Source.Mid ( 0, 2 );
        MonthStr = Source.Mid ( 2, 2 );
        YearStr = Source.Mid ( 4 );
        break;
      }
      case dfMDYY: {
        MonthStr = Source.Mid ( 0, 2 );
        DayStr = Source.Mid ( 2, 2 );
        YearStr = Source.Mid ( 4 );
        break;
      }
      case dfYYMD: {
        YearStr = Source.Mid ( 0, 4 );
        MonthStr = Source.Mid ( 4, 2 );
        DayStr = Source.Mid ( 6 );
        break;
      }
      default: return FALSE;
    }
  }


  int Year = atoi ( YearStr.GetBuffer(0) );
  int Month = atoi ( MonthStr.GetBuffer(0) );
  int Day = atoi ( DayStr.GetBuffer(0) );
  
  // handle two-digit years
  if ( ( m_Format == dfDMY || m_Format == dfMDY || m_Format == dfYMD ) && ( Year>= 0 && Year <= 99 ) ) {
    SYSTEMTIME Today;
    GetLocalTime ( &Today );

    int CurrentCentury = ( Today.wYear / 100 ) * 100;

    if ( Year <= 37 )
      Year += CurrentCentury;
    else 
      Year += ( CurrentCentury - 100 );
  }
  /*
  if ( ( m_Format == dfDMY || m_Format == dfMDY || m_Format == dfYMD ) && Year >= 1 && Year <= 99 )
  {
    CTime Test = GetCurrentTime();
    int CurrentCentury = ( Test.GetYear() / 100 ) * 100;

    if ( Year <= 37 )
      Year += CurrentCentury;
    else 
      Year += ( CurrentCentury - 100 );
  }
  */

  // check ranges
  if ( Year < 1970 || Year > 2038 )
    return FALSE;
  if ( Month < 1 || Month > 12 )
    return FALSE;
  if ( Day < 1 || Day > DaysPerMonth ( Year, Month ) )
    return FALSE;

  *((CTime*) Dest) = CTime ( Year, Month, Day, 0, 0, 0 );

  return TRUE;
}


BOOL CDateFieldConversion::LoadFromIniFile ( const CString &Filename, const CString& Sectionname )
{
  if ( !CFieldConversion::LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;
  
  m_Format = (CDateFieldConversion::DateFormat) GetPrivateProfileInt ( Sectionname, INI_KEYNAME_DATEFORMAT, dfDMYY, Filename );

  if ( !GetPrivateProfileString ( Sectionname, INI_KEYNAME_DATEDELIMITER, _T("."), &m_Delimiter, Filename ) )
    return FALSE;

  return TRUE;
}


BOOL CDateFieldConversion::SaveToIniFile ( const CString &Filename, const CString& Sectionname )
{
  if ( !CFieldConversion::SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;
  
  WritePrivateProfileInt ( Sectionname, INI_KEYNAME_DATEFORMAT, m_Format, Filename );

  WritePrivateProfileString ( Sectionname, INI_KEYNAME_DATEDELIMITER, m_Delimiter, Filename );

  return TRUE;
}


//////////////////////////////////////////////////////////////////////
// CTextFieldConversion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL ( CTextFieldConversion, CFieldConversion, CSVIMPORT_FILE_VERSION );


CTextFieldConversion::CTextFieldConversion()
{
  m_DataTypeName.LoadString ( IDS_DATATYPE_TEXT );
}


void CTextFieldConversion::Serialize ( CArchive& ar )
{
  CFieldConversion::Serialize ( ar );
}


void CTextFieldConversion::AssignFrom ( CFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
}


void CTextFieldConversion::Init()
{
  CFieldConversion::Init();
}


BOOL CTextFieldConversion::Exchange ( void* Dest )
{
  if ( !CFieldConversion::Exchange ( Dest ) )
    return FALSE;

  if ( GetSourceFieldID() == -1 )
    return TRUE;

  if ( !CheckSourceFieldID() )
    return FALSE;
  
  CString Source;
  if (GetSourceFieldID() > GetLine().GetSize() - 1)	// Konto-Festwert
	Source = GetFestwert();
  else
	Source = GetLine().GetAt ( GetSourceFieldID() );

  if (Source.GetLength())
  {
    Source.TrimLeft ( (TCHAR) _T(" ") );
	Source.TrimRight ( (TCHAR) _T(" ") );
  }
  *((CString*) Dest) = Source;

  return TRUE;
}


//////////////////////////////////////////////////////////////////////
// CCurrencyFieldConversion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL ( CCurrencyFieldConversion, CFieldConversion, CSVIMPORT_FILE_VERSION );


CCurrencyFieldConversion::CCurrencyFieldConversion() 
  : CFieldConversion()
{
  m_DataTypeName.LoadString ( IDS_DATATYPE_CURRENCY );
}


void CCurrencyFieldConversion::Serialize ( CArchive& ar )
{
  CFieldConversion::Serialize ( ar );
  if ( ar.IsStoring() )
  {
    ar.Write ( &m_Format, sizeof ( CurrencyFormat ) );
  }
  else
  {
    ar.Read ( &m_Format, sizeof ( CurrencyFormat ) );
  }
}


void CCurrencyFieldConversion::AssignFrom ( CFieldConversion* const  Source )
{
  CFieldConversion::AssignFrom ( Source );
}


void CCurrencyFieldConversion::AssignFrom ( CCurrencyFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
  SetFormat ( Source->GetFormat() );
}


void CCurrencyFieldConversion::Init()
{
  CFieldConversion::Init();
  m_Format = cfGerman;
}


BOOL CCurrencyFieldConversion::Exchange ( void* Dest )
{
  if ( !CFieldConversion::Exchange ( Dest ) )
    return FALSE;
  
  if ( GetSourceFieldID() == -1 )
    return TRUE;

  if ( !CheckSourceFieldID() )
    return FALSE;
  
  CString Source;
  if (GetSourceFieldID() > GetLine().GetSize() - 1) // MWSt-Satz Festwert
	Source = GetFestwert();
  else
	Source = GetLine().GetAt ( GetSourceFieldID() );
  int CommaPos = -1;
  
  Source.TrimLeft ( _T (" ") );
  Source.TrimRight ( _T (" ") );

  BOOL IsNegative = ( Source.Left ( 1 ) == _T("-") || Source.Right ( 1 ) == _T("-") );
  Source.TrimLeft ( _T ("-") );
  Source.TrimRight ( _T ("-") );

  /*
  if ( m_Format == cfGerman )
  {
    Source.Remove ( _T(".")[0] );
    CommaPos = Source.ReverseFind ( _T(",")[0] );
    if ( CommaPos != -1 ) {
      while ( Source.GetLength() - CommaPos < 2 ) 
        Source += _T("0");
      Source.Remove ( _T(",")[0] );
    }
  }

  if ( m_Format == cfEnglish )
  {
    Source.Remove ( _T(",")[0] );
    CommaPos = Source.ReverseFind ( _T(".")[0] );
    if ( CommaPos != -1 ) {
      while ( Source.GetLength() - CommaPos < 2 ) 
        Source += _T("0");
      Source.Remove ( _T(".")[0] );
    }
  }
  */

  switch ( m_Format )
  {
    case cfGerman:
    {
      Source.Remove ( _T(".")[0] );
      CommaPos = Source.ReverseFind ( _T(",")[0] );
      if ( CommaPos != -1 ) 
      {
        while ( Source.GetLength() - CommaPos <= 2 ) 
          Source += _T("0");
        Source.Remove ( _T(",")[0] );
      }
      break;
    }

    case cfEnglish:
    {
      Source.Remove ( _T(",")[0] );
      CommaPos = Source.ReverseFind ( _T(".")[0] );
      if ( CommaPos != -1 )
      {
        while ( Source.GetLength() - CommaPos <= 2 ) 
          Source += _T("0");
        Source.Remove ( _T(".")[0] );
      }
      break;
    }
          
    default:
      return FALSE;
  }
  
  Source.TrimRight ( _T("%") );
  Source.TrimLeft ( _T("0") );
  if ( Source.IsEmpty() )
    Source = _T("0");

  int Result = 0;

  if ( Source != _T("0") )
  {
    Result = _ttoi ( Source );
    if ( Result == 0 )
      return FALSE;
  }

  if ( IsNegative )
    Result = Result * ( -1 );

  if ( CommaPos == -1 ) 
    Result = Result * 100;

  *((int*) Dest) = Result;

  return TRUE;
}


BOOL CCurrencyFieldConversion::LoadFromIniFile ( const CString &Filename, const CString& Sectionname )
{
  if ( !CFieldConversion::LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;
  
  m_Format = (CCurrencyFieldConversion::CurrencyFormat) GetPrivateProfileInt ( Sectionname, INI_KEYNAME_CURRENCYFORMAT, cfGerman, Filename );

  return TRUE;
}


BOOL CCurrencyFieldConversion::SaveToIniFile ( const CString &Filename, const CString& Sectionname )
{
  if ( !CFieldConversion::SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;
  
  WritePrivateProfileInt ( Sectionname, INI_KEYNAME_CURRENCYFORMAT, m_Format, Filename );

  return TRUE;
}


//////////////////////////////////////////////////////////////////////
// CEAFieldConversion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL ( CEAFieldConversion, CFieldConversion, CSVIMPORT_FILE_VERSION );


CEAFieldConversion::CEAFieldConversion() 
  : CFieldConversion() 
{
  m_DataTypeName.LoadString ( IDS_DATATYPE_EA );
}


void CEAFieldConversion::AssignFrom ( CFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
}


void CEAFieldConversion::AssignFrom ( CEAFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
  SetFormatIncome ( Source->GetFormatIncome() );
  SetFormatExpense ( Source->GetFormatExpense() );
  SetConversionRule ( Source->GetConversionRule() );
}


void CEAFieldConversion::Init()
{
  CFieldConversion::Init();
  m_ConversionRule = cUseFormat;
  m_FormatIncome = _T("");
  m_FormatExpense = _T("");
}


void CEAFieldConversion::SetConversionRule ( const ConversionRule& Value )
{ 
  m_ConversionRule = Value;
  if ( m_ConversionRule != cUseFormat )
    SetSourceFieldID ( -1 );
}


void CEAFieldConversion::Serialize ( CArchive& ar )
{
  CFieldConversion::Serialize ( ar );
  if ( ar.IsStoring() )
  {
    ar << m_FormatIncome;
    ar << m_FormatExpense;
  }
  else
  {
    ar >> m_FormatIncome;
    ar >> m_FormatExpense;
  }
}


BOOL CEAFieldConversion::Exchange ( void* Dest )
{
  if ( !CFieldConversion::Exchange ( Dest ) )
    return FALSE;
  
  switch ( m_ConversionRule ) 
  {
    case cIncomeOnly:
      *((EinnahmeAusgabe*) Dest) = eaIncome;
      break;
    
    case cExpenseOnly:
      *((EinnahmeAusgabe*) Dest) = eaExpense;
      break;

    case cUseSign:
      *((EinnahmeAusgabe*) Dest) = eaUnknown;
      break;

    default:
      if ( GetSourceFieldID() == -1 )
      {
        *((EinnahmeAusgabe*) Dest) = eaIncome;
        return TRUE;
      }

      if ( !CheckSourceFieldID() )
        return FALSE;
  
      CString Source = GetLine().GetAt ( GetSourceFieldID() );

      Source.TrimLeft();
      Source.TrimRight();

      if ( GetFormatIncome().IsEmpty() && GetFormatExpense().IsEmpty() )
      {
        *((EinnahmeAusgabe*) Dest) = eaUnknown;
        return TRUE;
      }
      else 
        if ( GetFormatIncome().CompareNoCase ( Source ) == 0 )
        {
          *((EinnahmeAusgabe*) Dest) = eaIncome;
          return TRUE;
        }
        else
          if ( GetFormatExpense().CompareNoCase ( Source ) == 0 )
          {
            *((EinnahmeAusgabe*) Dest) = eaExpense;
            return TRUE;
          }
          else
            return FALSE;
  }

  return TRUE;
}


BOOL CEAFieldConversion::LoadFromIniFile ( const CString &Filename, const CString& Sectionname )
{
  if ( !CFieldConversion::LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;
  
  m_ConversionRule = (CEAFieldConversion::ConversionRule) GetPrivateProfileInt ( Sectionname, INI_KEYNAME_CONVERSIONRULE_INCOMEEXPENSE, cUseFormat, Filename );

  if ( !GetPrivateProfileString ( Sectionname, INI_KEYNAME_FORMATEINNAHME, _T(""), &m_FormatIncome, Filename ) )
    return FALSE;

  if ( !GetPrivateProfileString ( Sectionname, INI_KEYNAME_FORMATAUSGABE, _T(""), &m_FormatExpense, Filename ) )
    return FALSE;

  return TRUE;
}


BOOL CEAFieldConversion::SaveToIniFile ( const CString &Filename, const CString& Sectionname )
{
  if ( !CFieldConversion::SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;
  
  WritePrivateProfileInt ( Sectionname, INI_KEYNAME_CONVERSIONRULE_INCOMEEXPENSE, m_ConversionRule, Filename );

  WritePrivateProfileString ( Sectionname, INI_KEYNAME_FORMATEINNAHME, m_FormatIncome, Filename );

  WritePrivateProfileString ( Sectionname, INI_KEYNAME_FORMATAUSGABE, m_FormatExpense, Filename );

  return TRUE;
}



//////////////////////////////////////////////////////////////////////
// CEAFieldConversion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL ( CBruttoNettoFieldConversion, CFieldConversion, CSVIMPORT_FILE_VERSION );


CBruttoNettoFieldConversion::CBruttoNettoFieldConversion() 
  : CFieldConversion() 
{
  m_DataTypeName.LoadString ( IDS_DATATYPE_BRUTTONETTO );
}


void CBruttoNettoFieldConversion::AssignFrom ( CFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
}


void CBruttoNettoFieldConversion::AssignFrom ( CBruttoNettoFieldConversion* const Source )
{
  CFieldConversion::AssignFrom ( Source );
  SetFormatBrutto ( Source->GetFormatBrutto() );
  SetFormatNetto ( Source->GetFormatNetto() );
  SetConversionRule ( Source->GetConversionRule() );
}

#pragma warning( disable : 4482 )

void CBruttoNettoFieldConversion::Init()
{
  CFieldConversion::Init();
  m_ConversionRule = ConversionRule::cBruttoOnly;
  m_FormatBrutto = _T("");
  m_FormatNetto = _T("");
}


void CBruttoNettoFieldConversion::SetConversionRule ( const ConversionRule& Value )
{ 
  m_ConversionRule = Value;
  if ( m_ConversionRule != cUseFormat )
    SetSourceFieldID ( -1 );
}


void CBruttoNettoFieldConversion::Serialize ( CArchive& ar )
{
  CFieldConversion::Serialize ( ar );
  if ( ar.IsStoring() )
  {
    ar << m_FormatBrutto;
    ar << m_FormatNetto;
  }
  else
  {
    ar >> m_FormatBrutto;
    ar >> m_FormatNetto;
  }
}


BOOL CBruttoNettoFieldConversion::Exchange ( void* Dest )
{
  if ( !CFieldConversion::Exchange ( Dest ) )
    return FALSE;
  
  switch ( m_ConversionRule ) 
  {
    case cBruttoOnly:
      *((BruttoNetto*) Dest) = bnBrutto;
      break;
    
    case cNettoOnly:
      *((BruttoNetto*) Dest) = bnNetto;
      break;

    default: // ConversionRule::cUseFormat
      if ( GetSourceFieldID() == -1 )
      {
        *((BruttoNetto*) Dest) = bnBrutto;
        return TRUE;
      }

      if ( !CheckSourceFieldID() )
        return FALSE;
  
      CString Source = GetLine().GetAt ( GetSourceFieldID() );

      Source.TrimLeft();
      Source.TrimRight();

      if ( GetFormatBrutto().IsEmpty() && GetFormatNetto().IsEmpty() )
      {
        *((BruttoNetto*) Dest) = bnUnknown;
        return TRUE;
      }
      else 
        if ( GetFormatBrutto().CompareNoCase ( Source ) == 0 )
        {
          *((BruttoNetto*) Dest) = bnBrutto;
          return TRUE;
        }
        else
          if ( GetFormatNetto().CompareNoCase ( Source ) == 0 )
          {
            *((BruttoNetto*) Dest) = bnNetto;
            return TRUE;
          }
          else
            return FALSE;
  }

  return TRUE;
}


BOOL CBruttoNettoFieldConversion::LoadFromIniFile ( const CString &Filename, const CString& Sectionname )
{
  if ( !CFieldConversion::LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;
  
  m_ConversionRule = (CBruttoNettoFieldConversion::ConversionRule) GetPrivateProfileInt ( Sectionname, INI_KEYNAME_CONVERSIONRULE_BRUTTONETTO, cUseFormat, Filename );

  if ( !GetPrivateProfileString ( Sectionname, INI_KEYNAME_FORMATBRUTTO, _T(""), &m_FormatBrutto, Filename ) )
    return FALSE;

  if ( !GetPrivateProfileString ( Sectionname, INI_KEYNAME_FORMATNETTO, _T(""), &m_FormatNetto, Filename ) )
    return FALSE;

  return TRUE;
}


BOOL CBruttoNettoFieldConversion::SaveToIniFile ( const CString &Filename, const CString& Sectionname )
{
  if ( !CFieldConversion::SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;
  
  WritePrivateProfileInt ( Sectionname, INI_KEYNAME_CONVERSIONRULE_BRUTTONETTO, m_ConversionRule, Filename );

  WritePrivateProfileString ( Sectionname, INI_KEYNAME_FORMATBRUTTO, m_FormatBrutto, Filename );

  WritePrivateProfileString ( Sectionname, INI_KEYNAME_FORMATNETTO, m_FormatNetto, Filename );

  return TRUE;
}



//////////////////////////////////////////////////////////////////////
// CImportParams
//////////////////////////////////////////////////////////////////////

CImportParams* CImportParams::CreateStandard() /* static */
{
  CImportParams* Result = new CImportParams();
  Result->SetName ( CString ( MAKEINTRESOURCE ( IDS_STANDARD ) ) );
  return Result;
}


IMPLEMENT_SERIAL ( CImportParams, CObject, CSVIMPORT_FILE_VERSION );


CImportParams::CImportParams()
{
  m_FieldEA.SetName ( _T("EA") );
  m_FieldDatum.SetName ( _T("Datum") );
  m_FieldBetrag.SetName ( _T("Betrag") );
  m_FieldBeschreibung.SetName ( _T("Beschreibung") );
  m_FieldBelegnummer.SetName ( _T("Belegnummer") );
  m_FieldKonto.SetName ( _T("Konto") );
  m_FieldSteuersatz.SetName ( _T("Steuersatz") );
  m_FieldAfAAktJahr.SetName ( _T("AfAAktJahr") );
  m_FieldAfADauer.SetName ( _T("AfADauer") );
  m_FieldAfADegSatz.SetName ( _T("AfADegSatz") );
  m_FieldAfARestwert.SetName ( _T("AfARestwert") );
  m_FieldAfAGenauigkeit.SetName ( _T("AfAGenauigkeit") );
  m_FieldBetrieb.SetName ( _T("Betrieb") );
  m_FieldBestandskonto.SetName ( _T("Bestandskonto") );
  Init();
}


CImportParams::~CImportParams()
{
}


void CImportParams::Serialize ( CArchive& ar )
{
  CObject::Serialize ( ar );
  if ( ar.IsStoring() )
  {
    ar << m_Name;
    ar << m_Filename;
    ar << m_HeaderCount;
    ar << m_SeparatorChar;
  }
  else
  {
    ar >> m_Name;
    ar >> m_Filename;
    ar >> m_HeaderCount;
    ar >> m_SeparatorChar;
  }
  m_FieldEA.Serialize ( ar );
  m_FieldDatum.Serialize ( ar );
  m_FieldBetrag.Serialize ( ar );
  m_FieldBeschreibung.Serialize ( ar );
  m_FieldBelegnummer.Serialize ( ar );
  m_FieldKonto.Serialize ( ar );
  m_FieldSteuersatz.Serialize ( ar );
  m_FieldAfAAktJahr.Serialize ( ar );
  m_FieldAfADauer.Serialize ( ar );
  m_FieldAfADegSatz.Serialize ( ar );
  m_FieldAfARestwert.Serialize ( ar );
  m_FieldAfAGenauigkeit.Serialize ( ar );
  m_FieldBetrieb.Serialize ( ar );
  m_FieldBestandskonto.Serialize ( ar );
  if ( ar.IsStoring() )
  {
    ar << m_GawkScript;
  }
  else
  {
    ar >> m_GawkScript;
  }
 }


void CImportParams::Init()
{
  m_Name = _T("");
  m_SeparatorChar = _T(";");  
  m_Filename = _T("");
  m_GawkScript = _T("");
  m_HeaderCount = 0;

  m_FieldEA.Init();
  m_FieldDatum.Init();
  m_FieldBetrag.Init();
  m_FieldBruttoNetto.Init();
  m_FieldBeschreibung.Init();
  m_FieldBelegnummer.Init();
  m_FieldKonto.Init();
  m_FieldSteuersatz.Init();
  m_FieldAfAAktJahr.Init();
  m_FieldAfADauer.Init();
  m_FieldAfADegSatz.Init();
  m_FieldAfARestwert.Init();
  m_FieldAfAGenauigkeit.Init();
  m_FieldBetrieb.Init();
  m_FieldBestandskonto.Init();
}


BOOL CImportParams::Validate ( CString* ErrorMsg )
{
  // check filename
  if ( m_Filename.IsEmpty() )
  {
    if ( ErrorMsg )
      ErrorMsg->LoadString ( IDS_PARAMERR_NOFILENAME );
    return FALSE;
  }

  // check separator char
  if ( m_SeparatorChar.IsEmpty() )
  {
    if ( ErrorMsg )
      ErrorMsg->LoadString ( IDS_PARAMERR_NOFSEPARATORCHAR );
    return FALSE;
  }
  if ( m_SeparatorChar.GetLength() > 1 )
  {
    if ( ErrorMsg )
      ErrorMsg->LoadString ( IDS_PARAMETER_SEPARATORCHAR_TO_LONG );
    return FALSE;
  }

  /*
  // FieldEA must exists -- means must have an associated file column
  if ( m_FieldEA.GetSourceFieldID() == -1 ) {
    if ( ErrorMsg )
      ErrorMsg->LoadString ( IDS_PARAMERR_NOFIELDEA );
    return FALSE;
  }
  */

  // FieldBetrag must exists 
  if ( m_FieldBetrag.GetSourceFieldID() == -1 )
  {
    if ( ErrorMsg )
      ErrorMsg->LoadString ( IDS_PARAMERR_NOFIELDBETRAG );
    return FALSE;
  }
  
  return TRUE;
}


BOOL CImportParams::SetHeaderCount ( int Value ) { 
  if ( Value >= 0 )
  {
    m_HeaderCount = Value;
    return TRUE;
  }
  return FALSE;
}


CImportParams& CImportParams::operator = ( CImportParams& RightSideArgument )
{
  m_Name = RightSideArgument.m_Name;
  m_SeparatorChar = RightSideArgument.m_SeparatorChar;
  m_Filename = RightSideArgument.m_Filename;
  m_GawkScript = RightSideArgument.m_GawkScript;
  m_HeaderCount = RightSideArgument.m_HeaderCount;
  m_ColumnNames.Copy ( RightSideArgument.m_ColumnNames );
  m_FieldEA = RightSideArgument.m_FieldEA;
  m_FieldDatum = RightSideArgument.m_FieldDatum;
  m_FieldBetrag = RightSideArgument.m_FieldBetrag;
  m_FieldBruttoNetto = RightSideArgument.m_FieldBruttoNetto;
  m_FieldBeschreibung = RightSideArgument.m_FieldBeschreibung;
  m_FieldBelegnummer = RightSideArgument.m_FieldBelegnummer;
  m_FieldKonto = RightSideArgument.m_FieldKonto;
  m_FieldSteuersatz = RightSideArgument.m_FieldSteuersatz;
  m_FieldAfAAktJahr = RightSideArgument.m_FieldAfAAktJahr;
  m_FieldAfADauer = RightSideArgument.m_FieldAfADauer;
  m_FieldAfADegSatz = RightSideArgument.m_FieldAfADegSatz;
  m_FieldAfARestwert = RightSideArgument.m_FieldAfARestwert;
  m_FieldAfAGenauigkeit = RightSideArgument.m_FieldAfAGenauigkeit;
  m_FieldBetrieb = RightSideArgument.m_FieldBetrieb;
  m_FieldBestandskonto = RightSideArgument.m_FieldBestandskonto;
  return *this;  // Assignment operator returns left side.
}


BOOL CImportParams::LoadFromIniFile ( const CString &Filename, const CString& Sectionname )
{
  Init();
  
  if ( !GetPrivateProfileString ( Sectionname, INI_KEYNAME_NAME, m_Name, &m_Name, Filename ) )
    return FALSE;

  if ( !GetPrivateProfileString ( Sectionname, INI_KEYNAME_SEPARATIR, m_SeparatorChar, &m_SeparatorChar, Filename ) )
    return FALSE;
  
  if ( !GetPrivateProfileString ( Sectionname, INI_KEYNAME_FILENAME, m_Filename, &m_Filename, Filename ) )
    return FALSE;
  
  if ( !GetPrivateProfileString ( Sectionname, INI_KEYNAME_GAWKSCRIPT, m_GawkScript, &m_GawkScript, Filename ) )
    return FALSE;

  m_HeaderCount = GetPrivateProfileInt ( Sectionname, INI_KEYNAME_HEADERCOUNT, m_HeaderCount, Filename );

  int i = 0;
  CString KeyName, Value;
  while ( TRUE ) {
    KeyName.Format ("%s%d", INI_KEYNAME_COLUMNNAME, i );
    if ( !GetPrivateProfileString ( Sectionname, KeyName, _T("_***_"), &Value, Filename ) )
      return FALSE;

    if ( Value == _T("_***_") )
      break;

    m_ColumnNames.Add ( Value );

    KeyName.Format ("%s%d", INI_KEYNAME_COLUMNWIDTH, i );
    SetColumnWidth ( i, GetPrivateProfileInt ( Sectionname, KeyName, 0, Filename ) );

     i++;
  }

  if ( !m_FieldEA.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldDatum.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldBetrag.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldBruttoNetto.LoadFromIniFile( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldBeschreibung.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;
  
  if ( !m_FieldBelegnummer.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldKonto.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldSteuersatz.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldAfAAktJahr.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldAfADauer.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldAfADegSatz.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldAfARestwert.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldAfAGenauigkeit.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldBetrieb.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldBestandskonto.LoadFromIniFile ( Filename, Sectionname ) )
    return FALSE;

  return TRUE;
}


BOOL CImportParams::SaveToIniFile ( const CString &Filename, const CString& Sectionname )
{
  WritePrivateProfileString ( Sectionname, INI_KEYNAME_NAME, m_Name, Filename );

  WritePrivateProfileString ( Sectionname, INI_KEYNAME_SEPARATIR, m_SeparatorChar, Filename );
  
  WritePrivateProfileString ( Sectionname, INI_KEYNAME_FILENAME, m_Filename, Filename );

  WritePrivateProfileString ( Sectionname, INI_KEYNAME_GAWKSCRIPT, m_GawkScript, Filename );
  
  WritePrivateProfileInt ( Sectionname, INI_KEYNAME_HEADERCOUNT, m_HeaderCount, Filename );
  
  int i;
  CString KeyName;
  for ( i = 0; i <= m_ColumnNames.GetSize() - 1; i++ )
  {
    KeyName.Format ("%s%d", INI_KEYNAME_COLUMNNAME, i );
    WritePrivateProfileString ( Sectionname, KeyName, m_ColumnNames.GetAt ( i ), Filename );

	if (GetColumnWidth( i ))
	{
		KeyName.Format ("%s%d", INI_KEYNAME_COLUMNWIDTH, i );
		WritePrivateProfileInt ( Sectionname, KeyName, GetColumnWidth( i ), Filename );	
	}
  }

  if ( !m_FieldEA.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldDatum.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldBetrag.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldBruttoNetto.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldBeschreibung.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldBelegnummer.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldKonto.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldSteuersatz.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldAfAAktJahr.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldAfADauer.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldAfADegSatz.SaveToIniFile ( Filename, Sectionname ) )
    return FALSE;

  if ( !m_FieldAfARestwert.SaveToIniFile ( Filename, Sectionname ) ) 
    return FALSE;

  if ( !m_FieldAfAGenauigkeit.SaveToIniFile ( Filename, Sectionname ) ) 
    return FALSE;

  if ( !m_FieldBetrieb.SaveToIniFile ( Filename, Sectionname ) ) 
    return FALSE;

  if ( !m_FieldBestandskonto.SaveToIniFile ( Filename, Sectionname ) ) 
    return FALSE;

  return TRUE;
}


//////////////////////////////////////////////////////////////////////
// CImportParamsList
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL ( CImportParamsList, CObArray, CSVIMPORT_FILE_VERSION )


CImportParamsList::~CImportParamsList()
{
  RemoveAll();
}

void CImportParamsList::Serialize ( CArchive& ar )
{
  CObArray::Serialize ( ar );
}


BOOL CImportParamsList::SetDefaultImportParams ( CImportParams* const Value ) 
{ 
  if ( Value != NULL && !Contains( Value ) )
    return FALSE;

  m_DefaultImportParams = Value;
  return TRUE;
}

void CImportParamsList::RemoveAt ( int nIndex, int nCount )
{
  // free mem used by the items
  FreeObjects ( nIndex, nCount );

  // call "inherited" 
  CObArray::RemoveAt ( nIndex, nCount );
}


void CImportParamsList::RemoveAll()
{
  // free mem used by the items
  FreeObjects ( 0, GetSize() );

  // call "inherited" 
  CObArray::RemoveAll();
}


BOOL CImportParamsList::Contains ( CImportParams* Element ) const
{
  int i;
  for ( i = 0; i <= GetSize() - 1; i++ )
  {
    if ( Element == GetAt( i ) )
      return TRUE;
  }
  return FALSE;
}
 
int CImportParamsList::FindByName ( const CString& AName ) const 
{
  int i;
  for ( i = 0; i <= GetSize() - 1; i++ )
  {
    if ( ( (CImportParams*) GetAt ( i ) )->GetName().CompareNoCase ( AName ) == 0 )
      return i;
  }
  return -1;
}


void CImportParamsList::FreeObjects ( int StartIndex, int Count )
{
  int i = StartIndex;
  while ( i < StartIndex + Count )
  {
    if ( GetAt ( i ) ) 
    {
      // if the default import params entry is being freed, set the default import params pointer to null
      if ( GetAt( i ) == m_DefaultImportParams )
        m_DefaultImportParams = NULL;
      delete ( GetAt ( i ) );
    }
    i++;
  }
}


void CImportParamsList::BuildSectionname ( CString* Sectionname, const int Number )
{
  if ( Sectionname != NULL )  
    Sectionname->Format ( _T("%s.%d"), SEC_CSVIMPORT, Number );
}


BOOL CImportParamsList::LoadFromIniFile ()
{
  CString Filename;

  if ( !GetIniFileName ( Filename.GetBuffer ( MAX_PATH ), MAX_PATH ) )
    return FALSE;
  Filename.ReleaseBuffer();

  if ( Filename.IsEmpty() )
    return FALSE;

  return LoadFromIniFile ( Filename );
}


BOOL CImportParamsList::LoadFromIniFile ( const CString &Filename )
{
  // declare variables
  int i;
  CString Sectionname;
  CString DefaultImportParamsName;
  i = 0;
  
  while ( TRUE ) {
    BuildSectionname ( &Sectionname, i );
    
    if ( !ExistsPrivateProfileSection ( Sectionname, Filename ) )
      break;

    CImportParams* NewEntry = new CImportParams();
    Add ( NewEntry );

    if ( !NewEntry->LoadFromIniFile ( Filename, Sectionname ) )
      return FALSE;
    i++;
  }

  //
  m_DefaultImportParams = NULL;
  if ( !GetPrivateProfileString ( SEC_CSVIMPORT, KEY_DEFAULT_IMPORT_PARAMS, _T(""), &DefaultImportParamsName, Filename ) )
    return FALSE;
  if ( !DefaultImportParamsName.IsEmpty() )
  {
    int defaultImportParamsIndex = FindByName( DefaultImportParamsName );
    if ( defaultImportParamsIndex != -1 )
      m_DefaultImportParams = (CImportParams*) GetAt( defaultImportParamsIndex );
  }

  return TRUE;
}



BOOL CImportParamsList::SaveToIniFile ()
{
  CString Filename;

  if ( !GetIniFileName ( Filename.GetBuffer ( MAX_PATH ), MAX_PATH ) )
    return FALSE;
  Filename.ReleaseBuffer();

  if ( Filename.IsEmpty() )
    return FALSE;

  return SaveToIniFile ( Filename );
}


BOOL CImportParamsList::SaveToIniFile ( const CString &Filename )
{
  // declare variables
  int i;
  CString Sectionname;

  // delete previously saved import descriptions
  i = 0;
  while ( TRUE )
  {
    // build section name
    BuildSectionname ( &Sectionname, i );
    
    // stop processing if section doesn't exist
    if ( !ExistsPrivateProfileSection ( Sectionname, Filename ) )
      break;

    // delete section
    if ( !WritePrivateProfileString ( Sectionname, NULL, NULL, Filename ) && GetLastError != ERROR_SUCCESS )
      return FALSE;

    i++;
  }

  // store settings for each import description
  for ( i = 0; i <= GetSize() - 1; i++ )
  {
    // build section name
    BuildSectionname ( &Sectionname, i );

    // save a single import description
    if ( !( (CImportParams*) GetAt ( i ) )->SaveToIniFile ( Filename, Sectionname ) )
      return FALSE;
  }

  // store default import description (normally the last one that was selected)
  if ( m_DefaultImportParams == NULL )
  {
    if ( !WritePrivateProfileString( SEC_CSVIMPORT, KEY_DEFAULT_IMPORT_PARAMS, _T(""), Filename ) )
      return FALSE;
  }
  else
  {
    if ( !WritePrivateProfileString( SEC_CSVIMPORT, KEY_DEFAULT_IMPORT_PARAMS, m_DefaultImportParams->GetName(), Filename ) )
      return FALSE;
  }

  return TRUE;
}

