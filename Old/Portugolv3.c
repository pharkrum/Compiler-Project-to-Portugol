//Automato desenho: erro no estado ) -> Branco e \n
//					erro no estado 23 -> 42 e nao para o estado 0

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
#define QUANTIDADE_DE_ESTADOS 45
#define QUANTIDADE_DE_TOKENS 41
#define LIMITE_INICIAL_DE_ALOCACAO 30
#define TAM_TAB_HASH_SIMBOLOS 139
#define hash(v) ((2*v)+3)%139 //Multiplique, Adicione e Divida (MAD)



///Definicao de Tipos (ENUM) - Tokens, Classe de carcteres, Erros
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
	er_delimitador_esperado,
	er_ponto_isolado,
	er_cadeia_nao_fechada,
	er_comentario_de_bloco_nao_fechado,
	er_caracter_invalido
} tErro;



///Definicao de Estruturas - String
typedef struct{
	int tamanho_string;
	int limite_string;
	char * string;
} tSring;



///Tabela Hash + Definicao dos Atributos da tabela de simbolos
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


///Mecanismo que conecta, para cada par token-lexema, a ordem em que ele ocorre na entrada e sua posição na tabela de símbolos
typedef struct {
	int tab_simb_count;
	int limite_tab_simb_count;
	tSimbolo** ordem_de_entrada_da_tab_simbolos;
} tOrdem;


///Definicao de Estruturas - Identificador de Erros Lexicos, Lista de erros lexicos, Identificador de token, Lista de tokens
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



///PROTOTIPOS
tToken analizador_Lexico(void);
void iniciar_Tabela_Transicoes (void);
char ler_Proximo_Caractere(void);
void retroceder_Caracteres(const int, const char);
tClasse_caractere carctere_2_tClasse_caractere(const char);
void iniciar_Lexema(void);
void realocar_Lexema(void);
void reiniciar_Lexema(void);
void inserir_Caractere_No_Lexema(const char);
int identificar_Token(void);
void retroceder_Ate(const int, const int, const int);
void iniciar_Lista_De_Erros(void);
void iniciar_Lista_De_Tokens(void);
void adicionar_Erro_Na_Lista_De_Erros(const tErro, const char, const int, const int);
void adicionar_Token_Na_Lista_De_Tokens(const tToken, const int, const int);
const char * obter_Nome_Do_Token(const tToken);
const char * obter_Nome_Do_Erro(const tErro);
int imprimir_Linha(FILE *);
void imprimir_Seta(FILE *, const int);
void imprimir_Lista_De_Erros_Lexicos(const char*);
void iniciar_Tabela_de_Simbolos(void);
void liberar_Tabela_Simbolos(void);
void iniciar_Ordem_Tab(void);
int hash_Com_Shift(void);
void adiconar_Na_Tabela_De_Simbolos(const tToken);
tSimbolo * buscar_Na_Tabela_De_Simbolos(const tToken, const int);
void adiciona_Ocorrencia(tSimbolo *);
void imprimir_Lista_De_Tokens_Reconhecidos_E_Resumo(const char*);
int obter_Tamanho_Do_Maior_Nome_Token_Reconhecido(void);
int obter_Tamanho_Do_Maior_Lexema(void);
int obter_Tamanho_Do_Maior_Nome_Token_Com_Lexema(void);
int obter_Tamanho_Da_Maior_Qtd_De_Ocorrencias(void);
void imprimir_Tabela_De_Simbolos(const char*);



///VARIAVEIS
FILE *arquivo_de_entrada;
char tabela_Transicoes[QUANTIDADE_DE_ESTADOS][TOTAL_CLASSES_CARACTERES];
int linha_arquivo = 1, coluna_arquivo = 1, linha_token, coluna_token;
tSring lexema;
tLista_de_erros lista_de_erros;
tLista_de_tokens lista_de_tokens;
tSimbolo ** tab_simbolos;
tOrdem ordem_de_entrada;


