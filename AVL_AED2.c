/*
----------Programa AVL------------
Alunos: Jade Santos e Marlon Silva
--------------AED2----------------
*/
#include <stdio.h>
#include <stdlib.h>
#define tam 10 //definir a quantidade m�xima de valor a ser inserido na AVL

typedef struct ITEM{
	int valor;
}tipoItem;

typedef struct NOH{
	tipoItem item;
	struct NOH *esquerda;
	struct NOH *direita;
	int fb;
	int altura;
}No;//estrutura da AVL, com os ponteiros e o fator de balanceamento

No *iniciar(){//Iniciar a AVL
	return NULL;
}

void imprimir(No *raiz, int espaco){ 
    //Caso base 
    if (raiz == NULL){ 
        return; 
    }else{
    //Aumentar a dist�ncia entre os n�veis 
    espaco += tam; 

    //Trabalhando com a sub Arvore direita 
    imprimir(raiz->direita, espaco); 
  
    // Imprimir n� ap�s o espa�o 
    printf("\n"); 
    for (int i = tam; i < espaco; i++){
        printf(" "); 
    }
	
	//mostrar o n�
    printf("%d\n", raiz->item.valor); 
  
    //Trabalhando com a sub Arvore direita
    imprimir(raiz->esquerda, espaco); 
	}
}

void apagarArvore(No *raiz){//Funcao para liberar a memoria utilizada pela arvore
	if (raiz != NULL){
		apagarArvore(raiz->esquerda);
		apagarArvore(raiz->direita);
		free(raiz);
		raiz = NULL;
	}
}

tipoItem criarItem(int valor){
	tipoItem item;
	item.valor = valor;
	return item;
}

No *alocar(){
		No *novoNoh = (No*) malloc(sizeof(No));
		if (novoNoh != NULL){
			return novoNoh;
		}else{
			printf("Erro ao criar novo n�\n");
			exit(1); //interrompe o programa
		}
}

int alturaNoh(No *raiz){ // calcula a altura de um n�
    if (raiz==NULL) // se n�o existe o n�, retorna a altura -1
        return -1;
    else
        return raiz->altura;
}

int fb(No *raiz){ // calcula o fator de balanceamento
	//labs retorna o valor absoluto
    return labs(alturaNoh(raiz->esquerda)-alturaNoh(raiz->direita));
}

int alturaArvore(No *raiz){//verifica a altura da AVL
	int alturaDireita=0;
	int alturaEsquerda=0;

	if (raiz == NULL){	
		return -1;
	}

	alturaEsquerda = alturaArvore(raiz->esquerda);
	alturaDireita = alturaArvore(raiz->direita);

	if (alturaEsquerda > alturaDireita){
		return alturaEsquerda+1;
	}else{
		return alturaDireita+1;
	}
}

int contarNos(No *raiz){//contar quantos n�s tem na �rvore
	if (raiz == NULL){
		return 0;	
	}else{
		return contarNos(raiz->esquerda) + contarNos(raiz->direita) + 1; 
	}

}

int maior(int x, int y){//verifica maior valor
	if (x > y){
		return x;
	}else{
		return y;
	}
}

void rotacaoDireita(No **raiz){// �rvore pendendo � esquerda, gira pra direita

	No *nohAuxiliar;

	nohAuxiliar  = (*raiz)->esquerda;
	(*raiz)->esquerda  = nohAuxiliar->direita;
	nohAuxiliar->direita = *raiz;

	//atualiza altura do n�
	(*raiz)->altura = maior(alturaNoh((*raiz)->esquerda), alturaNoh((*raiz)->direita))+1;
	nohAuxiliar->altura = maior (alturaNoh(nohAuxiliar->esquerda), (*raiz)->altura)+1;

	*raiz = nohAuxiliar;
}

void rotacaoEsquerda(No **raiz){// �rvore pendendo � direita, gira pra esquerda
	No *nohAuxiliar;

	nohAuxiliar  = (*raiz)->direita;
	(*raiz)->direita  = nohAuxiliar->esquerda;
	nohAuxiliar->esquerda = *raiz;

	//atualiza altura do n�
	(*raiz)->altura = maior(alturaNoh((*raiz)->esquerda), alturaNoh((*raiz)->direita))+1;
	nohAuxiliar->altura = maior (alturaNoh(nohAuxiliar->esquerda), (*raiz)->altura)+1;

	*raiz = nohAuxiliar;
}

void rotacaoEsquerdaDireita(No **raiz){//Rota��o dupla a direita
	rotacaoEsquerda(&(*raiz)->esquerda);
	rotacaoDireita(raiz);
}


void rotacaoDireitaEsquerda(No **raiz){//Rota��o dupla a esquerda
	rotacaoDireita(&(*raiz)->direita);
	rotacaoEsquerda(raiz);
}

No* procuraMenor(No **raiz) {//procura pelo n� mais a esquerda
    No *nohAuxiliar1;
    No *nohAuxiliar2;

    nohAuxiliar1 = (*raiz);
    nohAuxiliar2 = (*raiz)->esquerda;

    while(nohAuxiliar2!=NULL){
        nohAuxiliar1 = nohAuxiliar2;
        nohAuxiliar2 = nohAuxiliar2->esquerda;
    }
    return nohAuxiliar1;
}

