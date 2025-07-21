-- t-sql praktika 3
CREATE TABLE Mitarbeiter (
    MitID CHAR(3) PRIMARY KEY NOT NULL,
    MitName VARCHAR(20) NOT NULL,
    MitVorname VARCHAR(20),
    MitGebDat Date NOT NULL,
    MitJob VARCHAR(20) NOT NULL,
    MitStundensatz SMALLMONEY NOT NULL,
    MitEinsatzort VARCHAR(20) NULL
)

CREATE TABLE Kunde (
    KunNr Int PRIMARY KEY NOT NULL,
    KunName VARCHAR(20) NOT NULL,
    KunOrt VARCHAR(20) NOT NULL,
    KunPLZ char(5) NOT NULL,
    KunStrasse VARCHAR(20) NOT NULL
)

CREATE TABLE Projekt (
    ProNr Int PRIMARY KEY NOT NULL,
)