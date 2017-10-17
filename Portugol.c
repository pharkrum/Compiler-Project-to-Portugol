#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#include TOKENS.H
//#include AUTOMATO.H
//#include ERROS.H
//#include TAB_SIMBOLOS.H
//#include RESULTADOS.H


#define TAMANHO_INICIAL 500
#define QUANTIDADE_DE_TOKENS 41


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
	// tString LEXEMA
	char LEXEMA[TAMANHO_INICIAL];
	char TOKEN[50];
} tToken_resultante;

typedef struct{
	int LIN, COL;
	char ERRO[TAMANHO_INICIAL];
} tErro;

/* (FAZER) - Corrigir tamanho de strings
typedef struct{
	int tamanho_lexema;
	int limite;
	char * lexema;
} tSring;
*/


//(FAZER) Retirar variaveis globais
FILE *arquivo_de_entrada;
int linha = 1, coluna = 0, tamanho_lexema = 0, estado = 0, contador_de_bloco = 0;
char lexema[TAMANHO_INICIAL], prox_Simb;
int resumo[QUANTIDADE_DE_TOKENS] = {0};
tErro erro_da_vez[TAMANHO_INICIAL]; 
int qtd_erros = 0;





///TOKENS.H
int identificar_Token(){
	//(FAZER)Corigir como obter tamanho de aux - Desse jeito funciona, uma vez q tamanho_lexema vai esta definido
	char aux[tamanho_lexema];
	strcpy(aux, lexema);

	for(int i = 0; i < strlen(aux); i++) // Torna todas as letras minusculas
		aux[i] = tolower(aux[i]);

	if(strcmp(aux, "inicio") == 0)
		return tk_inicio;
	else if(strcmp(aux, "fim") == 0)
		return tk_fim;
	else if(strcmp(aux, "int") == 0)
		return tk_int;
	else if(strcmp(aux, "dec") == 0)
		return tk_dec;
	else if(strcmp(aux, "leia") == 0)
		return tk_leia;
	else if(strcmp(aux, "imprima") == 0)
		return tk_imprima;
	else if(strcmp(aux, "para") == 0)
		return tk_para;
	else if(strcmp(aux, "de") == 0)
		return tk_de;
	else if(strcmp(aux, "ate") == 0)
		return tk_ate;
	else if(strcmp(aux, "passo") == 0)
		return tk_passo;
	else if(strcmp(aux, "fim_para") == 0)
		return tk_fim_para;
	else if(strcmp(aux, "se") == 0)
		return tk_se;
	else if(strcmp(aux, "entao") == 0)
		return tk_entao;
	else if(strcmp(aux, "senao") == 0)
		return tk_senao;
	else if(strcmp(aux, "fim_se") == 0)
		return tk_fim_se;
	else if(strcmp(aux, "e") == 0)
		return tk_e;
	else if(strcmp(aux, "ou") == 0)
		return tk_ou;
	else if(strcmp(aux, "nao") == 0)
		return tk_nao;
	else
		return -1;
}


void inserir_Caractere_No_Lexema(char prox_Simb){
	if (tamanho_lexema < TAMANHO_INICIAL-1){
		lexema[tamanho_lexema] = prox_Simb;
		tamanho_lexema++;
		lexema[tamanho_lexema] = '\0';
	}
	//(FAZER)
	//A maneira correta de lidar com essa questão é alocar a estrutura de dados dinamicamente, ainda que com um
	//valor inicial arbitrário, e, se necessário, redimensioná-la durante a execução do programa.
	// tString
}


