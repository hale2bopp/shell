#include <iostream>
#include <unistd.h>
#include "shell.h"
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
string replaceInput(queue<vector<string>>&cmdList);
queue<vector<string>> cmdHistory;
void mainWrapperAddCmdToHistory(vector<string> &cmd){
    addCmdToHistory(cmd, cmdHistory);
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

void clearInput(void){
    cin.clear();
}

string replaceInput(queue<vector<string>>&cmdList){
//    clearInput();
    string shellInput;
    for (string s: cmdList.back()){
        shellInput+= s+" " ;
    }
    cout << "replaced string: " << shellInput << endl;
//    string moreShellInput;
//    getline(cin, moreShellInput);
    return shellInput;
}

string getInput(void){ 
    string shellInput;
    int c = 0;
    while(c!=10){
        c = getchar();
        switch(c){
            case 27:
                // get 2 more
                c = getchar();
                c = getchar();
                if (c == 65){
                    cout << " showing you most recent history" << endl;
                    shellInput = handleUpArrow();
                }
                break;

            case 10:
                break;
            default:
                shellInput += c; 
                break;
        }

        /*
        if (c == 27){
            // get 2 more 
            c = getchar();
            c = getchar();
            if (c == 65){
                cout << " showing you most recent history" << endl;
                shellInput = handleUpArrow();
            }
        } else {
            shellInput += c;
        }
        */
    }
    // limit length of terminal input
    checkLength(shellInput);
    return shellInput;
}

int executeProgram(vector<string> argv){
    std::vector<char *> vec_cp;
    vec_cp.reserve(argv.size() + 1);
    for (auto s : argv){
        vec_cp.push_back(strdup(s.c_str()));
        cout << "command: " << s << endl;
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


