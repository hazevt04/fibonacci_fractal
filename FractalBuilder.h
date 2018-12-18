#ifndef _FRACTALBUILDER_H_
#define _FRACTALBUILDER_H_

#include "Fractal.h"
#include "FractalInput.h"

class FractalBuilder {
   public:
      virtual void BuildFractal( );
      virtual void BuildSegments( );
      virtual Fractal* GetFractal() { return 0; }
   protected:
      FractalBuilder( FractalInput* input );

};

#endif
