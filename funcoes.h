#ifndef FUNCOES_H
#define FUNCOES_H

void newOpen(ListaLD *, FILE *);
void inserir(ListaLD *, int, char *);
void save(ListaLD *, FILE *);
char *deletar(ListaLD *, int);
char *substituir(ListaLD *, int, char *);
char *concatenar(ListaLD *, int, char *);
int split(ListaLD *, int, int);
int join(ListaLD *, int);
int fun_unre(ListaLD *, Pilha *, Pilha *);
void clear(ListaLD*, Pilha*, Pilha*);
void help();
void imprimir(ListaLD *, nodeLD *);
#endif
