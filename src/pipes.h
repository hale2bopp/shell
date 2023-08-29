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
};

#endif  // PIPES_H
