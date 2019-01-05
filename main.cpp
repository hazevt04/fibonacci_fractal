
#include "FibFractalInputParser.h"
#include "FibFractalInput.h"
#include "FibFractalBuilder.h"
#include "ImageData.h"
#include "PNGRenderer.h"
typedef unsigned long ulong;


int main( int argc, char** argv ) {

   FibFractalInputParser* parser = new FibFractalInputParser();
   parser->parseInput( argc, argv );
   
   FibFractalInput* input = parser->getInput();
   input->display();

   FibFractalBuilder* builder = new FibFractalBuilder( input );
   builder->BuildFractal( );

   PNGRenderer* renderer = new PNGRenderer( input );
   delete builder; 
   delete input;
   delete parser;
   return 0;

}
