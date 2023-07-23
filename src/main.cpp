#include <iostream>
#include<sys/wait.h>
#include "shell.h"

string env = "PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games";
int main(void){
    Shell shell(CMD_HISTORY_SIZE);
    shell.putTerminalInPerCharMode();
    while (1) {
        shell.displayPrompt();
        string shellInput = shell.getInput(cin);
        vector<string> tokens = shell.tokenise(shellInput, ' ');
        shell.mainWrapperAddCmdToHistory(tokens);
        int pid = fork(); 
        if (pid == 0) { 
            shell.executeProgram(tokens);
            perror("unable to execute");
        } else {
            wait(NULL);
        }
    }
    shell.putTerminalBackInNormalMode();
    return 0;
}