const char * obter_Nome_Do_Token(tToken id_token){
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


void setar_Token (tToken_resultante* token_da_vez, const int id_token, const char* Lexema, Bool* continua, const int LIN, const int COL){
	//Melhorar copias (tToken_resultante) - (FAZER)
	strcpy(token_da_vez->TOKEN, obter_Nome_Do_Token(id_token));
	strcpy(token_da_vez->LEXEMA, Lexema);
	token_da_vez->LIN = LIN;
	token_da_vez->COL = COL;
	token_da_vez->COD = id_token + 1;
	
	resumo[id_token]++;
	*continua = False;
}





///ERROS.H
void setar_Erro(const char* tipo_de_erro){//tErro * erro_da_vez, const char prox_Simb
	//(FAZER) Erros nao eh um vetor
	erro_da_vez[qtd_erros].LIN = linha;
	erro_da_vez[qtd_erros].COL = coluna;
	strcpy(erro_da_vez[qtd_erros].ERRO, tipo_de_erro);
	qtd_erros++;
}





///TAB_SIMBOLOS.H
/// ???





///AUTOMATO.H
void retrocede_Caracteres(int n){
	fseek(arquivo_de_entrada, (-1)*n*sizeof(char), SEEK_CUR);
	coluna --; 
	if (prox_Simb == '\n')
		linha--; //Isso nao influencia na coluna, uma vez que o prox a ser lido será '\n, o valor da coluna é zerado
}


void recuar(int n, int LIN, int COL){
	// Volta o numero de caracters pedidos
	fseek(arquivo_de_entrada, (-1)*n*sizeof(char), SEEK_CUR);
	coluna = COL;
	linha = LIN;
	prox_Simb = ' ';
}


void reiniciar_Automato(){
	estado = 0;
	tamanho_lexema = 0;
	lexema[tamanho_lexema] = '\0';
	contador_de_bloco = 0;
}


char ler_Proximo_Caractere(){
	char prox_Simb = getc(arquivo_de_entrada);
	if (prox_Simb != EOF) //EOF não conta como coluna
		coluna++;
	if(prox_Simb == '\n'){ // Se o caracter for uma quebra de linha: Novos valores para linha e coluna
		linha++;
		coluna = 0;
	}
	return prox_Simb;
}


tToken_resultante analizador_Lexico (void){
	Bool continua = True;
	int id_token = -1, LIN, COL;
	tToken_resultante token_da_vez;
	
	//printf("\nEstado: %d \nLexema: %s \nCaractere: %c\n", estado, lexema, prox_Simb);
	//char PAUSE = getchar();
	
	reiniciar_Automato();
	while (continua){
		switch (estado){
			case 0: ///Estado Inicial
				if (prox_Simb != EOF){
					do{
						prox_Simb = ler_Proximo_Caractere();
					} while (isspace(prox_Simb));
				}
				LIN = linha;
				COL = coluna;
				if(isalpha(prox_Simb))
					estado = 1;
				else if(isdigit(prox_Simb))
					estado = 5;
				else if (prox_Simb == '.')
					estado = 11;
				else if (prox_Simb == '\"')
					estado = 13;
				else if (prox_Simb == EOF)
					estado = 16;
				else if (prox_Simb == ',')
					estado = 17;
				else if (prox_Simb == ';')
					estado = 18;
				else if (prox_Simb == ':')
					estado = 19;
				else if (prox_Simb == '(')
					estado = 20;
				else if (prox_Simb == ')')
					estado = 26;
				else if (prox_Simb == '<')
					estado = 27;
				else if (prox_Simb == '=')
					estado = 32;
				else if (prox_Simb == '>')
					estado = 33;
				else if (prox_Simb == '+')
					estado = 36;
				else if (prox_Simb == '-')
					estado = 39;
				else if (prox_Simb == '*')
					estado = 42;
				else if (prox_Simb == '/')
					estado = 43;
				else 
					estado = -1; //Outros: Apresenta o erro e ignora o caractere...
				break;
			
			case 1: ///Estado identificador ou palavra reservada
				while(isalpha(prox_Simb) || isdigit(prox_Simb) || prox_Simb == '_'){
				   inserir_Caractere_No_Lexema(prox_Simb);
				   prox_Simb = ler_Proximo_Caractere();
				}
				estado = 2;
				break;
			
			case 2: /// Estado de verificacao
				id_token = identificar_Token();
				if (id_token == -1) //Identificador
					estado = 4;
				else //Palavra reservada
					estado = 3;
				retrocede_Caracteres(1);
				break;
				
			case 3: ///Palavra reservada token++ (FINAL)
				setar_Token (&token_da_vez, id_token, "", &continua, LIN, COL);
				break;
				
			case 4: ///Estado identificador (FINAL)
				setar_Token (&token_da_vez, tk_IDEN, lexema, &continua, LIN, COL);
				//Adiconar identificador na tabela de simbolos (FAZER)
				break;

			case 5: /// Estado Digito (inteiro ou decimal)
				while(isdigit(prox_Simb)){
				   inserir_Caractere_No_Lexema(prox_Simb);
				   prox_Simb = ler_Proximo_Caractere();
				}
				if (isspace(prox_Simb)) //Se branco: fim do inteiro, muda pro estado 6
					estado = 6;
				else if(prox_Simb == '.'){ //Se '.': numero decimal, muda pro estado 8
					estado = 8;
				} else //Senao: caractere invalido, muda pro estado 7
					estado = 7;
				break;

			case 6: /// Estado Digito Inteiro (FINAL)
				setar_Token (&token_da_vez, tk_INTEIRO, lexema, &continua, LIN, COL);
				//Adicionar inteiro na tabela de simbolos (FAZER)
				break;
				
			case 7: ///Estado de erro lexico apos inteiro
				if (isalpha(prox_Simb))
					setar_Erro("Delimitador esperado");
				estado = 6;
				retrocede_Caracteres(1);
				break;
				
			case 8: ///Estado Digito Decimal Iniciando com digito
				do {
				   inserir_Caractere_No_Lexema(prox_Simb);
				   prox_Simb = ler_Proximo_Caractere();
				} while(isdigit(prox_Simb));
				if (isspace(prox_Simb)) //Se branco: fim do decimal, muda pro estado 10
					estado = 9;
				else //Senao: caractere invalido, muda pro estado 9
					estado = 10;
				break;
				
			case 9: /// Estado Digito Decimal (FINAL)
				setar_Token (&token_da_vez, tk_DECIMAL, lexema, &continua, LIN, COL);
				//Adicionar decimal na tabela de simbolos (FAZER)
				break;

			case 10: ///Estado de erro lexico apos decimal
				if (isalpha(prox_Simb))
					setar_Erro("Delimitador esperado");
				estado = 9;
				retrocede_Caracteres(1);
				break;
								
			case 11: /// Estado Digito Decimal Iniciando com ponto
				inserir_Caractere_No_Lexema(prox_Simb);
				prox_Simb = ler_Proximo_Caractere();
				if (isdigit(prox_Simb)) //Se digito: Decimal valido, muda pro estado 
					estado = 8;
				else //Senao: caractere invalido, muda pro estado 
					estado = 12;
				break;
				
			case 12: ///Estado de erro lexico Ponto Isolado (FINAL)
				setar_Erro("Ponto isolado");
				reiniciar_Automato();
				retrocede_Caracteres(1);
				break;

			case 13: ///Estado Cadeia
				inserir_Caractere_No_Lexema(prox_Simb);
				while(estado == 13){
					prox_Simb = ler_Proximo_Caractere();
					if(prox_Simb == '\n' || prox_Simb == EOF){ //Erro na cadeia
						estado = 15;
					} else if(prox_Simb == '\"'){ //Fim da Cadeia
						estado = 14;
					}
					inserir_Caractere_No_Lexema(prox_Simb);
				}
				break;

			case 14: ///Estado Cadeia (FINAL)
				setar_Token (&token_da_vez, tk_CADEIA, lexema, &continua, LIN, COL);
				//Adiciona cadeia na tabela de simbolos (FAZER)
				break;
			
			case 15: ///Estado de erro lexico nao fechameno da cadeia
				setar_Erro("Cadeia nao fechada");
				estado = 14;
				retrocede_Caracteres(1);
				break;

			case 16: ///Estado End of File (FINAL)
				setar_Token (&token_da_vez, tk_EOF, "", &continua, LIN, COL);
				break;

			case 17: ///Estado Virgula (FINAL)
				setar_Token (&token_da_vez, tk_virg, "", &continua, LIN, COL);
				break;

			case 18: ///Estado Ponto-e-virgula (FINAL)
				setar_Token (&token_da_vez, tk_pt_virg, "", &continua, LIN, COL);
				break;

			case 19: ///Estado Dois-Pontos (FINAL)
				setar_Token (&token_da_vez, tk_dois_pts, "", &continua, LIN, COL);
				break;

			case 20: ///Estado Abre Parenteses
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '*')
					estado = 21;
				else 
					estado = 22;
				break;
				
			case 21: ///Comentario de bloco
				while (prox_Simb != '*' && prox_Simb != EOF){
					contador_de_bloco++;
					prox_Simb = ler_Proximo_Caractere();
				}
				if (prox_Simb == '*') //Pode ser um fim de comentario
					estado = 23;
				else //Fim de arquivo
					estado = 25;
				break;
				
			case 22: ///Estado Abre Parenteses (FINAL)
				setar_Token (&token_da_vez, tk_abre_par, "", &continua, LIN, COL);
				retrocede_Caracteres(1);
				break;
				
			case 23: ///Possivel fim de comentario de bloco
				while (prox_Simb == '*'){ //Ler proximo char, se * continua
					contador_de_bloco++;
					prox_Simb = ler_Proximo_Caractere();
				}
				if (prox_Simb == ')') //Fim do comentario
					estado = 24;
				else if (prox_Simb == EOF) //Fim de arquivo
					estado = 25;
				else //Não é fim de comentario
					estado = 21;
				break;

			case 24: ///Fim do comentario de bloco (FINAL)
				//Comentários devem ser ignorados pelo analisador léxico para efeito de reconhecimento de tokens
				reiniciar_Automato();
				break;

			case 25: ///Estado de erro lexico Comentario de bloco não fechado
				recuar(contador_de_bloco, LIN, COL);
				setar_Erro("Comentario de bloco não fechado");
				estado = 44; // Volta pro inicio e ignora ate o primeiro /n ou EOF
				break;
			
			case 26:  ///Estado Fecha Parenteses (FINAL)
				setar_Token (&token_da_vez, tk_fecha_par, "", &continua, LIN, COL);
				break;

			case 27: ///Estado <
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '-') //Atribuicao
					estado = 28;
				else if (prox_Simb == '>') //Diferente
					estado = 29;
				else if (prox_Simb == '=') //Menor Igual
					estado = 30;
				else
					estado = 31; //Menor
				break;

			case 28: ///Estado Atribuicao (FINAL)
				setar_Token (&token_da_vez, tk_atrib, "", &continua, LIN, COL);
				break;

			case 29: ///Estado Diferente (FINAL)
				setar_Token (&token_da_vez, tk_diferente, "", &continua, LIN, COL);
				break;

			case 30: ///Estado Menor Igual (FINAL)
				setar_Token (&token_da_vez, tk_menor_igual, "", &continua, LIN, COL);
				break;

			case 31: ///Estado Menor (FINAL)
				setar_Token (&token_da_vez, tk_menor, "", &continua, LIN, COL);
				retrocede_Caracteres(1);
				break;

			case 32: ///Estado Igual (FINAL)
				setar_Token (&token_da_vez, tk_igual, "", &continua, LIN, COL);
				break;

			case 33: ///Estado >
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '=')
					estado = 34; //Maior Igual
				else
					estado = 35; //Maior
				break;

			case 34: ///Estado Maior Igual (FINAL)
				setar_Token (&token_da_vez, tk_maior_igual, "", &continua, LIN, COL);
				break;

			case 35: ///Estado Maior (FINAL)
				setar_Token (&token_da_vez, tk_maior, "", &continua, LIN, COL);
				retrocede_Caracteres(1);
				break;

			case 36: ///Estado +
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '+')
					estado = 37; //Incremento
				else
					estado = 38; //Mais
				break;

			case 37: ///Estado Incremento (FINAL)
				setar_Token (&token_da_vez, tk_incr, "", &continua, LIN, COL);
				break;

			case 38: ///Estado Mais (FINAL)
				setar_Token (&token_da_vez, tk_incr, "", &continua, LIN, COL);
				retrocede_Caracteres(1);
				break;

			case 39: ///Estado -
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '-')
					estado = 40; //Decremento
				else
					estado = 41; //Menos
				break;

			case 40: ///Estado Decremento (FINAL)
				setar_Token (&token_da_vez, tk_decr, "", &continua, LIN, COL);
				break;

			case 41: ///Estado Menos (FINAL)
				setar_Token (&token_da_vez, tk_menos, "", &continua, LIN, COL);
				retrocede_Caracteres(1);
				break;

			case 42: ///Estado Vezes (FINAL)
				setar_Token (&token_da_vez, tk_vezes, "", &continua, LIN, COL);
				break;

			case 43: ///Estado /
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '*')
					estado = 44;
				else
					estado = 45;
				break;

			case 44: ///Estado Comentario de linha (FINAL)
				while (prox_Simb != '\n' && prox_Simb != EOF)
					prox_Simb = ler_Proximo_Caractere();
				//Comentários devem ser ignorados pelo analisador léxico para efeito de reconhecimento de tokens
				reiniciar_Automato();
				break;

			case 45: ///Estado Dividido (FINAL)
				setar_Token (&token_da_vez, tk_dividido, "", &continua, LIN, COL);
				retrocede_Caracteres(1);
				break;

			default: ///Estado Defauult: Apresnta erro e ignora o caractere (FINAL)
				setar_Erro("Caracter invalido");
				reiniciar_Automato();
				break;
		}
		///estado = tabela_Transicoes[estado][prox_Simb];
	}
	return token_da_vez;
}





