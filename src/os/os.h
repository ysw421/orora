#ifndef OS_H
#define OS_H

// For development...
// I set default value to x64 based, linux...

// My personal os system....
// OS: GNU/Linux (Windows WSL)
// System type: x86_64

#include <stdint.h>

/* os checker */
#define OS_LINUX
// #define OS_WINDOWS
// #define OS_MAC

/* file size */
// typedef int32_t off_t;
typedef int64_t off_t;

#endif

