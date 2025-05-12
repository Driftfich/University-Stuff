#ifndef _AUDIO_H
#define _AUDIO_H

#include <vector>
#include <string>
#include "config.h"
#include "media.h"

class Audio : public Media {
    int duration; // in seconds
    std::string format;
    std::string bitrate; // in kbps
    std::string sample_rate; // in kHz
    std::string channels; // mono, stereo, etc.
    std::string codec; // e.g. MP3, AAC, FLAC, etc.
    std::string album; // album name
    std::vector<std::string> tracks;
}

#endif