#include <Arduino.h>
#include <syntax.h>
#include <tokens.h>

/*
#define _ENDCOND 4
#define _ENDBLOCK 5
#define _ENDFUNCTION 6
*/
int entry_tokens[] = {1, 104, 200, 302, 6, 105, 200, 302, 6, 106, 200, 302, 6, 1};
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

class Controller{
  public:
    static const int blocks_limit = 50;
    int sequence[blocks_limit];
    bool got_error = false;
    int blocks_read = 0;

    void Listener(){  
      int i = 0;
      bool waiting = true;    // Esperando encontrar valor para iniciar a escuta
      bool listening = false; // Escuta até encontrar FIM, INÍCIO ou estourar limite
      int token;
    
      while(waiting){
        //token recebe valor
        token = entry_tokens[i];

        if(token == _START){ // Se lê INICIO, passa a escutar e para de esperar
          listening = true;
          waiting = false;
        }

        delay(500);
        //Serial.print("Token: ");
        //Serial.println(token);

        i++; // Quando vier do pin, vai mudar, nao vai precisar desse cara
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

          delay(500);
          Serial.print("Token: ");
          Serial.println(token); 
        }
      }
    }
};


void setup() {
  Controller* arduino;
  Syntax* analisador;
  bool error_flag = false;

  arduino = new Controller ();

  Serial.begin(9600);
  delay(1000);

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