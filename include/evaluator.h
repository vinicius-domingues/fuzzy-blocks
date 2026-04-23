#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <Arduino.h>

class Evaluator{
  public:
    int* sequencia;
    int pc; // Position Counter
    int qtd_tokens;
    int is_loop;
    bool run = true;
  

    Evaluator(int dados[], int tamanho, bool is_loop);
    void Eval(unsigned long seconds);
};

#endif