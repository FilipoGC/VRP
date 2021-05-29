#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

//!!!!!!COMPILE COM FLAG -lm E COMPILADOR G++, E EXECUTE COM ./nomeExecutavel < instancia.txt !!!!

using namespace std;

struct vrp{

	float** dist; //matriz dos custos das distancias
	float cost;

	vector<int>* solution; //vector de solucoes
	int* y;				   //vetor das coordenadas x
	int* x;				   //vetor das coordenadas y
	int* demanda;		   //vetor da demanda de cada "local"
	int* visited;
	
	int capacity;
	int lenght; 

};

typedef struct vrp Vrp;

//Aloca VRP
Vrp *alocaVrp(int tamGrafo, int capacidade){

	int i;
	Vrp* p = (Vrp*) malloc (sizeof(Vrp));
	p->capacity = capacidade;
	p->lenght = tamGrafo + 1;
	p->solution = new vector<int> ();
	
	p->dist = (float**) malloc (sizeof(float*) * (p->lenght) );
	for (i = 0; i < p->lenght ; i++){
		p->dist[i] = (float*) malloc(sizeof (float) * (p->lenght));
	}
	p->x = (int*) malloc (sizeof(int) * (p->lenght) );
	p->y = (int*) malloc (sizeof(int) * (p->lenght) );
	p->demanda = (int*) malloc (sizeof(int) * (p->lenght) );
	p->visited = (int*) malloc (sizeof(int) * (p->lenght) );
	
	for(i = 1; i < p->lenght; i++) p->visited[i] = 0;

	p->cost = 0;

	return p;

}

void vizinhoMaisProximo(Vrp *p){

	int i, numVisited;
	int indiceInicial = 0; 
	
	p->solution->push_back(indiceInicial); 
	p->visited[indiceInicial] = 1;

	int indiceMenor = -1;
	int capacidadeAtual = p->capacity;
	float menorValor;

	numVisited = 1;

	while(numVisited < p->lenght){

		menorValor = 99999999;
		for(i = 0; i < p->lenght; i++){

			if(p->dist[indiceInicial][i] != 0 
				&& p->dist[indiceInicial][i]  <  menorValor
				&& p->visited[i] == 0 && capacidadeAtual > p->demanda[i]){

					indiceMenor = i;
					menorValor = p->dist[indiceInicial][i];

			}

		}
		if(menorValor == 99999999) {
			p->solution->push_back(0);
			p->cost += p->dist[indiceInicial][0];
			indiceInicial = 0;
			capacidadeAtual = p->capacity;
			
		}else {
			p->solution->push_back(indiceMenor);
			p->visited[indiceMenor] = 1;
			p->cost += menorValor;
			numVisited++;
			indiceInicial = indiceMenor;
			capacidadeAtual -= p->demanda[indiceInicial];
		}
	}
	p->cost += p->dist[p->solution->at(p->solution->size() - 1)][0];
	p->solution->push_back(0);

}

//printa a solucao
void printSolution(Vrp *p){

	int i;

	for(i = 0; i < p->solution->size(); i++){
		printf("%d ", p->solution->at(i));
		if(p->solution->at(i) == 0 && i != 0 && i != p->solution->size()-1) {
			printf("\n0 ");
		}
	}
}

//Calcula a distancia euclidiana
float distEuclidiana(int x1, int y1, int x2, int y2){
	float result;
	x1 -= x2;
	y1 -= y2;
	x1 = x1 * x1;
	y1 = y1 * y1;
	result = x1 + y1;
	result = sqrt(result);
	return result;

}
//verifica se a solucao esta correta
int verificaSolucao(Vrp* p, float custoAtual){
	int capacidadeAtual = 0;
	float custo = 0;

	for(int i = 0; i < (p->solution->size() - 1); i++) {
		custo += p->dist[p->solution->at(i)][p->solution->at(i+1)];
		if(p->solution->at(i) == 0) capacidadeAtual = 0;
		capacidadeAtual += p->demanda[p->solution->at(i)];
		if(capacidadeAtual > p->capacity) {
			return 0;
		}
	}
	
	if(custo < p->cost) { 
		p->cost = custo;
		return 1;	
	}
	else if(custoAtual = -1) return 2;
	return 0;

}
//modifica a solucao utilizando trocas aleatorias e verifica se melhorou ou nao
void modifica(Vrp* p){
	int cont = 0;
	
	while(cont < 1000000) {
		cont++;
		int a = rand() % (p->solution->size() - 1);
		if(a <= 0)
			a =1;
		int b = rand() % (p->solution->size() - 1);
		if(b <= 0)
			b = 1;
		iter_swap((p->solution->begin() + a), (p->solution->begin() + b));
		if(verificaSolucao(p, p->cost) != 1) {
			iter_swap((p->solution->begin() + a), (p->solution->begin() + b));
		}
	}

}

int main(){

	int i, j, k, tamGrafo, capacity;
	Vrp *p;
	scanf("%d %d", &tamGrafo, &capacity);												//lendo tamanho do grafo e capacidade maxima do "caminhao"
	p = alocaVrp(tamGrafo, capacity);
	for(i = 0; i < p->lenght; i++) {													//for para pegar as coordenadas do x e y e suas demandas, ignorando o inicial
		if(i > 0)
			scanf("%d %d %d", &p->x[i], &p->y[i], &p->demanda[i]); 
		else
			scanf("%d %d", &p->x[i], &p->y[i]);
	}
	
	for(i = 0; i < p->lenght; i++) {
		for(j = 0; j < p->lenght; j++) {
			if(i != j) {
				p->dist[i][j] = distEuclidiana(p->x[i], p->y[i], p->x[j], p->y[j]);		//registrando custos de distnacia entre "locais"
			
			}
		}
	}

	vizinhoMaisProximo(p);
	
	printf("Solucao Inicial: \n");	
	printSolution(p);
	
	printf("\nCusto: %f\n\n", p->cost);
	modifica(p);
	printf("Solucao Melhorada: \n");
	printSolution(p);
	printf("\nCusto apos melhoria: %f\n", p->cost);
	
}
