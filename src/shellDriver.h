/* \file shellDriver.h
 * \brief Wrappers for all functions 
 * that touch system level functions
 */

#ifndef SHELL_DRIVER_H
#define SHELL_DRIVER_H
#include <string>

int fileOpen(std::string outfilename, int rwFlag);
void fileClose(int fd);
int dupFile(int fd, FILE* stdinStdout);
#endif // SHELL_DRIVER_H
