#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <Wire.h>



class Controller{
  public:
    static const int blocks_limit = 50;
    int sequence[blocks_limit];
    int blocks_read = 0;
    bool got_error = false;
    bool is_loop = false;

    Controller();
    void Clock();
    void Prepare();
    void Listener();
    void Mapper();
    void DebugMenu();
    void Cleaner();
    void writeEEPROM(int address, byte data);
    void transmitI2C(byte slaveAddress);

  private:
    byte readEEPROM(int address);
};

#endif