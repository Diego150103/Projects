#include "CDPlayer.h"

CDPlayer::CDPlayer() : isOn{false}, isMultiChangerLoaded{false}, currentCD{nullptr}, currentTrackIndex{0} {

    //Intentionally left blank
    
}

CDPlayer::~CDPlayer() {

    delete currentCD;

    for (auto cd : multiChanger) {
        delete cd;
    }

    std::cout << "CD Player deleted." << std::endl;

}

void CDPlayer::turnOn() {

    isOn = true;
    std::cout << "CD Player is on." << std::endl;

}

void CDPlayer::playRandom() {

    if (!isOn) {

        std::cout << "CD Player is off. Turn it on first." << std::endl;
        return;

    }

    else if (!isMultiChangerLoaded && !currentCD) {

        std::cout << "No CD loaded. Load a CD first." << std::endl;
        return;

    }

    else if (isMultiChangerLoaded) {

        srand(static_cast<unsigned int>(time(0)));

        int randomCDIndex = rand() % multiChanger.size();
        currentCD = multiChanger[randomCDIndex];

    }

    srand(static_cast<unsigned int>(time(0)));

    int randomTrackIndex = rand() % currentCD->getTracks().size();
    currentTrackIndex = randomTrackIndex;

    showCurrentTrackInfo();

}

void CDPlayer::playSequential() {

    if (!isOn) {

        std::cout << "CD Player is off. Turn it on first." << std::endl;

    }

    else if (!isMultiChangerLoaded && !currentCD) {

        std::cout << "No CD loaded. Load a CD first." << std::endl;

    }

    currentTrackIndex = (currentTrackIndex + 1) % currentCD->getTracks().size();

    showCurrentTrackInfo();

}

void CDPlayer::nextTrack() {

    playSequential();

}

void CDPlayer::previousTrack() {

    if (!isOn) {

        std::cout << "CD Player is off. Turn it on first." << std::endl;

    }

    else if (!isMultiChangerLoaded && !currentCD) {

        std::cout << "No CD loaded. Load a CD first." << std::endl;

    }

    currentTrackIndex = (currentTrackIndex - 1 + currentCD->getTracks().size()) % currentCD->getTracks().size();

    showCurrentTrackInfo();

}

void CDPlayer::showCurrentTrackInfo() {

    if (!isOn) {

        std::cout << "CD Player is off. Turn it on first." << std::endl;
  
    }

    else if (!isMultiChangerLoaded && !currentCD) {

        std::cout << "No CD loaded. Load a CD first." << std::endl;
    
    }

    const Track& currentTrack = currentCD->getTracks()[currentTrackIndex];

    std::cout << "Current CD: " << currentCD->getTitle() << std::endl;
    std::cout << "Current Track: " << currentTrack.getTitle() << std::endl;
    std::cout << "Artist: " << currentTrack.getArtist() << std::endl;
    std::cout << "Duration: " << currentTrack.getMinutes() << ":" << currentTrack.getSeconds() << std::endl;
}

void CDPlayer::showCDContents() {

    if (!isOn) {

        std::cout << "CD Player is off. Turn it on first." << std::endl;

    }

    else if (!currentCD) {

        std::cout << "No CD loaded. Load a CD first." << std::endl;

    }

    std::cout << "Current CD Contents:" << std::endl;

    // Retrieve the tracks of the current CD
    const std::vector<Track>& tracks = currentCD->getTracks();

    // Display information about each track
    for (const auto& track : tracks) {

        std::cout << "- " << track.getTitle() << " (" << track.getMinutes() << ":" << track.getSeconds() << ")" << std::endl;

    }
}


const CD* CDPlayer::getCurrentCD() const {

    return currentCD;
}

bool CDPlayer::getIsOn() const{

    return isOn;

}

void CDPlayer::readFromFile(const std::string& filename) {

    std::ifstream file(filename);

    if (!file.is_open()) {

        std::cerr << "Error: Could not open file " << filename << std::endl;

    }

    std::string line = "";

    while (std::getline(file, line)) {

        std::istringstream iss(line);
        std::string cdTitle, trackTitle, artist, duration;

        if (std::getline(iss, cdTitle, '|') &&
            std::getline(iss, trackTitle, '|') &&
            std::getline(iss, artist, '|') &&
            std::getline(iss, duration, '|')) {

            // Extract minutes and seconds from the duration
            int minutes; 
            int seconds;
            char separator;

            std::istringstream durationStream(duration);

            if (durationStream >> minutes >> separator >> seconds && separator == ':') {

                Track track(trackTitle, artist, minutes, seconds);

                // Check if a CD with the given title already exists in multiChanger
                auto it = std::find_if(multiChanger.begin(), multiChanger.end(),
                                       [cdTitle](const CD* cd) { return cd->getTitle() == cdTitle; });

                if (it == multiChanger.end()) {

                    // If CD does not exist, create a new CD and add it to multiChanger
                    CD* cd = new CD(cdTitle);
                    cd->addTrack(track);
                    multiChanger.push_back(cd);

                } else {

                    // If CD already exists, add the track to the existing CD
                    (*it)->addTrack(track);

                }

            } else {

                std::cerr << "Error: Invalid duration format in line: " << line << std::endl;

            }

        } else {

            std::cerr << "Error: Insufficient tokens in line: " << line << std::endl;

        }
    }

    isMultiChangerLoaded = true;

    file.close();

}

