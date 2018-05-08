#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// Fibonacci Word a specific infinite sequence in a two-letter alphabet.
// Let f1 be "1" and f2 be "0".
// fn = fn-1fn-2, the concatenaton of the two previous terms
// 0 -> 01, 1->0, f1 = 1
// f1 = 1
// f2 = 0
// f3 = 01
// f4 = 010
// f5 = 01001
// f6 = 01001010
// f7 = 0100101001001
// ...

int main( int argc, char **argv ) {

   // The Fibonacci word may be represented as a fractal 
   // For F_wordm start with F_wordCharn=1
   //    Draw a segment forward
   //    If current F_wordChar is 0
   //       Turn left if n is even
   //       Turn right if n is odd
   //    next n and iterate until end of F_word
}
