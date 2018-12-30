
#include "FibFractalInputParser.h"
#include "FibFractalInput.h"
#include "ImageData.h"
#include "PNGRenderer.h"
typedef unsigned long ulong;


int main( int argc, char** argv ) {

   FibFractalInputParser* parser = new FibFractalInputParser();
   parser->parseInput( argc, argv );
   
   FibFractalInput* input = parser->getInput();
   input->display();


   
   delete parser;
   delete input;
   return 0;

}
