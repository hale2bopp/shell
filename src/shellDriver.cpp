#include "shellDriver.h"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int ShellDriver::fileOpen(string outfilename, int opts){
    return open(outfilename.c_str(), opts, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
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
