#ifndef _FRACTALBUILDER_H_
#define _FRACTALBUILDER_H_

#include "Fractal.h"
#include "BaseInput.h"

class FractalBuilder {
   public:
      virtual void BuildFractal( ) { };
      virtual Fractal* GetFractal() { return 0; }
   protected:
      FractalBuilder( ) { }
      FractalBuilder( BaseInput* input );

};

#endif
