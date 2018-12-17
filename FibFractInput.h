#ifndef _FIBFRACTINPUT_H_
#define _FIBFRACTINPUT_H_

#include <iostream>
#include <string>

#include "BaseInput.h"

class FibFractInput: public BaseInput {
   public:
      FibFractInput();
      ~FibFractInput() { }

      FibFractInput( unsigned int color, 
            unsigned long num_iterations, 
            std::string output_file,
            bool verbose ) {
      
         this->color = color;
         this->num_iterations = num_iterations;
         this->output_file = output_file;
         this->verbose = verbose;
      }

      void display() {
         std::cout << "Verbose Flag is " << (verbose ? "true" : "false") << std::endl;
         std::cout << "Color is " << std::hex << color << std::dec << std::endl;
         std::cout << "Number of iterations is " << num_iterations << std::endl;
         std::cout << "Output File name is " << output_file << std::endl;
      }

   private:
      unsigned int color;
      unsigned long num_iterations;
      std::string output_file;
};

#endif
