#include "CDPlayer.h"
#include "menu.h"

int main() {

    CDPlayer* player1 = new CDPlayer();

    mainMenu(*player1);

    delete player1;

    return 0;
    
}
