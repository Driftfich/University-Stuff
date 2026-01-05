from dbConn import getConn
from checker import handleInputInteger
import numpy as np
import pandas as pd

def getNiederlassung() -> int|None:
    """ Definition der Funktion getNiederlassung
    Die Funktion ruft alle Niederlassungen aus der Tabelle Niederlassung ab und gibt sie tabellarisch aus.
    Der Benutzer wird aufgefordert, eine der Niederlassungsnummern einzugeben.
    
    :return eingabe_nlnr - Niederlassungsnummer, die vom Benutzer eingegeben wurde
    :rtype  int
    """
    
    conn = getConn()
    cursor = conn.cursor()
    
    try:
        cursor.execute('SELECT nlnr, ort from niederlassung')
    except:
        print('Abfrage ist fehlerhaft')
        cursor.close()
        return None
    
    print('\nNiederlassungen:')
    liste_nlnr = [0]
    for row in cursor:
        print(row[0], ' - ', row[1])
        liste_nlnr.append(row[0])
    cursor.close()
    conn.close()

    # check if the entered Niederlassungsnummer is in the db
    eingabe_nlnr = -1
    while True:
        eingabe_nlnr = handleInputInteger('Ort eingeben (Nr)')
        if eingabe_nlnr not in liste_nlnr:
            print('Niederlassungsnummer nicht in Datenbank gefunden')
            continue
        break

    return eingabe_nlnr


def getMitarbeiter(p_nlnr: int) -> int|None:
    """ Definition der Funktion getMitarbeiter
    Die Funktion ruft alle Mitarbeiter mit der übergebenen Niederlassungsnummer ab und gibt
    sie tabellarisch aus.
    Der Benutzer wird aufgefordert eine Mitarbeiternummer einzugeben.

    .param  p_nlnr - Niederlassungsnummer
    :return eingabe_mitnr - Mitarbeiternummer, die vom Benutzer eingegebn wurde
    :rtype  int
    """

    conn = getConn()
    cursor = conn.cursor()

    try:
        cursor.execute('SELECT MitID, MitName, MitVorname, MitJob, MitStundensatz from Mitarbeiter where nlnr = ?', [p_nlnr])
    except:
        print('Abfrage ist fehlerhaft')
        cursor.close()
        return None

    columns = ['MitID', 'MitName', 'MitVorname', 'MitJob', 'MitStundensatz']

    df = pd.DataFrame(cursor, columns=pd.Index(columns))
    cursor.close()
    conn.close()

    if df.empty:
        print('Kein Mitarbeiter in der Niederlassung beschäftigt')
        return None

    df.rename(columns={'MitID': 'Mitarbeiternummer', 'MitName': 'Name', 'MitVorname': 'Vorname', 'MitJob': 'Job', 'MitStundensatz': 'Stundensatz'}, inplace=True)
    print(df.to_string(index=False))

    # check if the entered Mitarbeiternummer is in the db
    while True:
        eingabe_mitnr = handleInputInteger('Mitarbeiternummer eingeben')
        if eingabe_mitnr not in df['Mitarbeiternummer'].values:
            print('Mitarbeiternummer nicht in Datenbank gefunden')
            continue
        break
    return eingabe_mitnr


def getAuftrag(p_mitnr: int) -> None:
    """ Definition der Funktion getMitarbeiter
    Die Funktion ruft alle Aufträge des Mitarbeiter für die übergebene Mitarbeiternummer 
    der kommenden Kalenderwoche ab und gibt sie tabellarisch aus.

    :param  p_mitnr: int - Mitarbeiternummer
    :return None
    :rtype  None
    """

    conn = getConn()
    cursor = conn.cursor()

    try:
        cursor.execute('SELECT AufNr, KunNr, AufDat, ErlDat, Dauer, Anfahrt, Beschreibung from Auftrag where MitID = ? ORDER BY AufDat DESC', [p_mitnr])
    except:
        print('Abfrage ist fehlerhaft')
        cursor.close()
        return None
    
    columns = ['AufNr', 'KunNr', 'AufDat', 'ErlDat', 'Dauer', 'Anfahrt', 'Beschreibung']
    df = pd.DataFrame(cursor, columns=pd.Index(columns))

    cursor.close()
    conn.close()

    if df.empty:
        print('Kein Auftrag für den Mitarbeiter gefunden')
        return None

    df.rename(columns={'AufNr': 'Auftragsnummer', 'KunNr': 'Kundennummer', 'AufDat': 'Auftragsdatum', 'ErlDat': 'Erledigt am', 'Dauer': 'Dauer', 'Anfahrt': 'Anfahrt', 'Beschreibung': 'Beschreibung'}, inplace=True)
    print(df.to_string(index=False))

    return None
