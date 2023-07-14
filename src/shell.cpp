#include <iostream>
#include <unistd.h>
#include "shell.h"

void checkLength(string &shellInput){
    if (shellInput.size() > MAX_INPUT){
        shellInput = shellInput.substr(0, MAX_INPUT);
    }
}

string getInput(void){ 
    string shellInput;
    getline(cin, shellInput);
    // limit length of terminal input
    cout << "shellInput.size(): " << shellInput.size() << endl;
    checkLength(shellInput);
    return shellInput;
}


vector<string> tokenise(string s, char delimiter){
    // ignore whitespaces 
    // end on enter 
    vector<string> tokens;
    int i = 0;
    int delimIndex;
    do {
        delimIndex = s.find(delimiter);
        cout << "delimIndex: " << delimIndex << endl; 
        tokens.push_back(s.substr(i, delimIndex));
        printTokens(tokens);
        i = delimIndex+1;
        s = s.substr(i);
    } while (delimIndex != std::string::npos);

    return tokens;
}

void printTokens(const vector<string> &input){
    for(int i = 0; i < input.size(); i++){
        cout << input[i] << " " ;
    }
    cout << "\n"; 
}


