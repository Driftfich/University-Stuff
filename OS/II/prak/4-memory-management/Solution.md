# Lösungen Praktika 4 - Speicherverwaltung, Message Passing

## 1.Ein Pagingsystem habe die folgende Referenzkette seiner Seiten:

A, B, C, D, B, D, A, B, B, D, C, D
Der Hauptspeicher soll initial leer sein; die ersten 3 Referenzen führen also immer zu
einem Pagefault und lagern A, B und C ein
Der Hauptspeicher kann genau 3 Seiten fassen. Ermitteln Sie Zeitpunkte der Seiten-
fehler und die jeweils ausgelagerten Seiten für:

### a) den optimalen Algorithmus,


| Zeitpunkt | Pages   | Einlagern | Auslagern | Seitenfehler |
| --------- | ------- | --------- | --------- | ------------ |
| 1-3       | -       | A, B, C   | -         | 3            |
| 4-10      | A, B, C | D         | C         | 1            |
| 11        | A, B, D | C         | A         | 1            |
| 12        | B, D, C |           |           |              |


b) 2nd Chance,


| Zeitpunkt | Pages   | Einlagern | Auslagern | Seitenfehler | R-Bits  |
| --------- | ------- | --------- | --------- | ------------ | ------- |
| 1-3       | -       | A, B, C   | -         | 3            | A, B, C |
| 4         | B, C, D | D         | A         | 1            | D       |
| 5-6       | B, C, D | -         | -         | 0            | -       |
| 7         | B, C, D | A         | C         | 1            | A       | 
| 8-10     | D, B, A  | -         | -         | 0            | -       |
| 11       | B, A, C  | C         | B         | 1            | C       |
| 12       | A, C, D  |           |           | 0            | D       |


c) NFU. 


| Zeitpunkt | Pages   | Einlagern | Auslagern | Seitenfehler | Page Count [A, B, C, D] |
| --------- | ------- | --------- | --------- | ------------ | ----------------------- |
| 1         | -       | A, B, C   | -         | 3            | [0, 0, 0, 0]            |
| 4         | A, B, C | D         | A         | 1            | [1, 1, 1, 0]            |
| 7         | C, B, D | A         | C         | 1            | [0, 2, 1, 2]            |
| 11        | A, B, D | C         | A         | 1            | [1, 4, 0, 3]            |
| 12        | B, D, C |           |           |              | [0, 4, 1, 4]            |

## 2. “A small computer has four page frames. At the first clock tick, the R bits are 0111
(page 0 is 0, the rest are 1). At subsequent clock ticks, the values are 1011, 1010, 1101,
0010, 1010, 1100, and 0001. If the aging algorithm is used with an 8-bit counter, give
the values of the four counters after the last tick” (A. Tanenbaum: Modern Operating
Systems, 3rd ed., Pearson, Aufgabe 23, S. 249)

| Tick | R-Bits | Zähler Seite 0 | Zähler Seite 1 | Zähler Seite 2 | Zähler Seite 3 |
| Start | - | 00000000 | 00000000 | 00000000 | 00000000 |
| 1 | 0111 | 00000000 | 10000000 | 10000000 | 10000000 |
| 2 | 1011 | 10000000 | 01000000 | 11000000 | 11000000 |
| 3 | 1010 | 11000000 | 00100000 | 11100000 | 01100000 |
| 4 | 1101 | 11100000 | 10010000 | 01110000 | 10110000 |
| 5 | 0010 | 01110000 | 01001000 | 10111000 | 01011000 |
| 6 | 1010 | 10111000 | 00100100 | 11011100 | 00101100 |
| 7 | 1100 | 11011100 | 10010010 | 01101110 | 00010110 |
| 8 | 0001 | 01101110 | 01001001 | 00110111 | 10001011 |
| 8 |      | 110      | 73       | 55       | 139      |

Bei dem nächsten Seitenfehler müsste die Seite 2 ausgelagert werden, da diese den kleinsten Zählerstand aufweist.

## 3. Die Gr¨oße des Datensegmentes eines Prozesses kann unter UNIX mittels brk() bzw.
sbrk() vergr¨oßert werden. Entwickeln Sie ein kleines Testprogramm, mit dem Sie die
Differenz zwischen voreingestellter und maximal m¨oglicher Datensegmentgr¨oße ermit-
teln k¨onnen. Vergleichen Sie Ihre Ergebnisse f¨ur verschiedene Systeme!

## 4. Nutzen Sie mmap(), um ein einfaches Programm zum Kopieren von Dateien (analog
dem Kommando cp) zu schreiben

## 5. Zeigen/Demonstrieren Sie programmtechnisch, dass mmap() zur IPC eingesetzt wer-
den kann, selbst wenn keine Datei beteiligt ist.

## Task 6 and 7 still missing
