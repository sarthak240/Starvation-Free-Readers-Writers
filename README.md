# <p align="center"> Starvation Free Readers-Writers </p>
<p align="center"> *Sarthak Sachdeva (17116065) | ECE(Minors)* </p>

## Introduction
The implementation of a starvation free readers-writers is given in this github repository. 

***Problem Defination:*** Any number of readers can read from the shared resource (a variable in this case) simoultaneously. But, if one or more reader is reading, then writer can't write anything to this resource. Also, if there is no reader reading, then only 1 writer can write to the shared resource. We have considered 5 readers and 3 writers here.

***Language Used:*** C++

## Variables Defined
***Integer num_readers:*** Number of readers currently reading from the shared variable

***Integer val:*** The shared variable

***Semaphore reader_critical:*** Used so taht multiple readers dont go into the 'if(num_readers==1)' statement

***Semaphore cant_write:*** If there are 1 or more readers, then we can't write. So when first reader comes, it locks this semaphore

***Semaphore remove_starvation:*** Used to remove starvation. If a reader comes after a writer and writer is currently 'locked' by 'cant_write', then it locks 'remove_starvation' so that the next reader can't start executing and the writer gets an opportunity to execute

## Implementation of Reader
```
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
```

## Implementation of Writer
```
void *writer(void *x){

    sem_wait(&remove_starvation);  
    sem_wait(&cant_write); 

    val += 20;
    cout << "Writer " << x << " is writing " << val << endl;

    sem_post(&cant_write); 
    sem_post(&remove_starvation);  
}
```

## Output Considering 5 Readers and 3 Writers
```
Reader 0 is reading 10
Writer 0 is writing 30
Reader 0x1 is reading 30
Writer 0x1 is writing 50
Reader 0x2 is reading 50
Writer 0x2 is writing 70
Reader 0x4 is reading 70
Reader 0x3 is reading 70
```