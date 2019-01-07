
#include <sstream>
#include <iostream>

#include "FibFractalInput.h"
#include "ImageData.h"

#include "FibFractalBuilder.h"

FibFractalBuilder::FibFractalBuilder( BaseInput* input ) {
   std::cout << __func__ << ": "<< std::endl;
   
   input->display();

   this->input = dynamic_cast<FibFractalInput* >(input);
   if ( this->input == NULL ) {
      std::cerr << "Could not case BaseInput input pointer"
         << " to FibFractalInput pointer."
         << std::endl;
   }
   
   this->fractal = new FibFractal( this->input );
   this->current_direction = "UP";
}

void FibFractalBuilder::BuildFractal( ) {

   BuildFractalWord( );
   BuildFractalDirections( );
   BuildFractalImageData( );

}

void FibFractalBuilder::BuildFractalWord( ) {
   std::cout << "Building fractal word" << std::endl;
   const ulong num_iterations = input->Getnum_iterations();
   const ulong fib_word_length = input->Getfib_word_length();
   std::string t_fib_words[ num_iterations ];

   if ( num_iterations > 2 ) {
      t_fib_words[ 0 ] = "1";
      t_fib_words[ 1 ] = "0";
      for ( ulong index = 2; index < num_iterations; index++ ) {
        t_fib_words[ index ] += t_fib_words[ index - 1 ]; 
        t_fib_words[ index ] += t_fib_words[ index - 2 ]; 
      }
      this->fractal->Setfib_word( t_fib_words[ num_iterations - 1 ] );
   } else if ( num_iterations == 2 ) {
      this->fractal->Setfib_word( "10" ); 
   } else {
      this->fractal->Setfib_word( "1" ); 
   }

   std::cout << "Fibonacci Word is " << this->fractal->Getfib_word( ) 
      << std::endl;
}


void FibFractalBuilder::BuildFractalDirections( ) {
   std::cout << "Building fractal directions strings" << std::endl;

   const ulong fib_word_length = input->Getfib_word_length( );
   const std::string fib_word = this->fractal->Getfib_word( );

   std::string segment_directions[ fib_word_length + 1 ];
   std::string temp_dir;
   ulong segment_index = 0;
   for ( ulong index = 0; index < fib_word_length; index++ ) {
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

   std::string fib_directions = "";
   for ( long index = 0; index < fib_word_length; index++ ) {
      fib_directions += segment_directions[ index ];
      fib_directions += " ";
   }
   std::cout << __func__ << ": fib_directions = " 
      << fib_directions << std::endl;

   this->fractal->Setfib_directions( fib_directions );
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

void FibFractalBuilder::draw_segment_right( ulong* pixels, ulong width, 
      ulong height, ulong start_index, ulong* end_index, ulong length,
      ulong color ) {

   ulong index             = 0;
   ulong max_overall_index = width * height;
   ulong last_index        = ( start_index + length );
   for ( index = start_index; index < last_index; index++ ) {
      if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
         pixels[ index ] = color;
      }
   }
   *end_index = last_index;
}

void FibFractalBuilder::draw_segment_left( ulong* pixels, ulong width, 
      ulong height, ulong start_index, ulong* end_index, ulong length,
      ulong color ) {

   ulong index             = 0;
   ulong max_overall_index = width * height;
   ulong last_index        = start_index;

   if ( start_index > length ) {
      last_index = ( start_index - length );
      for ( index = start_index; index >= last_index; index-- ) {
         if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
            pixels[ index ] = color;
         }
      }
   }
   *end_index = last_index;
}

void FibFractalBuilder::draw_segment_up( ulong* pixels, ulong width, 
      ulong height, ulong start_index, ulong* end_index, ulong length,
      ulong color ) {

   ulong index             = 0;
   ulong max_overall_index = width * height;
   ulong last_index        = start_index;
   if ( start_index > ( length * width ) ) {
      last_index        = ( start_index - ( length * width ) );
      for ( index = start_index; index > last_index; index -= width ) {
         if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
            pixels[ index ] = color;
         }
      }
   }
   *end_index = index;
}

void FibFractalBuilder::draw_segment_down( ulong* pixels, ulong width, 
      ulong height, ulong start_index, ulong* end_index, ulong length,
      ulong color ) {

   ulong index             = 0;
   ulong max_overall_index = width * height;
   ulong last_index        = ( ulong )( start_index + ( length * width ) );
   for ( index = start_index; index < last_index; index += width ) {
      if ( ( index >= 0 ) && ( index < max_overall_index ) ) {
         pixels[ index ] = color;
      }
   }
   *end_index = index;
}

void FibFractalBuilder::BuildFractalImageData( ) {
   std::cout << __func__ << ": started." << std::endl;

   ImageData* image_data = this->fractal->Getimage_data( );
   ulong* pixels = image_data->Getpixels( );
   ulong width = image_data->Getwidth( );
   ulong height = image_data->Getheight( );
   const ulong fib_word_length = this->input->Getfib_word_length( );

   ulong start_index = ( width * ( height - 15 ) ) + 15;
   ulong end_index;
   ulong length   = 5;

   std::string fib_directions = this->fractal->Getfib_directions( );
   std::istringstream isstream( fib_directions );
   std::string temp_direction;

   for ( ulong index = 0; index < fib_word_length; index++ ) {

      if ( std::getline( isstream, temp_direction, ' ' ) ) {
         std::cout << __func__ << ": "
            << index << ": " << temp_direction << std::endl;

         ulong color = ( index & 0x00ffffff ) | 0x3;

         if ( temp_direction == "UP" ) {
            draw_segment_up( pixels, width, height, start_index,
                               &end_index, length, color );
         } else if ( temp_direction == "DOWN" ) {
            draw_segment_down( pixels, width, height, start_index,
                               &end_index, length, color );
         } else if ( temp_direction == "LEFT" ) {
            draw_segment_left( pixels, width, height, start_index,
                               &end_index, length, color );
         } else if ( temp_direction == "RIGHT" ) {
            draw_segment_right( pixels, width, height, start_index,
                                &end_index, length, color );
         } else {
            std::cerr << __func__ << ": ERROR: temp_direction[ "
               << index << " ] (" << temp_direction 
               << ") is invalid." << std::endl;
            exit( EXIT_FAILURE );
         }
         start_index = end_index;
      } else {
         std::cerr << __func__ << "ERROR: getline failed unexpectedly."
            << "Index is " << index << std::endl;
         exit( EXIT_FAILURE );
      } // end of if getline
   } // end of for loop

   std::cout << __func__ << ": done." << std::endl;
   
}
