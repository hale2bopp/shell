#include "shellSchedulerDriver.h"
#include "fileDef.h"
#include "os.h"
#include <iostream>
#include <unistd.h>
using namespace std;

int ShellSchedulerDriver::fileOpen(string outfilename, int opts){
    return open(outfilename.c_str(), opts, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

int ShellSchedulerDriver::dupFile(int fd, FILE* stdinStdout){
    return dup2(fd, fileno(stdinStdout));
}

void ShellSchedulerDriver::fileClose(int fd){
    close(fd);
}

int ShellSchedulerDriver::execute(const char *cmd, char *const argv[]){
    return execvp(cmd, argv);
}

pid_t ShellSchedulerDriver::processFork(void){
    return p_spawn();
}
