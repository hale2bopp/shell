#include <iostream>
#include <unistd.h>
#include "shell.h"
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>

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

void executeProgram(string cmd, vector<string> argv, string env){
    std::vector<char *> vec_cp;
    vec_cp.reserve(argv.size() + 1);
    for (auto s : argv){
        vec_cp.push_back(strdup(s.c_str()));
    }
    vec_cp.push_back(NULL);
    execvp(cmd.c_str(), const_cast<char* const*>(vec_cp.data()));
           
}

vector<string> tokenise(string s, char delimiter){
    // ignore whitespaces 
    // end on enter 
    vector<string> tokens;
    bool wordBoundaryFlag = true;
    string temp;
    for(int i = 0; i < s[i]; i++){
        switch(s[i]){
            case ' ':
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
                break;
            case char(72):
                cout << "UP Arrow" << endl; 
                break;
            case BACKSPACE:
                temp.pop_back();
                wordBoundaryFlag = false;
                break;
            default: 
                temp.push_back(s[i]);
                wordBoundaryFlag = false;
                break;

        }
/*
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
*/
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


