/* \file shell.cpp 
 * \brief defines a simple shell
 *
 */
#include <iostream>
#include <unistd.h>
#include "shell.h"
#include <termios.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
string prompt = "penn-shredder# ";
static struct termios oldt, newt;



Shell::Shell(void){
    this->shellPrompt = prompt;
}

Shell::Shell(CommandHistory commandHistory): commandHistory(commandHistory){
    this->shellPrompt = prompt;
}

CommandHistory::CommandHistory(int maxCmdHistorySize): maxCmdHistorySize(maxCmdHistorySize){
}

/**
 * \brief Adds every entered command to history of commands, wrapper
 * for main 
 * @param vector of strings describing a command to enter 
 */
void CommandHistory::mainWrapperAddCmdToHistory(vector<string> &cmd){
    addCmdToHistory(cmd, cmdHistoryList);
    currentHistoryIndex = cmdHistoryList.size();
    savedCurrentInput = "";
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
void CommandHistory::addCmdToHistory(vector<string> &cmd, deque<vector<string>> &cmdList){
    if (cmdList.size() >= maxCmdHistorySize){
        cmdList.pop_front();
    } 
    cmdList.push_back(cmd);
}


int CommandHistory::getCurrentHistoryIndex(void){
    return currentHistoryIndex;
}

string CommandHistory::getSavedCurrentInput(void){
    return savedCurrentInput;
}

void CommandHistory::setCurrentHistoryIndex(int val){
    currentHistoryIndex = val;
}

void CommandHistory::setSavedCurrentInput(string s){
    savedCurrentInput = s;
}

/**
 * \brief Handles up arrow press
 */
string Shell::handleUpArrow(string s){
    auto cmdHistory = getCommandHistory();
    cmdHistory->setSavedCurrentInput(s);
    int chi = cmdHistory->getCurrentHistoryIndex();
    if (chi > 0){
        chi--;        
        cmdHistory->setCurrentHistoryIndex(chi);
    }
    return replaceInput();
}

/**
 * \brief Handles down arrow press
 */
string Shell::handleDownArrow(string s){
    auto cmdHistory = getCommandHistory();
    cmdHistory->setSavedCurrentInput(s);
    int chi = cmdHistory->getCurrentHistoryIndex();
    if (chi < cmdHistory->cmdHistoryList.size()){
        chi++;
        cmdHistory->setCurrentHistoryIndex(chi);
    }
    return replaceInput();
}

/**
 * \brief Ensures that entered command is not larger than a max
 * input value. Truncates input string if larger.
 * @param input string 
 */
void Shell::checkLength(string &shellInput){
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
string Shell::replaceInput(void){
    auto cmdHistory = getCommandHistory();
    string shellInput;
    if (cmdHistory->cmdHistoryList.size() == 0){
        return "";
    }

    int chi = cmdHistory->getCurrentHistoryIndex();
    if (chi == cmdHistory->cmdHistoryList.size()){
        return cmdHistory->getSavedCurrentInput();
    }
    int n = cmdHistory->cmdHistoryList[chi].size();
    for (int i = 0; i < n-1; i++){
        shellInput+= cmdHistory->cmdHistoryList[chi][i]+" " ;
    }
    shellInput += cmdHistory->cmdHistoryList[chi][n-1];
    cout << shellInput;
    return shellInput;
}

/**
 * \brief get Shell input. Immediately handles backspace, arrows
 * passes to tokeniser 
 */
string Shell::getInput(istream& ifs){ 
    string shellInput;
    char c = 0;
    while(c!=ENTER){
        ifs.get(c);
        switch(c){
            case (char)ESCAPE_SEQ:
                // get 2 more
                ifs.get(c);
                ifs.get(c);
                if (c == (char)UP_ARROW){
                    moveCursorToBackDisplayPrompt();
                    shellInput = handleUpArrow(shellInput);
                }
                if (c == (char) DOWN_ARROW){
                    moveCursorToBackDisplayPrompt();
                    shellInput = handleDownArrow(shellInput);
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


/*
 * \brief erases whole line and moves cursor to beginning of line
 *
 */
void Shell::moveCursorToBackDisplayPrompt(void){
    cout << eraseTillStartOfLine + moveCursorToBeginningOfLine;
    displayPrompt();
}


/*
 * \brief erase last character and move the cursor one
 * step back
 *
 */
void Shell::eraseLastCharacter(string&s){
    s.pop_back();
    for (int i = 0; i < 3; i++){
        cout << moveCursorOneLeft << eraseCursorTillEndOfLine;
    }
}

/*
 * \brief Puts terminal in per character mode
 * so allows us to respond immediately as soon 
 * as a character is entered
 *
 */
void Shell::putTerminalInPerCharMode(void){
    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);          

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
}

/*
 * \brief Restore normal terminal
 *
 */ 
void Shell::putTerminalBackInNormalMode(void){
    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
