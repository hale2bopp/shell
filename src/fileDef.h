/* \file fileDef.h
 * \brief Provides a common header for file driver includes
 */


#ifndef FILE_DEF_H
#define FILE_DEF_H
#include <fcntl.h>

enum FileOpenOpts {
    S_INVALID = 0,
    S_CREAT = O_WRONLY | O_CREAT| O_TRUNC,
    S_APPEND  = O_WRONLY | O_CREAT| O_APPEND,
    S_RDONLY = O_RDONLY,
};

#endif // FILE_DEF_H
