from dbConnect import sessionLoader
from mapper import Ersatzteil, Montage
from sqlalchemy.orm import Session
import pandas as pd

def getErsatzteilListe() :
    """ Definition der Funktion getErsatzteilListe().
    Diese Funktion gibt alle Ersatzteile aus der Tabelle Ersatzteil aus.

    :return None
    :rtype None
    """

    session = sessionLoader()
    try:
        ersatzteile = session.query(Ersatzteil).all()
        columns = ['EtID', 'EtBezeichnung', 'EtPreis', 'EtAnzLager', 'EtHersteller']    
        df_data = []
        for row in ersatzteile:
            df_data.append([
                row.EtID,
                row.EtBezeichnung,
                row.EtPreis,
                row.EtAnzLager,
                row.EtHersteller
            ])
        df = pd.DataFrame(df_data, columns=pd.Index(columns))
        print(df.to_string(index=False))
    except:
        print('Abfrage ist fehlerhaft')
        session.close()
        return None
    
    
def getErsatzteilForAuftrag(p_aufnr: int, session: Session|None = None) :
    """ Definition der Funktion getErsatzteilForAuftrag().
    Diese Funktion gibt alle Ersatzteile für einen Auftrag aus der Tabelle Ersatzteil aus.
    
    :param p_aufnr - Auftragsnummer
    
    :return None
    """
    # Internsession: Session = sessionLoader() if session is None else session
    Internsession: Session
    ownsession: bool
    if session is None:
        Internsession = sessionLoader()
        ownsession = True
    else:
        Internsession = session
        ownsession = False
    try:
        ersatzteile = Internsession.query(Ersatzteil.EtID, Ersatzteil.EtBezeichnung, Ersatzteil.EtPreis, Ersatzteil.EtAnzLager, Ersatzteil.EtHersteller, Montage.Anzahl).join(Montage).filter(Montage.AufNr == p_aufnr).all()
        columns = ['EtID', 'EtBezeichnung', 'EtPreis', 'EtAnzLager', 'EtHersteller', 'Anzahl']
        df = pd.DataFrame(ersatzteile, columns=pd.Index(columns))
        print(df.to_string(index=False))
    except:
        print('Abfrage ist fehlerhaft')
    finally:
        if ownsession:
            Internsession.close()
        return

def addErsatzteilToAuftrag(p_aufnr, p_etid, p_anzahl):
    """ Definition der Funktion addErsatzteilToAuftrag().
    Diese Funktion fügt ein Ersatzteil zu einem Auftrag hinzu.
    
    :param p_aufnr - Auftragsnummer
    :param p_etid - Ersatzteilnummer
    :param p_anzahl - Anzahl
    """
    session = sessionLoader()
    try:
        montage = Montage(p_etid, p_aufnr, p_anzahl)
        session.add(montage)
        session.commit()

