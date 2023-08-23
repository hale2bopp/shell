/* \file shell.h
 * \brief Shell of OS
 *
 */

#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include<sys/wait.h>
#include <tuple>
#include <queue>
#include "redirection.h"
using std::deque;
using namespace std;

#define CMD_HISTORY_SIZE 10
#define MAX_INPUT 100
#define DELETE 127
#define ESCAPE_SEQ 27
#define UP_ARROW 65
#define DOWN_ARROW 66
#define ENTER 10
#define ASCII_BACKSPACE 8

class CommandHistory{
private:
    int maxCmdHistorySize = 10;
    string savedCurrentInput = "";
    int currentHistoryIndex = 0;

public: 
    bool SetSavedCurrentInputFlag= true;
    CommandHistory(){}
    CommandHistory(int maxCmdHistorySize);
    CommandHistory(const CommandHistory& cmdHistory)
    {
        this->maxCmdHistorySize = cmdHistory.maxCmdHistorySize;
        this->savedCurrentInput = cmdHistory.savedCurrentInput;
        this->currentHistoryIndex = cmdHistory.currentHistoryIndex;
    }
    deque<string> cmdHistoryList;   
    int GetCmdHistorySize(void){
        return this->cmdHistoryList.size();
    }
    int GetCurrentHistoryIndex(void);
    string GetSavedCurrentInput(void);
    void SetCurrentHistoryIndex(int val);
    void SaveCurrentEnteredString(string s);
    void SetSavedCurrentInput(string val);
    void AddCmdToHistory(string &cmd, deque<string> &cmdList);
    void MainWrapperAddCmdToHistory(string &cmd);	
};

class Shell{
private:
    string shellPrompt;
    CommandHistory commandHistory;

// potentially a list of features the shell has?
    bool upArrow;
    bool backSpace;
    bool downArrow;
    bool tabComplete;

    // Terminal string constants
    const string eraseCursorTillEndOfLine = "\33[K";
    const string moveCursorOneLeft = "\33[D";
    const string moveCursorOneRight = "\33[C";
    const string eraseTillStartOfLine = "\33[2K";
    const string moveCursorToBeginningOfLine = "\r";
    
    // Functions
    string handleUpArrow(string s, ostream& ofs);
    string handleDownArrow(string s, ostream& ofs);
    string replaceInput(ostream& ofs);
    void moveCursorToBackDisplayPrompt(ostream& ofs);
    void eraseLastCharacter(string& s, ostream& ofs);
    void tokenHelper(vector<string>& tokens, string& temp, bool& wordBoundary);
    void setCmdEnd(RedirectionParams& redirParams, int index);
public:
    Shell(void);
    Shell(CommandHistory& cmdHistory);
    Shell(const string& prompt):shellPrompt(prompt){}
    Shell(const string& prompt, const int cmdHistorySize): shellPrompt(prompt), commandHistory(cmdHistorySize){} 
    Shell(const string& prompt, CommandHistory& cmdHistory): shellPrompt(prompt), commandHistory(cmdHistory){}
// Getters, Setters 
    CommandHistory* GetCommandHistory(void){
        return &commandHistory;
    }
// Shell functionality    
    void DisplayPrompt(ostream& ofs);
    void CheckLength(string& s);
    string GetInput(istream& ifs, ostream& ofs);
    void PrintTokens(const vector<string> &input);
    int ExecuteProgram(vector<string>& args);
    void PutTerminalInPerCharMode(void);
    void PutTerminalBackInNormalMode(void);
    vector<string> Tokenise(string s, char delimiter);
    RedirErr PostTokeniseProcessing(RedirectionParams& redirParams, vector<string>& cmd);
    void HandleRedirection(RedirectionParams& redirParams);
    void printTokens(const vector<string> &input, ostream& ofs);   
};


#endif  // SHELL_H
