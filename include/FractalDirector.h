#ifndef _FRACTALDIRECTOR_H_
#define _FRACTALDIRECTOR_H_

#include "Fractal.h"
#include "FractalBuilder.h"

class FractalDirector {
   public:
      void setBuilder( FractalBuilder* builder ) {
         this->builder = builder;
      }
      Fractal* CreateFractal( FractalBuilder& builder );
   protected:
      FractalBuilder* builder;
};


#endif
