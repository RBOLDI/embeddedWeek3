// w3.1.2hwdriver

#include "hwdriver.hpp"

void *Threads(void* pThreadData) {

  long nNr = (long)pThreadData;

  // Ga door tot drukknop 1
  while (true) {

    // Toon unieke thread tekst
    printf("T[%ld] ", nNr); fflush(stdout);
    thread_delay(250 + nNr * 100);

    // Stoppen?
    if (oKnopStop.pushed()) {
      _bRunning = false;
      pthread_exit(NULL);
    };
  };

  // Thread vrijgeven
  pthread_exit(NULL);
};