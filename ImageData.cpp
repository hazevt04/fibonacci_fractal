#include "ImageData.h"

ImageData::ImageData( ulong width, ulong height, std::string filename ) {
   
   ulong num_pixels = width * height;

   this->width = width;
   this->height = height;
   this->num_pixels = width * height;
   this->pixels = new ulong[num_pixels];
   this->filename = filename;
   
   ulong white = 0xffffff;

   if (pixels) {
      for ( ulong idx = 0; idx < num_pixels; idx++ ) {
         this->pixels[ idx ] = white;
      }
   }
}

ImageData::~ImageData() {
   if (this->pixels) {
      delete [] this->pixels;
   }
}

void ImageData::setPixel( ulong idx, ulong val ) {
   if ( idx < this->num_pixels ) {
      if ( this->pixels ) {
         pixels[ idx ] = val;
      }
   }
}

void ImageData::setPixel( ulong x, ulong y, ulong val ) {
   if ( ( x <= this->width )  && ( y <= this->height ) ) {
      if ( this->pixels ) {
         pixels[ ( y * this->width ) + x ] = val;
      }
   }
}
