#ifndef DUPLA_ENCAD_H
#define DUPLA_ENCAD_H
#define max 256
//o max será a quantidade máxima de carcteres que a linha aceita]

//struct da nó da lista
typedef struct nodeLD{
	char data[max];
	int linha;   //variável importante que indica o número da linha
	struct nodeLD *next;
	struct nodeLD *prev;
}nodeLD;

//struct da lista em si
typedef struct ListaLD{
	int size;
	nodeLD *head;
	nodeLD *tail;
}ListaLD;

//protótipos das funções
nodeLD *newnodeLD(char *data, int linha);
ListaLD *newListaLD();
nodeLD *ad_inicioLD(ListaLD *lista, char *data, int linha);
void rm_inicioLD(ListaLD *);
nodeLD *ad_finalLD(ListaLD *lista, char *data, int linha);
void rm_finalLD(ListaLD *lista);
nodeLD *ad_meioLD(ListaLD *lista, char *data, int posicao);
char *rm_meioLD(ListaLD *lista, int posicao);

#endif
