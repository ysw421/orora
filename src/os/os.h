#ifndef OS_H
#define OS_H

// For development...
// I set default value to x64 based, linux...

// My personal os system....
// OS: GNU/Linux (Windows WSL)
// System type: x86_64

#include <stdint.h>
#include <stdio.h>

/* os checker */
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #define OS_MAC
#elif defined(__linux__)
    #define OS_LINUX
#elif defined(__unix) || defined(__unix__)
    #define OS_UNIX
#else
    #error "Unknown platform"
#endif


/* file size */
// typedef int32_t off_t;
typedef int64_t off_t;

char* get_lib_path();
char* get_executable_path();
char* get_data_directory();

#endif

