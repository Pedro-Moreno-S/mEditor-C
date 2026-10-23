#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dupla_encad.h"
#include "pilha.h"
#include "funcoes.h"

int main(){
	char desejo[15], arquivo[100];
	int flag = 0; 	//flag para garantir que certar operações só vão ocorrer com o arquivo aberto

	int flag_redo = 0; //essa flag não vai permitir que o usuário execute redo quando quando ele der undo e em seguida
				//fizer alguma outra modificação
	ListaLD *lista = NULL;
	Pilha *undo = NULL, *redo = NULL;
	FILE *arq = NULL;
	
	//interface simples para o usuário visualizar os comando existentes
	printf("-------------Bem-vindo ao mEditor-------------\n");
	help();
	printf("\n");
	
	//esse vai ser o loop principal do programa
	do{
		//interface básica do programa
		printf("mtext> ");
		scanf(" %s", desejo);

                //agora vou colocar uma sequência de if para direcionar para cada comando
		//eu não vou usar switch pois seria preciso converter cada string em um número
		if(!strcmp(desejo, "open")){
			if(flag == 1)
				printf("mtext Arquivo já aberto.\n");
			//inicio as listas e pilhas
			else{
				undo = newPilha(); //criar pilhas para o undo e redo
				redo = newPilha();
				lista = newListaLD();  //criar lista para carregar o arquivo do usuário
				
				if(lista == NULL)
					printf("mtext> Erro ao alocar espaço para abrir o arquivo.\n");
				else if(undo == NULL)
					printf("mtext> Erro ao alocar espaço para abrir o arquivo.\n");
				else if(redo == NULL)
					printf("mtext> Erro ao alocar espaço para abrir o arquivo.\n");
				else{
					scanf(" %[^\n]", arquivo);
					arq = fopen(arquivo, "r+");

					//conferindo se o arquivo abriu
					if(arq == NULL)
						printf("mtext> Falha ao abrir o arquivo.\n");
					else{
						newOpen(lista, arq);
						flag = 1;
						printf("mtext> Arquivo aberto com sucesso.\n");
						
					}
				}
			}	
		}	
		
		//função apenas para instruir o usuário
		else if(!strcmp(desejo, "help")){
			printf("\n-----------------------------------\n");
			help();
			printf("-----------------------------------\n\n");
		}
		//função para salvar as alterações no arquivo
		//não tem como dar undo nesse comando
		else if(!strcmp(desejo, "save")){
			if(flag != 1)
				printf("mtext> Abra um arquivo primeiro.\n");
			else{
				//arquivo temporário para backup
				//uso um nome aleatória para não sobrescrever nenhum arquivo existente
				FILE *temp = fopen("fpdfsfmnbvrtdksgpo3459gf4j309tj3kkert34", "w+");
				if(temp == NULL)
					printf("mtext> Erro ao alocar memória.\n");
				else{
					//passo tudo pro temporário
					save(lista, temp);
					fclose(temp);
					
					//apago o original
					if(remove(arquivo) != 0){
						printf("mtext> Erro ao salvar arquivo.\n");
						remove("fpdfsfmnbvrtdksgpo3459gf4j309tj3kkert34");
					}
					//renomeio o temp com o nome do original
					else if(rename("fpdfsfmnbvrtdksgpo3459gf4j309tj3kkert34", arquivo) != 0){
						printf("mtext> Erro ao salvar o arquivo.\n");
						remove("fpdfsfmnbvrtdksgpo3459gf4j309tj3kkert34");
					}
					else
						printf("mtext> Arquivo salvo com sucesso.\n");
				}
			}
		}

		//função para adicionar uma linha no local desejado
		else if(!strcmp(desejo, "i")){
			if(flag != 1) //flag que testa se não há arquivos abertos
				printf("mtext> Abra um arquivo primeiro.\n");
			else{
				int posicao;  //indica a linha 
				char texto[max];	//novo texto
				scanf("%d", &posicao);
				scanf(" %[^\n]", texto);
					
				//garantindo que não tenha posição negativa
				if(posicao < 0)
					printf("mtext> Posição inválida.\n");
				
				//garantindo que o usuário não vai digitar uma posição muito grande
				else if(posicao > 10000)
					printf("mtext> Posição muito alta.\n");
				else{
					//adicionando \n no final para não quebrar a impressão
					int salva = strlen(texto);
					texto[salva] = '\n';
					texto[salva + 1] = '\0';
					
					inserir(lista, posicao, texto);
					printf("mtext> Linha adicionada.\n");

					//adiciono na pilha undo a função reversa
					//passo -1 e "" pois essas informações não serão utilizadas
					//guardo também as informações para redo
					push(undo, "d", posicao, -1, "", posicao, texto);

					//nesse caso eu confiro o estado da redo para tratar os erros dela corretamente e eu
					//vou fazer isso em toda função que modifique o arquivo
					if(flag_redo)
						flag_redo = 0;
				}					
			}
		}	


		//função para apagar uma linha
		else if(!strcmp(desejo, "d")){
			if(flag != 1)
				printf("mtext> Abra um arquivo primeiro.\n");
			else
			{
                                int posicao;
                                scanf("%d", &posicao);
                                
				if(lista->head == NULL)
					printf("mtext> Arquivo vazio.\n");

				//garantindo que não tenha posição negativa
				else if(posicao < 0 || posicao > lista->tail->linha)
                                        printf("mtext> Posição inválida.\n");
				else{
					//a função deletar retorna a string apagada
					char *apagar = deletar(lista, posicao);
					if(apagar != NULL){
						push(undo, "i", posicao, -1, apagar, posicao, "");
						free(apagar);
						printf("mtext> Linha removida com sucesso.\n");
						if(flag_redo)
							flag_redo = 0;
					}
				}
			}
		}
		
		//funçao para substituir uma linha
		else if(!strcmp(desejo, "r")){
			if(flag != 1)
				printf("mtext> Abra um arquivo primeiro.\n");	
			else{
				int posicao;
				char texto[max];
			
				scanf("%d", &posicao);

				if(lista->head == NULL)
					printf("mtext> Arquivo vazio.\n");
				//tratar erros de posição
				else if(posicao < 0 || posicao > lista->tail->linha)
					printf("mtext> Posição inválida.\n");

				else{
					scanf(" %[^\n]", texto);
					
					if(strlen(texto) == max - 1)
						printf("mtext> Erro. Número de caracteres ultrapassa o permitido.\n");
					else{
						//adicionando \n no final para não quebrar a impressão
                                        	int salva = strlen(texto);
                                        	texto[salva] = '\n';
                                        	texto[salva + 1] = '\0';

						//função substituir retorna a string substituída
						char *apagar = substituir(lista, posicao, texto);
						if(apagar != NULL){
							push(undo, "r", posicao, -1, apagar, posicao, texto);
							free (apagar);
							printf("mtext> Linha substituida com sucesso.\n");
							if(flag_redo)
								flag_redo = 0;
						}
					}
				}
			}
		}

		//função para concatenar um sufixo ao final da linha indicada
		else if(!strcmp(desejo, "a")){
			if(flag != 1)
                                printf("mtext> Abra um arquivo primeiro.\n");
                        else{
                                int posicao;
                                char texto[max];

                                scanf("%d", &posicao);

                                if(lista->head == NULL)
                                        printf("mtext> Arquivo vazio.\n");
                                //tratar erros de posição
				else if(posicao < 0 || posicao > lista->tail->linha)
                                        printf("mtext> Posição inválida.\n");

				 else{
					getchar(); //faço isso para limpar o buffer de \n

					//leio dessa vez com fgets para não ignorar espaços iniciais
                                        fgets(texto, sizeof(texto), stdin);
					
					//confiro se o \n não foi excluído por conta da string cheia
					size_t len = strlen(texto);
      			  	  	if(len > 0 && texto[len - 1] != '\n') //uso o tamanho do vetor para verificar
                              			texto[len - 1] = '\n'; //adiciono o \n		  
					
					//executo concatenar e guardo as informações em undo
					char *apagar = concatenar(lista, posicao, texto);
					if(apagar != NULL){
					        //procuro o nó com a linha concatenada para passar o conteúdo como parâmetro
				                nodeLD *procurar = lista->head;
				                while(procurar->linha != posicao)
                                                        procurar = procurar->next;
                                                //agora dou o push com todas as informações
                                                push(undo, "a", posicao, -1, apagar, posicao, procurar->data);
						printf("mtext> Linha concatenada com sucesso.\n");
						free(apagar);
						if(flag_redo)
							flag_redo = 0;
					}
				}
			}
		}

		//funçao para quebrar a linha no índice indicado
		else if(!strcmp(desejo, "split")){
			    if(flag != 1)
                                printf("mtext> Abra um arquivo primeiro.\n");
                        else{
                                int posicao, indice;
				scanf("%d", &posicao);
				
				if(lista->head == NULL)
                                        printf("mtext> Arquivo vazio.\n");

                                //tratar erros de posição
				else if(posicao < 0 || posicao > lista->tail->linha)
                                        printf("mtext> Posiçao inválida.\n");
				else{
					scanf("%d", &indice);

				//confiro os índices. Caso o índice seja o último caractere, nada acontece
				// por isso não valido ele no if
					if(indice < 0 || indice >= max - 2)
						printf("mtext> Índice inválido.\n");
					else{
						push(undo, "join", posicao, -1, "", indice, "");
						//utilizo um print pro if pois há tratamento de erro dentro da função
						if(split(lista, posicao, indice)){
							printf("mtext> Linha cortada com sucesso.\n");	
							if(flag_redo)
								flag_redo = 0;
						}
					}
				}
			}
		}

		//função para concatenar a próxima linha na linha escolhida
		   else if(!strcmp(desejo, "join")){
                            if(flag != 1)
                                printf("mtext> Abra um arquivo primeiro.\n");
                        else{
                                int posicao;
                                scanf("%d", &posicao);
				
				if(lista->head == NULL)
                                        printf("mtext> Arquivo vazio.\n");

                                //tratar erros de posição (não vou permitir fazer com a última linha)
				else if(posicao < 0 || posicao >= lista->tail->linha)
                                        printf("mtext> Posiçao inválida.\n");
				
				else{
					//join retorna -1 em caso de erro, caso contrário retorna o índice
					//onde o split da undo deve ser feito
					int teste = join(lista, posicao);
					if(teste != -1){	
						printf("mtext> Linha concatenada com sucesso.\n");
						push(undo, "split", posicao, teste, "", posicao, "");
						if(flag_redo)
							flag_redo = 0;
					}
				}
			}
		   }

		//função para desfazer um comando realizado
		else if(!strcmp(desejo, "undo")){
			if(flag != 1)
				printf("mtext> Abra um arquivo primeiro.\n");
			else{
				if(fun_unre(lista, undo, redo)){
					printf("mtext> Ação desfeita.\n");
					flag_redo = 1; //habilito a função redo
				}
			}
		}

		//função para refazer um comando
		else if(!strcmp(desejo, "redo")){
			if(flag != 1)
				printf("mtext> Abra um arquivo primeiro.\n");

			else{
				//nesse caso o usuário usou undo e fez uma outra modificação no arquivo, logo, eu desabilito a
				//redo
				//e esvazio a pilhaa dela
				if(!flag_redo && redo->top != NULL){
					printf("mtext> Não há ações para desfazer.\n");
					esvaziarP(redo);
				}
				//nesse caso o usuário ainda não desfez nada
				else if(!flag_redo)
					printf("mtext> Não há ações para desfazer.\n");	
			
				//confiro se tudo ocorreu bem em redo para printar a mensagem que deu certo
				else if(fun_unre(lista, redo, undo)){
					printf("mtext> Ação refeita.\n");
				}
			}
		}

		//função para mostrar o arquivo na tela
		else if(!strcmp(desejo, "print")){
			if(flag != 1)
				printf("mtext> Abra um arquivo primeiro.\n");
			else{
				//caso em que a lista está vazia
				if(lista->head == NULL)
					printf("mtext> Arquivo vazio.\n");
				
				else{
					//chamando a função recursiva para imprimir o arquivo
					imprimir(lista, lista->tail);

					//organizando a saída
					printf("------------------------------------------------------------\n\n");
				}
			}
		}
		
		//função que fechará o arquivo e possibilitará que o usuário abra outro
		else if(!strcmp(desejo, "close")){
			if(flag == 0)
				printf("mtext> Abra um arquivo primeiro.\n");
			else{
				clear(lista, undo, redo);
				fclose(arq);
				printf("mtext> Arquivo fechado.\n");
				flag = 0;

				//deixo tudo como NULL
				arq = NULL;
				lista = NULL;
				undo = NULL;
				redo = NULL;
			}
		}

		//como o loop sempre é verdadeiro, eu uso o break para sair dele
		else if(!strcmp(desejo, "quit")){
		        if(flag == 1)
			        clear(lista, undo, redo); //libero a memória
			
			if(arq != NULL)
				fclose(arq); //fecha o arquivo para impedir qualquer problema	
			printf("\nPrograma encerrado.\n");
			break;
		}

		//tratar comando inválidos
		else
			printf("mtext> Comando inválido.\n");

	}while(1);
	return 0;
}
