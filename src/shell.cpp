/* \file shell.cpp 
 * \brief defines a simple shell
 *
 */
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

Shell::Shell(void){
    this->shellPrompt = prompt;
    this->maxCmdHistorySize = CMD_HISTORY_SIZE;
    this->cmdHistoryList = cmdHistory;
}

Shell::Shell(int maxCmdHistorySize): maxCmdHistorySize(maxCmdHistorySize){
    this->shellPrompt = prompt;
    this->cmdHistoryList = cmdHistory;
}

/**
 * \brief Adds every entered command to history of commands, wrapper
 * for main 
 * @param vector of strings describing a command to enter 
 */
void Shell::mainWrapperAddCmdToHistory(vector<string> &cmd){
    addCmdToHistory(cmd, cmdHistory);
}

/**
 * \brief Small helper to display prompt
 */
void Shell::displayPrompt(void){
    cout << prompt;
}


/**
 * \brief Adds every entered command to history of commands.
 * Works as a FIFO, if number of commands exceeds a maximum
 * defined value, first value entered is discarded.
 * @param vector of strings describing a command to enter 
 * @param vector of vector of strings with current command history
 */
void Shell::addCmdToHistory(vector<string> &cmd, queue<vector<string>> &cmdList){
    if (cmdList.size() >= CMD_HISTORY_SIZE){
        cmdList.pop();
    } 
    cmdList.push(cmd);
}


/**
 * \brief Handles up arrow press
 */
string Shell::handleUpArrow(void){
    return replaceInput(cmdHistory);
}

/**
 * \brief Ensures that entered command is not larger than a max
 * input value. Truncates input string if larger.
 * @param input string 
 */
void checkLength(string &shellInput){
    if (shellInput.size() > MAX_INPUT){
        shellInput = shellInput.substr(0, MAX_INPUT);
    }
}

/**
 * \brief Handles up arrow press. Replaces currently typed string 
 * with the string from command history. Allows for 
 * continued input after replacing currently typed string
 * with command from history.
 * @param vector of vector of strings (Command History)
 */
string Shell::replaceInput(queue<vector<string>>&cmdList){
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

/**
 * \brief get Shell input. Immediately handles backspace, arrows
 * passes to tokeniser 
 */
string Shell::getInput(void){ 
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

/**
 * \brief Executes currently entered command with arguments.
 * converts vector of strings into a format understandable
 * by execvp. 
 * @param vector of strings with command to be run
 */
int Shell::executeProgram(vector<string> cmd){
    std::vector<char *> vec_cp;
    vec_cp.reserve(cmd.size() + 1);
    for (auto s : cmd){
        vec_cp.push_back(strdup(s.c_str()));
    }
    vec_cp.push_back(NULL);
    return execvp(cmd[0].c_str(), const_cast<char* const*>(vec_cp.data()));           
}


/**
 * \brief Tokenise entered input string from shell to extract command
 * and arguments
 * @param s input string from shell
 * @param delimiter character to split string into command and args. 
 *        in normal operation, this should be ' '.
 */
vector<string> Shell::tokenise(string s, char delimiter){
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

/**
 * \brief Simple helper to print out a vector of strings 
 * @param input vector of strings to print
 */
void Shell::printTokens(const vector<string> &input){
    for(int i = 0; i < input.size(); i++){
        cout << input[i] << " " ;
    }
    cout << "\n"; 
}
