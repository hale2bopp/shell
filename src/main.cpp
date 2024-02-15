#include <iostream>
#include<sys/wait.h>
#include "shell.h"
#include "shellDriver.h"
#include "shellSignal.h"
#include <string.h>

const char* err_msg = "unable to execute";
string env = "PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games";

string mainPrompt = "penn-shredder# ";
int main(void){
    ShellDriver shellDriver;
    auto pennshPtr = createShellWithDriver(mainPrompt, shellDriver);
    pennshPtr->shellRun();
    return 0;
}

