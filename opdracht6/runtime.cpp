// BCM Library IO Ding
//
// Compile: g++ -Wall -o io io.cpp -lbcm2835
// Run: ./io


#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <fstream>
#include "runtime.hpp"


using namespace std;

int RUNTIME_CLASS::initBcm(void)
{
	if ( !bcm2835_init() ) return 1;
	bcm2835_gpio_fsel(BUTTON1, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(BUTTON2, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(BUTTON3, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(BUTTON4, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(LED1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(LED2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(LED3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(LED1, LOW);
	bcm2835_gpio_write(LED2, LOW);
	bcm2835_gpio_write(LED3, LOW);
	return 0;
}

int RUNTIME_CLASS::closeBcm(void)
{
	bcm2835_gpio_write(RPI_GPIO_P1_11, LOW);
	bcm2835_gpio_write(LED2, LOW);
	bcm2835_gpio_write(LED3, LOW);
	bcm2835_close();
	return 0;
}

int RUNTIME_CLASS::setLed(char led, int state, char *output)
{
	sprintf(output,"execute setLed");
	//cout<<"execute setLed"<<endl;
	if ( state == HIGH ) 
		bcm2835_gpio_write(led, state);
	else bcm2835_gpio_write(led, LOW);
	return 0;
}

int RUNTIME_CLASS::pollLed(char led, char *output)
{
	sprintf(output,"execute returnLed");
	//cout<<"execute returnLed"<<endl;
	return bcm2835_gpio_lev(led);
}

int RUNTIME_CLASS::pollButton(char button, char *output)
{
	sprintf(output,"execute returnKnop");
	//cout<<"execute returnKnop"<<endl;
	return bcm2835_gpio_lev(button);
}

int RUNTIME_CLASS::delayRPI(uint16_t ms, char *output)
{ 
	oTime.tv_sec = 0; 
	oTime.tv_nsec = (ms) * 1000000L; 
	nanosleep(&oTime, &oTimeLeft);
	//cout<<"Delay "<< (unsigned)ms<< "ms"<<endl;
	//bcm2835_delay(ms);
	return 0;
}