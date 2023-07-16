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
        cout << "pid: " << pid << endl;
        if (pid == 0) { 
            cout << "Hello from child: " <<tokens[0] << " " <<   pid << endl; 
            vector<string> args = {tokens.begin() + 1, tokens.end()}; 
            executeProgram(tokens[0], args, env);
        } else {
            cout << "hello from parent" << endl;
            wait(NULL);
            cout << "child has terminated" << endl;
        }
    }
    return 0;
}
