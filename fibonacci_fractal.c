#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <png.h>
#include <getopt.h>




// Let's let val be 687. MAIN COLOR is val/768
// This takes the float value 'val', converts it to red, green & blue values, then
// sets those values into the image memory buffer location pointed to by 'ptr'
void set_rgb(png_byte *ptr, uint32_t val )
{
   ptr[0] = ( val >> 16 ) & 0xFFUL;
   ptr[1] = ( val >> 8 ) & 0xFFUL;
   ptr[2] = ( val ) & 0xFFUL;
}


// This function actually writes out the PNG image file. The string 'title' is
// also written into the image file
int write_image(char* filename, int width, int height, uint32_t* buffer, char* title)
{
   int code = 0;
   FILE *fp = NULL;
   png_structp png_ptr = NULL;
   png_infop info_ptr = NULL;
   png_bytep row = NULL;
   
   // Open file for writing (binary mode)
   fp = fopen(filename, "wb");
   if (fp == NULL) {
      fprintf(stderr, "Could not open file %s for writing\n", filename);
      code = 1;
      goto finalise;
   }

   // Initialize write structure
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL) {
      fprintf(stderr, "Could not allocate write struct\n");
      code = 1;
      goto finalise;
   }

   // Initialize info structure
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
      fprintf(stderr, "Could not allocate info struct\n");
      code = 1;
      goto finalise;
   }

   // Setup Exception handling
   if (setjmp(png_jmpbuf(png_ptr))) {
      fprintf(stderr, "Error during png creation\n");
      code = 1;
      goto finalise;
   }

   png_init_io(png_ptr, fp);

   // Write header (8 bit colour depth)
   png_set_IHDR(png_ptr, info_ptr, width, height,
      8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   // Set title
   if (title != NULL) {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key = "Title";
      title_text.text = title;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
   }

   png_write_info(png_ptr, info_ptr);

   // Allocate memory for one row (3 bytes per pixel - RGB)
   row = (png_bytep) malloc(3 * width * sizeof(png_byte));

   // Write image data
   int x, y;
   for (y=0 ; y<height ; y++) {
      for (x=0 ; x<width ; x++) {
         set_rgb(&(row[x*3]), buffer[y*width + x]);
      }
      png_write_row(png_ptr, row);
   }

   // End write
   png_write_end(png_ptr, NULL);

   finalise:
   if (fp != NULL) fclose(fp);
   if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
   if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
   if (row != NULL) free(row);

   return code;
} // end of write_image()


#define VERBOSE_PRINTF(fmt, ...) { \
   if ( verbose_flag ) { \
      printf(fmt, ##__VA_ARGS__); \
   } \
}


#define CHECK_NULL_PTR(ptr) { \
   if (!ptr) { \
      fprintf( stderr, "File %s, Line %d in %s(): ERROR: " #ptr \
         " is NULL! malloc failed?\n", \
         __FILE__, __LINE__, __func__ ); \
      exit(EXIT_FAILURE); \
   } \
}

/*
const int fib_numbers[16] = { 
      1,    1,    2,    3, 
      5,    8,   13,   21, 
     34,   55,   89,  144, 
    233,  377,  610,  987 
};
*/

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
//void gen_fractal_word( char* fractal, int num_iterations ) {
//   
//}

#define THICKNESS 5

void draw_segment_forward( uint32_t* pixels, double width, double height, int x0, int y0, int length, uint32_t color, int prev_dir ) {
   
   for ( int y_index = y0; y_index < ( y0 + THICKNESS ); y_index++ ) {
      int init_index = (int)( y_index * width + x0 );
      int last_index = (int)( init_index + length );
      for ( int index = init_index; index < last_index; index++ ) {
         pixels[ index ] = color;
      }
   }
   
}

static int verbose_flag;

static struct option long_options[] = {
   { "verbose", no_argument, NULL, 'v' },
   { "num_iterations", required_argument, NULL, 'n' },
   { 0, 0, 0, 0 }
};   

