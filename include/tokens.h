#ifndef TOKENS_H
#define TOKENS_H

#define _END -1

// Fluxo inicial
#define _START 1  

// Condição
#define _WHILE 2
#define _IF 3

// Fechadores solitários
#define _ENDCONDITION 4
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
// Void
#define _BRAKE 100     // Comando para freiar (VOID)
#define _ACCELERATE 101   // Comando para acelerar (VOID)
#define _HONK 103    // Comando para buzinar (VOID)
#define _RED_LED 104  // Comando para acender led vermelho (VOID)
#define _GREEN_LED 105     // Comando para acender led verde (VOID)
#define _BLUE_LED 106      // CComando para acender led azul (VOID)

// Não void
#define _PROXIMITY 200 // Comando para ler sensor

// Função
#define _DELAY 300

// Valor
#define _ZERO 400
#define _ONE 401
#define _FIVE 402
#define _FIFTY 403
#define _THOUSAND 404
#define _TRUE 500
#define _FALSE 501

// Varáveis
#define _SEGUNDOS 600

// Etapas
#define _LISTENING 0
#define _MAPPING 1
#define _INTERPRETING 2
#define _RUNNING 3
#define _FINISHING 4

#endif

// 1. Ações e Sensores (Separados para corrigir erros semânticos)
inline bool isVoidMethod(int t)        { return (t >= _BRAKE && t <= _BLUE_LED); } 
inline bool isNonVoidMethod(int t)         { return (t >= _PROXIMITY && t <= _PROXIMITY); }                      
inline bool isMethod(int t)         { return (isVoidMethod(t) || isNonVoidMethod(t)); } 

// 2. Valores e Variáveis

inline bool isVariable(int t)            { return (t == _SEGUNDOS); }   
inline bool isValue(int t)          { return (t >= _ZERO && t <= _FALSE); }          
inline bool isBooleanValue(int t)            { return (t >= _TRUE && t <= _FALSE); }        
inline bool isNumberValue(int t)            { return (t >= _ZERO && t <= _THOUSAND); }

// 3. Operadores
inline bool isOperation(int t)       { return (t >= _EQUAL && t <= _SMALLER); } 
inline bool isLogical(int t)        { return (t >= _AND && t <= _OR); }        

// 4. Estruturas e Funções
inline bool isCondition(int t)       { return (t == _IF || t == _WHILE); }      
inline bool isFunction(int t)       { return (t == _DELAY); }                 

// 5. Fechadores
inline bool isEndCondition(int t)        { return (t == _ENDCONDITION); }                
inline bool isEndBlock(int t)       { return (t == _ENDBLOCK); }               
inline bool isEndFunction(int t)    { return (t == _ENDFUNCTION); }            