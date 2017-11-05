/* ------------------------------------------------------------- */
/*                     ARQUIVO: Portugol.c                       */
/* ------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lexema.h"
#include "tokens.h"
#include "automato.h"
#include "erros.h"
#include "resultados.h"


///FUNCAO PRINCIPAL
int main (int argc, char *argv[]){
	tToken token_da_vez;
	
	if (argc < 2){
		printf("Exemplo de execucao: ./Portugol prog01.ptg\nTente novamente\n");
	} else {
		iniciar_Lexema();
		// Roda os arquivos que sao digitados na entrada
		for (int i=1; i < argc; i++){ 
			printf("\nArquivo: %s \n", argv[i]);
			
			if ((arquivo_de_entrada = fopen(argv[i], "r")) == NULL){
				printf("Erro ao abrir o arquivo!!! \nPor favor, verifique a existencia do mesmo e tente novamente.\n");
			} else {
				//Inicia listas de tokens, erros, tabelas e arquivo
				iniciar_Lista_De_Erros();
				iniciar_Lista_De_Tokens();
				iniciar_Tabela_de_Simbolos();
				iniciar_Ordem_Tab();
				linha_arquivo = 1, coluna_arquivo = 1;
				
				//Recebe tokens
				do {
					token_da_vez = analizador_Lexico();
					adicionar_Token_Na_Lista_De_Tokens(token_da_vez, linha_token, coluna_token);
				} while(token_da_vez != tk_EOF);

				//Imprime nos arquivos
				imprimir_Lista_De_Erros_Lexicos(argv[i]);
				imprimir_Lista_De_Tokens_Reconhecidos_E_Resumo(argv[i], lista_de_erros.tamanho_lista);
				imprimir_Tabela_De_Simbolos(argv[i]);
				
				printf("Os seguintes arquivos gerados:\n");
				printf("   %s.err com o conteúdo do arquivo de entrada e os erros léxicos devidamente marcados\n", argv[1]);
				printf("   %s.tbl com a lista de tokens reconhecidos\n", argv[1]);
				printf("   %s.tok com o conteúdo da tabela de símbolos após processamento.\n\n", argv[1]);
				
				//Libera Memoria
				free(lista_de_erros.id_erro);
				free(lista_de_tokens.id_token);
				free(ordem_de_entrada.ordem_de_entrada_da_tab_simbolos);
				liberar_Tabela_Simbolos();
				
				//Fecha Arquivo de entrada
				fclose(arquivo_de_entrada);
			}
		}
		//Libera Memoria para tString lexema
		free(lexema.string);
	}
	return 0;
}
