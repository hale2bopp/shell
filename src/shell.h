#ifndef SHELL_H
#define SHELL_H

#include <vector>
using namespace std;
#define MAX_INPUT 100
string getInput(void);
void checkLength(string& s);
vector<string> tokenise(string s, char delimiter);
void printTokens(const vector<string> &input);
#endif 






