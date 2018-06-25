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

#define THICKNESS 1

//void draw_segment_forward( uint32_t* pixels, double width, double height, int x0, int y0, int length, uint32_t color, int prev_dir ) {
//   
//   for ( int y_index = y0; y_index < ( y0 + THICKNESS ); y_index++ ) {
//      int init_index = (int)( y_index * width + x0 );
//      int last_index = (int)( init_index + length );
//      for ( int index = init_index; index < last_index; index++ ) {
//         pixels[ index ] = color;
//      }
//   }
//   
//}

typedef struct {
   int x;
   int y;
} point_t;

void print_point( point_t pt ) {
   printf( "y: %d, x: %d", pt.y, pt.x );
}


void draw_segment_right( uint32_t* pixels, double width, double height, point_t start_pt, point_t* end_pt, int length, uint32_t color ) {

   printf( "%s() Startpoint is ", __func__ );
   print_point( start_pt );
   printf( "\n" );
   for ( int y_index = start_pt.y; y_index < ( start_pt.y + THICKNESS ); y_index++ ) {
      int init_index = (int)( y_index * width + start_pt.x );
      int last_index = (int)( init_index + length );
      for ( int index = init_index; index < last_index; index++ ) {
         pixels[ index ] = color;
      }
   }
   end_pt->x = start_pt.x + length;
   end_pt->y = start_pt.y;
   printf( "%s() Endpoint is ", __func__ );
   print_point( *end_pt );
   printf( "\n" );
}

void draw_segment_left( uint32_t* pixels, double width, double height, point_t start_pt, point_t* end_pt, int length, uint32_t color ) {

   printf( "%s() Startpoint is ", __func__ );
   print_point( start_pt );
   printf( "\n" );
   for ( int y_index = start_pt.y; y_index < ( start_pt.y + THICKNESS ); y_index++ ) {
      int init_index = (int)( y_index * width + start_pt.x );
      int last_index = (int)( init_index - length );
      for ( int index = last_index; index > init_index; index-- ) {
         pixels[ index ] = color;
      }
   }
   end_pt->x = start_pt.x - length;
   end_pt->y = start_pt.y;
   printf( "%s() Endpoint is ", __func__ );
   print_point( *end_pt );
   printf( "\n" );
}


void draw_segment_up( uint32_t* pixels, double width, double height, point_t start_pt, point_t* end_pt, int length, uint32_t color ) {

   printf( "width x height = %f\n", ( width * height ) );
   printf( "%s() Startpoint is ", __func__ );
   print_point( start_pt );
   printf( "\n" );
   for ( int x_index = start_pt.x; x_index < ( start_pt.x + THICKNESS ); x_index++ ) {
      int init_index = (int)( x_index * height + start_pt.y );
      printf( "init_index is %d\n", init_index );
      int last_index = (int)( init_index + ( length * ( int )width ) );
      printf( "last_index is %d\n", last_index );
      for ( int index = last_index; index > init_index; index-=(int)width ) {
         pixels[ index ] = color;
         printf( "%d- index = %d\n", index, ( index ) );
      }
   }
   end_pt->x = start_pt.x;
   end_pt->y = start_pt.y + length;
   printf( "%s() Endpoint is ", __func__ );
   print_point( *end_pt );
   printf( "\n" );
}

