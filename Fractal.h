#ifndef _FRACTAL_H_
#define _FRACTAL_H_

#include "BaseInput.h"

class Fractal {
   public:
      Fractal( ) { }
      Fractal( BaseInput* input ) { this->input = input; }
      ~Fractal( ) { }

      BaseInput* Getinput( ) { return input; }
   protected:
      BaseInput* input;
};

#endif
