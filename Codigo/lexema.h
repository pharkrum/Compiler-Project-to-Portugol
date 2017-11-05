/* ------------------------------------------------------------- */
/*                      ARQUIVO: lexema.h                        */
/*                                                               */
/* Certifique-se que este arquivo não é incluído mais de uma vez */
/* ------------------------------------------------------------- */
#ifndef _LEXEMA_H_
#define _LEXEMA_H_

/* -------------------------------------- */
/*      CONSTANTES, TIPOS E VARIÁVEIS     */
/* -------------------------------------- */
#ifndef LIMITE_INICIAL_DE_ALOCACAO
	#define LIMITE_INICIAL_DE_ALOCACAO 30
#endif

//Definicao de Estruturas - String
typedef struct{
	int tamanho_string;
	int limite_string;
	char * string;
} tSring;

//VARIAVEIS
tSring lexema;


/* ------------------------------- */
/*      PROTÓTIPOS DAS FUNÇÕES     */
/* ------------------------------- */
void iniciar_Lexema(void);
void realocar_Lexema(void);
void reiniciar_Lexema(void);
void inserir_Caractere_No_Lexema(const char);


/* ------------------------------- */
/*    IMPLEMENTAÇÃO DAS FUNÇÕES    */
/* ------------------------------- */
void iniciar_Lexema(void){
	//Define tamanho do lexema como zero (vazio) e tamanho maximo permitido para a insersao de caracteres, esse tamanho maximo pode ser alterado posteriormente
	lexema.limite_string = LIMITE_INICIAL_DE_ALOCACAO;
	lexema.tamanho_string = 0;
	lexema.string = (char*) malloc(lexema.limite_string * sizeof(char));
	if (lexema.string == NULL){
        printf("Erro durante a alocacao da string d lexema!!! \nInfelizmente o programa travou\n");
        exit(-1);
    }
    lexema.string[0] = '\0';
}


void realocar_Lexema(void){
	//Ao atingir o tamanho maximo, é nescessario realocar o tamanho da string do lexema, essa funcao é responsavel por isso
	lexema.limite_string *= 2;
	lexema.string = (char*) realloc (lexema.string, lexema.limite_string * sizeof(char));
	if (lexema.string == NULL){
        printf("Erro durante a realocacao da stringo do lexema!!! \nInfelizmente o programa travou\n");
        exit(-1);
    }
}


void reiniciar_Lexema(void){
	//Define tamanho do lexema como zero, se tornando uma string vazia
	lexema.tamanho_string = 0;
    lexema.string[0] = '\0';
}


void inserir_Caractere_No_Lexema(const char prox_Simb){
	//O nome da funcao é auto-explicativa
	if (lexema.tamanho_string == (lexema.limite_string - 2)) //Ao atingir o tamanho maximo, é nescessario realocar o tamanho da string do lexema
		realocar_Lexema();
	lexema.string[lexema.tamanho_string] = prox_Simb;
	lexema.tamanho_string++;
	lexema.string[lexema.tamanho_string] = '\0';
}

#endif
