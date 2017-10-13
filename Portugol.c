#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define TAMANHO_MAX 100
#define QUANTIDADE_DE_TOKENS 42


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
	tk_dividido,
	ERRO
} tToken;

char * NOMES[] = {"tk_EOF","tk_IDEN","tk_INTEIRO","tk_DECIMAL","tk_CADEIA","tk_inicio","tk_fim","tk_int","tk_dec","tk_leia","tk_imprima","tk_para","tk_de","tk_ate",
"tk_passo","tk_fim_para","tk_se","tk_entao","tk_senao","tk_fim_se","tk_e","tk_ou","tk_nao","tk_virg","tk_pt_virg","tk_dois_pts","tk_abre_par","tk_fecha_par",
"tk_menor","tk_menor_igual","tk_maior","tk_maior_igual","tk_diferente","tk_igual","tk_incr","tk_decr","tk_atrib","tk_mais","tk_menos","tk_vezes","tk_dividido","ERRO"};
FILE *Arquivo;
int linha = 0, coluna = 0, tamanho_lexema=0, id_token;
char lexema[TAMANHO_MAX];
int token[QUANTIDADE_DE_TOKENS] = {0};
char c;
int estado = 0; 
int Bool = 1;

void printa_Resultado(void){
	for (int i=0; i<QUANTIDADE_DE_TOKENS; i++)
		printf("%s => %d\n", NOMES[i], token[i]);
}


void zera_Lexema_E_Estado(){
	estado = 0;
	tamanho_lexema = 0;
	lexema[tamanho_lexema] = '\0';
}


char leia_Proximo_Caractere(){
	char c = getc(Arquivo);
	coluna++;
	if(c == '\n'){ // Se o caracter for uma quebra de linha: Novos valores para linha e coluna
		linha++;
		coluna = 0;
	}
	return c;
}


void insere_Caractere_No_Lexema(char c){
	if (tamanho_lexema < TAMANHO_MAX-1){
		lexema[tamanho_lexema] = c;
		tamanho_lexema++;
		lexema[tamanho_lexema] = '\0';
	}
}


void retrair(int n){ // Volta na na coluna o numero de caracters pedidos
    fseek(Arquivo, n*(-1), SEEK_CUR);
    coluna -= n; 
}


tToken palavra_reservada(){
	char aux[tamanho_lexema];
	strcpy(aux,lexema);

	for(int i = 0; i < strlen(aux); i++)
		aux[i] = tolower(aux[i]); // Torna todas as letras minusculas

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
		return tk_IDEN;
}


