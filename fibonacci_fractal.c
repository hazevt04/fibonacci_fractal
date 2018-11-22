#include <getopt.h>
#include <math.h>
#include <png.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generates a Fibonacci Word Fractal when given
// a number of iterations. Output is to a PNG file.


#define VERBOSE_PRINTF( fmt, ... )        \
   {                                      \
         if ( verbose_flag ) {            \
            printf( fmt, ##__VA_ARGS__ ); \
      }                                   \
   }


#define CHECK_NULL_PTR( ptr )                                 \
   {                                                          \
         if ( !ptr ) {                                        \
            fprintf( stderr,                                  \
                     "File %s, Line %d in %s(): ERROR: " #ptr \
                     " is NULL! malloc failed?\n",            \
                     __FILE__, __LINE__, __func__ );          \
            exit( EXIT_FAILURE );                             \
      }                                                       \
   }

typedef enum direction { UP, DOWN, LEFT, RIGHT, FORWARD } direction_e;


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

/*
unsigned long  fib_recursive( unsigned num ) {
   if ( num < 2 ) {
      return 1;
   } else {
      return fib_recursive( num - 1 ) + fib_recursive( num - 2 );
   }
}
*/

unsigned long fib_recursive_memoized( unsigned long* table, unsigned long num ) {
   unsigned long result;
   if ( num  < 2 ) {
      return 1;
   } else {
      // If Fib(num) is in the table
		// return it
      if ( 0 != table[ num ] ) {
         return table[ num ];
      } else {
			// Fib(num) not in table; calculate it.
         result = fib_recursive_memoized( table, ( num - 1 ) ) +
            fib_recursive_memoized( table, ( num - 2 ) );
         table[ num ] = result;
         return result;
      } // end of else if table[num] == 0
   } // end of else num >= 2
}


// Let's let val be 687. MAIN COLOR is val/768
// This takes the float value 'val', converts it to red, green & blue values,
// then sets those values into the image memory buffer location pointed to by
// 'ptr'
void set_rgb( png_byte* ptr, uint32_t val ) {
   ptr[ 0 ] = ( val >> 16 ) & 0xFFUL;
   ptr[ 1 ] = ( val >> 8 ) & 0xFFUL;
   ptr[ 2 ] = ( val ) & 0xFFUL;
}


// This function actually writes out the PNG image file. The string 'title' is
// also written into the image file
int write_image( char* filename, int width, int height, uint32_t* buffer,
                 char* title ) {
   int code            = 0;
   FILE* fp            = NULL;
   png_structp png_ptr = NULL;
   png_infop info_ptr  = NULL;
   png_bytep row       = NULL;

   // Open file for writing (binary mode)
   fp = fopen( filename, "wb" );
   if ( fp == NULL ) {
      fprintf( stderr, "Could not open file %s for writing\n", filename );
      code = 1;
      goto finalise;
   }

   // Initialize write structure
   png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
   if ( png_ptr == NULL ) {
      fprintf( stderr, "Could not allocate write struct\n" );
      code = 1;
      goto finalise;
   }

   // Initialize info structure
   info_ptr = png_create_info_struct( png_ptr );
   if ( info_ptr == NULL ) {
      fprintf( stderr, "Could not allocate info struct\n" );
      code = 1;
      goto finalise;
   }

   // Setup Exception handling
   if ( setjmp( png_jmpbuf( png_ptr ) ) ) {
      fprintf( stderr, "Error during png creation\n" );
      code = 1;
      goto finalise;
   }

   png_init_io( png_ptr, fp );

   // Write header (8 bit colour depth)
   png_set_IHDR( png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE );

      // Set title
   if ( title != NULL ) {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key         = "Title";
      title_text.text        = title;
      png_set_text( png_ptr, info_ptr, &title_text, 1 );
   }

   png_write_info( png_ptr, info_ptr );

   // Allocate memory for one row (3 bytes per pixel - RGB)
   row = ( png_bytep )malloc( 3 * width * sizeof( png_byte ) );

   // Write image data
   int x, y;
   for ( y = 0; y < height; y++ ) {
      for ( x = 0; x < width; x++ ) {
         set_rgb( &( row[ x * 3 ] ), buffer[ y * width + x ] );
      }
      png_write_row( png_ptr, row );
   }

   // End write
   png_write_end( png_ptr, NULL );

finalise:
   if ( fp != NULL )
      fclose( fp );
   if ( info_ptr != NULL )
      png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
   if ( png_ptr != NULL )
      png_destroy_write_struct( &png_ptr, ( png_infopp )NULL );
   if ( row != NULL )
      free( row );

   return code;
} // end of write_image()


void draw_segment_right( uint32_t* pixels, double width, double height,
                         unsigned long start_index, unsigned long* end_index, unsigned long length,
                         uint32_t color ) {

   unsigned long index             = 0;
   unsigned long max_overall_index = ( unsigned long )width * ( unsigned long )height;
   unsigned long last_index        = ( start_index + length );
   for ( index = start_index; index < last_index; index++ ) {
      if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
         pixels[ index ] = color;
      }
   }
   *end_index = last_index;
}


