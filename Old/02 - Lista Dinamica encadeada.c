#include <stdio.h>
#include <stdlib.h>

typedef struct aluno{
    int matricula;
    char nome[30];
    float n1,n2,n3;
}tAluno; //fazendo estrutura aluno para armazemar dados de um aluno

struct elemento{
    tAluno dados;
    struct elemento *prox;
}; //definindo que cada elemento da lista posui os dados de um aluno e aponta para o prox

typedef struct elemento* Lista; //definindo a cabeca q ira apontar para os elementos
typedef struct elemento Elem; //disendo q cada elemento se chamara Elem

//funcoes
Lista* CriaL();
void LiberaL(Lista*);
int ConsultaPOS(Lista*, int, tAluno*);
int ConsultaMAT(Lista*, int, tAluno*);
int InsereLf(Lista*, tAluno);
int InsereLi(Lista*, tAluno);
int InsereLo(Lista*, tAluno);
int RemoveL(Lista*, int);
int RemoveLi(Lista*);
int RemoveLf(Lista*);
int TamanhoL(Lista*);
int Lcheia(Lista*);
int Lvazia(Lista*);
void ImprimeL(Lista*);


int main(){
    Lista* li;
    int i;
    tAluno a[4] = {{2,"Andre",9.5,7.8,8.5},
                   {4,"Ricardo",7.5,8.7,6.8},
                   {1,"Bianca",9.7,6.7,8.4},
                   {3,"Ana",5.7,6.1,7.4}};
    li = CriaL();
    printf("Tamanho: %d\n\n\n",TamanhoL(li));

    for(i=0; i < 4; i++)
        InsereLo(li,a[i]);

    ImprimeL(li);
    printf("\nTamanho: %d\n\n\n",TamanhoL(li));

    for(i=0; i < 4; i++){
        RemoveLf(li);
        ImprimeL(li);
        printf("Tamanho: %d\n\n",TamanhoL(li));
    }

    for(i=0; i < 4; i++)
        InsereLo(li,a[i]);
    ImprimeL(li);

    LiberaL(li);
    system("pause");
    return 0;
}

//(*li) = o que ele aponta
// *li o que esta sendo apontado

Lista* CriaL(){
    Lista* li; //criando a cabeca da lista
    li = (Lista*) malloc(sizeof(Lista)); //fazendo a cabeca apontar bara alguem
    if(li != NULL) //foi bem sucedido?
        *li = NULL; //acabei de criar, portanto ela ta vazia
    return li;
}


void LiberaL(Lista* li){ //libera a lista por completo
    if(li != NULL){ //tem algum elemento na lista?
        Elem* aux; //criando ponteiro auxiliar para apontar para o elemento que eu quero remover
        while((*li) != NULL){ //minha cabeca aponta para o final (vazio, null?)
            aux = *li; //auxiliar vai apontar para o mesmo elemento que a minha cabeca aponta
            *li = (*li)->prox; //minha cabeca vai apontar para o o mesmo lugar que o o elemento prox que ela aponta apontar
            free(aux); //liberando o auxiliar (primeiro elemento da lista)
        }
        free(li); //toda memoria foi liberada
    }
}


int ConsultaPOS(Lista* li, int pos, tAluno *al){ //consulta por posição
    Elem *aux; //criando ponteiro auxiliar para apontar para o elemento que eu quero remover
    int i = 1;
    if(li == NULL || pos <= 0) //lista é valida? A posisao informada eh valida?
        return 0; // se for retorna falso
    aux = *li; //auxiliar vai apontar para o mesmo elemento que a minha cabeca aponta

    while(aux != NULL && i < pos){ //ou voce percorre a lista toda ou voce percorre ate o elemento, note q i comeca com 1 de primeiro elemento
        aux = aux->prox; //caso nao seja quem eu procure, auxiliar aponta para o proximo
        i++; //conta em qual elemento eu to
    }
    if(aux == NULL) // eu encontrei o elemento?
        return 0; //caso eu pecorri a lista toda e nao achei retorna falso
    else{
        *al = aux->dados; //os dados do elemento serao passados para um tAluno
        return 1; //tAluno é meu retorno com o dados, o 1 eu consegui fazer tudo
    }
}


int ConsultaMAT(Lista* li, int mat, tAluno *al){
    Elem *aux;
    if(li == NULL) //lista eh valida (possui elementos)?
        return 0;
    aux = *li; //auxiliar vai apontar para o mesmo elemento que a minha cabeca aponta
    while(aux != NULL && aux->dados.matricula != mat){ //cheguei no fim? achei a matricula?
        aux = aux->prox; //aponta para o prox elemento
    }
    if(aux == NULL) //caso eu nao achei a matricula
        return 0;
    else{
        *al = aux->dados; //os dados do elemento serao passados para um tAluno
        return 1; //tAluno é meu retorno com o dados, o 1 eu consegui fazer tudo
    }
}


