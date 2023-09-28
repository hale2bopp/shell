/* \file shellDriverInterface.h
 * \brief Provides abstract class interface for shell driver
 */

#ifndef SHELL_DRIVER_INTERFACE_H
#define SHELL_DRIVER_INTERFACE_H
#include <string>

class ShellDriverInterface {
public:
    virtual int fileOpen(std::string outfilename, int opts) = 0;
    virtual void fileClose(int fd) = 0;
    virtual int dupFile(int fd, FILE* stdinStdout) = 0;
    virtual int execute( const char *file, char *const argv[]) = 0;  
    virtual pid_t processFork( void ) = 0;  
    // Define other wrapper functions here
    virtual ~ShellDriverInterface() {}
};

#endif // SHELL_DRIVER_INTERFACE_H
