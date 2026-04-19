#include "syntax.h"
#include "tokens.h"

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

    if(!error_flag){
        for(int i = 0 ; i < total ; i++){
            token_da_vez = tokens[i];

            switch (token_da_vez) {
                
                case _IF:
                case _WHILE:
                    count_conditions++;
                    count_blocks++;
                    break;

                case _DELAY:
                    count_functions++;
                    break;

                case _ENDBLOCK:
                    count_blocks--;
                    break;

                case _ENDFUNCTION:
                    count_functions--;
                    break;

                case _ENDCONDITION:
                    count_conditions--;
                    break;
            }
        }
        if (count_conditions != 0){
            if(count_conditions < 0) {
                result = 1;
                Serial.println(F("[PARSER] Erro: Há mais fechamentos condicionais que condições"));
            }else{
                result = 2;
                Serial.println(F("[PARSER] Erro: Faltam fechamentos condicionais"));
            }
            error_flag = true;
        }
    }

    if(!error_flag){
        if (count_blocks != 0){
            if(count_blocks < 0) {
                result = 3;
                Serial.println(F("[PARSER] Erro: Há mais fechamentos de bloco que condições"));
            }else{
                result = 4;
                Serial.println(F("[PARSER] Erro: Faltam fechamentos de bloco"));
            }
            error_flag = true;
        }
    }

    if(!error_flag){
        if (count_functions != 0){
            if(count_functions < 0) {
                result = 5;
                Serial.println(F("[PARSER] Erro: Há mais fechamentos de função que funções"));
            }else{
                result = 6;
                Serial.println(F("[PARSER] Erro: Faltam fechamentos de função"));
            }
            error_flag = true;
        }
    } 

    if(error_flag){
        Serial.print(F("[PARSER] Falha: "));
        Serial.println(result);
    }else{
        Serial.print(F("[PARSER] Sucesso: "));
        Serial.println(result);
    }

    return error_flag;
}

bool Syntax::LookAhead(){
    bool error_flag = false;
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
                Serial.println(F("[LOOKAHEAD] Erro 7: INICIO seguido de INICIO vazio"));
                error_flag = true;
            }else if(!isCondition(proximo) && !isMethod(proximo) && !isFunction(proximo) && proximo != _END){
                result = 8;
                Serial.println(F("[LOOKAHEAD] Erro 8: INICIO deve ser seguido por Condicao, Metodo ou Funcao"));
                error_flag = true;
            }
        }

        // Token CONDIÇÃO (2/11)
        else if(isCondition(token_da_vez)){
            if(!isMethod(proximo) && !isVariable(proximo) && proximo != _START){
                result = 9;
                Serial.println(F("[LOOKAHEAD] Erro 9: Condicao deve ser seguida por Metodo, Variavel ou INICIO"));
                error_flag = true;
            }
        }
        
        // Token OPERADOR (3/11)
        else if(isOperation(token_da_vez)){
            if(!isMethod(proximo) && !isValue(proximo)){
                result = 10;
                Serial.println(F("[LOOKAHEAD] Erro 10: Operador deve ser seguido por Metodo ou Valor"));
                error_flag = true;
            }
        }

        // Token MÉTODO (4/11)
        else if(isMethod(token_da_vez)){
            if(isVariable(proximo) || isValue(proximo) || isCondition(proximo)){ 
                result = 11;
                Serial.println(F("[LOOKAHEAD] Erro 11: Metodo nao pode ser seguido por MÉTODO, VARIÁVEL ou VALOR"));
                error_flag = true;
            }
        }

        // Token FUNÇÃO (5/11)
        else if(isFunction(token_da_vez)){
            if(proximo != _START && !isValue(proximo)){
                result = 12;
                Serial.println(F("[LOOKAHEAD] Erro 12: Funcao deve ser seguida por INICIO ou Valor"));
                error_flag = true;
            }
        }

        // Token VALOR (6/11)
        else if(isValue(token_da_vez)){
            if(!isLogical(proximo) && !isEndCondition(proximo) && !isEndFunction(proximo)){
                result = 13;
                Serial.println(F("[LOOKAHEAD] Erro 13: Valor deve ser seguido por Logico ou Fechamento de função/condição"));
                error_flag = true;
            }
        }

        // Token VARIAVEL (7/11)
        else if(isVariable(token_da_vez)){
            if(proximo != _START && !isOperation(proximo)){
                result = 14;
                Serial.println(F("[LOOKAHEAD] Erro 14: Variavel deve ser seguida por Operacao ou INICIO"));
                error_flag = true;
            }
        }

        // Token ENDCOND (8/11)
        else if(isEndCondition(token_da_vez)){
            if(proximo != _START && !isCondition(proximo) && !isMethod(proximo) && !isFunction(proximo) && !isEndBlock(proximo) ){
                result = 15;
                Serial.println(F("[LOOKAHEAD] Erro 15: Proximo token invalido apos Fechar Condicao"));
                error_flag = true;
            }
        }

        // Token ENDBLOCK (9/11)
        else if(isEndBlock(token_da_vez)){
            if(proximo != _START && !isCondition(proximo) && !isMethod(proximo) && !isFunction(proximo) && !isEndBlock(proximo) && proximo != _END){
                result = 16;
                Serial.println(F("[LOOKAHEAD] Erro 16: Proximo token invalido apos Fechar Bloco"));
                error_flag = true;
            }
        }

        // Token ENDFUNCTION (10/11)
        else if(isEndFunction(token_da_vez)){
            if(proximo != _START && !isCondition(proximo) && !isMethod(proximo) && !isFunction(proximo) && !isEndBlock(proximo) && proximo != _END){
                result = 17;
                Serial.println(F("[LOOKAHEAD] Erro 17: Proximo token invalido apos Fechar Funcao"));
                error_flag = true;
            }
        }

       // Token LÓGICO (11/11)
        else if(isLogical(token_da_vez)){
            if(!isValue(proximo) && !isMethod(proximo) ){
                result = 18;
                Serial.println(F("[LOOKAHEAD] Erro 18: Operador Logico deve ser seguido por Valor ou Metodo"));
                error_flag = true;
            }
        }

        if(error_flag){
            Serial.print(F("[LOOKAHEAD] Atual/Prox: "));
            Serial.print(token_da_vez);
            Serial.print(F(" / "));
            Serial.println(proximo);
            break;
        }
    }

    if(!error_flag){
        Serial.print(F("[LOOKAHEAD] Sucesso: "));
        Serial.println(result);
    }

    return error_flag;
}

