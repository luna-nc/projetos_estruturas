#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cctype>
using namespace std;
// Inicializar, vazio, inser??o, remo??o e impress?o
struct Elemento{
	char Operador[10]; 
	float Numero;
	Elemento *Prox;
};
//	FIFO -> First In, First Out
struct Fila{
	Elemento *inicio, *fim;
	int total;
};
// LIFO -> Last In, First Out
struct Pilha{
	Elemento *topo;
	int total;
};

void ini_fila(Fila *F){
	F->inicio = NULL;
	F->fim = NULL;
	F->total = 0;
}

void ini_pilha(Pilha *P){
	P->topo = NULL;
	P->total = 0;
}

bool Fila_vazia(Fila *F){
	return (F->inicio == NULL);
}

bool Pilha_vazia(Pilha *P){
	return (P->topo == NULL);
}
Elemento * Criar_Elemento(const char* Op, float Num){
	Elemento *Ret;
	Ret = (Elemento *) malloc(sizeof(Elemento));	
	if (Ret !=NULL){
		strcpy(Ret->Operador, Op);
		Ret->Numero = Num;
		Ret->Prox = NULL;
	}
	return Ret;
}

void Imprimir(Elemento *E){
	Elemento *Aux;
	cout << endl << "IMPRESSAO:\n\t";	
	for(Aux = E;Aux!=NULL;Aux = Aux->Prox){
		if (strcmp(Aux->Operador, "#") == 0){ //N?ero
			cout<<Aux->Numero;	
		} else{
			cout<<Aux->Operador;	
		}
		if(Aux->Prox != NULL){
			cout << " ";
		}
	}
	cout<<endl;
}
//FIFO - First In, First Out
//Enqueue (Queue = fila!!)
void Enfileirar(Fila *F, const char* Op, float Num){
	Elemento *ptE;
	ptE = Criar_Elemento(Op, Num);
	if (ptE !=NULL){
		if (Fila_vazia(F)){
			//Atualiza in?io e fim
			F->inicio = ptE;			
		} else {
			//Atualiza o fim
			F->fim->Prox = ptE;				
		}
		F->fim = ptE;
		++F->total;
	}	
}
//Dequeue
Elemento * Desenfileirar(Fila *F){
	Elemento *Ret = NULL;
	if (!Fila_vazia(F)){
		Ret = F->inicio;
		if (F->inicio == F->fim) {
			//1 elemento
			F->inicio = NULL;
			F->fim = F->inicio;
		} else {
			//Mais de 1 elemento
			F->inicio = F->inicio->Prox;
		}
		--F->total;			
	}
	return Ret;
}

//LIFO - Last In, First Out
//Push (Stack = pilha!!)
void Empilhar(Pilha *P, Elemento *E){
	if (E != NULL){
		if (!Pilha_vazia(P)){		
			//Atualiza o fim
			E->Prox = P->topo;	
		}	
		P->topo = E;		
		++P->total;
	}	
}

//Pop
Elemento * Desempilhar(Pilha *P){
	Elemento *Ret = NULL;
	if (!Pilha_vazia(P)){
		Ret = P->topo;
		P->topo = P->topo->Prox;		
		--P->total;			
	}
	return Ret;
}

Fila clonar_fila(Fila *F) {
    Fila F_clone;
    ini_fila(&F_clone);
    Elemento *aux = F->inicio;
    while (aux != NULL) {
        Enfileirar(&F_clone, aux->Operador, aux->Numero);
        aux = aux->Prox;
    }
    return F_clone;
}

void cadastrar_expressao(Fila *F){
	float num = 0;
	char exp;
	// Limpa a fila antiga se houver
	while(!Fila_vazia(F)){
		Elemento *rem = Desenfileirar(F);
		free(rem);
	}
	cin.ignore();
	cout << "Digite a expressao: ";
	while (cin >> noskipws >> exp && exp != '\n'){		
		if (isdigit(exp) && exp!=' '){
			cin.unget(); // Devolve o dígito para ser lido como número completo
			cin >> num;
			Enfileirar(F, "#", num);
		} else {
			Enfileirar(F, &exp, 0);
		}
	}
}

int checar_parenteses(Fila f){
	int error = 0, ret = 1;
	Elemento *elem;
	Fila FAux;
	ini_fila(&FAux);
	FAux = clonar_fila(&f);
	Pilha p;
	ini_pilha(&p);
	while (!Fila_vazia(&FAux) && error == 0){
		elem = Desenfileirar(&FAux);
		if (strcmp(elem->Operador, "(") == 0){
			Empilhar(&p, elem);
		} else if (strcmp(elem->Operador, ")") == 0){
			if (!Pilha_vazia(&p)){
				Desempilhar(&p);
			} else {
				error = 1;
			}
		}
	}
	if (p.total != 0){
		if (error != 0){
			ret = 0;
		}
		ret = 0;
	}
	return ret;
}

