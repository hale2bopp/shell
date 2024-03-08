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
    virtual int fork_and_execute(const char *cmd, char *const argv[]) = 0;
    virtual int execute( const char *cmd, char *const argv[]) = 0;  
    virtual pid_t processFork( void ) = 0;  
    virtual pid_t pWaitpid(pid_t pid, int *retVal, int val) = 0;
    virtual void setProcessGroup() = 0;
    virtual void setPGID(pid_t pid, int val) = 0;
    virtual pid_t getPID(void) = 0;
    // Define other wrapper functions here
    virtual ~ShellDriverInterface() = default;
};

#endif // SHELL_DRIVER_INTERFACE_H
