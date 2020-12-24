// w3.1.3hwlayer
// g++ -pthread -o hwlayer hwlayer.cpp osthread.cpp knop.cpp led.cpp -I ~/bcm2835-1.60/src ~/bcm2835-1.60/src/bcm2835.c -fpermissive
// ./hwlayer

//TODO: voeg de juiste include toe
#include "hwlayer.hpp"

//Ncuses windows
WINDOW *nWindows[WINDOW_NUM];
WINDOW * mainwin, * childWin_1, * childWin_2, * childwinh;

char *fileNames[255];
bool programs;


pthread_mutex_t oLockOutput;
pthread_mutex_t oLockInput;

void initNcursesWindows(void){
	startNcurses();
	nWindows[PROG_1] = childWin_1;
	nWindows[PROG_2] = childWin_2;
}
int main(int nArgc, char* aArgv[]) {
	// Initialiseren
	 int nRet;
	uint8_t max_numPrograms = PROG_NUM;
	nRet = pthread_mutex_init(&oLockOutput, NULL);
	if (nRet > 0) {
	printf("Mutex kon niet gemaakt worden!\n");
	return -1;
	};
	
	nRet = pthread_mutex_init(&oLockInput, NULL);
	if (nRet > 0) {
	printf("Mutex kon niet gemaakt worden!\n");
	return -1;
	};
  
	printf("w3.1.3hwlayer\n\n");
	if (!bcm2835_init()) {
		printf("BCM init error!\n");
		return 1;
	};
	// Laad en start de OS hardware laag
	if(aArgv[1] != NULL){
		programs = true;
		for(int i = 1; i < nArgc; i++){
			if(i-1 < max_numPrograms) {
				fileNames[i-1] = aArgv[i];
				//printf("%s\n", fileNames[i-1]);
			}
		}
	}
	else programs = false;
	
	if(programs){
		if((nArgc - 1) > max_numPrograms) printf("Program limit = %u... Will only run first %u given programs\n", 
													max_numPrograms, max_numPrograms);		
		progThreadsInit();
		//WINDOW CLASS POINTERS
		initNcursesWindows();
		progThread oProgThread(fileNames[PROG_1], WINDOW_1);
		progThread oProgThread2(fileNames[PROG_2], WINDOW_2);
		OsThread oOsThread;
		oProgThread.join();
		oProgThread2.join();
		oOsThread.join();
	}
	else printf("no programs to run...\n");

	// Afsluiten na het wachten op de hoofdthread
	closeNcurses();
	bcm2835_close();
	pthread_mutex_destroy(&oLockOutput);
	printf("\nAfgesloten :-)\n\n");

	return 0;
};