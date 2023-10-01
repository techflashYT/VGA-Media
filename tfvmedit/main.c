#include "include/file.h"
#include "include/flags.h"
#include "include/version.h"
#include "include/compression.h"

const char* __asan_default_options() { return "detect_leaks=0"; }


extern void GUI_Init(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    uint16_t flags = FLAGS_DEFAULT;
    Header header = {"TFVM", TFVM_FILE_VERSION_01, flags, COMBINE_COMPRESSION_FPS(COMP_MODE_GZIP, 30), "Video Name"};
    
    GUI_Init(argc, argv);
    return 0;
}