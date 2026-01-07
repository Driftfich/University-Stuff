from dbConnect import sessionLoader
from mapper import Mitarbeiter, Auftrag, Kunde, Ersatzteil, Montage
from checker import handleInputInteger, handleInputDatum
from logicErsatzteil import getErsatzteilForAuftrag
import datetime
from sqlalchemy import exc, or_
from typing import List
import pandas as pd

def getAuftrag(p_mitnr):
    """ Definition der Funktion getMitarbeiter
    Die Funktion ruft alle Aufträge des Mitarbeiter für die übergebene Mitarbeiternummer 
    der kommenden Kalenderwoche ab und gibt sie tabellarisch aus.

    :param  p_mitnr - Mitarbeiternummer
    :return eingabe_aufnr - Auftragsnummer, die vom Benutzer eingegeben wurde
    :rtype  int
    """
    jetzt = datetime.datetime.now()  # ermitteln des aktuellen Datums
    aktuelle_woche = jetzt.isocalendar()[1]  # ermitteln der aktuellen Kalenderwoche
    naechste_woche = aktuelle_woche + 1  # ermitteln der nächsten Kalenderwoche
    anz_auftraege = 0

    session = sessionLoader()
    # Abruf der Mitarbeiterdaten zur übergebenen Mitarbeiternummer
    mitarbeiter = session.query(Mitarbeiter).get(p_mitnr)
    # Überprüfung, ob der Mitarbeiter in der vorher ausgewählten Niederlassung tätig ist, ist nicht nötig, da
    # in getMitarbeiter nur Mitarbeiternummern der Niederlassung eingegeben werden können.
    
    # Abbruch, falls der Mitarbeiter nicht vorhanden ist
    if isinstance(mitarbeiter, type(None)):
        print(f'Mitarbeiter mit der MitNr: {p_mitnr} existiert nicht in der Datenbank.')
        session.close()
        return 0 
    
    # Ausgabe der Mitarbeiterdaten
    print(f'{mitarbeiter.Name}, {mitarbeiter.Vorname} - {mitarbeiter.Geburtsdatum.strftime(format="%d.%m.%Y")}, {Mitarbeiter.Job}')
    print()

    # Alle Aufträge des Mitarbeiters ermitteln
    if len(mitarbeiter.ListeAuftrag) > 0:  # Überprüfung, ob Aufträge ermittelt werden konnten
        print(f'Die Aufträge des Mitarbeiters {mitarbeiter.Name} in den Kalenderwochen {aktuelle_woche} und {naechste_woche}:')
        for auf in mitarbeiter.ListeAuftrag:
            # Vergleich, ob das Erledigungsdatum in dieser oder der kommenden Kalenderwoche liegt
            if auf.Erledigungsdatum.isocalendar()[1] in (aktuelle_woche, naechste_woche):
                # Zugriff auf die Kundendaten über die Beziehung, die in Klasse Auftrag definiert ist
                print(f'{auf.AufNr} - {auf.MitId} {auf.Erledigungsdatum} {auf.Kunde.Name} {auf.Kunde.Plz} {auf.Kunde.Ort}')
                anz_auftraege += 1  # Anzahl der ausgegebenen Aufträge hochzählen
        # Falls keine Aufträge ausgegeben wurden (keine Aufträge in der kommenden Woche liegen)
        if anz_auftraege == 0:
            print('Der Mitarbeiter hat in dieser Zeit keine Aufträge')
    print('-----------------------------------------------------------------')
    print()
    session.close()
    return 0 #Return anpassen


