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

public:
    Shell(void);
    Shell(int maxCmdHistorySize);
    Shell(string prompt, int maxCmdHistorySize, queue<vector<string>> cmdHistoryList):shellPrompt(prompt), maxCmdHistorySize(maxCmdHistorySize), cmdHistoryList(cmdHistoryList)   {}
        
    void displayPrompt(void);
    string getInput(void);
    string handleUpArrow(void);
    string replaceInput(queue<vector<string>>&cmdList);
    void checkLength(string& s);
    vector<string> tokenise(string s, char delimiter);
    void printTokens(const vector<string> &input);
    int executeProgram(vector<string> args);
    void addCmdToHistory(vector<string> &cmd, queue<vector<string>> &cmdList);
    void mainWrapperAddCmdToHistory(vector<string> &cmd);	
};




#endif  // SHELL_H






