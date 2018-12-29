#ifndef _FIBFRACTAL_H
#define _FIBFRACTAL_H_

#include <list>

#include "Fractal.h"
#include "Segment.h"

class FibFractal {
   public:
      FibFractal( unsigned long num_iterations = 1 ) :
         num_iterations( num_iterations ) { }
      ~FibFractal() { }
      Segment* getSegment( unsigned long num_segment ) {
         return &(segments[num_segment]);
      }
      void render() { renderer->doRender(); }
   private:
      list<Segment>* segments;
      Renderer* renderer;
};

#endif
