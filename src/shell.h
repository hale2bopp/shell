#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include<sys/wait.h>
#include <queue>
using std::queue;
using namespace std;
#define CMD_HISTORY_SIZE 10
#define MAX_INPUT 100
#define BACKSPACE_1 '^'
#define BACKSPACE_2 '?'

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

#endif  // SHELL_H






