/* \file shell.cpp 
 * \brief defines a simple shell
 *
 */
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "shell.h"
#include "redirection.h"
#include <termios.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <fcntl.h>
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
void CommandHistory::MainWrapperAddCmdToHistory(vector<string> &cmd){
    if (cmd.size() !=0){
        AddCmdToHistory(cmd, cmdHistoryList);
        currentHistoryIndex = cmdHistoryList.size();
        savedCurrentInput = "";
    }
}

/**
 * \brief Small helper to display prompt
 */
void Shell::DisplayPrompt(ostream& ofs){
    ofs << prompt;
}

/**
 * \brief Adds every entered command to history of commands.
 * Works as a FIFO, if number of commands exceeds a maximum
 * defined value, first value entered is discarded.
 * @param vector of strings describing a command to enter 
 * @param vector of vector of strings with current command history
 */
void CommandHistory::AddCmdToHistory(vector<string> &cmd, deque<vector<string>> &cmdList){
    if (cmdList.size() >= maxCmdHistorySize){
        cmdList.pop_front();
    } 
    cmdList.push_back(cmd);
}

/**
 * \brief Getter for current history index
 */
int CommandHistory::GetCurrentHistoryIndex(void){
    return currentHistoryIndex;
}

/**
 * \brief Getter for currently saved input string
 */
string CommandHistory::GetSavedCurrentInput(void){
    return savedCurrentInput;
}

/**
 * \brief Save currently entered string
 */
void CommandHistory::SaveCurrentEnteredString(string s){
    if ((SetSavedCurrentInputFlag) && (s.size()>0)){
        SetSavedCurrentInput(s);
        SetSavedCurrentInputFlag = false;
    }
}

/**
 * \brief Sets index in the command history vector
 */
void CommandHistory::SetCurrentHistoryIndex(int val){
    currentHistoryIndex = val;
}

/**
 * \brief Saves current input string before being replaced by up arrow
 */
void CommandHistory::SetSavedCurrentInput(string s){
    savedCurrentInput = s;
}

/**
 * \brief Handles up arrow press
 */
string Shell::handleUpArrow(string s, ostream& ofs){
    auto cmdHistory = GetCommandHistory();
    cmdHistory->SaveCurrentEnteredString(s);
    int chi = cmdHistory->GetCurrentHistoryIndex();
    if (chi > 0){
        chi--;        
        cmdHistory->SetCurrentHistoryIndex(chi);
    }
    return replaceInput(ofs);
}

/**
 * \brief Handles down arrow press
 */
string Shell::handleDownArrow(string s, ostream& ofs){
    auto cmdHistory = GetCommandHistory();
    cmdHistory->SaveCurrentEnteredString(s);
    int chi = cmdHistory->GetCurrentHistoryIndex();
    if (chi < cmdHistory->cmdHistoryList.size()){
        chi++;
        cmdHistory->SetCurrentHistoryIndex(chi);
    }
    return replaceInput(ofs);
}

/**
 * \brief Ensures that entered command is not larger than a max
 * input value. Truncates input string if larger.
 * @param input string 
 */
void Shell::CheckLength(string &shellInput){
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
string Shell::replaceInput(ostream& ofs){
    auto cmdHistory = GetCommandHistory();
    string shellInput;

    int chi = cmdHistory->GetCurrentHistoryIndex();
    int msize = cmdHistory->GetCmdHistorySize();
    if (chi == msize || ((chi == 0) && (msize==0))){
        string s = cmdHistory->GetSavedCurrentInput();
        ofs << s;
        return s;
    }
    int n = cmdHistory->cmdHistoryList[chi].size();
    for (int i = 0; i < n-1; i++){
        shellInput+= cmdHistory->cmdHistoryList[chi][i]+" " ;
    }
    shellInput += cmdHistory->cmdHistoryList[chi][n-1];
    ofs << shellInput;
    return shellInput;
}

/**
 * \brief get Shell input. Immediately handles backspace, arrows
 * passes to tokeniser 
 */
string Shell::GetInput(istream& ifs, ostream& ofs){ 
    auto cmdHistory = GetCommandHistory();
    cmdHistory->SetSavedCurrentInputFlag = true;

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
                    moveCursorToBackDisplayPrompt(ofs);
                    shellInput = handleUpArrow(shellInput, ofs);
                }
                if (c == (char) DOWN_ARROW){
                    moveCursorToBackDisplayPrompt(ofs);
                    shellInput = handleDownArrow(shellInput, ofs);
                } 
                break;
            case (char) DELETE: 
                eraseLastCharacter(shellInput, ofs);
                break;
            case (char)ENTER:
                break;
            default:
                shellInput += c; 
                break;
        }
    }
    // limit length of terminal input
    CheckLength(shellInput);
    return shellInput;
}

/**
 * \brief Executes currently entered command with arguments.
 * converts vector of strings into a format understandable
 * by execvp. 
 * @param vector of strings with command to be run
 */
