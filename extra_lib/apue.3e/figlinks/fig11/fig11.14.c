#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "apue.h"

struct job {
    struct job *j_next;
    struct job *j_prev;
    pthread_t   j_id;   /* tells which thread handles this job */
    /* ... more stuff here ... */
};

struct queue {
    struct job      *q_head;
    struct job      *q_tail;
    pthread_rwlock_t q_lock;
};

/*
 * Initialize a queue.
 */
int
queue_init(struct queue *qp)
{
    int err;

    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init(&qp->q_lock, NULL);
    if (err != 0)
    	return(err);
    /* ... continue initialization ... */
    return(0);
}

/*
 * Insert a job at the head of the queue.
 */
void
job_insert(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if (qp->q_head != NULL)
    	qp->q_head->j_prev = jp;
    else
    	qp->q_tail = jp;	/* list was empty */
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Append a job on the tail of the queue.
 */
void
job_append(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = NULL;
    jp->j_prev = qp->q_tail;
    if (qp->q_tail != NULL)
    	qp->q_tail->j_next = jp;
    else
    	qp->q_head = jp;	/* list was empty */
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Remove the given job from a queue.
 */
void
job_remove(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head) {
    	qp->q_head = jp->j_next;
    	if (qp->q_tail == jp)
    		qp->q_tail = NULL;
    	else
    		jp->j_next->j_prev = jp->j_prev;
    } else if (jp == qp->q_tail) {
    	qp->q_tail = jp->j_prev;
    	jp->j_prev->j_next = jp->j_next;
    } else {
    	jp->j_prev->j_next = jp->j_next;
    	jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Find a job for the given thread ID.
 */
struct job *
job_find(struct queue *qp, pthread_t id)
{
    struct job *jp = NULL;

    if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
    	return(NULL);
    
    //for debug the queue can check other thread create struct job.
    //it also prove that thread'stack/heap all in same area.
    for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
        printf("in find thread ID %lu \n", jp->j_id);

    for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
        if (pthread_equal(jp->j_id, id))//check for two thread is equal or not
            break;

    pthread_rwlock_unlock(&qp->q_lock);
    return(jp);
}

int 
job_destory(struct queue *qp)
{
    //check all job'thread have been done
    //delete the rwlock
    //delete this queue
    return 0;
}

void *
thr_fn1(void *arg)
{
    printf("thread ID : %lu, start\n", (unsigned long)pthread_self());

    struct queue *qp = arg;
    struct job *tjob;
    
    //in this way: ohter thread access it by its'address, but every thread have its stack only.
    /*struct job job;
    job.j_id = pthread_self();
    job_insert(qp, &job);*/

    //in this way: ohter thread access it also by its'address, but all job in the process'heap area.
    struct job *job = malloc(sizeof(struct job));
    job->j_id = pthread_self();
    job_insert(qp, job);
    
    for(int i=0; i< 2; i++){
        usleep(rand()%1333);
        tjob = job_find(qp, pthread_self());
        if(NULL != tjob)
            printf("thread ID : %lu\n", (unsigned long)tjob->j_id);
    }
    //while(1);
    pthread_detach(pthread_self());
    printf("thread ID : %lu, stop\n", (unsigned long)pthread_self());
    job_remove(qp, tjob);
    pthread_exit((void *)pthread_self());
}


int
main(int argc, char *argv[])
{
    int			ret;
    pthread_t	tid1;
    int		*tret;

    srand(time(NULL));
    struct queue *qp = malloc(sizeof(struct queue));
    ret = queue_init(qp);

    for(int i=0; i<2; i++){
        ret = pthread_create(&tid1, NULL, thr_fn1, qp);
        if (ret != 0)
        	err_exit(ret, "can't create thread 1");
    }

    sleep(1);
    //while(1);
    /* wait for two thread run, and you can think if not used pthread_mutex_lock */
    
    /*ret = pthread_join(tid1, (void *)&tret);
    if (ret != 0)
    	err_exit(ret, "can't join with thread 1");
    else
        printf("thread 1 exit code : %lu\n", (long)tret);*/

    ret = job_destory(qp);
    if(0 != ret)
        printf("job_destory err\n");

    exit(0);
}
