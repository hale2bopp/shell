#include <iostream>
#include <unistd.h>
#include "shell.h"
#include "termBehaviour.h"
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
string prompt = "penn-shredder# ";
queue<vector<string>> cmdHistory;

void mainWrapperAddCmdToHistory(vector<string> &cmd){
    addCmdToHistory(cmd, cmdHistory);
}

void displayPrompt(void){
    cout << prompt;
}



void addCmdToHistory(vector<string> &cmd, queue<vector<string>> &cmdList){
    if (cmdList.size() >= CMD_HISTORY_SIZE){
        cmdList.pop();
    } 
    cmdList.push(cmd);
}

string handleUpArrow(void){
    return replaceInput(cmdHistory);
}

void checkLength(string &shellInput){
    if (shellInput.size() > MAX_INPUT){
        shellInput = shellInput.substr(0, MAX_INPUT);
    }
}

string replaceInput(queue<vector<string>>&cmdList){
    string shellInput;
    if (cmdList.size() == 0){
        return "";
    }
    for (string s: cmdList.back()){
        shellInput+= s+" " ;
    }
    cout << shellInput;
    return shellInput;
}

string getInput(void){ 
    string shellInput;
    char c = 0;
    putTerminalInPerCharMode();
    while(c!=ENTER){
        c = getchar();

        switch(c){
            case (char)ESCAPE_SEQ:
                // get 2 more
                c = getchar();
                c = getchar();
                if (c == (char)UP_ARROW){
                    moveCursorToBackDisplayPrompt();
                    shellInput = handleUpArrow();
                } 
                break;
            case (char) DELETE: 
                eraseLastCharacter(shellInput);               
                break;
            case (char)ENTER:
                break;
            default:
                shellInput += c; 
                break;
        }
    }
    // limit length of terminal input
    checkLength(shellInput);
    putTerminalBackInNormalMode();
    return shellInput;
}

int executeProgram(vector<string> argv){
    std::vector<char *> vec_cp;
    vec_cp.reserve(argv.size() + 1);
    for (auto s : argv){
        vec_cp.push_back(strdup(s.c_str()));
    }
    vec_cp.push_back(NULL);
    return execvp(argv[0].c_str(), const_cast<char* const*>(vec_cp.data()));           
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
            default: 
                temp.push_back(s[i]);
                wordBoundaryFlag = false;
                break;

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


