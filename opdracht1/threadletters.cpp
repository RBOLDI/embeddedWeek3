// w3.1.1multithreading

#include "multithreading.hpp"

void *ThreadLetters(void* pThreadData) {

  long nTid = (long)pThreadData;

  printf("ThreadLetters(%ld)\n", nTid);

  // Ga door tot het programma niet meer draait
  while (true) {

    for (char c = 'A'; c <= 'Z'; c++) {
      printf("%c", c); fflush(stdout);
      //TODO: zorg voor en delay van 500 msec
      thread_delay(500);

      // Stoppen?
      if (!_bRunning) {
        pthread_exit(NULL);
      };
    };
  };

  // Thread vrijgeven
  pthread_exit(NULL);
};