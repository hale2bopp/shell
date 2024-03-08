/* \file shellDriverMock.h
 * \brief Wrappers for all functions 
 * that touch system level functions mocked
 */

#ifndef SHELL_DRIVER_MOCK_H
#define SHELL_DRIVER_MOCK_H
#include <string>
#include "shellDriverInterface.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class ShellDriverMock: public ShellDriverInterface {
public:
    MOCK_METHOD(int, fileOpen, (std::string outfilename, int opts), (override));
    MOCK_METHOD(void, fileClose, (int fd), (override));
    MOCK_METHOD(int, dupFile, (int fd, FILE* stdinStdout), (override));
    MOCK_METHOD(int, execute, (const char *file, char *const argv[]), (override));
    MOCK_METHOD(int, fork_and_execute, (const char *cmd, char *const argv[]), (override));
    MOCK_METHOD(pid_t, pWaitpid, (pid_t pid, int *retVal, int val), (override));
    MOCK_METHOD(pid_t, processFork, (), (override));
    MOCK_METHOD(void, setProcessGroup, (), (override));
    MOCK_METHOD(void,  setPGID, (pid_t pid, int val), (override));
    MOCK_METHOD(pid_t, getPID, (), (override));
};

#endif // SHELL_DRIVER_MOCK_H
