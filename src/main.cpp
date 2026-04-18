#include <Arduino.h>
#include "syntax.h"
#include "tokens.h"
#include "controller.h"



void setup() {
    Controller* arduino;
    Syntax* analisador;
    bool error_flag = false;

    arduino = new Controller ();

    Serial.begin(9600);

    // Escuta os pulsos elétricos
    arduino->Listener();

    // Cria o objeto analisador, e passa o array lido e o tanto de blocos lidos (que pode ser diferente do limite total de 50)
    analisador = new Syntax (arduino->sequence, arduino->blocks_read);

    // Se passar pelo Parser, chama o LookAhead;
    if(!analisador->Parser()){
      error_flag = analisador->LookAhead();
    }
  
}

void loop() {}