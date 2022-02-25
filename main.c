#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Lista encadeada
FILE *arq;

//Armazena nome e frequência do arquivo
typedef struct {
    char nome[20];
    int frequencia;
} arquivo;

//Estrutura do nó da Lista Encadeada
typedef struct {
    arquivo dado;
    struct nodo *prox;
} nodo;

//Armazena o topo da Lista
typedef struct {
    nodo *prim;
} ListaArquivos;

//Inicia a Lista
void iniciar(ListaArquivos *lista) {
    lista->prim = NULL;
}

//Método de ordenação da Lista
void ordenacao(ListaArquivos *li) {

    if(li->prim != NULL){
        for(nodo *i = li->prim; i->prox != NULL; i = i->prox){
            nodo *menor = i;
            for(nodo *j = i->prox; j != NULL; j = j->prox){
                if(j->dado.frequencia > menor->dado.frequencia){
                    menor = j;
                }
            }

            arquivo aux = i->dado;
            i->dado = menor->dado;
            menor->dado = aux;
        }
    }

}

//Método de impressão da Lista
void imprimir(ListaArquivos *li) {
    nodo *no = li->prim;
    while(no != NULL){
        printf("Arquivo \"%s\"  Ocorrencia: %i\n", no->dado.nome, no->dado.frequencia);

        no = no->prox;
    }

}

//Procura o nome de um arquivo na Lista. Retorna o Nó coincidente
nodo *procurar_arquivo(ListaArquivos *li, char nome_arquivo[20]) {
    nodo *no = li->prim;

    while(no != NULL){
        if(strcmp(nome_arquivo, no->dado.nome) == 0){
            return no;
            break;
        }
        no = no->prox;
    }
    return no; //se arquivo não for encontrado, no = NULL
}

//Método de inserção na Lista
//Caso o arquivo já exista, a frequência do arquivo referenciado no parâmetro é SOMADA ao nó correspondente
void insereLista(ListaArquivos *desc, arquivo dado) {
    nodo *novo = malloc(sizeof(nodo));

    novo->dado = dado;
    nodo *topo = desc->prim;
    nodo *no = desc->prim;

    int n;
    nodo *encontrado = procurar_arquivo(desc, dado.nome);


    if(topo != NULL) {
        if(encontrado == NULL){
            novo->prox = topo;
            desc->prim = novo;
        } else {
            n = encontrado->dado.frequencia;
            n = n + dado.frequencia;
            encontrado->dado.frequencia = n;
        }
    } else {
        novo->prox = NULL;
        desc->prim = novo;
    }
    ordenacao(desc);
}

//Retorna o tamanho da lista
int tamanho_lista(nodo *topo){
    int tam;
    nodo *no = topo->prox;
    while(no != NULL){
        tam++;
        no = no->prox;
    }
    return tam;
}


//Árvore Binária

//Estrutura do NO da Árvore
typedef struct {
    char dado[50]; //termo da busca
    ListaArquivos *lista; //uma lista por nodo
    struct NoArvore *esq;
    struct NoArvore *dir;
} NoArvore;


//Estrutura que armaza a RAIZ da Árvore
typedef struct {
    NoArvore *raiz;

} ArvoreBinariaBusca;

//Método que inicializa o nó da Árvore
NoArvore *criaNoArvore(char dado[20], arquivo arqui){
    NoArvore *n = malloc(sizeof(NoArvore));

    if(n == NULL){
        exit(1);
    } else {
        strcpy(n->dado, dado);//termo de busca
        n->esq = NULL;
        n->dir = NULL;


        ListaArquivos *lista = malloc(sizeof(ListaArquivos));
        iniciar(lista);
        n->lista = lista;
        insereLista(n->lista, arqui);
    }
    return n;
}

