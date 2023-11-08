#include <iostream>
#include<sys/wait.h>
#include "shell.h"
#include "shellSignal.h"
#include <string.h>
const char* err_msg = "unable to execute";
string env = "PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games";

string mainPrompt = "penn-shredder# ";
int Shell::shellRun(void){
    registerSignals();
    ShellDriver shellDriver;
    Shell shell(mainPrompt, CMD_HISTORY_SIZE, shellDriver);
    shell.PutTerminalInPerCharMode();
    while (1) {
        shell.DisplayPrompt(cout);
        string shellInput = shell.GetInput(cin, cout);
        vector<string> tokens = shell.Tokenise(shellInput, ' ');
        shell.GetCommandHistory()->MainWrapperAddCmdToHistory(shellInput);
        fflush(stdout);
        Command command;
        PipesErr pipesErr = shell.ParsePipes(tokens, command);
        if (pipesErr!=PipesErrNone){
            perror("error in parsing pipes");
            continue;
        }
        pipesErr = shell.HandlePipes(command);
        if (pipesErr!=PipesErrNone){
            perror("error in handling pipes");
            continue;
        } 
    }
    shell.PutTerminalBackInNormalMode();
    return 0;
}
