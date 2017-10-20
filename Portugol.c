#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#include "Codigo/tokens.h"
//#include "Codigo/automato.h"
//#include "Codigo/erros.h"
//#include "Codigo/tabSimbolos.h"
//#include "Codigo/resultados.h"


#define TOTAL_CLASSES_CARACTERES 21
//#define QUANTIDADE_DE_TOKENS 41
//#define TAMANHO_DO_MAIOR_NOME_TOKEN 15
#define QUANTIDADE_DE_ESTADOS 45
#define LIMITE_INICIAL_DE_ALOCACAO 255


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

typedef enum{
	tc_branco,
	tc_quebra_linha,
	tc_letra,
	tc_digito,
	tc_underline,
	tc_aspas,
	tc_ponto,
	tc_virgula,
	tc_ponto_virgula,
	tc_dois_pontos,
	tc_abre_parenteses,
	tc_fecha_parenteses,
	tc_menor,
	tc_igual,
	tc_maior,
	tc_mais,
	tc_menos,
	tc_vezes,
	tc_dividido,
	tc_EOF,
	tc_outro
} tClasse_caractere;

typedef enum{
	er_caracter_invalido,
	er_delimitador_esperado,
	er_ponto_isolado,
	er_cadeia_nao_fechada,
	er_comentario_de_bloco_nao_fechado
} tErro;


typedef struct{
	int tamanho_string;
	int limite_string;
	char * string;
} tSring;

//struct simbolos
//Para cada token: COD, LEXEMA, ocorrencias(LIN, COL)
//combinação token–lexema incluida uma única vez na tabela

//tabela de simbolos HASH


///PROTOTIPOS
tToken analizador_Lexico(void);
void inicia_Tabela_Transicoes (void);
char leia_Proximo_Caractere(void);
void retrocede_Caracteres(const int, const char);
tClasse_caractere carctere_2_tClasse_caractere(char prox_Simb);
//void alocar_Vetor(tToken *, limite_de_caracteres);
//void realocar_Vetor(tToken *, limite_de_caracteres);
//void desalocar_Vetor(tToken *);
void iniciar_Lexema(void);
void realocar_Lexema(void);
void reiniciar_Lexema(void);
void inserir_Caractere_No_Lexema(char prox_Simb);
//int identificar_Token(void);
//setar_Erro();
//retrocede_Ate();
//adiconar_Na Tabela De Simbolos();

//imprimir_Lista_De_Erros_Lexicos();
//imprimir_Lista_De_Tokens_Reconhecidos();
//imprimir_Resumo();
//imprimir_Tabela_De_Simbolos();

///VARIAVEIS
FILE *arquivo_de_entrada;
char tabela_Transicoes[QUANTIDADE_DE_ESTADOS][TOTAL_CLASSES_CARACTERES];
int linha_arquivo = 1, coluna_arquivo = 0, linha_token, coluna_token;
tSring lexema;





///FUNCAO PRINCIPAL
int main (int argc, char *argv[]){
	tToken * token_da_vez;
	int numero_de_tokens, limite_de_caracteres;
	
	if (argc < 2){
		printf("Exemplo de execucao: ./Portugol prog01.ptg\nTente novamente\n\n");
	} else {
		inicia_Tabela_Transicoes();
		iniciar_Lexema();
		
		/*printf("      b  \\n  l  d  _  \"  .  ,  ;  :  (  )  <  =  >  +  -  *  /  e  ot\n");
		for (int i = 0; i < QUANTIDADE_DE_ESTADOS; i++){
			printf("%3d |", i);
			for (int j = 0; j < TOTAL_CLASSES_CARACTERES; j++){
				printf ("%3d", tabela_Transicoes[i][j]);
			}
			printf("\n");
		}*/
		
		for (int i=1; i < argc; i++){
			printf("\nArquivo : %s \n", argv[i]);
			
			if ((arquivo_de_entrada = fopen(argv[i], "r")) == NULL){
				printf("Erro ao abrir o arquivo!!! \nPor favor, verifique a existencia do mesmo e tente novamente.\n");
				exit(1);
			}
			
			///Iniciando tabela com os tokens recebidos
			numero_de_tokens = 0;
			limite_de_caracteres = LIMITE_INICIAL_DE_ALOCACAO;
			token_da_vez = (tToken *) malloc(limite_de_caracteres * sizeof(tToken));
			
			///Recebendo tokens
			do {
				token_da_vez[numero_de_tokens] = analizador_Lexico();
				numero_de_tokens++;
				if (numero_de_tokens == limite_de_caracteres-1){
					limite_de_caracteres *= 2;
					token_da_vez = (tToken *) realloc(token_da_vez, limite_de_caracteres * sizeof(tToken));
				}
				//ADD TOKEN NA LISTA
				printf("Token: %d   Lexema: %s\n", token_da_vez[numero_de_tokens-1], lexema.string);
			} while(token_da_vez[numero_de_tokens-1] != tk_EOF);

			
			//imprimir_Lista_De_Erros_Lexicos(argv[i]);
			//imprimir_Lista_De_Tokens_Reconhecidos(argv[i], token_da_vez, n);
			//imprimir_Resumo();
			//imprimir_Tabela_De_Simbolos(argv[i]);
			
			printf("Os seguintes arquivos gerados:\n");
			printf("   %s.err com o conteúdo do arquivo de entrada e os erros léxicos devidamente marcados\n", argv[1]);
			printf("   %s.tbl com a lista de tokens reconhecidos\n", argv[1]);
			printf("   %s.tok com o conteúdo da tabela de símbolos após processamento.\n\n", argv[1]);
			
			///Free de token_da_vez
			free(token_da_vez);
			
			///Fechando Arquivo de entrada
			fclose(arquivo_de_entrada);
		}
		///Free tString lexema
		free(lexema.string);
	}
	return 0;
}






