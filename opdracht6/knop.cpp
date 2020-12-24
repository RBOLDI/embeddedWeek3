#include "knop.hpp"

using namespace std;

void knop::init(int nPin) {
	if(!bcm2835_init()) {
		cout << "initialize failed\n";
	}
	nPin_ = nPin;
	bcm2835_gpio_fsel(nPin_,BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(nPin_, BCM2835_GPIO_PUD_DOWN); // set pulldown
}

bool knop::pushed() {
	return bcm2835_gpio_lev(nPin_);
}