void automato (void){
	//printf("Estado: %d\n", estado);
	//printf("Lexema: %s\n", lexema);
	//char d = getchar();
	switch (estado){
		case 0: ///Estado Inicial
			c = leia_Proximo_Caractere();
			
            while (isspace(c))
            	c = leia_Proximo_Caractere(); 	
			//printf("caracter: %c\n", c);
			if(isalpha(c))
				estado = 1;
			else if(isdigit(c))
				estado = 4;
			else if (c == '.')
				estado = 6;
			else if (c == '\"')
				estado = 8;
			else if (c == EOF)
				estado = 10;
			else if (c == ',')
				estado = 11;
			else if (c == ';')
				estado = 12;
			else if (c == ':')
				estado = 13;
			else if (c == '(')
				estado = 14;
			else if (c == ')')
				estado = 15;
			else if (c == '<')
				estado = 16;
			else if (c == '=')
				estado = 21;
			else if (c == '>')
				estado = 22;
			else if (c == '+')
				estado = 25;
			else if (c == '-')
				estado = 28;
			else if (c == '*')
				estado = 31;
			else if (c == '/')
				estado = 32;
			else //Outros: Apresnta erro e ignora o caractere....
				estado = -100; // Default
			break;
		
		
		case 1: ///Estado identificador ou palavra reservada
			while(isalpha(c) || isdigit(c) || c == '_'){
               insere_Caractere_No_Lexema(c);
               c = leia_Proximo_Caractere();
            }
            if (isspace(c)) //Se branco, fim da palavra muda pro estado 2
				estado = 2;
			else //Se caractere invalido muda pro estado -1
				estado = -1;
			break;
		
		
		case 2: /// Estado de verificacao
			id_token = palavra_reservada();
			if (id_token == tk_IDEN) //Identificador
				estado = 3;
			else //Palavra reservada
				estado = 39;
			break;


		case 3: ///Estado identificador (FINAL)
			//Adicona identificador na tabela de simbolos (FAZER)
			token[tk_IDEN]++;
			zera_Lexema_E_Estado();
			break;


		case 4: /// Estado Digito (inteiro ou decimal)
			while(isdigit(c)){
               insere_Caractere_No_Lexema(c);
               c = leia_Proximo_Caractere();
			}
			if (isspace(c)) //Se branco, fim do inteiro: muda pro estado 5
				estado = 5;
			else if(c == '.'){ // se '.' estado 6
				estado = 6;
			} else //Se caractere invalido muda pro estado -2
				estado = -2;
			break;


		case 5: /// Estado Digito Inteiro (FINAL)
			//Adiciona inteiro na tabela de simbolos (FAZER)
			token[tk_INTEIRO]++;
			zera_Lexema_E_Estado();
			break;


		case 6: /// Estado Digito Decimal (após o ponto)
			do {
				insere_Caractere_No_Lexema(c);
				c = leia_Proximo_Caractere();
			} while(isdigit(c));
			
			if (isspace(c)) //Se branco, fim do decimal: muda pro estado 7
				estado = 7;
			else //Se caractere invalido muda pro estado -3
				estado = -3;
			break;
			

		case 7: /// Estado Digito Decimal (FINAL)
			//Adiciona decimal na tabela de simbolos (FAZER)
			token[tk_DECIMAL]++;
			zera_Lexema_E_Estado();
			break;

		case 8: ///Estado Cadeia
			while(1){
				insere_Caractere_No_Lexema(c);
				c = leia_Proximo_Caractere();

				if(c == '\n' || c == EOF){ //Erro na cadeia
					estado = -4;
					break;
				} else if(c == '\"'){ //Fim da Cadeia
					estado = 9;
					break;
				}
			}
			break;

		case 9: ///Estado Cadeia (FINAL)
			//Adiciona cadeia na tabela de simbolos (FAZER)
			token[tk_CADEIA]++;
			zera_Lexema_E_Estado();
			break;

		case 10: ///Estado End of File (FINAL)
			token[tk_EOF]++;
			zera_Lexema_E_Estado();
			Bool = 0;
			break;

		case 11: ///Estado Virgula (FINAL)
			token[tk_virg]++;
			zera_Lexema_E_Estado();
			break;

		case 12: ///Estado Ponto-e-virgula (FINAL)
			token[tk_pt_virg]++;
			zera_Lexema_E_Estado();
			break;

		case 13: ///Estado Dois-Pontos (FINAL)
			token[tk_dois_pts]++;
			zera_Lexema_E_Estado();
			break;

		case 14: ///Estado Abre Parenteses
			c = leia_Proximo_Caractere();
			if (c == '*')
				estado = 35;
			else 
				estado = 38;
			break;

		case 15:  ///Estado Fecha Parenteses (FINAL)
			token[tk_fecha_par]++;
			zera_Lexema_E_Estado();
			break;

		case 16: ///Estado <
			c = leia_Proximo_Caractere();
			if (c == '-') //Atribuicao
				estado = 17;
			else if (c == '>') //Diferente
				estado = 18;
			else if (c == '=') //Menor Igual
				estado = 19;
			else
				estado = 20; //Menor
			break;

		case 17: ///Estado Atribuicao (FINAL)
			token[tk_atrib]++;
			zera_Lexema_E_Estado();
			break;

		case 18: ///Estado Diferente (FINAL)
			token[tk_diferente]++;
			zera_Lexema_E_Estado();
			break;

		case 19: ///Estado Menor Igual (FINAL)
			token[tk_menor_igual]++;
			zera_Lexema_E_Estado();
			break;

		case 20: ///Estado Menor (FINAL)
			token[tk_menor]++;
			zera_Lexema_E_Estado();
			break;

		case 21: ///Estado Igual (FINAL)
			token[tk_igual]++;
			zera_Lexema_E_Estado();
			break;

		case 22: ///Estado >
			c = leia_Proximo_Caractere();
			if (c == '=')
				estado = 23; //Maior Igual
			else
				estado = 24; //Maior
			break;

		case 23: ///Estado Maior Igual (FINAL)
			token[tk_maior_igual]++;
			zera_Lexema_E_Estado();
			break;

		case 24: ///Estado Maior (FINAL)
			token[tk_maior]++;
			zera_Lexema_E_Estado();
			break;

		case 25: ///Estado +
			c = leia_Proximo_Caractere();
			if (c == '+')
				estado = 26; //Incremento
			else
				estado = 27; //Mais
			break;

		case 26: ///Estado Incremento (FINAL)
			token[tk_incr]++;
			zera_Lexema_E_Estado();
			break;

		case 27: ///Estado Mais (FINAL)
			token[tk_mais]++;
			zera_Lexema_E_Estado();
			break;

		case 28: ///Estado -
			c = leia_Proximo_Caractere();
			if (c == '-')
				estado = 29; //Decremento
			else
				estado = 30; //Menos
			break;

		case 29: ///Estado Decremento (FINAL)
			token[tk_decr]++;
			zera_Lexema_E_Estado();
			break;

		case 30: ///Estado Menos (FINAL)
			token[tk_menos]++;
			zera_Lexema_E_Estado();
			break;

		case 31: ///Estado Vezes (FINAL)
			token[tk_vezes]++;
			zera_Lexema_E_Estado();
			break;
		
		case 32: ///Estado /
			c = leia_Proximo_Caractere();
			if (c == '*')
				estado = 33;
			else
				estado = 34;
			break;
			
		case 33: ///Estado Comentario (FINAL)
			while (c != '\n')
				c = leia_Proximo_Caractere();
			zera_Lexema_E_Estado();
			break;
			
		case 34: ///Estado Dividido (FINAL)
			token[tk_dividido]++;
			zera_Lexema_E_Estado();
			break;
			
		case 35: ///Comentario
			while (c != '*' && c != EOF)
				c = leia_Proximo_Caractere();
			if (c == '*') //Pode ser um fim de comentario
				estado = 36;
			else //Fim de arquivo
				estado = -5;
			break;
			
		case 36: ///Possivel fim de comentario
			while (c == '*') //ler proximo char, se * continua
				c = leia_Proximo_Caractere();
			if (c == ')') //Fim do comentario
				estado = 37;
			else if (c == EOF) //Fim de arquivo
				estado = -5;
			else //Não é fim de comentario
				estado = 35;
			break;
			
		case 37: ///Fim do comentario (FINAL)
			//printf("Fim comentario\n");
			zera_Lexema_E_Estado();
			break;
			
		case 38: ///Estado Abre Parenteses (FINAL)
			token[tk_abre_par]++;
			zera_Lexema_E_Estado();
			break;
		
		case 39: ///Palavra reservada token++ (FINAL)
			token[id_token]++;
			zera_Lexema_E_Estado();
			break;
			
		case -1:
			// Identifica Erro lexico (pode ou nao ser erro)
			// estado 2
			break;

		case -2:
			// Identifica Erro lexico
			// estado 5
			break;

		case -3:
			// Identifica Erro lexico
			// estado 7
			break;

		case -4:
			// Identifica Erro lexico(nao fechameno da cadeia)
			// estado 9
			break;
		
		case -5:
			// Identifica Erro lexico(nao fechameno do comentario)
			// Volta pro inicio e ignora ate o primeiro /n (estado 33)
			
			//Um comentário de bloco não fechado deve gerar o erro léxico “Comentário de bloco não fechado”. A análise
			//deve ser retomada no início da linha seguinte àquela onde o comentário foi aberto, efetivamente transfor-
			//mando-o em um comentário de linha válido, apesar de truncado.
			break;

		default:
			//Apresnta erro e ignora o caractere....
			break;
	}
}


int main (int argc, char *argv[]){	
	if (argc != 2){
		printf("Exemplo de execucao: ./Portugol prog01.ptg\nTente novamente\n");
		exit(1);
	}
	
	printf("Usando : %s \n", argv[1]);

	if ((Arquivo = fopen(argv[1], "r")) == NULL){
		printf("Erro ao abrir o arquivo!!!\n");
		exit(1);
	}
	
	while(Bool){
		//processa os elementos do arquivo
		automato();
	}
	
	fclose(Arquivo);
	printa_Resultado();
	return 0;
}


