// w3.1.1multithreading

#include "multithreading.hpp"

void *ThreadOptellen(void* pThreadData) {

  char *pText = (char*)pThreadData;
  int a, b;

  printf("Optellen %s:\n", pText);

  // Ga door tot een foute invoer
  while (true) {

    if (scanf("%d+%d", &a, &b) != 2) {
      _bRunning = false;
      pthread_exit(NULL);
    };
    printf("=%d\n", a+b); fflush(stdout);
  };

  // Thread vrijgeven
  pthread_exit(NULL);
};