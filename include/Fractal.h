#ifndef _FRACTAL_H_
#define _FRACTAL_H_

#include "BaseInput.h"
#include "ImageData.h"

class Fractal {
   public:
      Fractal( ) { }
      Fractal( BaseInput* input ) { this->input = input; }
      ~Fractal( ) { }

      BaseInput* Getinput( ) { return input; }
      void Setimage_data( ImageData* image_data ) { 
         this->image_data = image_data; 
      }
      ImageData* Getimage_data( ) { return image_data; }
   protected:
      BaseInput* input;
      ImageData* image_data;
};

#endif
