#include <syntax.h>
#include <tokens.h>

Syntax::Syntax(int dados[], int tamanho){
   tokens = dados; 
   total = tamanho;
}

bool Syntax::Parser(){
    int token_da_vez;
    int count_conditions = 0;
    int count_blocks = 0;
    int count_functions = 0;
    bool error_flag = false;

    for(int i = 0 ; i < total ; i++){
        token_da_vez = tokens[i];

        //Serial.print("[PARSER] Token da vez: ");
        // Serial.println(token_da_vez);

        switch (token_da_vez) {
            
            case _IF:
            case _WHILE:
                count_conditions++;
                count_blocks++;

                //Serial.print("[PARSER] Somei condição e bloco");
                // Serial.println(count_conditions);
                // Serial.println(count_blocks);

                break;

            case _ESPERA:
                count_functions++;

                //Serial.print("[PARSER] Somei função");
                // Serial.println(count_functions);

                break;

            case _ENDBLOCK:
                count_blocks--;

                //Serial.print("[PARSER] Tirei blocos");
                // Serial.println(count_blocks);

                break;

            case _ENDFUNCTION:
                count_functions--;

                //Serial.print("[PARSER] Tirei funções");
                // Serial.println(count_functions);

                break;

            case _ENDCOND:
                count_conditions--;

                //Serial.print("[PARSER] Tirei condições");
                // Serial.println(count_conditions);

                break;
        }
    }

    if (count_conditions != 0){
        if(count_conditions < 0) {
            result = 1; // Erro: Há mais fechamentos condicionais que condições
            // Serial.println("[PARSER] Erro: Há mais fechamentos condicionais que condições");
        }else{
            result = 2; // Erro: Faltam fechamentos condicionais
            // Serial.println("[PARSER] Erro: Faltam fechamentos condicionais");
        }
        error_flag = true;
    }

    if(!error_flag){
        if (count_blocks != 0){
            if(count_blocks < 0) {
                result = 3; // Erro: Há mais fechamentos de bloco que condições
                // Serial.println("[PARSER] Erro: Há mais fechamentos de bloco que condições");
            }else{
                result = 4; // Erro: Faltam fechamentos de bloco
                // Serial.println("[PARSER] Erro: Faltam fechamentos de bloco");
            }

            error_flag = true;
        }
    }

    if(!error_flag){
        if (count_functions != 0){
            if(count_functions < 0) {
                result = 5; // Erro: Há mais fechamentos de função que funções
                // Serial.println("[PARSER] Erro: Há mais fechamentos de função que funções");
            }else{
                result = 6; // Erro: Faltam fechamentos de função
                // Serial.println("[PARSER] Erro: Faltam fechamentos de função");
            }

            error_flag = true;
        }
    } 

    if(error_flag){
        Serial.print("[PARSER] Deu erro: ");
        Serial.println(result);
    }else{
        Serial.print("[PARSER] Sucesso: ");
        Serial.println(result);
    }

    return error_flag;
}

bool Syntax::LookAhead(){
    // tokens
    // total
    int token_da_vez;
    int contador = 0;
    int proximo = 0;

    for (int i = 0; i < total; i++) {
        
        token_da_vez = tokens[i];

        if (i + 1 < total){
            proximo = tokens[i + 1];
        }else{
            proximo = _END;
        }

        Serial.print("[LOOKAHEAD] Atual/Prox: ");
        Serial.print(token_da_vez);
        Serial.print(" / ");
        Serial.println(proximo);
    }

}   
