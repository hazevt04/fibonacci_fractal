#ifndef _FRACTALBUILDER_H_
#define _FRACTALBUILDER_H_

#include "Fractal.h"
#include "FractalInput.h"

class FractalBuilder {
   public:
      virtual void BuildFractal( ) = 0;
      virtual void BuildSegments( ) = 0;
      virtual Fractal* GetFractal() { return 0; }
   protected:
      FractalBuilder( FractalInput* input );

};

#endif
