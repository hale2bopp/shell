/* \file shell.h
 * \brief Shell of OS
 *
 */

#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include<sys/wait.h>
#include <queue>
using std::queue;
using namespace std;

#define CMD_HISTORY_SIZE 10
#define MAX_INPUT 100
#define DELETE 127
#define ESCAPE_SEQ 27
#define UP_ARROW 65
#define ENTER 10
#define ASCII_BACKSPACE 8

class Shell{
private:
    string shellPrompt;
    int maxCmdHistorySize;
    queue<vector<string>> cmdHistoryList;

// potentilaly a list of features the shell has?
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

public:
    Shell(void);
    Shell(int maxCmdHistorySize);
    Shell(string prompt, int maxCmdHistorySize, queue<vector<string>> cmdHistoryList):shellPrompt(prompt), maxCmdHistorySize(maxCmdHistorySize), cmdHistoryList(cmdHistoryList)   {}

// Getters, Setters 
    int getCmdHistorySize(void){
        return this->cmdHistoryList.size();
    }

// Shell functionality    
    void displayPrompt(void);
    string getInput(std::istream& ifs);
    string handleUpArrow(void);
    string replaceInput(queue<vector<string>>&cmdList);
    void checkLength(string& s);
    vector<string> tokenise(string s, char delimiter);
    void printTokens(const vector<string> &input);
    int executeProgram(vector<string> args);
    void addCmdToHistory(vector<string> &cmd, queue<vector<string>> &cmdList);
    void mainWrapperAddCmdToHistory(vector<string> &cmd);	
    void moveCursorToBackDisplayPrompt(void);
    void eraseLastCharacter(string& s);
    void putTerminalInPerCharMode(void);
    void putTerminalBackInNormalMode(void);
};




#endif  // SHELL_H






