//Carlos Henrique Hannas de Carvalho 11965988

#include <stdio.h>
#include <pthread.h>

#define N 5 //Define a quantidade de filosofos

//Definicao dos estados de filosofos e mutex
typedef enum { PENSAR, FOME, COMER } Estado;
Estado estados[N];
pthread_mutex_t mutex;
pthread_cond_t cond_filosofo[N];

pthread_mutex_t mutex_garcom;
pthread_cond_t cond_garcom;

//Definicao do filosofo a direita e a esaquerda do filosofo f
#define FILO_ESQ(f) ((f + N - 1) % N)
#define FILO_DIR(f) ((f + 1) % N)

//Executa o acesso ao recurso
void testar(int filosofo) {
    if (estados[filosofo] == FOME && estados[FILO_ESQ(filosofo)] != COMER && estados[FILO_DIR(filosofo)] != COMER) {
        estados[filosofo] = COMER;
        pthread_cond_signal(&cond_filosofo[filosofo]);
    }
}

//Funcao para filosofo que quer acessar ao recurso
void pegar_hashis(int filosofo) {
    pthread_mutex_lock(&mutex_garcom);
    while (estados[FILO_ESQ(filosofo)] == COMER || estados[FILO_DIR(filosofo)] == COMER) {
        pthread_cond_wait(&cond_garcom, &mutex_garcom);
    }
    pthread_mutex_unlock(&mutex_garcom);

    pthread_mutex_lock(&mutex);
    estados[filosofo] = FOME;
    testar(filosofo);
    while (estados[filosofo] != COMER) {
        pthread_cond_wait(&cond_filosofo[filosofo], &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

//Altera o estado do filosofo que terminou de acessar ao recurso
void devolver_hashis(int filosofo) {
    pthread_mutex_lock(&mutex);
    estados[filosofo] = PENSAR;
    testar(FILO_ESQ(filosofo));
    testar(FILO_DIR(filosofo));
    pthread_cond_broadcast(&cond_filosofo[FILO_ESQ(filosofo)]);
    pthread_cond_broadcast(&cond_filosofo[FILO_DIR(filosofo)]);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex_garcom);
    pthread_cond_signal(&cond_garcom);
    pthread_mutex_unlock(&mutex_garcom);
}

//Print da acao do filosofo. Pensar ou comer
void *filosofo(void *arg) {
    int id = *(int *)arg;
    while (1) {
        printf("Filosofo %d pensando\n", id);

        pegar_hashis(id);

        printf("Filosofo %d comendo\n", id);

        devolver_hashis(id);
    }
}

int main() {
    pthread_t filosofos[N];
    int ids[N];

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_garcom, NULL);
    pthread_cond_init(&cond_garcom, NULL);
    for (int i = 0; i < N; i++) {
        pthread_cond_init(&cond_filosofo[i], NULL);
    }

    //Associacao de uma thread a um filosofo
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, &ids[i]);
    }

    //Sincronizacao entre trheads
    for (int i = 0; i < N; i++) {
        pthread_join(filosofos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < N; i++) {
        pthread_cond_destroy(&cond_filosofo[i]);
    }

    return 0;
}
