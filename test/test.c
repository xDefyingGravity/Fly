#include "fly.h"

int main(int argc, char** argv) {
    const char *input = "test/img/test.png";
    const char *output = "test/output.png";
    grayscale(input, output);

    return 0;
}
