/*
 * NAME, etc.
 *
 * sync.c
 *
 * Synchronization routines for SThread
 */
#define MAX_THREADS 1000
#define _REENTRANT

#include "sthread.h"

/*
 * Mutex routines
 */

int sthread_mutex_init(sthread_mutex_t *mutex)
{
    mutex->state = 1;
    mutex->lock = 0;
    mutex->counter = 0;
    mutex->tid = 0;
    mutex->wcount = 0;
    mutex->front = 0;
    mutex->back = MAX_THREADS-1;

    return 1;
}

int sthread_mutex_destroy(sthread_mutex_t *mutex,int tid)
{
    /*
        Did not need to dynamically allocate memory so don't really need this function
    */

    return 1;
}

int sthread_mutex_lock(sthread_mutex_t *mutex,int tid)
{
    int ret_val;
    if(mutex->lock==1){
        if(mutex->tid==tid){
            printf("Incrementing counter on lock\n");
            mutex->counter = mutex->counter + 1;
        }

        else if(mutex->tid!=tid){
            enqueueWait(mutex);
            printf("WAIT COUNTER: %d\n",mutex->wcount);
            printf("Suspending thread  %d because mutex is not available\n", tid);
            sthread_suspend();
            //After waking up from suspend assign it the lock and tid. Lock is available at this point.
            printf("Thread %d obtaining lock after waking up\n", tid);
            mutex->tid = tid;
            mutex->lock = 1;
            ret_val = -1;
        }
    }
    else if (mutex->lock==0){
            printf("Thread %d has obtained a mutex\n",tid);
            mutex->lock = 1;
            mutex->tid = tid;
            ret_val = 0;
        }
    return ret_val;
    }




//doesnt allow blocking
int sthread_mutex_trylock(sthread_mutex_t *mutex,int tid)
{
    int ret_val;
    //if return val == 1 then suspend thread
    if(mutex->lock==0){
        printf("Thread %d has obtained a mutex\n",tid);
        mutex->lock = 1;
        mutex->tid = tid;
        ret_val = 0;
    }
    else if(mutex->lock==1){
        if(mutex->tid==tid){
            mutex->counter = mutex->counter + 1;
        }
        else if(mutex->tid!=tid){
            ret_val = -1;
        }
    }

    return ret_val;

}

int sthread_mutex_unlock(sthread_mutex_t *mutex,int tid)
{
    //enable if you want to test other thread unable to get lock...
   // sleep(1);
    int ret_val;

    if(mutex->lock==1){
        if(mutex->tid==tid){
            //check counter
            if(mutex->counter==0){
                mutex->lock = 0;
                ret_val = 0;

                if(isWaitingEmpty(mutex)){
                    printf("No threads waiting\n");
                }
                else{
                    //wake up the mutex in waiting that is in front
                    sthread_wake(mutex->waiting[mutex->front]);
                    //dequeue the mutex

                    dequeueWait(mutex);
                }
            }
            else if(mutex->counter>0){
                mutex->counter = mutex->counter - 1;
                printf("Thread %d, making recursive call to unlock lock\n",tid);

                sthread_mutex_unlock(mutex,tid);
            }
        }
        mutex->lock=0;
        ret_val = 0;

    }
    else{
        ret_val = -1;
    }

    return ret_val;

}

int isWaitingEmpty(sthread_mutex_t *mutex){
    if(mutex->wcount==0){
        return 1;
    }
    else{
        return 0;
    }
}

int enqueueWait(sthread_mutex_t *mutex){
    if(mutex->wcount == MAX_THREADS){
        printf("Queue is full when callling enqueueWait()\n");
        return -1;
    }
    else{
        mutex->back = (mutex->back+1) % MAX_THREADS;
        mutex->waiting[mutex->back] = sthread_self();
        mutex->wcount = mutex->wcount + 1;
        return 0;
    }

}

int dequeueWait(sthread_mutex_t *mutex){
    if(isWaitingEmpty(mutex)){
        printf("Waiting queue is empty\n");
    }
    else{
        mutex->front = (mutex->front+1) % MAX_THREADS;
        mutex->wcount = mutex->wcount - 1;
    }
}

int getMutexFront(sthread_mutex_t *mutex){
    if(isWaitingEmpty(mutex)){
        printf("Waiting queue is empty\n");
    }
    else{
        printf("Not implemented yet :)\n");
    }
}




