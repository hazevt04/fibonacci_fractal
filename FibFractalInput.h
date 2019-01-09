#ifndef _FIBFRACTINPUT_H_
#define _FIBFRACTINPUT_H_

#include <iostream>
#include <string>

#include "BaseInput.h"

typedef unsigned long ulong;

class FibFractalInput: public BaseInput {
   public:
      FibFractalInput();
      ~FibFractalInput() { delete [] fib_memo_table; }

      FibFractalInput( ulong background_color, 
            ulong num_iterations, 
            std::string output_file,
            bool verbose );

      void display();
   
      ulong Getbackground_color( ) { return background_color; }
      ulong Getnum_iterations( ) { return num_iterations; }
      std::string Getoutput_file( ) { return output_file; }
      ulong Getfib_word_length( ) { return fib_word_length; }

   private:
      ulong* fib_memo_table;
      ulong calcFibMemoized( ulong* table, ulong num );
      
      ulong background_color;
      ulong num_iterations;
      std::string output_file;
      ulong fib_word_length;
};

#endif
