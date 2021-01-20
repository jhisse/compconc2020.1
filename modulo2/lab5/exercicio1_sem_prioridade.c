#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifdef DEBUG
    #define debug if (1)
#else
    #define debug if (0)
#endif

/* Variaveis globais */
pthread_mutex_t x_mutex;
pthread_cond_t cond_read, cond_write;

int nthreads_writers;
int nthreads_readers;

int dim;
int *vetor;

int n_readers=0;
int n_writers=0; 


void init_reader(long int id) {
    debug printf(ANSI_COLOR_GREEN "[Thread %ld] Init\n" ANSI_COLOR_RESET, id);
    
    pthread_mutex_lock(&x_mutex);
    if (n_writers > 0) {
        debug printf(ANSI_COLOR_RED "[Thread %ld] Blocked\n" ANSI_COLOR_RESET, id);
        pthread_cond_wait(&cond_read, &x_mutex);
        debug printf(ANSI_COLOR_YELLOW "[Thread %ld] Released\n" ANSI_COLOR_RESET, id);
    }
    n_readers++;
    pthread_mutex_unlock(&x_mutex);
}

void close_reader(long int id) {
    pthread_mutex_lock(&x_mutex);
    debug printf(ANSI_COLOR_BLUE "[Thread %ld] Closed\n" ANSI_COLOR_RESET, id);
    n_readers--;
    if (n_readers==0) pthread_cond_signal(&cond_write);
    pthread_mutex_unlock(&x_mutex);
}

void init_writer(long int id) {
    debug printf(ANSI_COLOR_GREEN "[Thread %ld] Init\n" ANSI_COLOR_RESET, id);
    
    pthread_mutex_lock(&x_mutex);
    if((n_readers>0) || (n_writers>0)) {
        debug printf(ANSI_COLOR_RED "[Thread %ld] Blocked\n" ANSI_COLOR_RESET, id);
        pthread_cond_wait(&cond_write, &x_mutex);
        debug printf(ANSI_COLOR_YELLOW "[Thread %ld] Released\n" ANSI_COLOR_RESET, id);
    }
    n_writers++;
    pthread_mutex_unlock(&x_mutex);
}

void close_writer(long int id) {
    pthread_mutex_lock(&x_mutex);
    debug printf(ANSI_COLOR_BLUE "[Thread %ld] Closed\n" ANSI_COLOR_RESET, id);
    
    n_writers--;
    pthread_cond_signal(&cond_write);
    pthread_cond_broadcast(&cond_read);
    pthread_mutex_unlock(&x_mutex);
}

void * reader (void * arg) {
    long int id = (long int) arg;
    
    init_reader(id);

    debug printf(ANSI_COLOR_CYAN "[Thread %ld] Read\n" ANSI_COLOR_RESET, id);

    double media = 0;
    for (int i=0; i<dim; i++){
        media += vetor[i];
    }
    media = media/(double) dim;
    printf("Media %.2f \n", media);

    close_reader(id);
    
    pthread_exit(NULL);
}

void * writer(void * arg) {
    long int id = (long int) arg;
    init_writer(id);

    debug printf(ANSI_COLOR_CYAN "[Thread %ld] Write\n" ANSI_COLOR_RESET, id);    

    for (int i=0; i < dim; i++){
        if (id == 0 || id == dim)
            vetor[i] = id;
        else
            vetor[i] = id*2;
    }

    close_writer(id);
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Digite: %s <dimensao do vetor> <numero de threads leitoras> <numero de threads escritoras>\n", argv[0]);
        return 1;  
    }

    dim = atoi(argv[1]);
    nthreads_readers = atoi(argv[2]);
    nthreads_writers = atoi(argv[3]);

    pthread_t *tid;

    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&cond_read, NULL);
    pthread_cond_init(&cond_write, NULL);

    tid = (pthread_t *) malloc(sizeof(pthread_t)*(nthreads_writers+nthreads_readers));
    if(tid==NULL) {puts("Error malloc"); return 2;}

    /* Inicializa vetor */
    vetor = malloc(sizeof(int)*dim);
    if(!vetor){fprintf(stderr, "Error | Malloc\n");exit(1);}
    for (int i=0; i<dim; i++) vetor[i] = i;

    /* Inicializa threads readers */
    for(long int i=0; i<nthreads_readers; i++) {
        if(pthread_create(tid+i, NULL, reader, (void *) i)) {
            fprintf(stderr, "Error | Pthread\n");
            exit(1);
        }
    }

    /* Inicializa threads writers */
    for(long int i=nthreads_readers; i<(nthreads_writers+nthreads_readers); i++) {
        if(pthread_create(tid+i, NULL, writer, (void *) i)) {
            fprintf(stderr, "Error | Pthread\n");
            exit(1);
        } 
    }

    /* Aguarda término de todas as threads */
    for(long int i=0; i<(nthreads_writers+nthreads_readers); i++) {
        if(pthread_join(*(tid+i), NULL)) {
            fprintf(stderr, "ERROR |  Pthread\n");
            return 3;
        }
    }

    return 0;
}
