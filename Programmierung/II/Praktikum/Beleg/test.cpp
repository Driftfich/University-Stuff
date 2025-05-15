#include "media.h"
#include "text.h"
#include <QDate>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <iostream>

int main() {
    // Test the Media class
    Media m1 Media(1, "Test Title", QDate::currentDate(), QVector<int>{1, 2}, "Test Publisher", "Test Description", "Test Genre", QVector<QString>{"English"}, QMap<QString, QVariant>{});
    Text t1 Text(1, "Test Title", QDate::currentDate(), QVector<int>{1, 2}, "Test Publisher", "Test Description", "Test Genre", QVector<QString>{"English"}, 5, 100, "1234567890123", "Paperback", "Book");

    // create a magic pointer array on the m1 and t1 objects
    std::shared_ptr<Media> mediaPtr1 = std::make_shared<Media>(m1);
    std::shared_ptr<Text> textPtr1 = std::make_shared<Text>(t1);
    // create a magic pointer array on the m1 and t1 objects
    std::vector<std::shared_ptr<Media>> mediaPtrs = {mediaPtr1, textPtr1};

    return 0;
}