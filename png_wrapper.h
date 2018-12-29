#ifdef _PNG_WRAPPER_H_
#define _PNG_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <png.h>

typedef unsigned long ulong;

void set_rgb( png_byte* ptr, ulong val );

int write_png( char* filename, int width, int height, ulong* buffer,
                 char* title )

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif
