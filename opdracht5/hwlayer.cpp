// w3.1.3hwlayer
// g++ -pthread -o hwlayer hwlayer.cpp osthread.cpp knop.cpp led.cpp -I ~/bcm2835-1.60/src ~/bcm2835-1.60/src/bcm2835.c -fpermissive
// ./hwlayer

//TODO: voeg de juiste include toe
#include "hwlayer.hpp"

int main(int nArgc, char* aArgv[]) {

  // Initialiseren
  printf("w3.1.3hwlayer\n\n");
  if (!bcm2835_init()) {
    printf("BCM init error!\n");
    return 1;
  };

  // Laad en start de OS hardware laag
  OsThread oOsThread;
  oOsThread.join();

  // Afsluiten na het wachten op de hoofdthread
  bcm2835_close();
  printf("\nAfgesloten :-)\n\n");

  return 0;
};