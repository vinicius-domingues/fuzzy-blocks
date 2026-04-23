#ifndef CAR_ACTUATOR_H

#define CAR_ACTUATOR_H
#define PIN_RED_LED   8
#define PIN_GREEN_LED 9
#define PIN_BLUE_LED  10
#define PIN_BUZZER    11
#define PIN_MOTOR_A   5  
#define PIN_MOTOR_B   6  
#define PIN_SENSOR    12

#include <Arduino.h>

class Car{
  public:

    Car();

    void Brake();
    void Accelerate();
    void Honk();

    bool Proximity();

    void RedLed();
    void GreenLed();
    void BlueLed();
};

#endif


/*
    #define _BRAKE 100      // Comando para freiar                  (VOID)
    #define _ACCELERATE 101 // Comando para acelerar                (VOID)
    #define _HONK 103       // Comando para buzinar                 (VOID)
    #define _RED_LED 104    // Comando para acender led vermelho    (VOID)
    #define _GREEN_LED 105  // Comando para acender led verde       (VOID)
    #define _BLUE_LED 106   // CComando para acender led azul       (VOID)
*/