///FUNCAO PRINCIPAL
int main (int argc, char *argv[]){
	tToken token_da_vez;
	char * caminho;
	
	if (argc < 2){
		printf("Exemplo de execucao: ./Portugol prog01.ptg\nTente novamente\n\n");
	} else {
		iniciar_Tabela_Transicoes();
		iniciar_Lexema();
		
		/// Roda os arquivos que digitei na entrada
		for (int i=1; i < argc; i++){ 
			printf("\nArquivo : %s \n", argv[i]);
			caminho = (char *) malloc(10 + strlen(argv[i])); /// Alocação para o caminho do arquivo
			sprintf(caminho,"./Testes/%s", argv[i]); /// O arquivo está na pasta testes
			
			if ((arquivo_de_entrada = fopen(caminho, "r")) == NULL){
				printf("Erro ao abrir o arquivo!!! \nPor favor, verifique a existencia do mesmo na pasta 'Testes' e tente novamente.\n");
			} else {
				///Iniciando listas de tokens e erros
				iniciar_Lista_De_Erros();
				iniciar_Lista_De_Tokens();
				iniciar_Tabela_de_Simbolos();
				iniciar_Ordem_Tab();
				
				///Recebendo tokens
				do {
					token_da_vez = analizador_Lexico();
					adicionar_Token_Na_Lista_De_Tokens(token_da_vez, linha_token, coluna_token);
					//printf("%3d | %3d | %3d | %-15s | %s\n", linha_token, coluna_token, token_da_vez+1, obter_Nome_Do_Token(token_da_vez), lexema.string);
				} while(token_da_vez != tk_EOF);

				
				imprimir_Lista_De_Erros_Lexicos(argv[i]);
				imprimir_Lista_De_Tokens_Reconhecidos_E_Resumo(argv[i]);
				imprimir_Tabela_De_Simbolos(argv[i]);
				
				printf("Os seguintes arquivos gerados:\n");
				printf("   %s.err com o conteúdo do arquivo de entrada e os erros léxicos devidamente marcados\n", argv[1]);
				printf("   %s.tbl com a lista de tokens reconhecidos\n", argv[1]);
				printf("   %s.tok com o conteúdo da tabela de símbolos após processamento.\n\n", argv[1]);
				
				///Liberando Memoria
				free(caminho);
				free(lista_de_erros.id_erro);
				free(lista_de_tokens.id_token);
				free(ordem_de_entrada.ordem_de_entrada_da_tab_simbolos);
				liberar_Tabela_Simbolos();
				
				///Fechando Arquivo de entrada
				fclose(arquivo_de_entrada);
			}
		}
		///Free tString lexema
		free(lexema.string);
	}
	return 0;
}






