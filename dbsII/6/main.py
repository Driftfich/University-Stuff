from logicDatenzugriff import getNiederlassung, getMitarbeiter, getAuftrag

nlnr: int|None = getNiederlassung()            # Niederlassung aus Niederlassungsliste auswählen
if nlnr is None:
    exit(0)

while nlnr is not None and nlnr > 0:
    print("Niederlassung: " + str(nlnr) + " ausgewählt\n")
    mitnr: int|None = getMitarbeiter(nlnr)     # Mitarbeiter aus Mitarbeiterliste auswählen
    while mitnr is not None and mitnr > 0:
        print("Mitarbeiter: " + str(mitnr) + " ausgewählt\n")
        getAuftrag(mitnr)            # Aufträge des Mitarbeiters anzeigen
        mitnr = getMitarbeiter(nlnr) # neuen Mitarbeiter aus Mitarbeiterliste auswählen
    nlnr = getNiederlassung()        # neue Niederlassung aus Niederlassungsliste auswählen
