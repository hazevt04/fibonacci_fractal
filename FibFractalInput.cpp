#include "FibFractalInput.h"

typedef unsigned long ulong;

FibFractalInput::FibFractalInput( 
            ulong background_color, 
            ulong num_iterations, 
            std::string output_file,
            bool verbose ) {
      
   this->background_color = background_color;

   this->num_iterations = num_iterations;
   this->output_file = output_file;
   this->verbose = verbose;

   this->fib_memo_table = new ulong[num_iterations + 1];  
   for( int index = 0; index <= num_iterations; index++ ) {
      this->fib_memo_table[ index ] = 0;
   }
   ulong junk = calcFibMemoized( this->fib_memo_table, num_iterations );
   this->fib_word_length = fib_memo_table[ num_iterations - 1 ];
}

void FibFractalInput::display() {
   std::cout << "Verbose Flag is " << (verbose ? "true" : "false") << std::endl;
   std::cout << "Background Color is 0x" << std::hex 
      << background_color << std::dec << std::endl;
   std::cout << "Number of iterations is " << num_iterations 
      << std::endl;
   std::cout << "Output File name is " << output_file << std::endl;
   std::cout << "Fibonacci Word Length will be " 
      << fib_word_length << std::endl;
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

