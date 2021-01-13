#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifdef DEBUG
    #define debug if (1)
#else
    #define debug if (0)
#endif

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread 1 */
void *tarefaA (void *t) {
  debug printf(ANSI_COLOR_GREEN "[Thread A] Início\n" ANSI_COLOR_RESET);

  printf("Tudo bem?\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  if (x==2) {
      pthread_cond_signal(&x_cond);
      debug printf(ANSI_COLOR_GREEN "[Thread A] Liberou\n" ANSI_COLOR_RESET);
  }
  pthread_mutex_unlock(&x_mutex);
  debug printf(ANSI_COLOR_GREEN "[Thread A] Finalizada\n" ANSI_COLOR_RESET);
  pthread_exit(NULL);
}

/* Thread 2 */
void *tarefaB (void *t) {
  debug printf(ANSI_COLOR_GREEN "[Thread B] Início\n" ANSI_COLOR_RESET);

  printf("Bom dia!\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  if (x==2) {
      pthread_cond_signal(&x_cond);
      debug printf(ANSI_COLOR_GREEN "[Thread B] Liberou\n" ANSI_COLOR_RESET);
  }
  pthread_mutex_unlock(&x_mutex);
  debug printf(ANSI_COLOR_GREEN "[Thread B] Finalizada\n" ANSI_COLOR_RESET);
  pthread_exit(NULL);
}

/* Thread 3 */
void *tarefaC (void *t) {
  debug printf(ANSI_COLOR_GREEN "[Thread C] Início\n" ANSI_COLOR_RESET);

  pthread_mutex_lock(&x_mutex);
  if (x < 2) { 
     debug printf(ANSI_COLOR_GREEN "[Thread C] Bloqueada\n" ANSI_COLOR_RESET);
     pthread_cond_wait(&x_cond, &x_mutex);
     pthread_cond_signal(&x_cond);
     debug printf(ANSI_COLOR_GREEN "[Thread C] Liberada\n" ANSI_COLOR_RESET);
  }
  printf("até mais!\n");
  pthread_mutex_unlock(&x_mutex);
  debug printf(ANSI_COLOR_GREEN "[Thread C] Finalizada\n" ANSI_COLOR_RESET);
  pthread_exit(NULL);
}

/* Thread 4 */
void *tarefaD (void *t) {
  debug printf(ANSI_COLOR_GREEN "[Thread D] Início\n" ANSI_COLOR_RESET);

  pthread_mutex_lock(&x_mutex);
  if (x < 2) {
     debug printf(ANSI_COLOR_GREEN "[Thread D] Bloqueada\n" ANSI_COLOR_RESET);
     pthread_cond_wait(&x_cond, &x_mutex);
     pthread_cond_signal(&x_cond);
     debug printf(ANSI_COLOR_GREEN "[Thread D] Liberada\n" ANSI_COLOR_RESET);
  }
  printf("boa tarde\n");
  pthread_mutex_unlock(&x_mutex);
  debug printf(ANSI_COLOR_GREEN "[Thread D] Finalizada\n" ANSI_COLOR_RESET); 
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[3], NULL, tarefaA, NULL);
  pthread_create(&threads[2], NULL, tarefaB, NULL);
  pthread_create(&threads[1], NULL, tarefaC, NULL);
  pthread_create(&threads[0], NULL, tarefaD, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}

