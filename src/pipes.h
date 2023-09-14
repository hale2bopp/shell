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

typedef struct _PipeSegment {
    std::vector<std::string> cmd;
    int cpid;
    Redirectio
} PipeSegment;

typedef struct _Pipeline {
    int numPipes;
    std::vector<PipeSegment> pipes;
} Pipeline;

#endif  // PIPES_H
