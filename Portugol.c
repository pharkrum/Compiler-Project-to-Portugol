#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#include "Codigo/tokens.h"
//#include "Codigo/automato.h"
//#include "Codigo/erros.h"
//#include "Codigo/tabSimbolos.h"
//#include "Codigo/resultados.h"


#define TAMANHO_INICIAL 256
#define QUANTIDADE_DE_TOKENS 41
#define TAMANHO_DO_MAIOR_TOKEN 15
#define QUANTIDADE_DE_ESTADOS 45

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


///PROTOTIPOS
tToken analizador_Lexico(void);
void inicia_Tabela_Transicoes (void);
unsigned char leia_Proximo_Caractere(void);
void transicao_digitos(const int, const int);
void transicao_letras(const int, const int);
void transicao_branco(const int, const int);
void retrocede_Caracteres(const int, const char);
//inserir_Caractere_No_Lexema();
//identificar_Token();
//setar_Erro();
//retrocede_Ate();



///VARIAVEIS
FILE *arquivo_de_entrada;
int tabela_Transicoes[QUANTIDADE_DE_ESTADOS][TAMANHO_INICIAL];
int linha_arquivo = 1, coluna_arquivo = 0, linha_token, coluna_token;



///FUNCAO PRINCIPAL
int main (int argc, char *argv[]){
	tToken token_da_vez;
	//int n = 0;
	
	if (argc != 2){
		printf("Exemplo de execucao: ./Portugol prog01.ptg\nTente novamente\n");
		exit(1);
	}
	
	printf("Arquivo : %s \n\n", argv[1]);

	if ((arquivo_de_entrada = fopen(argv[1], "r")) == NULL){
		printf("Erro ao abrir o arquivo!!! \nPor favor, verifique a existencia do mesmo e tente novamente.\n");
		exit(1);
	}
	
	inicia_Tabela_Transicoes();
	do {
		token_da_vez = analizador_Lexico();
		//n++;
		//printf("%s\n",obter_Nome_Do_Token(token_da_vez));
	} while(token_da_vez != tk_EOF);

	
	//printar_Lista_De_Erros_Lexicos(argv[1]);
	//printar_Lista_De_Tokens_Reconhecidos(argv[1], token_da_vez, n);
	//printar_Resumo();
	//printar_Tabela_De_Simbolos(argv[1]);
	
	printf("\nAnalise concluida com sucesso, os seguintes arquivos gerados:\n");
	printf("   %s.err com o conteúdo do arquivo de entrada e os erros léxicos devidamente marcados\n", argv[1]);
	printf("   %s.tbl com a lista de tokens reconhecidos\n", argv[1]);
	printf("   %s.tok com o conteúdo da tabela de símbolos após processamento.\n\n", argv[1]);
	
	fclose(arquivo_de_entrada);
	
	return 0;
}






