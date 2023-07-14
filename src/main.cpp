#include <iostream>
#include "shell.h"
string prompt = "penn-shredder# ";

int main(void){
    while (1) {
        cout << prompt << " ";
        string shellInput = getInput();
        vector<string> tokens =  tokenise(shellInput, ' ');
        printTokens(tokens);
        /*
        pid = fork(); 
        if (pid == 0) { 
            execve(); 
            perror(""); 
            exit(EXIT_FAILURE);
        }

        if (pid > 0) 
            wait();
        }
        */

    }
    return 0;
}
