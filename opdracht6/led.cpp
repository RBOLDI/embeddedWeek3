#include "led.hpp"

using namespace std;

void led::init(int nPin){
	if(!bcm2835_init()) {
		cout << "initialize failed\n";
	}
	nPin_ = nPin;
	bcm2835_gpio_fsel(nPin_, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_set_pud(nPin_, BCM2835_GPIO_PUD_DOWN); // set pulldown
}

void led::on(){
	bcm2835_gpio_write(nPin_, HIGH);
	bState_ = bcm2835_gpio_lev(nPin_);
}

void led::off(){
	bcm2835_gpio_write(nPin_, LOW);
	bState_ = bcm2835_gpio_lev(nPin_);
}

bool led::state(){
	bState_ = bcm2835_gpio_lev(nPin_);
	return bState_;
}
