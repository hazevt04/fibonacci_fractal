#include <getopt.h>
#include <math.h>
#include <png.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generates a Fibonacci Word Fractal when given
// a number of iterations. Output is to a PNG file or SVG file

#define GREATER_THAN( A, B ) ( ( A ) > ( B ) ? ( A ) : ( B ) )
#define LESS_THAN( A, B ) ( ( A ) < ( B ) ? ( A ) : ( B ) )

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

typedef struct {
   ulong y;
   ulong x;
} point_t;

int write_svg(char* filename, point_t min_point, point_t max_point, int num_points, point_t* points, char* title) {
   
   int code = 0;
   ulong height = max_point.y;
   ulong width = max_point.x;
   FILE *fp = NULL;
   // Open file for writing
   fp = fopen(filename, "w");

   if ( !fp ) {
      fprintf(stderr, "Could not open file %s for writing\n", filename);
      code = 1;
      if (fp != NULL) fclose(fp);
      return code;
   }
   
   // Write the data
   fprintf( fp, "<!DOCTYPE html>" );
   fprintf( fp, "<html>" );
   fprintf( fp, "<body>" );

   fprintf( fp, "<svg height=\"%ld\" width=\"%ld\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink= \"http:www.w3.org/1999/xlink\">", height, width );
   //<polyline points="20,20 40,25 60,40 80,120 120,140 200,180" style="fill:none;stroke:black;stroke-width:3" />
   fprintf( fp, "<polyline points=\"" );
   for( int index = 0; index < num_points; index++ ) {
      fprintf( fp, "%ld,%ld", points[ index ].x, points[ index ].y );
      if ( index == num_points - 1 ) {
         fprintf( fp, "\"" );
      } else {
         fprintf( fp, " " );
      }
   }
   fprintf( fp, " style=\"fill:none;stroke:black;stroke-width:1\" />" );
   fprintf( fp, "Sorry, your browser does not support inline SVG." );
   fprintf( fp, "</svg>" );

   fprintf( fp, "</body>" );
   fprintf( fp, "</html>" );
   
   if (fp != NULL) fclose(fp);
   return code;
}

ulong image_dims[11][2] = {
   {30,30},
   {45,30},
   {45,45},
   {165,165},
   {540,400},
   {950,1250},
   {2950,2100},
   {5000,2100},
   {5000,6000},
   {5200,7200},
   {30050,20050}
};

typedef struct {
   ulong width;
   ulong height;
} image_dim_t;

image_dim_t image_dim_lookup( ulong num_iterations ) {
   image_dim_t image_dim;
   int dim_idx = 0;
   switch( num_iterations ) {
      case 1 ... 4:
         dim_idx = 0;
         break;
      case 5 ... 6:
         dim_idx = 1;
         break;
      case 7 ... 12:
         dim_idx = 2;
         break;
      case 13 ... 16:
         dim_idx = 3;
         break;
      case 17 ... 19:
         dim_idx = 4;
         break;
      case 20 ... 21:
         dim_idx = 5;
         break;
      case 22 ... 23:
         dim_idx = 6;
         break;
      case 24:
         dim_idx = 7;
         break;
      case 25:
         dim_idx = 8;
         break;
      case 26:
         dim_idx = 9;
         break;
      default:
         dim_idx = 10;
         break;

   } // end of switch
   image_dim.width = image_dims[dim_idx][0];
   image_dim.height = image_dims[dim_idx][1];

   return image_dim;
}

