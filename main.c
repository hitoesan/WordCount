#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Lista encadeada
FILE *arq;

//Armazena nome e frequ�ncia do arquivo
typedef struct {
    char nome[20];
    int frequencia;
} arquivo;

//Estrutura do n� da Lista Encadeada
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

//M�todo de ordena��o da Lista
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

//M�todo de impress�o da Lista
void imprimir(ListaArquivos *li) {
    nodo *no = li->prim;
    while(no != NULL){
        printf("Arquivo \"%s\"  Ocorrencia: %i\n", no->dado.nome, no->dado.frequencia);

        no = no->prox;
    }

}

//Procura o nome de um arquivo na Lista. Retorna o N� coincidente
nodo *procurar_arquivo(ListaArquivos *li, char nome_arquivo[20]) {
    nodo *no = li->prim;

    while(no != NULL){
        if(strcmp(nome_arquivo, no->dado.nome) == 0){
            return no;
            break;
        }
        no = no->prox;
    }
    return no; //se arquivo n�o for encontrado, no = NULL
}

//M�todo de inser��o na Lista
//Caso o arquivo j� exista, a frequ�ncia do arquivo referenciado no par�metro � SOMADA ao n� correspondente
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


//�rvore Bin�ria

//Estrutura do NO da �rvore
typedef struct {
    char dado[50]; //termo da busca
    ListaArquivos *lista; //uma lista por nodo
    struct NoArvore *esq;
    struct NoArvore *dir;
} NoArvore;


//Estrutura que armaza a RAIZ da �rvore
typedef struct {
    NoArvore *raiz;

} ArvoreBinariaBusca;

//M�todo que inicializa o n� da �rvore
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

//M�todo que insere n�s na �rvore
void InserirArvore(ArvoreBinariaBusca *arvore, NoArvore *novo){
    NoArvore *no = NULL;               //Armazena n� folha
    NoArvore *atual = arvore->raiz;    //N� seguinte ao n� folha

    while(atual != NULL){              //Procura o n� folha
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
        arvore->raiz = novo;                       //Atualiza a ra�z com o NOVO n� se for nula
        } else {                                   //Insere o NOVO n� ao n� folha
        if(strcmp(novo->dado, no->dado) < 0){
            no->esq = novo;
        } else if(strcmp(novo->dado, no->dado) > 0){
            no->dir = novo;
        } else {
            nodo *aux = novo->lista->prim;          //Caso o n� j� exista, insere-se os dados do NOVO n� � lista do n� existente
            arquivo arqui = aux->dado;
            insereLista(no->lista, arqui);
            free(novo);
        }
    }
}

//M�todo que imprime �rvore Em Ordem
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

//M�todo que busca por um TERMO na �rvore
//Retorna a Lista Encadeada do n� em caso de coincid�ncia
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

        if(atual == NULL){      //se n�o for encontrado um valor
            return NULL;
        }
    }

//M�todo que insere as palavras do arquivo especificado na �rvore
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
    //Criando e inserindo dados na �rvore
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
        ListaArquivos *atual = busca(termos[i], arvore);         //Captura a lista do n� da �rvore que coincide com a busca

        if(atual == NULL){
            printf("\nTermo '%s' nao encontrado em nenhum arquivo!", termos[i]);
        } else {
            nodo *noatual = atual->prim;
            while (noatual != NULL){                            //Itera pela lista encontrada e adiciona cada termo � lista "resultado"
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
