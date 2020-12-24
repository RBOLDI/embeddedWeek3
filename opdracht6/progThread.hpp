// w3.1.3hwlayer

//TODO: zorg voor de juiste # commando's
#ifndef _PROGTHREAD_HPP_
#define _PROGTHREAD_HPP_

#include <bcm2835.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <iostream>
#include <fstream>

//#include "ncursesio.hpp"
#include "compiler.hpp"
#include "knop.hpp"
#include "led.hpp"

void progThreadsInit(void);

//Shared values
extern pthread_mutex_t oLockOutput;
extern pthread_mutex_t oLockInput;
extern uint8_t activeProgramIndex;
extern bool stopPushed;
extern bool switchPushed;
bool getCurrProgOutput(char *dest);

// De OS thread hardware laag class
class progThread {
	public:
		progThread(char *filename, uint8_t i);
		~progThread();
		void join();

	private:
		pthread_t progThread_;
		pthread_attr_t progAttr_;
		void delay(long nMSec);
		static void *thread(void *pThreadData);
		uint16_t programIndex(char *input);
		uint8_t index;
		//program information
		//std::ifstream file;
		//Classes:
		PARSER	parseObjct;
		void skipToLine(uint16_t lineNumber, char *fileName, uint8_t currProg);
};

#endif // _PROGTHREAD_HPP_