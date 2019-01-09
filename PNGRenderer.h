#ifndef _PNGRENDERER_H_
#define _PNGRENDERER_H_

#include <iostream>
#include <string>

#include <stdio.h>
#include <png.h>

#include "ImageData.h"
#include "Renderer.h"
typedef unsigned long ulong;

class PNGRenderer : public Renderer {
   public:
      PNGRenderer() { };
      PNGRenderer( ImageData* image_data );
      ~PNGRenderer( );
      void doRender(); 
      void set_rgb( png_byte* ptr, ulong val );
      int write_png( char* filename, int width, 
            int height, ulong* buffer, char* title );
   private:
      ImageData* image_data;
      FILE* fp;
      png_structp png_ptr;
      png_infop info_ptr;
      png_bytep row;
            
};

#endif