int InsereLf(Lista* li, tAluno al){
    if(li == NULL) // a lista eh valida (esta vazia) ?
        return 0;
    Elem *no; //criando ponteiro para elemento a ser inserido
    no = (Elem*) malloc(sizeof(Elem)); // alocando elemento
    if(no == NULL) //consegui alocar?
        return 0;
    no->dados = al; //recebeos dados do aluno que eu mandei como parametro
    no->prox = NULL; //apontando pra null pq ele é no fim da lista
    if((*li) == NULL){//lista vazia: insere início
        *li = no;
    }else{
        Elem *aux; //criando um ponteiro auxiliar
        aux = *li; //apontar para o mesmo lugar q a cabeca aponta (inicio da lista)
        while(aux->prox != NULL){ //percorrer ate achar o ultimo elemento (o q aponta pra null)
            aux = aux->prox;
        }
        aux->prox = no; //quando eu encontrei, faço meu prox do ultimo elemento apontar pro no que eu quero inserir
    }
    return 1; // consegui fazer tudo
}


int InsereLi(Lista* li, tAluno al){
    if(li == NULL) // a lista eh valida ?
        return 0;
    Elem* no; //criando ponteiro para elemento a ser inserido
    no = (Elem*) malloc(sizeof(Elem)); // alocando elemento
    if(no == NULL) //consegui alocar?
        return 0;
    no->dados = al; //recebeos dados do aluno que eu mandei como parametro
    no->prox = (*li); //ele vai apontar pro mesmo que li aponta
    *li = no; //li agora aponta pro meu no
    return 1; // td certo
}


int InsereLo(Lista* li, tAluno al){
    if(li == NULL) // a lista eh valida?
        return 0;
    Elem* no; //criando ponteiro para elemento a ser inserido
    no = (Elem*) malloc(sizeof(Elem)); // alocando elemento
    if(no == NULL) //consegui alocar?
        return 0;
    no->dados = al;//recebeos dados do aluno que eu mandei como parametro

    if((*li) == NULL){//lista vazia: insere início e aponta pro null
        no->prox = NULL;
        *li = no;
        return 1;
    }
    else{
        Elem *ant, *atual; //criando auxiliares
        atual = *li; //atual aponta pro mesmo que lista (cabeca) aponta
        while(atual != NULL && atual->dados.matricula < al.matricula){ //percorrer ate o fim ou ate achar o dado maior do que o q eu quero inserir
            ant = atual;
            atual = atual->prox;
        }
        if(atual == *li){//insere início (atual nao pecorreu nada)
            no->prox = (*li); //aponta pro primeiro
            *li = no; //ele vira oprimeiro
        }else{
            no->prox = atual; //aponta pro que o autauta apontadno (insere antes dele)
            ant->prox = no; //o que tava antes do atual aponta pro q inseri
        }
        return 1; //consegui
    }
}


int RemoveL(Lista* li, int mat){
    if(li == NULL) // a lista eh valida?
        return 0;
    if((*li) == NULL)//lista vazia
        return 0;
    Elem *ant, *no; //criando ponteiros
    no = *li; //no aponta pro inicio da lista
    while(no != NULL && no->dados.matricula != mat){ //no procura quem ele quer ou vai at´´e o fim sem acahr
        ant = no; //anterior
        no = no->prox; //prox
    }
    if(no == NULL)//não encontrado
        return 0;

    if(no == *li)//remover o primeiro?
        *li = no->prox; //li passa a pontar pro proximo
    else
        ant->prox = no->prox; //o anterior aponta pro cara q ta dpois do no
    free(no); //remover o q eu quero
    return 1; //consegui
}


int RemoveLi(Lista* li){
    if(li == NULL)// a lista eh valida?
        return 0;
    if((*li) == NULL)//lista vazia
        return 0;

    Elem *no = *li; //criando ponteiro que aponta pro inicio da lista
    *li = no->prox; //faco  a cabeca apontar pro proximo elemento
    free(no); //elimino o peimeiro
    return 1;
}


int RemoveLf(Lista* li){
    if(li == NULL) // a lista eh valida?
        return 0;
    if((*li) == NULL)//lista vazia
        return 0;

    Elem *ant, *no = *li; //criando ponteiro que aponta pro inicio da lista, e criando o anterior a ele para auxiliar
    while(no->prox != NULL){ //percorro ate encontrar o ultimo
        ant = no;
        no = no->prox;
    }

    if(no == (*li))//remover o primeiro?
        *li = no->prox; //o que li aponta agora é o proximo
    else
        ant->prox = no->prox; //o anterior aponta para o proximo
    free(no); //libero memoria
    return 1;
}


int TamanhoL(Lista* li){
    if(li == NULL) //lista valida?
        return 0;
    int cont = 0; //criando um contador para informar tamanho
    Elem* aux; //auxiliar para verificar final
    aux = *li; //auxiliar aponta pro inicio
    while(aux != NULL){
        cont++; //somando
        aux = aux->prox; //prox
    }
    return cont; //quantidade
}


int Lcheia(Lista* li){ //a lista nunca vai encher
    return 0;
}


int Lvazia(Lista* li){
    if(li == NULL) //lista valida? existe?
        return 1;
    if(*li == NULL) //lista vazia
        return 1;
    return 0; //tem algum
}


void ImprimeL(Lista* li){ //imprime lista
    if(li == NULL) //lista valida?
        return;
    Elem* no = *li; //ponteiro que aponta pro inicio
    while(no != NULL){
        printf("Matricula: %d\n",no->dados.matricula);
        printf("Nome: %s\n",no->dados.nome);
        printf("Notas: %f %f %f\n",no->dados.n1,
                                   no->dados.n2,
                                   no->dados.n3);
        printf("-------------------------------\n");

        no = no->prox; //passa pro proximo
    }
}
