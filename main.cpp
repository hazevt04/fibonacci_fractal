#include "FibFractInputParser.h"
#include "FibFractInput.h"

int main( int argc, char** argv ) {
   FibFractInputParser* parser = new FibFractInputParser();
   parser->parseInput( argc, argv );
   FibFractInput* input = parser->getInput();
   input->display();
   delete parser;
   delete input;
   return 0;
}