tToken analizador_Lexico(void){
	int estado = 0, id_token, contador_de_bloco;
	char prox_Simb = ' ';
	
	reiniciar_Lexema();
	while (1){
		switch (estado){
			case 0: ///Estado Inicial
				linha_token = linha_arquivo;
				coluna_token = coluna_arquivo;
				break;
				
			case 1: ///Estado identificador ou palavra reservada
				inserir_Caractere_No_Lexema(prox_Simb);
				break;
				
			case 2: /// Estado de verificacao Palavra reservada ou identificador (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				id_token = -1;
				//id_token = identificar_Token();
				if (id_token == -1){ //Identificador
					//Adiconar identificador na tabela de simbolos (FAZER)
					return (tk_IDEN);
				} else { //Palavra reservada
					return (id_token);
				}
				break;

			case 3: /// Estado Digito (inteiro ou decimal)
				inserir_Caractere_No_Lexema(prox_Simb);
				break;

			case 4: /// Estado Digito Inteiro (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				//Adicionar inteiro na tabela de simbolos (FAZER)
				return (tk_INTEIRO);
				break;
				
			case 5: ///Estado de erro lexico apos inteiro (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				//setar_Erro("Delimitador esperado");
				//Adicionar inteiro na tabela de simbolos (FAZER)
				return (tk_INTEIRO);
				break;
				
			case 6: ///Estado Digito Decimal apos o ponto
				inserir_Caractere_No_Lexema(prox_Simb);
				break;
				
			case 7: /// Estado Digito Decimal (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				//Adicionar Decimal na tabela de simbolos (FAZER)
				return (tk_DECIMAL);
				break;

			case 8: ///Estado de erro lexico apos decimal (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				//setar_Erro("Delimitador esperado");
				//Adicionar Decimal na tabela de simbolos (FAZER)
				return (tk_DECIMAL);
				break;
			
			case 9: /// Estado Digito Decimal Iniciando com ponto
				inserir_Caractere_No_Lexema(prox_Simb);
				break;
				
			case 10: ///Estado de erro lexico Ponto Isolado (FINAL)
				retrocede_Caracteres(2, prox_Simb); //Para ser lido novamente e mudar para q0 (para todo char lido)
				//setar_Erro("Ponto isolado");
				break;

			case 11: ///Estado Cadeia
				inserir_Caractere_No_Lexema(prox_Simb);
				break;

			case 12: ///Estado Cadeia (FINAL)
				inserir_Caractere_No_Lexema(prox_Simb);
				//Adiciona cadeia na tabela de simbolos (FAZER)
				return (tk_CADEIA);
				break;
			
			case 13: ///Estado de erro lexico nao fechameno da cadeia (FINAL)
				//setar_Erro("Cadeia nao fechada");
				retrocede_Caracteres(1, prox_Simb);
				//Adiciona cadeia na tabela de simbolos (FAZER)
				return (tk_CADEIA);
				break;

			case 14: ///Estado End of File (FINAL)
				return (tk_EOF);
				break;

			case 15: ///Estado Virgula (FINAL)
				return (tk_virg);
				break;

			case 16: ///Estado Ponto-e-virgula (FINAL)
				return (tk_pt_virg);
				break;

			case 17: ///Estado Dois-Pontos (FINAL)
				return (tk_dois_pts);
				break;

			case 18: ///Estado Abre Parenteses
				contador_de_bloco = 0;
				break;
				
			case 19: ///Comentario de bloco
				contador_de_bloco++;
				break;
				
			case 20: ///Estado Abre Parenteses (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				return (tk_abre_par);
				break;
				
			case 21: ///Possivel fim de comentario de bloco
				contador_de_bloco++;
				break;

			case 22: ///Fim do comentario de bloco
				retrocede_Caracteres(1, prox_Simb); //Para ser lido novamente e mudar para q0 (para todo char lido)
				break;

			case 23: ///Estado de erro lexico Comentario de bloco não fechado
				//retrocede_Ate(contador_de_bloco, linha_token, coluna_token); //(muitos) Para serem lidos novamente mudando para q44 (para todo char lido)
				//setar_Erro("Comentario de bloco nao fechado");
				break;
			
			case 24:  ///Estado Fecha Parenteses (FINAL)
				return (tk_fecha_par);
				break;

			case 25: ///Estado <
				break;

			case 26: ///Estado Atribuicao (FINAL)
				return (tk_atrib);
				break;

			case 27: ///Estado Diferente (FINAL)
				return (tk_diferente);
				break;

			case 28: ///Estado Menor Igual (FINAL)
				return (tk_menor_igual);
				break;

			case 29: ///Estado Menor (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				return (tk_menor);
				break;

			case 30: ///Estado Igual (FINAL)
				return (tk_igual);
				break;

			case 31: ///Estado >
				break;

			case 32: ///Estado Maior Igual (FINAL)
				return (tk_maior_igual);
				break;

			case 33: ///Estado Maior (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				return (tk_maior);
				break;

			case 34: ///Estado +
				break;

			case 35: ///Estado Incremento (FINAL)
				return (tk_incr);
				break;

			case 36: ///Estado Mais (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				return (tk_mais);
				break;

			case 37: ///Estado -
				break;

			case 38: ///Estado Decremento (FINAL)
				return (tk_decr);
				break;

			case 39: ///Estado Menos (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				return (tk_menos);
				break;

			case 40: ///Estado Vezes (FINAL)
				return (tk_vezes);
				break;

			case 41: ///Estado /
				break;

			case 42: ///Estado Comentario de linha
				break;

			case 43: ///Estado Dividido (FINAL)
				retrocede_Caracteres(1, prox_Simb);
				return (tk_dividido);
				break;

			default: ///Estado Defauult: Apresnta erro e ignora o caractere
				//setar_Erro("Caracter invalido");
				retrocede_Caracteres(1, prox_Simb); //Para ser lido novamente e mudar para q0 (para todo char lido)
				break;
		}
		prox_Simb = leia_Proximo_Caractere();
		estado = tabela_Transicoes[estado][carctere_2_tClasse_caractere(prox_Simb)];
	}
	return 0;
}


