
#include <iostream>
#include <array>

//#include "FibFractal.h"
#include "FibFractalInput.h"
#include "FibFractalBuilder.h"

FibFractalBuilder::FibFractalBuilder( BaseInput* input ) {
   std::cout << __func__ << ": "<< std::endl;
   
   input->display();

   this->input = dynamic_cast<FibFractalInput* >(input);
   if ( this->input == NULL ) {
      std::cerr << "Could not case BaseInput input pointer to FibFractalInput pointer."
         << std::endl;
   }
   
   this->fractal = new FibFractal( this->input );
   this->current_direction = "UP";
}

void FibFractalBuilder::BuildFractal( ) {

   BuildFractalWord( );
   BuildFractalDirections( );
}

void FibFractalBuilder::BuildFractalWord( ) {
   std::cout << "Building fractal word" << std::endl;
   const ulong num_iterations = input->Getnum_iterations();
   const ulong fib_word_length = input->Getfib_word_length();
   std::string t_fib_words[ num_iterations ];

   if ( num_iterations > 2 ) {
      t_fib_words[ 0 ] = "1";
      t_fib_words[ 1 ] = "0";
      for ( int index = 2; index < num_iterations; index++ ) {
        t_fib_words[ index ] += t_fib_words[ index - 1 ]; 
        t_fib_words[ index ] += t_fib_words[ index - 2 ]; 
      }
      this->fractal->Setfib_word( t_fib_words[ num_iterations - 1 ] );
   } else if ( num_iterations == 2 ) {
      this->fractal->Setfib_word( "10" ); 
   } else {
      this->fractal->Setfib_word( "1" ); 
   }

   std::cout << "Fibonacci Word is " << this->fractal->Getfib_word( ) << std::endl;
}


void FibFractalBuilder::BuildFractalDirections( ) {
   std::cout << "Building fractal directions strings" << std::endl;

   const ulong fib_word_length = input->Getfib_word_length( );
   const std::string fib_word = this->fractal->Getfib_word( );

   std::string segment_directions[ fib_word_length + 1 ];
   std::string temp_dir;
   ulong segment_index = 0;
   for ( long index = 0; index < fib_word_length; index++ ) {
      temp_dir = "FORWARD";
      // updates current_direction
      DetermineNextDirection( temp_dir );

      segment_directions[ segment_index++ ] = Getcurrent_direction();
      if ( fib_word[ index ] == '0' ) {
         // If odd
         if ( ( index + 1 ) & 1 ) {
            temp_dir = "RIGHT";
         } else {
            temp_dir = "LEFT";
         }
         // updates current_direction
         DetermineNextDirection( temp_dir );
      }
   } // end of for loop   

   for ( long index = 0; index < fib_word_length; index++ ) {
      std::cout << index << ": " << segment_directions[ index ] << std::endl;
   }

}

void FibFractalBuilder::DetermineNextDirection( std::string direction ) {
   std::string next_direction;
   if ( this->current_direction == "UP" ) {
      if ( direction == "FORWARD" ) {
         next_direction = this->current_direction;
      } else if ( direction != "DOWN" ) {
         next_direction = direction;
      } else {
         std::cerr << "ERROR: current direction is "
            << this->current_direction << ". "
            << "Invalid input direction " << direction
            << std::endl;
         exit( EXIT_FAILURE );
      }
   } else if ( this->current_direction == "DOWN" ) {
      if ( direction == "LEFT" ) {
         next_direction = "RIGHT";
      } else if ( direction == "RIGHT" ) {
         next_direction = "LEFT";
      } else if ( direction == "FORWARD" || direction == "DOWN" ) {
         next_direction = this->current_direction;
      } else {
         std::cerr << "ERROR: current direction is "
            << this->current_direction << ". "
            << "Invalid input direction " << direction
            << std::endl;
         exit( EXIT_FAILURE );
      }
   } else if ( this->current_direction == "LEFT" ) {
      if ( direction == "RIGHT" ) {
         next_direction = "UP";
      } else if ( direction == "LEFT" ) {
         next_direction = "DOWN";
      } else if ( direction == "FORWARD" ) {
         next_direction = this->current_direction;
      } else {
         std::cerr << "ERROR: current direction is "
            << this->current_direction << ". "
            << "Invalid input direction " << direction
            << std::endl;
         exit( EXIT_FAILURE );
      }
   } else if ( this->current_direction == "RIGHT" ) {
      if ( direction == "LEFT" ) {
         next_direction = "UP";
      } else if ( direction == "RIGHT" ) {
         next_direction = "DOWN";
      } else if ( direction == "FORWARD" ) {
         next_direction = this->current_direction;
      } else {
         std::cerr << "ERROR: current direction is "
            << this->current_direction << ". "
            << "Invalid input direction " << direction
            << std::endl;
         exit( EXIT_FAILURE );
      }
   }
   this->current_direction = next_direction;
}
