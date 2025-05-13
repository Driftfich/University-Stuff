#include <iostream>
#include <string>
#include <vector>
#include "media.h"
#include "artist.h"

using namespace std;

int Artist::setArtistType(const string& artist_type) {
    this->artist_type = artist_type;
    return 0;
}

int Artist::setMediaIds(const vector<int>& media_ids) {
    this->media_ids = media_ids;
    return 0;
}