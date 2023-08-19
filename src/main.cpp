#include <iostream>
#include<sys/wait.h>
#include "shell.h"
#include <string.h>
const char* err_msg = "unable to execute";
string env = "PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games";
int main(void){
    CommandHistory cmdHistory(CMD_HISTORY_SIZE);
    Shell shell(cmdHistory);
    shell.PutTerminalInPerCharMode();
    while (1) {
        shell.DisplayPrompt(cout);
        string shellInput = shell.GetInput(cin, cout);
        vector<string> tokens = shell.Tokenise(shellInput, ' ');
        shell.GetCommandHistory()->MainWrapperAddCmdToHistory(tokens);
        fflush(stdout);
        int pid = fork(); 
        if (pid == 0) {
//            RedirectionParams redirParams;
//            vector<string> cmd;
            //tie(redirParams, cmd) = shell.PostTokeniseProcessing(tokens);
            RedirectionParams redirParams = shell.PostTokeniseProcessing(tokens);
            //shell.HandleRedirection(redirParams, cmd);
            shell.HandleRedirection(redirParams);
//            shell.ExecuteProgram(cmd)//
            shell.ExecuteProgram(redirParams.cmd);
            perror("unable to execute");
        } else {
            wait(NULL);
        }
    }
    shell.PutTerminalBackInNormalMode();
    return 0;
}
