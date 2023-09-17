#include "shellSignal.h"
#include <iostream>
using namespace std;


void registerSignals(void){
    registerSigint();
    registerSigchld();
}

void registerSigint(void){
    struct sigaction sigint_action = {0};
    sigint_action.sa_handler = &signalHandler;
    sigaction(SIGINT,&sigint_action,NULL);
}

void registerSigchld(void){
    struct sigaction sigchld_action = {0};
    sigchld_action.sa_handler = &handleSigchld;
    sigaction(SIGCHLD,&sigchld_action,NULL);
}


void signalHandler(int sig){

}

void handleSigchld(int sig){
    int status;
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){}
}
