#include "shellSignal.h"
#include <iostream>
using namespace std;


void registerSignals(){
    signal ( SIGINT, signalHandler); 
}

void signalHandler(int sig){

}

void handleSigchld(int sig){
}
