#include <QDate>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <iostream>
#include "media.h"
#include "text.h"
#include "video.h"
#include "audio.h"

int main() {
    // Test the Media class
    Media m1 = Media((unsigned long) 1, QString("Test Title"), QDate::currentDate(), QVector<unsigned long>{1, 2}, QString("Test Publisher"), QString("Test Description"), QString("Test Genre"), QVector<QString>{"English"}, QMap<QString, QVariant>{});
    Text t1 = Text((unsigned long) 1, QString("Test Title"), QDate::currentDate(), QVector<unsigned long>{1, 2}, QString("Test Publisher"), QString("Test Description"), QString("Test Genre"), QVector<QString>{"English"}, QMap<QString, QVariant>{}, 5, QString("1234567890123"), QString("Paperback"), QString("Book"));
    Video v1 = Video((unsigned long) 1, QString("Test Title"), QDate::currentDate(), QVector<unsigned long>{1, 2}, QString("Test Publisher"), QString("Test Description"), QString("Test Genre"), QVector<QString>{"English"}, QMap<QString, QVariant>{}, 120, QString("Movie"), 1080, QString("H.264"), QString("16:9"), QString("Stereo"));
    Audio a1 = Audio((unsigned long) 1, QString("Test Title"), QDate::currentDate(), QVector<unsigned long>{1, 2}, QString("Test Publisher"), QString("Test Description"), QString("Test Genre"), QVector<QString>{"English"}, QMap<QString, QVariant>{}, 300, QString("Song"), 320, 44100, QString("Stereo"), QString("MP3"), QString("Test Album"), QVector<QString>{"Track 1", "Track 2"});

    // create a magic pointer array on the m1 and t1 objects
    std::shared_ptr<Media> mediaPtr1 = std::make_shared<Media>(m1);
    std::shared_ptr<Text> textPtr1 = std::make_shared<Text>(t1);
    std::shared_ptr<Video> videoPtr1 = std::make_shared<Video>(v1);
    std::shared_ptr<Audio> audioPtr1 = std::make_shared<Audio>(a1);
    // create a magic pointer array on the m1 and t1 objects
    std::vector<std::shared_ptr<Media>> mediaPtrs = {mediaPtr1, textPtr1, videoPtr1, audioPtr1};

    // // save all objects to a file
    QString filename = QString("test.ndjson");
    for (int i = 0; i < (int) mediaPtrs.size(); ++i) {
        std::cout << "Saving object " << i << " to file: " << *mediaPtrs[i] << std::endl;
        QFile file(filename);
        mediaPtrs[i]->toFile(file);
        file.close();
    }
    
    // load all objects from a file by using the factory method from the media class
    // clear the mediaPtrs vector
    mediaPtrs.clear();
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: Could not open file for reading" << std::endl;
        return -1;
    }
    // std::cout << "Loading objects from file" << std::endl;
    while (!file.atEnd()) {
        std::cout << "Loading object from file" << std::endl;
        std::shared_ptr<Media> mediaPtr = Media::fromFile(file);
        if (mediaPtr) {
            mediaPtrs.push_back(mediaPtr);
        }
        if (mediaPtr) {
            std::cout << "Loaded object from file: " << *mediaPtr << std::endl;
        } else {
            std::cerr << "Failed to load object from file" << std::endl;
            break;
        }
    }
    return 0;
}