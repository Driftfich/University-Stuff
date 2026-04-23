# Bibliotheksverwaltungssystem - Deutsche Übersetzung (Google Translate)

## Autor
Franz Rehschuh (Matr.-Nr.: MATRICULATION_NUMBER) (S-Nr.: s88216)

## Offenlegung
Für die UI-Komponenten dieses Projekts, insbesondere für die Teile, die ein tiefes Verständnis des Qt-Frameworks, seiner verschiedenen Klassen und Methoden erforderten, habe ich KI-Tools und -Ressourcen genutzt.
Beispiele für den Einsatz von KI-Tools:
– Erstellen der Datei /ui/material.qss, um einen ähnlichen UI-Stil wie Android Material You zu erstellen
– Überprüfen meiner Design-/Architekturentscheidungen für InfoPanel, CustomFilterProxModel oder die Mainw-Klasse

Qt Creator habe ich hingegen nicht verwendet, um die UI-Elemente zu erstellen oder Verbindungen zwischen UI-Elementen herzustellen.
Ich habe Qt Designer kurzzeitig genutzt, um Dummy-Layouts zu erstellen und so ein Gefühl für die Gestaltung der UI zu bekommen.

Darüber hinaus möchte ich klarstellen, dass ich für die Kernimplementierungen des zugrunde liegenden Datenmodells des Projekts keine KI-Tools verwendet habe. Dies umfasst Dateien in den Verzeichnissen /core und /manager sowie Tabellenmodelldateien im Verzeichnis /tablemodel.

## Zusammenfassung
Dieses Projekt ist ein Bibliotheksverwaltungssystem, das in C++ mit dem Qt-Framework für die GUI geschrieben wurde. Es ermöglicht Nutzern die Verwaltung einer Sammlung verschiedener Medientypen in Text-, Audio- und Videoformaten. Zusätzlich können Personen verwaltet werden, die Entleiher und/oder Künstler eines Bibliothekselements sein können. Abschließend können Transaktionen erstellt werden, die ein Bibliothekselement einer Person zuordnen. Das Programm unterstützt grundlegende Operationen wie das Hinzufügen, Löschen, Suchen und Sortieren von Elementen, Personen und Transaktionen.

Einige architektonische Entscheidungen, die im gesamten Projekt umgesetzt wurden:
- Verschiedene Entitäten referenzieren sich gegenseitig über ihre eindeutige ID (unsigned long), um redundante Daten zu vermeiden, Beziehungen sichtbar zu machen und einfache Änderungen zu ermöglichen. Obwohl ein DBMS für diese Aufgabe besser geeignet wäre, habe ich mich für ein benutzerdefiniertes Datenmanagement entschieden, um Standarddateien zu verwenden und einen einfachen Zugriff auf die Daten zu ermöglichen, ohne ein DBMS installieren zu müssen.
- Die Daten werden in durch Zeilenumbrüche getrennten JSON-Dateien gespeichert, was eine flexible und einfache Arbeit mit den Daten ermöglicht.
Bei der Implementierung der Saver-/Loader-Methoden habe ich festgestellt, wie leistungsstark JSON ist. Da die Methoden zum Laden/Speichern der Kernelemente bereits JSON verwenden, habe ich diese Methoden beim Erfassen von Klassenparametern für die UI-Elemente wiederverwendet. Beispielsweise nutzt die Klasse InfoPanel das JSON-Format, um ein übersichtliches und strukturiertes Eingabeformular zu erstellen.

Um einigen Parametern spezifische Attribute und Einschränkungen zuzuweisen, verwende ich Schema-JSON. Dies ermöglicht benutzerfreundlichere und typsichere Formulareingaben. Beispielsweise verwendet das Integer-Feld eine SpinBox, um die Eingabe auf Zahlen zu beschränken. Darüber hinaus können Felder als erforderlich, optional oder schreibgeschützt gekennzeichnet werden.