tToken analizador_Lexico(void){
	int estado = 0, id_token, contador_de_bloco = 0;
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
				retroceder_Caracteres(1, prox_Simb);
				id_token = identificar_Token();
				if (id_token == -1){ //Identificador
					adiconar_Na_Tabela_De_Simbolos(tk_IDEN);
					return (tk_IDEN);
				} else { //Palavra reservada
					return (id_token);
				}
				break;

			case 3: /// Estado Digito (inteiro ou decimal)
				inserir_Caractere_No_Lexema(prox_Simb);
				break;

			case 4: /// Estado Digito Inteiro (FINAL)
				retroceder_Caracteres(1, prox_Simb);
				adiconar_Na_Tabela_De_Simbolos(tk_INTEIRO);
				return (tk_INTEIRO);
				break;
				
			case 5: ///Estado de erro lexico apos inteiro (FINAL)
				retroceder_Caracteres(1, prox_Simb);
				adicionar_Erro_Na_Lista_De_Erros(er_delimitador_esperado, prox_Simb, linha_arquivo, coluna_arquivo);
				adiconar_Na_Tabela_De_Simbolos(tk_INTEIRO);
				return (tk_INTEIRO);
				break;
				
			case 6: ///Estado Digito Decimal apos o ponto
				inserir_Caractere_No_Lexema(prox_Simb);
				break;
				
			case 7: /// Estado Digito Decimal (FINAL)
				retroceder_Caracteres(1, prox_Simb);
				adiconar_Na_Tabela_De_Simbolos(tk_DECIMAL);
				return (tk_DECIMAL);
				break;

			case 8: ///Estado de erro lexico apos decimal (FINAL)
				retroceder_Caracteres(1, prox_Simb);
				adicionar_Erro_Na_Lista_De_Erros(er_delimitador_esperado, prox_Simb, linha_arquivo, coluna_arquivo);
				adiconar_Na_Tabela_De_Simbolos(tk_DECIMAL);
				return (tk_DECIMAL);
				break;
			
			case 9: /// Estado Digito Decimal Iniciando com ponto
				inserir_Caractere_No_Lexema(prox_Simb);
				break;
				
			case 10: ///Estado de erro lexico Ponto Isolado (FINAL)
				reiniciar_Lexema();
				retroceder_Caracteres(2, prox_Simb); //Para ser lido novamente e mudar para o estado 0
				adicionar_Erro_Na_Lista_De_Erros(er_ponto_isolado, prox_Simb, linha_arquivo, coluna_arquivo);
				break;

			case 11: ///Estado Cadeia
				inserir_Caractere_No_Lexema(prox_Simb);
				break;

			case 12: ///Estado Cadeia (FINAL)
				inserir_Caractere_No_Lexema(prox_Simb);
				adiconar_Na_Tabela_De_Simbolos(tk_CADEIA);
				return (tk_CADEIA);
				break;
			
			case 13: ///Estado de erro lexico nao fechameno da cadeia (FINAL)
				retroceder_Caracteres(1, prox_Simb);
				adicionar_Erro_Na_Lista_De_Erros(er_cadeia_nao_fechada, prox_Simb, linha_arquivo, coluna_arquivo);
				adiconar_Na_Tabela_De_Simbolos(tk_CADEIA);
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
				retroceder_Caracteres(1, prox_Simb);
				return (tk_abre_par);
				break;
				
			case 21: ///Possivel fim de comentario de bloco
				contador_de_bloco++;
				break;

			case 22: ///Fim do comentario de bloco
				retroceder_Caracteres(1, prox_Simb); //Para ser lido novamente e mudar para o estado 0
				break;

			case 23: ///Estado de erro lexico Comentario de bloco não fechado
				retroceder_Ate(contador_de_bloco, linha_token, coluna_token); //Muitos para serem lidos novamenteem outro estado
				adicionar_Erro_Na_Lista_De_Erros(er_comentario_de_bloco_nao_fechado, prox_Simb, linha_arquivo, coluna_arquivo);
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
				retroceder_Caracteres(1, prox_Simb);
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
				retroceder_Caracteres(1, prox_Simb);
				return (tk_maior);
				break;

			case 34: ///Estado +
				break;

			case 35: ///Estado Incremento (FINAL)
				return (tk_incr);
				break;

			case 36: ///Estado Mais (FINAL)
				retroceder_Caracteres(1, prox_Simb);
				return (tk_mais);
				break;

			case 37: ///Estado -
				break;

			case 38: ///Estado Decremento (FINAL)
				return (tk_decr);
				break;

			case 39: ///Estado Menos (FINAL)
				retroceder_Caracteres(1, prox_Simb);
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
				retroceder_Caracteres(1, prox_Simb);
				return (tk_dividido);
				break;

			default: ///Estado Defauult: Apresnta erro e ignora o caractere
				retroceder_Caracteres(1, prox_Simb); //Para ser lido novamente e mudar para o estado 0
				adicionar_Erro_Na_Lista_De_Erros(er_caracter_invalido, prox_Simb, linha_arquivo, coluna_arquivo);
				break;
		}
		prox_Simb = ler_Proximo_Caractere();
		estado = tabela_Transicoes[estado][carctere_2_tClasse_caractere(prox_Simb)];
	}
	return 0;
}


void iniciar_Tabela_Transicoes (void){
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
		{10, 10, 10, 6, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},  //Estado 9
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
	
	/*printf("      b  \\n  l  d  _  \"  .  ,  ;  :  (  )  <  =  >  +  -  *  /  e  ot\n");
	for (int i = 0; i < QUANTIDADE_DE_ESTADOS; i++){
		printf("%3d |", i);
		for (int j = 0; j < TOTAL_CLASSES_CARACTERES; j++){
			printf ("%3d", tabela_Transicoes[i][j]);
		}
		printf("\n");
	}*/
}


