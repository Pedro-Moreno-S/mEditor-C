#ifndef PILHA_H
#define PILHA_H

//adiociono essa constante simbólica com o mesmo valor da que existe em  dupla_encad
#define max2 256


//tamanho máximo da pilha
#define maximo 10
typedef struct node{
	struct node *prev;
	char funcao[10];
	int parametros[2];
	char info[max2];  
	int reversa1;  //essas duas vriáveis guardarão informações que permitirião fazer o undo e o redo
	char reversa2[max2];
}nodeP;			      
			       
typedef struct{
	nodeP *top;
	int size;
}Pilha;

Pilha *newPilha();
nodeP *newNodeP(char *, int, int, char *, int, char*);
void rm_primeiro(Pilha *);
int push(Pilha *, char *, int, int, char *, int, char *);
int pop(Pilha *);
void esvaziarP(Pilha *);

#endif
