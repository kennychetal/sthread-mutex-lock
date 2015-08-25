#define _REENTRANT
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "sthread.h"
#include "sync.h"


#define MAX_THREADS 1000
static sthread_mutex_t mutex;
static sthread_mutex_t *mut_ptr = &mutex;


int threadmain(void *arg)
{
    int tid = (int)arg;

    sthread_mutex_lock(mut_ptr,tid);

    sthread_mutex_lock(mut_ptr,tid);

    //I am intentionally putting a delay here to show that threads 2 and 3 become suspended.
    if(tid==1){
        sleep(10);
    }

    //critical region
    printf("Thread %d entered critical region\n",tid);

    sthread_mutex_unlock(mut_ptr,tid);

  return 0;
}

void Test1(){
    sthread_mutex_init(mut_ptr);
    sthread_t thr1,thr2,thr3;

    if (sthread_init() == -1){}

    printf("Creating Thread 1\n");

    if (sthread_create(&thr1, threadmain, (void *)1) == -1){}

    sleep(2);
    printf("Creating Thread 2\n");
    if (sthread_create(&thr2, threadmain, (void *)2) == -1){}

    sleep(2);
    printf("Creating Thread 3\n");
    if (sthread_create(&thr3, threadmain, (void *)3) == -1){}

}
int main(int argc, char *argv[])
{
    Test1();

    //add like 10 second delay to run all tests so main thread doesnt exit!
    while(1){sleep(10);break;}
    printf("END OF MAIN SCRIPT\n");
    sleep(1);


  return 0;
}
