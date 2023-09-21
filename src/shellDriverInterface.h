/* \file shellDriver.h
 * \brief Wrappers for all functions 
 * that touch system level functions
 */

#ifndef SHELL_DRIVER_INTERFACE_H
#define SHELL_DRIVER_INTERFACE_H
#include <string>

class ShellDriverInterface {
public:
    virtual int fileOpen(std::string outfilename, int rwFlag) = 0;
    virtual void fileClose(int fd) = 0;
    virtual int dupFile(int fd, FILE* stdinStdout) = 0;
    virtual int execute( const char *file, char *const argv[]) = 0;  
    // Define other wrapper functions here
    virtual ~ShellDriverInterface() {}
};


/*
int fileOpen(std::string outfilename, int rwFlag);
void fileClose(int fd);
int dupFile(int fd, FILE* stdinStdout);
int execute( const char *file, char *const argv[]);
*/

#endif // SHELL_DRIVER_INTERFACE_H
