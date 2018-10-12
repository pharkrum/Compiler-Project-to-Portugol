# Lexical analyzer in compiler project for Portugol language

## About

### Portugol 
Portugol is a pseudolanguage that allows 
the reader to develop algorithms structured in Portuguese in a simple and intuitive way, regardless of programming language.
[See in Wikipedia(PT)](https://pt.wikipedia.org/wiki/Portugol)
## Grammar
Programa → inicio Declarações Comandos fim <br>
Declarações → Declaração Declarações<br>
| ε<br>
Declaração → Tipo : Identificadores ;<br>
Tipo → int<br>
| dec<br>
Identificadores → Identificador , Identificadores<br>
| Identificador<br>
Comandos → Comando Comandos<br>
| ε<br>
Comando → CMD-Se | CMD-Para | CMD-Obtenha | CMD-Mostre | CMD-Atrib<br>
CMD-Se → se Expr entao Comandos senao Comandos fim_se<br>
| se Expr entao Comandos fim_se<br>
CMD-Para → para Identificador de Expr ate Expr passo Expr Comandos fim_para<br>
| para Identificador de Expr ate Expr Comandos fim_para<br>
CMD-Obtenha → leia ( Identificador ) ;<br>
CMD-Mostre → imprima ( Identificador ) ;<br>
| imprima ( Número );<br>
| imprima ( Cadeia ) ;<br>
CMD-Atrib → Identificador <- Expr ;<br>
Expr → Expr OpBinario Expr<br>
| OpUnario Expr<br>
| Número<br>
OpBinario → + | - | * | / | e | ou | = | <> | > | >= | < | <=<br>
OpUnario → - | ++ | -- | nao<br>
Número → Inteiro<br>
| Decimal<br>
## Automato
![alt text](https://github.com/pharkrum/Compiler-Project-to-Portugol/automato/automato.jpg "Automato")
## Transition Table

## Input

## Output