///RESULTADOS.H
int imprimir_Linha(void){
	int carcteres_na_linha = 0;
	prox_Simb = ' ';
	while (prox_Simb != '\n' && prox_Simb != EOF){
		printf("%c", prox_Simb);
		prox_Simb = ler_Proximo_Caractere();
		carcteres_na_linha++;
	}
	printf("\n");
	return carcteres_na_linha;
}


void imprimir_seta(int n){
	printf("       ");
	for (int i=0; i<n-1; i++)
		printf("-");
	printf("^\n");
}


void printar_Resumo(void){
	//Procurar tamanho max (FAZER)
	int tot = 0;
	printf("RESUMO \n\n");
	printf("+-----+----------------+------+\n");
	printf("| COD | TOKEN          | USOS |\n");
	printf("+-----+----------------+------+\n");
	for (int i=0; i<QUANTIDADE_DE_TOKENS; i++){
		printf("| %3d | %-14s | %4d |\n", i+1, obter_Nome_Do_Token(i), resumo[i]);
		tot += resumo[i];
	}
	printf("+-----+----------------+------+\n");
	printf("|   0 | TOTAL          | %4d |\n", tot);
	printf("+-----+----------------+------+\n\n");
	printf("TOTAL DE ERROS: %d\n\n", qtd_erros);
}


