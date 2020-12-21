/* Cálculo do número pi utilizando a Fórmula de Leibniz para Pi */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include"timer.h"

#define POTENCIA(a) (a&1?-1:1)

long long int dim; /* Dimensao da serie */
int nthreads; /* Numero de threads */
double *vetor; /* Valores da série  */
double *retorno;
pthread_t *tid;

/* calculo do elemento na posicao */
double calcula_elemento(long long int posicao){
    int potencia = POTENCIA(posicao);
    double resultado = (double) (potencia)/(2*posicao+1);
    return resultado;
}

void* tarefa(void * arg) {
    long int id = (long int) arg; /* Identificador da thread */
    
    double *soma_local;
    soma_local = (double*) malloc(sizeof(double));
    if(soma_local == NULL) {
        fprintf(stderr, "Erro - malloc\n");
        exit(1);
    }
    *soma_local = 0;
    
    for(long long int i=id; i<dim; i+=nthreads)
/*    for(long long int i=dim-id; i>=0; i-=nthreads)*/
        *soma_local += vetor[i];
    
    pthread_exit((void *) soma_local);
}

/* fluxo principal */
int main(int argc, char *argv[]) {
    
    double start, stop, elapsed;

    double soma_seq = 0;
    double soma_conc = 0;

    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensao da serie> <numero de threads>\n", argv[0]);
        return 1;
    }
    
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    
    vetor = (double*) malloc(sizeof(double)*dim);
    if(vetor == NULL){
        fprintf(stderr, "Erro - malloc\n");
        return 2;
    }
    
    /* Inicializacao das estruturas */
    GET_TIME(start);
    for(long long int i =0; i<dim; i++)
        vetor[i] = calcula_elemento(i);
    GET_TIME(stop);
    elapsed = stop - start;
    printf("Tempo para inicialização das estruturas: %lf\n", elapsed);
    
    /* Calculo sequencial */
    GET_TIME(start);
    for(long long int i=0; i<dim; i++){
        soma_seq += vetor[i];
    }
    soma_seq = 4*soma_seq;
    GET_TIME(stop);
    elapsed = stop - start;
    printf("Tempo para calculo sequencial: %lf\n", elapsed);
   
    /* Calculo concorrente */
    GET_TIME(start);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
        fprintf(stderr, "Erro - malloc\n");
        return 2;
    }

    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void *) i)) {
            fprintf(stderr, "ERRO - pthread_create\n");
            return 3;
        }        
    }
    
    for(long int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void **) &retorno)) {    
            fprintf(stderr, "ERRO -pthread_create\n");
            return 3;
        }
        soma_conc += *retorno;
        free(retorno);
    }
    soma_conc = 4*soma_conc;
    GET_TIME(stop);
    elapsed = stop - start;
    printf("Tempo para calculo concorrente: %lf\n", elapsed);

    /* exibir os resultado */
    printf("Soma sequencial:    %.15f\n", soma_seq);
    printf("Soma concorrente:   %.15f\n", soma_conc);
    printf("Valor de pi (M_PI): %.15f\n", M_PI);

    /* Libera memoria */
    free(vetor);
    free(tid);

    return 0;
}
