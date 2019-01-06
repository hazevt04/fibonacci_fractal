#include "ImageData.h"

ImageData::ImageData( ulong num_iterations, std::string filename ) {
   ulong height;
   ulong width;
   ulong num_pixels;
   if ( num_iterations > 23 ) {
      width = 16530;
      height = 16530;
   } else {
      width = 8192;
      height = 8192;
   }
   num_pixels = width * height;
   const ulong BLACK = 0x00000000;
   const ulong RED	= 0x00ff0000;
   const ulong BLUE	= 0x000000ff;
   const ulong WHITE = 0x00ffffff;	
   
   this->width = width;
   this->height = height;
   this->pixels = new ulong[num_pixels];
   this->num_pixels = num_pixels;
 
   if ( this->pixels ) {
      for ( ulong index = 0; index < num_pixels; index++ ) {
         pixels[ index ] = WHITE;
      }
   }
   this->filename = filename;

}

ImageData::ImageData( ulong width, ulong height, std::string filename ) {
   
   ulong num_pixels = width * height;

   this->width = width;
   this->height = height;
   this->num_pixels = width * height;
   this->pixels = new ulong[num_pixels];
   this->filename = filename;
   
   const ulong BLACK = 0x00000000;
   const ulong RED	= 0x00ff0000;
   const ulong BLUE	= 0x000000ff;
   const ulong WHITE = 0x00ffffff;	
   

   if ( this->pixels ) {
      for ( ulong index = 0; index < num_pixels; index++ ) {
         this->pixels[ index ] = WHITE;
      }
   }
}

ImageData::~ImageData() {
   if (this->pixels) {
      delete [] this->pixels;
   }
}

void ImageData::Setpixel( ulong index, ulong val ) {
   if ( index < this->num_pixels ) {
      if ( this->pixels ) {
         pixels[ index ] = val;
      }
   }
}

void ImageData::Setpixel( ulong x, ulong y, ulong val ) {
   if ( ( x <= this->width )  && ( y <= this->height ) ) {
      if ( this->pixels ) {
         pixels[ ( y * this->width ) + x ] = val;
      }
   }
}
