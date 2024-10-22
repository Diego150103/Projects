#ifndef CD_H
#define CD_H

#include <string>
    using std::string;

#include <vector>
    using std::vector;

#include "Track.h"

class CD {

public:
    CD(string title);
    void addTrack(const Track& track);
    string getTitle() const;
    const vector<Track>& getTracks() const;

private:
    string title;
    vector<Track> tracks;
    
};

#endif