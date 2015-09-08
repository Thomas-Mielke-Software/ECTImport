/*//////////////////////////////////////////////////////////////////////////////
// Name:        utils.h
// Purpose:     utility routines
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/


#include "stdafx.h"
#include "utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// helper function to save int values to an ini file
BOOL WritePrivateProfileInt ( LPCTSTR AppName, LPCTSTR KeyName, INT Value, LPCTSTR FileName ) {
  // convert int to string
  CString ValueStr;
  _itot ( Value, ValueStr.GetBuffer ( 20 ), 10 );
  ValueStr.ReleaseBuffer();

  // write string to ini file
  if ( WritePrivateProfileString ( AppName, KeyName, ValueStr, FileName ) && GetLastError() == ERROR_SUCCESS )
    return TRUE;
  else
    return FALSE;
}

 
// helper function to load values into a CString 
BOOL GetPrivateProfileString ( LPCTSTR AppName, LPCTSTR KeyName, LPCSTR Default, CString* Value, LPCTSTR FileName ) {
  if ( !Value )
    return FALSE;

  GetPrivateProfileString ( AppName, KeyName, Default, Value->GetBuffer ( 200 ), 200, FileName );
  Value->ReleaseBuffer();

  return TRUE;
}


// helper function to determine wether a given section-name exists
BOOL ExistsPrivateProfileSection ( LPCTSTR AppName, LPCTSTR FileName )
{
 CString Result;
 DWORD ResultSize;

 ResultSize = GetPrivateProfileSection ( AppName, Result.GetBuffer ( 10 ), 10, FileName );
 Result.ReleaseBuffer();

 return ( ResultSize > 0 ? TRUE : FALSE );
}



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