void draw_segment_down( uint32_t* pixels, double width, double height, point_t start_pt, point_t* end_pt, int length, uint32_t color ) {

   printf( "width x height = %f\n", ( width * height ) );
   printf( "%s() Startpoint is ", __func__ );
   print_point( start_pt );
   printf( "\n" );
   for ( int x_index = start_pt.x; x_index < ( start_pt.x + THICKNESS ); x_index++ ) {
      int init_index = (int)( x_index * height + start_pt.y );
      printf( "init_index is %d\n", init_index );
      int last_index = (int)( init_index + ( length * ( int )width ) );
      printf( "last_index is %d\n", last_index );
      for ( int index = init_index; index < init_index; index+=(int)width ) {
         pixels[ index ] = color;
         printf( "%d- index = %d\n", index, ( index ) );
      }
   }
   end_pt->x = start_pt.x;
   end_pt->y = start_pt.y + length;
   printf( "%s() Endpoint is ", __func__ );
   print_point( *end_pt );
   printf( "\n" );
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
   //printf( "Prev direction updated to " );
   //print_dir( prev_dir );
   //printf( "\n" );
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
   
   prev_dir = UP;

   printf( "Prev direction is " );
   print_dir( prev_dir );
   printf( "\n" );
 
   direction_e temp_dir;
   direction_e* segment_directions = malloc( ( fib_word_len ) * sizeof( direction_e ) );
   CHECK_NULL_PTR( segment_directions );
   int segment_index = 0;
   
   for( int index = 0; index < fib_word_len; index++ ) {
      //printf( "Go Forward-" );
      temp_dir = FORWARD;
      choose_direction( temp_dir );
      printf( "%d- (In Loop) Go ", ( index + 1 ) );
      print_dir( prev_dir );
      printf( "\n" );
      
      // prev_dir here corresponds to the direction of the line segments we want
      // save it into the segment_directions array
      segment_directions[ segment_index ] = prev_dir;      
      
      printf( "Segment Direction %d- ", ( segment_index + 1 ) );
      print_dir( segment_directions[ segment_index ] );
      printf( "\n" );
      
      segment_index++;

      if ( fib_words[ num_iterations - 1 ][ index ] == '0' ) {   
         // If odd
         if ( ( index + 1 ) & 1 ) {
            //printf( "Go Right" );
            temp_dir = RIGHT;
         } else {
            //printf( "Go Left" );
            temp_dir = LEFT;
         }
         choose_direction( temp_dir );
         //printf( "%d- Turn ", ( index + 1 ) );
         //print_dir( prev_dir );
         //printf( "\n" );
      }
      //printf( "\n" );
   } // end of for loop
   
   char title[64];
   strcpy( title, "FibFractal" );
    
   char outfile[64];
   strcpy( outfile, "fib_fractal.png" );
   double width = 1000.0;
   double height = 1000.0;
   
   //point_t start_pt;
   //start_pt.x = 50;
   //start_pt.y = 50;
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
   
   //point_t end_pt;
   //end_pt.x = start_pt.x;
   //end_pt.y = start_pt.y;

   point_t start_pt[4];
   point_t end_pt[4];

   for ( int index = 0; index < 4; index++ ) {
      start_pt[ index ].x = ( 50 * ( index + 1 ) );
      start_pt[ index ].y = 50;    
   }

   draw_segment_up( pixels, width, height, start_pt[0], &end_pt[0], length, color );
   draw_segment_right( pixels, width, height, start_pt[1], &end_pt[1], length, color );
   draw_segment_down( pixels, width, height, start_pt[2], &end_pt[2], length, color );
   draw_segment_left( pixels, width, height, start_pt[3], &end_pt[3], length, color );
   /*
   for ( int index = 0; index < fib_word_len; index++ ) {
      direction_e temp_dir = segment_directions[ index ];
      printf( "%d: temp_dir is ", ( index + 1 ) );
      print_dir( temp_dir );
      printf( "\n" );
// void draw_segment_up( uint32_t* pixels, double width, double height, point_t start_pt, point_t* end_pt, int length, uint32_t color ) {   
      if ( temp_dir == UP ) {
         draw_segment_up( pixels, width, height, start_pt, &end_pt, length, color );
      } else if ( temp_dir == DOWN ) {
         draw_segment_down( pixels, width, height, start_pt, &end_pt, length, color );
      } else if ( temp_dir == LEFT ) {
         draw_segment_left( pixels, width, height, start_pt, &end_pt, length, color );
      } else {
         draw_segment_right( pixels, width, height, start_pt, &end_pt, length, color );
      }
      start_pt.x = end_pt.x;
      start_pt.y = end_pt.y;
   }
   */

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

