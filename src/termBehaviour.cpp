#include <iostream>
#include "shell.h"


static void moveCursorToBackDisplayPrompt(void){
    cout << eraseTillStartOfLine + moveCursorToBeginningOfLine;
    displayPrompt();
}


