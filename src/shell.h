/* \file shell.h
 * \brief Shell of OS
 *
 */

#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include <memory>
#include <tuple>
#include <queue>
#include "redirection.h"
#include "pipes.h"
#include "shellDriverInterface.h"
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
#define BACKGROUND '&'

enum PostTokeniseProcessingErr{
    PostTokeniseProcessingErrNone = 0,
    RedirErrWrongOrder = 1,
    BgErrWrongPosition = 2,
    BgErrDoubleBg = 3,
};



class Command{
private:
    Command *next;  
    bool m_isBackground;
    int pgid;
    string cmdName;
public:
    vector<int> cpid;
    Pipeline pipeline = {0};
    RedirectionParams redirParams = {0} ;

    void SetIsBackground(bool isBackground){
        m_isBackground = isBackground;
    }
    bool GetIsBackground(void){
        return m_isBackground;
    }
};

class CommandHistory{
private:
    int maxCmdHistorySize;
    string savedCurrentInput;
    int currentHistoryIndex = 0;
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
    string shellPrompt = "penn-shredder# ";
    CommandHistory commandHistory;
    ShellDriverInterface &shellDriverIntf;
    int cursorPosition = 0;
    int maxInputLength = MAX_INPUT;
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
    Shell(const string& prompt, const int cmdHistorySize, ShellDriverInterface& driverIntf): shellPrompt(prompt), commandHistory(cmdHistorySize), shellDriverIntf(driverIntf) {
        this->maxInputLength = MAX_INPUT;
    } 
// Getters, Setters 
    CommandHistory* GetCommandHistory(void){
        return &commandHistory;
    }
    int GetMaxInputLength(void){
        return maxInputLength;
    }
    int GetCursorPosition(void){ return cursorPosition; }
// Shell functionality    
    void DisplayPrompt(ostream& ofs);
    void CheckLength(string& s);
    string GetInput(istream& ifs, ostream& ofs);
    void PrintTokens(const vector<string> &input);
    int ExecuteProgram(const vector<string>& args);
    void PutTerminalInPerCharMode(void);
    void PutTerminalBackInNormalMode(void);
    vector<string> Tokenise(const string& s, const char &delimiter);
    PipesErr ParsePipes(vector<string> tokens, Command& command);
    PipesErr HandlePipes(Command& command);
    PostTokeniseProcessingErr PostTokeniseProcessing(RedirectionParams& redirParams, const vector<string> &cmd);
    void HandleRedirection(const RedirectionParams& redirParams);
    void printTokens(const vector<string> &input, ostream& ofs);   
    void shellRun(void);
};

// Non-member functions
std::unique_ptr<Shell> createShellWithDriver(const std::string& mainPrompt,  ShellDriverInterface &shellDriverIntf);
void shellRunWrapper(Shell& shell); 
#endif  // SHELL_H
