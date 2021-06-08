#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#define T 6

char file[T] =  "12345";
char content[T]="ABCDE";
sem_t write_block, mutex;
int readcount = 0;

void *writer_function(void *arg){
    long int i = (long int)arg;
    sem_wait(&write_block);
    printf("writer%ld\n", i);
    file[i]=content[i];
    sleep(1);
    sem_post(&write_block);
    return NULL;
}

void *reader_function(void *arg){
    long int i = (long int)arg;
    sem_wait(&mutex);
    readcount++;
    if(readcount==1){
        sem_wait(&write_block);
    }
    sem_post(&mutex);
    //printf("reader%ld starts\n",i);
    printf("reader%ld: %s\n",i,file);
    //printf("reader%ld finishes\n",i);
    sem_wait(&mutex);
    readcount--;
    if(readcount==0){
        sem_post(&write_block);
    }
    sem_post(&mutex);
    return NULL;
}


int main(){
    printf("%s\n",file);
    sem_init(&write_block, 0,  1);
    sem_init(&mutex, 0, 1);
    pthread_t writer[T];
    pthread_t reader[T];
    for(long int i=0;i<T-1;i++){
        pthread_create(&writer[i], NULL, writer_function, (void*) i);
        pthread_create(&reader[i], NULL, reader_function, (void*) i);
    }
    for(int i=0;i<T-1;i++){
        pthread_join(writer[i], NULL);
        pthread_join(reader[i], NULL);
    }
    sem_destroy(&write_block);
    sem_destroy(&mutex);
    printf("%s\n",file);
    return 0;
}