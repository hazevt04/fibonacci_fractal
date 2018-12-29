#include "ImageData.h"
#include "PNGRenderer.h"

//#include "FibFractalInputParser.h"
//#include "FibFractalInput.h"
typedef unsigned long ulong;


int main( int argc, char** argv ) {

   ulong width = 640;
   ulong height = 480;
   std::string filename = "test.png";

   ulong num_pixels = width * height;
   ImageData* data = new ImageData( width, height, filename );
   ulong black = 0x0;

   for ( ulong idx = 0; idx < num_pixels; idx++ ) {
      ulong y_val = idx/height;
      ulong x_val = idx - ( y_val * height );
     
      // Square 
      ulong x_min = ( width >> 2 );
      ulong x_max = ( ( 3 * width ) >> 2 );
      ulong y_min = ( height >> 2 );
      ulong y_max = ( ( 3 * height ) >> 2 );
      
      if ( ( x_val > x_min) && ( x_val < x_max ) &&
           ( y_val > y_min ) && ( y_val < y_max ) ) {

         data->setPixel( x_val, y_val, black );
      }
   } // end of for loop
  
   PNGRenderer* renderer = new PNGRenderer( data );
   renderer->doRender();
   
   return 0;

#if(0)
   FibFractalInputParser* parser = new FibFractalInputParser();
   parser->parseInput( argc, argv );
   FibFractalInput* input = parser->getInput();
   input->display();
   delete parser;
   delete input;
   return 0;
#endif

}
