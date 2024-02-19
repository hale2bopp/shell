#include "shellSignal.h"
#include "shellDriverInterface.h"
#include <sys/wait.h>
#include <iostream>
using namespace std;


bool coutAlarm = false;
bool getCoutAlarm(void){
    return coutAlarm; 
}

void registerSignals(void){
    registerSigint();
    registerSigchld();
}

void registerSigint(void){
    /*
    struct sigaction sigint_action = {0};
    sigint_action.sa_handler = &signalHandler;
    sigaction(SIGINT,&sigint_action,NULL);
    */
    signal(SIGINT, signalHandler);
}

void registerSigchld(void){
    /*
    struct sigaction sigchld_action = {0};
    sigchld_action.sa_handler = &handleSigchld;
    sigaction(SIGCHLD,&sigchld_action,NULL);
    */
    signal(SIGCHLD, handleSigchld);
}


void signalHandler(int sig){

}

void handleSigchld(int sig){
    int status;
    pid_t pid;

    while((pid = waitpid(-1, &status, WNOHANG)) > 0){}
    coutAlarm = true;
}
