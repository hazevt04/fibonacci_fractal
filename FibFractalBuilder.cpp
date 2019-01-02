
#include <iostream>

#include "FibFractal.h"
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
}

void FibFractalBuilder::BuildFractal( ) {

   std::cout << "Building fractal word" << std::endl;
   std::string fib_word = "";
   ulong num_iterations = input->Getnum_iterations();
   ulong fib_word_length = input->Getfib_word_length();
   
   std::cout << "Need to finish rest of " << __func__ << std::endl; 
}

void FibFractalBuilder::DetermineNextDirection( std::string direction ) {
   std::string next_direction;
   if ( direction == "UP" ) {
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
   } else if ( direction == "DOWN" ) {
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
   } else if ( direction == "LEFT" ) {
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
   } else if ( direction == "RIGHT" ) {
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
