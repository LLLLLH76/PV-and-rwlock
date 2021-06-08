#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#define T 6

char file[T] =  "12345";
char content[T]="ABCDE";
pthread_rwlock_t lock;

void *writer_function(void *arg){
    long int i = (long int)arg;
    pthread_rwlock_wrlock(&lock);
    printf("writer%ld\n", i);
    file[i]=content[i];
    sleep(1);
    pthread_rwlock_unlock(&lock);
    return NULL;
}

void *reader_function(void *arg){
    long int i = (long int)arg;
    pthread_rwlock_rdlock(&lock);
    //printf("reader%ld starts\n",i);
    printf("reader%ld: %s\n",i,file);
    //printf("reader%ld finishes\n",i);
    pthread_rwlock_unlock(&lock);
    return NULL;
}

int main(){
    printf("%s\n",file);
    pthread_rwlock_init(&lock, NULL);
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
    pthread_rwlock_destroy(&lock);
    printf("%s\n",file);
    return 0;
}