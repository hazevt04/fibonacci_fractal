#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <png.h>
#include <getopt.h>

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

typedef enum direction {UP, DOWN, LEFT, RIGHT, FORWARD} direction_e;


void print_dir( direction_e dir ) {
   if ( dir == UP ) {
      printf( "UP" );
   } else if ( dir == DOWN ) {
      printf( "DOWN" );
   } else if ( dir == LEFT ) {
      printf( "LEFT" );
   } else if ( dir == RIGHT ) {
      printf( "RIGHT" );
   } else if ( dir == FORWARD ) {
      printf( "FORWARD" );
   } else {
      printf( "<INVALID>" );
   }
}


int fib_recursive( int n ) {
   if ( n < 2 ) {
      return 1;   
   } else {
      return fib_recursive( n - 1 ) + fib_recursive( n - 2 );
   }   
}


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
   fprintf( stdout, "start of %s()\n", __func__ );
   
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
      title_text.key = (char *)malloc( 6 ); 
      strcpy( title_text.key, "Title" );
      title_text.text = title;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
   }

   png_write_info(png_ptr, info_ptr);

   // Allocate memory for one row (3 bytes per pixel - RGB)
   row = (png_bytep) malloc(3 * width * sizeof(png_byte));
   /*
   try {
      row = (png_bytep) malloc(3 * width * sizeof(png_byte));
      if (!row) throw "Malloc failed.";
   } catch ( const char* msg ) {
      fprintf( stdout, "ERROR: %s\n", msg );
   }  
   */

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

   fprintf( stdout, "end of %s()\n", __func__ );
   finalise:
   if (fp != NULL) fclose(fp);
   if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
   if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
   if (row != NULL) free(row);

   return code;
} // end of write_image()


void draw_segment_right( uint32_t* pixels, double width, double height, int start_index, int* end_index, int length, uint32_t color ) {

   int index = 0;
   int max_overall_index = ( int )width * ( int )height;
   int last_index = (int)( start_index + length );
   for ( index = start_index; index < last_index; index++ ) {
      if ( ( index >= 0 ) && ( index < max_overall_index ) ) { 
         pixels[ index ] = color;
      }
   }
   *end_index = last_index;
}


void draw_segment_left( uint32_t* pixels, double width, double height, int start_index, int* end_index, int length, uint32_t color ) {

   int index = 0;
   int max_overall_index = ( int )width * ( int )height;
   int last_index = (int)( start_index - length );
   for ( index = start_index; index >= last_index; index-- ) {
      if ( ( index >= 0 ) && ( index < max_overall_index ) ) { 
         pixels[ index ] = color;
      }
   }
   *end_index = last_index;
}


void draw_segment_up( uint32_t* pixels, double width, double height, int start_index, int* end_index, int length, uint32_t color ) {

   int index = 0;
   int max_overall_index = ( int )width * ( int )height;
   int last_index = (int)( start_index - ( length * ( int )width ) );
   for ( index = start_index; index > last_index; index-=(int)width ) {
      if ( ( index >= 0 ) && ( index < max_overall_index ) ) { 
         pixels[ index ] = color;
      }
   }
   *end_index = index;
}


void draw_segment_down( uint32_t* pixels, double width, double height, int start_index, int* end_index, int length, uint32_t color ) {

   int index = 0;
   int max_overall_index = ( int )width * ( int )height;
   int last_index = (int)( start_index + ( length * ( int )width ) );
   for ( index = start_index; index < last_index; index+=(int)width ) {
      if ( ( index >= 0 ) && ( index < max_overall_index ) ) { 
         pixels[ index ] = color;
      }
   }
   *end_index = index;
}

static direction_e prev_dir;


void choose_direction( direction_e dir ) {
   direction_e next_dir;
   char* dir_str = NULL;
   switch( prev_dir ) {
      case UP:
         if ( dir == FORWARD ) {
            next_dir = prev_dir;
         } else if ( dir != DOWN ) {
            next_dir = dir;
         } else {
            printf( "ERROR: prev dir is UP" );
            printf( "Invalid input direction " );
            print_dir( dir );
            printf( "\n" );
            exit( EXIT_FAILURE );
         }
         break;
      case DOWN:
         if ( dir == LEFT ) {
            next_dir = RIGHT;
         } else if ( dir == RIGHT ) {
            next_dir = LEFT;
         } else if ( dir == FORWARD || dir == DOWN ) {
            next_dir = prev_dir;
         } else {
            printf( "ERROR: prev dir is DOWN" );
            printf( "Invalid input direction " );
            print_dir( dir );
            printf( "\n" );
            exit( EXIT_FAILURE );
         }
         break;
      case LEFT:
         if ( dir == RIGHT ) {
            next_dir = UP;
         } else if ( dir == LEFT ) {
            next_dir = DOWN;
         } else if ( dir == FORWARD ) {
            next_dir = prev_dir;
         } else {
            printf( "ERROR: prev dir is LEFT" );
            printf( "Invalid input direction " );
            print_dir( dir );
            printf( "\n" );
            exit( EXIT_FAILURE );
         }
         break;
      case RIGHT:
         if ( dir == LEFT ) {
            next_dir = UP;
         } else if ( dir == RIGHT ) {
            next_dir = DOWN;
         } else if ( dir == FORWARD ) {
            next_dir = prev_dir;
         } else {
            printf( "ERROR: prev dir is RIGHT" );
            printf( "Invalid input direction " );
            print_dir( dir );
            printf( "\n" );
            exit( EXIT_FAILURE );
         }
         break;
   } //
   prev_dir = next_dir;   
}


