#ifndef CDPLAYER_H
#define CDPLAYER_H

#include <vector>
#include "CD.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <ctime>

class CDPlayer {

public:
    CDPlayer();
    ~CDPlayer();
    void turnOn();
    void playRandom();
    void playSequential();
    void nextTrack();
    void previousTrack();
    void showCurrentTrackInfo();
    void showCDContents();
    const CD* getCurrentCD() const;
    bool getIsOn() const; 
    void readFromFile(const std::string& filename);
    void programMode();
    void chooseCDTrack();
    void showMultiChanger();

private:
    bool isOn;
    bool isMultiChangerLoaded;
    CD* currentCD;
    std::vector<CD*> multiChanger;
    int currentTrackIndex;
};

#endif // CDPLAYER_H

