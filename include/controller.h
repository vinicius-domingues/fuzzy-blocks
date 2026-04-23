#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

class Controller{
  public:
    static const int blocks_limit = 50;
    int sequence[blocks_limit];
    int blocks_read = 0;
    bool got_error = false;
    bool is_loop = false;

    void Listener();
};

#endif