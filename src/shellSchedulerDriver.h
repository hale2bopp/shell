/* \file shellSchedulerDriver.h
 * \brief Driver for all functions in shell
 * that touch system level functions
 */

#ifndef SHELL_SCHEDULER_DRIVER_H
#define SHELL_SCHEDULER_DRIVER_H
#include <string>
#include "shellDriverInterface.h"

class ShellSchedulerDriver: public ShellSchedulerDriverInterface {
public:
    int fileOpen(std::string outfilename, int opts) override;
    void fileClose(int fd) override;
    int dupFile(int fd, FILE* stdinStdout) override;
    int execute( const char *file, char *const argv[]) override;
    pid_t pWaitpid(pid_t pid, int *retVal, int val) override;
    pid_t processFork(void) override;
};

#endif // SHELL_SCHEDULER_DRIVER_H
