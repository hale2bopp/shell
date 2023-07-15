#include <iostream>
#include <unistd.h>
#include "shell.h"
#include <string>
#include <vector>

void checkLength(string &shellInput){
    if (shellInput.size() > MAX_INPUT){
        shellInput = shellInput.substr(0, MAX_INPUT);
    }
}

string getInput(void){ 
    string shellInput;
    getline(cin, shellInput);
    // limit length of terminal input
    checkLength(shellInput);
    return shellInput;
}

void convertStringToConstCharArr(vector<string> vec, char** str){
    std::vector<char*> pvec(vec.size());
    std::transform(vec.begin(), vec.end(), pvec.begin(), [](auto& str) {
        return &str[0];
    });
    str = pvec.data();
}

void executeProgram(string cmd, vector<string> args, string env){
    // args need to be NULL terminated 
    args.push_back(NULL);
    // env too 
    env.push_back(NULL);

    std::vector<char*> pvec(args.size());
    std::transform(args.begin(), args.end(), pvec.begin(), [](auto& str) {
        return &str[0];
    });
    execve(cmd.c_str(), pvec.data(), NULL);
}

vector<string> tokenise(string s, char delimiter){
    // ignore whitespaces 
    // end on enter 
    vector<string> tokens;
    bool wordBoundaryFlag = true;
    string temp;
    for(int i = 0; i < s[i]; i++){
        if (s[i] != ' '){
            if ((int) s[i] == BACKSPACE){
                temp.pop_back();
            } else {
                temp.push_back(s[i]);
            }
            wordBoundaryFlag = false;
        } else {
            // if previous state was false
            // this is a transition from finding a word
            // to finding a space 
            if (!wordBoundaryFlag){
                // add to tokens
                tokens.push_back(temp);
                // empty temp string
                temp = "";
                wordBoundaryFlag = true;
            }
        }

    }
    if (!wordBoundaryFlag){
        // if the last word was not a space,
        // need to add the temp at the end
        tokens.push_back(temp);
    }
    return tokens;
}

void printTokens(const vector<string> &input){
    for(int i = 0; i < input.size(); i++){
        cout << input[i] << " " ;
    }
    cout << "\n"; 
}