void CDPlayer::programMode() {

    if (!isOn) {

        std::cout << "CD Player is off. Turn it on first." << std::endl;

    }

    else if (!isMultiChangerLoaded) {

        std::cout << "Multi-changer is not loaded. Load CDs first." << std::endl;
    
    }

    std::vector<const Track*> programTracks;

    int selectedTrackCount {0};

    std::cout << "Program Mode: Select up to 3 tracks. Enter track numbers (0 to exit):" << std::endl;

    while (selectedTrackCount < 3) {

        showMultiChanger(); // Show contents of all CDs in the multi-changer

        int cdIndex, trackNumber;
        std::cout << "Enter CD number (0 to exit): ";
        std::cin >> cdIndex;

        if (cdIndex == 0) {

            break; 

        }

        else if (cdIndex >= 1 && cdIndex <= multiChanger.size()) {

            const CD* selectedCD = multiChanger[cdIndex - 1];

            std::cout << "Selected CD: " << selectedCD->getTitle() << std::endl;
            std::cout << "Tracks of " << selectedCD->getTitle() << ":" << std::endl;

            // Display tracks of the selected CD
            const std::vector<Track>& tracks = selectedCD->getTracks();
            for (size_t i = 0; i < tracks.size(); ++i) {
                std::cout << "  " << i + 1 << ") " << tracks[i].getTitle()
                          << " (" << tracks[i].getMinutes() << ":" << tracks[i].getSeconds() << ")" << std::endl;
            }

            std::cout << "Enter track number (0 to exit): ";
            std::cin >> trackNumber;

            if (trackNumber == 0) {
                break; 
            }

            else if (trackNumber >= 1 && trackNumber <= tracks.size()) {

                // Valid track number, add the track to the program
                const Track& selectedTrack = tracks[trackNumber - 1];
                programTracks.push_back(&selectedTrack);
                std::cout << "Track added: " << selectedTrack.getTitle() << std::endl;
                selectedTrackCount++;

            } else {

                std::cout << "Invalid track number. Please try again." << std::endl;

            }

        } else {

            std::cout << "Invalid CD number. Please try again." << std::endl;

        }
    }

    // Play selected tracks in sequence
    for (const auto& track : programTracks) {

        std::cout << "Now playing: " << track->getTitle() << std::endl;

    }

    std::cout << "Program Mode ended." << std::endl;
}


void CDPlayer::chooseCDTrack() {

    if (!isOn) {

        std::cout << "CD Player is off. Turn it on first." << std::endl;
        
    }

    if (!isMultiChangerLoaded) {

        std::cout << "Multi-changer is not loaded. Load CDs first." << std::endl;
        
    }

    // Display available CDs in the multi-changer
    std::cout << "Available CDs in Multi-Changer:" << std::endl;

    for (size_t i = 0; i < multiChanger.size(); ++i) {

        std::cout << i + 1 << ") " << multiChanger[i]->getTitle() << std::endl;

    }

    // Choose a CD
    size_t chosenCDIndex;
    std::cout << "Choose a CD (enter CD number): ";
    std::cin >> chosenCDIndex;

    if (chosenCDIndex >= 1 && chosenCDIndex <= multiChanger.size()) {

        // Valid CD choice, play tracks from the chosen CD
        const CD* chosenCD = multiChanger[chosenCDIndex - 1];

        // Display tracks of the chosen CD
        std::cout << "Tracks of " << chosenCD->getTitle() << ":" << std::endl;

        const std::vector<Track>& tracks = chosenCD->getTracks();

        for (size_t i = 0; i < tracks.size(); ++i) {

            std::cout << i + 1 << ") " << tracks[i].getTitle() << std::endl;

        }

        // Choose a track from the chosen CD
        size_t chosenTrackIndex;

        std::cout << "Choose a track (enter track number): ";
        std::cin >> chosenTrackIndex;

        if (chosenTrackIndex >= 1 && chosenTrackIndex <= tracks.size()) {

            // Valid track choice, play the chosen track
            const Track& chosenTrack = tracks[chosenTrackIndex - 1];
            std::cout << "Now playing: " << chosenTrack.getTitle() << std::endl;

        } else {

            std::cout << "Invalid track number. Operation canceled." << std::endl;

        }
    } else {

        std::cout << "Invalid CD number. Operation canceled." << std::endl;

    }
}

void CDPlayer::showMultiChanger() {

    if (!isOn) {

        std::cout << "CD Player is off. Turn it on first." << std::endl;


    }

    else if (!isMultiChangerLoaded) {

        std::cout << "Multi-changer is not loaded. Load CDs first." << std::endl;
        
    }

    std::cout << "Contents of Multi-Changer:" << std::endl;

    for (const auto& cd : multiChanger) {

        std::cout << "CD: " << cd->getTitle() << std::endl;
        const std::vector<Track>& tracks = cd->getTracks();

        for (size_t i = 0; i < tracks.size(); ++i) {

            std::cout << "  " << i + 1 << ") " << tracks[i].getTitle()
                      << " (" << tracks[i].getMinutes() << ":" << tracks[i].getSeconds() << ")" << std::endl;

        }
    }
}

