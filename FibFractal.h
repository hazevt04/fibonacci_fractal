#ifndef _FIBFRACTAL_H
#define _FIBFRACTAL_H_

#include <string>

#include "Fractal.h"
#include "FibFractalInput.h"

class FibFractal: public Fractal {
   public:
      FibFractal( ) { } 
      FibFractal( std::string fib_word, std::string fib_directions ) { 
         this->fib_word = fib_word;
         this->fib_directions = fib_directions;
      } 
      ~FibFractal( ) { }

      void Setfib_word( std::string fib_word ) { this->fib_word = fib_word; }
      void Setfib_directions( std::string fib_directions ) { 
         this->fib_directions = fib_directions; 
      }
      std::string Getfib_word( ) { return fib_word; }
      std::string Getfib_directions( ) { return fib_directions; }
   private:
      // binary string
      std::string fib_word;
      // string with directions based on the fib_word
      std::string fib_directions;

};

#endif
