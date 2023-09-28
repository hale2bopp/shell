#include "shellDriver.h"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int ShellDriver::fileOpen(string outfilename, int opts){

    FileOpenOpts options;
    switch (opts){
        case 1:
            options = S_CREAT;
            break;
        case 2:
            options = S_APPEND;
            break;
        case 3:
            options = S_RDONLY;
            break;
        default:
            options = S_INVALID;
    }
    
    return open(outfilename.c_str(), options, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

int ShellDriver::dupFile(int fd, FILE* stdinStdout){
    return dup2(fd, fileno(stdinStdout));
}

void ShellDriver::fileClose(int fd){
    close(fd);
}

int ShellDriver::execute(const char *cmd, char *const argv[]){
    return execvp(cmd, argv);
}

pid_t ShellDriver::processFork(void){
    return fork();
}
