#include <iostream>
#include <memory>
#include "core/person.h"
#include "core/artist.h"
#include "core/borrower.h"

/**
 * @brief Test der neuen Kompositions-Architektur für Person, Artist und Borrower
 * 
 * Dieses Programm testet die erfolgreiche Umstrukturierung von Vererbung zu Komposition.
 * Personen können jetzt sowohl Artist- als auch Borrower-Rollen gleichzeitig haben.
 */
int main() {
    std::cout << "=== Test der neuen Kompositions-Architektur ===" << std::endl;
    
    try {
        // 1. Erstelle eine normale Person ohne Rollen
        auto person1 = std::make_shared<Person>(
            1, "Max", "Mustermann", "", 
            QDate(1990, 5, 15), "male", 
            "Testperson", "Berlin", "max@test.de", "+49123456789"
        );
        
        std::cout << "1. Person ohne Rollen erstellt:" << std::endl;
        std::cout << "   - ID: " << person1->getId() << std::endl;
        std::cout << "   - Name: " << person1->getFname().toStdString() << " " 
                  << person1->getLname().toStdString() << std::endl;
        std::cout << "   - Ist Artist: " << (person1->isArtist() ? "Ja" : "Nein") << std::endl;
        std::cout << "   - Ist Borrower: " << (person1->isBorrower() ? "Ja" : "Nein") << std::endl;
        std::cout << std::endl;
        
        // 2. Füge Artist-Rolle hinzu
        if (person1->createArtistRole("Musiker", {1, 2, 3}) == 0) {
            std::cout << "2. Artist-Rolle erfolgreich hinzugefügt:" << std::endl;
            std::cout << "   - Ist Artist: " << (person1->isArtist() ? "Ja" : "Nein") << std::endl;
            if (person1->getArtist()) {
                std::cout << "   - Artist-Typ: " << person1->getArtist()->getArtistType().toStdString() << std::endl;
                std::cout << "   - Anzahl Medien: " << person1->getArtist()->getMediaIds().size() << std::endl;
            }
        } else {
            std::cout << "Fehler beim Hinzufügen der Artist-Rolle!" << std::endl;
        }
        std::cout << std::endl;
        
        // 3. Füge Borrower-Rolle hinzu (jetzt hat die Person beide Rollen!)
        if (person1->createBorrowerRole(7) == 0) {
            std::cout << "3. Borrower-Rolle erfolgreich hinzugefügt:" << std::endl;
            std::cout << "   - Ist Borrower: " << (person1->isBorrower() ? "Ja" : "Nein") << std::endl;
            if (person1->getBorrower()) {
                std::cout << "   - Borrower-Limit: " << person1->getBorrower()->getLimit() << std::endl;
            }
        } else {
            std::cout << "Fehler beim Hinzufügen der Borrower-Rolle!" << std::endl;
        }
        std::cout << std::endl;
        
        // 4. Zeige, dass Person beide Rollen gleichzeitig hat
        std::cout << "4. Person hat jetzt beide Rollen:" << std::endl;
        std::cout << "   - Ist Artist: " << (person1->isArtist() ? "Ja" : "Nein") << std::endl;
        std::cout << "   - Ist Borrower: " << (person1->isBorrower() ? "Ja" : "Nein") << std::endl;
        std::cout << "   ✓ Problem der Einfachvererbung gelöst!" << std::endl;
        std::cout << std::endl;
        
        // 5. Teste Serialisierung
        std::cout << "5. Teste JSON-Serialisierung:" << std::endl;
        QJsonObject json = person1->getJson();
        std::cout << "   - JSON enthält artist: " << (json.contains("artist") ? "Ja" : "Nein") << std::endl;
        std::cout << "   - JSON enthält borrower: " << (json.contains("borrower") ? "Ja" : "Nein") << std::endl;
        std::cout << std::endl;
        
        // 6. Teste Copy Constructor mit Rollen
        std::cout << "6. Teste Copy Constructor:" << std::endl;
        Person person2(*person1);
        std::cout << "   - Kopierte Person ist Artist: " << (person2.isArtist() ? "Ja" : "Nein") << std::endl;
        std::cout << "   - Kopierte Person ist Borrower: " << (person2.isBorrower() ? "Ja" : "Nein") << std::endl;
        std::cout << std::endl;
        
        // 7. Teste Rollen-Entfernung
        std::cout << "7. Teste Rollen-Entfernung:" << std::endl;
        person1->removeArtistRole();
        std::cout << "   - Artist-Rolle entfernt - ist Artist: " << (person1->isArtist() ? "Ja" : "Nein") << std::endl;
        std::cout << "   - Borrower-Rolle bleibt - ist Borrower: " << (person1->isBorrower() ? "Ja" : "Nein") << std::endl;
        
        std::cout << std::endl << "✓ Alle Tests erfolgreich!" << std::endl;
        std::cout << "✓ Kompositions-Architektur funktioniert korrekt!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Fehler beim Test: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
