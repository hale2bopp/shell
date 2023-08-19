/* \file redirection.h
 * \brief Helper file for redirection
 *
 */

#ifndef REDIRECTION_H
#define REDIRECTION_H

#include <vector>
enum RedirectionType
{
    None = 0,   
    OutputCreate = 1, 
    OutputAppend = 2, 
    Input = 3,
};

typedef struct _RedirectionParams{
    int outputFileIndex;
    int inputFileIndex;
    int cmdStart;
    int cmdEnd;
    RedirectionType redirectionType;
    std::string outfilename;
    std::vector<std::string> cmd;
} RedirectionParams;

#endif  // REDIRECTION_H
