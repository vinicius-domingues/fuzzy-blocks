#include "Controller.h"
#include <tokens.h>

void Controller::Listener(){  
  //int entry_tokens[] = {1, 104, 200, 302, 6, 105, 200, 302, 6, 106, 200, 302, 6, 1}; // OK
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


// Erros semânticos mapeados (Que passam por Parser e LookAhead)

  // Tratamento de coisas dentro de condições
    // INICIO, IF, SEGUNDOS, MAIOR, OBSTACULO, MENOR, TRUE, FECHA_COND, FECHA_BLOCO, INICIO (FIM)
    // int entry_tokens[] = {_START, _IF, _SEGUNDOS, _BIGGER, _PROXIMITY, _SMALLER, _TRUE, _ENDCONDITIONITION, _ENDBLOCK, _START};
    // 3 - Type mismatch (Mistura de tipos incompatíveis)

    // INICIO, IF, SEGUNDOS, IGUAL, OBSTACULO, FECHA_COND, FECHA_BLOCO, INICIO (FIM)
    // int entry_tokens[] = {_START, _IF, _SEGUNDOS, _EQUAL, _PROXIMITY, _ENDCONDITION, _ENDBLOCK, _START};
    // 10 - Comparação Inter-Domínios: Comparando uma unidade de tempo (_SEGUNDOS) com um estado/distância (_PROXIMITY).

    // INICIO, IF, OBSTACULO, IGUAL, OBSTACULO, MENOR, OBSTACULO, FECHA_COND, FECHA_BLOCO, INICIO (FIM)
    // int entry_tokens[] = {_START, _IF, _PROXIMITY, _EQUAL, _PROXIMITY, _SMALLER, _PROXIMITY, _ENDCONDITION, _ENDBLOCK, _START};
    // 11 - Cascata Relacional: Encadeamento de operadores. No C++, vira `(obstaculo == obstaculo) < obstaculo`, avaliando 1 < distância.

    // INICIO, IF, OBSTACULO, AND, CINCO, FECHA_COND, FECHA_BLOCO, INICIO (FIM)
    // int entry_tokens[] = {_START, _IF, _PROXIMITY, _AND, _FIVE, _ENDCONDITION, _ENDBLOCK, _START};
    // 12 - Curto-Circuito Lógico com Constante: "IF OBSTACULO E 5". O C++ ignora o 5 (pois é sempre verdadeiro), matando a intenção do usuário.


// --- NOVOS ERROS LÓGICOS (Passam por Parser, LookAhead e Semantic) ---


  int i = 0;
  bool waiting = true;    
  bool listening = false; 
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
    token = entry_tokens[i];   // Pulou o início, agora só valores validos (limpa o início)
    
    if(token == _START || token == _END || blocks_read == blocks_limit || got_error){
      listening = false;
    }else{
      
      if(token == _IF || token == _WHILE){
        is_condition = true;
      } else if (token == _DELAY){
        is_function = true;
      }

      sequence[blocks_read] = token;
      blocks_read++;

      // Se for fim da linha e não for condição e função ao mesmo tempo (pois indicaria erro de blocos)
      /* if ( fim da linha || !(is_condition && is_function )){
          if ( is_condition ) {
            sequence[blocks_read] = _ENDCONDITIONITION;
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

