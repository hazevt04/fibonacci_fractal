#ifndef _FIBFRACTALBUILDER_H_
#define _FIBFRACTALBUILDER_H_

#include "FibFractalInput.h"
#include "Fractal.h"
#include "FractalBuilder.h"

class FibFractalBuilder: public FractalBuilder {
   public:
      FibFractalBuilder( ) { }
      FibFractalBuilder( BaseInput* input );
      void BuildFractal( );

      virtual Fractal* GetFractal( ) { return fractal; };
   private:
      void DetermineNextDirection( std::string direction );
      std::string current_direction;

      Fractal* fractal; 
      FibFractalInput* input;
};

#endif
