#ifndef _FIBFRACTINPUTPARSER_H_
#define _FIBFRACTINPUTPARSER_H_

#include <string>

#include "BaseInput.h"
#include "BaseInputParser.h"
#include "FibFractalInput.h"

class FibFractalInputParser: public BaseInputParser {
   public:
      FibFractalInputParser() { }
      ~FibFractalInputParser() { }
      void parseInput( int argc, char* argv[] );
      FibFractalInput* getInput() { return input; }
      void usage( char* name );
   private:
      FibFractalInput* input;
};

#endif
