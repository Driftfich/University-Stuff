import time

def handleInputInteger(prompttext) -> int:
    """ Definition der Funktion handleInput
    Die Funktion stellt sicher, dass es sich bei der eingegebenen Zeichenkette um
    ein ganze postive Zahlen ist. Sollte nichts eingegeben werden, wird der Wert 0 zurück gegeben

    :param prompttext (String) - Text, der mit angezeigt werden soll
    :return nr - Werte der eingegebenen Zahl, 0 wenn die Eingabe leer war.
    :rtype int
    """
    # inputstring = 'bla'
    # while inputstring.isdigit() is False:
    #     inputstring = input(prompttext + ': ')
    #     if len(inputstring) == 0:
    #         inputstring = '0'
    #     if inputstring.isdigit():
    #         nr = int(inputstring)
    # return nr

    while True:
        inpStr: str = input(prompttext + ': ')
        if inpStr.isdigit():
            return int(inpStr)
        elif inpStr == '':
            return 0
        else:
            print('Bitte eine ganze positive Zahl eingeben')
            continue


def handleInputDatum(prompttext) -> str:
    """ Definition der Funktion handleInputDatum
    Die Funktion stellt sicher, dass es sich bei der eingegebenen Zeichenkette um
    ein Datum handelt. Das Datum wird als Zeichenkette zurückgegeben.
    Die Eingabe wird solange erzwungen, bis ein gültiges Datum eingegeben wurde.

    :param prompttext (String) Text, der als Eingabeaufforderung angezeigt werden soll
    :return eingabe_datum Das eingegebene Datum im Format dd.mm.YYYY
    :rtype string
    """
    # isDatum = False
    # while isDatum is False:
    #     eingabe_datum = input(prompttext + ': ')
    #     try:
    #         isDatum=bool(time.strptime(eingabe_datum, "%d.%m.%Y"))
    #     except:
    #         print('Bitte ein gültiges Datum im Format dd.mm.YYYY eingeben')
    #         isDatum = False
    # return(eingabe_datum)

    
    while True:
        inpStr: str = input(prompttext + ': ')
        try:
            isDatum=bool(time.strptime(inpStr, "%d.%m.%Y"))
        except:
            print('Bitte ein gültiges Datum im Format dd.mm.YYYY eingeben')
            continue
        return inpStr