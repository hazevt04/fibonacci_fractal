#ifndef _FRACTAL_H_
#define _FRACTAL_H_

class Fractal {
   public:
      Fractal( unsigned long num_iterations ) : 
         num_iterations( num_iterations ) { }
      ~Fractal() {}
   protected:
      unsigned long num_iterations;
};

#endif
