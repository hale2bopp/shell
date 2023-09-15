/* \file pipes.h
 * \brief Helper file for pipes
 *
 */

#ifndef PIPES_H
#define PIPES_H

#include <vector>
enum PipesErr{
    PipesErrNone = 0,
    PipesDoublePipe = 1,
    PipesEndsWithPipe = 2,
    PipesExecErr = 3,
};

typedef struct _Pipeline {
    int numPipes;
    std::vector<std::vector<std::string>> pipes;
} Pipeline;

#endif  // PIPES_H