char ler_Proximo_Caractere(void){
	char prox_Simb = getc(arquivo_de_entrada); //Obtem caractere do arquivo
	coluna_arquivo++; //Nova linha
	if(prox_Simb == '\n'){ // Se o caracter for uma quebra de linha: Novos valores para linha e coluna
		linha_arquivo++;
		coluna_arquivo = 1;
	}
	return prox_Simb;
}


void retroceder_Caracteres(const int n, const char prox_Simb){
	fseek(arquivo_de_entrada, -n*sizeof(char), SEEK_CUR); //Retocede n caracteres
	coluna_arquivo -= n; //Diminui o numero de caracteres da coluna
	if (prox_Simb == '\n') //Isso nao influencia na coluna, uma vez que o prox a ser lido será '\n, o valor da coluna é zerado
		linha_arquivo--;
	if (prox_Simb == EOF) //Tratamento de EOF (que nao é considerado no fseek)
		fseek(arquivo_de_entrada, 1, SEEK_CUR);
}


tClasse_caractere carctere_2_tClasse_caractere(const char prox_Simb){
	//Funcao Responsavel pr converter o carctere lido em uma classe de simbolos (indice) da tabela de transissoes
	if (isalpha(prox_Simb))	return tc_letra;
	if (isdigit(prox_Simb))	return tc_digito;
	if (prox_Simb == '\n')	return tc_quebra_linha;
	if (isspace(prox_Simb))	return tc_branco;
	switch (prox_Simb){
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


int identificar_Token(void){
	//Ao encontrar uma palavra qualquer, verifique se é palavra reservada com uma função que retorna o código da palavra reservada
	//ou –1 se o identificador não for palavra reservada.
	//Tal função deve conhecer as palavras reservadas da linguagem.
	const char * palavras_reservadas[] = {"inicio", "fim", "int", "dec", "leia", "imprima", "para", "de", "ate",
										  "passo", "fim_para", "se", "entao", "senao", "fim_se", "e", "ou", "nao"};
										  
	char * aux = (char *) malloc(lexema.tamanho_string * sizeof(char));
	strcpy(aux, lexema.string); //Auxiliar, Letras maiúsculas e minúsculas são distinguidas em nomes de identificadoresm, mas, é nescessario observar o que vem a seguir:

	for(int i = 0; i < strlen(aux); i++) // Torna todas as letras minusculas Letras maiúsculas e minúsculas nao são distinguidas em palavras reservadas
		aux[i] = tolower(aux[i]);
		
	for(int i = 0; i < 18; i++) { //18 = numero de palavras_reservadas
		if(strcmp(aux, palavras_reservadas[i]) == 0)
			return (i + tk_inicio);
	}

	return -1;
}


void retroceder_Ate(const int n, const int linha, const int coluna){
	//Similar a funcao retroceder_Caracteres, mas lida com um alcance muito maior para o valor de n
	fseek(arquivo_de_entrada, (-1)*n*sizeof(char), SEEK_CUR);
	linha_arquivo = linha;
	coluna_arquivo = coluna;
}


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
	//Funcao auto explicativa
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
	if(simb->tamanho_ocorrencias == simb->limite_ocorrencias-1){
		simb->limite_ocorrencias *= 2;
		simb->ocorrencias = (tPos*) realloc (simb->ocorrencias, simb->limite_ocorrencias * sizeof(tPos));
		if (simb->ocorrencias == NULL){
			printf("Erro durante a realocacao da lista de ocorrencias!!! \nInfelizmente o programa travou\n");
			exit(-1);
		}
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


int imprimir_Linha(FILE * arquivo_de_saida){
	//Usada durante a impressao de erros lexicos para a entrada portugol
	//Essa funcao é responsavel por imprimir em arquivo uma linha do aquivo texto
	int carcteres_na_linha = 0;
	char prox_Simb = ' ';
	while (prox_Simb != '\n' && prox_Simb != EOF){
		fprintf(arquivo_de_saida, "%c", prox_Simb);
		prox_Simb = ler_Proximo_Caractere();
		carcteres_na_linha++;
	}
	fprintf(arquivo_de_saida, "\n");
	return carcteres_na_linha;
}


void imprimir_Seta(FILE * arquivo_de_saida, const int n){
	//Essa funcao é responsavel por imprimir em arquivo uma seta indicando um erro lexico em uma determinada linha
	fprintf(arquivo_de_saida, "       ");
	for (int i=0; i<n-1; i++)
		fprintf(arquivo_de_saida, "-");
	fprintf(arquivo_de_saida, "^\n");
}


void imprimir_Lista_De_Erros_Lexicos(const char* nomeArquivoEntrada){
	int carcteres_na_linha = -1, i = 0; //Variaveis
	FILE *arquivo_de_saida;
	char * nome_arquivo;
	
	rewind(arquivo_de_entrada); //Voltar ao inico do arquivo
	linha_arquivo = coluna_arquivo = 1; //Zerar linha e coluna
	
	nome_arquivo = (char *) malloc(4 + strlen(nomeArquivoEntrada)); // Alocação para o nome do arquivo
	sprintf(nome_arquivo,"%s.err", nomeArquivoEntrada); // O arquivo esta na pasta testes
	
	if ((arquivo_de_saida = fopen(nome_arquivo, "w")) == NULL){
		//Erro ao abrir o arquivo
		printf("Erro ao abrir o arquivo para a saida de erros lexicos!!!\n");
	} else {
		//Inserindo dados no arquivo
		fprintf(arquivo_de_saida, "LISTA DE ERROS LEXICOS EM \"%s\" \n\n", nomeArquivoEntrada);
		while(!feof(arquivo_de_entrada)){
			fprintf(arquivo_de_saida, "[%4d]", linha_arquivo);
			carcteres_na_linha = imprimir_Linha(arquivo_de_saida);
			if (lista_de_erros.id_erro[i].LIN == linha_arquivo-1){
				imprimir_Seta(arquivo_de_saida, lista_de_erros.id_erro[i].COL);
				if (lista_de_erros.id_erro[i].ERRO == er_caracter_invalido)
					fprintf(arquivo_de_saida, "       Erro lexico na linha %d coluna %d: %s '%c'\n", lista_de_erros.id_erro[i].LIN, lista_de_erros.id_erro[i].COL, obter_Nome_Do_Erro(lista_de_erros.id_erro[i].ERRO), lista_de_erros.id_erro[i].CARACTER);
				else
					fprintf(arquivo_de_saida, "       Erro lexico na linha %d coluna %d: %s \n", lista_de_erros.id_erro[i].LIN, lista_de_erros.id_erro[i].COL, obter_Nome_Do_Erro(lista_de_erros.id_erro[i].ERRO));
				i++;
				if (lista_de_erros.id_erro[i].LIN == linha_arquivo-1)
					retroceder_Caracteres(carcteres_na_linha, '\n');
			}
		}
		
		fprintf(arquivo_de_saida, "\nTOTAL DE ERROS: %d\n\n", lista_de_erros.tamanho_lista);
		//Fechando arquivo de saida e liberando memoria alocada
		fclose(arquivo_de_saida);
		free(nome_arquivo);
	}
}


void imprimir_Lista_De_Tokens_Reconhecidos_E_Resumo(const char* nomeArquivoEntrada){
	int resumo[QUANTIDADE_DE_TOKENS] = {0};
	int token_max = obter_Tamanho_Do_Maior_Nome_Token_Reconhecido();
	int lexema_max = obter_Tamanho_Do_Maior_Lexema();
	int linha_Da_Vez = 0;
	
	FILE *arquivo_de_saida;
	char * nome_arquivo;
	
	nome_arquivo = (char *) malloc(4 + strlen(nomeArquivoEntrada)); // Alocação para o nome do arquivo
	sprintf(nome_arquivo,"%s.tbl", nomeArquivoEntrada); // O arquivo esta na pasta testes
	
	if ((arquivo_de_saida = fopen(nome_arquivo, "w")) == NULL){
		//Erro ao abrir o arquivo
		printf("Erro ao abrir o arquivo para a saida de erros lexicos!!!\n");
	} else {
		fprintf(arquivo_de_saida,"LISTA DE TOKENS RECONHECIDOS EM \"%s\" \n\n", nomeArquivoEntrada);
		
		fprintf(arquivo_de_saida,"+-----+-----+-----+-");
		for (int i=0; i<token_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+-");
		for (int i=0; i<lexema_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+--------------+\n");
		
		fprintf(arquivo_de_saida,"| LIN | COL | COD | %-*s | %-*s | POS TAB SIMB |\n", token_max, "TOKEN", lexema_max, "LEXEMA");
		
		fprintf(arquivo_de_saida,"+-----+-----+-----+-");
		for (int i=0; i<token_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+-");
		for (int i=0; i<lexema_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+--------------+\n");
		
		
		for (int i=0; i<lista_de_tokens.tamanho_lista; i++){
			resumo[lista_de_tokens.id_token[i].TOKEN]++;
			
			if (lista_de_tokens.id_token[i].LIN == linha_Da_Vez){
				fprintf(arquivo_de_saida,"|     | ");
			} else{
				fprintf(arquivo_de_saida,"| %3d | ", lista_de_tokens.id_token[i].LIN);
			}
			
			fprintf(arquivo_de_saida,"%3d | %3d | %-*s | ", lista_de_tokens.id_token[i].COL, lista_de_tokens.id_token[i].TOKEN+1, token_max, obter_Nome_Do_Token(lista_de_tokens.id_token[i].TOKEN));
			
			if (lista_de_tokens.id_token[i].posisao_na_tabela_de_simbolos == -1){
				fprintf(arquivo_de_saida,"%*s |              |\n", lexema_max, " ");
			} else{
				//ERRO
				fprintf(arquivo_de_saida,"%*s |     %3d      |\n", lexema_max, " ", lista_de_tokens.id_token[i].posisao_na_tabela_de_simbolos);
			}
			
			if (lista_de_tokens.id_token[i].LIN != linha_Da_Vez)
				linha_Da_Vez = lista_de_tokens.id_token[i].LIN;
		}
		
		fprintf(arquivo_de_saida,"+-----+-----+-----+-");
		for (int i=0; i<token_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+-");
		for (int i=0; i<lexema_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+--------------+\n\n");
		
		
		
		int tot = 0;
		fprintf(arquivo_de_saida,"RESUMO \n\n");
		fprintf(arquivo_de_saida,"+-----+----------------+------+\n");
		fprintf(arquivo_de_saida,"| COD | TOKEN          | USOS |\n");
		fprintf(arquivo_de_saida,"+-----+----------------+------+\n");
		for (int i=0; i<QUANTIDADE_DE_TOKENS; i++){
			fprintf(arquivo_de_saida,"| %3d | %-14s | %4d |\n", i+1, obter_Nome_Do_Token(i), resumo[i]);
			tot += resumo[i];
		}
		fprintf(arquivo_de_saida,"+-----+----------------+------+\n");
		fprintf(arquivo_de_saida,"|   0 | TOTAL          | %4d |\n", tot);
		fprintf(arquivo_de_saida,"+-----+----------------+------+\n\n");
		fprintf(arquivo_de_saida,"TOTAL DE ERROS: %d\n\n", lista_de_erros.tamanho_lista);
		
		//Fechando arquivo de saida e liberando memoria alocada
		fclose(arquivo_de_saida);
		free(nome_arquivo);
	}
}


int obter_Tamanho_Do_Maior_Nome_Token_Reconhecido(void){
	int tamanho_token[] = {6, 7, 10, 10, 9, 9, 6, 6, 6, 7, 10, 7, 5, 6, 8, 11, 5, 8, 8, 9, 4, 5, 6, 7, 10, 11, 11, 12, 8, 14, 8, 14, 12, 8, 7, 7, 8, 7, 8, 8, 11};
	int maior = 5; //Tamanho da palavra TOKEN
	
	for (int i=0; i < lista_de_tokens.tamanho_lista; i++){
		if (tamanho_token[lista_de_tokens.id_token[i].TOKEN] > maior)
			maior = tamanho_token[lista_de_tokens.id_token[i].TOKEN];
	}
	return maior;
}


int obter_Tamanho_Do_Maior_Lexema(void){
	int maior = 6; //Tamanho da palavra LEXEMA
	for (int i=0; i < ordem_de_entrada.tab_simb_count; i++){
		int aux = strlen(ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->lexema_cadeia);
		if (aux > maior)
			maior = aux;
	}
	return maior;
}


int obter_Tamanho_Do_Maior_Nome_Token_Com_Lexema(void){
	int tamanho_token[] = {0, 7, 10, 10, 9};
	int maior = 5; //Tamanho da palavra TOKEN
	for (int i=0; i < ordem_de_entrada.tab_simb_count; i++){
		if (tamanho_token[ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->COD] > maior)
			maior = tamanho_token[ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->COD];
	}
	return maior;
}


int obter_Tamanho_Da_Maior_Qtd_De_Ocorrencias(void){
	int maior = 3; //Tamanho de "POS NA ENTRADA (linha,coluna)" /10
	for (int i=0; i < ordem_de_entrada.tab_simb_count; i++){
		if (ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->tamanho_ocorrencias > maior)
			maior = ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->tamanho_ocorrencias;
	}
	return maior;
}


void imprimir_Tabela_De_Simbolos(const char* nomeArquivoEntrada){
	int token_max = obter_Tamanho_Do_Maior_Nome_Token_Com_Lexema();
	int lexema_max = obter_Tamanho_Do_Maior_Lexema();
	int maior_numero_ocorrencias = obter_Tamanho_Da_Maior_Qtd_De_Ocorrencias() * 10;
	FILE *arquivo_de_saida;
	char * nome_arquivo;
	
	nome_arquivo = (char *) malloc(4 + strlen(nomeArquivoEntrada)); // Alocação para o nome do arquivo
	sprintf(nome_arquivo,"%s.tok", nomeArquivoEntrada); // O arquivo esta na pasta testes
	
	if ((arquivo_de_saida = fopen(nome_arquivo, "w")) == NULL){
		//Erro ao abrir o arquivo
		printf("Erro ao abrir o arquivo para a saida de erros lexicos!!!\n");
	} else {
		fprintf(arquivo_de_saida,"TABELA DE SIMBOLOS - \"%s\" \n\n", nomeArquivoEntrada);
		fprintf(arquivo_de_saida,"+-----+-");
		for (int i=0; i<token_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+-");
		for (int i=0; i<lexema_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+-");
		for (int i=0; i<maior_numero_ocorrencias; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"+\n");
		
		fprintf(arquivo_de_saida,"| POS | %-*s | %-*s | %-*s|\n", token_max, "TOKEN", lexema_max, "LEXEMA", maior_numero_ocorrencias, "POS NA ENTRADA (linha,coluna)");
		
		fprintf(arquivo_de_saida,"+-----+-");
		for (int i=0; i<token_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+-");
		for (int i=0; i<lexema_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+-");
		for (int i=0; i<maior_numero_ocorrencias; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"+\n");
		
		for (int i=0; i<ordem_de_entrada.tab_simb_count; i++){
			fprintf(arquivo_de_saida,"| %3d | %-*s | ", ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->posicao, token_max, obter_Nome_Do_Token(ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->COD));
			
			//fprintf(arquivo_de_saida,"%-*s", lexema_max, ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->lexema_cadeia);
			if (ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->COD == tk_INTEIRO){
				fprintf(arquivo_de_saida,"%-*d", lexema_max, ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->lexema_inteiro);
			}else if(ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->COD == tk_DECIMAL){
				int aux = strlen(ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->lexema_cadeia)-1;
				fprintf(arquivo_de_saida,"%-*.*f", lexema_max, aux, ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->lexema_decimal);
			}else{
				fprintf(arquivo_de_saida,"%-*s", lexema_max, ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->lexema_cadeia);
			}
			
			fprintf(arquivo_de_saida," | ");
			for(int j=0; j<ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->tamanho_ocorrencias; j++)
				fprintf(arquivo_de_saida,"(%3d,%3d) ", ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->ocorrencias[j].LIN, ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->ocorrencias[j].COL);
			fprintf(arquivo_de_saida,"%*s|\n", maior_numero_ocorrencias - ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->tamanho_ocorrencias * 10,"");
		}

		fprintf(arquivo_de_saida,"+-----+-");
		for (int i=0; i<token_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+-");
		for (int i=0; i<lexema_max; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"-+-");
		for (int i=0; i<maior_numero_ocorrencias; i++)
			fprintf(arquivo_de_saida,"-");
		fprintf(arquivo_de_saida,"+\n\n");
		
		//Fechando arquivo de saida e liberando memoria alocada
		fclose(arquivo_de_saida);
		free(nome_arquivo);	
	}
}