## Projektarchitektur
Das Projekt ist grob in mehrere Verzeichnisse gegliedert:
- **/**: Enthält die Dateien test.cpp, Beleg.pro, ressources.qrc und die Datei README.md.
- **/build**: Enthält die Build-Dateien, die von make und qmake generiert werden.
- **/build/moc**: Enthält die vom Qt Meta Object Compiler generierten MOC-Dateien.
- **/build/obj**: Enthält die vom Compiler generierten Objektdateien.
- **/build/rcc**: Enthält die vom Qt Resource Compiler generierten Ressourcendateien.
- **/core**: Enthält die Kerndatenmodellklassen, die Bibliothekselemente, Medienelemente, Personen und Transaktionen repräsentieren.
- **/manager**: Enthält die Klassen für die Verwaltung mehrerer Instanzen von Bibliothekselementen, Medien, Personen und Transaktionen.
- **/tablemodel**: Enthält Klassen, die die Qt-Modellansichtsarchitektur implementieren und die Anzeige von Daten in Tabellenform ermöglichen. Bietet weitere Methoden für die Interaktion mit dem Info-/Hinzufügen-Panel.
- **/ui**: Enthält die Klassen für die Benutzeroberfläche, einschließlich Hauptfenster, Symbolleiste, Dialoge und Infopanel. Enthält außerdem die Datei material.qss.
- **/icons**: Enthält die in der Benutzeroberfläche verwendeten Symbole.
- **/utils**: Enthält Hilfsfunktionen, z. B. zum Erstellen eines Standard-JSON-Codes aus dem Schema oder für den benutzerdefinierten Rückgabetyp.
- **/data**: Enthält die von der Anwendung verwendeten Datendateien.

## Konfiguration
Die Konfigurationsdatei befindet sich im Verzeichnis /core.
Sie enthält folgende Einstellungen:
- MIN_AGE: Das Mindestalter einer Person.
- MAX_AGE: Das Höchstalter einer Person.
- MAX_ITEMS_PER_BORROWER_HARD: Die maximale Anzahl von Medien, die ein Entleiher ausleihen kann.
- MAX_ITEMS_PER_BORROWER_DEFAULT: Die Standardanzahl von Medien, die ein Entleiher ausleihen kann.
- INVALID_MEDIA_ID: Die ungültige Medien-ID. Diese wird beim Initialisieren eines Libitems verwendet. Wenn also onMediaChangeCallback aufgerufen wird, erhält kein Medienelement einen negativen Referenzzähler. Ändern Sie diesen Wert nicht.

## Kompilierungsanleitung
- Während der Entwicklung wurde Version 6.9.0 verwendet. Das Projekt wurde anschließend auf Qt 5 zurückportiert.
- Zum Kompilieren des Projekts müssen Qt, C++ und make auf Ihrem System installiert sein.
- Navigieren Sie zum Projektverzeichnis und führen Sie die folgenden Befehle aus:
```bash
qmake Beleg.pro
make
```

- Sollten Probleme mit der Kompilierung auftreten, versuchen Sie, das Build-Verzeichnis zu bereinigen und erneut zu kompilieren:
```bash
find build -type f -delete
```

## Bedienungshinweise
### Daten hinzufügen
Bevor Medien ausgeliehen werden können, müssen Personen und Medien/Bibliothekselemente hinzugefügt werden, die miteinander interagieren können.

### Personen hinzufügen
1. Klicken Sie in der Tab-Auswahlleiste auf „Personen“.
2. Klicken Sie oben rechts im Fenster auf das Pluszeichen.
3. Füllen Sie das Formular aus. Rot umrandete Eingabefelder sind Pflichtfelder. Sie haben außerdem die Möglichkeit, eine Person als Künstler, Entleiher oder beides zu kennzeichnen, indem Sie die entsprechenden Kontrollkästchen aktivieren.
4. Sobald alle Pflichtfelder ausgefüllt sind, klicken Sie auf „Speichern“.
5. Wenn Sie die Eingabe abbrechen möchten, klicken Sie auf „Abbrechen“ mit dem Kreuzsymbol.
6. Die neue Person sollte nun in der Tabelle angezeigt werden.