tToken analizador_Lexico(void){
	int estado = 0, id_token, contador_de_bloco;
	char prox_Simb;
	
	//reiniciar_Automato(); ???
	while (1){
		switch (estado){
			case 0: ///Estado Inicial
				linha_token = linha_arquivo;
				coluna_token = coluna_arquivo;
				break;
				
			case 1: ///Estado identificador ou palavra reservada
				//inserir_Caractere_No_Lexema(prox_Simb);
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
				//inserir_Caractere_No_Lexema(prox_Simb);
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
				//inserir_Caractere_No_Lexema(prox_Simb);
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
				//inserir_Caractere_No_Lexema(prox_Simb);
				break;
				
			case 10: ///Estado de erro lexico Ponto Isolado (FINAL)
				retrocede_Caracteres(2, prox_Simb); //Para ser lido novamente e mudar para q0 (para todo char lido)
				//setar_Erro("Ponto isolado");
				break;

			case 11: ///Estado Cadeia
				//inserir_Caractere_No_Lexema(prox_Simb);
				break;

			case 12: ///Estado Cadeia (FINAL)
				//inserir_Caractere_No_Lexema(prox_Simb);
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
				//Verificar se o retrocede volta certo ?????
				//setar_Erro("Comentario de bloco não fechado");
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
		estado = tabela_Transicoes[estado][(unsigned char)prox_Simb];
	}
	return 0;
}



void inicia_Tabela_Transicoes (void) {
	/*	tabela_Transicoes[QUANTIDADE_DE_ESTADOS][TAMANHO_INICIAL]
		transicao_digitos(estado_atual, estado_destino);
		transicao_letras(estado_atual, estado_destino);
		EOF (unsigned = 255)
	*/
	
	int i;
	
	//Setar padrao para o estado de transicao Defalt
	for (int i=0; i<QUANTIDADE_DE_ESTADOS; i++){
		for (int j=0; j<TAMANHO_INICIAL; j++){
			tabela_Transicoes[i][j] = 44;
		}
	}
	
	//Estado 0
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[0][i] = 44;
	transicao_branco(0, 0);
	transicao_letras(0, 1);
	transicao_digitos(0, 3);
	tabela_Transicoes[0]['.'] = 9;
	tabela_Transicoes[0]['\"'] = 11;
	tabela_Transicoes[0][255] = 14; //EOF
	tabela_Transicoes[0][','] = 15;
	tabela_Transicoes[0][';'] = 16;
	tabela_Transicoes[0][':'] = 17;
	tabela_Transicoes[0]['('] = 18;
	tabela_Transicoes[0][')'] = 24;
	tabela_Transicoes[0]['<'] = 25;
	tabela_Transicoes[0]['='] = 30;
	tabela_Transicoes[0]['>'] = 31;
	tabela_Transicoes[0]['+'] = 34;
	tabela_Transicoes[0]['-'] = 37;
	tabela_Transicoes[0]['*'] = 40;
	tabela_Transicoes[0]['/'] = 41;
    
    
    //Estado 1
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[1][i] = 2;
	transicao_letras(1, 1);
	transicao_digitos(1, 1);
	tabela_Transicoes[1]['_'] = 1;
	
	//Estado 2
	//NULL
	
	//Estado 3
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[3][i] = 4;
	transicao_letras(3, 5);
	transicao_digitos(3, 3);
	tabela_Transicoes[3]['.'] = 6;
	
	//Estado 4 e 5
	//NULL
	
	//Estado 6
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[6][i] = 7;
	transicao_letras(6, 8);
	transicao_digitos(6, 6);
	
	
	//Estado 7 e 8
	//NULL
	
	//Estado 10
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[10][i] = 0;
		
	//Estado 11
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[11][i] = 11;
	tabela_Transicoes[11]['\"'] = 12;
	tabela_Transicoes[11][255] = 13; //EOF
	tabela_Transicoes[11]['\n'] = 13;
	
	
	//Estado 12, 13, 14, 15, 16 e 17
	//NULL
	
	//Estado 18
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[18][i] = 20;
	tabela_Transicoes[18]['*'] = 19;
	
	//Estado 19
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[19][i] = 19;
	tabela_Transicoes[19]['*'] = 21;
	tabela_Transicoes[19][255] = 23; //EOF
	
	//Estado 20
	//NULL
	
	//Estado 21
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[21][i] = 19;
	tabela_Transicoes[21]['*'] = 21;
	tabela_Transicoes[21][')'] = 22;
	tabela_Transicoes[21][255] = 23; //EOF
	
	//Estado 22
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[22][i] = 0;
	
	//Estado 23
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[23][i] = 42;
	
	//Estado 24
	//NULL
	
	//Estado 25
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[25][i] = 29;
	tabela_Transicoes[25]['-'] = 26;
	tabela_Transicoes[25]['>'] = 27;
	tabela_Transicoes[25]['='] = 28;
	
	//Estado 26, 27, 28, 29, 30
	//NULL
	
	
	//Estado 31
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[31][i] = 33;
	tabela_Transicoes[31]['='] = 32;
	
	//Estado 32, 33
	//NULL
	
	//Estado 34
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[34][i] = 36;
	tabela_Transicoes[34]['+'] = 35;
	
	//Estado 35 e 36
	//NULL
		
	//Estado 37
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[37][i] = 39;
	tabela_Transicoes[37]['-'] = 38;
	
	//Estado 38, 39, 40
	//NULL
		
	//Estado 41
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[41][i] = 43;
	tabela_Transicoes[41]['*'] = 42;
	
	//Estado 42
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[42][i] = 42;
	tabela_Transicoes[42]['\n'] = 0;
	tabela_Transicoes[42][255] = 14;
	
	//Estado 43
	//NULL

	//Estado 44 (Default)
	for (i=0; i<TAMANHO_INICIAL; i++)
		tabela_Transicoes[44][i] = 0;
	
    return;
}

unsigned char leia_Proximo_Caractere(void){
	unsigned char prox_Simb = getc(arquivo_de_entrada);
	if (prox_Simb != EOF) //EOF não conta como coluna
		coluna_arquivo++;
	if(prox_Simb == '\n'){ // Se o caracter for uma quebra de linha: Novos valores para linha e coluna
		linha_arquivo++;
		coluna_arquivo = 0;
	}
	return prox_Simb;
}

void transicao_digitos(const int estado_atual, const int estado_destino){
	int i;
	for (i='0'; i<='9'; i++)
		tabela_Transicoes[estado_atual][i] = estado_destino;
}

void transicao_letras(const int estado_atual, const int estado_destino){
	int i;
	for (i='a'; i<='z'; i++)
		tabela_Transicoes[estado_atual][i] = estado_destino;
	for (i='A'; i<='Z'; i++)
		tabela_Transicoes[estado_atual][i] = estado_destino;
}

void transicao_branco(const int estado_atual, const int estado_destino){
	tabela_Transicoes[estado_atual][' '] = estado_destino;
	tabela_Transicoes[estado_atual]['\t'] = estado_destino;
	tabela_Transicoes[estado_atual]['\n'] = estado_destino;
	tabela_Transicoes[estado_atual]['\v'] = estado_destino;
	tabela_Transicoes[estado_atual]['\f'] = estado_destino;
	tabela_Transicoes[estado_atual]['\r'] = estado_destino;
}

void retrocede_Caracteres(const int n, const char prox_Simb){
	fseek(arquivo_de_entrada, -n*sizeof(char), SEEK_CUR);
	coluna_arquivo --; 
	if (prox_Simb == '\n')
		linha_arquivo--; //Isso nao influencia na coluna, uma vez que o prox a ser lido será '\n, o valor da coluna é zerado
}