void printar_Lista_De_Tokens_Reconhecidos(const char* nomeArquivo, const tToken_resultante * token_da_vez, const int n){
	//Procurar tamanho max (FAZER)
	//ERRO nas distancias (FAZER)
	int linha_Da_Vez = 0;
	printf("LISTA DE TOKENS RECONHECIDOS EM \"%s\" \n\n", nomeArquivo);
	printf("+-----+-----+-----+----------------+------------------+--------------+\n");
	printf("| LIN | COL | COD | TOKEN          | LEXEMA           | POS TAB SIMB |\n");
	printf("+-----+-----+-----+----------------+------------------+--------------+\n");
	for (int i=0; i<n; i++){
		if (token_da_vez[i].LIN == linha_Da_Vez)
			printf("|     | %3d | %3d | %-14s | %-16s |              |\n", token_da_vez[i].COL, token_da_vez[i].COD, token_da_vez[i].TOKEN, token_da_vez[i].LEXEMA);
		else{
			printf("| %3d | %3d | %3d | %-14s | %-16s |              |\n", token_da_vez[i].LIN, token_da_vez[i].COL, token_da_vez[i].COD, token_da_vez[i].TOKEN, token_da_vez[i].LEXEMA);
			linha_Da_Vez = token_da_vez[i].LIN;
		}
	}
	printf("+-----+-----+-----+----------------+------------------+--------------+\n\n");
}


