// w3.1.3hwlayer

//TODO: zorg voor de juiste # commando's
#ifndef _OSTHREAD_HPP_
#define _OSTHREAD_HPP_

  #include <bcm2835.h>
  #include <stdbool.h>
  #include <stdio.h>
  #include <pthread.h>
  #include <iostream>
  //TODO: zorg dat de Knop en Led class bekend zijn
  #include "ncursesio.hpp"
  #include "knop.hpp"
  #include "led.hpp"

  // De OS thread hardware laag class
  class OsThread {
    public:
      OsThread();
      ~OsThread();
      void join();

    private:
      pthread_t oThread_;
      pthread_attr_t oAttr_;
      knop oHwStop_;
      knop oHwKnop_;
      led oLed1_;
      led oLed2_;

      void delay(long nMSec);
      static void *thread(void *pThreadData);
  };

#endif // _OSTHREAD_HPP_