#include "shellDriver.h"
#include "fileDef.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
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

int ShellDriver::fork_and_execute(const char *cmd, char *const argv[]) {
    pid_t cpid = processFork();
    return execute(cmd, argv); 
}

int ShellDriver::execute(const char *cmd, char *const argv[]){
    return execvp(cmd, argv);
}

pid_t ShellDriver::pWaitpid(pid_t pid, int *retVal, int val){
    return waitpid(pid, retVal, val);
}

void ShellDriver::setProcessGroup(void) {
    setpgrp();
}

void ShellDriver::setPGID(pid_t pid, int val){
    setpgid(pid, val);
}

pid_t ShellDriver::getPID(void){
    return getpid();
}

pid_t ShellDriver::processFork(void){
    return fork();
}
