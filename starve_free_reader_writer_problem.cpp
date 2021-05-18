#include <bits/stdc++.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

pthread_t r[5],w[3]; 

int num_readers = 0; // Number of readers currently
int val = 10; // Value updated on writing and printed on reading

sem_t reader_critical;  // Used so taht multiple readers dont go into the 'if(num_readers==1)' statement
sem_t cant_write;  // If there are 1 or more readers, then we can't write. So when first reader comes, it locks this semaphore
sem_t remove_starvation;  // Used to remove starvation. If a reader comes after a writer and writer is currently 'locked' by 'cant_write', then it locks 'remove_starvation' so that the next reader can't start executing and the writer gets an opportunity to execute

// Implementation of readers

void *reader(void *x){

	// Wont run if writer is running
    sem_wait(&remove_starvation); 

   	// Critical Section for the reader
    sem_wait(&reader_critical); 
    num_readers++;
    if(num_readers==1) 
    	sem_wait(&cant_write); 
    sem_post(&reader_critical); 

    sem_post(&remove_starvation);  

    cout << "Reader " << x << " is reading " << val << endl;
 
 	// Critical Section for the reader
    sem_wait(&reader_critical); 
    num_readers--;
    if(num_readers==0)
    	sem_post(&cant_write); 
    sem_post(&reader_critical); 

}
// Implementation of Writers

void *writer(void *x){

    sem_wait(&remove_starvation);  
    sem_wait(&cant_write); 

    val += 20;
    cout << "Writer " << x << " is writing " << val << endl;

    sem_post(&cant_write); 
    sem_post(&remove_starvation);  
}

int main(){

    // Semaphores initialised
    sem_init(&remove_starvation,0,1);
    sem_init(&cant_write,0,1);
    sem_init(&reader_critical,0,1);

    // Thread are created
    pthread_create(&r[0],NULL,reader,(void *)0);
    pthread_create(&w[0],NULL,writer,(void *)0);
    pthread_create(&r[1],NULL,reader,(void *)1);
    pthread_create(&w[1],NULL,writer,(void *)1);
    pthread_create(&r[2],NULL,reader,(void *)2);
    pthread_create(&r[3],NULL,reader,(void *)3);
    pthread_create(&w[2],NULL,writer,(void *)2);
    pthread_create(&r[4],NULL,reader,(void *)4);
    
    // All threads completed         
    pthread_join(r[0],NULL);
    pthread_join(w[0],NULL);
    pthread_join(r[1],NULL);
    pthread_join(w[1],NULL);
    pthread_join(r[2],NULL);
    pthread_join(r[3],NULL);
    pthread_join(r[2],NULL);
    pthread_join(r[4],NULL);
}