/* \file shellDriver.h
 * \brief Wrappers for all functions 
 * that touch system level functions
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


/*
int fileOpen(std::string outfilename, int rwFlag);
void fileClose(int fd);
int dupFile(int fd, FILE* stdinStdout);
int execute( const char *file, char *const argv[]);
*/

#endif // SHELL_DRIVER_MOCK_H
