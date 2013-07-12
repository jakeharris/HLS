#ifndef HMS_H
#define HMS_H

// #include "bq.h"

#include <iostream>
#include <pthread.h>

namespace hms {
  /* Read text into a queue. */
  void * producer(void * arg);
  
  /* Replace spaces with asterisks. */
  void * crunch(void * arg);
  
  /* Uppercase all letters. */
  void * gobble(void * arg);
  
  /* Write text to a file. */
  void * consumer(void * arg);
}

#endif
