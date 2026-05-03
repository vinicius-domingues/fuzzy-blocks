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

    // SOLVED!
    // INICIO, IF, SEGUNDOS, MAIOR, OBSTACULO, MENOR, TRUE, FECHA_COND, FECHA_BLOCO, INICIO (FIM)
    // int entry_tokens[] = {_START, _IF, _SEGUNDOS, _BIGGER, _PROXIMITY, _ENDCONDITION, _ENDBLOCK, _START};
    // 3 - Type mismatch (Mistura de tipos incompatíveis)

    // SOLVED! Erro: Elemento 3 não é um valor
    // INICIO, IF, SEGUNDOS, IGUAL, OBSTACULO, FECHA_COND, FECHA_BLOCO, INICIO (FIM)
    // int entry_tokens[] = {_START, _IF, _SEGUNDOS, _EQUAL, _PROXIMITY, _ENDCONDITION, _ENDBLOCK, _START};
    // 10 - Comparação Inter-Domínios: Comparando uma unidade de tempo (_SEGUNDOS) com um estado/distância (_PROXIMITY).

    // SOLVED!  Erro: Fração da condição tem valores demais (4+)
    // INICIO, IF, OBSTACULO, IGUAL, OBSTACULO, MENOR, OBSTACULO, FECHA_COND, FECHA_BLOCO, INICIO (FIM)
    // int entry_tokens[] = {_START, _IF, _PROXIMITY, _EQUAL, _PROXIMITY, _SMALLER, _PROXIMITY, _ENDCONDITION, _ENDBLOCK, _START};
    // 11 - Cascata Relacional: Encadeamento de operadores. No C++, vira `(obstaculo == obstaculo) < obstaculo`, avaliando 1 < distância.

    // SOLVED! Erro: Condição única não é MÉTODO do tipo BOOLEANO
    // INICIO, IF, OBSTACULO, AND, CINCO, FECHA_COND, FECHA_BLOCO, INICIO (FIM)
    // int entry_tokens[] = {_START, _IF, _PROXIMITY, _BIGGER, _FIVE, _AND, _FIVE, _ENDCONDITION, _ENDBLOCK,_START};
    // 12 - Curto-Circuito Lógico com Constante: "IF OBSTACULO E 5". O C++ ignora o 5 (pois é sempre verdadeiro), matando a intenção do usuário.

    //   int entry_tokens[] = {_START, _IF, _PROXIMITY, _BIGGER, _FIVE, _AND, _PROXIMITY, _ENDCONDITION, _ENDBLOCK,_START};
   // int entry_tokens[] = {_START, _IF, _PROXIMITY, _BIGGER, _FIVE, _ENDCONDITION, _ENDBLOCK,_START};



  // IF MÉTODO AND MÉTODO > 5 AND MÉTODO (ok)
  // int entry_tokens[] = {_START, _IF, _PROXIMITY, _AND, _PROXIMITY, _BIGGER, _FIVE, _OR, _PROXIMITY,_ENDCONDITION, _ENDBLOCK, _START};

  // IF MÉTODO > 5 AND MÉTODO OR MÉTODO > 5 (ok)
  // int entry_tokens[] = {_START, _IF, _PROXIMITY, _BIGGER, _FIVE, _OR, _PROXIMITY, _AND, _PROXIMITY, _BIGGER, _FIVE, _ENDCONDITION, _ENDBLOCK, _START};

  // IF METODO (ok)
  // int entry_tokens[] = {_START, _IF, _PROXIMITY, _ENDCONDITION, _ENDBLOCK, _START};

  // IF METODO > 5 (ok)
  //int entry_tokens[] = {_START, _IF, _PROXIMITY, _BIGGER, _FIVE, _ENDCONDITION, _ENDBLOCK, _START};

  // IFs aninhados (ok)
  int entry_tokens[] = {_START, _IF, _PROXIMITY, _AND, _PROXIMITY, _EQUAL, _TRUE, _OR, _PROXIMITY,_ENDCONDITION, _IF, _PROXIMITY, _EQUAL, _FALSE, _OR, _PROXIMITY, _AND, _PROXIMITY, _EQUAL, _TRUE, _ENDCONDITION, _ENDBLOCK, _ENDBLOCK, _END};
  
  //int entry_tokens[] = {_START, _RED_LED, _DELAY, _FIVE, _ENDFUNCTION, _GREEN_LED, _END};
  
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
 


  while(listening){
    token = entry_tokens[i];   // Pulou o início, agora só valores validos (limpa o início)
    
    if(token == _START || token == _END || blocks_read == blocks_limit || got_error){
      if (token == _START){
        is_loop = true;
      }
      listening = false;
    }else{
      


      sequence[blocks_read] = token;
      blocks_read++;


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
  Serial.print(F("[LISTENER] Sucesso: "));
  Serial.println(0);
}

