/* \file redirection.h
 * \brief Helper file for redirection
 *
 */

#ifndef REDIRECTION_H
#define REDIRECTION_H

enum RedirectionType
{   
    OutputCreate = 0, 
    OutputAppend = 4, 
    Input = 8,
};

#endif  // REDIRECTION_H
