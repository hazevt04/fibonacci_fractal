#include <getopt.h>
#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>

#include "utils.h"
#include "BaseInput.h"
#include "FibFractalInput.h"
#include "FibFractalInputParser.h"

static int verbose_flag;

static struct option long_options[] = {
   { "help", no_argument, NULL, 'h' },
   { "verbose", no_argument, NULL, 'v' },
   { "num_iterations", required_argument, NULL, 'n' },
   { "background_color", optional_argument, NULL, 'c' },
   { "output_file", optional_argument, NULL, 'o' },
   { NULL, 0, NULL, 0 }
};

void FibFractalInputParser::parseInput( int argc, char* argv[] ) {

   char* pwd = std::getenv( "PWD" );
   
   std::string output_file;
   unsigned long background_color = 0;
   unsigned long num_iterations = 1;
   bool verbose = false;

   output_file = "/fib_fractal.png";
   output_file.insert( 0, pwd );

   char* endptr = NULL;

   int option_index = 0;
   verbose_flag = 0;

   int ch = getopt_long( argc, argv, "hvn:c:o:", long_options, 
         &option_index );
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
            verbose_flag = 1;
            break;
         case 'n':
            num_iterations = std::stoul( optarg, 0, 10 );
            break;
         case 'c':
            background_color = std::stoul( optarg, 0, 16 );
            background_color &= 0xffffff;
            break;
         case 'o': 
            output_file = optarg;
            break;
         default:
            usage( argv[0] ); 
            exit( EXIT_FAILURE );
            break;
      } // end of switch
      
      ch = getopt_long( argc, argv, "hvn:c:o:", long_options, &option_index );
   } // end of while loop

   this->input = new FibFractalInput( background_color, num_iterations, output_file, verbose_flag );
   //this->input->display();
} // end of FibFractalInputParser() constructor


void FibFractalInputParser::usage( char* name ) {
   std::cout << "Usage " << name << " <options>" << "\n";
   std::cout << "Options one of: " << "\n";
   std::cout << std::setw(20) << "--verbose"
             << std::setw(4) << "-v" 
             << std::setw(56) << "Verbose output" << "\n"; 
   
   std::cout << std::setw(20) << "--help"
             << std::setw(4) << "-h" 
             << std::setw(56) << "Show this usage message" << "\n"; 
   
   std::cout << std::setw(20) << "--num_iterations"
             << std::setw(4) << "-n" 
             << std::setw(56) << "Number of iterations" << "\n"; 
   
   std::cout << std::setw(20) << "--background_color"
             << std::setw(4) << "-c" 
             << std::setw(56) << "Background Color of the Fibonacci Fractal" << "\n";

   std::cout << std::setw(80) << "(6 RGB hex chars, ex: 0xffffff for white)" << "\n"; 
   
   std::cout << std::setw(20) << "--output_file"
             << std::setw(4) << "-o" 
             << std::setw(56) << "Path to the output file for the fractal PNG" << std::endl; 
   
}



