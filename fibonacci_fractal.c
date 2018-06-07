#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <png.h>
#include <getopt.h>


// This takes the double value 'val', converts it to red, green & blue values, then
// sets those values into the image memory buffer location pointed to by 'ptr'
void set_rgb(png_byte *ptr, double val)
{
   int v = (int)(val * 767);
   if (v < 0) v = 0;
   if (v > 767) v = 767;
   int offset = v % 256;

   if (v<256) {
      ptr[0] = 0; 
      ptr[1] = 0; 
      ptr[2] = offset;
   }
   else if (v<512) {
      ptr[0] = 0; 
      ptr[1] = offset; 
      ptr[2] = 255-offset;
   }
   else {
      ptr[0] = offset; 
      ptr[1] = 255-offset; 
      //ptr[2] = 0;
      ptr[2] = 255;
   }
}

// This function actually writes out the PNG image file. The string 'title' is
// also written into the image file
int write_image(char* filename, int width, int height, double *buffer, char* title)
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


const int fib_numbers[16] = { 
	   1,    1,    2,    3, 
	   5,    8,   13,   21, 
     34,   55,   89,  144, 
    233,  377,  610,  987 
};


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


static int verbose_flag;

static struct option long_options[] = {
   { "num_iterations", required_argument, NULL, 'n' },
   { "verbose", no_argument, &verbose_flag, 1 },
   { 0, 0, 0, 0 }
};   

void usage( char* argv ) {
   printf( "Usage %s <options>\n", argv );
   printf( "num_iterations, n\n" );  
   printf( "verbose, v\n" );  
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

   char ch;
   while( (ch = getopt_long( argc, argv, "n:v", long_options, NULL )) != -1 ) {
      switch( ch ) {
         case 'n':
            num_iterations = strtod( optarg, &endptr );
            break;
         default:
            printf( "ERROR: option %c invalid", ch );
            usage( argv[0] ); 
            break; 
      }
   }

   VERBOSE_PRINTF( "num_iterations set to %d \n", num_iterations );  
   
	char* fib_word = malloc(1);

	int result = fib_recursive( num_iterations );
	
	if ( fib_numbers[num_iterations] != result ) {
		printf( "Fibonacci number %d was %d. That is wrong. It should be %d.\n", 
			num_iterations, result, fib_numbers[num_iterations] );
	} else {
		printf( "Fibonacci number %d is %d\n", num_iterations, result );
	}

	free(fib_word);
   return 0;
}




