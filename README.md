# ECTImport Project Overview

### Compiling ECTImport Plugin ###

Der Code compiliert momentan unter Visual Studio 2013 -- benutze einfach die freie Community Edition

Zum Debuggen ist es empfohlen eine Version von EC&T außerhalb von "C:\Programme (x86)" 
zu installieren und den Linker-Output-Pfad entsprechend zu setzen. 
Das erspart das ständige Kopieren ins Plugin-Verzeichnis mit Administratorrechten.

"Error MSB8011: Failed to register output. Please try enabling Per-user Redirection or register 
the component from a command prompt with elevated permissions." ist eine normale Ausgabe.
Ggf. mit RegSrv32 und Admin-Privilegien manuell im CSV-Import-Plugin registrieren.

### Nutzung ###

Im Menü Start findet sich im Feld "Ansicht" die "Plugin"-Schaltfläche. Durch den Pfeil unter dem Puzzelteil lässt sich das Plugin starten.

#### Bei Fehlermeldung ####

Wenn eine Fehlermeldung erscheint, hilft es oft, die Pluginverwaltung zu starten. Dazu einfach auf "Plugin" zu klicken. Dann wird angezeigt, ob Updates verfügbar sind. Ist dies der Fall, sollten alle Updates installiert werden. Danach ist die Fehlermeldung üblicherweise behoben.

### ToDo ###

Benötigt ECTIFace im Plugin-Verzeichnis, damit das Registrieren funktioniert.
Das könnte mal geändert werden.

Platzhalter für Zeitangaben im Dateinamen einführen.

### Changelog ###

####v5.3 und v5.4: 
- versucht, das Problem abgeschnittener Anfangsbuchstaben bei Beschreibungstexten oder Kontennamen zu lösen

####v5.2: 
- XP-Kompatibilität wiederhergestellt
- gawk.exe Aufruf erfolgt nun explizit im DOS 8.3 Dateinamensformat, um Leerzeichen nich escapen zu müssen

####v5.0: 
 - auf Visual Studio 2013 umgestellt -- umgeht hoffentlich das 'P'-Problem; Umzug auf GitHub 

####v4.3:
 - GAWK Static 'optional' hinzugefügt
 - kleine Änderungen zum Debuggen des seltsamen P-Problems (Bugtracker #0000989 )
 
####v4.2: 
 - Es gab bei der Berechnung des Brutto-Betrags aus importierten Netto-Beträgen > 11302,54€ einen 32-bit-Überlauf, 
   der dazu führte, dass ein zu niedriger Betrag importiert wurde. Die Berechnung ist jetzt auf 64 bit umgestellt. 
 - Destroy und delete getrennt
 - Skript-Funktion für den Installer programmiert, der prüft, ob der Task noch läuft und ggf. um Beendigung bittet.
 
####v4.1:
 - GAWK geupdatet, Nachkommaanteil beim Import von MWSt-Sätzen wird jetzt nicht mehr abgehackt
 - gawk 4.0.0 hinzugefügt

####v4.0: 
 - GAWK-Skripting Unterstützung und F1-Hilfe auf HTML umgestellt

####v3.9: 
 - Bugfix, beim Restwert wurde nur der Nachkommaanteil importiert.

####v3.8:
 - Als Release funktioniert das geupdatete Import-Plugin. Aber erst nach einem IS-Setup Lauf.
 - Ausgaben-Konten werden jetzt als Festwert aufgelistet bei 'gem.Vorzeichen'.
 - AbschreibungGenauigkeit, Betrieb und Bestandskonto berücksichtigt. Gibt heap corruption.
 - Die Felder machen ein Problem mit dem EC&T-ActiveX-IDs. Muss ich eine neue Version des Controls bauen?
 
####v3.6: 
 - Toleranterer Code zum Parsen des Datums (evtl. angehängte Uhrzeit wird jetzt ignoriert)
 - Bugfix: Datum wurde manchmal auf einen Tag früher gesetzt beim Importieren.
 - ToDo aktualisiert: "Leerzeilen in den Combo-Boxen, wo eine nicht-Eingabe sinnvoll ist, ist eine gute Idee!"
 
####v3.5: 
 - Bug beseitigt, der dazu führte, dass die Spaltengrößen nicht gespeichert wurden.

####v3.4: 
 - Bug beseitigt, der dazu führte, dass die Festwerte nicht übernommen wurden.

####v3.3: 
 - Bug beseitigt, der dazu führte, dass die Festwerte bei Konto und Steuersatz nicht mehr übernommen wurden.

####v3.2: 
- Liste wird bei Änderung der Fenstergröße jetzt nicht mehr komplett neu aufgebaut (was zu einem erheblich schnelleren Bildaufbau führt)
- Spaltenbreiten werden jetzt in der Importbeschreibung gespeichert
- Fehlermeldung 'Invalid Parameter' unterdrückt

####v1.2.0.3 - 28.03.2004

Erweiterungen:
- Import von Dateien deren Felder durch Tabs getrennt sind ist jetzt möglich.

Fehler behoben:
- Bei der Unterscheidung von Einnahmen und Ausgaben "Entsprechend Vorzeichen"
  wird bei Ausgaben das Vorzeichen beibehalten wenn die Buchung erstellt wird.
  Das führt - logischerweise - zu "negativen" Ausgaben die wiederum wie eine
  Einnahme zu Buche schlagen.
  Fehler korrigert,

####v1.2.0.2 - 26.03.2004

Fehler behoben:
- Allg. Schutzverletzung beim Öffnen des Import-Dialoges korrigiert.

####v1.2.0.2 - 26.03.2004

Fehler behoben:
- Allg. Schutzverletzung beim Öffnen des Import-Dialoges korrigiert.

####v1.2.0.1 - 04.03.2004

Fehler behoben:
- AfA Restwert kann nun importiert werden. Wenn nicht angegeben wird
  er berechnet.

####v1.2.0.0 - 24.02.2004

Erweiterungen:
- Die zuletzt verwendete (in ComboBox Importbeschreibung ausgewählte) 
  Importbeschreibung wird beim Öffnen des Import-Dialoges ausgewählt.
- Belegnummer und Deg. AfA-Satz können nun importiert werden.
- Brutto- und/oder Nettobeträge können importiert werden.
- Hilfe vervollständigt.

Fehler behoben:
- Speicherleck gestopft

####v1.0.2.0 - 23.05.2001

Erweiterungen:

- Abschreibung (Dauer und akt. Jahr) können nun importiert werden.
- Kontext-Hilfe vervollständigt.

####v1.0.3.0 - 30.05.2001

Erweiterungen:
- Fortschritt-Anzeige bei längeren Import-Vorgängen (geschätzte 
  Zeit > 4 Sekunden). Wird in eigenem Fenster Angezeigt, Möglichkeit
  zum Abbruch der Aktion.
  
####v1.0.3.0 - 13.06.2001

Fehler behoben:
- Bei der Konvertierung von Währungsbeträgen bestand ein Fehler:
  so wurde z. B. aus "122,4" 12,2.
  Fehler behoben.

####v1.0.3.0 - 14.06.2001

Fehler behoben:
- Fehler bei der Konvertierung von Datumsangaben. Dati mit 
  zweistelliger Jahreszahl wurden nicht konvertiert.
  Grund dafür war die falsche Ermittlung des aktuellen Jahrhunderts.
  Fehler behoben.