void printar_Tabela_De_Simbolos(const char* nomeArquivo){
	//Procurar tamanho max (FAZER)
	//ERRO nas distancias (FAZER)
	printf("TABELA DE SIMBOLOS - \"%s\" \n\n", nomeArquivo);
	printf("+-----+-----------+------------------+-------------------------------+\n");
	printf("| POS | TOKEN     | LEXEMA           | POS NA ENTRADA (linha,coluna) |\n");
	printf("+-----+-----------+------------------+-------------------------------+\n");
	printf("|     |           |                  |                               |\n");
	printf("+-----+-----------+------------------+-------------------------------+\n\n");
}


void printar_Lista_De_Erros_Lexicos(const char* nomeArquivo){
	//Voltar ao inico do arquivo
	rewind(arquivo_de_entrada);
	//Zerar linha e coluna
	linha  = 1;
	coluna = 0;
	//Variaveis
	int carcteres_na_linha = -1, i = 0;
	
	printf("LISTA DE ERROS LEXICOS EM \"%s\" \n\n", nomeArquivo);
	while(!feof(arquivo_de_entrada)){
		printf("[%4d]", linha);
		carcteres_na_linha = imprimir_Linha();
		while (erro_da_vez[i].LIN == linha-1) {
			imprimir_seta(erro_da_vez[i].COL);
			printf("       Erro lexico na linha %d coluna %d: %s\n", erro_da_vez[i].LIN, erro_da_vez[i].COL, erro_da_vez[i].ERRO);
			i++;
			if (erro_da_vez[i].LIN == linha-1){	
				retrocede_Caracteres(carcteres_na_linha);
				printf("[%4d]", linha);
				carcteres_na_linha = imprimir_Linha();
			}
		}
	}
	
	printf("\nTOTAL DE ERROS: %d\n\n", qtd_erros);
}