int Shell::ExecuteProgram(vector<string>& cmd){
    vector<char *> vec_cp;
    vec_cp.reserve(cmd.size() + 1);
    for (auto s : cmd){
        vec_cp.push_back(strdup(s.c_str()));
    }
    vec_cp.push_back(NULL);
    return execvp(cmd[0].c_str(), const_cast<char* const*>(vec_cp.data()));
}


void Shell::tokenHelper(vector<string>& tokens, string& temp, bool& wordBoundary){
    if (!wordBoundary){
        // add to tokens
        tokens.push_back(temp);
        // empty temp string
        temp = "";
        wordBoundary = true;
    }
}

/**
 * \brief Tokenise entered input string from shell to extract command
 * and arguments
 * @param s input string from shell
 * @param delimiter character to split string into command and args. 
 *        in normal operation, this should be ' '.
 */
vector<string> Shell::Tokenise(string s, char delimiter){
    // ignore whitespaces 
    // end on enter 
    vector<string> tokens;
    bool wordBoundaryFlag = true;
    int numberOfRedirect = 0;
    bool multipleRedirect = false;
    string temp;
    for(int i = 0; i < s[i]; i++){
        switch(s[i]){
            case '>':
            case '<':
                if (multipleRedirect){
                    string str(numberOfRedirect+1,s[i]);
                    tokens.pop_back();
                    tokens.push_back(str);
                } else {
                    tokenHelper(tokens, temp, wordBoundaryFlag);
                    string str(1,s[i]);
                    tokens.push_back(str);
                }
                multipleRedirect = true;
                numberOfRedirect++;
                break;
            case ' ':
                // if previous state was false
                // this is a transition from finding a word
                // to finding a space
                tokenHelper(tokens, temp, wordBoundaryFlag);
                multipleRedirect = false;
                numberOfRedirect = 0;
                break;
            default: 
                temp.push_back(s[i]);
                wordBoundaryFlag = false;
                multipleRedirect = false;
                numberOfRedirect = 0;
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
void Shell::printTokens(const vector<string> &input, ostream& ofs){
    for(int i = 0; i < input.size(); i++){
        ofs << input[i];
        ofs << " ";
    }
    ofs << "\n"; 
}

/**
 * \brief Check for Redirection and split out command
 * @param input vector of strings
 */
//std::tuple <RedirectionParams, vector<string>> Shell::PostTokeniseProcessing(vector<string>& cmd){
RedirectionParams Shell::PostTokeniseProcessing(vector<string>& cmd){
    RedirectionParams redirParams = {0};
    redirParams.cmdEnd = cmd.size();
    ofstream outputFile;
    ifstream inputFile;
    for (int i = 0; i < cmd.size(); i++){
        if (cmd[i] == ">"){
                redirParams.redirectionType = OutputCreate;
                redirParams.outputFileIndex = i+1;
                redirParams.cmdEnd = i;
        } else if (cmd[i] == ">>"){
                redirParams.redirectionType = OutputAppend;
                redirParams.outputFileIndex = i+1;
                redirParams.cmdEnd = i;
        } else if (cmd[i] == "<") {
                    
                redirParams.redirectionType = Input;
                redirParams.inputFileIndex = i-1;
        } else if (cmd[i] == "<<"){
                redirParams.redirectionType = Input;
        } 
    }
    vector<string> inputCmd;
    redirParams.cmd.assign(cmd.begin()+redirParams.cmdStart, cmd.begin()+redirParams.cmdEnd);
    
    redirParams.outfilename = cmd[redirParams.outputFileIndex];
    return redirParams; 
}

/**
 * \brief Handle Redirection 
 * @param input command, redirectionType
 */
//void Shell::HandleRedirection(RedirectionParams& redirParams, vector<string>& cmd){
void Shell::HandleRedirection(RedirectionParams& redirParams){
    switch(redirParams.redirectionType){
        case (OutputCreate):
            {
                fflush(stdout);
                int newstdout = open(redirParams.outfilename.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                dup2(newstdout, fileno(stdout));
                close(newstdout);
            }
            break;
        case(OutputAppend):
            {
                fflush(stdout);
                int newstdout = open(redirParams.outfilename.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                dup2(newstdout, fileno(stdout));
                close(newstdout);
            }
            break;
        case(Input):
            break;
        default:
            break;

    }
}

/*
 * \brief erases whole line and moves cursor to beginning of line
 *
 */
void Shell::moveCursorToBackDisplayPrompt(ostream& ofs){
    ofs << eraseTillStartOfLine + moveCursorToBeginningOfLine;
    DisplayPrompt(ofs);
}


/*
 * \brief erase last character and move the cursor one
 * step back
 *
 */
void Shell::eraseLastCharacter(string&s, ostream& ofs){
    s.pop_back();
    for (int i = 0; i < 3; i++){
        ofs << moveCursorOneLeft << eraseCursorTillEndOfLine;
    }
}

/*
 * \brief Puts terminal in per character mode
 * so allows us to respond immediately as soon 
 * as a character is entered
 *
 */
void Shell::PutTerminalInPerCharMode(void){
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
void Shell::PutTerminalBackInNormalMode(void){
    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
