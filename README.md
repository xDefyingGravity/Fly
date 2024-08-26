# Fly

Fly is a simple image grayscale converter created in C.

## Installation

*Note: Fly is designed for macOS on Intel processors.*

1. **Install Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install libpng**:
   ```bash
   brew install libpng
   ```

3. **Download the latest binary** from the [releases page](https://github.com/xDefyingGravity/Fly/releases/).

4. **Add Fly to your PATH**:
   ```bash
   mv ~/Downloads/fly /usr/local/bin/fly && chmod +x /usr/local/bin/fly
   ```

5. **You can now use Fly!**

## CLI Usage

*Note: Fly only supports PNG files.*

```bash
fly input_file.png output_file.png
```

## API Usage

```c
#include "fly.h"

int main(int argc, char** argv) {
    const char* input = "input.png";
    const char* output = "output.png";
    grayscale(input, output);

    return 0;
}
```