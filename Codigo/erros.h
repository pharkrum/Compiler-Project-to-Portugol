/* ------------------------------------------------------------- */
/* Certifique-se que este arquivo não é incluído mais de uma vez */
/* ------------------------------------------------------------- */
#ifndef _ERROS_H_
#define _ERROS_H_

/* -------------------------------------- */
/*      CONSTANTES, TIPOS E VARIÁVEIS     */
/* -------------------------------------- */
#ifndef LIMITE_INICIAL_DE_ALOCACAO
	#define LIMITE_INICIAL_DE_ALOCACAO 30
#endif

//Definicao de Tipos (ENUM) - Erros
typedef enum{
	er_delimitador_esperado,
	er_ponto_isolado,
	er_cadeia_nao_fechada,
	er_comentario_de_bloco_nao_fechado,
	er_caracter_invalido
} tErro;

//Definicao de Estruturas - Identificador de Erros Lexicos, Lista de erros lexicos
typedef struct{
	int LIN, COL;
	tErro ERRO;
	char CARACTER;
} tIndentificador_De_Erro;

typedef struct{
	int tamanho_lista;
	int limite_lista;
	tIndentificador_De_Erro * id_erro;
} tLista_de_erros;

//VARIAVEIS
tLista_de_erros lista_de_erros;


/* ------------------------------- */
/*      PROTÓTIPOS DAS FUNÇÕES     */
/* ------------------------------- */
void iniciar_Lista_De_Erros(void);
void adicionar_Erro_Na_Lista_De_Erros(const tErro, const char, const int, const int);
const char * obter_Nome_Do_Erro(const tErro);


/* ------------------------------- */
/*    IMPLEMENTAÇÃO DAS FUNÇÕES    */
/* ------------------------------- */
void iniciar_Lista_De_Erros(void){
	//Define tamanho da lista de erros como zero (vazio) e tamanho maximo permitido para a insersao de erros, esse tamanho maximo pode ser alterado posteriormente
	lista_de_erros.tamanho_lista = 0;
	lista_de_erros.limite_lista = LIMITE_INICIAL_DE_ALOCACAO;
	lista_de_erros.id_erro = (tIndentificador_De_Erro *) malloc (lista_de_erros.limite_lista * sizeof(tIndentificador_De_Erro));
	if (lista_de_erros.id_erro == NULL){
        printf("Erro durante a alocacao da lista de erros!!! \nInfelizmente o programa travou\n");
        exit(-1);
    }
}


void adicionar_Erro_Na_Lista_De_Erros(const tErro erro, const char c, const int linha, const int coluna){
	//Setar o erro
	lista_de_erros.id_erro[lista_de_erros.tamanho_lista].LIN = linha;
	lista_de_erros.id_erro[lista_de_erros.tamanho_lista].COL = coluna;
	lista_de_erros.id_erro[lista_de_erros.tamanho_lista].ERRO = erro;
	lista_de_erros.id_erro[lista_de_erros.tamanho_lista].CARACTER = c;
	lista_de_erros.tamanho_lista++;
	
	//Verificar tamanho da alocacao
	if (lista_de_erros.tamanho_lista  == lista_de_erros.limite_lista-1){
		lista_de_erros.limite_lista *= 2;
		lista_de_erros.id_erro = (tIndentificador_De_Erro *) realloc(lista_de_erros.id_erro, lista_de_erros.limite_lista * sizeof(tIndentificador_De_Erro));
		if (lista_de_erros.id_erro == NULL){
			printf("Erro durante a realocacao da lista de erros!!! \nInfelizmente o programa travou\n");
			exit(-1);
		}
	}
}


const char * obter_Nome_Do_Erro(const tErro id_token){
	const char * NOMES[] = {
		"Delimitador esperado",
		"Ponto isolado",
		"Cadeia nao fechada",
		"Comentario de Bloco nao fechado",
		"Caracter Invalido"
	};
	return (NOMES[id_token]);
}

#endif 
