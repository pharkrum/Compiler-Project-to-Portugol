#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**Potências de dois devem ser evitadas: deve ser um número primo distante de pequenas potências de dois**/
struct aluno{
    int matricula;
    char nome[30];
    float n1,n2,n3;
};

/**Definição do tipo Hash**/
typedef struct hash{
    int qtd, TABLE_SIZE;
    struct aluno **itens; //crio um vetor com o indereco do elemento da tabela
    //nesse caso ela so vai conter o endereco para a estrutura aluno
} Hash;


/**FUNCOES DEFINICAO**/
Hash* criaHash(int TABLE_SIZE);
void liberaHash(Hash* ha);
int valorString(char *str);
int chaveDivisao(int chave, int TABLE_SIZE);
int chaveDobra(int chave, int TABLE_SIZE);
int chaveMultiplicacao(int chave, int TABLE_SIZE);
int insereHash_SemColisao(Hash* ha, struct aluno al);
int buscaHash_SemColisao(Hash* ha, int mat, struct aluno* al);
int sondagemLinear(int pos, int i, int TABLE_SIZE);
int sondagemQuadratica(int pos, int i, int TABLE_SIZE);
int duploHashing(int pos, int chave, int i, int TABLE_SIZE);
int insereHash_EnderAberto(Hash* ha, struct aluno al);
int buscaHash_EnderAberto(Hash* ha, int mat, struct aluno* al);


/**TESTE MAIN**/
int main(){
    int tamanho = 1024; //potencia de 2
    Hash *tabela = criaHash(tamanho);

    struct aluno al, a[4] = {{12352,"Andre",9.5,7.8,8.5},//64
                         {7894,"Ricardo",7.5,8.7,6.8},//726
                         {3451,"Bianca",9.7,6.7,8.4},//379
                         {5293,"Ana",5.7,6.1,7.4}};//173

    int i;
    for(i=0; i < 4; i++){
        insereHash_SemColisao(tabela,a[i]);
    }

    printf("------------\n\n");

    buscaHash_SemColisao(tabela, 12352, &al);
    printf("%s, %d\n", al.nome, al.matricula);

    buscaHash_SemColisao(tabela, 3451, &al);
    printf("%s, %d\n", al.nome, al.matricula);

    buscaHash_SemColisao(tabela, 5293, &al);
    printf("%s, %d\n", al.nome, al.matricula);

    liberaHash(tabela);
    return 0;
}


/**FUNCOES**/
Hash* criaHash(int TABLE_SIZE){
    Hash* ha = (Hash*) malloc(sizeof(Hash)); //aloca memoria pra stuct hash
    if(ha != NULL){ //se consegui alocar
        int i;
        ha->TABLE_SIZE = TABLE_SIZE; //associo o tamanho int q defini ao campo tamanho da estrutura q aloqui
        ha->itens = (struct aluno**) malloc(TABLE_SIZE * sizeof(struct aluno*)); //inicializo ponteiros itens (**)
        if(ha->itens == NULL){ //se deu erro na alocacao dos items
            free(ha); //libero o primeiro ponteiro
            return NULL; //retorno null pra dizer q nao existe tabela
        }
        //consegui alocar? entao faz:
        ha->qtd = 0; //nao ha elementos nela ainda
        for(i=0; i < ha->TABLE_SIZE; i++) //tenho q zerar tudo
            ha->itens[i] = NULL; //NULL pq nao ha nada guardado
    }
    return ha; //retorna o ponteiro q aloquei
}

void liberaHash(Hash* ha){
    if(ha != NULL){ //se ela existir faz
        int i;
        for(i=0; i < ha->TABLE_SIZE; i++){ //percorro todas as posicoes
            if(ha->itens[i] != NULL) // se aquela posição tiver alguem
                free(ha->itens[i]); //desaloca a posicao q tiver alguem
        }
        free(ha->itens); //desaloca vetor da tabela
        free(ha); //desaloca struct q foi criada e q continha a tabela hash
    }
}


/**Hashing - Definem a posicao onde vao ficar os dados - depende do dado**/
int valorString(char *str){ //converte string para int
    //calcular valor numerico a partir de valores ascii, considerando posicao da letra
    int i, valor = 7; //7 eh estudo
    int tam = strlen(str);
    for(i=0; i < tam; i++)//somo todos os caracteres
        valor = 31 * valor + (int) str[i]; //31 ta relacionado a pesquisas
    return (valor & 0x7FFFFFFF); //evita over flow
}

int chaveDivisao(int chave, int TABLE_SIZE){
    //Hash Divisao: calcula o % do interio q representa o elemento peo tamanho da tabela
    //int chave = valorString(nome);
    //pos = chaveDivisao(chave, TABLE_SIZE)
    return (chave & 0x7FFFFFFF) % TABLE_SIZE; //e bit-abit pra evitar overflow e numeros negativos
}

int chaveMultiplicacao(int chave, int TABLE_SIZE){
    //Hash Multiplicação: usa )<A<1 para multiplicar chave do elemento, a parte fracionaria e multiplicada pelo tamanho da tabela para saber posição
    //int chave = valorString(nome);
    //pos = chaveMultiplicacao(chave, TABLE_SIZE)
    double A = 0.6180339887; // constante: 0 < A < 1
    double val = chave * A;
    val = val - (int) val;
    return (int) (TABLE_SIZE * val);
}

