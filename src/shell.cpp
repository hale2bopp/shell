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

void convertStringToConstCharArr(vector<string> vec, vector<const char*> ptrs){
//    std::vector<const char*> ptrs;
//    for (std::string const& str : args) {
//        ptrs.push_back(str.data());
//    }
//    std::vector<char*> pvec(vec.size());
//    std::transform(vec.begin(), vec.end(), pvec.begin(), [](auto& str) {
//        return &str[0];
//    });
//    str = pvec.data();
}

void executeProgram(string cmd, vector<string> argv, string env){
    // args need to be NULL terminated 
//    args.push_back(NULL);
    // env too 
//    env.push_back(NULL);

    std::vector<char *> vec_cp;
    vec_cp.reserve(argv.size() + 1);
    int i = 0;
    for (auto s : argv){
        vec_cp.push_back(strdup(s.c_str()));
        cout << "vec_cp: " << vec_cp[i] << endl;
        i++;
    }

    vec_cp.push_back(NULL);
//

//    const char **argv = new const char* [args.size()+1];   // extra room for program name and sentinel
//    for (int j = 0;  j < args.size()+1;  ++j)     // copy args
//        argv [j+1] = args[j] .c_str();

//    argv [args.size()+1] = NULL;  // end of arguments sentinel is NULL

    // cout << vec_cp << endl;
//    execve(cmd.c_str(), const_cast<char* const*>(vec_cp.data()), NULL);
    execvp(cmd.c_str(), const_cast<char* const*>(vec_cp.data()));
//    for (auto p : vec_cp)
//        free(p);
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


