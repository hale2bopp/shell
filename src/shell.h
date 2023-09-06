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
#include "pipes.h"
using std::deque;
using namespace std;

#define CMD_HISTORY_SIZE 10
#define MAX_INPUT 100
#define DELETE 127
#define ESCAPE_SEQ 27
#define UP_ARROW 65
#define DOWN_ARROW 66
#define RIGHT_ARROW 67
#define LEFT_ARROW 68
#define ENTER 10
#define ASCII_BACKSPACE 8

class CommandHistory{
private:
    int maxCmdHistorySize;
    string savedCurrentInput;
    int currentHistoryIndex;

public: 
    bool SetSavedCurrentInputFlag= true;
    CommandHistory(){}
    CommandHistory(int maxCmdHistorySize);
    deque<string> cmdHistoryList;   
    int GetCmdHistorySize(void){
        return this->cmdHistoryList.size();
    }
    int GetCurrentHistoryIndex(void);
    string GetSavedCurrentInput(void);
    void SetCurrentHistoryIndex(const int& val);
    void SaveCurrentEnteredString(const string& s);
    void SetSavedCurrentInput(const string& val);
    void AddCmdToHistory(const string &cmd, deque<string> &cmdList);
    void MainWrapperAddCmdToHistory(const string &cmd);	
};

class Shell{
private:
    string shellPrompt;
    CommandHistory commandHistory;
    int cursorPosition = 0;
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
    const string insertCharacterStr = "\E[4h";  
    const string moveCursorTillStart = "\u001b[1000D";
    // Functions
    string handleUpArrow(const string& s, ostream& ofs);
    string handleDownArrow(const string& s, ostream& ofs);
    string handleLeftArrow(const string& s, const char& c, ostream& ofs);
    string handleRightArrow(const string& s, const char& c, ostream& ofs);
    string replaceInput(ostream& ofs);
    void moveCursorToBackDisplayPrompt(ostream& ofs);
    void eraseCharacter(string& s, ostream& ofs);
    void cursorOneRight(string& s, ostream& ofs);
    void cursorOneLeft(string& s, ostream& ofs);
    void moveCursorLeftRight(string& s, ostream& ofs);
    void tokenHelper(vector<string>& tokens, string& temp, bool& wordBoundary);
    void detectDoubleChar(const char& charDetect, int& numChar, vector<string>& tokens, string& temp, bool& wordBoundaryFlag, bool& multipleChar);

    void setCmdEnd(RedirectionParams& redirParams, const int& index);
    void incrementCursorPosition(const string&s, int& cursor);
    void decrementCursorPosition(const string&s, int& cursor);
    void insertCharacter(string& s, const char&c, int& cursor, ostream& ofs);
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
    int ExecuteProgram(const vector<string>& args);
    void PutTerminalInPerCharMode(void);
    void PutTerminalBackInNormalMode(void);
    vector<string> Tokenise(const string& s, const char &delimiter);
    PipesErr ParsePipes(vector<string> tokens, Pipeline& pipeline);
    PipesErr HandlePipes(const Pipeline& pipeline, RedirectionParams& redirPrams);
    RedirErr PostTokeniseProcessing(RedirectionParams& redirParams, const vector<string>& cmd);
    void HandleRedirection(const RedirectionParams& redirParams);
    void printTokens(const vector<string> &input, ostream& ofs);   
};


#endif  // SHELL_H
