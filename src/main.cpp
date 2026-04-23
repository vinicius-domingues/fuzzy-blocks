#include <Arduino.h>
#include "syntax.h"
#include "tokens.h"
#include "controller.h"
#include "car_actuator.h"
#include "evaluator.h"

Controller* arduino;
Syntax* analisador;
Car* carrinho;
Evaluator* executor;

unsigned long ready_instant = 0;
unsigned long seconds_running = 0;

void setup() {
    bool error_flag = false;

    arduino = new Controller();
    Serial.begin(9600);
    
    // Escuta os pulsos elétricos
    arduino->Listener();

    // Cria o objeto analisador, e passa o array lido e o tanto de blocos lidos (que pode ser diferente do limite total de 50)
    analisador = new Syntax(arduino->sequence, arduino->blocks_read);

    // Se passar pelo Parser, chama o LookAhead;
    if(!analisador->Parser()){
      // Se passar pelo LookAehad, chama o semantic
      if(!analisador->LookAhead()){
        error_flag = analisador->Semantic();
      }
    }
  
    if(!error_flag){
      Serial.println("[MAIN] Preparando atuador");
      carrinho = new Car();
      
      Serial.println("[MAIN] Preparando executor");
      executor = new Evaluator(arduino->sequence, arduino->blocks_read, arduino->is_loop);

      ready_instant = millis();
    }else{
      
    }
}

void loop() {
  unsigned long actual_instant = millis();

  // Se passou 1 segundo desde a última rodada, conta segundo (inibe super atualização)
  if (actual_instant - ready_instant >= 1000) {
    seconds_running++;           
    ready_instant = actual_instant; 
  }

  // Executor
  if(executor->run){
    executor->Eval(seconds_running);
  }else{
    while(true){}
  }

}