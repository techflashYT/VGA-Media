#ifndef COMPRESSION_H
#define COMPRESSION_H

#define COMP_MODE_RAW   0
#define COMP_MODE_GZIP  1
#define COMP_MODE_ZSTD  2
#define COMP_MODE_LZMA  3
#define COMP_MODE_LZ4   4

#define COMBINE_COMPRESSION_FPS(x, y) (((x) << 5) | ((y) >> 1))
#define EXTRACT_COMPRESSION_MODE(x) ((x) >> 5)
#define EXTRACT_FPS(x) ((x) << 1 & 0x3E)

#endif // COMPRESSION_H