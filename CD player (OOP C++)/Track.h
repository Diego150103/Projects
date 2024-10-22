#ifndef TRACK_H
#define TRACK_H

#include <string>
    using std::string;

class Track {

public:

    Track(string title, string artist, int minutes, int second);
    string getTitle() const;
    string getArtist() const;
    int getMinutes() const;
    int getSeconds() const;

private:
    string title;
    string artist;
    int minutes;
    int seconds;
    
};

#endif