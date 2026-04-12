#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define _START 1
#define _END -1

#define _WHILE 2
#define _IF 3
#define _ENDCOND 4
#define _ENDBLOCK 5

#define _NOT 6
#define _EQUAL 7
#define _BIGGER 8
#define _SMALLER 9

#define _TRUE 10
#define _FALSE 11

#define ESCUTANDO 0
#define MAPEANDO 1
#define TRANSPILANDO 2
#define EXECUTANDO 3
#define FINALIZANDO 4

extern bool sd;

class Controller {
  public:
    int estado;
    String generated_code;
    String code; 

    Controller(){
      estado = 0;
    }

    void JuntaCodigo(){
      if(sd){
        File arquivo = SD.open("codigo.ino", FILE_WRITE);
        
        if (arquivo) {
          arquivo.print(F(R"(
#include <Arduino.h>

#define PIN_SENSOR_OBSTACULO 12
#define PIN_MOTOR_ACELERAR   14
#define PIN_FREIO_LED        27
#define PIN_BUZINA_BEEP      26

class Carro {
  public:
    bool obstaculo() {
      return (digitalRead(PIN_SENSOR_OBSTACULO) == LOW);
    }

    void acelerar() {
      digitalWrite(PIN_MOTOR_ACELERAR, HIGH);
      digitalWrite(PIN_FREIO_LED, LOW);
    }

    void freiar() {
      digitalWrite(PIN_MOTOR_ACELERAR, LOW);
      digitalWrite(PIN_FREIO_LED, HIGH);    
    }

    void buzinar() {
      digitalWrite(PIN_BUZINA_BEEP, HIGH);
      delay(1000);
      digitalWrite(PIN_BUZINA_BEEP, LOW);
    }
};

Carro carrinho;

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(PIN_SENSOR_OBSTACULO, INPUT_PULLUP);
  pinMode(PIN_MOTOR_ACELERAR, OUTPUT);
  pinMode(PIN_FREIO_LED, OUTPUT);
  pinMode(PIN_BUZINA_BEEP, OUTPUT);
}

void loop() {
)"));
          arquivo.print(this->generated_code);
          arquivo.print(F("\n}\n"));
          arquivo.close();
        } else {
          Serial.println(F("[ERROR] Nao foi possivel abrir/criar o arquivo."));
        }
} else {
  Serial.print(F(R"(
#include <Arduino.h>

#define PIN_SENSOR_OBSTACULO 12
#define PIN_MOTOR_ACELERAR   14
#define PIN_FREIO_LED        27
#define PIN_BUZINA_BEEP      26

class Carro {
  public:
    bool obstaculo() {
      return (digitalRead(PIN_SENSOR_OBSTACULO) == LOW);
    }

    void acelerar() {
      digitalWrite(PIN_MOTOR_ACELERAR, HIGH);
      digitalWrite(PIN_FREIO_LED, LOW);
    }

    void freiar() {
      digitalWrite(PIN_MOTOR_ACELERAR, LOW);
      digitalWrite(PIN_FREIO_LED, HIGH);    
    }

    void buzinar() {
      digitalWrite(PIN_BUZINA_BEEP, HIGH);
      delay(1000);
      digitalWrite(PIN_BUZINA_BEEP, LOW);
    }
};

Carro carrinho;

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(PIN_SENSOR_OBSTACULO, INPUT_PULLUP);
  pinMode(PIN_MOTOR_ACELERAR, OUTPUT);
  pinMode(PIN_FREIO_LED, OUTPUT);
  pinMode(PIN_BUZINA_BEEP, OUTPUT);
}

void loop() {
)"));
  Serial.print(this->generated_code);
  Serial.println(F("\n}"));  
}
    }

    void TranspilaCodigo(int tokens[], int tamanho, int token_sequence){
      bool is_condition = false;
      bool is_method;
      int token;
      String current_token;
   
      this->generated_code = "";

      for(int i = 0; i < token_sequence; i++){
        is_method = false;
        token = tokens[i];

        if(token == _WHILE){
          current_token = "while(";
          is_condition = true;

        }else if(token == _IF) {
          current_token = "if(";
          is_condition = true;

        }else if(token == _ENDCOND) {
          current_token = "){";
          is_condition = false;

        }else if(token == _ENDBLOCK) {
          current_token = "}";

        }else if(token == _NOT) {
          current_token = "!";

        }else if(token == _EQUAL) {
          current_token = "=";

        }else if(token == _BIGGER) {
          current_token = ">";
          
        }else if(token == _SMALLER) {
          current_token = "<";

        }else if(token == _TRUE) {
          current_token = "true";

        }else if(token == _FALSE) {
          current_token = "false";

        }else if(token == _END) {
          current_token = "while (true){}";

        }else if(token == 100) {
          is_method = true;
          current_token = "carrinho.freiar()";

        }else if(token == 101) {
          is_method = true;
          current_token = "carrinho.acelerar()";

        }else if(token == 102) {
          is_method = true;
          current_token = "carrinho.obstaculo()";

        }else if(token == 103) {
          is_method = true;
          current_token = "carrinho.buzinar()";
        }

        if(is_method && !is_condition){
          current_token += ";";
        }

        Serial.print("[LOG] ");
        Serial.print(token);
        Serial.print(" -> ");
        Serial.println(current_token);   

        this->generated_code += current_token;
        this->generated_code += "\n";
      }

      this->JuntaCodigo();
    }
};