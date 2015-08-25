/*
 * Kenny Chetal
 *
 * sync.h
 */
#define MAX_THREADS 1000
#ifndef _STHREAD_SYNC_H_
#define _STHREAD_SYNC_H_



struct sthread_mutex_struct {
  int state;
  int lock;
  volatile int counter;
  int tid; //curent thread id
  sthread_t *waiting[MAX_THREADS];
  //wait counter
  int wcount;
  //front of array
  int front;
  //back of array
  int back;


};

typedef struct sthread_mutex_struct sthread_mutex_t;

int sthread_mutex_init(sthread_mutex_t *mutex);
int sthread_mutex_destroy(sthread_mutex_t *mutex,int tid);
int sthread_mutex_lock(sthread_mutex_t *mutex,int tid);
int sthread_mutex_trylock(sthread_mutex_t *mutex,int tid);
int sthread_mutex_unlock(sthread_mutex_t *mutex,int tid);
int isWaitingEmpty(sthread_mutex_t *mutex);
int enqueueWait(sthread_mutex_t *mutex);
int dequeueWait(sthread_mutex_t *mutex);
int getWaitFront(sthread_mutex_t *mutex);

#endif
