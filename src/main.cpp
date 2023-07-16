#include <iostream>
#include<sys/wait.h>
#include "shell.h"
string prompt = "penn-shredder# ";

string env = "PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games";
int main(void){
    while (1) {
        cout << prompt << " ";
        string shellInput = getInput();
        vector<string> tokens =  tokenise(shellInput, ' ');
        printTokens(tokens);
        int pid = fork(); 
        if (pid == 0) { 
            executeProgram(tokens[0], tokens, env);
        } else {
            wait(NULL);
        }
    }
    return 0;
}
