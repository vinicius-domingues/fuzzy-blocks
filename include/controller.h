#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

class Controller{
  public:
    static const int blocks_limit = 50;
    int sequence[blocks_limit];
    bool got_error = false;
    int blocks_read = 0;
  

    void Listener();
};

#endif