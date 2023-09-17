#include "shellDriver.h"
#include <fcntl.h>
#include <iostream>
using namespace std;

int fileOpen(string outfilename, int createFlag){
    return open(outfilename.c_str(), O_WRONLY | O_CREAT | createFlag, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}
