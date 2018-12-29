#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <iostream>

#include "ImageData.h"

class Renderer {
   public:
      Renderer() { }
      Renderer( ImageData* data ) { }
      ~Renderer() { }
      virtual void doRender() = 0;
   protected:
      ImageData* data;
};

#endif
