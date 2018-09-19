# fibonacci\_fractal

# Description
The Fibonacci word may be represented as a fractal as described here:

    For F\_wordm start with F\_wordChar\_n=1
    Draw a segment forward
    If current F_wordChar is 0

        Turn left if n is even
        Turn right if n is odd

    next n and iterate until end of F\_word


# Task
Create and display a fractal similar to Fig 1

# Clang-format command line:
```
find . -iname *.h -o -iname *.c -o -iname *.hpp -o -iname *.cpp | xargs -clang-format -style=file -fallback-style=none -i 
