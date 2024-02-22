#include "functionLib.h"
#include <iostream>

using namespace std;    

std::unordered_map <const char*, void (*) (char *const argv[])> funcMap = {{"ls", &ls}};
int dispatchFunction(const char *cmd, char *const argv[]){
    if(funcMap.find(cmd)== funcMap.end()){
        return -1;
    }

    funcMap[cmd](argv);
    return 0;
}

void ls(char *const argv[]){
    cout << "running ls" << endl;
}

