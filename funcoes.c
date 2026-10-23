/*Nesse arquivo serão implementadas as principais funções para o funcionamento
 do editor de texto. Para isso, utilizaremos a lista duplamente encadeada para
 manipulas as linhas de texto do arquivo. A pilha será usada para fazer o undo
 e o redo.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dupla_encad.h"
#include "pilha.h"
#include "funcoes.h"

//função para carregar as linhas do arquivo na lista
void newOpen(ListaLD *lista, FILE *arq){
	char vetor[max]; //vetor para receber o fgets e passar para a lista
	int i = 0; //variável para numerar as linhas

	while(fgets(vetor, max, arq) != NULL){
		if(vetor[0] != '\n'){
			//nessa parte eu confiro se a string não comeu o \n por ter enchido
			if(strlen(vetor) == max - 1 && vetor[max - 2] != '\n'){
				vetor[254] = '\n';
				//volto o cursor para depois ler o caractere que eu apaguei
				fseek(arq, -1, SEEK_CUR);
			}
			ad_finalLD(lista, vetor, i);
		}
		i++;
	}
}

//função para salvar o arquivo 
void save(ListaLD *lista, FILE *arq){
	nodeLD *aux = lista->head;

	//verificação para um só elemento para colocar ele no arquivo junto com os \n
	if(lista->head == lista->tail){
		if(aux->linha != 0){  

			//comparo com a linha pra saber quantos \n vem antes dela
			for(int i = 0; i < aux->linha; i++)
				fprintf(arq, "\n");
		}
		fprintf(arq, "%s", aux->data);
	}

	else{
		while(aux->next != NULL){
			fprintf(arq, "%s", aux->data);
			if(aux->linha != aux->next->linha - 1){
	
				//conto quantas vezes preciso imprimir o \n entre as lacunas de linhas
				int a = aux->next->linha - aux->linha -1;   
				//nesse trecho eu coloco os \n
				for(int i = 0; i < a; i++)
					fprintf(arq, "\n");
			}
			aux = aux->next;
		}
		//printo a última linha que eu deixei de fora para não acessar memória inválida no next
		fprintf(arq, "%s", aux->data);
	}
}

//função para enserir elemento em uma linha
void inserir(ListaLD *lista, int linha, char *data){
	
	//tratar o caso da lista vazia
	if(lista->head == NULL)
		ad_finalLD(lista, data, linha);

	//confiro se a nova linha vai estar depois do tail da lista
	else if(linha > lista->tail->linha){    //não é preciso atualizar a linha de nada
		if(ad_finalLD(lista, data, linha) == 0){  //colocar linha no final
			printf("mtext> Falha ao adicionar linha.\n");
			return;
		}
	}

	//confiro se a linha é menor que a head, ou seja, vem antes dela
	else if(linha <= lista->head->linha){
                 if(ad_inicioLD(lista, data, linha) == 0){  //colocar linha no inicio
                   	printf("mtext> Falha ao adicionar linha.\n");
			 return;
		}
		
		nodeLD *aux = lista->head->next;  //preciso atualizar todas menos a head nesse caso
		while(aux != NULL){
			aux->linha++;
			aux = aux->next;
	 	}
	}

	else{

		//salvo o nó adicionado para fazer a incrementação das linhas posteriores
		nodeLD *aux = ad_meioLD(lista, data, linha);  
	

		if(aux == NULL){
			printf("mtext> Falha ao adicionar linha.\n");
			return;
		}

		aux = aux->next;    //aqui sim eu aumento 1 em cada linha posterior a adicionada
		while(aux != NULL){
			aux->linha++;
			aux = aux->next;
		}
	}
}

char *deletar(ListaLD *lista, int posicao){
	
	//salvar o conteúdo da linha
	char *string = (char *)malloc(sizeof(char) * max);
	//testo se alocou e trato o erro caso não tenha alocado
	if(string == NULL){
		printf("mtext> Erro ao alocar memória.\n");
		return NULL;
	}
	string[0] = '\0';
        //caso o usuário queira tirar o primeiro nó da lista
	if(posicao == lista->head->linha){	
		//salvo o conteúdo e removo
		strcpy(string, lista->head->data);
		rm_inicioLD(lista);

		//agora é preciso atualizar as posições removendo um
		nodeLD *aux = lista->head;
		while(aux!= NULL){
			aux->linha--;
			aux = aux->next;
		}
		//subtraio da linha que sobrou. Faço dessa forma para evitar erro de segmentação
	}
	
	//agora testar se o usuário quer remover a última linha	
	else if(posicao == lista->tail->linha){
		//guardo o conteúdo
		strcpy(string, lista->tail->data);
		//nesse caso não é preciso atualizar nenhuma linha
		rm_finalLD(lista);
	}
	   
	else{
		//rm retorna a string  retirada
		char *a = rm_meioLD(lista, posicao);

		strcpy(string, a);
		
		free(a);

		//percorro os nós para consertar as linhas após a remoção
		nodeLD *aux = lista->head;
		while(aux->next!= NULL){
			if(aux->linha > posicao)
				aux->linha--;
			aux = aux->next;
		}
		aux->linha--;
	}
	return string;
}

//função para substituir uma linha
char *substituir(ListaLD *lista, int posicao, char *texto){

	//string para salvar o conteúdo da linha
        char *string = (char *)malloc(sizeof(char) * max);
	if(string == NULL){
		printf("mtext> Erro ao alocar meemória.\n");
		return NULL;
	}

	string[0] = '\0';

	//percorro a lista procurando o nó e paro no anterior
	nodeLD *aux = lista->head;
	while(aux->next != NULL && aux->next->linha < posicao)
		aux = aux->next;
	
	//caso em que existe um nó na posição que eu quero substituir
	if( aux->next != NULL && aux->next->linha == posicao){
		//salvo a string anteior antes de sobrescrever	
		strcpy(string, aux->next->data);
		strcpy(aux->next->data, texto);
	}

	//caso de ser o primeiro nó o desejado
	else if(aux->linha == posicao){	
		strcpy(string, aux->data);
		strcpy(aux->data, texto);
	}

	//caso contrário, eu crio o novo nó
	else{
		//caso de parar logo no início e o nó não existir
		if(posicao < lista->head->linha){
			ad_inicioLD(lista, texto, posicao);
		}

		else if(posicao > lista->tail->linha)
			ad_finalLD(lista, texto, posicao);
		else
			ad_meioLD(lista, texto, posicao);
	}
	return string;
}

//função para concatenar um sufixo no final da linha indicada
char *concatenar(ListaLD *lista, int posicao, char *texto){
	//variável para retornar o texto antes da modificação
	char *string = (char*)malloc(sizeof(char) * max);
	if(string == NULL){
		printf("mtext: Erro ao alocar memória.\n");
		return NULL;
	}	
	
	string[0] = '\0';
	//mesma estrutura da função anterior
	nodeLD *aux = lista->head;
	while(aux->next != NULL && aux->next->linha < posicao)
                aux = aux->next;

        //caso em que existe um nó na posição que eu quero concatenar
        if( aux->next != NULL && aux->next->linha == posicao){
		if(strlen(aux->next->data) + strlen(texto) >= max){
			printf("mtext> Erro! O tamanho excede a capacidade da linha.\n");
			return NULL;
		}

		else{
			strcpy(string, aux->next->data);
			//primeiro preciso retirar o \n da primeira string
			aux->next->data[strlen(aux->next->data) - 1] = '\0';
         		strcat(aux->next->data, texto);
		}
        }

        //caso de ser o primeiro nó o desejado
        else if(aux->linha == posicao){

		 if(strlen(aux->data) + strlen(texto) >= max){
			 printf("mtext> Erro! O tamanho excede a capacidade da linha.\n");
			 return NULL;
		 }

		 else{
			 strcpy(string, aux->data);
			 //primeiro preciso retirar o \n da primeira string
			 aux->data[strlen(aux->data) - 1] = '\0';
			 strcat(aux->data, texto);
		 }
	}
	else{
		//caso de parar logo no início e o nó não existir
		if(posicao < lista->head->linha){
			ad_inicioLD(lista, texto, posicao);
		}
		
		//caso do nó não existir no final
		else if(posicao > lista->tail->linha)
			ad_finalLD(lista, texto, posicao);

		//caso do nó não existir no meio
		else
			ad_meioLD(lista, texto, posicao);
	}
	return string;
}

//função para separar uma linha no índice indicado
int split(ListaLD *lista, int posicao, int indice){

	//dessa vez paro no nó que eu quero
	nodeLD *aux = lista->head;
	while(aux != NULL && aux->linha != posicao)
		aux = aux->next;

	if(aux == NULL){
		printf("mtext> Linha vazia.\n");
		return 0;
	}

	//confiro se não há só um caractere na linha, o que não faria sentido para essa função
	else if(strlen(aux->data) == 1){
		printf("mtext> Só há um elemento na linha.\n");
		return 0;
	}

	//confiro se o indice está entre o primeiro elemento (pode ser igual a ele) e último elemento da lista
	else if(strlen(aux->data) - 3 < indice){
		printf("mtext> Índice inválido.\n");
		return 0;
	}

	//agora sim eu faço o split
	else{
		char novo[max];
		int cont = 0;

		//salvo o que vem depois do índice
		for(int i = indice + 1; i < strlen(aux->data) + 1; i++){
			novo[cont] = aux->data[i];
			cont ++;
		}

		//devolvo o \n e o \0 para a string cortada
		aux->data[indice + 1] = '\n';
		aux->data[indice + 2] = '\0';

		inserir(lista, posicao + 1, novo);
		return 1;
	}
}

//função para concatenar a linha seguinte à linha escolhida
int join(ListaLD *lista, int posicao){
	nodeLD *aux = lista->head;

	//esse início estou fazendo igual à função anterior
	while(aux != NULL && aux->linha != posicao)
		aux = aux->next;

	if(aux == NULL){
		printf("mtext> Linha vazia.\n");
		return -1;
	}


	//aqui vou conferir se a próxima linha não está vazia
	if(aux->next != NULL && aux->next->linha > aux->linha + 1){
		printf("mtext> Próxima linha vazia.\n");
		return -1;
	}

	//confiro se a soma das linhas não execede a capacidade máxima da linha
	//nesse caso, o máximo que pode dar a soma dos strlen é o max da linha
	if(strlen(aux->data) + strlen(aux->next->data) > max + 1){
		printf("mtext> Tamanho da concatenção excede o limite da linha.\n");	
		return -1;
	}

	else{
		//retiro o \n da string
		aux->data[strlen(aux->data) - 1] = '\0';
		
		//salvo a posição da última letra da primeira string pra usar no undo
		int save = strlen(aux->data) - 1;

		//concateno
		strcat(aux->data, aux->next->data);

		//casos para retirar o nó
		if(aux->next == lista->tail)
			rm_finalLD(lista);

		else{
			rm_meioLD(lista, posicao + 1);

			//atualizo os nós seguintes
			aux = aux->next;
			while(aux != NULL){
				aux->linha--;
				aux = aux->next;
			}
		}
		return save;
	}
}

//Essa função realizará tanto undo quanto redo, bastará inverter a ordem que eu passo as pilhas
int fun_unre(ListaLD * lista, Pilha *p1, Pilha *p2){
	//confiro se ainda há operações para fazer
	if(p1->top == NULL){
		printf("mtext> Máximo de operações alcançado.\n");
		return 0;
	}

	else{
		//agora comparo com if para saber a função que devo usar
		if(!strcmp(p1->top->funcao, "i")){
			//nesse caso eu tirei uma linha vazia, então só preciso atualizar as posições
			if(p1->top->info[0] == '\0'){
				nodeLD *aux = lista->head;
				while(aux != NULL){
					if(aux->linha >= p1->top->parametros[0])
						aux->linha++;
					aux = aux->next;
				}
			}
			//nesse caso tinha conteúdo na linha, então uso inserir
			else{
				inserir(lista, p1->top->parametros[0], p1->top->info);
			}
			//antes de dar pop eu dou undo em push
			push(p2, "d", p1->top->parametros[0], -1, "", p1->top->parametros[0], p1->top->info);
			pop(p1);
		}

		//só aplica a função deletar
		else if(!strcmp(p1->top->funcao, "d")){
			deletar(lista, p1->top->parametros[0]);
			push(p2, "i", p1->top->parametros[0], -1, p1->top->reversa2, p1->top->parametros[0], "");
			pop(p1);	
		}
		
		else if(!strcmp(p1->top->funcao, "r")){
			//caso em que não tinha o nó e eu precisei criar
			if(p1->top->info[0] == '\0'){
				nodeLD *aux = lista->head;
				//procuro esse nó para retirar ele
				//caso de ser logo no inicio o elemetno
				if(p1->top->parametros[0] == lista->head->linha)
					rm_inicioLD(lista);

				//caso de ser no final
				else if(p1->top->parametros[0] == lista->tail->linha)
					rm_finalLD(lista);
				else
					rm_meioLD(lista, p1->top->parametros[0]);	
				//eu faço a remoção sem usar a função deletar para não alterar o número das linhas
			}
			//caso contrário, eu só sobrescrevo
			else
				substituir(lista, p1->top->parametros[0], p1->top->info);
			push(p2, "r", p1->top->reversa1, -1, p1->top->reversa2, p1->top->parametros[0], p1->top->info);
			pop(p1);
		}
		
		else if(!strcmp(p1->top->funcao, "a")){
			   //caso em que não tinha o nó e eu precisei criar
                        if(p1->top->info[0] == '\0'){
                                nodeLD *aux = lista->head;
                                //procuro esse nó para retirar ele
                                //caso de ser logo no inicio o elemento
                                if(p1->top->parametros[0] == lista->head->linha)
                                        rm_inicioLD(lista);

                                //caso de ser no final
                                else if(p1->top->parametros[0] == lista->tail->linha)
                                        rm_finalLD(lista);
                                else
                                        rm_meioLD(lista, p1->top->parametros[0]);
			}
			else{
				//caso do nó existir anteriormente
				nodeLD *aux = lista->head;
				while(aux != NULL && aux->linha != p1->top->parametros[0])
					aux = aux->next;
				strcpy(aux->data, p1->top->info);
			}
			printf("teste\n");
			push(p2, "a", p1->top->reversa1, -1, p1->top->reversa2, p1->top->reversa1, p1->top->info);
			pop(p1);
		}
		
		else if(!strcmp(p1->top->funcao, "split")){
			split(lista, p1->top->parametros[0], p1->top->parametros[1]);
			push(p2, "join", p1->top->parametros[0], -1, "", p1->top->parametros[1], "");
			pop(p1);
		}

		//caso da função join
		else{
			join(lista, p1->top->parametros[0]);
			push(p2, "split", p1->top->parametros[0], p1->top->reversa1, "", p1->top->parametros[0], "");
			pop(p1);
		}
		return 1;
	}
}

//função para liberar toda a memória das listas e pilhas
void clear(ListaLD* lista, Pilha *undo, Pilha *redo){
	//eu libero cada nó e depois eu libero a struct que representa a estrutura
	//libero a lista
	nodeLD *aux1 = lista->head;
	while(aux1 != NULL){
		nodeLD *save1 = aux1;
		aux1 = aux1->next;
		free(save1);
	}
	free(lista);

	//libero a undo
	nodeP *aux2 = undo->top;
	nodeP *save2;
	while(aux2 != NULL){
		save2 = aux2;
		aux2 = aux2->prev;
		free(save2);
	}
	free(undo);

	//libero a redo
	aux2 = redo->top;
	while(aux2 != NULL){
		save2 = aux2;
		aux2 = aux2->prev;
		free(save2);
	}
	free(redo);
}

//essa função apenas mostrará os possíveis comandos
void help(){
printf("Lista de comandos:\n");
	printf("-open <arquivo>\n");
	printf("-save\n");
	printf("-i <posição> <texto>\n");
	printf("-d <posição>\n");
	printf("-r <posição> <novo texto>\n");
	printf("-a <posição> <sufixo>\n");
	printf("-split <posição> <índice>\n");
	printf("-join <pos>\n");
	printf("-undo\n");
	printf("-redo\n");
	printf("-print\n");
	printf("-quit\n");	
	printf("-close\n");
	printf("-help\n");
}

//função para imprimir o arquivo utilizando recursão
void imprimir(ListaLD *lista, nodeLD *aux){  //esse node vai receber a cauda da lista
	//caso base da recursão
	if(aux == lista->head){	
	//for para printar os \n caso a head da lista não seja a primeira linha do arquivo
		if(aux->linha != 0){
			for(int i = 0; i < aux->linha; i++)
				printf("%d | \n", i);
			}
		printf("%d | %s", aux->linha, aux->data);
	}

	else{
		imprimir(lista, aux->prev); //chamada recursiva passando o elemento anterior
					    
		//if para imprimir os \n pelo caminho
		//comparo para testar se as linhas são consecutivas
		if(aux->linha != aux->prev->linha + 1){

			//preciso estruturar meu if dessa forma estranha pra usar o i para printar as linhas
			for(int i = aux->prev->linha + 1; i < aux->linha; i++)
				printf("%d | \n", i);
		}	

		printf("%d | %s", aux->linha, aux->data);		
	}	
}
