/* ------------------------------------------------------------- */
/*                    ARQUIVO: resultados.h                      */
/*                                                               */
/* Certifique-se que este arquivo não é incluído mais de uma vez */
/* ------------------------------------------------------------- */
#ifndef _RESULTADOS_H_ 
#define _RESULTADOS_H_

/* --------------------------- */
/* BIBLIOTECAS E INCLUDE FILES */
/* --------------------------- */
#include "tokens.h"
#include "automato.h"


/* ------------------------------- */
/*      PROTÓTIPOS DAS FUNÇÕES     */
/* ------------------------------- */
char reconhecer_Proximo_Simbolo(void);
int imprimir_Linha(FILE *);
void imprimir_Seta(FILE *, const int);
void imprimir_Lista_De_Erros_Lexicos(const char*);
tSimbolo * procurar_Lexema(const tIndentificador_De_Token meuToken);
void imprimir_Lista_De_Tokens_Reconhecidos_E_Resumo(const char*, int);
int obter_Tamanho_Do_Maior_Nome_Token_Reconhecido(void);
int obter_Tamanho_Do_Maior_Lexema(void);
int obter_Tamanho_Do_Maior_Nome_Token_Com_Lexema(void);
int obter_Tamanho_Da_Maior_Qtd_De_Ocorrencias(void);
void imprimir_Tabela_De_Simbolos(const char*);


/* ------------------------------- */
/*    IMPLEMENTAÇÃO DAS FUNÇÕES    */
/* ------------------------------- */
char reconhecer_Proximo_Simbolo(void){
	char prox_Simb = getc(arquivo_de_entrada);
	fseek(arquivo_de_entrada, -sizeof(char), SEEK_CUR);
	return prox_Simb;
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
			if (reconhecer_Proximo_Simbolo() == EOF)
				break;
			fprintf(arquivo_de_saida, "[%4d]", linha_arquivo);
			carcteres_na_linha = imprimir_Linha(arquivo_de_saida);
			if (lista_de_erros.id_erro[i].LIN == linha_arquivo-1 && i < lista_de_erros.tamanho_lista){
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


tSimbolo * procurar_Lexema(const tIndentificador_De_Token meuToken){
	tSimbolo * simb = tab_simbolos[meuToken.posisao_na_tabela_de_simbolos];
	while (simb != NULL) {
		if (simb->COD == meuToken.TOKEN) {
			for(int i=0; i < simb->tamanho_ocorrencias; i++){
				if (simb->ocorrencias[i].LIN == meuToken.LIN && simb->ocorrencias[i].COL == meuToken.COL)
					return simb;
			}
		}
		simb = simb->proximo;
	}
   	return NULL;
}


void imprimir_Lista_De_Tokens_Reconhecidos_E_Resumo(const char* nomeArquivoEntrada, int qtdErros){
	int resumo[QUANTIDADE_DE_TOKENS] = {0};
	int token_max = obter_Tamanho_Do_Maior_Nome_Token_Reconhecido();
	int lexema_max = obter_Tamanho_Do_Maior_Lexema();
	int linha_Da_Vez = 0;
	
	FILE *arquivo_de_saida;
	char * nome_arquivo;
	
	nome_arquivo = (char *) malloc(4 + strlen(nomeArquivoEntrada)); // Alocação para o nome do arquivo
	sprintf(nome_arquivo,"%s.tok", nomeArquivoEntrada); // O arquivo esta na pasta testes
	
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
				fprintf(arquivo_de_saida,"%-*s |              |\n", lexema_max, " ");
			} else{
				tSimbolo * simb = procurar_Lexema(lista_de_tokens.id_token[i]);
				
				if (simb->COD == tk_INTEIRO){
					fprintf(arquivo_de_saida,"%-*d |     %3d      |\n", lexema_max, simb->lexema_inteiro, lista_de_tokens.id_token[i].posisao_na_tabela_de_simbolos);
				}else if(simb->COD == tk_DECIMAL){
					int aux = strlen(simb->lexema_cadeia);
					int qtd_apos = 1, qtd_antes = 0;
					for (int j=0; j<aux; j++){
						qtd_antes++; if (simb->lexema_cadeia[j]=='.') break;
					}
					qtd_apos = aux - qtd_antes;
					if (qtd_apos == 0) qtd_apos++;
					if (qtd_apos >= lexema_max-1) qtd_apos--;
					fprintf(arquivo_de_saida,"%-*.*f |     %3d      |\n", lexema_max, qtd_apos, simb->lexema_decimal, lista_de_tokens.id_token[i].posisao_na_tabela_de_simbolos);
				}else{
					fprintf(arquivo_de_saida,"%-*s |     %3d      |\n", lexema_max,  simb->lexema_cadeia, lista_de_tokens.id_token[i].posisao_na_tabela_de_simbolos);
				}
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
		fprintf(arquivo_de_saida,"TOTAL DE ERROS: %d\n\n", qtdErros);
		
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
	sprintf(nome_arquivo,"%s.tbl", nomeArquivoEntrada); // O arquivo esta na pasta testes
	
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
				int aux = strlen(ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->lexema_cadeia);
				int qtd_apos = 1, qtd_antes = 0;
				for (int j=0; j<aux; j++){
					qtd_antes++; if (ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->lexema_cadeia[j]=='.') break;
				}
				qtd_apos = aux - qtd_antes;
				if (qtd_apos == 0) qtd_apos++;
				
				fprintf(arquivo_de_saida,"%-*.*f", lexema_max, qtd_apos, ordem_de_entrada.ordem_de_entrada_da_tab_simbolos[i]->lexema_decimal);
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

#endif 
