/* ------------------------------------------------------------- */
/* Certifique-se que este arquivo não é incluído mais de uma vez */
/* ------------------------------------------------------------- */
#ifndef _AUTOMATO_H_
#define _AUTOMATO_H_

/* --------------------------- */
/* BIBLIOTECAS E INCLUDE FILES */
/* --------------------------- */
#include "erros.h"


/* -------------------------------------- */
/*      CONSTANTES, TIPOS E VARIÁVEIS     */
/* -------------------------------------- */
#define TOTAL_CLASSES_CARACTERES 21
#define QUANTIDADE_DE_ESTADOS 45
#define QUANTIDADE_DE_TOKENS 41

//Definicao de Tipos (ENUM) - Classe de carcteres
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

//VARIAVEIS
FILE *arquivo_de_entrada;
char tabela_Transicoes[QUANTIDADE_DE_ESTADOS][TOTAL_CLASSES_CARACTERES] = {
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
int linha_arquivo = 1, coluna_arquivo = 1;


/* ------------------------------- */
/*      PROTÓTIPOS DAS FUNÇÕES     */
/* ------------------------------- */
tToken analizador_Lexico(void);
void mostrar_Tabela_Transicoes (void);
char ler_Proximo_Caractere(void);
void retroceder_Caracteres(const int, const char);
tClasse_caractere carctere_2_tClasse_caractere(const char);
void retroceder_Ate(const int, const int, const int);


/* ------------------------------- */
/*    IMPLEMENTAÇÃO DAS FUNÇÕES    */
/* ------------------------------- */
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


void mostrar_Tabela_Transicoes (void){
	printf("      b  \\n  l  d  _  \"  .  ,  ;  :  (  )  <  =  >  +  -  *  /  e  ot\n");
	for (int i = 0; i < QUANTIDADE_DE_ESTADOS; i++){
		printf("%3d |", i);
		for (int j = 0; j < TOTAL_CLASSES_CARACTERES; j++){
			printf ("%3d", tabela_Transicoes[i][j]);
		}
		printf("\n");
	}
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


void retroceder_Ate(const int n, const int linha, const int coluna){
	//Similar a funcao retroceder_Caracteres, mas lida com um alcance muito maior para o valor de n
	fseek(arquivo_de_entrada, (-1)*n*sizeof(char), SEEK_CUR);
	linha_arquivo = linha;
	coluna_arquivo = coluna;
}

#endif 
