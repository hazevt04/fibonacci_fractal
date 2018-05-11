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
#define FRAC_POW 20
#define SEG_POW 3
#define FRACTAL_LEN (1UL<<FRAC_POW)
#define SEG_LEN (1UL<<SEG_POW)
#define FIB_WORD_LEN (1UL<<(FRAC_POW - SEG_POW))
typedef enum { FORWARD, RIGHT, LEFT } direction_t;

/*
void gen_fractal_word( char* fractal, int num_fractal_letters ) {
}

void draw_segment( char* fractal, int* start_pos, direction_t direction ) {
   if ( direction == FORWARD ) {
   } else if ( direction == RIGHT ) {
   } else if ( direction == LEFT ) {
   }
}
*/

/*
void foo() {
   char wordchar[FIB_WORD_LEN];
   char fractal[FRACTAL_LEN];
   int start_pos = 0;
   // The Fibonacci word may be represented as a fractal 
   // For F_wordm start with F_wordCharn=1
   //    Draw a segment forward
   //    If current F_wordChar is 0
   //       Turn left if n is even
   //       Turn right if n is odd
   //    next n and iterate until end of F_word
   // initial guess:
   for( int wordchar = 0; wordchar < FIB_WORD_LEN; wordchar++ ) {
      // draw a segment forward
      draw_segment( fractal, &start_pos, FORWARD );
      if ( word[wordchar] == '0') {
         direction = ( wordchar & 0x1 ) ? RIGHT : LEFT;
         draw_segment( fractal, &start_pos, direction );
      } // end of if wordchar
   } // end of for
}
*/

int main( int argc, char **argv ) {
   double width = 1024;
   double height = 768;
   
   double center_row = height/2.0;
   double center_col = width/2.0;
   double radius = 6.0;

   double* pixels = calloc( height * width * sizeof(double) ); 
   
   // Write to pixels
   for( double row = 0; row < height ; row++ ) {
      for( double col = 0; col < width; col++ ) {
         if 
      } 
   } 

}
