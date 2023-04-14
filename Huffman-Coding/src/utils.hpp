#include <string>
#include <vector>

#ifndef MAX_FILE_SIZE
// 512MB
#define MAX_FILE_SIZE (1024 * 1024 * 512)
#endif

inline std::string readfile(int fd) {
    auto infile = fdopen(fd, "r");
    auto indata = new char[MAX_FILE_SIZE];
    auto insize = fread(indata, 1, MAX_FILE_SIZE, infile);
    if (insize == MAX_FILE_SIZE) {
        char c;
        if (fread(&c, 1, 1, infile)) {
            fprintf(stderr, "file size exceed %d bytes.\n", MAX_FILE_SIZE);
            exit(EXIT_FAILURE);
        }
    }
    return { indata, insize };
}