void draw_segment_left( uint32_t* pixels, double width, double height,
                        unsigned long start_index, unsigned long* end_index, unsigned long length,
                        uint32_t color ) {

   unsigned long index             = 0;
   unsigned long max_overall_index = ( unsigned long )width * ( unsigned long )height;
   unsigned long last_index        = start_index;

   if ( start_index > length ) {
      last_index = ( start_index - length );
      for ( index = start_index; index >= last_index; index-- ) {
         if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
            pixels[ index ] = color;
         }
      }
   }
   *end_index = last_index;
}


void draw_segment_up( uint32_t* pixels, double width, double height,
                      unsigned long start_index, unsigned long* end_index, unsigned long length,
                      uint32_t color ) {

   unsigned long index             = 0;
   unsigned long max_overall_index = ( unsigned long )width * ( unsigned long )height;
   unsigned long last_index        = start_index;
   if ( start_index > ( length * ( unsigned long )width ) ) {
      last_index        = ( unsigned long )( start_index - ( length * ( unsigned long )width ) );
      for ( index = start_index; index > last_index; index -= ( unsigned long )width ) {
         if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
            pixels[ index ] = color;
         }
      }
   }
   *end_index = index;
}


void draw_segment_down( uint32_t* pixels, double width, double height,
                        unsigned long start_index, unsigned long* end_index, unsigned long length,
                        uint32_t color ) {

   unsigned long index             = 0;
   unsigned long max_overall_index = ( unsigned long )width * ( unsigned long )height;
   unsigned long last_index        = ( unsigned long )( start_index + ( length * ( unsigned long )width ) );
   for ( index = start_index; index < last_index; index += ( unsigned long )width ) {
      if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
         pixels[ index ] = color;
      }
   }
   *end_index = index;
}

static direction_e prev_dir;

// Updates prev_dir which is static
void choose_direction( direction_e dir ) {
   direction_e next_dir;
   char* dir_str = NULL;
   switch ( prev_dir ) {
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
   } // end of switch
   prev_dir = next_dir;
}


static int verbose_flag;


static struct option long_options[] = {
   { "help", no_argument, NULL, 'h' },
   { "verbose", no_argument, NULL, 'v' },
   { "num_iterations", required_argument, NULL, 'n' },
   { "color", optional_argument, NULL, 'c' },
   { "output_file", optional_argument, NULL, 'o' },
   { NULL, 0, NULL, 0 }
};   

void usage( char* argv ) {
   printf( "Usage %s <options>\n", argv );
   printf( "Options one of:\n" );
   printf( "%20s%4s%56s", 
      "--verbose", "-v", 
      "Verbose output\n" );  
   printf( "%20s%4s%56s", 
      "--help", "-h", 
      "Show this usage message\n" );  
   printf( "%20s%4s%56s", 
      "--num_iterations", "-n", 
      "Number of iterations for the Fibonacci fractal\n" );  
   printf( "%20s%4s%56s", 
      "--color", "-c", 
      "Color of the Fibonacci fractal\n" );  
   printf( "%20s%4s%56s", 
      "--output_file", "-n", 
      "path to the output PNG file for the fractal\n" );  
}


