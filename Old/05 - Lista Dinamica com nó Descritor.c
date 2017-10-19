#include <stdio.h>
#include <stdlib.h>

typedef struct aluno{
    int matricula;
    char nome[30];
    float n1,n2,n3;
}tAluno;

typedef struct elemento{
    struct aluno dados;
    struct elemento *prox;
}Elem;//Definição do tipo lista

typedef struct descritor{
    struct elemento *inicio;
    struct elemento *final;
    int qtd;
}Lista;//Definição do Nó Descritor


Lista* cria_lista();
void libera_lista(Lista* li);
int insere_lista_final(Lista* li, struct aluno al);
int insere_lista_inicio(Lista* li, struct aluno al);
int remove_lista_inicio(Lista* li);
int remove_lista_final(Lista* li);
int tamanho_lista(Lista* li);
int lista_vazia(Lista* li);
int lista_cheia(Lista* li);
int consulta_lista_mat(Lista* li, int mat, struct aluno *al);
int consulta_lista_pos(Lista* li, int pos, struct aluno *al);
void imprime_lista(Lista* li);


int main(){
    struct aluno a1,a[4] = {{2,"Andre",9.5,7.8,8.5},
                            {4,"Ricardo",7.5,8.7,6.8},
                            {1,"Bianca",9.7,6.7,8.4},
                            {3,"Ana",5.7,6.1,7.4}};
    Lista* li = cria_lista();
    printf("Tamanho: %d\n\n\n\n",tamanho_lista(li));

    int i;
    for(i=0; i < 4; i++)
        insere_lista_final(li,a[i]);

    imprime_lista(li);
    printf("\nTamanho: %d\n\n\n",tamanho_lista(li));


    for(i=0; i < 4; i++){
        if (consulta_lista_pos(li, i+1, &a1))
            printf("Ok: %s\n",a1.nome);
        else
            printf("Erro: %s\n",a1.nome);

    }

    printf("\n\n======================\n\n");

    for(i=1; i <= 6; i++){
        if(!remove_lista_final(li))
            printf("Erro!!!!!!!!!\n");
        imprime_lista(li);
        printf("\n\n======================\n\n");
    }

    for(i=0; i < 2; i++)
        insere_lista_final(li,a[i]);

    imprime_lista(li);

    libera_lista(li);
    system("pause");
    return 0;
}


Lista* cria_lista(){
    Lista* li = (Lista*) malloc(sizeof(Lista)); //cria e aloca ponteiro para no descritor
    if(li != NULL){ //consegui alocar
        li->inicio = NULL; //td mundo eh nul e nao tem elemento
        li->final = NULL;
        li->qtd = 0;
    }
    return li;
}

void libera_lista(Lista* li){
    if(li != NULL){ //lista valida?
        Elem* no; //cria no auxiliar pra remover
        while((li->inicio) != NULL){ //inicio da lista == null?
            no = li->inicio; //no aponta pro inicio
            li->inicio = li->inicio->prox; //inicio agr eh prox
            free(no); // libera elemento
        }
        free(li); //libera a lista vazia
    }
}

int tamanho_lista(Lista* li){
    if(li == NULL) //valida
        return 0;
    return li->qtd; //ponto pro descritor
}

int lista_cheia(Lista* li){
    return 0; //nunca vai encher
}

int lista_vazia(Lista* li){
    if(li == NULL) //valida?
        return 1;
    if(li->inicio == NULL) //vazia?
        return 1;
    return 0;//tem algum
}

int insere_lista_inicio(Lista* li, struct aluno al){
    if(li == NULL) //lista valida?
        return 0;
    Elem* no; //cria elemento pra isnerir
    no = (Elem*) malloc(sizeof(Elem)); //aloca o ponteiro
    if(no == NULL) //consegui alocar?
        return 0;
    no->dados = al; //recebe os dados
    no->prox = li->inicio; //ele aponta pro inicio agora
    if(li->inicio == NULL)
        li->final = no; //so tem esse elemento
    li->inicio = no; //insere no inicio
    li->qtd++; //ind=serido qtd +1
    return 1;//ok
}

