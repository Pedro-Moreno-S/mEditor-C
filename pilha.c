#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"
//função para criar um novo nó da pilha
Pilha *newPilha(){
	Pilha *aux = (Pilha *)malloc(sizeof(Pilha));
	
	//tratar erros
	if(aux == NULL)
		return NULL;
	aux->top = NULL;
	aux->size = 0;
	return aux;
}

//crio o nó com todas as informações passadas
nodeP *newNodeP(char *funcao, int par1, int par2, char *info, int par3, char *info2){
	nodeP *new = (nodeP *)malloc(sizeof(nodeP));
	
	//tratar erros
	if(new == NULL)
		return NULL;

	strcpy(new->funcao, funcao);
	new->parametros[0] = par1;
	new->parametros[1] = par2;
	strcpy(new->info, info);
	new->reversa1 = par3;
	strcpy(new->reversa2, info2);
	new->prev = NULL;
	return new;
}

//funcão para retirar o primeiro elemento caso a pilha esteja cheia
void rm_primeiro(Pilha *pilha){
	nodeP *aux = pilha->top;

	//percorro a pilha até achar antes do último elemento
	while(aux->prev->prev != NULL)
		aux = aux->prev;

	free(aux->prev);
	aux->prev = NULL;
	pilha->size--;
}

//uma função push de pilha e eu retorno int para tratar erros de alocação
int push(Pilha *pilha, char *funcao, int par1, int par2, char *info, int par3, char *info2){
	//confiro se a pilha está cheia para tirar o último elemento dela
	if(pilha->size == maximo)
		rm_primeiro(pilha);

	//crio o novo nó e faço as operações da pilha
	nodeP *aux = newNodeP(funcao, par1, par2, info, par3, info2);

	if(aux == NULL)
		return 0;
	
	aux->prev = pilha->top;
	pilha->top = aux;
	pilha->size++;
	return 1;
}

//um pop normal, porém, que eu não retorno o elemento que eu estou tirando
int pop(Pilha *pilha){
	if(pilha->top == NULL){
		return 0;
	}
	nodeP *aux = pilha->top;
	pilha->top = pilha->top->prev;
	free(aux);
	pilha->size--;
	return 1;
}

//função para eu esvaziar uma pilha limpando cada nó
void esvaziarP(Pilha *pilha){
	nodeP *aux = pilha->top;
	while(pilha->top != NULL){
		aux = aux->prev;
		free(pilha->top);
		pilha->top = aux;
	}
	//me certifico que o topo está apontando para NULL
	pilha->top = NULL;
}
