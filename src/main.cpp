#include <iostream>
#include<sys/wait.h>
#include "shell.h"

string env = "PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games";
int main(void){
    while (1) {
        displayPrompt();
        string shellInput = getInput();
        vector<string> tokens = tokenise(shellInput, ' ');
        mainWrapperAddCmdToHistory(tokens);
        int pid = fork(); 
        if (pid == 0) { 
            executeProgram(tokens);
            perror("unable to execute");
        } else {
            wait(NULL);
        }
    }
    return 0;
}
