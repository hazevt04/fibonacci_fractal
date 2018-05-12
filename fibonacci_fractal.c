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
#include <math.h>


#define PREC 10

int main( int argc, char **argv ) {
   double width = 500;
   double height = 500;
   
   double y0 = height/2.0;
   double x0 = width/2.0;
   double radius = 2.0;

   double scale = 1.0/PREC;
   printf( "Scale is %f\n", scale );
   printf( "The center is at %f, %f\n", x0, y0 ); 
   printf( "Radius is %f\n", radius ); 
   printf( "There will be %f points on the x-axis\n", ( PREC * width ) );  
   printf( "There will be %f points on the y-axis\n", ( PREC * height ) );  
   double* pixels = calloc( ( height * width * PREC * PREC ),  sizeof( double ) ); 
   
   // Write to pixels
   for( double row = 0; row < height; row+=scale ) {
      for( double col = 0; col < width; col+=scale ) {
         double x_diff = col - x0;
         double y_diff = row - y0;
         double t_radius = sqrt( x_diff * x_diff + y_diff * y_diff );
         //printf( "%f, %f: t_radius is %f\n", row, col, t_radius  ); 
         if ( t_radius <= radius ) {
            printf("\t%f, %f is %f away from the center: %f, %f (It's in the circle!)\n",
               row, col, radius, x0, y0); 
            int idx = ( int )( row * width + col );
            pixels[ idx ] = 1.0;
         }
      } 
   } 
   printf("\n"); 
}
