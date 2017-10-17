#ifndef _TOKENS_H_   // Nao incluir duas vezes
#define _TOKENS_H_

//Includes
#include "strings.h"

//Macros
#define QUANTIDADE_DE_TOKENS 41
#define TAMANHO_DO_MAIOR_TOKEN 15

int resumo[QUANTIDADE_DE_TOKENS] = {0};


//typedefs
typedef enum{
	False, True
} Bool;

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

typedef struct{
	int LIN, COL, COD;
	tString LEXEMA;
	char TOKEN[TAMANHO_DO_MAIOR_TOKEN];
} tToken_resultante;

struct elemento{
    tToken_resultante dados;
    struct elemento *prox;
}; //definindo que cada elemento da lista posui os dados de um Token resultante e aponta para o prox

typedef struct elemento* Lista; //definindo a cabeca q ira apontar para os elementos
typedef struct elemento Elem; //disendo q cada elemento se chamara Elem


//Prototipos
Lista* CriaL();
void LiberaL(Lista*);
int InsereLf(Lista*, tToken_resultante);
int identificar_Token(const tString);
void inserir_Caractere_No_Lexema(const char, tString *);
const char * obter_Nome_Do_Token(tToken id_token);
void setar_Token (tToken_resultante*, const int, const char *, Bool* , const int, const int);


//Funcoes

#endif