int insere_lista_final(Lista* li, struct aluno al){
    if(li == NULL) //lista valida?
        return 0;
    Elem *no; //cria elemento para inserir
    no = (Elem*) malloc(sizeof(Elem)); //aloca ponteiro
    if(no == NULL) //consegui alocar?
        return 0;
    no->dados = al; //recebe dados
    no->prox = NULL; //ele deve ser o ultimo, portanto aponta pro final
    if(li->inicio == NULL)//lista vazia: insere início
        li->inicio = no; //so tem esse elemento
    else
        li->final->prox = no; //final agora eh o no

    li->final = no; //no agora eh o fim
    li->qtd++; //inseri
    return 1; //ok
}

int remove_lista_inicio(Lista* li){
    if(li == NULL) //lista valida?
        return 0;
    if(li->inicio == NULL)//lista vazia
        return 0;

    Elem *no = li->inicio; //crio um auxiliar q aponta pro inicio
    li->inicio = no->prox; //inicio agr eh prox
    free(no); //libera o auxiliar
    if(li->inicio == NULL) //ficou vazia?
        li->final = NULL;
    li->qtd--; //removido
    return 1;
}

int remove_lista_final(Lista* li){
    if(li == NULL) //lista valida?
        return 0;
    if(li->inicio == NULL)//lista vazia
        return 0;

    Elem *ant, *no = li->inicio; //crio um no que vai percorrer ate o fim e um anterior para ser o ultimo quando liberar o no
    while(no->prox != NULL){ //percorrendo ate o fim
        ant = no;
        no = no->prox;
    }
    if(no == li->inicio){//remover o primeiro?
        li->inicio = NULL; //nao ha mais ngm
        li->final = NULL;
    }else{ //qualquer outro
        ant->prox = no->prox; //pula o no
        li->final = ant; //final agora eh o ant
    }
    free(no); //libera o no
    li->qtd--; //removi
    return 1; //ok
}

int consulta_lista_pos(Lista* li, int pos, struct aluno *al){
    if(li == NULL || li->inicio == NULL || pos <= 0) //lista valida? lista vazia? posicao valida?
        return 0;
    Elem *no = li->inicio;//primeiro elemento
    int i = 1; //contador ate a pos
    while(no != NULL && i < pos){ //cheguei no fim, achei a posiçao?
        no = no->prox; //pulo o no
        i++; //verifiquei
    }
    if(no == NULL) //nao achei
        return 0;
    else{ //achei
        *al = no->dados; //al recebe os dados do no
        return 1; //ok
    }
}

int consulta_lista_mat(Lista* li, int mat, struct aluno *al){
    if(li == NULL || li->inicio == NULL) //lista valida? lista vazia?
        return 0;
    Elem *no = li->inicio; //crio um aux q aponta pro inicio
    while(no != NULL && no->dados.matricula != mat) //percorrendo ate o final ou ate achar a mat
        no = no->prox; //pulando no verificado
    if(no->dados.matricula != mat) //nao achei
        return 0;
    else{//achei
        *al = no->dados; //al recebe dados do no com a mat
        return 1; //ok
    }
}

void imprime_lista(Lista* li){
    if(li == NULL || li->inicio == NULL) //lista valida ou vazia?
        return;
    Elem* no = li->inicio; //ponteiro q aponta pro inicio
    while(no != NULL){ //nao cheguei no fim ainda
        printf("Matricula: %d\n",no->dados.matricula);
        printf("Nome: %s\n",no->dados.nome);
        printf("Notas: %f %f %f\n",no->dados.n1,
                                   no->dados.n2,
                                   no->dados.n3);
        printf("-------------------------------\n");
        no = no->prox;
    }
}