//Método que insere nós na Árvore
void InserirArvore(ArvoreBinariaBusca *arvore, NoArvore *novo){
    NoArvore *no = NULL;               //Armazena nó folha
    NoArvore *atual = arvore->raiz;    //Nó seguinte ao nó folha

    while(atual != NULL){              //Procura o nó folha
        no = atual;
        if(strcmp(novo->dado, atual->dado) < 0){
            atual = atual->esq;
        } else if(strcmp(novo->dado, atual->dado) > 0){
            atual = atual->dir;
        } else {
            break;
        }
    }


    if(no == NULL){
        arvore->raiz = novo;                       //Atualiza a raíz com o NOVO nó se for nula
        } else {                                   //Insere o NOVO nó ao nó folha
        if(strcmp(novo->dado, no->dado) < 0){
            no->esq = novo;
        } else if(strcmp(novo->dado, no->dado) > 0){
            no->dir = novo;
        } else {
            nodo *aux = novo->lista->prim;          //Caso o nó já exista, insere-se os dados do NOVO nó à lista do nó existente
            arquivo arqui = aux->dado;
            insereLista(no->lista, arqui);
            free(novo);
        }
    }
}

//Método que imprime Árvore Em Ordem
void imprimirEmOrdem(ArvoreBinariaBusca *arvore){
    NoArvore *no = arvore->raiz;
    if(no != NULL){
        imprimirEmOrdem(no->esq);
        //printf("%s ", no->dado);
        return no->dado;
        imprimirEmOrdem(no->dir);
    }
}

//Buscador
typedef struct {
    ArvoreBinariaBusca *arvoreArquivos;

}Buscador;

//Método que busca por um TERMO na Árvore
//Retorna a Lista Encadeada do nó em caso de coincidência
ListaArquivos *busca(char termo[20], ArvoreBinariaBusca *arvore){
        NoArvore *no = NULL;
        NoArvore *atual = arvore->raiz;

        while(atual != NULL){
            no = atual;
            if(strcmp(termo, no->dado) < 0){
                atual = atual->esq;
            } else if(strcmp(termo, no->dado) > 0){
                atual = atual->dir;
            } else {
                return no->lista;
                break;
            }
        }

        if(atual == NULL){      //se não for encontrado um valor
            return NULL;
        }
    }

//Método que insere as palavras do arquivo especificado na árvore
void InsereDados(ArvoreBinariaBusca* arvore, char nome_arquivo[20]){
    char palavra[20];
    arq = fopen(nome_arquivo, "r");

    while(!feof(arq)){
        fscanf(arq, "%s ", palavra);

        arquivo dado;
        strcpy(dado.nome, nome_arquivo);
        dado.frequencia = 1;

        InserirArvore(arvore, criaNoArvore(palavra, dado));
    }
    fclose(arq);
}


int main()
{
    //Criando e inserindo dados na árvore
    ArvoreBinariaBusca *arvore = malloc(sizeof(ArvoreBinariaBusca));
    arvore->raiz = NULL;

    InsereDados(arvore, "arquivo.txt");
    InsereDados(arvore, "arquivo1.txt");
    InsereDados(arvore, "arquivo2.txt");
    InsereDados(arvore, "arquivo3.txt");
    InsereDados(arvore, "arquivo4.txt");

    //Busca
    int ntermos;
    printf("Quantos termos deseja pesquisar? ");
    scanf("%i", &ntermos);
    char termos[ntermos][20];

    printf("Entre com os termos a ser pesquisados (separados por espaco): ");
    for(int i = 0; i < ntermos; i++){
        scanf("%s[^ ]", &termos[i]);
    }

    ListaArquivos *resultado = malloc(sizeof(ListaArquivos));
    iniciar(resultado);


    for(int i = 0; i < ntermos; i++){
        ListaArquivos *atual = busca(termos[i], arvore);         //Captura a lista do nó da Árvore que coincide com a busca

        if(atual == NULL){
            printf("\nTermo '%s' nao encontrado em nenhum arquivo!", termos[i]);
        } else {
            nodo *noatual = atual->prim;
            while (noatual != NULL){                            //Itera pela lista encontrada e adiciona cada termo à lista "resultado"
                arquivo dado = noatual->dado;
                insereLista(resultado, dado);
                noatual = noatual->prox;
            }
        }
    }

    printf("\nOs arquivos encontrados foram:\n\n");
    printf("---------------\n");
    imprimir(resultado);
    printf("---------------\n");



    free(resultado);
    free(arvore);

}
