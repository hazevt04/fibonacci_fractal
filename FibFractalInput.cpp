#include "FibFractalInput.h"

typedef unsigned long ulong;

FibFractalInput::FibFractalInput( 
            ulong color, 
            ulong num_iterations, 
            std::string output_file,
            bool verbose ) {
      
   this->color = color;
   this->num_iterations = num_iterations;
   this->output_file = output_file;
   this->verbose = verbose;

   this->fib_memo_table = new ulong[num_iterations];  
   this->fib_word_length = calcFibMemoized( this->fib_memo_table, num_iterations );
}

void FibFractalInput::display() {
   std::cout << "Verbose Flag is " << (verbose ? "true" : "false") << std::endl;
   std::cout << "Color is 0x" << std::hex << color << std::dec << std::endl;
   std::cout << "Number of iterations is " << num_iterations << std::endl;
   std::cout << "Output File name is " << output_file << std::endl;
   std::cout << "Fibonacci Word Length will be " << fib_word_length << std::endl;
}

ulong FibFractalInput::calcFibMemoized( ulong* table, ulong num ) {
   ulong result;
   if ( num  < 2 ) {
      return 1;
   } else {
      // If Fib(num) is in the table
		// return it
      if ( 0 != table[ num ] ) {
         return table[ num ];
      } else {
			// Fib(num) not in table; calculate it.
         result = calcFibMemoized( table, ( num - 1 ) ) +
            calcFibMemoized( table, ( num - 2 ) );
         table[ num ] = result;
         return result;
      } // end of else if table[num] == 0
   } // end of else num >= 2
}
