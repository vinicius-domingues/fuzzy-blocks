// --------------------------------------------------------------------------------------
// Fuzzy Interpreter 1.1
// --------------------------------------------------------------------------------------

1 - Executando
2 - Coordenando execuções

Regras:
1 - Não existe criação ou atribuição de variáveis, logo o sinal 'IGUAL' é usado para comparação e nunca recebimento     (Ex proibido: Segundos recebe cinco)
2 - Não pode ter comando com retorno (métodos não void) fora de condições                                               (Ex proibido: Verificar sensor de proximidade fora de IF ou WHILE)
3 - Não pode ter comandos sem retorno (métodos void) dentro de condições                                                (Ex proibido: Se acelerar igual a verdadeiro)
4 - Não pode ser comparado em uma condição, estruturas de tipos diferentes                                              (Ex proibido: Segundos igual a verdadeiro)

Sintaxe:
    Comandos do atuador:
        <MÉTODO>

        Exemplo:
            _HONK


    Comandos de função:
        <FUNCAO> <VALOR>

        Exemplo:
            _DELAY _FIVE


    Comandos de condição:
        <CONDICAO> <VARIAVEL> <OPERADOR> <VALOR>  (opcional)  <LÓGICO> <...>
            <...>
        <FIM BLOCO>

        Exemplo:
            _IF _SEGUNDOS _BIGGER _FIVE _AND _SEGUNDOS _SMALLER _FIFTY
                _HONK
            _ENDBLOCK

