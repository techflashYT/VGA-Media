#ifndef FILE_H
#define FILE_H

#include <stdint.h>

typedef struct {
    char magic[4];
    uint8_t version;
    uint16_t flags;
    uint8_t compressionType_StartingFPS;
    char videoName[57];
} Header;

typedef struct {
    char magic[4];
    uint8_t identifier;
    uint16_t dataSize;
    void *data;
} Chunk;

#endif