#include "shellDriverMock.h"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int ShellDriverMock::fileOpen(string outfilename, int opts){
    return 0;
}

int ShellDriverMock::dupFile(int fd, FILE* stdinStdout){
    return 0;
}

void ShellDriverMock::fileClose(int fd){

}
int ShellDriverMock::execute(const char *cmd, char *const argv[]){
    return 0;
}