bool Syntax::Semantic(){
    bool error_flag = false;
    int token_da_vez;
    int qtd_conditions = 0;
    int qtd_functions = 0;
    int missing_endblocks = 0;
    bool is_in_condition = false;
    bool is_condition_ended = true;
    bool is_in_function = false;

    for (int i = 0; i < total; i++) {

        token_da_vez = tokens[i];

        if(isCondition(token_da_vez)){
            qtd_conditions++;
            missing_endblocks++;

        }else if(isEndCondition(token_da_vez)){
            qtd_conditions--;
        }

        if(isEndBlock(token_da_vez)){
            missing_endblocks--;
        }

        if(isFunction(token_da_vez)){
            qtd_functions++;
        }else if(isEndFunction(token_da_vez)){
            qtd_functions--;
        }

        // Se abertura de condições maior que zero, está em uma condição
        if(qtd_conditions > 0){
            is_in_condition = true;
        }else{
            is_in_condition = false;  // IMPORTANTE: resetar quando sair da condição
        }

        // Se abertura de funções maior que zero, está em uma função
        if(qtd_functions > 0){
            is_in_function = true;
        }else{
            is_in_function = false;  // IMPORTANTE: resetar quando sair da função
        }

        // Se não falta fechar nenhum bloco, é porque não foi aberto, logo, a condição foi terminada / nem começou
        if(missing_endblocks == 0){
            is_condition_ended = true;
        }

        if(is_in_condition){
            // Não pode ter método void
            if(isVoidMethod(token_da_vez)){
                Serial.println(F("[SEMANTIC] Erro: Não é permitido funções de comando (VOID) em condições"));
                error_flag = true;  
            }

            // Não pode ter comparação de varáveis de tipo diferente

            // Não pode lógicos infinitos, seguir estrutura variavel + operador + valor + lógico
        }else{
            // Não pode operações de comparação ou lógicas fora de condição
            if(isOperation(token_da_vez) || isLogical(token_da_vez)){
                Serial.println(F("[SEMANTIC] Erro: Operacoes fora de condicoes"));
                error_flag = true;
            }
        }
        
        // Não pode fechar bloco de condição 
        if(missing_endblocks < 0){
            Serial.println(F("[SEMANTIC] Erro: Fechou blocos sem poder"));
            error_flag = true;
        } else if(qtd_conditions < 0){
            Serial.println(F("[SEMANTIC] Erro: Fechou condicoes sem poder"));
            error_flag = true; 
        }else if(qtd_functions < 0){
            Serial.println(F("[SEMANTIC] Erro: Fechou funcoes sem poder"));
            error_flag = true; 
        }

        if(is_in_function){
            // Não pode nada além de valor numérico dentro de função
            if(!isNumberValue(token_da_vez) && !isFunction(token_da_vez) && !isEndFunction(token_da_vez)){
                Serial.println(F("[SEMANTIC] Erro: Funcao aceita apenas valores numericos"));
                error_flag = true; 
            }
        }

        // Para o loop se encontrar erro
        if(error_flag){
            break;
        }
    }

    if(!error_flag){
        Serial.print(F("[SEMANTIC] Sucesso: "));
        Serial.println(result);
    }
    
    return error_flag;
}