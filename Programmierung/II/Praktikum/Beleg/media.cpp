#include <chrono>
#include <map>
#include <string>
#include <vector>
#include "media.h"
#include "person.h"
#include "config.h"
#include <iostream>

using namespace std;

int Media::setTitle(const string& title) {
    if (MIN_TITLE_LENGTH != -1 && MAX_TITLE_LENGTH != -1 && MIN_TITLE_LENGTH > MAX_TITLE_LENGTH) {
        cerr << "Error: MIN_TITLE_LENGTH > MAX_TITLE_LENGTH" << endl;
        return -1;
    }

    if (MIN_TITLE_LENGTH != -1 && title.length() < MIN_TITLE_LENGTH) {
        cerr << "Error: Title is too short" << endl;
        return -1;
    }

    if (MAX_TITLE_LENGTH != -1 && title.length() > MAX_TITLE_LENGTH) {
        cerr << "Warning: Title is too long, truncating to " << MAX_TITLE_LENGTH << " characters" << endl;
        this->title = title.substr(0, MAX_TITLE_LENGTH);
    }
    else {
        this->title = title;
    }
    return 0;
}

int Media::setPublicationDate(const std::chrono::year_month_day publication_date) {
    this->publication_date = publication_date;
    return 0;
}

int Media::setArtistIds(const std::vector<int>& artist_ids) {
    if (artist_ids.size() > MAX_ARTIST_COUNT) {
        cerr << "Warning: Too many artists, truncating to " << MAX_ARTIST_COUNT << " artists" << endl;
        this->artist_ids = std::vector<int>(artist_ids.begin(), artist_ids.begin() + MAX_ARTIST_COUNT);
    }
    this->artist_ids = artist_ids;
    return 0;
}

int Media::setPublisher(const string& publisher) {
    if (MAX_PUBLISHER_LENGTH != -1 && publisher.length() > MAX_PUBLISHER_LENGTH) {
        cerr << "Warning: Publisher is too long, truncating to " << MAX_PUBLISHER_LENGTH << " characters" << endl;
        this->publisher = publisher.substr(0, MAX_PUBLISHER_LENGTH);
    }
    else {
        this->publisher = publisher;
    }
    return 0;
}

int Media::setDescription(const string& description) {
    if (MAX_DESCRIPTION_LENGTH != -1 && description.length() > MAX_DESCRIPTION_LENGTH) {
        cerr << "Warning: Description is too long, turncating to " << MAX_DESCRIPTION_LENGTH << " characters" << endl;
        this->description = description.substr(0, MAX_DESCRIPTION_LENGTH);
    }

    else {
        this->description = description;
    }

    return 0;
}

int Media::setGenre(const string& genre) {
    if (MAX_GENRE_LENGTH != -1 && genre.length() > MAX_GENRE_LENGTH) {
        cerr << "Warning: Genre is too long, truncating to " << MAX_GENRE_LENGTH << " characters" << endl;
        this->genre = genre.substr(0, MAX_GENRE_LENGTH);
    }
    else {
        this->genre = genre;
    }
    return 0;
}

int Media::setLanguages(const vector<string>& languages) {
    this->languages.clear();
    size_t num_languages = languages.size();

    if (MAX_LANGUAGE_COUNT != -1 && languages.size() > MAX_LANGUAGE_COUNT) {
        cerr << "Warning: Too many languages provided (" << languages.size()
             << "), processing only the first " << MAX_LANGUAGE_COUNT << " languages." << endl;
        num_languages = MAX_LANGUAGE_COUNT;
    }

    for (size_t i = 0; i < num_languages; ++i) {
        const string& lang_str = languages[i];
        if (MAX_LANGUAGE_LENGTH != -1 && lang_str.length() > static_cast<size_t>(MAX_LANGUAGE_LENGTH)) {
            cerr << "Warning: Language string '" << lang_str << "' is too long (length " << lang_str.length()
                 << "), truncating to " << MAX_LANGUAGE_LENGTH << " characters." << endl;
            this->languages.push_back(lang_str.substr(0, static_cast<size_t>(MAX_LANGUAGE_LENGTH)));
        } else {
            this->languages.push_back(lang_str);
        }
    }
    return 0;
}

int Media::setMetadata(const map<string, variant<int, float, string>>) {
    this->metadata.clear(); // Clear existing metadata

    size_t processed_count = 0;
    for (const auto& pair : new_metadata) {
        if (MAX_METADATA_COUNT != -1 && processed_count >= static_cast<size_t>(MAX_METADATA_COUNT)) {
            cerr << "Warning: Too many metadata entries provided (input size: " << new_metadata.size()
                 << "), processing only the first " << MAX_METADATA_COUNT << " metadata entries." << endl;
            break; // Stop processing further entries
        }

        const string& key = pair.first;
        variant<int, float, string> value_to_store = pair.second; // Make a copy to potentially modify

        // Check if the variant holds a string and if its length needs truncation
        if (std::holds_alternative<string>(value_to_store)) {
            string& str_val = std::get<string>(value_to_store); // Get a reference to the string inside the variant
            if (MAX_METADATA_LENGTH != -1 && str_val.length() > static_cast<size_t>(MAX_METADATA_LENGTH)) {
                cerr << "Warning: Metadata string value for key '" << key << "' is too long (length "
                     << str_val.length() << "), truncating to " << MAX_METADATA_LENGTH << " characters." << endl;
                // Update the string within the variant by assigning the truncated string
                value_to_store = str_val.substr(0, static_cast<size_t>(MAX_METADATA_LENGTH));
            }
        }
        // For int or float, no length check is applicable here.

        this->metadata[key] = value_to_store;
        processed_count++;
    }
    return 0;
}