def anlegenAuftrag():
    """ Definition der Funktion anlegenAuftrag
    Die Funktion legt einen neuen Autrag an. Wenn es sich um einen neuen Kunden handelt,
    wird auch der Kunde neu angelegt.
    """
    session = sessionLoader()
    eingabe_kunname = input('Name des Kunden: ')
    # Nachschlagen der Kunden mit diesem Namen
    menge_kun = session.query(Kunde).filter(Kunde.Name == eingabe_kunname).all()
    # falls es Kunden mit diesem Namen gibt
    if len(menge_kun) > 0:
        liste_kunnr = [0]  # Liste der Kundennummern mit dem ersten Element 0 anlegen
        for kun in menge_kun:
            print(f' {kun.KunNr} - {kun.Name}, {kun.Ort}')
            liste_kunnr.append(kun.KunNr)  # Kundennummer zur Liste hinzufügen
        # Eingabe der Kundennummer, wobei diese Eingabe solange wiederholt wird, bis einer der gerade
        # angezeigten Kundennummern eingegeben wird
        eingabe_kunnr = -1
        while eingabe_kunnr not in liste_kunnr:
            eingabe_kunnr = handleInputInteger('Kundenummer auswählen - neuer Kunde bitte 0 eingeben')
    else:
        eingabe_kunnr = 0
        
    # falls ein neuer Kunde angelegt werden soll -> Eingabe der Daten
    if eingabe_kunnr == 0:
        eingabe_kunname = input('Name: ')
        eingabe_kunort  = input('Ort: ')
        eingabe_kunplz  = input('PLZ: ')
        eingabe_kunstrasse = input('Straße: ')
        # Anlegen des neuen Kunden
        kunde = Kunde(eingabe_kunname, eingabe_kunort, eingabe_kunplz, eingabe_kunstrasse)
        try:
            session.add(kunde)  # Einfügen des Datensatzes in die DB
            session.commit()  # Commit - ist bei Datenänderungen zwingend erforderlich
            eingabe_kunnr = kunde.KunNr  # merken der Kundennummer, um damit weiter zu arbeiten
            print(f'Kunde {eingabe_kunnr} - {kunde.Name} angelegt.')
        except exc.SQLAlchemyError():
            print('Fehler beim einfügen des Kunden')
            session.close()
            return

    eingabe_aufdat = handleInputDatum('Auftragsdatum')
    auftrag = Auftrag(eingabe_kunnr, eingabe_aufdat)
    try:
        session.add(auftrag)
        session.commit()
        # Abfrage des neuen Auftrags aus der DB und Anzeige der Daten zur Überprüfung
        angelegter_auftrag = session.query(Auftrag).get(auftrag.AufNr)
        print()
        print(f'Neuer Auftrag: {angelegter_auftrag.AufNr} - {angelegter_auftrag.KunNr} {angelegter_auftrag.Auftragsdatum}')
        print()
    except exc.SQLAlchemyError():
        print('Fehler - Auftrag einfügen')
    session.close()


def planenAuftrag():
    """ Definition der Funktion planenAuftrag
    Bei der Planung wird dem Auftrag ein Mitarbeiter und ein Erledigungsdatum zugewiesen.
    """
    session = sessionLoader()
    # ungeplante Aufträge der letzten 20 Tage abfragen und ausgeben
    heute = datetime.datetime.now()
    abdatum = heute - datetime.timedelta(days=20)
    menge_auftrag = session.query(Auftrag) \
        .filter(Auftrag.Erledigungsdatum == None, Auftrag.Auftragsdatum > abdatum, Auftrag.Auftragsdatum <= heute) \
        .order_by(Auftrag.Auftragsdatum).all()
    if len(menge_auftrag) > 0:
        liste_aufnr = [0]  # Liste der angezeigten Auftragsnummern initialisieren
        for auf in menge_auftrag:
            print(f' {auf.AufNr} - {auf.Auftragsdatum}; {auf.Kunde.Ort}')
            liste_aufnr.append(auf.AufNr)  # Auftragsnummer zur Liste hinzufügen
        
        # Auftragsnummer eingeben lassen - muss in der erstellten Liste sein
        eingabe_aufnr = -1
        while eingabe_aufnr not in liste_aufnr:
            eingabe_aufnr = handleInputInteger('Auftragsnummer')
        if eingabe_aufnr != 0:           
            #Eingabe Erledigungsdatum
            eingabe_erldat = handleInputDatum('Erledigungsdatum')
            print("")
            
            # Absichern, dass nur Mitarbeiternummern von Monteuren oder Meistern eingegeben werden können
            menge_mitarbeiter = session.query(Mitarbeiter)\
                .filter(or_(Mitarbeiter.Job == 'Monteur', Mitarbeiter.Job == 'Meister')).all()
            liste_mit = []
            for mit in menge_mitarbeiter:
                print(f' {mit.MitId} - {mit.Name} {mit.Vorname}')
                liste_mit.append(int(mit.MitId))
            eingabe_mitid = -1
            while eingabe_mitid not in liste_mit:
                eingabe_mitid = handleInputInteger('Mitarbeiternummer')
            # Ende Absicherung
            
            # Auftragsobjekt-Objekt aus der Datenbank laden
            auftrag = session.query(Auftrag).get(eingabe_aufnr)
            # Abbruch, wenn der Auftrag nicht existiert
            if isinstance(auftrag, type(None)): 
               print(f'Auftrag {eingabe_aufnr} existiert nicht in der Datenbank.')  
               session.close()
               return
            try:
                # Update des Datensatzes mit der eingegebenen Auftragsnummer
                auftrag.Erledigungsdatum = eingabe_erldat
                auftrag.MitId = eingabe_mitid
                session.commit()
                # Abruf und Ausgabe des gerade geänderten Auftrages
                auftragUpdated = session.query(Auftrag).get(eingabe_aufnr)
                print(f' {auftragUpdated.AufNr} - Mitarbeiter: {auftragUpdated.Mitarbeiter.Name}, \
                Kunde: {auftragUpdated.Kunde.Name}, {auftragUpdated.Auftragsdatum}, {auftragUpdated.Erledigungsdatum}')
            except exc.SQLAlchemyError():
                print('Datenänderung nicht möglich.')
        else:
            print('Keine Auftragsnummer ausgewählt')
            session.close()
            return
    else:
        print('Keine neuen Aufträge vorhanden\n')
    session.close()

