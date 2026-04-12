#include <Arduino.h>
#include "Controller.h"

# define _TEST 1
# define PIN_CS_SD 4

Controller arduino; // from "Controller.h
bool sd = false;

void setup() {
  
  Serial.begin(9600);
  delay(1000);
  randomSeed(analogRead(LOW));

  if (!SD.begin(PIN_CS_SD)) {
    Serial.println(F("[WARNING] SD não encontrado"));
  } else {
    Serial.println(F("[LOG] SD montado com sucesso."));
    sd = true;
  }

  // Se teste unitário
  #if _TEST
    #include "test.h"  
    test_execute();
    while(true){}
  #endif

  bool stop = false;
  int limite = 50;
  int token_queue[limite];
  int token_sequence = 0;
  int token;

  // Compilação
  while(!stop){
    // Escutando eventos
    while(arduino.estado==ESCUTANDO){
        token = random(0, 2);
        Serial.print("[ESTADO 0] Token recebido: "); 
        Serial.println(token);

        if (token == 1){
          Serial.println("Entrei no start");
          arduino.estado=MAPEANDO;
        }
        delay(1000);
      }

    // Mapeamento
    while (arduino.estado==MAPEANDO && token_sequence <= limite){
      Serial.println("[LOG] Mapeando");
      token = random(-1, 12);
      //Serial.print("[ESTADO 1] Token recebido: "); 
      //Serial.println(token);

      //Serial.println("[ACUMULADOR] ----------------");
      token_queue[token_sequence] = token;
      token_sequence++;
      if (token == _END || token_sequence == limite){
        //Serial.println("[END] ----------------");
        arduino.estado=TRANSPILANDO;
      }
    }
    
    // Transpilando
    while (arduino.estado == TRANSPILANDO){
      Serial.println("[LOG] Transpilando");
      arduino.TranspilaCodigo(token_queue, limite, token_sequence);
      arduino.estado = EXECUTANDO;
    }
      
    // Executando
    while (arduino.estado == EXECUTANDO){
      Serial.println("[LOG] Executando");  
      arduino.estado = FINALIZANDO;
    }

    // Encerrando
    if (arduino.estado == FINALIZANDO){
      Serial.println("[LOG] Finalizando");  
      stop = true;
    }
  }
}

void loop() {}