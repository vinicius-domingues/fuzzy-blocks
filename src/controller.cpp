#include "Controller.h"
#include <tokens.h>

void Controller::Listener(){  
  int entry_tokens[] = {1, 104, 200, 302, 6, 105, 200, 302, 6, 106, 200, 302, 6, 1}; // OK
  // 1 = INICIO
  // 104 = ACENDE VERMELHO
  // 200 = ESPERA
  // 302 = 5
  // 6 = FECHA FUNCAO
  // 105 = ACENDE VERDE
  // 200 = ESPERA
  // 302 = 5
  // 6 = FECHA FUNCAO
  // 106 = ACENDE AZUL
  // 200 = ESPERA
  // 302 = 5
  // 6 = FECHA FUNCAO
  // 1 = INICIO

  int i = 0;
  bool waiting = true;    // Esperando encontrar valor para iniciar a escuta
  bool listening = false; // Escuta até encontrar FIM, INÍCIO ou estourar limite
  int token;

  // Reseta os contadores
  blocks_read = 0;
  got_error = false;

  while(waiting){
    //token recebe valor
    token = entry_tokens[i];

    if(token == _START){ // Se lê INICIO, passa a escutar e para de esperar
      listening = true;
      waiting = false;
    }

    i++; // Avança para o próximo token
  }
 

  bool is_condition = false;
  bool is_function = false;
  while(listening){
    token = entry_tokens[i];   // Pulou o início, agora só valores validos
    
    if(token == _START || token == _END || blocks_read == blocks_limit || got_error){
      listening = false;
    }else{
      
      if(token == _IF || token == _WHILE){
        is_condition = true;
      } else if (token == _ESPERA){
        is_function = true;
      }

      sequence[blocks_read] = token;
      blocks_read++;

      // Se for fim da linha e não for condição e função ao mesmo tempo (pois indicaria erro de blocos)
      /* if ( fim da linha || !(is_condition && is_function )){
          if ( is_condition ) {
            sequence[blocks_read] = _ENDCOND;
            is_condition = false;
            blocks_read++;
            }

            if (is_function){
            sequence[blocks_read] = _ENDFUNCTION;
            is_function = false;
            blocks_read++;
           }
        }else{
          got_error = true;
        }
      */

      i++;


      if(blocks_read == 1){
        Serial.println("");
        Serial.print("[LISTENER] Tokens lidos: ");
        Serial.print(token); 
      }else{
        Serial.print(" - "); 
        Serial.print(token); 
      }

    }
    
  }

  Serial.println(""); 
  Serial.print("[LISTENER] Total de blocos lidos: ");
  Serial.println(blocks_read);
}