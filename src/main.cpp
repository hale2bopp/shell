#include <iostream>
#include<sys/wait.h>
#include "shell.h"

string env = "PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games";
int main(void){
    CommandHistory cmdHistory(CMD_HISTORY_SIZE);
    Shell shell(cmdHistory);
    shell.PutTerminalInPerCharMode();
    while (1) {
        shell.DisplayPrompt(cout);
        string shellInput = shell.GetInput(cin, cout);
        vector<string> tokens = shell.Tokenise(shellInput, ' ');
        vector<string> cmd = shell.PostTokeniseProcessing(tokens);
        shell.GetCommandHistory()->MainWrapperAddCmdToHistory(cmd);
        int pid = fork(); 
        if (pid == 0) { 
            shell.ExecuteProgram(tokens);
            perror("unable to execute");
        } else {
            wait(NULL);
        }
    }
    shell.PutTerminalBackInNormalMode();
    return 0;
}
