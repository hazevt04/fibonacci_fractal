#ifndef _BASEINPUTPARSER_H_
#define _BASEINPUTPARSER_H_

#include "BaseInput.h"

class BaseInputParser {
   public:
      BaseInputParser() { }
      ~BaseInputParser() { }
      virtual void parseInput( int argc, char* argv[] ) = 0;
      virtual void usage( char* name ) = 0;
};

#endif
