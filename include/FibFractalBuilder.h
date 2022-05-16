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
      void BuildFractalImageData( );

      virtual Fractal* GetFractal( ) { return fractal; };
   private:
      std::string Getcurrent_direction( ) { return current_direction; }
      void DetermineNextDirection( std::string direction );
      std::string current_direction;

      void draw_segment_up( ulong* pixels, ulong width, 
         ulong height, ulong start_index, ulong* end_index, ulong length,
         ulong color );
      void draw_segment_down( ulong* pixels, ulong width, 
         ulong height, ulong start_index, ulong* end_index, ulong length,
         ulong color );
      void draw_segment_left( ulong* pixels, ulong width, 
         ulong height, ulong start_index, ulong* end_index, ulong length,
         ulong color );
      void draw_segment_right( ulong* pixels, ulong width, 
         ulong height, ulong start_index, ulong* end_index, ulong length,
         ulong color );

      FibFractal* fractal; 
      FibFractalInput* input;
};

#endif
