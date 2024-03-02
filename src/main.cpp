#include "shell.h"
#include "shellSchedulerDriver.h"
#include "shellSignal.h"
#include <string.h>

const char* err_msg = "unable to execute";
string env = "PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games";

string mainPrompt = "penn-shredder# ";
int main(void){
    ShellSchedulerDriver shellDriver;
    auto pennshPtr = createShellWithDriver(mainPrompt, shellDriver);
    pennshPtr->shellRun();
    return 0;
}

