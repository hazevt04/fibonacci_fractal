#ifndef _BASEINPUT_H_
#define _BASEINPUT_H_

#include <stdbool.h>

class BaseInput {
   public:
      BaseInput() { }
      BaseInput( bool verbose ) {
         this->verbose = false;
      }

      ~BaseInput() { }
   protected:
      bool verbose;
};

#endif
