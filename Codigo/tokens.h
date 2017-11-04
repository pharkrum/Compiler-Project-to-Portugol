/* ------------------------------------------------------------- */
/* Certifique-se que este arquivo não é incluído mais de uma vez */
/* ------------------------------------------------------------- */
#ifndef _TOKENS_H_
#define _TOKENS_H_

/* --------------------------- */
/* BIBLIOTECAS E INCLUDE FILES */
/* --------------------------- */
#include "lexema.h"

/* -------------------------------------- */
/*      CONSTANTES, TIPOS E VARIÁVEIS     */
/* -------------------------------------- */
#ifndef LIMITE_INICIAL_DE_ALOCACAO
	#define LIMITE_INICIAL_DE_ALOCACAO 30
#endif

#define TAM_TAB_HASH_SIMBOLOS 139
#define hash(v) ((2*v)+3)%139 //Multiplique, Adicione e Divida (MAD)

//Definicao de Tipos (ENUM) - Tokens
typedef enum {
	tk_EOF,
	tk_IDEN,
	tk_INTEIRO,
	tk_DECIMAL,
	tk_CADEIA,
	tk_inicio,
	tk_fim,
	tk_int,
	tk_dec,
	tk_leia,
	tk_imprima,
	tk_para,
	tk_de,
	tk_ate,
	tk_passo,
	tk_fim_para,
	tk_se,
	tk_entao,
	tk_senao,
	tk_fim_se,
	tk_e,
	tk_ou,
	tk_nao,
	tk_virg,
	tk_pt_virg,
	tk_dois_pts,
	tk_abre_par,
	tk_fecha_par,
	tk_menor,
	tk_menor_igual,
	tk_maior,
	tk_maior_igual,
	tk_diferente,
	tk_igual,
	tk_incr,
	tk_decr,
	tk_atrib,
	tk_mais,
	tk_menos,
	tk_vezes,
	tk_dividido
} tToken;

//Definicao de Estruturas - Identificador de token, Lista de tokens
typedef struct{
	int LIN, COL;
	tToken TOKEN;
	int posisao_na_tabela_de_simbolos;
} tIndentificador_De_Token;

typedef struct{
	int tamanho_lista;
	int limite_lista;
	tIndentificador_De_Token * id_token;
} tLista_de_tokens;

//Tabela Hash + Definicao dos Atributos da tabela de simbolos
typedef struct{
	int LIN, COL;
} tPos;

typedef struct simbolo{
	tToken COD;
	int posicao;
	
	char * lexema_cadeia;
	int lexema_inteiro;
	float lexema_decimal;
	
	tPos *ocorrencias;
	int tamanho_ocorrencias;
	int limite_ocorrencias;
	
	struct simbolo * proximo; //Colisao: endereçamento separado
} tSimbolo;

//Mecanismo que conecta, para cada par token-lexema, a ordem em que ele ocorre na entrada e sua posição na tabela de símbolos
typedef struct {
	int tab_simb_count;
	int limite_tab_simb_count;
	tSimbolo** ordem_de_entrada_da_tab_simbolos;
} tOrdem;

//VARIAVEIS
tLista_de_tokens lista_de_tokens;
int linha_token, coluna_token;
tSimbolo ** tab_simbolos;
tOrdem ordem_de_entrada;


/* ------------------------------- */
/*      PROTÓTIPOS DAS FUNÇÕES     */
/* ------------------------------- */
//TOKENS
int identificar_Token(void);
void iniciar_Lista_De_Tokens(void);
void adicionar_Token_Na_Lista_De_Tokens(const tToken, const int, const int);
const char * obter_Nome_Do_Token(const tToken);
//TABELA DE SIMBOLOS
void iniciar_Tabela_de_Simbolos(void);
void liberar_Tabela_Simbolos(void);
void iniciar_Ordem_Tab(void);
int hash_Com_Shift(void);
void adiconar_Na_Tabela_De_Simbolos(const tToken);
tSimbolo * buscar_Na_Tabela_De_Simbolos(const tToken, const int);
void adiciona_Ocorrencia(tSimbolo *);


