#ifndef _SEGMENT_H_
#define _SEGMENT_H_

#include <iostream>

typedef unsigned long ulong;

class Segment {
   public:
      enum {UP, DOWN, LEFT, RIGHT} Direction_e;
      Direction_e dir;
      void display( ) { std::cout << this->dir.name() << std::endl; }
      void display( ulong num ) { 
         std::cout << num << ":" << this->dir.name() << std::endl; }
      unsigned long index;

};

#endif

