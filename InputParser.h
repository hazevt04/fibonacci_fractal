#ifndef _INPUTPARSER_H_
#define _INPUTPARSER_H_

#include <string>

class InputParser {
   public:
      InputParser( int argc, char* argv[] );
      ~InputParser() { }
      void usage( char* argv );
      void displayArgs();
   private:
      bool verbose_flag;
      unsigned int color;
      unsigned long num_iterations;
      std::string output_file;

};

#endif
