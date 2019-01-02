#ifndef _FIBFRACTALBUILDER_H_
#define _FIBFRACTALBUILDER_H_

#include "FibFractalInput.h"
#include "FibFractal.h"
#include "FractalBuilder.h"

class FibFractalBuilder: public FractalBuilder {
   public:
      FibFractalBuilder( ) { }
      FibFractalBuilder( BaseInput* input );
      void BuildFractal( );
      void BuildFractalWord( );
      void BuildFractalDirections( );

      virtual Fractal* GetFractal( ) { return fractal; };
   private:
      std::string Getcurrent_direction( ) { return current_direction; }
      void DetermineNextDirection( std::string direction );
      std::string current_direction;

      FibFractal* fractal; 
      FibFractalInput* input;
};

#endif
