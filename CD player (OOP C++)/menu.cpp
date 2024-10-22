#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;

#include <string>
    using std::string;

#include <vector>

#include <sstream>
    using std::ostringstream;
#include "menu.h"

void displaySubMenu() {

    cout << "---------------------------------" << endl;
    cout << "             Welcome to CD player" << endl;
    cout << "---------------------------------" << endl;
    cout << "1. Turn On CD Player" << endl;
    cout << "2. Quit" << endl;
    cout << "---------------------------------" << endl;
    
}


//Function to display menu
void displayMenu() {

    cout << "---------------------------------" << endl;
    cout << "            Hi, CD Player options" << endl;
    cout << "---------------------------------" << endl;
    cout << "1) Load CD" << endl;
    cout << "2) Show contents from multi-changer" << endl;
    cout << "3) Play Random" << endl;
    cout << "4) Play Sequential" << endl;
    cout << "5) Next Track" <<endl;
    cout << "6) Previous Track" << endl;
    cout << "7) Show Current Track Info" << endl;
    cout << "8) Show CD Contents" << endl;
    cout << "9) Program mode" << endl;
    cout << "10) Select track from any CD" << endl;
    cout << "---------------------------------" << endl;
    cout << "              99) CLOSE CD PLAYER" << endl;
    cout << "---------------------------------" << endl;

}

void mainMenu(CDPlayer& player) {

    int choice1{0};

    displaySubMenu();

    cout << "Enter your choice: ";
    cin >> choice1;

    if (choice1 == 1) {

        player.turnOn();

    } 
    
    else if (choice1 == 2){

        exit(EXIT_SUCCESS);

    }
    
    //Display menu
    displayMenu();

    int choice{0};

    cout << "Enter your choice: ";
    cin >> choice;

    //When user enters option 99 the program ends.
    while(choice != 99){

        if (choice == 1){

            string fileName = "";

            cout << "Please eneter the name of the file." << endl;
            std::cin.ignore();
            std::getline(cin, fileName);

            player.readFromFile(fileName);

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        }

        else if (choice == 2){

            player.showMultiChanger();

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        }

        else if (choice == 3){

            player.playRandom();

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        }
        

        else if (choice == 4){

            player.playSequential();

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        }

        else if (choice == 5){

            player.nextTrack();

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        }

        else if (choice == 6){

            player.previousTrack();

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        } 

        else if (choice == 7){

            player.showCurrentTrackInfo();

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        }

        else if (choice == 8){

            player.showCDContents();

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        } 

        else if (choice == 9){

            player.programMode();

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        }

        else if (choice == 10){

            player.chooseCDTrack();

            displayMenu();

            cout << "Enter your choice: ";
            cin >> choice;

        } else {

            //If the user enters an invalid option continious the program
            std::cerr << "Invalid choice. Please try again." << endl;

            displayMenu();
            cout << "Enter your choice: ";
            cin >> choice;

        }
    }
}