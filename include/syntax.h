#ifndef SYNTAX_H
#define SYNTAX_H

#include <Arduino.h>


class Syntax {
    public:
        int result = 0; // success
        int* tokens;
        int total;

        Syntax(int dados[], int tamanho);
        bool Parser();
        bool LookAhead();
        
};

#endif