#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "apue.h"

#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)

struct foo *fh[NHASH];

pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
	int             f_count;
	pthread_mutex_t f_lock;
	int             f_id;
	struct foo     *f_next; /* protected by hashlock */
	/* ... more stuff here ... */
};

struct foo *
foo_alloc(int id) /* allocate the object */
{
	struct foo	*fp;
	int			idx;

	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id = id;
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return(NULL);
		}
		idx = HASH(id);
		pthread_mutex_lock(&hashlock);
		fp->f_next = fh[idx];
		fh[idx] = fp;
		pthread_mutex_lock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);
		/* ... continue initialization ... */
		pthread_mutex_unlock(&fp->f_lock);
	}
	return(fp);
}

void
foo_hold(struct foo *fp) /* add a reference to the object */
{
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}

struct foo *
foo_find(int id) /* find an existing object */
{
	struct foo	*fp;

	pthread_mutex_lock(&hashlock);
	for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
		if (fp->f_id == id) {
			foo_hold(fp);
			break;
		}
	}
	pthread_mutex_unlock(&hashlock);
	return(fp);
}

void
foo_rele(struct foo *fp) /* release a reference to the object */
{
	struct foo	*tfp;
	int			idx;

	pthread_mutex_lock(&fp->f_lock);
	if (fp->f_count == 1) { /* last reference */
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_lock(&hashlock);
		pthread_mutex_lock(&fp->f_lock);
		/* need to recheck the condition */
		if (fp->f_count != 1) {
			fp->f_count--;
			pthread_mutex_unlock(&fp->f_lock);
			pthread_mutex_unlock(&hashlock);
			return;
		}
		/* remove from list */
		idx = HASH(fp->f_id);
		tfp = fh[idx];
		if (tfp == fp) {
			fh[idx] = fp->f_next;
		} else {
			while (tfp->f_next != fp)
				tfp = tfp->f_next;
			tfp->f_next = fp->f_next;
		}
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		fp->f_count--;
		pthread_mutex_unlock(&fp->f_lock);
	}
}


void *
thr_fn1(void *arg)
{
    printf("thread 1: ID is %lu\n", (unsigned long)pthread_self());
    
    struct foo * foo = arg;

    for(int i=0; i< 3; i++){
        usleep(500);
        foo_hold(foo);
        printf("thr_fn1, foo->f_count number: %d\n", foo->f_count);
        foo_rele(foo);
        printf("thr_fn1, foo->f_count number: %d\n", foo->f_count);
    }    
    //while(1);//add while want to check the pthread by pstree on bash 
    
    pthread_exit((void *)&foo);
}

void *
thr_fn2(void *arg)
{
    printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());

    struct foo * foo = arg;

    for(int i=0; i< 3; i++){
        usleep(300);
        foo_hold(foo);
        printf("thr_fn2, foo->f_count number: %d\n", foo->f_count);
        foo_rele(foo);
        printf("thr_fn2, foo->f_count number: %d\n", foo->f_count);
    }
    //while(1);
    
    pthread_exit((void *)0);
}


int
main(void)
{
    int			err;
    pthread_t	tid1, tid2;
    void		*tret;

    struct foo * foo = foo_alloc(1);
    if (NULL == foo)
    	err_exit((int)foo, "foo_alloc err ");

    err = pthread_create(&tid1, NULL, thr_fn1, foo);
    if (err != 0)
    	err_exit(err, "can't create thread 1");

    err = pthread_create(&tid2, NULL, thr_fn2, foo);
    if (err != 0)
    	err_exit(err, "can't create thread 2");

    sleep(1);
    /* wait for two thread run, and you can think if not used pthread_mutex_lock */
    
    err = pthread_join(tid1, (void *)&tret);
    if (err != 0)
    	err_exit(err, "can't join with thread 1");
    err = pthread_join(tid2, (void *)&tret);
    if (err != 0)
    	err_exit(err, "can't join with thread 2");

    foo_rele(foo);
    printf("parent, foo->f_count number: %d\n", foo->f_count);

    exit(0);
}

