Tworzenie procesów i zarządzanie nimi w C
1. fork() Funkcja fork() tworzy nowy proces (kopię procesu macierzystego).


pid_t pid = fork();
if (pid == 0) {
    // Kod dla procesu potomnego 
} else if (pid > 0) {
    // Kod dla procesu macierzystego
} else {
    // Błąd
}
2. exec() Funkcje exec() uruchamiają nowy program zastępując aktualny proces.
execlp("/bin/ls", "ls", NULL); // Przykład użycia execlp

3. wait() Funkcja wait() czeka na zakończenie procesu potomnego.

int status;
pid_t pid = wait(&status);

Komunikacja między procesami

mkfifo() tworzy potok nazwany (FIFO).

#include <sys/types.h>
#include <sys/stat.h>

int result = mkfifo("/tmp/myfifo", 0666);


pipe() tworzy nienazwany potok.

int fd[2];
pipe(fd); // fd[0] - do odczytu, fd[1] - do zapisu


fflush() opróżnia bufor strumienia.

fflush(stdout);


pthread_create() tworzy nowy wątek.

#include <pthread.h>

void* thread_function(void* arg) {
    // Kod wątku
}

pthread_t thread;
pthread_create(&thread, NULL, thread_function, NULL);

pthread_join() czeka na zakończenie wątku.

pthread_join(thread, NULL);

pthread_mutex_t, pthread_mutex_lock, pthread_mutex_unlock
Muteksy służą do synchronizacji wątków.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&mutex);
// Sekcja krytyczna
pthread_mutex_unlock(&mutex);



msgget() tworzy kolejkę komunikatów lub uzyskuje dostęp do istniejącej.

#include <sys/ipc.h>
#include <sys/msg.h>
int msgid = msgget(key, 0666 | IPC_CREAT);



msgsnd() wysyła komunikat do kolejki komunikatów.

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};
struct msg_buffer message;
message.msg_type = 1;
strcpy(message.msg_text, "Hello");
msgsnd(msgid, &message, sizeof(message.msg_text), 0);


msgrcv() odbiera komunikat z kolejki komunikatów.
msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0);



Inicjalizacja semafora
#include <semaphore.h>
sem_t semaphore;
sem_init(&semaphore, 0, 1); // Drugi argument 0 - semafor współdzielony między wątki, a nie procesy; trzeci argument - początkowa wartość semafora

sem_wait() zmniejsza wartość semafora, blokując jeśli wartość jest zero.
sem_wait(&semaphore);
sem_post
sem_post() zwiększa wartość semafora.
sem_post(&semaphore);

Przykładowy program: Komunikacja między wątkami z użyciem muteksów i semaforów

Копировать код
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;
void* thread_function(void* arg) {
    sem_wait(&semaphore);
    pthread_mutex_lock(&mutex);
    // Sekcja krytyczna
    printf("Wątek %ld\n", (long)arg);
    pthread_mutex_unlock(&mutex);
    sem_post(&semaphore);
    return NULL;
}

int main() {
    pthread_t threads[5];
    sem_init(&semaphore, 0, 1); // Inicjalizacja semafora z wartością początkową 1
    for (long i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void*)i);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    sem_destroy(&semaphore);
    pthread_mutex_destroy(&mutex);
    return 0;
}
