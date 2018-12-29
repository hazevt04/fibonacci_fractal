#ifndef _IMAGEDATA_H_
#define _IMAGEDATA_H_

#include <string>

typedef unsigned long ulong;

class ImageData {
   public:
      ImageData( ulong width, ulong height, std::string filename );
      ~ImageData();
      void setPixel( ulong idx, ulong val );
      void setPixel( ulong x, ulong y, ulong val );
      ulong* getPixels( ) { return pixels; }
      ulong width;
      ulong height;
      ulong num_pixels;
      std::string filename;
   private:
      ulong* pixels;
};

#endif