/* ------------------------------- */
/*    IMPLEMENTAÇÃO DAS FUNÇÕES    */
/* ------------------------------- */
int identificar_Token(void){
	//Ao encontrar uma palavra qualquer, verifique se é palavra reservada com uma função que retorna o código da palavra reservada
	//ou –1 se o identificador não for palavra reservada.
	//Tal função deve conhecer as palavras reservadas da linguagem.
	const char * palavras_reservadas[] = {"inicio", "fim", "int", "dec", "leia", "imprima", "para", "de", "ate",
										  "passo", "fim_para", "se", "entao", "senao", "fim_se", "e", "ou", "nao"};
										  
	char * aux = (char *) malloc(lexema.tamanho_string * sizeof(char));
	strcpy(aux, lexema.string); //Auxiliar, Letras maiúsculas e minúsculas são distinguidas em nomes de identificadoresm, mas, é nescessario observar o que vem a seguir:

	for(unsigned int i = 0; i < strlen(aux); i++) // Torna todas as letras minusculas Letras maiúsculas e minúsculas nao são distinguidas em palavras reservadas
		aux[i] = tolower(aux[i]);
		
	for(int i = 0; i < 18; i++) { //18 = numero de palavras_reservadas
		if(strcmp(aux, palavras_reservadas[i]) == 0)
			return (i + tk_inicio);
	}

	return -1;
}


void iniciar_Lista_De_Tokens(void){
	//Define tamanho da lista de tokens reconhecidos e suas ocorrencias como zero (vazio) e tamanho maximo permitido para a insersao, esse tamanho maximo pode ser alterado posteriormente
	lista_de_tokens.tamanho_lista = 0;
	lista_de_tokens.limite_lista = LIMITE_INICIAL_DE_ALOCACAO;
	lista_de_tokens.id_token = (tIndentificador_De_Token *) malloc (lista_de_tokens.limite_lista * sizeof(tIndentificador_De_Token));
	if (lista_de_tokens.id_token == NULL){
        printf("Erro durante a alocacao da lista de tokens!!! \nInfelizmente o programa travou\n");
        exit(-1);
    }
}


void adicionar_Token_Na_Lista_De_Tokens(const tToken token, const int linha, const int coluna){
	//Adicionar token
	lista_de_tokens.id_token[lista_de_tokens.tamanho_lista].LIN = linha;
	lista_de_tokens.id_token[lista_de_tokens.tamanho_lista].COL = coluna;
	if (token == tk_EOF)
		lista_de_tokens.id_token[lista_de_tokens.tamanho_lista].COL--;
	lista_de_tokens.id_token[lista_de_tokens.tamanho_lista].TOKEN = token;
	
	//Posicao na tabela de simbolos
	if(token == tk_INTEIRO || token == tk_DECIMAL || token == tk_CADEIA || token == tk_IDEN)
		lista_de_tokens.id_token[lista_de_tokens.tamanho_lista].posisao_na_tabela_de_simbolos = hash_Com_Shift();
	else
		lista_de_tokens.id_token[lista_de_tokens.tamanho_lista].posisao_na_tabela_de_simbolos = -1; //Nao existe
	
	//Novo tamanho
	lista_de_tokens.tamanho_lista++;
	
	//Verificar tamanho da alocacao
	if (lista_de_tokens.tamanho_lista  == lista_de_tokens.limite_lista-1){
		lista_de_tokens.limite_lista *= 2;
		lista_de_tokens.id_token = (tIndentificador_De_Token *) realloc(lista_de_tokens.id_token, lista_de_tokens.limite_lista * sizeof(tIndentificador_De_Token));
		if (lista_de_tokens.id_token == NULL){
			printf("Erro durante a realocacao da lista de tokens!!! \nInfelizmente o programa travou\n");
			exit(-1);
		}
	}
}


