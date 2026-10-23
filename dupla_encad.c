/*Esta biblioteca contem as função que serão utilizadas para
 manipular uma lista duplamente encadeada e que tambpém serão
 utilizadas dentro das principais funções do editor de texto.
 É importante lembrar que cada nó será uma linha e que o editor
 não vai criar arquivos, mas receber arquivos já existentes.
 Por conta disso, as funções não tratam os casos em que a lista
 está vazia, pois isso nunca vai acontecer.*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dupla_encad.h"

//função para criar um novo nó na lista
nodeLD *newnodeLD(char *data, int linha){
	nodeLD *aux = malloc(sizeof(nodeLD));
	if(aux == NULL)   //if para tratar erros na função principal
		return NULL; 
	aux->next = NULL;
	aux->prev = NULL;
	aux->linha = linha;
	strcpy(aux->data, data);
	return aux;
}

//função para criar uma nova lista
ListaLD *newListaLD(){
	ListaLD *aux = (ListaLD *)malloc(sizeof(ListaLD));
	if(aux == NULL)
		return NULL; //if para tratar erros na função principal
	aux->size = 0;
	aux->head= NULL;
	aux->tail = NULL;
	return aux;
}

//função para adicionar um elemento no início da lista
//retorna o novo nó para tratar erros
nodeLD *ad_inicioLD(ListaLD *lista, char *data, int linha){
	nodeLD *aux = newnodeLD(data, linha);
	if(aux == NULL)
		return NULL;  //tratar os erros de alocação
	
	if(lista->head == NULL){
		lista->head = aux;
		lista->tail = aux;	
	}
	else{
		lista->head->prev = aux;
		aux->next = lista->head;
		lista->head = aux;	
	}

	lista->size++;
	return aux;
}

//função para remover no inicio da lista
//não trato o caso da lista vazia pois o código principal faz isso
void rm_inicioLD(ListaLD *lista){
	nodeLD *aux = lista->head; 
	if(lista->head == lista->tail){
		lista->head = NULL;
		lista->tail = NULL;

	}
	else{
		lista->head = aux->next;
		lista->head->prev = NULL;
	}
	
	free(aux);
	lista->size--;
}

//função para adicionar no final da lista
nodeLD *ad_finalLD(ListaLD *lista, char *data, int linha){
	nodeLD *aux = newnodeLD(data, linha);
	if(aux == NULL)		
		return NULL;	//tratar os erros de alocação

	if(lista->head == NULL){
	        lista->tail = aux;
	        lista->head = aux;
	}
	else{
	        lista->tail->next = aux;
	        aux->prev = lista->tail;
	        lista->tail = aux;
	}
	lista->size++;
	return aux;
}

//função para remover elemento no final da lista
void rm_finalLD(ListaLD *lista){
	nodeLD *aux = lista->tail;

	if(lista->head == lista->tail){
		lista->head = NULL;
		lista->tail = NULL;
	}

	else{
		lista->tail = lista->tail->prev;
		lista->tail->next = NULL;
	}
	lista->size--;
	free(aux);
}

//função para adicionar elemento no meio da lista
/*não é preciso se procupar com a validade da posição,
 só se ela for negativa, o que vai ser tradado na função principal
 Além disso, eu só tratei o caso em que é adicionado um elemento
 no meio pois no início e no final a função inserir já trata*/
nodeLD *ad_meioLD(ListaLD *lista, char *data, int posicao){
	if(posicao < 0)
		return NULL;
    
	else{
		//aqui eu só tenho que bucar o nó anterior a posição para modificar tudo
		nodeLD *aux = newnodeLD(data, posicao);
		nodeLD *aux2 = lista->head;

		//busco o nó que é anterior onde eu quero colocar e tomo cuidado para não acessar memória inválida
		while(aux2->next != NULL && aux2->next->linha < aux->linha)
			aux2 = aux2->next;
		aux->next = aux2->next;
		aux->prev = aux2;
		aux2->next->prev = aux;
		aux2->next = aux;
		lista->size++;
		return aux;
	}
}

//função para remover no meio da lista
char *rm_meioLD(ListaLD *lista, int posicao){
	
	//string para salvar o conteúdo da linha	
	char *string = (char *)malloc(sizeof(char) * max);
	if(string == NULL){
		printf("mtext> Erro ao alocar memória");
		return NULL;
	}

	string[0] = '\0';

	nodeLD *aux = lista->head;
	//nessa parte eu só quero tirar o nó caso ele exista, por isso essa condição extensa no while
	//o outro caso eu trato na função principal
  	while(aux != NULL && aux->linha != posicao)
		aux = aux->next;
	
	//só realizar as operações caso o nó exista (caso ele não exista o while percorre até o penúltimo elemento)
	if(aux != NULL && aux->linha == posicao){
		//salvo o conteúdo
		strcpy(string, aux->next->data);

		//faço as operações
		nodeLD *save = aux;
		aux->prev->next = aux->next;
		aux->next->prev = aux->prev;
		free(save);
		lista->size--;
		
		return string;
	}
	return string;
}

