#ifndef _FIBFRACTINPUTPARSER_H_
#define _FIBFRACTINPUTPARSER_H_

#include <string>

#include "BaseInput.h"
#include "BaseInputParser.h"
#include "FibFractInput.h"

class FibFractInputParser: public BaseInputParser {
   public:
      FibFractInputParser() { }
      ~FibFractInputParser() { }
      void parseInput( int argc, char* argv[] );
      FibFractInput* getInput() { return input; }
      void usage( char* name );
   private:
      FibFractInput* input;
};

#endif
