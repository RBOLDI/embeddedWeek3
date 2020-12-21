// w3.1.2hwdriver

//TODO: zorg voor de juiste 2 #-commando's
#ifndef _KNOP_HPP_
#define _KNOP_HPP_

  #include <bcm2835.h>
  #include <stdbool.h>
  #include <stdio.h>
  #include <iostream>

  // De drukknop class
  class knop {
    public:
      //knop();
      //~knop();
      void init(int nPin);
      bool pushed();

    private:
      int nPin_;
  };

#endif // _KNOP_HPP_