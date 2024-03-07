#include "functionLib.h"
#include <iostream>
#include "os.h"

using namespace std;    

//std::unordered_map <string, void (*) (void)> funcMap = {{"ls", &ls}};
int dispatchFunction(const char *cmd, char *const argv[]){
    string cmd_s(cmd);
//    if(funcMap.count(cmd_s) <=0 ){
//        return -1;
//    }

    //return p_spawn(funcMap[cmd], argv);
//    return p_spawn(reinterpret_cast<void (*)()>(ls) ,static_cast<void*>(argv));
//    return p_spawn(ls, argv);
    string str1 = "thread1";
//    return p_spawn(reinterpret_cast<void (*)()>(ls), static_cast<void*>(&str1));
//    return p_spawn(ls, "-l");
//    funcMap[cmd](argv);
    return 0;
}

void ls(char *const argv[]){
    cout << "running ls" << endl;
}

void ps(char *const argv[]){
    cout << "running ps" << endl;
}


