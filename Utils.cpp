/*//////////////////////////////////////////////////////////////////////////////
// Name:        utils.cpp
// Purpose:     utility routines
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/


#include "stdafx.h"
#include "EinstellungCtrl.h"
#include "utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// -----------------------------------------------------------------------------
// Statischer Zeiger auf das vom Dialog gehostete CEinstellung-Control.
// CDlgImportDescr::OnInitDialog setzt ihn, CDlgImportDescr::OnDestroy räumt
// ihn wieder ab.
// -----------------------------------------------------------------------------
static CEinstellung* s_pEinstellungCtrl = NULL;


void ECT_SetEinstellungCtrl ( CEinstellung* pCtrl )
{
  s_pEinstellungCtrl = pCtrl;
}


CString ECT_HoleEinstellung ( LPCTSTR Key )
{
  if ( !s_pEinstellungCtrl || !s_pEinstellungCtrl->GetSafeHwnd() || !Key )
    return CString();
  return s_pEinstellungCtrl->HoleEinstellung ( Key );
}


void ECT_SpeichereEinstellung ( LPCTSTR Key, LPCTSTR Wert )
{
  if ( !s_pEinstellungCtrl || !s_pEinstellungCtrl->GetSafeHwnd() || !Key )
    return;
  s_pEinstellungCtrl->SpeichereEinstellung ( Key, Wert ? Wert : _T("") );
}


int ECT_HoleEinstellungInt ( LPCTSTR Key, int DefaultValue )
{
  CString s = ECT_HoleEinstellung ( Key );
  if ( s.IsEmpty() )
    return DefaultValue;
  return _ttoi ( s );
}


void ECT_SpeichereEinstellungInt ( LPCTSTR Key, int Value )
{
  CString s;
  s.Format ( _T("%d"), Value );
  ECT_SpeichereEinstellung ( Key, s );
}


// -----------------------------------------------------------------------------
// Datums-Helfer
// -----------------------------------------------------------------------------

short DayTable[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


BOOL IsLeapYear ( const int Year )
{
  return Year % 4 == 0 && ( Year % 100 != 0 || Year % 400 == 0 ) ? TRUE : FALSE;
}


int DaysPerMonth ( const int Year, const int Month )
{
  if ( Month == 2 && IsLeapYear ( Year ) )
    return 29;
  else 
    return DayTable[Month - 1];
}