const char * obter_Nome_Do_Token(const tToken id_token){
	const char * NOMES[] = {
		"tk_EOF",
		"tk_IDEN",
		"tk_INTEIRO",
		"tk_DECIMAL",
		"tk_CADEIA",
		"tk_inicio",
		"tk_fim",
		"tk_int",
		"tk_dec",
		"tk_leia",
		"tk_imprima",
		"tk_para",
		"tk_de",
		"tk_ate",
		"tk_passo",
		"tk_fim_para",
		"tk_se",
		"tk_entao",
		"tk_senao",
		"tk_fim_se",
		"tk_e",
		"tk_ou",
		"tk_nao",
		"tk_virg",
		"tk_pt_virg",
		"tk_dois_pts",
		"tk_abre_par",
		"tk_fecha_par",
		"tk_menor",
		"tk_menor_igual",
		"tk_maior",
		"tk_maior_igual",
		"tk_diferente",
		"tk_igual",
		"tk_incr",
		"tk_decr",
		"tk_atrib",
		"tk_mais",
		"tk_menos",
		"tk_vezes",
		"tk_dividido"
	};
	return (NOMES[id_token]);
}


void iniciar_Tabela_de_Simbolos(void){
	//inicia a tabela de simbolos com tamanho máximo do numero primo definido
	tab_simbolos = (tSimbolo **) malloc(TAM_TAB_HASH_SIMBOLOS * sizeof(tSimbolo *));
	if (tab_simbolos == NULL) {
		printf("Erro durante a alocacao da tabela de simbolos!!! \nInfelizmente o programa travou\n");
        exit(-1);
	}
	
	//Inicia todas as posições desocupadas
	for (int i = 0; i < TAM_TAB_HASH_SIMBOLOS; i++) 
      tab_simbolos[i] = NULL; 
}


void liberar_Tabela_Simbolos(void){
	//Funcao para liberar memoria alocada dos itens da tabela de simbolo (incluindo o vetor de ocorrencias)
	for(int i=0; i < TAM_TAB_HASH_SIMBOLOS; i++){
		tSimbolo *ant, *atual;
		ant = atual = tab_simbolos[i];
		while (atual != NULL){
			atual = atual->proximo;
			free(ant->ocorrencias);
			free(ant);
			ant = atual;
		}
	}
	free(tab_simbolos);
}


void iniciar_Ordem_Tab(void){
	//Inicia o Mecanismo que conecta, para cada par token-lexema, a ordem em que ele ocorre na entrada e sua posição na tabela de símbolos
	//definindo o tamanho maximo permitido para a insersao, esse tamanho maximo pode ser alterado posteriormente
	ordem_de_entrada.tab_simb_count = 0;
	ordem_de_entrada.limite_tab_simb_count = LIMITE_INICIAL_DE_ALOCACAO;
	ordem_de_entrada.ordem_de_entrada_da_tab_simbolos = (tSimbolo**) malloc (ordem_de_entrada.limite_tab_simb_count * sizeof(tSimbolo*));
	if (ordem_de_entrada.ordem_de_entrada_da_tab_simbolos == NULL){
        printf("Erro durante a alocacao da lista de oredenacao!!! \nInfelizmente o programa travou\n");
        exit(-1);
    }
}


int hash_Com_Shift(void){
	//Funcao HASH com shift: após ela é realizada a compressao (o valor deve ser positivo pois rdefinirar o indice do Hash)
	int h = 0;
	for (int i = 0; i < lexema.tamanho_string; i++){
		h += lexema.string[i];
		h <<= 2; //shift de 2 bits na soma atual
	}
	return abs(hash(h)); //Compressao do valor h obtido
}


