#include "Controller.h"
#include <tokens.h>
#include <evaluator.h>


Evaluator::Evaluator(int dados[], int tamanho, bool is_loop){
   sequencia = dados;
   qtd_tokens = tamanho;
   pc = 0;
   this->is_loop = is_loop;
}

void Evaluator::Eval(unsigned long segundos){
    int token = sequencia[pc];

    Serial.print("[EVAL] Token: "); Serial.println(token);
    Serial.print("[EVAL] Segundos: "); Serial.println(segundos);

    // Se o ponteiro estiver no último item lido e ainda não bateu em nenhum _END, é porque é LOOP, então volta para o começo
    if(pc == (qtd_tokens - 1)){
        if(this->is_loop){
            pc = 0;
        }else{
            run = false;
        }
        
    }else{
        pc++;
    }
}
