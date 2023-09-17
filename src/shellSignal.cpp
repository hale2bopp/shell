#include "shellSignal.h"
#include <iostream>
using namespace std;


void registerSignals(){
    struct sigaction sa = {0};
    sa.sa_handler = signalHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
}


void signalHandler(int sig){

}

void handleSigchld(int sig){
}