def erledigenAuftrag():
    """
    1. Anzeigen geplanter Aufträge der letzten 20 Tage bis heute.
    2. Eingabe der Auftragsnummer
    3. Anzeigen der Auftragsdaten und Abfrage ob der Auftrag als erledigt markiert werden soll
    5. Eingabe der Dauer und Anfahrt
    4. Eingabe der Ersatzteile und deren Anzahl
    6. Markierung des Auftrags als erledigt
    """
    session = sessionLoader()
    try:
        currDate: datetime.datetime = datetime.datetime.now()
        AufList: List[Auftrag] = session.query(Auftrag).filter(
            Auftrag.Auftragsdatum.between(currDate - datetime.timedelta(days=20), currDate),
            Auftrag.MitId != None,
            Auftrag.Dauer == None
        ).all()

        if not AufList:
            print('Keine geplanten Aufträge vorhanden')
            session.close()
            return

        # aufDf = pd.DataFrame(AufList, columns=pd.Index(['AufNr', 'MitId', 'KunNr', 'Auftragsdatum', 'Erledigungsdatum', 'Dauer', 'Anfahrt']))
        df_data = []
        for row in AufList:
            df_data.append([
                row.AufNr,
                row.MitId,
                row.KunNr,
                row.Auftragsdatum,
                row.Erledigungsdatum,
                row.Dauer,
                row.Anfahrt
            ])
        
        aufDf = pd.DataFrame(df_data, columns=pd.Index(['AufNr', 'MitId', 'KunNr', 'Auftragsdatum', 'Erledigungsdatum', 'Dauer', 'Anfahrt']))

        if aufDf.empty:
            print('Keine geplanten Aufträge vorhanden')
            session.close()
            return
        
        print(aufDf.to_string(index=False))
        aufNr = handleInputInteger('Eingabe der Auftragsnummer')
        while aufNr not in aufDf['AufNr'].values:
            aufNr = handleInputInteger('Eingabe der Auftragsnummer')

        print("Folgenden Auftrag als erledigt markieren?\n")
        print(aufDf[aufDf['AufNr'] == aufNr].to_string(index=False))
        if 'j' in input('j/n: ').lower():

            auftrag: Auftrag|None = session.query(Auftrag).get(aufNr)
            if isinstance(auftrag, type(None)):
                print(f'Auftrag {aufNr} existiert nicht in der Datenbank.')
                session.rollback()
                session.close()
                return

            print(f"Eingabe der Dauer und Anfahrt für Auftrag {aufNr}:")
            dauer = handleInputInteger('Eingabe der Dauer')
            anfahrt = handleInputInteger('Eingabe der Anfahrt')

            # Ersatzteile des Auftrags abfragen
            print(f"Ersatzteile des Auftrags {aufNr}:")
            getErsatzteilForAuftrag(aufNr, session)

            while True:
                etid = input('Ersatzteilnummer [int] / break (q) zum Beenden / show (s) zum Anzeigen: ')
                if 's' in etid:
                    print(f"Ersatzteile des Auftrags {aufNr}:")
                    getErsatzteilForAuftrag(aufNr, session)
                    continue
                if 'q' in etid or not etid.isnumeric():
                    break
                etid = int(etid)
                anzahl: int = handleInputInteger(f'Eingabe der Anzahl für Ersatzteil {etid}')

                # query Ersatzteil from database
                ersatzteil: Ersatzteil|None = session.query(Ersatzteil).get(etid)
                if isinstance(ersatzteil, type(None)):
                    print(f"Ersatzteil {etid} existiert nicht in der Datenbank.")
                    continue

                if ersatzteil.EtAnzLager < anzahl:
                    print(f"Nicht genügend Ersatzteile auf Lager. {ersatzteil.EtAnzLager} verfügbar.")
                    continue

                # update the ersatzteil
                ersatzteil.EtAnzLager -= anzahl

                if ersatzteil.EtAnzLager < 0:
                    print(f"Nicht genügend Ersatzteile auf Lager. {ersatzteil.EtAnzLager} verfügbar.") 
                    session.rollback()
                    # continue
                    return
                
                # create montage
                existing_montage: Montage|None = session.query(Montage).filter_by(EtID=etid, AufNr=aufNr).first()

                if existing_montage:
                    existing_montage.Anzahl += anzahl
                else:
                    newMontage: Montage = Montage(etid, aufNr, anzahl)
                    session.add(newMontage)
                    
                print(f"{anzahl}x {ersatzteil.EtBezeichnung} hinzugefügt.")
                    
            
            auftrag.Erledigungsdatum = currDate
            auftrag.Dauer = dauer
            auftrag.Anfahrt = anfahrt

            session.commit()
            print(f'Auftrag {aufNr} erledigt')
        else:
            print('Auftrag nicht erledigt')

        session.close()
        return
    except Exception as e:
        print(f"Rolling back transaction due to Exception {e}") 
        session.rollback()
        return

    finally:
        session.close()
