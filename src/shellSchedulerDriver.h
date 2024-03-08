/* \file shellSchedulerDriver.h
 * \brief Driver for all functions in shell
 * that touch system level functions
 */

#ifndef SHELL_SCHEDULER_DRIVER_H
#define SHELL_SCHEDULER_DRIVER_H
#include <string>
#include "shellDriverInterface.h"

class ShellSchedulerDriver: public ShellDriverInterface {
public:
    int fileOpen(std::string outfilename, int opts) override;
    void fileClose(int fd) override;
    int dupFile(int fd, FILE* stdinStdout) override;
    int fork_and_execute(const char *cmd, char *const argv[]) override;
    int execute( const char *cmd, char *const argv[]) override;
    pid_t pWaitpid(pid_t pid, int *retVal, int val) override;
    void setProcessGroup(void) override;
    void setPGID(pid_t pid, int val) override;
    pid_t getPID(void) override;
    pid_t processFork(void) override;
};

#endif // SHELL_SCHEDULER_DRIVER_H
