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
enum SystemState {
    STATE_DEBUG,
    STATE_COMPILE,
    STATE_RUNNING
};

SystemState currentState = STATE_DEBUG; 

void setup() {
    Serial.begin(9600);
    arduino = new Controller();
    carrinho = new Car();
    
    Serial.println(F("[MAIN] Hardware inicializado."));
}

void loop() {
    switch (currentState) {
        
        case STATE_DEBUG:
            // 1. O código estaciona aqui dentro do método até receber o comando 'L'
            arduino->DebugMenu();
            
            // 2. O usuário enviou 'L'. Transita para a próxima fase.
            currentState = STATE_COMPILE;
            break;

        case STATE_COMPILE: {
            bool error_flag = false;

            // Inicia o fluxo padrão do interpretador
            arduino->Listener(); // Aguarda o botão do Pino 5 ser pressionado fisicamente

            // Limpeza
            arduino->Cleaner();

            arduino->Mapper();   // Mapeia o hardware

            // Validação em 3 camadas
            analisador = new Syntax(arduino->sequence, arduino->blocks_read);

            error_flag = analisador->Parser() || 
                              analisador->LookAhead() || 
                              analisador->Semantic();

            // Controle de transição
            if (!error_flag) {
                Serial.println(F("[MAIN] Código Limpo! Preparando Evaluator..."));
                
                // Evita vazamento de memória deletando instâncias de rodadas anteriores
                if (executor != nullptr) { delete executor; }
                
                executor = new Evaluator(arduino->sequence, arduino->blocks_read, arduino->is_loop, carrinho);
                
                ready_instant = millis();
                seconds_running = 0;
                
                // Vai para a execução contínua
                currentState = STATE_RUNNING; 
            } else {
                Serial.println(F("[MAIN] Erros identificados. Abortando execução e voltando ao Debug."));
                delete analisador; 
                currentState = STATE_DEBUG; // Devolve pro terminal
            }
            break;
        }

        case STATE_RUNNING: {
            unsigned long actual_instant = millis();

            // Relógio interno de execução
            if (actual_instant - ready_instant >= 1000) {
                seconds_running++;           
                ready_instant = actual_instant; 
            }

            // Executa os comandos lidos do hardware
            if (executor->run) {
                executor->Eval(seconds_running);
            } else {
                // Ao invés de prender em um while(true) no final, o sistema limpa as mãos e recomeça
                Serial.println(F("\n[MAIN] Fim do script alcançado! Execução do carrinho concluída."));
                Serial.println(F("[MAIN] Retornando ao console de depuração..."));
                
                delete analisador; // Libera RAM
                currentState = STATE_DEBUG; // Transita de volta para a espera serial
            }
            break;
        }
    }
}