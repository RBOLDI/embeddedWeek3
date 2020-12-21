// w3.1.3hwlayer

#include "osthread.hpp"

using namespace std;

OsThread::OsThread() {
  
  // Initialiseer de hardwarelaag voordat hij gebruikt gaat worden
  //TODO: kies bijvoorbeeld knoppen pin 15 en 16, en LED's pin 11 en 13
  oHwStop_.init(RPI_GPIO_P1_15);
  oHwKnop_.init(RPI_GPIO_P1_16);
  oLed1_.init(RPI_GPIO_P1_07);
  oLed2_.init(RPI_GPIO_P1_13);

  // Initialiseer de thread instellingen
  pthread_attr_init(&oAttr_);
  pthread_attr_setdetachstate(&oAttr_, PTHREAD_CREATE_JOINABLE);

  // Maak de thread zelf en koppel de class instantie via het dataveld
  int nRet = pthread_create(&oThread_, &oAttr_, thread, (void*)this); 
  if (nRet > 0) {
    printf("OsThread kon niet gemaakt worden!\n");
    return;
  };
};

OsThread::~OsThread() {

  // Vrijgeven thread instellingen
  pthread_attr_destroy(&oAttr_);
};

void OsThread::join() {

  void *pStatus;

  // Wacht tot de thread klaar is
  pthread_join(oThread_, &pStatus); 
};

void OsThread::delay(long nMSec) {

  struct timespec oTime, oTimeLeft;

  // Handige delay-functie in msec
  oTime.tv_sec = 0; 
  oTime.tv_nsec = nMSec * 1000000L;
  nanosleep(&oTime, &oTimeLeft);
};

void *OsThread::thread(void *pThreadData) {

  OsThread *pThis;
  bool bLed1;

  // Maak een referentie naar het class object
  pThis = (OsThread*)pThreadData;

  // Start met LED1
  bLed1 = true;
  pThis->oLed1_.on();

  // Ga door tot drukknop 1
  while (true) {

    // Toon unieke thread tekst
    printf("."); fflush(stdout);
    pThis->delay(400);

    // Wisselen?
    if (pThis->oHwKnop_.pushed()) {
		cout << "kanker" << endl;
      bLed1 = !bLed1;
      if (bLed1) {
        pThis->oLed1_.on();
        pThis->oLed2_.off();
      }
      else {
        pThis->oLed1_.off();
        pThis->oLed2_.on();
      }
    };

    // Stoppen?
    if (pThis->oHwStop_.pushed()) {
		cout << "kanker" << endl;
      pThis->oLed1_.off();
      pThis->oLed2_.off();
      pthread_exit(NULL);
    };
  };
 
  // Thread vrijgeven
  pthread_exit(NULL);
};

