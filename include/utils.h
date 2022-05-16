#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <iostream>
#include <utility>


// From stackoverflow. parameter pack expansion
template <typename Arg, typename... Args>
void doPrint(bool verbose, std::ostream& out, Arg&& arg, Args&&... args) {
   if ( verbose ) {
      out << std::forward<Arg>(arg);
      using expander = int[];
      (void)expander{0, (void(out << std::forward<Args>(args)), 0)...};
   }
}

#endif
