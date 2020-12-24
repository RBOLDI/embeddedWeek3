// w3.1.2hwdriver

//TODO: zorg voor de juiste 2 #-commando's
#ifndef _LED_HPP_
#define _LED_HPP_

  #include <bcm2835.h>
  #include <stdbool.h>
  #include <stdio.h>
  #include <iostream>

  // De led class
  class led {
    public:
      //led();
      //~led();
      void init(int nPin);
      void on();
      void off();
     // void toggle();
      bool state();

    private:
      int nPin_;
      bool bState_;
  };

#endif // _LED_HPP_