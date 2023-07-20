#include <iostream>
#include <termios.h>
#include "shell.h"
#include "termBehaviour.h"

static struct termios oldt, newt;
void moveCursorToBackDisplayPrompt(void){
    cout << eraseTillStartOfLine + moveCursorToBeginningOfLine;
    displayPrompt();
}

void eraseLastCharacter(string&s){
    s.pop_back();
    for (int i = 0; i < 3; i++){
        cout << moveCursorOneLeft << eraseCursorTillEndOfLine;
    }
}

// This allows us to process characters as soon as they are typed
void putTerminalInPerCharMode(void){
    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);          

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
}

// put it back into normal mode once I receive input
void putTerminalBackInNormalMode(void){
    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

