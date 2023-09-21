/* \file shellDriverMock.h
 * \brief Wrappers for all functions 
 * that touch system level functions mocked
 */

#ifndef SHELL_DRIVER_MOCK_H
#define SHELL_DRIVER_MOCK_H
#include <string>
#include "shellDriverInterface.h"

class ShellDriverMock: public ShellDriverInterface {
public:
    int fileOpen(std::string outfilename, int rwFlag) override;
    void fileClose(int fd) override;
    int dupFile(int fd, FILE* stdinStdout) override;
    int execute( const char *file, char *const argv[]) override;
};

#endif // SHELL_DRIVER_MOCK_H
