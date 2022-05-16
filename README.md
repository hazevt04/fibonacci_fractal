# fibonacci\_fractal
======================================
# Description
Generates an image of a fibonacci fractal based on a fibonacci word, which is a binary string (ex: 10010101)
The Fibonacci word may be represented as a fractal as described here:

    For F\_wordm start with F\_wordChar\_n=1
    Draw a segment forward
    If current F_wordChar is 0

        Turn left if n is even
        Turn right if n is odd

    next n and iterate until end of F\_word

# Usage
```bash
./bin/fibonacci_fractal -h
Usage ./bin/fibonacci_fractal <options>
Options one of: 
           --verbose  -v                                          Verbose output
              --help  -h                                 Show this usage message
    --num_iterations  -n                                    Number of iterations
  --background_color  -c               Background Color of the Fibonacci Fractal
                                       (6 RGB hex chars, ex: 0xffffff for white)
       --output_file  -o             Path to the output file for the fractal PNG
```

# Dependencies
This depends on libpng. It works with version 1.6.37. As of this development, libpng depends on zlib.

# Build
```bash
mkdir build && cd build && cmake .. && make
```

# Install
From build directory:
```bash
make install
```

# Clang-format command line:
```bash
find . -iname *.h -o -iname *.c -o -iname *.hpp -o -iname *.cpp | xargs -clang-format -style=file -fallback-style=none -i 
```
