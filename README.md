# ECTImport Project Overview

### Compiling ECTImport Plugin ###

Der Code compiliert momentan unter Visual Studion 2013 -- benutze einfach die freie Community Edition

Zum Debuggen ist es empfohlen eine Version von EC&T au�erhalb von "C:\Programme (x86)" 
zu installieren und den Linker-Output-Pfad entsprechend zu setzen. 
Das erspart das st�ndige Kopieren ins Plugin-Verzeichnis mit Administratorrechten.

"Error MSB8011: Failed to register output. Please try enabling Per-user Redirection or register 
the component from a command prompt with elevated permissions." ist eine normale Ausgabe.
Ggf. mit RegSrv32 und Admin-Privilegien manuell im CSV-Import-Plugin registrieren.

### ToDo ###

Ben�tigt ECTIFace im Plugin-Verzeichnis, damit das Registrieren funktioniert.
Das k�nnte mal ge�ndert werden.

Platzhalter f�r Zeitangaben im Dateinamen einf�hren.

### Changelog ###

####v5.0: auf Visual Studio 2013 umgestellt -- umgeht hoffentlich das 'P'-Problem; Umzug auf GitHub 

####v4.3:
 - GAWK Static 'optional' hinzugef�gt
 - kleine �nderungen zum Debuggen des seltsamen P-Problems (Bugtracker #0000989 )
 
####v4.2: 
 - Es gab bei der Berechnung des Brutto-Betrags aus importierten Netto-Betr�gen > 11302,54� einen 32-bit-�berlauf, 
   der dazu f�hrte, dass ein zu niedriger Betrag importiert wurde. Die Berechnung ist jetzt auf 64 bit umgestellt. 
 - Destroy und delete getrennt
 - Skript-Funktion f�r den Installer programmiert, der pr�ft, ob der Task noch l�uft und ggf. um Beendigung bittet.
 
####v4.1: GAWK geupdatet, Nachkommaanteil beim Import von MWSt-S�tzen wird jetzt nicht mehr abgehackt; gawk 4.0.0 hinzugef�gt

####v4.0: GAWK-Skripting Unterst�tzung und F1-Hilfe auf HTML umgestellt

####v3.9: Bugfix, beim Restwert wurde nur der Nachkommaanteil importiert.

####v3.8:
 - Als Release funktioniert das geupdatete Import-Plugin. Aber erst nach einem IS-Setup Lauf.
 - Ausgaben-Konten werden jetzt als Festwert aufgelistet bei 'gem.Vorzeichen'.
 - AbschreibungGenauigkeit, Betrieb und Bestandskonto ber�cksichtigt. Gibt heap corruption.
 - Die Felder machen ein Problem mit dem EC&T-ActiveX-IDs. Muss ich eine neue Version des Controls bauen?
 
####v3.6: 
 - Toleranterer Code zum Parsen des Datums (evtl. angeh�ngte Uhrzeit wird jetzt ignoriert)
 - Bugfix: Datum wurde manchmal auf einen Tag fr�her gesetzt beim Importieren.
 - ToDo aktualisiert: "Leerzeilen in den Combo-Boxen, wo eine nicht-Eingabe sinnvoll ist, ist eine gute Idee!"
 
####v3.5: Bug beseitigt, der dazu f�hrte, dass die Spaltengr��en nicht gespeichert wurden.

####v3.4: Bug beseitigt, der dazu f�hrte, dass die Festwerte nicht �bernommen wurden.

####v3.3: Bug beseitigt, der dazu f�hrte, dass die Festwerte bei Konto und Steuersatz nicht mehr �bernommen wurden.

####v3.2: 
- Liste wird bei �nderung der Fenstergr��e jetzt nicht mehr komplett neu aufgebaut (was zu einem erheblich schnelleren Bildaufbau f�hrt)
- Spaltenbreiten werden jetzt in der Importbeschreibung gespeichert
- Fehlermeldung 'Invalid Parameter' unterdr�ckt

####v1.2.0.3 - 28.03.2004

Erweiterungen:
- Import von Dateien deren Felder durch Tabs getrennt sind ist jetzt m�glich.

Fehler behoben:
- Bei der Unterscheidung von Einnahmen und Ausgaben "Entsprechend Vorzeichen"
  wird bei Ausgaben das Vorzeichen beibehalten wenn die Buchung erstellt wird.
  Das f�hrt - logischerweise - zu "negativen" Ausgaben die wiederum wie eine
  Einnahme zu Buche schlagen.
  Fehler korrigert,

####v1.2.0.2 - 26.03.2004

Fehler behoben:
- Allg. Schutzverletzung beim �ffnen des Import-Dialoges korrigiert.

####v1.2.0.2 - 26.03.2004

Fehler behoben:
- Allg. Schutzverletzung beim �ffnen des Import-Dialoges korrigiert.

####v1.2.0.1 - 04.03.2004

Fehler behoben:
- AfA Restwert kann nun importiert werden. Wenn nicht angegeben wird
  er berechnet.

####v1.2.0.0 - 24.02.2004

Erweiterungen:
- Die zuletzt verwendete (in ComboBox Importbeschreibung ausgew�hlte) 
  Importbeschreibung wird beim �ffnen des Import-Dialoges ausgew�hlt.
- Belegnummer und Deg. AfA-Satz k�nnen nun importiert werden.
- Brutto- und/oder Nettobetr�ge k�nnen importiert werden.
- Hilfe vervollst�ndigt.

Fehler behoben:
- Speicherleck gestopft

####v1.0.2.0 - 23.05.2001

Erweiterungen:

- Abschreibung (Dauer und akt. Jahr) k�nnen nun importiert werden.
- Kontext-Hilfe vervollst�ndigt.

####v1.0.3.0 - 30.05.2001

Erweiterungen:
- Fortschritt-Anzeige bei l�ngeren Import-Vorg�ngen (gesch�tzte 
  Zeit > 4 Sekunden). Wird in eigenem Fenster Angezeigt, M�glichkeit
  zum Abbruch der Aktion.
  
####v1.0.3.0 - 13.06.2001

Fehler behoben:
- Bei der Konvertierung von W�hrungsbetr�gen bestand ein Fehler:
  so wurde z. B. aus "122,4" 12,2.
  Fehler behoben.

####v1.0.3.0 - 14.06.2001

Fehler behoben:
- Fehler bei der Konvertierung von Datumsangaben. Dati mit 
  zweistelliger Jahreszahl wurden nicht konvertiert.
  Grund daf�r war die falsche Ermittlung des aktuellen Jahrhunderts.
  Fehler behoben.