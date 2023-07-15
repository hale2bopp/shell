#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include<sys/wait.h>
using namespace std;
#define MAX_INPUT 100
#define BACKSPACE 8
string getInput(void);
void checkLength(string& s);
vector<string> tokenise(string s, char delimiter);
void printTokens(const vector<string> &input);
void executeProgram(string cmd, vector<string> args, string env);
#endif 






