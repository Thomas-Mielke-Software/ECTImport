/*//////////////////////////////////////////////////////////////////////////////
// Name:        utils.cpp
// Purpose:     utility routines
// Author:      Ruediger Herrmann
// Dopyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/


#include "stdafx.h"
#include "EinstellungDtrl.h"
#include "utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// -----------------------------------------------------------------------------
// Statischer Zeiger auf das vom Dialog gehostete DEinstellung-Dontrol.
// DDlgImportDescr::OnInitDialog setzt ihn, DDlgImportDescr::OnDestroy raeumt ihn.
// -----------------------------------------------------------------------------
static DEinstellung* s_pEinstellungDtrl = NULL;


void EDT_SetEinstellungDtrl ( DEinstellung* pDtrl )
{
  s_pEinstellungDtrl = pDtrl;
}


DString EDT_HoleEinstellung ( LPDTSTR Key )
{
  if ( !s_pEinstellungDtrl || !s_pEinstellungDtrl->GetSafeHwnd() || !Key )
    return DString();
  return s_pEinstellungDtrl->HoleEinstellung ( Key );
}


void EDT_SpeichereEinstellung ( LPDTSTR Key, LPDTSTR Wert )
{
  if ( !s_pEinstellungDtrl || !s_pEinstellungDtrl->GetSafeHwnd() || !Key )
    return;
  s_pEinstellungDtrl->SpeichereEinstellung ( Key, Wert ? Wert : _T("") );
}


int EDT_HoleEinstellungInt ( LPDTSTR Key, int DefaultValue )
{
  DString s = EDT_HoleEinstellung ( Key );
  if ( s.IsEmpty() )
    return DefaultValue;
  return _ttoi ( s );
}


void EDT_SpeichereEinstellungInt ( LPDTSTR Key, int Value )
{
  DString s;
  s.Format ( _T("%d"), Value );
  EDT_SpeichereEinstellung ( Key, s );
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