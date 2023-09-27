/* \file shellDriver.h
 * \brief Driver for all functions in shell
 * that touch system level functions
 */

#ifndef SHELL_DRIVER_H
#define SHELL_DRIVER_H
#include <string>
#include "shellDriverInterface.h"

class ShellDriver: public ShellDriverInterface {
public:
    int fileOpen(std::string outfilename, int rwFlag) override;
    void fileClose(int fd) override;
    int dupFile(int fd, FILE* stdinStdout) override;
    int execute( const char *file, char *const argv[]) override;
    pid_t processFork(void) override;
};

#endif // SHELL_DRIVER_H
