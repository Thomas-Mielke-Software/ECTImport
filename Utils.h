/*//////////////////////////////////////////////////////////////////////////////
// Name:        utils.h
// Purpose:     utility routines
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined ( _UTILS_H_ )
#define _UTILS_H_

BOOL WritePrivateProfileInt ( LPCTSTR AppName, LPCTSTR KeyName, INT Value, LPCTSTR FileName );
BOOL GetPrivateProfileString ( LPCTSTR AppName, LPCTSTR KeyName, LPCSTR Default, CString* Value, LPCTSTR FileName );
BOOL ExistsPrivateProfileSection ( LPCTSTR AppName, LPCTSTR FileName );


BOOL IsLeapYear ( const int Year );
int DaysPerMonth ( const int Year, const int Month );


#endif /* _UTILS_H_ */