ulong color_select( ulong index, ulong max_val ) {

   ulong quarter = max_val + 4 / 4;
   ulong half = max_val + 1 / 2;
   ulong three_quarters = 3 * quarter;

   ulong adjusted_index;
   if ( adjusted_index < quarter ) {
      adjusted_index = (( max_val - index ) & 0x3f ) * 0x010203;
   } else if ( ( adjusted_index >= quarter ) && ( adjusted_index < half ) ) {
      adjusted_index = (( max_val - index ) & 0x7f ) * 0x040506;
   } else if ( ( adjusted_index >= half ) && ( adjusted_index < three_quarters ) ) {
      adjusted_index = (( max_val - index ) & 0xbf ) * 0x070809;
   } else {
      adjusted_index = (( max_val - index ) & 0xff ) * 0x0a0b0c;
   }
   return adjusted_index; 

   /*
   const ulong BLACK = 0x00000000;
   const ulong RED	= 0x00ff0000;
   const ulong GREEN	= 0x0000ff00;
   const ulong BLUE	= 0x000000ff;
   const ulong WHITE = 0x00ffffff;
   
   const ulong PURPLE= RED | BLUE;
   const ulong YELLOW= RED | GREEN;
   */
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

ulong fib_recursive_memoized( ulong* table, ulong num ) {
   ulong result;
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
void set_rgb( png_byte* ptr, ulong val ) {
   ptr[ 0 ] = ( val >> 16 ) & 0xFFUL;
   ptr[ 1 ] = ( val >> 8 ) & 0xFFUL;
   ptr[ 2 ] = ( val ) & 0xFFUL;
}


// This function actually writes out the PNG image file. The string 'title' is
// also written into the image file
int write_png( char* filename, int width, int height, ulong* buffer,
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
} // end of write_png()


void draw_segment_right( ulong* pixels, ulong width, ulong height,
                         ulong start_index, ulong* end_index, ulong length,
                         ulong color ) {

   ulong index             = 0;
   ulong max_overall_index = width * height;
   ulong last_index        = ( start_index + length );
   for ( index = start_index; index < last_index; index++ ) {
      if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
         pixels[ index ] = color;
      }
   }
   *end_index = last_index;
}


void draw_segment_left( ulong* pixels, ulong width, ulong height,
                        ulong start_index, ulong* end_index, ulong length,
                        ulong color ) {

   ulong index             = 0;
   ulong max_overall_index = width * height;
   ulong last_index        = start_index;

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


void draw_segment_up( ulong* pixels, ulong width, ulong height,
                      ulong start_index, ulong* end_index, ulong length,
                      ulong color ) {

   ulong index             = 0;
   ulong max_overall_index = width * height;
   ulong last_index        = start_index;
   if ( start_index > ( length * width ) ) {
      last_index        = ( start_index - ( length * width ) );
      for ( index = start_index; index > last_index; index -= width ) {
         if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
            pixels[ index ] = color;
         }
      }
   }
   *end_index = index;
}


void draw_segment_down( ulong* pixels, ulong width, ulong height,
                        ulong start_index, ulong* end_index, ulong length,
                        ulong color ) {

   ulong index             = 0;
   ulong max_overall_index = width * height;
   ulong last_index        = ( ulong )( start_index + ( length * width ) );
   for ( index = start_index; index < last_index; index += width ) {
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
#ifdef TESTING
   { "width", optional_argument, NULL, 'x' },
   { "height", optional_argument, NULL, 'y' },
#endif
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
#ifdef TESTING
   printf( "%20s%4s%56s", 
      "--width", "-x", 
      "width in pixels of the fractal image\n" );  
   printf( "%20s%4s%56s", 
      "--height", "-y", 
      "height in pixels of the fractal image\n" );  
#endif
}


int main( int argc, char** argv ) {
   const ulong BLACK = 0x00000000;
   const ulong RED	= 0x00ff0000;
   const ulong GREEN	= 0x0000ff00;
   const ulong BLUE	= 0x000000ff;
   const ulong WHITE = 0x00ffffff;
   ulong color = 0;   
   ulong index = 0;

   char title[64];
   strcpy( title, "FibFractal" );

   char* output_file = getenv( "PWD" );
   CHECK_NULL_PTR( output_file );
   strcat( output_file, "/fib_fractal.png" );

   ulong num_iterations = 1;
   char* endptr = NULL;

   int option_index = 0;
   verbose_flag = 0;

   ulong width = 16350;
   ulong height = 16350;
#ifdef TESTING
   printf( "TESTING set\n" );
   int ch = getopt_long( argc, argv, "hvn:c:o:x:y:", long_options, &option_index );
#else
   int ch = getopt_long( argc, argv, "hvn:c:o:", long_options, &option_index );
#endif
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
#ifdef TESTING
         case 'x':
            width = strtoul( optarg, &endptr, 10 );
            break;
         case 'y':
            height = strtoul( optarg, &endptr, 10 );
            break;
#endif
         default:
            usage( argv[0] ); 
            exit( EXIT_FAILURE );
            break;
      } // end of switch
#ifdef TESTING
      ch = getopt_long( argc, argv, "hvn:c:o:x:y:", long_options, &option_index );
#else
      ch = getopt_long( argc, argv, "hvn:c:o:", long_options, &option_index );
#endif
   } // end of while loop
   
   image_dim_t image_dim = image_dim_lookup( num_iterations );
   width = image_dim.width;
   height = image_dim.height;

   VERBOSE_PRINTF( "num_iterations set to %lu.\n", num_iterations );  
   VERBOSE_PRINTF( "color set to %0lx.\n", color );  
   VERBOSE_PRINTF( "output_file set to %s.\n", output_file );  
#ifdef TESTING
   printf( "width set to %lu.\n", width );  
   printf( "height set to %lu.\n", height );  
#endif
   
	ulong* fib_len_table = ( ulong* )calloc( num_iterations, sizeof( ulong ) );
   CHECK_NULL_PTR( fib_len_table );

   ulong num_chars = fib_recursive_memoized( fib_len_table, num_iterations );

   char** fib_words = malloc( num_iterations * sizeof( char* ) );
   CHECK_NULL_PTR( fib_words );

   ulong fib_word_len = 1;
   ulong prev_fib_word_len = 1; 
	for ( index = 0; index < num_iterations; index++ ) {
      fib_word_len = fib_recursive_memoized( fib_len_table, index );
      VERBOSE_PRINTF( "fib_word_len %lu is %lu\n", index, fib_word_len );

      fib_words[ index ] = calloc( ( fib_word_len + 1 ), sizeof( char ) );
      CHECK_NULL_PTR( fib_words[ index ] );
   }
	
	prev_fib_word_len = fib_len_table[ num_iterations - 2 ];
	
	printf( "Fibonacci Word length was %lu\n", fib_word_len );
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

   //printf( "fib_word for %lu iterations is %s\n", num_iterations,
   //        fib_words[ num_iterations - 1 ] );

   prev_dir = UP;

   direction_e temp_dir;
   direction_e* segment_directions =
      malloc( ( fib_word_len ) * sizeof( direction_e ) );
   CHECK_NULL_PTR( segment_directions );
   ulong segment_index = 0;

   for ( long index = 0; index < fib_word_len; index++ ) {
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

   
   ulong num_pixels  = width * height;
   ulong* pixels = malloc( num_pixels * sizeof( ulong ) );
   CHECK_NULL_PTR( pixels );
   for ( index = 0; index < num_pixels; index++ ) {
      pixels[ index ] = BLACK;
   }

   ulong start_index = ( width * ( height - 15 ) ) + 15;
   ulong end_index;
   ulong length   = 5;

	int prev_dir = 0;
	
   color = WHITE;
   int points_index = 0;
   
   // Actually just a container for the max x and max y
   point_t max_point;
   // Actually just a container for the min x and min y
   point_t min_point;

   max_point.y = 0;
   max_point.x = 0;
   min_point.y = LONG_MAX;
   min_point.x = LONG_MAX;
   point_t* points = malloc( fib_word_len * 2 * sizeof(point_t) );
   CHECK_NULL_PTR( points );

	for ( index = 0; index < fib_word_len; index++ ) {
		direction_e temp_dir = segment_directions[ index ];

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
      points[ points_index ].y = start_index/width;
      max_point.y = GREATER_THAN( max_point.y, points[ points_index ].y );
      min_point.y = LESS_THAN( min_point.y, points[ points_index ].y );
      
      points[ points_index ].x = start_index % width;
      max_point.x = GREATER_THAN( max_point.x, points[ points_index ].x );
      min_point.x = LESS_THAN( min_point.x, points[ points_index ].x );

      points_index++;

      points[ points_index ].y = end_index/width;
      max_point.y = GREATER_THAN( max_point.y, points[ points_index ].y );
      min_point.y = LESS_THAN( min_point.y, points[ points_index ].y );
      
      points[ points_index ].x = end_index % width;
      max_point.x = GREATER_THAN( max_point.x, points[ points_index ].x );
      min_point.x = LESS_THAN( min_point.x, points[ points_index ].x );
      points_index++;

      start_index = end_index;
     
      color = color_select( index, fib_word_len ); 
   }
   char output_svg_file[64];
   strcpy( output_svg_file, "fixed_fib_fractal.svg" );
   char svg_title[64];
   strcpy( svg_title, "Fibonacci Fractal" );
   
   printf( "Saving points to %s...\n", output_svg_file );
   write_svg( output_svg_file, min_point, max_point, points_index, 
         points, svg_title );


   printf( "Saving PNG to %s...\n", output_file );
   write_png( output_file, width, height, pixels, title );
   printf( "DONE.\n\n" );

   for ( index = 0; index < num_iterations; index++ ) {
      free( fib_words[ index ] );
   }
   free( fib_len_table );
   free( fib_words );
   free( segment_directions );
   free( pixels );
   free( points );
   exit( EXIT_SUCCESS );
}