void inicia_Tabela_Transicoes (void) {
	char aux[QUANTIDADE_DE_ESTADOS][TOTAL_CLASSES_CARACTERES] = {
		{0, 0, 1, 3, 44, 11, 9, 15, 16, 17, 18, 24, 25, 30, 31, 34, 37, 40, 41, 14, 44},  //Estado 0
		{2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},  //Estado 1
		{ 0 },  //Estado 2
		{4, 4, 5, 3, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},  //Estado 3
		{ 0 },  //Estado 4
		{ 0 },  //Estado 5
		{7, 7, 8, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},  //Estado 6
		{ 0 },  //Estado 7
		{ 0 },  //Estado 8
		{10, 10, 6, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},  //Estado 9
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //Estado 10
		{11, 13, 11, 11, 11, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 13, 11},  //Estado 11
		{ 0 },  //Estado 12
		{ 0 },  //Estado 13
		{ 0 },  //Estado 14
		{ 0 },  //Estado 15
		{ 0 },  //Estado 16
		{ 0 },  //Estado 17
		{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 19, 20, 20, 20},  //Estado 18
		{19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 21, 19, 23, 19},  //Estado 19
		{ 0 },  //Estado 20
		{19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 22, 19, 19, 19, 19, 19, 21, 19, 23, 19},  //Estado 21
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //Estado 22
		{42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42},  //Estado 23
		{ 0 },  //Estado 24
		{29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 28, 27, 29, 26, 29, 29, 29, 29},  //Estado 25
		{ 0 },  //Estado 26
		{ 0 },  //Estado 27
		{ 0 },  //Estado 28
		{ 0 },  //Estado 29
		{ 0 },  //Estado 30
		{33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 32, 33, 33, 33, 33, 33, 33, 33},  //Estado 31
		{ 0 },  //Estado 32
		{ 0 },  //Estado 33
		{36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 35, 36, 36, 36, 36, 36},  //Estado 34
		{ 0 },  //Estado 35
		{ 0 },  //Estado 36
		{39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 38, 39, 39, 39, 39},  //Estado 37
		{ 0 },  //Estado 38
		{ 0 },  //Estado 39
		{ 0 },  //Estado 40
		{43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 42, 43, 43, 43},  //Estado 41
		{42, 0, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 14, 42},  //Estado 42
		{ 0 },  //Estado 43
		{ 0 },  //Estado 44
	};
	
	for (int i = 0; i < QUANTIDADE_DE_ESTADOS; i++)
		for (int j = 0; j < TOTAL_CLASSES_CARACTERES; j++)
			tabela_Transicoes[i][j] = aux[i][j];
	
	return ;
}


