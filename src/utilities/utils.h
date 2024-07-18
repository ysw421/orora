#ifndef UTILIS_H
#define UTILIS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

const char* const_strcat(const char* str1, const char* str2);
const char* float_to_string(float value);
const char* int_to_string(int value);
size_t orora_write(const char* message, uint32_t status);

#endif
