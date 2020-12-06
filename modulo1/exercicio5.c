#include<stdio.h>
#include<pthread.h>

#define NTHREADS 2 
#define TAMVETOR 19

int vetor[TAMVETOR] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 , 11, 12, 13, 14, 15, 16, 17, 18, 19 };

void * tarefa(void * arg) {
    int ident = * (int *) arg;
    int atual = ident * TAMVETOR/NTHREADS;
    int fim = TAMVETOR%(ident+1) ? (ident+1) * TAMVETOR/NTHREADS+1: (ident+1) * TAMVETOR/NTHREADS;
    
    for(atual; atual<fim; atual++)
        vetor[atual] = vetor[atual]+1;
    
    pthread_exit(NULL);
}

int main(void) {
    
    printf("Vetor inicial: ");
    for(int i=0; i<TAMVETOR; i++)
        printf("%d ", vetor[i]);

    pthread_t tid[NTHREADS];
    
    int ident[NTHREADS];

    for(int i=0; i<NTHREADS; i++) {
        ident[i] = i;
        if(pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i]))
            printf("Error -- pthread create\n");
    }
    
    for(int i=0; i<NTHREADS; i++) {
        if(pthread_join(tid[i], NULL))
            printf("Error -- pthread create\n");
    }

    printf("\nVetor final: ");
    for(int i=0; i<TAMVETOR; i++)
        printf("%d ", vetor[i]);

    printf("\n");
    return 0;
}

