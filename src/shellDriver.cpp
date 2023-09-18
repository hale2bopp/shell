#include "shellDriver.h"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int fileOpen(string outfilename, int opts){
    return open(outfilename.c_str(), opts, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

int dupFile(int fd, FILE* stdinStdout){
    return dup2(fd, fileno(stdinStdout));
}

void fileClose(int fd){
    close(fd);
}
