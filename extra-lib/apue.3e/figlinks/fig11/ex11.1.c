#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

//#define checkResults printf
void
checkResults(char *string, int ret_err)
{
    printf("%s %d", string, ret_err);
}

/* For safe condition variable usage, must use a boolean predicate and  */
/* a mutex with the condition.                                          */
int                 conditionMet = 0;
pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;

#define NTHREADS    5

void *
threadfunc(void *parm)
{
  int           rc;

  rc = pthread_mutex_lock(&mutex);
  checkResults("pthread_mutex_lock()\n", rc);

  while (!conditionMet) {
    printf("Thread blocked\n");
    rc = pthread_cond_wait(&cond, &mutex);
    checkResults("pthread_cond_wait()\n", rc);
  }

  rc = pthread_mutex_unlock(&mutex);
  checkResults("pthread_mutex_lock()\n", rc);
  return NULL;
}

int main(int argc, char **argv)
{
  int                   rc=0;
  int                   i;
  pthread_t             threadid[NTHREADS];

  printf("Enter Testcase - %s\n", argv[0]);

  printf("Create %d threads\n", NTHREADS);
  for(i=0; i<NTHREADS; ++i) {
    rc = pthread_create(&threadid[i], NULL, threadfunc, NULL);
    checkResults("pthread_create()\n", rc);
  }

  sleep(2);  /* Sleep is not a very robust way to serialize threads */
  rc = pthread_mutex_lock(&mutex);
  checkResults("pthread_mutex_lock()\n", rc);

  /* The condition has occured. Set the flag and wake up any waiting threads */
  conditionMet = 1;
  printf("Wake up all waiting threads...\n");
  rc = pthread_cond_broadcast(&cond);
  checkResults("pthread_cond_broadcast()\n", rc);

  rc = pthread_mutex_unlock(&mutex);
  checkResults("pthread_mutex_unlock()\n", rc);

  printf("Wait for threads and cleanup\n");
  for (i=0; i<NTHREADS; ++i) {
    rc = pthread_join(threadid[i], NULL);
    checkResults("pthread_join()\n", rc);
  }
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);

  printf("Main completed\n");
  return 0;
}
