#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#include TOKENS.H
//#include AUTOMATO.H
//#include ERROS.H
//#include TAB_SIMBOLOS.H
//#include RESULTADOS.H


#define TAMANHO_INICIAL 100
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
	char LEXEMA[TAMANHO_INICIAL], TOKEN[TAMANHO_INICIAL];
} tToken_resultante;


FILE *arquivo_de_entrada;
int linha = 1, coluna = 0, tamanho_lexema = 0, estado = 0, total_de_erros = 0;//, inicio_do_comentario;
Bool FINAL_DO_ARQUIVO = False;
char lexema[TAMANHO_INICIAL], prox_Simb;
int resumo[QUANTIDADE_DE_TOKENS] = {0};





///TOKENS.H
int identificar_Token(){
	char aux[tamanho_lexema];
	strcpy(aux,lexema);

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


void setar_Token (tToken_resultante* token_da_vez, const int id_token, const char* Lexema, Bool* continua, int LIN, int COL){
	*continua = False;
	strcpy(token_da_vez->TOKEN, obter_Nome_Do_Token(id_token));
	strcpy(token_da_vez->LEXEMA, Lexema);
	token_da_vez->LIN = LIN;
	token_da_vez->COL = COL;
	token_da_vez->COD = id_token + 1;
	
	resumo[id_token]++;
}





///AUTOMATO.H
void recuar(int n){
	// Volta o numero de caracters pedidos
	fseek(arquivo_de_entrada, n*(-1), SEEK_CUR);
	coluna -= n; 
}


void reiniciar_Automato(){
	estado = 0;
	tamanho_lexema = 0;
	lexema[tamanho_lexema] = '\0';
}


char ler_Proximo_Caractere(){
	char prox_Simb = getc(arquivo_de_entrada);
	if (prox_Simb != EOF)
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
	
	//printf("Estado: %d \nLexema: %s\n Caractere: %c\n", estado, lexema, prox_Simb);
	//char PAUSE = getchar();
	
	reiniciar_Automato();
	while (continua){
		switch (estado){
			case 0: ///Estado Inicial
				do{
					prox_Simb = ler_Proximo_Caractere();
				} while (isspace(prox_Simb)); 	
				
				LIN = linha;
				COL = coluna;
				
				if(isalpha(prox_Simb))
					estado = 1;
				else if(isdigit(prox_Simb))
					estado = 4;
				else if (prox_Simb == '.')
					estado = 6;
				else if (prox_Simb == '\"')
					estado = 8;
				else if (prox_Simb == EOF)
					estado = 10;
				else if (prox_Simb == ',')
					estado = 11;
				else if (prox_Simb == ';')
					estado = 12;
				else if (prox_Simb == ':')
					estado = 13;
				else if (prox_Simb == '(')
					estado = 14;
				else if (prox_Simb == ')')
					estado = 15;
				else if (prox_Simb == '<')
					estado = 16;
				else if (prox_Simb == '=')
					estado = 21;
				else if (prox_Simb == '>')
					estado = 22;
				else if (prox_Simb == '+')
					estado = 25;
				else if (prox_Simb == '-')
					estado = 28;
				else if (prox_Simb == '*')
					estado = 31;
				else if (prox_Simb == '/')
					estado = 32;
				else 
					estado = -8000; //Outros: Apresenta o erro e ignora o caractere...
				break;
			
			case 1: ///Estado identificador ou palavra reservada
				while(isalpha(prox_Simb) || isdigit(prox_Simb) || prox_Simb == '_'){
				   inserir_Caractere_No_Lexema(prox_Simb);
				   prox_Simb = ler_Proximo_Caractere();
				}
				if (isspace(prox_Simb)) //Se branco: fim da palavra, muda pro estado 2
					estado = 2;
				else //Se caractere invalido: muda pro estado -1
					estado = -1;
				break;
			
			case 2: /// Estado de verificacao
				id_token = identificar_Token();
				if (id_token == -1) //Identificador
					estado = 3;
				else //Palavra reservada
					estado = 39;
				break;

			case 3: ///Estado identificador (FINAL)
				setar_Token (&token_da_vez, tk_IDEN, lexema, &continua, LIN, COL);
				//Adiconar identificador na tabela de simbolos (FAZER)
				break;

			case 4: /// Estado Digito (inteiro ou decimal)
				while(isdigit(prox_Simb)){
				   inserir_Caractere_No_Lexema(prox_Simb);
				   prox_Simb = ler_Proximo_Caractere();
				}
				if (isspace(prox_Simb)) //Se branco: fim do inteiro, muda pro estado 5
					estado = 5;
				else if(prox_Simb == '.'){ //Se '.': numero decimal, muda pro estado 6
					estado = 6;
				} else //Senao: caractere invalido, muda pro estado -2
					estado = -2;
				break;

			case 5: /// Estado Digito Inteiro (FINAL)
				setar_Token (&token_da_vez, tk_INTEIRO, lexema, &continua, LIN, COL);
				//Adicionar inteiro na tabela de simbolos (FAZER)
				break;

			case 6: /// Estado Digito Decimal (após o ponto)
				do {
					inserir_Caractere_No_Lexema(prox_Simb);
					prox_Simb = ler_Proximo_Caractere();
				} while(isdigit(prox_Simb));
				
				if (isspace(prox_Simb)) //Se branco: fim do decimal, muda pro estado 7
					estado = 7;
				else //Senao: caractere invalido, muda pro estado -3
					estado = -3;
				break;
				
			case 7: /// Estado Digito Decimal (FINAL)
				setar_Token (&token_da_vez, tk_DECIMAL, lexema, &continua, LIN, COL);
				//Adicionar decimal na tabela de simbolos (FAZER)
				break;

			case 8: ///Estado Cadeia
				inserir_Caractere_No_Lexema(prox_Simb);
				while(estado == 8){
					prox_Simb = ler_Proximo_Caractere();
					if(prox_Simb == '\n' || prox_Simb == EOF){ //Erro na cadeia
						estado = -4;
					} else if(prox_Simb == '\"'){ //Fim da Cadeia
						estado = 9;
					}
					inserir_Caractere_No_Lexema(prox_Simb);
				}
				break;

			case 9: ///Estado Cadeia (FINAL)
				setar_Token (&token_da_vez, tk_CADEIA, lexema, &continua, LIN, COL);
				//Adiciona cadeia na tabela de simbolos (FAZER)
				break;

			case 10: ///Estado End of File (FINAL)
				setar_Token (&token_da_vez, tk_EOF, "", &continua, LIN, COL);
				FINAL_DO_ARQUIVO = True;
				break;

			case 11: ///Estado Virgula (FINAL)
				setar_Token (&token_da_vez, tk_virg, "", &continua, LIN, COL);
				break;

			case 12: ///Estado Ponto-e-virgula (FINAL)
				setar_Token (&token_da_vez, tk_pt_virg, "", &continua, LIN, COL);
				break;

			case 13: ///Estado Dois-Pontos (FINAL)
				setar_Token (&token_da_vez, tk_dois_pts, "", &continua, LIN, COL);
				break;

			case 14: ///Estado Abre Parenteses
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '*')
					estado = 35;
				else 
					estado = 38;
				break;

			case 15:  ///Estado Fecha Parenteses (FINAL)
				setar_Token (&token_da_vez, tk_fecha_par, "", &continua, LIN, COL);
				break;

			case 16: ///Estado <
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '-') //Atribuicao
					estado = 17;
				else if (prox_Simb == '>') //Diferente
					estado = 18;
				else if (prox_Simb == '=') //Menor Igual
					estado = 19;
				else
					estado = 20; //Menor
				break;

			case 17: ///Estado Atribuicao (FINAL)
				setar_Token (&token_da_vez, tk_atrib, "", &continua, LIN, COL);
				break;

			case 18: ///Estado Diferente (FINAL)
				setar_Token (&token_da_vez, tk_diferente, "", &continua, LIN, COL);
				break;

			case 19: ///Estado Menor Igual (FINAL)
				setar_Token (&token_da_vez, tk_menor_igual, "", &continua, LIN, COL);
				break;

			case 20: ///Estado Menor (FINAL)
				setar_Token (&token_da_vez, tk_menor, "", &continua, LIN, COL);
				break;

			case 21: ///Estado Igual (FINAL)
				setar_Token (&token_da_vez, tk_igual, "", &continua, LIN, COL);
				break;

			case 22: ///Estado >
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '=')
					estado = 23; //Maior Igual
				else
					estado = 24; //Maior
				break;

			case 23: ///Estado Maior Igual (FINAL)
				setar_Token (&token_da_vez, tk_maior_igual, "", &continua, LIN, COL);
				break;

			case 24: ///Estado Maior (FINAL)
				setar_Token (&token_da_vez, tk_maior, "", &continua, LIN, COL);
				recuar(1);
				break;

			case 25: ///Estado +
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '+')
					estado = 26; //Incremento
				else
					estado = 27; //Mais
				break;

			case 26: ///Estado Incremento (FINAL)
				setar_Token (&token_da_vez, tk_incr, "", &continua, LIN, COL);
				break;

			case 27: ///Estado Mais (FINAL)
				setar_Token (&token_da_vez, tk_incr, "", &continua, LIN, COL);
				recuar(1);
				break;

			case 28: ///Estado -
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '-')
					estado = 29; //Decremento
				else
					estado = 30; //Menos
				break;

			case 29: ///Estado Decremento (FINAL)
				setar_Token (&token_da_vez, tk_decr, "", &continua, LIN, COL);
				break;

			case 30: ///Estado Menos (FINAL)
				setar_Token (&token_da_vez, tk_menos, "", &continua, LIN, COL);
				recuar(1);
				break;

			case 31: ///Estado Vezes (FINAL)
				setar_Token (&token_da_vez, tk_vezes, "", &continua, LIN, COL);
				break;

			case 32: ///Estado /
				prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '*')
					estado = 33;
				else
					estado = 34;
				break;

			case 33: ///Estado Comentario de linha (FINAL)
				while (prox_Simb != '\n' && prox_Simb != EOF)
					prox_Simb = ler_Proximo_Caractere();
				//Comentários devem ser ignorados pelo analisador léxico para efeito de reconhecimento de tokens
				reiniciar_Automato();
				break;

			case 34: ///Estado Dividido (FINAL)
				setar_Token (&token_da_vez, tk_dividido, "", &continua, LIN, COL);
				recuar(1);
				break;

			case 35: ///Comentario de bloco
				while (prox_Simb != '*' && prox_Simb != EOF)
					prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == '*') //Pode ser um fim de comentario
					estado = 36;
				else //Fim de arquivo
					estado = -5;
				break;

			case 36: ///Possivel fim de comentario de bloco
				while (prox_Simb == '*') //Ler proximo char, se * continua
					prox_Simb = ler_Proximo_Caractere();
				if (prox_Simb == ')') //Fim do comentario
					estado = 37;
				else if (prox_Simb == EOF) //Fim de arquivo
					estado = -5;
				else //Não é fim de comentario
					estado = 35;
				break;

			case 37: ///Fim do comentario de bloco (FINAL)
				//Comentários devem ser ignorados pelo analisador léxico para efeito de reconhecimento de tokens
				reiniciar_Automato();
				break;

			case 38: ///Estado Abre Parenteses (FINAL)
				setar_Token (&token_da_vez, tk_abre_par, "", &continua, LIN, COL);
				recuar(1);
				break;

			case 39: ///Palavra reservada token++ (FINAL)
				setar_Token (&token_da_vez, id_token, "", &continua, LIN, COL);
				break;

			case -1:
				// Identifica Erro lexico(pode ou nao ser erro)
				estado = 2;
				if (prox_Simb != EOF)
					recuar(1);
				break;

			case -2:
				// Identifica Erro lexico (pode ou nao ser erro)
				estado = 5;
				if (prox_Simb != EOF)
					recuar(1);
				break;

			case -3:
				// Identifica Erro lexico (pode ou nao ser erro)
				estado = 7;
				if (prox_Simb != EOF)
					recuar(1);
				break;

			case -4:
				// Identifica Erro lexico(nao fechameno da cadeia)
				estado = 9;
				if (prox_Simb != EOF)
					recuar(1);
				break;

			case -5:
				// Identifica Erro lexico(nao fechameno do comentario)
				// Volta pro inicio e ignora ate o primeiro /n (estado 33)
				
				//Um comentário de bloco não fechado deve gerar o erro léxico “Comentário de bloco não fechado”. A análise
				//deve ser retomada no início da linha seguinte àquela onde o comentário foi aberto, efetivamente transfor-
				//mando-o em um comentário de linha válido, apesar de truncado.
				///recuar(n);
				break;

			default:
				//Apresnta erro e ignora o caractere....
				total_de_erros++;
				reiniciar_Automato();
				break;
		}
		///estado = tabela_Transicoes[estado][prox_Simb];
	}
	return token_da_vez;
}





///ERROS.H
void setar_Erro(){
	//De acordo com a secao 3.4
}


/// Ponto de vista lexico(FAZER)
// "Ponto isolado";
// "Cadeia nao fechada";
// "Comentario de bloco não fechado";
// "Caracter invalido";
// "Delimitador esperado";





///TAB_SIMBOLOS.H
/// ???





///RESULTADOS.H
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
	printf("TOTAL DE ERROS: %d\n\n", total_de_erros);
}


void printar_Lista_De_Tokens_Reconhecidos(const char* nomeArquivo, const tToken_resultante * token_da_vez, int n){
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
	printf("LISTA DE ERROS LEXICOS EM \"%s\" \n\n", nomeArquivo);
	
	//(FAZER)
	
	printf("TOTAL DE ERROS: %d\n\n", total_de_erros);
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
	
	while(!FINAL_DO_ARQUIVO){
		token_da_vez[n] = analizador_Lexico();
		n++;
	}

	
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