char leia_Proximo_Caractere(void){
	char prox_Simb = getc(arquivo_de_entrada);
	if (prox_Simb != EOF) //EOF não conta como coluna
		coluna_arquivo++;
	if(prox_Simb == '\n'){ // Se o caracter for uma quebra de linha: Novos valores para linha e coluna
		linha_arquivo++;
		coluna_arquivo = 0;
	}
	return prox_Simb;
}


void retrocede_Caracteres(const int n, const char prox_Simb){
	fseek(arquivo_de_entrada, -n*sizeof(char), SEEK_CUR);
	coluna_arquivo --; 
	if (prox_Simb == '\n')
		linha_arquivo--; //Isso nao influencia na coluna, uma vez que o prox a ser lido será '\n, o valor da coluna é zerado
}


tClasse_caractere carctere_2_tClasse_caractere(char prox_Simb){
	if (isalpha(prox_Simb))
		return tc_letra;
	if (isdigit(prox_Simb))
		return tc_digito;
	switch (prox_Simb){
		case '\t':  case ' ': return tc_branco;
		case '\n': return tc_quebra_linha;
		case '_': return tc_underline;
		case '\"': return tc_aspas;
		case '.': return tc_ponto;
		case ',': return tc_virgula;
		case ';': return tc_ponto_virgula;
		case ':': return tc_dois_pontos;
		case '(': return tc_abre_parenteses;
		case ')': return tc_fecha_parenteses;
		case '<': return tc_menor;
		case '=': return tc_igual;
		case '>': return tc_maior;
		case '+': return tc_mais;
		case '-': return tc_menos;
		case '*': return tc_vezes;
		case '/': return tc_dividido;
		case EOF: return tc_EOF;
		default: return tc_outro;
	}	
}


void iniciar_Lexema(void){
	lexema.limite_string = LIMITE_INICIAL_DE_ALOCACAO;
	lexema.tamanho_string = 0;
	lexema.string = (char*) malloc(lexema.limite_string * sizeof(char));
	if (lexema.string == NULL){
        printf("Erro durante a alocacao!!! \nInfelizmente o programa travou\n");
        exit(-1);
    }
    lexema.string[0] = '\0';
}


void realocar_Lexema(void){
	lexema.limite_string *= 2;
	lexema.string = (char*) realloc (lexema.string, lexema.limite_string * sizeof(char));
	if (lexema.string == NULL){
        printf("Erro durante a alocacao!!! \nInfelizmente o programa travou\n");
        exit(-1);
    }
}


void reiniciar_Lexema(void){
	lexema.tamanho_string = 0;
    lexema.string[0] = '\0';
}


void inserir_Caractere_No_Lexema(char prox_Simb){
	if (lexema.tamanho_string == (lexema.limite_string - 2))
		realocar_Lexema();
	lexema.string[lexema.tamanho_string] = prox_Simb;
	lexema.tamanho_string++;
	lexema.string[lexema.tamanho_string] = '\0';
}

