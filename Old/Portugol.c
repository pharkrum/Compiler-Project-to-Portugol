#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "Codigo/tokens.h"
#include "Codigo/automato.h"
#include "Codigo/erros.h"
#include "Codigo/tabSimbolos.h"
#include "Codigo/resultados.h"


#define TAMANHO_INICIAL 100


FILE *arquivo_de_entrada;


///FUNCAO PRINCIPAL
int main (int argc, char *argv[]){
	tToken_resultante token_da_vez;
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
	
	do {
		//token_da_vez[n] = analizador_Lexico();
		//n++;
	} while((token_da_vez.COD - 1) != tk_EOF);

	
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


