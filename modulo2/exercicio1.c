#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

float *mat1;
float *mat2;
float *saida;
int nthreads;

typedef struct{
    int id;
    int dim;
} tArgs;

void * tarefa(void *arg) {
    tArgs *args = (tArgs*) arg;
    for(int i=args->id; i< args->dim; i+=nthreads) 
        for(int j=0; j < args->dim; j++)
            for(int k=0; k < args->dim; k++)
                saida[i * (args->dim) + j] += mat1[i * (args->dim) + k] * mat2[k * (args->dim) + j];
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int dim;
    
    pthread_t *tid;
    tArgs *args;

    double start, finish, elapsed;

    if (argc < 3) {
        printf("Digite: %s <dimensao das matrizes> <numero de threads>\n", argv[0]);
        return 1;  
    }

    dim = atoi(argv[1]);

    nthreads = atoi(argv[2]);

    if(nthreads>dim) nthreads=dim;

    mat1 = (float *) malloc(sizeof(float) * dim * dim);
    if (mat1 == NULL) {printf("ERROR--malooc\n"); return 2;}

    mat2 = (float *) malloc(sizeof(float) * dim * dim);
    if (mat2 == NULL) {printf("ERROR--malooc\n"); return 2;}

    saida = (float *) malloc(sizeof(float) * dim * dim);
    if (saida == NULL) {printf("ERROR--malooc\n"); return 2;}
    
    GET_TIME(start);
    // inicializacao das estruturas
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            mat1[i*dim+j] = 1;
            mat2[i*dim+j] = 2;
            saida[i*dim+j] = 0;
        }
    }
    GET_TIME(finish);
    elapsed = finish-start;
    printf("Tempo de inicialização: %lf\n", elapsed);

    GET_TIME(start);
    //multiplicacao
    tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("Error malloc"); return 2;}
    args = (tArgs *) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("Error malloc"); return 2;}    

    for(int i=0; i<nthreads; i++){
        (args+i)->id = i;
        (args+i)->dim = dim;
        if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
            puts("ERROR-pthread_create");
            return 3;
        }
    }

    for(int i=0; i<nthreads; i++){
        pthread_join(*(tid+i), NULL);
    }

    GET_TIME(finish);

    elapsed = finish-start;
    printf("Tempo de multiplicação: %lf\n", elapsed);
    
/* Impressao das matrizes de entrada e saida
    puts("Matriz 1 de entrada:");
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++)
            printf("%.1f ", mat1[i*dim+j]);
        puts("");
    }
    
    puts("Matriz 2 de entrada:");
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++)
            printf("%.1f ", mat2[i*dim+j]);
        puts("");
    }

    puts("Matriz de saída:");
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++)
            printf("%.1f ", saida[i*dim+j]);
        puts("");
    }
*/

    GET_TIME(start);
    free(mat1);
    free(mat2);
    free(saida);
    free(args);
    free(tid);
    GET_TIME(finish);
    elapsed = finish-start;
    printf("Tempo de finalização: %lf\n", elapsed);

    return 0;
}
