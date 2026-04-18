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
    int error_flag = false;
    int token_da_vez;
    int proximo = 0;

    for (int i = 0; i < total; i++) {
        
        token_da_vez = tokens[i];

        if (i + 1 < total){
            proximo = tokens[i + 1];
        }else{
            proximo = _END;
        }

        // Token _START (1/11)
        if(token_da_vez == _START){
            if(proximo == _START){
                result = 7;
                Serial.println("[LOOKAHEAD] Erro 7: INICIO seguido de INICIO vazio");
                error_flag = true;
            }else if(!isCondicao(proximo) && !isMetodo(proximo) && !isFunction(proximo) && proximo != _END){
                result = 8;
                Serial.println("[LOOKAHEAD] Erro 8: INICIO deve ser seguido por Condicao, Metodo ou Funcao");
                error_flag = true;
            }
        }

        // Token CONDIÇÃO (2/11)
        else if(isCondicao(token_da_vez)){
            if(!isMetodo(proximo) && !isVar(proximo) && proximo != _START){
                result = 9;
                Serial.println("[LOOKAHEAD] Erro 9: Condicao deve ser seguida por Metodo, Variavel ou INICIO");
                error_flag = true;
            }
        }
        
        // Token OPERADOR (3/11)
        else if(isOperacao(token_da_vez)){
            if(!isMetodo(proximo) && !isValor(proximo)){
                result = 10;
                Serial.println("[LOOKAHEAD] Erro 10: Operador deve ser seguido por Metodo ou Valor");
                error_flag = true;
            }
        }

        // Token MÉTODO (4/11)
        else if(isMetodo(token_da_vez)){
            if(isVar(proximo) || isValor(proximo)){ 
                result = 11;
                Serial.println("[LOOKAHEAD] Erro 11: Metodo nao pode ser seguido por Variavel ou Valor");
                error_flag = true;
            }
        }

        // Token FUNÇÃO (5/11)
        else if(isFunction(token_da_vez)){
            if(proximo != _START && !isValor(proximo)){
                result = 12;
                Serial.println("[LOOKAHEAD] Erro 12: Funcao deve ser seguida por INICIO ou Valor");
                error_flag = true;
            }
        }

        // Token VALOR (6/11)
        else if(isValor(token_da_vez)){
            if(proximo != _START && !isLogical(proximo) && !isEndCond(proximo) && !isEndFunction(proximo)){
                result = 13;
                Serial.println("[LOOKAHEAD] Erro 13: Valor deve ser seguido por Logico, INICIO ou Fechamento");
                error_flag = true;
            }
        }

        // Token VARIAVEL (7/11)
        else if(isVar(token_da_vez)){
            if(proximo != _START && !isOperacao(proximo)){
                result = 14;
                Serial.println("[LOOKAHEAD] Erro 14: Variavel deve ser seguida por Operacao ou INICIO");
                error_flag = true;
            }
        }

        // Token ENDCOND (8/11)
        else if(isEndCond(token_da_vez)){
            if(proximo != _START && !isCondicao(proximo) && !isMetodo(proximo) && !isFunction(proximo) && !isEndBlock(proximo) ){
                result = 15;
                Serial.println("[LOOKAHEAD] Erro 15: Proximo token invalido apos Fechar Condicao");
                error_flag = true;
            }
        }

        // Token ENDBLOCK (9/11)
        else if(isEndBlock(token_da_vez)){
            if(proximo != _START && !isCondicao(proximo) && !isMetodo(proximo) && !isFunction(proximo) && !isEndBlock(proximo) && proximo != _END){
                result = 16;
                Serial.println("[LOOKAHEAD] Erro 16: Proximo token invalido apos Fechar Bloco");
                error_flag = true;
            }
        }

        // Token ENDFUNCTION (10/11)
        else if(isEndFunction(token_da_vez)){
            if(proximo != _START && !isCondicao(proximo) && !isMetodo(proximo) && !isFunction(proximo) && !isEndBlock(proximo) && proximo != _END){
                result = 17;
                Serial.println("[LOOKAHEAD] Erro 17: Proximo token invalido apos Fechar Funcao");
                error_flag = true;
            }
        }

       // Token LÓGICO (11/11)
        else if(isLogical(token_da_vez)){
            if(!isValor(proximo) && !isMetodo(proximo) ){
                result = 18;
                Serial.println("[LOOKAHEAD] Erro 18: Operador Logico deve ser seguido por Valor ou Metodo");
                error_flag = true;
            }
        }



        
    }

    if(error_flag){
        Serial.print("[LOOKAHEAD] Atual/Prox: ");
        Serial.print(token_da_vez);
        Serial.print(" / ");
        Serial.println(proximo);
    }else{
        Serial.print("[LOOKAHEAD] Sucesso: ");
        Serial.println(result);
    }

    return error_flag;
}