int inserir(No **raiz, int valor){
    int controle;

	if (*raiz == NULL){
		No *nohAuxiliar = alocar();
		nohAuxiliar->item.valor = valor;
		nohAuxiliar->altura = 0;
		nohAuxiliar->esquerda  = NULL;
		nohAuxiliar->direita = NULL;
        *raiz = nohAuxiliar;
		return 1;
	}

	No *nohAtual;

	nohAtual = *raiz;

    if(valor < nohAtual->item.valor){
        if((controle=inserir(&(nohAtual->esquerda),valor))==1){
            if(fb(nohAtual)>=2){
                if(valor < (*raiz)->esquerda->item.valor)
                    rotacaoDireita(raiz);
                else
                    rotacaoEsquerdaDireita(raiz);
            }
        }
    }else{
        if(valor > nohAtual->item.valor){
            if ((controle=inserir(&(nohAtual->direita),valor))==1){
                if(fb(nohAtual)>=2){
                    if(valor > (*raiz)->direita->item.valor)
                        rotacaoEsquerda(raiz);
                    else
                        rotacaoDireitaEsquerda(raiz);
                }
            }
        }else{
            printf("\nValor Duplicado!\n");
            return 0;
        }
    }

    // recalcular a altura do n�
    nohAtual->altura = maior(alturaNoh(nohAtual->esquerda),alturaNoh(nohAtual->direita))+1; 

    return controle;

}

int remover(No **raiz,int valor){//Remover um valor da AVL
    int controle;

    if(*raiz==NULL){
        printf("\nEste valor n�o existe\n");
        return 0;
    }
    if(valor < (*raiz)->item.valor){
        if((controle=remover(&((*raiz)->esquerda),valor))==1){
            if(fb(*raiz)>=2){
                if(alturaNoh((*raiz)->direita->esquerda)<=alturaNoh((*raiz)->direita->direita))
                    rotacaoEsquerda(raiz);
                else
                    rotacaoEsquerdaDireita(raiz);
            }
        }
    }
    if(valor>(*raiz)->item.valor){
        if ((controle=remover(&((*raiz)->direita),valor))==1){

            if(fb(*raiz)>=2){
                if(alturaNoh((*raiz)->esquerda->direita)<=alturaNoh((*raiz)->esquerda->esquerda))
                    rotacaoDireita(raiz);
                else
                    rotacaoDireitaEsquerda(raiz);
            }
        }
    }
    if((*raiz)->item.valor==valor){
        if(((*raiz)->esquerda==NULL)||((*raiz)->direita==NULL)){ //Pai tem um filho ou nenhum filho
            No *nohAuxiliar = *raiz;

            if((*raiz)->esquerda!=NULL)
                *raiz=(*raiz)->esquerda;
            else
                *raiz=(*raiz)->direita;
            free(nohAuxiliar);
            nohAuxiliar = NULL;
            printf("\nNo removido!\n");
            
        }else{
        	//o pai tem dois filhos
        	//Substituir pelo n� mais a esquerda da sub�rvore da direita
            No *nohAuxiliar = procuraMenor(&(*raiz)->direita);
            (*raiz)->item.valor = nohAuxiliar->item.valor;
            remover(&(*raiz)->direita,(*raiz)->item.valor);
            if(fb(*raiz)>=2){
                if(alturaNoh((*raiz)->esquerda->direita)<=alturaNoh((*raiz)->esquerda->esquerda))
                    rotacaoDireita(raiz);
                else
                    rotacaoEsquerdaDireita(raiz);
            }
        }
        return 1;
    }
    return controle;
}

int main(){
	No *raiz = iniciar();
	int op;
	int v=0;

	do{//fazer a impress�o da arvore a cada ciclo de execu��o
		imprimir(raiz, tam);

		printf("|---------------MENU---------------|\n");
		printf("|----------------AVL---------------|\n");
		printf("|------------0: Sair---------------|\n");
		printf("|------------1: Inserir------------|\n");
		printf("|------------2: Remover------------|\n");
		printf("|------------3: Mostra-------------|\n");
		printf("|----------------------------------|\n");
		printf("|-------Informe a sua opcao: ------|\n");
		scanf("%d", &op);

		switch(op){
			case 0:
				printf("\nPrograma AVL encerrado!\n");
				break;
			case 1:
				printf("\n------INSERINDO------\n");
				printf("Digite um valor\n");
				scanf("%d",&v);
				inserir(&raiz, v);
				break;
			case 2:
				printf("\n------REMOVENDO------\n");
				printf("Digite um valor\n");
				scanf("%d",&v);
				remover(&raiz, v);
				break;
			case 3:
				printf("\n------IMPRIMINDO------\n");
				imprimir(raiz, tam);
				break;
			default:
				printf("\nOpcao invalida!!!\n");
				break;
		}
	}while(op!=0);

	apagarArvore(raiz);

	return 0;
}