int Obter_precedencia(const char* operador) {  // Vinicius - Func.1
    int precedencia = 0;
    if (strcmp(operador, "(") == 0 || strcmp(operador, ")") == 0) {
        precedencia = 5;
    } else if (strcmp(operador, "*") == 0 || strcmp(operador, "/") == 0) {
        precedencia = 4;
    } else if (strcmp(operador, "+") == 0 || strcmp(operador, "-") == 0) {
        precedencia = 3;
    } else if (strcmp(operador, ">") == 0 || strcmp(operador, "<") == 0 || 
               strcmp(operador, ">=") == 0 || strcmp(operador, "<=") == 0) {
        precedencia = 2;
    } else if (strcmp(operador, "=") == 0 || strcmp(operador, "!=") == 0) {
        precedencia = 1;
    }
    return precedencia;
}

float Calcular_operacao(float num1, float num2, const char *op){ // Marcos - Func.2
	float resultado =0.0;
	if(strcmp(op, "+") == 0){
		resultado = num1 + num2;
	}else if(strcmp(op, "-") == 0){
		resultado = num1 - num2;	
	} else if(strcmp(op, "*") == 0){
		resultado = num1 * num2;
	} else if(strcmp(op, "/") == 0){
		if(num2 !=0){
			resultado = num1/num2;
		} else{
			cout << "erro, nao existe divisao por 0"<<endl;
		}	
	} else if(strcmp(op, ">") == 0){
		resultado = (num1 > num2) ? 1.0 : 0.0;	
	} else if(strcmp(op, "<") == 0){
		resultado = (num1 < num2) ? 1.0 : 0.0;
	} else if(strcmp(op, "=") == 0 || strcmp(op, "==") == 0){
		resultado = (num1 == num2) ? 1.0 : 0.0;
	} else if(strcmp(op, "!=") == 0){
		resultado = (num1 != num2) ? 1.0 : 0.0;
	} else if(strcmp(op, ">=") == 0){
		resultado = (num1 >= num2) ? 1.0 : 0.0;
	} else {
		resultado = (num1 <= num2) ? 1.0 : 0.0;	
	}
	return resultado;
}

void Resolver_pilhas(Pilha *pNum, Pilha *pOp) { // Gustavo - Func.3
    if ((!Pilha_vazia(pOp) && pNum->total >= 2)){
        Elemento *num2 = Desempilhar(pNum);
        Elemento *num1 = Desempilhar(pNum);
        Elemento *op = Desempilhar(pOp);
        //Integracao com a funcionalidade 2
        num1->Numero = Calcular_operacao(num1->Numero, num2->Numero, op->Operador);
        num1->Prox = NULL;
        Empilhar(pNum, num1);
        
        free(num2);
        free(op);
    }
}

float calcular_expressao(Fila *F) {
    float ret = 0.0;
    Pilha PNum, POp;
    Fila FAux;
	ini_fila(&FAux);
	FAux = clonar_fila(F);
    ini_pilha(&PNum);
    ini_pilha(&POp);
    
    while (!Fila_vazia(&FAux)) {
        Elemento *Elem = Desenfileirar(&FAux);
        Elem->Prox = NULL;
        if (strcmp(Elem->Operador, "#") == 0) {
            Empilhar(&PNum, Elem);
        } else if (strcmp(Elem->Operador, "(") == 0) {
            Empilhar(&POp, Elem);
        } else if (strcmp(Elem->Operador, ")") == 0) {
            while (!Pilha_vazia(&POp) && strcmp(POp.topo->Operador, "(") != 0) {
                Resolver_pilhas(&PNum, &POp);
            }
            if (!Pilha_vazia(&POp)) {
                Elemento *rem = Desempilhar(&POp);
                free(rem);
            }
            free(Elem);
        } else {
            while (!Pilha_vazia(&POp) && strcmp(POp.topo->Operador, "(") != 0 &&
                   Obter_precedencia(Elem->Operador) <= Obter_precedencia(POp.topo->Operador)) {
                Resolver_pilhas(&PNum, &POp);
            }
            Empilhar(&POp, Elem);
        }
    }
    
    while (!Pilha_vazia(&POp)) {
        Resolver_pilhas(&PNum, &POp);
    }
    
    if (!Pilha_vazia(&PNum)) {
        Elemento *resElem = Desempilhar(&PNum);
        ret = resElem->Numero;
        free(resElem);
    }
    
    return ret;
}

void menu(Fila F){
	int op = 0;
	do{
		cout << "\n\t--- Menu ---\n[1] - Cadastro de expressao\n[2] - Checagem de parenteses";
		cout << "\n[3] - Calculo do resultado considerando os parenteses\n[0] - Sair\nOpcao: ";
		cin >> op;
		cin.ignore();
		switch (op){
			case 0: 
				break;
			case 1: 
				cadastrar_expressao(&F);
				break;
			case 2: 
				cout << endl;
				if(Fila_vazia(&F)){
					cout << "Nao ha expressao para ser checada!!\n";
				} else {	
					cout << "- Verificacao: ";
					if (checar_parenteses(F)){
						cout << "tudo certo!\n";
					} else {
						cout << "sintaxe errada!\n";
					}
				}
				break;
			case 3: 
				cout << endl;
				if(Fila_vazia(&F)){
					cout << "Nao ha expressao para ser calculada!!\n";
				} else {
					cout << "- Resultado: " << calcular_expressao(&F) << endl;
				}
				break;
			default:
				cout << "\n- Opcao invalida!\n";
		}
	} while (op != 0);
}

int main(){
	setlocale(LC_ALL, "Portuguese");
	Fila F;
	ini_fila(&F);
	menu(F);
	return 0;
}
