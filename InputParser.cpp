#include <getopt.h>
#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>

#include "utils.h"

static int verbose_flag;

static struct option long_options[] = {
   { "help", no_argument, NULL, 'h' },
   { "verbose", no_argument, NULL, 'v' },
   { "num_iterations", required_argument, NULL, 'n' },
   { "color", optional_argument, NULL, 'c' },
   { "output_file", optional_argument, NULL, 'o' },
   { NULL, 0, NULL, 0 }
};

class InputParser {
   public:
      InputParser( int argc, char* argv[] ) {

         char* pwd = std::getenv( "PWD" );
         
         this->output_file = "/fib_fractal";
         this->output_file.insert( 0, pwd );

         char* endptr = NULL;

         int option_index = 0;
         verbose_flag = 0;

         //int error_flag = 0;
         //int help_flag = 0;
         int ch = getopt_long( argc, argv, "hvn:c:o:", long_options, &option_index );
         while( ch != -1 ) {
            switch( ch ) {
               case 0:
                  // For verbose flag
                  // There is no argument after the flag in this case
                  if ( long_options[option_index].flag != 0 ) {
                     break;
                  }
                  break;
               case 'h':
                  usage( argv[0] ); 
                  exit( EXIT_SUCCESS );
                  break;
               case 'v':
                  this->verbose_flag = 1;
                  break;
               case 'n':
                  this->num_iterations = std::stoul( optarg, 0, 10 );
                  break;
               case 'c':
                  this->color = std::stoul( optarg, 0, 16 );
                  this->color &= 0xffffff;
                  break;
               case 'o': 
                  this->output_file = optarg;
                  break;
               default:
                  usage( argv[0] ); 
                  exit( EXIT_FAILURE );
                  break;
            } // end of switch
            
            ch = getopt_long( argc, argv, "hvn:c:o:", long_options, &option_index );
         } // end of while loop

      } // end of InputParser() constructor
      
      ~InputParser() { }

      void usage( char* argv ) {
         std::cout << "Usage " << argv << " <options>" << std::endl;
         std::cout << "Options one of: " << std::endl;
         std::cout << std::setw(20) << "--verbose"
                   << std::setw(4) << "-v" 
                   << std::setw(56) << "Verbose output" << std::endl; 
         
         std::cout << std::setw(20) << "--help"
                   << std::setw(4) << "-h" 
                   << std::setw(56) << "Show this usage message" << std::endl; 
         
         std::cout << std::setw(20) << "--num_iterations"
                   << std::setw(4) << "-n" 
                   << std::setw(56) << "Number of iterations" << std::endl; 
         
         std::cout << std::setw(20) << "--color"
                   << std::setw(4) << "-c" 
                   << std::setw(56) << "Color of the Fibonacci Fractal" << std::endl; 
         
         std::cout << std::setw(20) << "--output_file"
                   << std::setw(4) << "-o" 
                   << std::setw(56) << "Path to the output file for the fractal PNG" << std::endl; 
         
      }

      void displayArgs() {
         std::cout << "Verbose Flag is " << (verbose_flag ? "true" : "false") << std::endl;
         std::cout << "Color is " << std::setbase(16) << color << std::resetiosflags() << std::endl;
         std::cout << "Number of iterations is " << num_iterations << std::endl;
         std::cout << "Output File name is " << output_file << std::endl;
      }

   private:
      bool verbose_flag;
      unsigned int color;
      unsigned long num_iterations;
      std::string output_file;

};

int main( int argc, char** argv ) {
   
   InputParser parser( argc, argv );
   parser.displayArgs();
   return 0;
}

