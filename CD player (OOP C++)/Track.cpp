#include "Track.h"

Track::Track(string title, string artist, int minutes, int seconds) : title{title}, artist{artist}, minutes{minutes}, seconds{seconds} {

    //Intentionally left blank

}

string Track::getTitle() const {

    return title;

}

string Track::getArtist() const {

    return artist;

}

int Track::getMinutes() const {

    return minutes;
    
}

int Track::getSeconds() const{

    return seconds;

}