void adiconar_Na_Tabela_De_Simbolos(const tToken tk){
	int posicao = hash_Com_Shift();
	tSimbolo * simb = buscar_Na_Tabela_De_Simbolos(tk, posicao);
	
	if(simb != NULL){ //se o token já está instalado na tab simbolos, adicionar ocorrencia
		adiciona_Ocorrencia(simb);
	} else {
		//Setar as caracteristicas do novo simbolo
		simb = (tSimbolo *) malloc(sizeof(tSimbolo));
		if (simb  == NULL){
			printf("Erro durante a alocacao de um novo simbolo na tabela hash!!! \nInfelizmente o programa travou\n");
			exit(-1);
		}
		
		simb->COD = tk;
		simb->posicao = posicao;
		simb->lexema_cadeia = (char *) malloc(lexema.tamanho_string * sizeof(char));
		strcpy(simb->lexema_cadeia, lexema.string);
		
		if (tk == tk_INTEIRO)
			simb->lexema_inteiro = atoi(lexema.string);
		else if (tk == tk_DECIMAL)
			simb->lexema_decimal = atof(lexema.string);
			
		simb->tamanho_ocorrencias = 0;
		simb->limite_ocorrencias = LIMITE_INICIAL_DE_ALOCACAO;
		simb->ocorrencias = (tPos*) malloc (simb->limite_ocorrencias * sizeof(tPos));
		if (simb->ocorrencias == NULL){
			printf("Erro durante a alocacao da lista de ocorrencias!!! \nInfelizmente o programa travou\n");
			exit(-1);
		}
		adiciona_Ocorrencia(simb);
		
		//Adicionar na tabela com Insersao no inicio
		simb->proximo = tab_simbolos[posicao];
		tab_simbolos[posicao] = simb;
		
		//Adicionando
		ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[ordem_de_entrada.tab_simb_count] = simb;
		ordem_de_entrada.tab_simb_count++;
		if (ordem_de_entrada.tab_simb_count == ordem_de_entrada.limite_tab_simb_count-1){
			ordem_de_entrada.limite_tab_simb_count *= 2;
			ordem_de_entrada.ordem_de_entrada_da_tab_simbolos = (tSimbolo**) realloc (ordem_de_entrada.ordem_de_entrada_da_tab_simbolos, ordem_de_entrada.limite_tab_simb_count * sizeof(tSimbolo*));
			if (ordem_de_entrada.ordem_de_entrada_da_tab_simbolos == NULL){
				printf("Erro durante a realocacao da lista de ordenacao!!! \nInfelizmente o programa travou\n");
				exit(-1);
			}
		}
	}
}


tSimbolo * buscar_Na_Tabela_De_Simbolos(const tToken tk, const int pos){
	//combinação token–lexema deve ser incluida uma unica vez na tabela
	//É realizada uma busca pela combinacao
	tSimbolo * simb = tab_simbolos[pos];
	while (simb != NULL) {
		if (simb->COD == tk && (strcmp(lexema.string, simb->lexema_cadeia) == 0)) {
			return simb;
		}
		simb = simb->proximo;
	}
   	return NULL;
}


void adiciona_Ocorrencia(tSimbolo * simb){
	//Funcao que armazena a linha e coluna da ocorrencia de um token com lexema na tabela de simbolos
	simb->ocorrencias[simb->tamanho_ocorrencias].LIN = linha_token;
	simb->ocorrencias[simb->tamanho_ocorrencias].COL = coluna_token;
	simb->tamanho_ocorrencias++;
	
	//Verificar tamanho da alocacao
	if(simb->tamanho_ocorrencias == simb->limite_ocorrencias-1){
		simb->limite_ocorrencias *= 2;
		simb->ocorrencias = (tPos*) realloc (simb->ocorrencias, simb->limite_ocorrencias * sizeof(tPos));
		if (simb->ocorrencias == NULL){
			printf("Erro durante a realocacao da lista de ocorrencias!!! \nInfelizmente o programa travou\n");
			exit(-1);
		}
	}
}

#endif
