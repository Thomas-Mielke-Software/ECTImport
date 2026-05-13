/*//////////////////////////////////////////////////////////////////////////////
// Name:        utils.h
// Purpose:     utility routines
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined ( _UTILS_H_ )
#define _UTILS_H_

class CEinstellung;  // forward declaration (siehe EinstellungCtrl.h)

// -----------------------------------------------------------------------------
// Einstellungs-Zugriff via ActiveX-Schnittstelle (CEinstellung-OCX)
//
// Designprinzip:
//   Das OCX kann nicht per CoCreateInstance standalone benutzt werden, weil es
//   mit OLEMISC_SETCLIENTSITEFIRST registriert ist und CWnd-Hosting erwartet.
//   Der Plugin-Dialog (CDlgImportDescr) hostet bereits ein m_EinstellungCtrl
//   via DDX_Control - das benutzen wir.
//
// Lebenszyklus:
//   - CDlgImportDescr::OnInitDialog ruft ECT_SetEinstellungCtrl(&m_EinstellungCtrl)
//     nachdem die DDX-Subklassierung gelaufen ist
//   - CDlgImportDescr::OnDestroy ruft ECT_SetEinstellungCtrl(NULL)
//   - Solange das gesetzt ist, können ECT_HoleEinstellung/ECT_SpeichereEinstellung
//     überall im Plugin (auch in ImportParams.cpp) verwendet werden
//
// Schlüsselformat wie beim OCX: "Key" (Kurzform) oder "[Sektion]Key".
// -----------------------------------------------------------------------------
void    ECT_SetEinstellungCtrl ( CEinstellung* pCtrl );

CString ECT_HoleEinstellung    ( LPCTSTR Key );
void    ECT_SpeichereEinstellung ( LPCTSTR Key, LPCTSTR Wert );
int     ECT_HoleEinstellungInt ( LPCTSTR Key, int DefaultValue );
void    ECT_SpeichereEinstellungInt ( LPCTSTR Key, int Value );


BOOL IsLeapYear ( const int Year );
int DaysPerMonth ( const int Year, const int Month );


#endif /* _UTILS_H_ */