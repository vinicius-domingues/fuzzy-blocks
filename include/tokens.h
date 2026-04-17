#ifndef TOKENS_H
#define TOKENS_H

#define _END -1

// Fluxo inicial
#define _START 1  

// Condição
#define _WHILE 2
#define _IF 3

// Fechadores solitários
#define _ENDCOND 4
#define _ENDBLOCK 5
#define _ENDFUNCTION 6

// Operação / Recebe
#define _EQUAL 10
#define _BIGGER 11
#define _SMALLER 12

// Lógico
#define _AND 20
#define _OR 21

// Método
#define _FREIA 100
#define _ACELERA 101
#define _OBSTACULO 102
#define _BUZINA 103
#define _VERMELHO 104
#define _VERDE 105
#define _AZUL 106

// Função
#define _ESPERA 200

// Valor
#define _ZERO 300
#define _UM 301
#define _CINCO 302
#define _CINQUENTA 303
#define _MIL 304
#define _TRUE 400
#define _FALSE 401

// Varáveis
#define _SEGUNDOS 500

// Etapas
#define ESCUTANDO 0
#define MAPEANDO 1
#define INTERPRETANDO 2
#define EXECUTANDO 3
#define FINALIZANDO 4

#endif

inline bool isMetodo(int t) {
    return (t >= _FREIA && t <= _AZUL); 
}

inline bool isValor(int t) {
    return (t >= _ZERO && t <= _FALSE); 
}

inline bool isOperacao(int t) {
    return (t >= _EQUAL && t <= _SMALLER); 
}

inline bool isCondicao(int t) {
    return (t == _IF || t == _WHILE); 
}

inline bool isFunction(int t) {
    return (t >= _ESPERA && t <= _ESPERA); 
}

inline bool isVar(int t) {
    return (t >= _SEGUNDOS && t <= _SEGUNDOS); 
}

inline bool isEndCond(int t) {
    return (t >= _ENDCOND && t <= _ENDCOND); 
}

inline bool isEndBlock(int t) {
    return (t >= _ENDBLOCK && t <= _ENDBLOCK); 
}

inline bool isEndFunction(int t) {
    return (t >= _ENDFUNCTION && t <= _ENDFUNCTION); 
}

inline bool isLogical(int t){
    return (t >= _AND || t <= _OR); 
}