### Bibliothekselemente hinzufügen
– Im Wesentlichen dasselbe Verfahren wie beim Hinzufügen von Personen.
– Bibliothekselemente beziehen sich auf ein Medienelement. Während das Medienelement die Informationen zum Medium selbst enthält, ist das Bibliothekselement die physische Darstellung des Mediums in der Bibliothek. Es enthält Informationen zu Standort, Verfügbarkeit und weiteren Attributen.
– Wenn Sie ein Bibliothekselement mit einem neuen Medienelement erstellen möchten, müssen Sie eine freie Medien-ID auswählen. Standardmäßig wird die nächste freie Medien-ID ausgewählt, d. h. die höchste ID + 1 aller Medienelemente im System.
– Wenn Sie ein bereits vorhandenes Medienelement auswählen, füllt das Formular die Felder des Medienelements vorab aus, sodass Sie problemlos ein neues Bibliothekselement (neue physische Darstellung) dafür erstellen können.
– Sie haben die Wahl zwischen verschiedenen Medientypen. Suchen Sie im Formular das Feld „Medientyp“ und wählen Sie den gewünschten Typ aus dem Dropdown-Menü aus. Je nach Medientyp stehen unterschiedliche Felder zum Ausfüllen zur Verfügung.

Nachdem Sie mindestens einen Entleiher und ein Bibliothekselement hinzugefügt haben, können Sie Transaktionen erstellen, die die beiden Entitäten miteinander verknüpfen.
### Transaktionen hinzufügen
1. Klicken Sie in der Tab-Auswahlleiste auf „Transaktionen“.
2. Klicken Sie oben rechts im Fenster auf das Pluszeichen.
3. Wenn Sie die IDs kennen, können Sie diese direkt eingeben. Alternativ können Sie auch den Namen des Entleihers oder Bibliothekselements eingeben. Das System macht Ihnen dann automatisch Vorschläge zur Auswahl.
4. Füllen Sie das Formular aus.
5. Klicken Sie auf „Speichern“.

### Daten löschen
Hinweis: Wenn Sie Daten löschen, auf die von einer anderen Entität verwiesen wird, sind diese Daten in der Entität nicht mehr sichtbar.
Beispiel:
- Löschen Sie eine Person, die Teil einer Transaktion ist.
- Die Person ist in der Transaktion nicht mehr sichtbar. Stattdessen enthält sie einige leere Felder.

Medienelemente werden automatisch mit dem letzten darauf verweisenden Element entfernt.

1. Wechseln Sie zum entsprechenden Reiter.
2. Wählen Sie die zu löschende(n) Zeile(n) aus.
3. Klicken Sie auf die Schaltfläche „Löschen“ oben rechts im Fenster neben dem Hinzufügen-Button.
4. Bestätigen Sie den Löschvorgang im angezeigten Dialogfenster.

### Datensuche
- Groß-/Kleinschreibung wird nicht berücksichtigt.
- Durchsucht alle Spalten der Tabelle.
- Jedes Suchtoken (durch Leerzeichen getrennt) muss mindestens einer Spalte entsprechen, damit die Zeile übereinstimmt.

1. Wechseln Sie zum entsprechenden Reiter.
2. Klicken Sie in die Suchleiste oben links im Fenster.
3. Geben Sie den Suchbegriff ein.
4. Die Tabelle wird entsprechend gefiltert.

### Daten sortieren
- Klicken Sie auf die Spaltenüberschrift der Spalte, nach der Sie sortieren möchten.
- Klicken Sie erneut, um die Sortierreihenfolge umzukehren.
ODER
- Öffnen Sie das zweite Dropdown-Menü in der Symbolleiste und wählen Sie die Spalte aus, nach der Sie sortieren möchten.

### Bestimmte Spalten anzeigen
- Klicken Sie auf das erste Dropdown-Menü in der Symbolleiste.
- Wählen Sie die Spalten aus, die Sie ein- oder ausblenden möchten.

### Details anzeigen und Daten bearbeiten
- Doppelklicken Sie auf eine Zeile in der Tabelle, die Sie anzeigen möchten.
- Ein Widget öffnet sich auf der rechten Seite des Fensters.
- Sie können die Daten bearbeiten, indem Sie im Widget auf den „Bearbeiten“-Button mit dem Stift klicken.
- Machen Sie die Änderungen rückgängig, indem Sie auf den „Abbrechen“-Button mit dem Kreuz klicken.
- Speichern Sie die Änderungen, indem Sie auf den „Speichern“-Button mit dem Diskettensymbol klicken.
- Einige Felder sind schreibgeschützt, z. B. eine ID.
- Sie können das Dock-Widget auch aus dem Fenster ziehen.