///FUNCAO PRINCIPAL
int main (int argc, char *argv[]){
	tToken_resultante token_da_vez[TAMANHO_INICIAL];
	int n = 0;
	
	if (argc != 2){
		printf("Exemplo de execucao: ./Portugol prog01.ptg\nTente novamente\n");
		exit(1);
	}
	
	printf("Arquivo : %s \n\n", argv[1]);

	if ((arquivo_de_entrada = fopen(argv[1], "r")) == NULL){
		printf("Erro ao abrir o arquivo!!! \nPor favor, verifique a existencia do mesmo e tente novamente.\n");
		exit(1);
	}
	
	do {
		token_da_vez[n] = analizador_Lexico();
		n++;
	} while(token_da_vez[n-1].COD-1 != tk_EOF);

	
	printar_Lista_De_Erros_Lexicos(argv[1]);
	printar_Lista_De_Tokens_Reconhecidos(argv[1], token_da_vez, n);
	printar_Resumo();
	printar_Tabela_De_Simbolos(argv[1]);
	
	printf("\nAnalise concluida com sucesso, os seguintes arquivos gerados:\n");
	printf("   %s.err com o conteúdo do arquivo de entrada e os erros léxicos devidamente marcados\n", argv[1]);
	printf("   %s.tbl com a lista de tokens reconhecidos\n", argv[1]);
	printf("   %s.tok com o conteúdo da tabela de símbolos após processamento.\n\n", argv[1]);
	
	fclose(arquivo_de_entrada);
	
	return 0;
}