static int verbose_flag;


static struct option long_options[] = {
   { "verbose", no_argument, NULL, 'v' },
   { "num_iterations", required_argument, NULL, 'n' },
   { "output_file", optional_argument, NULL, 'o' },
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
   printf( "%20s%4s%56s", 
      "--output_file", "-n", 
      "path to the output PNG file for the fractal\n" );  
}


int main( int argc, char **argv ) {
   int num_iterations = 1;
   char title[64];
   strcpy( title, "FibFractal" );
    
   char* output_file = getenv( "PWD" );
   CHECK_NULL_PTR( output_file );
   strcat( output_file, "/fib_fractal.png" );

   char* endptr = NULL;

   int option_index = 0;
   verbose_flag = 0;
   int ch = getopt_long( argc, argv, "vn:o:", long_options, &option_index );
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
         case 'o': 
            strcpy( output_file, optarg );
            break;
         default:
            printf( "ERROR: option %c (%d) invalid\n", ch, ch );
            break;
      } // end of switch
      
      ch = getopt_long( argc, argv, "vn:o:", long_options, &option_index );
   }

   VERBOSE_PRINTF( "num_iterations set to %d.\n", num_iterations );  
   VERBOSE_PRINTF( "output_file set to %s.\n", output_file );  
   
   int num_chars = fib_recursive( num_iterations );
   char** fib_words = ( char** )malloc(num_iterations * sizeof(char*));

   CHECK_NULL_PTR( fib_words );

   int fib_word_len = 1;
   for( int index = 0; index < num_iterations; index++ ) {
      fib_word_len = fib_recursive( index );
      VERBOSE_PRINTF( "fib_word_len %d is %d\n", index, fib_word_len );
      fib_words[ index ] = ( char* )calloc( ( fib_word_len + 1 ), sizeof(char) );
      CHECK_NULL_PTR( fib_words[ index ] );
   }

   // Generate Fibonacci Word
   // ======================================
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
    
   strcpy( fib_words[0], "1" );
   strcpy( fib_words[1], "0" );
   for( int index = 2; index < num_iterations; index++ ) {
      strcat( fib_words[ index ], fib_words[ index - 1 ] );
      strcat( fib_words[ index ], fib_words[ index - 2 ] );
   }
   printf( "\n" );

   printf( "fib_word for %d iterations is %s\n", num_iterations, fib_words[ num_iterations - 1 ] );
   
   prev_dir = UP;

   direction_e temp_dir;
   direction_e* segment_directions = ( direction_e* )malloc( ( fib_word_len ) * sizeof( direction_e ) );
   CHECK_NULL_PTR( segment_directions );
   int segment_index = 0;
   
   for( int index = 0; index < fib_word_len; index++ ) {
      temp_dir = FORWARD;
      choose_direction( temp_dir );
      
      segment_directions[ segment_index ] = prev_dir;      
      segment_index++;
      
      if ( fib_words[ num_iterations - 1 ][ index ] == '0' ) {   
         // If odd
         if ( ( index + 1 ) & 1 ) {
            temp_dir = RIGHT;
         } else {
            temp_dir = LEFT;
         }
         choose_direction( temp_dir );
      }
   } // end of for loop
   
   double width = 4015.0;
   double height = 4015.0;
   uint32_t black = 0;
   uint32_t white = 0xffffff;
   uint32_t color = 0;   
   uint32_t* pixels = ( uint32_t* )malloc( width * height * sizeof(uint32_t) );
   CHECK_NULL_PTR( pixels );
   for ( int index = 0; index < width * height; index++ ) {
      pixels[ index ] = white;
   }
   
   int start_index = ( ( int )width * ( height - 15 ) ) + 15;
   int end_index;
   int length = 5;
   int prev_dir = 0;
   
   for ( int index = 0; index < fib_word_len; index++ ) {
      direction_e temp_dir = segment_directions[ index ];
      if ( temp_dir == UP ) {
         draw_segment_up( pixels, width, height, start_index, &end_index, length, color );
      } else if ( temp_dir == DOWN ) {
         draw_segment_down( pixels, width, height, start_index, &end_index, length, color );
      } else if ( temp_dir == LEFT ) {
         draw_segment_left( pixels, width, height, start_index, &end_index, length, color );
      } else {
         draw_segment_right( pixels, width, height, start_index, &end_index, length, color );
      }
      start_index = end_index;
   }

   printf( "Saving PNG to %s...\n", output_file );
   write_image( output_file, width, height, pixels, title );
   printf( "DONE.\n\n" );

   for( int index = 0; index < num_iterations; index++ ) {
      free( fib_words[ index ] );
   }
   free( fib_words );
   free( segment_directions );
   free( pixels );
   exit( EXIT_SUCCESS );
}

