#include "functionLib.h"
#include <iostream>

using namespace std;    

std::unordered_map <string, void (*) (char *const argv[])> funcMap = {{"ls", &ls}};
int dispatchFunction(const char *cmd, char *const argv[]){
    string cmd_s(cmd);
    if(funcMap.count(cmd_s) <=0 ){
        return -1;
    }

    funcMap[cmd](argv);
    return 0;
}

void ls(char *const argv[]){
    cout << "running ls" << endl;
}

void ps(char *const argv[]){
    cout << "running ps" << endl;
}


