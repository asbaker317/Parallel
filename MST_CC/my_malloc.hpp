#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

//extern "C" {
  #include <stdio.h>
  #include <sys/types.h>
  #include <fcntl.h>
  #include <sys/mman.h>
  #include "../simple-4.4J/src/simple.h"
//}

void * my_malloc(size_t size,THREADED);

#endif