void usage( char* argv ) {
   printf( "Usage %s <options>\n", argv );
   printf( "Options one of:\n" );
   printf( "%20s%4s%56s", 
      "--verbose", "-v", 
      "Verbose output\n\n" );  
   printf( "%20s%4s%56s", 
      "--num_iterations", "-n", 
      "Number of iterations for the Fibonacci fractal\n" );  
}

int fib_recursive( int n ) {
   if ( n < 2 ) {
      return 1;   
   } else {
      return fib_recursive( n - 1 ) + fib_recursive( n - 2 );
   }   
}

int main( int argc, char **argv ) {
   int num_iterations = 1;
   char* endptr = NULL;

   int option_index = 0;
   verbose_flag = 0;
   int ch = getopt_long( argc, argv, "vn:", long_options, &option_index );
   while( ch != -1 ) {
      switch( ch ) {
         case 0:
            // For verbose flag
            // There is no argument after the flag in this case
            if ( long_options[option_index].flag != 0 ) {
               break;
            }
            break;
         case 'v':
            verbose_flag = 1;
            break;
         case 'n':
            num_iterations = strtod( optarg, &endptr );
            break;
         default:
            printf( "ERROR: option %c (%d) invalid\n", ch, ch );
            break;
      } // end of switch
      
      ch = getopt_long( argc, argv, "vn:", long_options, &option_index );
   }

   VERBOSE_PRINTF( "num_iterations set to %d \n", num_iterations );  
   
   int num_chars = fib_recursive( num_iterations );
   char** fib_words = malloc(num_iterations * sizeof(char*));

   CHECK_NULL_PTR( fib_words );

   int fib_word_len = 1;
   for( int index = 0; index < num_iterations; index++ ) {
      fib_word_len = fib_recursive( index );
      VERBOSE_PRINTF( "fib_word_len %d is %d\n", index, fib_word_len );
      fib_words[ index ] = calloc( ( fib_word_len + 1 ), sizeof(char) );
      CHECK_NULL_PTR( fib_words[ index ] );
   }
   
 
   strcpy( fib_words[0], "1" );
   //printf( "fib_words[%3d] is %s\n", 0, fib_words[ 0 ] );
   strcpy( fib_words[1], "0" );
   //printf( "fib_words[%3d] is %s\n", 1, fib_words[ 1 ] );
   for( int index = 2; index < num_iterations; index++ ) {
      strcat( fib_words[ index ], fib_words[ index - 1 ] );
      strcat( fib_words[ index ], fib_words[ index - 2 ] );
      //printf( "fib_words[%3d] is %s\n", index, fib_words[ index ] );
   }
   printf( "\n" );

   printf( "fib_word is %s\n", fib_words[ num_iterations - 1 ] );
   for( int index = 0; index < fib_word_len; index++ ) {
      printf( "Go Forward-" );
      if ( fib_words[ num_iterations - 1 ][ index ] == '0' ) {   
         if ( index & 1 ) {
            printf( "Go Right" );
         } else {
            printf( "Go Left" );
         }
      }
      printf( "\n" );
   } // end of for loop
   
   char title[64];
   strcpy( title, "FibFractal" );
    
   char outfile[64];
   strcpy( outfile, "fib_fractal.png" );
   double width = 1000.0;
   double height = 1000.0;
   int x0 = 50;
   int y0 = 50;
   int length = 20;
   uint32_t black = 0;
   uint32_t white = 0xffffff;
   uint32_t color = 0;   
   int prev_dir = 0;
   uint32_t* pixels = malloc( width * height * sizeof(uint32_t) );
   CHECK_NULL_PTR( pixels );
   for ( int index = 0; index < width * height; index++ ) {
      pixels[ index ] = white;
   }

   draw_segment_forward( pixels, width, height, x0, y0, length, black, prev_dir );   

   printf( "Saving PNG to %s...\n", outfile );
   write_image( outfile, width, height, pixels, title );
   printf( "DONE.\n\n" );

   for( int index = 0; index < num_iterations; index++ ) {
      free( fib_words[ index ] );
   }
   free( fib_words );
   free( pixels );
   exit(EXIT_SUCCESS);
}




