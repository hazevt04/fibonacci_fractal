#ifndef _FIBFRACTALBUILDER_H_
#define _FIBFRACTALBUILDER_H_

#include "Segment.h"
#include "Fractal.h"

#include "FractalBuilder.h"

class FibFractalBuilder : public FractalBuilder {
   public:
      FibFractalBuilder( FractalInput* input );
      void BuildFractal( );
      void BuildSegments( );

      Fractal* GetFractal() { return fractal; };
   private:
      Segment::Direction_e DetermineNextDirection( );
      Segment::Direction_e prevDirection;
      Fractal* fractal;
};

#endif
