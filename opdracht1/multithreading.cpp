// w3.1.1multithreading
// g++ -Wall -pthread -o multithreading multithreading.cpp threadletters.cpp threadoptellen.cpp
// ./multithreading

#include "multithreading.hpp"

// Het programma begint
bool _bRunning = true;

int main(int nArgc, char* aArgv[]) {

  int nRet;
  pthread_t oThreadLetters;
  pthread_t oThreadOptellen;
  pthread_attr_t oAttr;
  void *pStatus;

  // Initialiseren
  printf("w3.1.1multithreading\n\n");

  // Thread eigenschappen instellen
  pthread_attr_init(&oAttr);
  pthread_attr_setdetachstate(&oAttr, PTHREAD_CREATE_JOINABLE);

  nRet = pthread_create(&oThreadLetters, &oAttr, ThreadLetters, (void*)123); 
  if (nRet > 0) {
    printf("Thread kon niet gemaakt worden!\n");
    return -1;
  };

  nRet = pthread_create(&oThreadOptellen, &oAttr, ThreadOptellen, (void*)"a+b en 'foute' invoer is stoppen"); 
  if (nRet > 0) {
    printf("Thread kon niet gemaakt worden!\n");
    return -1;
  };

  // Afsluiten door te wachten op de hoofdthread en daarna elke andere thread
  pthread_attr_destroy(&oAttr);
  printf("\nAfsluiten threads: "); fflush(stdout);
  pthread_join(oThreadOptellen, &pStatus); printf(" ThreadOptellen=%ld ", (long)pStatus);
  pthread_join(oThreadLetters, &pStatus); printf(" ThreadLetters=%ld ", (long)pStatus);
  printf("\nAfgesloten :-)\n\n");
  return 0;
};