#include "CD.h"

CD::CD(string title) : title{title} {

    //Intentionally left blank

}

void CD::addTrack(const Track& track) {

    tracks.push_back(track);

}

string CD::getTitle() const {
    
    return title;

}

const vector<Track>& CD::getTracks() const {

    return tracks;
    
}