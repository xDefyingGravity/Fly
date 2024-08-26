#include "fly.h"
int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    grayscale(argv[1], argv[2]);

    return 0;
}