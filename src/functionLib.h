/* \file functionLib.h
 * \brief library of shell functionality OS
 *
 */

#ifndef FUNCTIONLIB_H
#define FUNCTIONLIB_H

#include <unordered_map>

int dispatchFunction(const char *cmd, char *const argv[]);

void ls(char *const argv[]);


#endif // FUNCTIONLIB_H