int chaveDobra(int chave, int TABLE_SIZE){
    //Hash Dobra: dobrar e somar digitos do valor para calcular posicao, apos dobrar na metade, soma os sobrepostos, ate achar valor menor q tamanho da tabela
    //int chave = valorString(nome);
    //pos = chaveDobra(chave, TABLE_SIZE)
    int num_bits = 10; //dobro a cada 10 bits
    int parte1 = chave >> num_bits; //pego a chave e desloco 10 bits
    int parte2 = chave & (TABLE_SIZE-1); //faco um e bit a bit
    return (parte1 ^ parte2); //faco um ou exclusivo enre as duas partes
}


/**Insere e busca sem tratamento de colisão**/
int insereHash_SemColisao(Hash* ha, struct aluno al){
    if(ha == NULL || ha->qtd == ha->TABLE_SIZE) //verifico se a tabela eh valida ou se ta cheia
        return 0;

    int chave = al.matricula; //chave é a matricula
    //int chave = valorString(al.nome); //a chave tambem poderia ser o nome

    int pos = chaveDivisao(chave,ha->TABLE_SIZE); //calcula a posição (exemplo divisao)
    struct aluno* novo; //novo pontiro
    novo = (struct aluno*) malloc(sizeof(struct aluno)); //aloco p guardar
    if(novo == NULL) //deu certo?
        return 0; //nao
    *novo = al; //copio os dados do aluino pro novo ponteiro
    ha->itens[pos] = novo; //coloco o novo aluno dentro da posicao pos(calculada) da tabela
    ha->qtd++; //inseri ok
    return 1; //tudo certo
}

int buscaHash_SemColisao(Hash* ha, int mat, struct aluno* al){
    if(ha == NULL) //hash eh valido?
        return 0;
    //se valido, faz:
    int pos = chaveDivisao(mat,ha->TABLE_SIZE); //calculo posicao
    if(ha->itens[pos] == NULL) //acesso a posicao, se nao existir tem q dar erro
        return 0;
    *al = *(ha->itens[pos]); //se existir copio os dados
    return 1; //ok
}


/** Insere e busca com tratamento de colisão: Endereçamento Aberto**/
int sondagemLinear(int pos, int i, int TABLE_SIZE){
    //problema: agrupamento primario
    return ((pos + i) & 0x7FFFFFFF) % TABLE_SIZE; //tenta espalhar de forma sequncial(pos+i) - sendo i a qtd de elementos colididos(tentativa) - Garantindo valores positivos 0x7F
}

int sondagemQuadratica(int pos, int i, int TABLE_SIZE){
    pos = pos + 2*i + 5*i*i; // hash + (c1 * i) + (c2 * i^2)
    //probleba: agrupamento secundario
    //tento espalhar de acordo com uma funcao de 2grau - i é o numero de tentativas
    return (pos & 0x7FFFFFFF) % TABLE_SIZE; //tratamento de oiverflow e circularidade
}

int duploHash(int H1, int chave, int i, int TABLE_SIZE){
    //duas funcoes hashing sao usadas para calcular
    //h2 usado pra caso de colisao
    int H2 = chaveDivisao(chave,TABLE_SIZE-1) + 1;
    return ((H1 + i*H2) & 0x7FFFFFFF) % TABLE_SIZE; //tratamento overflow e circular
}

int insereHash_EnderAberto(Hash* ha, struct aluno al){
    if(ha == NULL || ha->qtd == ha->TABLE_SIZE) //verifica se eh valida ou esta cheia
        return 0;

    int chave = al.matricula; //usando matricula como chave
    //int chave = valorString(al.nome);

    int i, pos, newPos;
    pos = chaveDivisao(chave,ha->TABLE_SIZE); //metodo da divisao pra calcular posicao inicial
    for(i=0; i < ha->TABLE_SIZE; i++){ //fazer quantas tentativas qunato tamanho da tabela
        ///Procura nova posicao a partir da sondagem escolhoida
        newPos = sondagemLinear(pos, i, ha->TABLE_SIZE);
        ///newPos = sondagemQuadratica(pos,i,ha->TABLE_SIZE);
        ///newPos = duploHash(pos,chave,i,ha->TABLE_SIZE);

        if(ha->itens[newPos] == NULL){ //se nao existir ngm onde quero colocar faço:
            struct aluno* novo; //crio um novo aluno
            novo = (struct aluno*) malloc(sizeof(struct aluno)); //aloco
            if(novo == NULL) //tratamento de erro
                return 0;
            *novo = al; //copio os dados
            ha->itens[newPos] = novo; //guardo o cara q aloqui na posicao escolhida
            ha->qtd++; //adicionei
            return 1; //fim, saida
        }
    } //caso ainda nao achei
    return 0; //nunca consegui inserir: falha
}

int buscaHash_EnderAberto(Hash* ha, int mat, struct aluno* al){
    if(ha == NULL) //Hash valide?
        return 0;

    int i, pos, newPos;
    pos = chaveDivisao(mat, ha->TABLE_SIZE); //calcula posicao da chave
    for(i=0; i < ha->TABLE_SIZE; i++){ //faco isso o max de vezes posivel:
        ///newPos = sondagemLinear(pos, i, ha->TABLE_SIZE); //recalcula posicao da chave
        ///newPos = sondagemQuadratica(pos,i,ha->TABLE_SIZE);
        newPos = duploHash(pos,mat,i,ha->TABLE_SIZE);
        if(ha->itens[newPos] == NULL) //verifico se ha dados na posicao calculada
            return 0; //falha na busca

        if(ha->itens[newPos]->matricula == mat){ //se a posicao for valida, esses dados combinam com achave?
            *al = *(ha->itens[newPos]); //se sim, copio o valor
            return 1; //ok
        }
    } //caso ainda nao achei
    return 0; //erro
}
