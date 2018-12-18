#ifndef _SEGMENT_H_
#define _SEGMENT_H_

class Segment {
   public:
      enum {UP, DOWN, LEFT, RIGHT} Direction_e;
      Direction_e dir;
      void display();
      unsigned long index;

};

#endif

