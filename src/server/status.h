#ifndef CODE_H
#define CODE_H

#include <stdint.h>

// #define CODE_SUCCESS "200"
#define ORORA_STATUS_SUCCESS 0x00000000
#define ORORA_STATUS_ERROR   0x10000000
#define ORORA_STATUS_CONSOLE 0x30000000
#define ORORA_STATUS_MORE 0x40000000

// #define CODE_ERROR "500"
// #define CODE_CONSOLE "300"

// #define CODE_ERROR 0x10000000
// #define CODE_CONSOLE 0x30000000
// #define CODE_SUCCESS 0x00000000

typedef struct {
    uint32_t status;
    uint32_t length;
} MessageHeader;

#endif
