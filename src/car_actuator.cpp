#include "Controller.h"
#include <tokens.h>
#include "car_actuator.h"

Car::Car(){
    Serial.println(F("[CARRO] Inicializando hardware e pinos..."));
    pinMode(PIN_RED_LED, OUTPUT);
    pinMode(PIN_GREEN_LED, OUTPUT);
    pinMode(PIN_BLUE_LED, OUTPUT);
    
    pinMode(PIN_SENSOR, INPUT);
    
    digitalWrite(PIN_RED_LED, LOW);
    digitalWrite(PIN_GREEN_LED, LOW);
    digitalWrite(PIN_BLUE_LED, LOW);    
}

void Brake() {
    Serial.println(F("[CARRO] Acao executada: Frear"));
    digitalWrite(PIN_MOTOR_A, LOW);
    digitalWrite(PIN_MOTOR_B, LOW);
}

void Accelerate() {
    Serial.println(F("[CARRO] Acao executada: Acelerar"));
    digitalWrite(PIN_MOTOR_A, HIGH);
    digitalWrite(PIN_MOTOR_B, LOW);
}

void Honk() {
    Serial.println(F("[CARRO] Acao executada: Buzinar"));
    digitalWrite(PIN_BUZZER, HIGH);
    delay(200); 
    digitalWrite(PIN_BUZZER, LOW);
}

void RedLed() {
    Serial.println(F("[CARRO] Acao executada: Acender LED Vermelho"));
    digitalWrite(PIN_GREEN_LED, LOW);
    digitalWrite(PIN_BLUE_LED, LOW);
    digitalWrite(PIN_RED_LED, HIGH);
}

void GreenLed() {
    Serial.println(F("[CARRO] Acao executada: Acender LED Verde"));
    digitalWrite(PIN_RED_LED, LOW);
    digitalWrite(PIN_BLUE_LED, LOW);
    digitalWrite(PIN_GREEN_LED, HIGH);
}

void BlueLed() {
    Serial.println(F("[CARRO] Acao executada: Acender LED Azul"));
    digitalWrite(PIN_RED_LED, LOW);
    digitalWrite(PIN_GREEN_LED, LOW);
    digitalWrite(PIN_BLUE_LED, HIGH);
}

bool Proximity() {
    Serial.println(F("[CARRO] Leitura de Sensor: Proximidade"));
    return(HIGH == digitalRead(PIN_SENSOR));
}