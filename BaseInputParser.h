#ifndef _BASEINPUTPARSER_H_
#define _BASEINPUTPARSER_H_

#include "BaseInput.h"

class BaseInputParser {
   public:
      BaseInputParser() { }
      ~BaseInputParser() { }
      virtual void parseInput( int argc, char* argv[] ) = 0;
      BaseInput* getInput() { return input; }
      virtual void usage( char* name ) = 0;
   protected:
      BaseInput* input;
};

#endif