int main( int argc, char** argv ) {
   unsigned long num_iterations = 1;
   uint32_t color = 0;   
   char title[64];
   strcpy( title, "FibFractal" );

   char* output_file = getenv( "PWD" );
   CHECK_NULL_PTR( output_file );
   strcat( output_file, "/fib_fractal.png" );

   char* endptr = NULL;

   int option_index = 0;
   verbose_flag = 0;

   //int error_flag = 0;
   //int help_flag = 0;
   int ch = getopt_long( argc, argv, "hvn:c:o:", long_options, &option_index );
   while( ch != -1 ) {
      switch( ch ) {
         case 0:
            // For verbose flag
            // There is no argument after the flag in this case
            if ( long_options[option_index].flag != 0 ) {
               break;
            }
            break;
         case 'h':
            usage( argv[0] ); 
            exit( EXIT_SUCCESS );
            break;
         case 'v':
            verbose_flag = 1;
            break;
         case 'n':
            num_iterations = strtoul( optarg, &endptr, 10 );
            break;
         case 'c':
            color = strtod( optarg, &endptr );
            color &= 0xffffff;
            break;
         case 'o': 
            strcpy( output_file, optarg );
            break;
         default:
            usage( argv[0] ); 
            exit( EXIT_FAILURE );
            break;
      } // end of switch
      
      ch = getopt_long( argc, argv, "hvn:c:o:", long_options, &option_index );
   } // end of while loop

   VERBOSE_PRINTF( "num_iterations set to %lu.\n", num_iterations );  
   VERBOSE_PRINTF( "color set to %0x.\n", color );  
   VERBOSE_PRINTF( "output_file set to %s.\n", output_file );  
   
	unsigned long* fib_len_table = ( unsigned long* )calloc( num_iterations, sizeof( unsigned long ) );
   CHECK_NULL_PTR( fib_len_table );

   unsigned long num_chars = fib_recursive_memoized( fib_len_table, num_iterations );

   char** fib_words = malloc( num_iterations * sizeof( char* ) );
   CHECK_NULL_PTR( fib_words );

   unsigned long fib_word_len = 1;
   unsigned long prev_fib_word_len = 1; 
	for ( unsigned long index = 0; index < num_iterations; index++ ) {
      fib_word_len = fib_recursive_memoized( fib_len_table, index );
      VERBOSE_PRINTF( "fib_word_len %lu is %lu\n", index, fib_word_len );

		if ( index == ( num_iterations - 2 ) ) {
			prev_fib_word_len = fib_word_len;
		}
      fib_words[ index ] = calloc( ( fib_word_len + 1 ), sizeof( char ) );
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

   if ( num_iterations > 2 ) {
      strcpy( fib_words[ 0 ], "1" );
      strcpy( fib_words[ 1 ], "0" );
      for ( int index = 2; index < num_iterations; index++ ) {
         strcat( fib_words[ index ], fib_words[ index - 1 ] );
         strcat( fib_words[ index ], fib_words[ index - 2 ] );
      }
   } else if ( num_iterations == 2 ) {
      strcpy( fib_words[ 0 ], "1" );
      strcpy( fib_words[ 1 ], "0" );
   } else {
      strcpy( fib_words[ 0 ], "1" );
   } 

   printf( "fib_word for %lu iterations is %s\n", num_iterations,
           fib_words[ num_iterations - 1 ] );

   prev_dir = UP;

   direction_e temp_dir;
   direction_e* segment_directions =
      malloc( ( fib_word_len ) * sizeof( direction_e ) );
   CHECK_NULL_PTR( segment_directions );
   unsigned long segment_index = 0;

   for ( unsigned long index = 0; index < fib_word_len; index++ ) {
      temp_dir = FORWARD;
      // updates static prev_dir
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
         // updates static prev_dir
         choose_direction( temp_dir );
      }
   } // end of for loop

	// For num_iterations=23, I need the image to be wider
	// TODO: Make this resize the drawing
   double width     = 8030.0;
   double height    = 3515.0;
   double area  = width * height;
   uint32_t black   = 0;
   uint32_t white   = 0xffffff;
   uint32_t* pixels = malloc( area * sizeof( uint32_t ) );
   CHECK_NULL_PTR( pixels );
   for ( unsigned long index = 0; index < ( unsigned long )area; index++ ) {
      pixels[ index ] = white;
   }

   unsigned long start_index = ( ( unsigned long )width * ( ( unsigned long )height - 15 ) ) + 15;
   unsigned long end_index;
   unsigned long length   = 5;

	int prev_dir = 0;
	
	
	for ( unsigned long index = 0; index < fib_word_len; index++ ) {
		direction_e temp_dir = segment_directions[ index ];

		if ( index < prev_fib_word_len ) {
			color = black;
		} else {
			color = 0xff0000;
		}
		if ( temp_dir == UP ) {
         draw_segment_up( pixels, width, height, start_index,
                            &end_index, length, color );
		} else if ( temp_dir == DOWN ) {
         draw_segment_down( pixels, width, height, start_index,
                            &end_index, length, color );
		} else if ( temp_dir == LEFT ) {
         draw_segment_left( pixels, width, height, start_index,
                            &end_index, length, color );
      } else {
         draw_segment_right( pixels, width, height, start_index,
                             &end_index, length, color );
      }
      start_index = end_index;
   }

   printf( "Saving PNG to %s...\n", output_file );
   write_image( output_file, width, height, pixels, title );
   printf( "DONE.\n\n" );

   for ( unsigned long index = 0; index < num_iterations; index++ ) {
      free( fib_words[ index ] );
   }
   free( fib_len_table );
   free( fib_words );
   free( segment_directions );
   free( pixels );
   exit( EXIT_SUCCESS );
}
